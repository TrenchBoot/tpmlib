/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 */

#include <tpm.h>
#include <tpmbuff.h>

#include "tpm_common.h"
#include "tpm2.h"
#include "tpm2_auth.h"
#include "tis.h"
#include "crb.h"

static int tpm2_alloc_cmd(struct tpmbuff *b, struct tpm2_cmd *c, u16 tag,
		u32 code)
{
	c->header = (struct tpm_header *)tpmb_reserve(b);
	if (!c->header)
		return -ENOMEM;

	c->header->tag = cpu_to_be16(tag);
	c->header->code = cpu_to_be32(code);

	return 0;
}

static u16 convert_digest_list(struct tpml_digest_values *digests)
{
	int i;
	u16 size = 0;
	struct tpmt_ha *h = digests->digests;

	for (i=0; i<digests->count; i++) {
		switch(h->alg) {
		case TPM_ALG_SHA1:
			h->alg = cpu_to_be16(h->alg);
			h = (struct tpmt_ha *)((u8 *)h + SHA1_SIZE);
			size += sizeof(u16) + SHA1_SIZE;
			break;
		case TPM_ALG_SHA256:
			h->alg = cpu_to_be16(h->alg);
			h = (struct tpmt_ha *)((u8 *)h + SHA256_SIZE);
			size += sizeof(u16) + SHA256_SIZE;
			break;
		case TPM_ALG_SHA384:
			h->alg = cpu_to_be16(h->alg);
			h = (struct tpmt_ha *)((u8 *)h + SHA384_SIZE);
			size += sizeof(u16) + SHA384_SIZE;
			break;
		case TPM_ALG_SHA512:
			h->alg = cpu_to_be16(h->alg);
			h = (struct tpmt_ha *)((u8 *)h + SHA512_SIZE);
			size += sizeof(u16) + SHA512_SIZE;
			break;
		case TPM_ALG_SM3_256:
			h->alg = cpu_to_be16(h->alg);
			h = (struct tpmt_ha *)((u8 *)h + SM3256_SIZE);
			size += sizeof(u16) + SHA1_SIZE;
			break;
		default:
			return 0;
		}
	}

	return size;
}

int tpm2_extend_pcr(struct tpm *t, u32 pcr,
		struct tpml_digest_values *digests)
{
	struct tpmbuff *b = t->buff;
	struct tpm2_cmd cmd;
	u8 *ptr;
	u16 size;
	int ret = 0;

	ret = tpm2_alloc_cmd(b, &cmd, TPM_ST_SESSIONS, TPM_CC_PCR_EXTEND);
	if (ret < 0)
		return ret;

	cmd.handles = (u32 *)tpmb_put(b, sizeof(u32));
	if (cmd.handles == NULL) {
		tpmb_free(b);
		return -ENOMEM;
	}

	*cmd.handles = cpu_to_be32(pcr);

	cmd.auth = (struct tpm2b *)tpmb_put(b, tpm2_null_auth_size());
	if (cmd.auth == NULL) {
		tpmb_free(b);
		return -ENOMEM;
	}

	cmd.auth->size = tpm2_null_auth(cmd.auth->buffer);
	cmd.auth->size = cpu_to_be16(cmd.auth->size);

	size = convert_digest_list(digests);
	if (size == 0) {
		tpmb_free(b);
		return -EINVAL;
	}

	cmd.params = (u8 *)tpmb_put(b, size);
	if (cmd.params == NULL) {
		tpmb_free(b);
		return -ENOMEM;
	}

	memcpy(cmd.params, digests, size);

	cmd.header->size = cpu_to_be16(tpmb_size(b));

	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		ret = tis_send(b);
		break;
	case TPM_CRB:
		ret = crb_send(b);
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	tpmb_free(b);
	return ret;
}
