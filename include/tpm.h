/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 */

#ifndef _TPM_H
#define _TPM_H

#include <types.h>

#define TPM_NO_LOCALITY		0xFF

enum tpm_hw_intf {
	TPM_DEVNODE,
	TPM_TIS,
	TPM_CRB,
	TPM_UEFI
};

enum tpm_family {
	TPM12,
	TPM20
};

struct tpmbuff;

struct tpm {
	u32 vendor;
	enum tpm_family family;
	enum tpm_hw_intf intf;
	struct tpmbuff *buff;
};

struct tpm *enable_tpm(void);
u8 tpm_request_locality(struct tpm *t, u8 l);
void tpm_relinquish_locality(struct tpm *t);
int tpm_extend_pcr(struct tpm *t, u32 pcr, u16 algo,
		u8 *digest);
void free_tpm(struct tpm *t);
#endif
