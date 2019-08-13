/*
 * Copyright (c) 2019 Daniel P. Smith, Apertus Solutions, LLC
 *
 */

void tpm_io_delay(void)
{
	io_delay();
}

void tpm_udelay(int loops)
{
	while (loops--)
		tpm_io_delay();	/* Approximately 1 us */
}

u8 tpm_read8(u32 field)
{
	return ioread8((void*)(u64)(TPM_MMIO_BASE | field));
}

void tpm_write8(unsigned char val, u32 field)
{
	iowrite8((void*)(u64)(TPM_MMIO_BASE | field), val);
}

u32 tpm_read32(u32 field)
{
	return ioread32((void*)(u64)(TPM_MMIO_BASE | field));
}

void tpm_write32(u32 val, u32 field)
{
	iowrite32((void*)(u64)(TPM_MMIO_BASE | field), val);
}
