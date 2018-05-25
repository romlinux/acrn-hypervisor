/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LOGMSG_H
#define LOGMSG_H

/* Logging severity levels */
#define LOG_FATAL		1
/* For msg should be write to console and sbuf meanwhile but not fatal error */
#define LOG_ACRN		2
#define LOG_ERROR		3
#define LOG_WARNING		4
#define LOG_INFO		5
#define LOG_DEBUG		6

/* Logging flags */
#define LOG_FLAG_STDOUT		0x00000001
#define LOG_FLAG_MEMORY		0x00000002

#if defined(HV_DEBUG)

extern uint32_t console_loglevel;
extern uint32_t mem_loglevel;
void init_logmsg(uint32_t mem_size, uint32_t flags);
void print_logmsg_buffer(uint32_t cpu_id);
void do_logmsg(uint32_t severity, const char *fmt, ...);

#else /* HV_DEBUG */

static inline void init_logmsg(__unused uint32_t mem_size,
			__unused uint32_t flags)
{
}

static inline void do_logmsg(__unused uint32_t severity,
			__unused const char *fmt, ...)
{
}

static inline void print_logmsg_buffer(__unused uint32_t cpu_id)
{
}

#endif /* HV_DEBUG */

#ifndef pr_prefix
#define pr_prefix
#endif

#define pr_fatal(...)						\
	do {							\
		do_logmsg(LOG_FATAL, pr_prefix __VA_ARGS__);	\
	} while (0)

#define pr_acrnlog(...)						\
	do {							\
		do_logmsg(LOG_ACRN, pr_prefix __VA_ARGS__);	\
	} while (0)

#define pr_err(...)						\
	do {							\
		do_logmsg(LOG_ERROR, pr_prefix __VA_ARGS__);	\
	} while (0)

#define pr_warn(...)						\
	do {							\
		do_logmsg(LOG_WARNING, pr_prefix __VA_ARGS__);	\
	} while (0)

#define pr_info(...)						\
	do {							\
		do_logmsg(LOG_INFO, pr_prefix __VA_ARGS__);	\
	} while (0)

#define pr_dbg(...)						\
	do {							\
		do_logmsg(LOG_DEBUG, pr_prefix __VA_ARGS__);	\
	} while (0)

#define dev_dbg(lvl, ...)					\
	do {							\
		do_logmsg(lvl, pr_prefix __VA_ARGS__);	\
	} while (0)

#define panic(...) 							\
	do { pr_fatal("PANIC: %s line: %d\n", __func__, __LINE__);	\
		pr_fatal(__VA_ARGS__); 					\
		while (1) { asm volatile ("pause" ::: "memory"); }; } while (0)

#endif /* LOGMSG_H */
