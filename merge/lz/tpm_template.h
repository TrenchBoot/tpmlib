/*
 * Copyright (c) 2018 Daniel P. Smith, Apertus Solutions, LLC
 *
 * The definitions in this header are extracted from the Trusted Computing
 * Group's "TPM Main Specification", Parts 1-3.
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
 */

#ifndef _TPM_H
#define _TPM_H

#include <types.h>

#define TPM_HASH_ALG_SHA1    (u16)(0x0004)
#define TPM_HASH_ALG_SHA256  (u16)(0x000B)
#define TPM_HASH_ALG_SHA384  (u16)(0x000C)
#define TPM_HASH_ALG_SHA512  (u16)(0x000D)
#define TPM_HASH_ALG_SM3_256 (u16)(0x0012)

