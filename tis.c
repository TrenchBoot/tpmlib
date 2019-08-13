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

#include <tpm.h>
#include <tpmbuff.h>

#include "tpm_common.h"
#include "tis.h"

static u8 locality = TPM_NO_LOCALITY;

static u32 burst_wait(void)
{
	u32 count = 0;

	while (count == 0) {
		count = tpm_read8(STS(locality) + 1);
		count += tpm_read8(STS(locality) + 2) << 8;

		if (count == 0)
			tpm_io_delay(); /* wait for FIFO to drain */
	}

	return count;
}

void tis_relinquish_locality(void)
{
        if (locality < TPM_MAX_LOCALITY)
		tpm_write8(ACCESS_RELINQUISH_LOCALITY, ACCESS(locality));

        locality = TPM_NO_LOCALITY;
}

s8 tis_request_locality(u8 l)
{
        if (l > TPM_MAX_LOCALITY)
                return -EINVAL;

	if (l == locality)
		return 0;

	tis_relinquish_locality();

        tpm_write8(ACCESS_REQUEST_USE, ACCESS(l));

        /* wait for locality to be granted */
        if (tpm_read8(ACCESS(l)) & ACCESS_ACTIVE_LOCALITY)
                locality = l;

        return 0;
}

u8 tis_init(struct tpm *t)
{
        u8 i;

        for (i=0; i <= TPM_MAX_LOCALITY; i++)
                tpm_write8(ACCESS_RELINQUISH_LOCALITY, ACCESS(i));

        locality = TPM_NO_LOCALITY;

        if (tis_request_locality(0) != 0)
                return 0;

        t->vendor = tpm_read32(DID_VID(0));
        if ((t->vendor & 0xFFFF) == 0xFFFF)
                return 0;

        return 1;
}

size_t tis_send(struct tpmbuff *buf)
{
	u8 status, *buf_ptr;
	u32 burstcnt = 0;
	u32 count = 0;

	if (locality > TPM_MAX_LOCALITY)
		return 0;

	tpm_write8(STS_COMMAND_READY, STS(locality));

	buf_ptr = buf->head;

	/* send all but the last byte */
	while (count < (buf->len - 1)) {
		burstcnt = burst_wait();
		for (; burstcnt > 0 && count < (buf->len - 1); burstcnt--) {
			tpm_write8(buf_ptr[count], DATA_FIFO(locality));
			count++;
		}

		/* check for overflow */
		for (status = 0; (status & STS_VALID) == 0; )
			status = tpm_read8(STS(locality));

		if ((status & STS_DATA_EXPECT) == 0)
			return 0;
	}

	/* write last byte */
	tpm_write8(buf_ptr[buf->len - 1], DATA_FIFO(locality));
	count++;

	/* make sure it stuck */
	for (status = 0; (status & STS_VALID) == 0; )
		status = tpm_read8(STS(locality));

	if ((status & STS_DATA_EXPECT) != 0)
		return 0;

	/* go and do it */
	tpm_write8(STS_GO, STS(locality));

	return (size_t)count;
}

static size_t recv_data(unsigned char *buf, size_t len)
{
	size_t size = 0;
	u8 status, *bufptr;
	u32 burstcnt = 0;

	bufptr = (u8 *)buf;

	while (tis_data_available(locality) && size < len) {
		burstcnt = burst_wait();
		for (; burstcnt > 0 && size < len; burstcnt--) {
			*bufptr = tpm_read8(DATA_FIFO(locality));
			bufptr++;
			size++;
		}
	}

	return size;
}

size_t tis_recv(struct tpmbuff *buf)
{
	u32 expected;
	u8 status, *buf_ptr;
	struct tpm_header *hdr;

	if (locality > TPM_MAX_LOCALITY)
		goto err;

	/* ensure that there is data available */
	if (! tis_data_available(locality))
		goto err;

	/* read header */
	hdr = (struct tpm_header *)buf->head;
	expected = sizeof(struct tpm_header);
	if (recv_data(buf->head, expected) < expected)
		goto err;

	/* convert header */
	hdr->tag = be16_to_cpu(hdr->tag);
	hdr->size = be32_to_cpu(hdr->size);
	hdr->code = be32_to_cpu(hdr->code);

	/* hdr->size = header + data */
	expected = hdr->size - expected;
	buf_ptr = tpmb_put(buf, expected);
	if (! buf_ptr)
		goto err;

	/* read all data, except last byte */
	if (recv_data(buf_ptr, expected - 1) < (expected - 1))
		goto err;

	/* check for receive underflow */
	if (! tis_data_available(locality))
		goto err;

	/* read last byte */
	if (recv_data(buf_ptr, 1) != 1)
		goto err;

	/* make sure we read everything */
	if (tis_data_available(locality)) {
		goto err;
	}

	tpm_write8(STS_COMMAND_READY, STS(locality));

	return hdr->size;
err:
	return 0;
}
