/*
 * Copyright (c) 2019 Apertus Solutions, LLC
 *
 * Author(s):
 *      Daniel P. Smith <dpsmith@apertussolutions.com>
 *
 */

#include <tpm.h>

#include "tpm_common.h"

void tpm_io_delay(void)
{
	/* This is the default delay type in native_io_delay */
	asm volatile ("outb %al, $0x80");
}

void tpm_udelay(int loops)
{
        while (loops--)
                tpm_io_delay();     /* Approximately 1 us */
}

u8 tpm_read8(u32 field)
{
	void *mmio_addr = (void*)(u64)(TPM_MMIO_BASE | field);

	return ioread8(mmio_addr);
}

void tpm_write8(unsigned char val, u32 field)
{
	void *mmio_addr = (void*)(u64)(TPM_MMIO_BASE | field);

	iowrite8(val, mmio_addr);
}

u32 tpm_read32(u32 field)
{
	void *mmio_addr = (void*)(u64)(TPM_MMIO_BASE | field);

	return ioread32(mmio_addr);
}

void tpm_write32(unsigned int val, u32 field)
{
	void *mmio_addr = (void*)(u64)(TPM_MMIO_BASE | field);

	iowrite32(val, mmio_addr);
}
