/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 */

#ifdef LINUX_KERNEL

#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>

#elif defined LINUX_USERSPACE

#include <string.h>
#include <errno.h>

#endif

#include "tpm.h"
#include "tpmbuff.h"
#include "tis.h"
#include "crb.h"
#include "tpm_common.h"
#include "tpm1.h"
#include "tpm2.h"
#include "tpm2_constants.h"

static struct tpm tpm;

static void find_interface_and_family(struct tpm *t)
{
	struct tpm_interface_id intf_id;
	struct tpm_intf_capability intf_cap;

	/* Sort out whether if it is 1.2 */
	intf_cap.val = tpm_read32(TPM_INTF_CAPABILITY_0);
	if ((intf_cap.interface_version == TPM12_TIS_INTF_12)||
	    (intf_cap.interface_version == TPM12_TIS_INTF_13)) {
		t->family = TPM12;
		t->intf = TPM_TIS;
		return;
	}

	/* Assume that it is 2.0 and TIS */
	t->family = TPM20;
	t->intf = TPM_TIS;

	/* Check if the interface is CRB */
	intf_id.val = tpm_read32(TPM_INTERFACE_ID_0);
	if (intf_id.interface_type == TPM_CRB_INTF_ACTIVE)
		t->intf = TPM_CRB;
}

struct tpm *enable_tpm(void)
{
	struct tpm *t = &tpm;

	find_interface_and_family(t);

	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		if (!tis_init(t))
			goto err;
		break;
	case TPM_CRB:
		if (!crb_init(t))
			goto err;
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	/* TODO: ACPI TPM discovery */

	return t;

err:
	return NULL;
}

u8 tpm_request_locality(struct tpm *t, u8 l)
{
	u8 ret = TPM_NO_LOCALITY;

	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		ret = tis_request_locality(l);
		break;
	case TPM_CRB:
		ret = crb_request_locality(l);
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	if (ret < TPM_MAX_LOCALITY)
		t->buff = alloc_tpmbuff(t->intf, ret);

	return ret;
}

void tpm_relinquish_locality(struct tpm *t)
{
	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		tis_relinquish_locality();
		break;
	case TPM_CRB:
		crb_relinquish_locality();
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	free_tpmbuff(t->buff, t->intf);
}

#define MAX_TPM_EXTEND_SIZE 70 /* TPM2 SHA512 is the largest */
int tpm_extend_pcr(struct tpm *t, u32 pcr, u16 algo,
		u8 *digest)
{
	int ret = 0;

	if (t->buff == NULL) {
		ret = -EINVAL;
		goto out;
	}

	if (t->family == TPM12) {
		struct tpm_digest d;

		if (algo != TPM_ALG_SHA1) {
			ret = -EINVAL;
			goto out;
		}

		d.pcr = pcr;
		memcpy((void*)d.digest.sha1.digest,
                        digest, SHA1_DIGEST_SIZE);

		ret = tpm1_pcr_extend(t, &d);
	} else if (t->family == TPM20) {
		struct tpml_digest_values *d;
		u8 buf[MAX_TPM_EXTEND_SIZE];

		d = (struct tpml_digest_values *) buf;
		d->count = 1;
		switch (algo) {
		case TPM_ALG_SHA1:
			d->digests->alg = TPM_ALG_SHA1;
			memcpy(d->digests->digest, digest, SHA1_SIZE);
			break;
		case TPM_ALG_SHA256:
			d->digests->alg = TPM_ALG_SHA256;
			memcpy(d->digests->digest, digest, SHA256_SIZE);
			break;
		case TPM_ALG_SHA384:
			d->digests->alg = TPM_ALG_SHA384;
			memcpy(d->digests->digest, digest, SHA384_SIZE);
			break;
		case TPM_ALG_SHA512:
			d->digests->alg = TPM_ALG_SHA512;
			memcpy(d->digests->digest, digest, SHA512_SIZE);
			break;
		case TPM_ALG_SM3_256:
			d->digests->alg = TPM_ALG_SM3_256;
			memcpy(d->digests->digest, digest, SM3256_SIZE);
			break;
		default:
			ret = -EINVAL;
			goto out;
		}

		ret = tpm2_extend_pcr(t, pcr, d);
	} else {
		ret = -EINVAL;
	}
out:
	return ret;
}

void free_tpm(struct tpm *t)
{
	tpm_relinquish_locality(t);
}
