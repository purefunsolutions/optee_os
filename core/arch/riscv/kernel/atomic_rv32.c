// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2026, Mika Tammi
 */

#include <compiler.h>
#include <stdint.h>

/*
 * RV32 has no 64-bit atomic load, so GCC calls this libatomic routine for
 * __atomic_load_n() on 64-bit objects. The core does that only through
 * READ_ONCE() on fields of messages the normal world writes in shared
 * memory, where the point is reading each value once (no double fetch),
 * not atomicity against concurrent writers: the normal world doesn't take
 * any lock of ours anyway. So each half is read exactly once, each read
 * being single-copy atomic, and no retry loop lets a hostile normal world
 * keep us spinning.
 */
uint64_t __atomic_load_8(const volatile void *ptr, int memorder);

uint64_t __atomic_load_8(const volatile void *ptr, int memorder __unused)
{
	const volatile uint32_t *w = ptr;
	uint32_t lo = w[0];
	uint32_t hi = w[1];

	return ((uint64_t)hi << 32) | lo;
}
