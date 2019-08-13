/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 * The code in this file is based on the article "Writing a TPM Device Driver"
 * published on http://ptgmedia.pearsoncmg.com.
 *
 */

#include <mem.h>
#include <tpm.h>
#include <tpmbuff.h>

#include "tis.h"
#include "tpm_common.h"
#include "tpm1.h"

u8 tpm1_pcr_extend(struct tpm *t, struct tpm_digest *d)
{
	struct tpmbuff *b = t->buff;
	struct tpm_header *hdr;
	struct tpm_extend_cmd *cmd;
	struct tpm_extend_resp *resp;
	size_t bytes;

	if (! tpmb_reserve(b))
		goto out;

	hdr = (struct tpm_header *)b->head;

	hdr->tag = TPM_TAG_RQU_COMMAND;
	hdr->code = TPM_ORD_EXTEND;

	cmd = (struct tpm_extend_cmd *)
		tpmb_put(b, sizeof(struct tpm_extend_cmd));
	if (cmd == NULL)
		goto free;

	cmd->pcr_num = d->pcr;
	memcpy(&(cmd->digest), &(d->digest), sizeof(TPM_DIGEST));

	hdr->size = tpmb_size(b);

	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		if (hdr->size != tis_send(b))
			goto free;
		break;
	case TPM_CRB:
		/* Not valid for TPM 1.2 */
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	tpmb_free(b);

	/* Reset buffer for receive */
	if (! tpmb_reserve(b))
		goto out;

	hdr = (struct tpm_header *)b->head;
	resp = (struct tpm_extend_resp *)
		tpmb_put(b, sizeof(struct tpm_extend_resp));
	if (resp == NULL)
		goto free;

	switch (t->intf) {
	case TPM_DEVNODE:
		/* Not implemented yet */
		break;
	case TPM_TIS:
		if (tpmb_size(b) != tis_recv(b))
			goto free;
		break;
	case TPM_CRB:
		/* Not valid for TPM 1.2 */
		break;
	case TPM_UEFI:
		/* Not implemented yet */
		break;
	}

	tpmb_free(b);

	if (resp->ordinal != TPM_SUCCESS)
		goto out;

	return 1;
free:
	tpmb_free(b);
out:
	return 0;
}
