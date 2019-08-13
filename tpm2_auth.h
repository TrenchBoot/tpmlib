/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 * The definitions in this header are extracted and/or dervied from the
 * Trusted Computing Group's TPM 2.0 Library Specification Parts 1&2.
 *
 */

#ifndef _TPM2_AUTH_H
#define _TPM2_AUTH_H

u16 tpm2_null_auth_size(void);
u16 tpm2_null_auth(u8 *b);

#endif
