/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hv_lib.h>

static int do_udiv32(uint32_t dividend, uint32_t divisor,
	struct udiv_result *res)
{

	uint32_t mask;
	/* dividend is always greater than or equal to the divisor. Neither
	 * divisor nor dividend are 0. Thus: * clz(dividend) and clz(divisor)
	 * are valid * clz(dividend)<=clz(divisor)
	 */

	mask = clz(divisor) - clz(dividend);
	/* align divisor and dividend */
	divisor <<= mask;
	mask = 1U << mask;
	/* division loop */
	do {
		if (dividend >= divisor) {
			dividend -= divisor;
			res->q.dwords.low |= mask;
		}
		divisor >>= 1;
	} while (((mask >>= 1) != 0) && (dividend != 0));
	/* dividend now contains the reminder */
	res->r.dwords.low = dividend;
	return 0;
}

int udiv32(uint32_t dividend, uint32_t divisor, struct udiv_result *res)
{

	/* initialize the result */
	res->q.dwords.low = res->r.dwords.low = 0;
	/* test for "division by 0" condition */
	if (divisor == 0) {
		res->q.dwords.low = 0xffffffff;
		return !0;
	}
	/* trivial case: divisor==dividend */
	if (divisor == dividend) {
		res->q.dwords.low = 1;
		return 0;
	}
	/* trivial case: divisor>dividend */
	if (divisor > dividend) {
		res->r.dwords.low = dividend;
		return 0;
	}
	/* now that the trivial cases are eliminated we can call the generic
	 * function.
	 */
	return do_udiv32(dividend, divisor, res);
}

int udiv64(uint64_t dividend, uint64_t divisor, struct udiv_result *res)
{

	uint64_t mask;
	uint64_t bits;

	/* initialize the result */
	res->q.qword = res->r.qword = 0;
	/* test for "division by 0" condition */
	if (divisor == 0) {
		res->q.qword = 0xffffffffffffffffull;
		return -1;
	}
	/* trivial case: divisor==dividend */
	if (divisor == dividend) {
		res->q.qword = 1;
		return 0;
	}
	/* trivial case: divisor>dividend */
	if (divisor > dividend) {
		res->r.qword = dividend;
		return 0;
	}
	/* simplified case: only 32 bit operands Note that the preconditions
	 * for do_udiv32() are fulfilled, since the tests were made above.
	 */
	if (((divisor >> 32) == 0) && ((dividend >> 32) == 0))
		return do_udiv32((uint32_t) dividend, (uint32_t) divisor, res);

	/* dividend is always greater than or equal to the divisor. Neither
	 * divisor nor dividend are 0. Thus: * clz(dividend) and clz(divisor)
	 * are valid * clz(dividend)<=clz(divisor)
	 */

	/* align divisor and dividend. */
	bits = clz64(divisor) - clz64(dividend);
	divisor <<= bits;
	mask = 1ULL << bits;
	/* division loop */
	do {
		if (dividend >= divisor) {
			dividend -= divisor;
			res->q.qword |= mask;
		}
		divisor >>= 1;
		mask >>= 1;
	} while ((bits-- != 0) && (dividend != 0));

	res->r.qword = dividend;
	return 0;
}
