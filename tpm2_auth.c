/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 */

#include <types.h>
#include <mem.h>

#include "tpm2_constants.h"

#define NULL_AUTH_SIZE 9

u16 tpm2_null_auth_size(void)
{
	return NULL_AUTH_SIZE;
}

u16 tpm2_null_auth(u8 *b)
{
	u32 *handle = (u32 *)b;

	memset(b, 0, NULL_AUTH_SIZE);

	*handle = cpu_to_be32(TPM_RS_PW);

	return NULL_AUTH_SIZE;
}
