/*
 * Copyright (c) 2018 Daniel P. Smith, Apertus Solutions, LLC
 *
 * The definitions in this header are extracted from:
 *  - Trusted Computing Group's "TPM Main Specification", Parts 1-3.
 *  - Trusted Computing Group's TPM 2.0 Library Specification Parts 1&2.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <types.h>
#include <io.h>
#include <errno-base.h>
#include <string.h>
#include <be_byteshift.h>
#include <tpm.h>

#include "early_tpm.h"

static inline u32 be16_to_cpu(u16 val)
{
	return get_unaligned_be16((const void*)(u64)&val);
}
static inline u32 cpu_to_be16(u16 val)
{
	return get_unaligned_be16((const void*)(u64)&val);
}
static inline u32 cpu_to_be32(u32 val)
{
	return get_unaligned_be32((const void*)(u64)&val);
}

static u8 locality = TPM_NO_LOCALITY;

