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

#ifndef _TIS_H
#define _TIS_H

#include <types.h>
#include <tpm.h>

#include "tpm_common.h"

#define STATIC_TIS_BUFFER_SIZE 1024

/* macros to access registers at locality ’’l’’ */
#define ACCESS(l)			(0x0000 | ((l) << 12))
#define STS(l)				(0x0018 | ((l) << 12))
#define DATA_FIFO(l)			(0x0024 | ((l) << 12))
#define DID_VID(l)			(0x0F00 | ((l) << 12))
/* access bits */
#define ACCESS_ACTIVE_LOCALITY		0x20 /* (R)*/
#define ACCESS_RELINQUISH_LOCALITY	0x20 /* (W) */
#define ACCESS_REQUEST_USE		0x02 /* (W) */
/* status bits */
#define STS_VALID			0x80 /* (R) */
#define STS_COMMAND_READY		0x40 /* (R) */
#define STS_DATA_AVAIL			0x10 /* (R) */
#define STS_DATA_EXPECT			0x08 /* (R) */
#define STS_GO				0x20 /* (W) */

static inline bool tis_data_available(int locality) 
{ 
        int status; 

        status = tpm_read8(STS(locality)); 
        return ((status & (STS_DATA_AVAIL | STS_VALID)) == 
		(STS_DATA_AVAIL | STS_VALID));
} 

/* TPM Interface Specification functions */
u8 tis_request_locality(u8 l);
void tis_relinquish_locality(void);
u8 tis_init(struct tpm *t);
size_t tis_send(struct tpmbuff *buf);
size_t tis_recv(enum tpm_family f, struct tpmbuff *buf);

#endif
