/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 * The definitions in this header are extracted from the Trusted Computing
 * Group's "TPM Main Specification", Parts 1-3.
 *
 */

#ifndef _CRB_H
#define _CRB_H

#include <types.h>
#include <tpm.h>

#define STATIC_TIS_BUFFER_SIZE 1024
/* TPM Interface Specification functions */
u8 crb_request_locality(u8 l);
void crb_relinquish_locality(void);
u8 crb_init(struct tpm *t);
size_t crb_send(struct tpmbuff *buf);
size_t crb_recv(struct tpmbuff *buf);

#endif
