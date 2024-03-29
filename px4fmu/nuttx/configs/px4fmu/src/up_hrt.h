/*
 *   Copyright (C) 2012 Michael Smith. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the author or the names of contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * High-resolution timer callouts and timekeeping.
 */

#include <sys/types.h>
#include <stdbool.h>

#include <time.h>
#include <queue.h>

/*
 * Absolute time, in microsecond units.
 *
 * Absolute time is measured from some arbitrary epoch shortly after
 * system startup.  It should never wrap or go backwards.
 */
typedef uint64_t	hrt_abstime;

/*
 * Callout function type.
 *
 * Note that callouts run in the timer interrupt context, so
 * they are serialised with respect to each other, and must not
 * block.
 */
typedef void		(* hrt_callout)(void *arg);

/*
 * Callout record.
 */
struct hrt_call {
	struct sq_entry_s	link;

	hrt_abstime		deadline;
	hrt_abstime		period;
	hrt_callout		callout;
	void			*arg;
};

/*
 * Get absolute time.
 */
extern hrt_abstime hrt_absolute_time(void);

/*
 * Convert a timespec to absolute time.
 */
extern hrt_abstime ts_to_abstime(struct timespec *ts);

/*
 * Convert absolute time to a timespec.
 */
extern void	abstime_to_ts(struct timespec *ts, hrt_abstime abstime);

/*
 * Call callout(arg) after delay has elapsed.
 */
extern void	hrt_call_after(struct hrt_call *entry, hrt_abstime delay, hrt_callout callout, void *arg);

/*
 * Call callout(arg) at absolute time calltime.
 */
extern void	hrt_call_at(struct hrt_call *entry, hrt_abstime calltime, hrt_callout callout, void *arg);

/*
 * Call callout(arg) after delay, and then after every interval.
 *
 * Note thet the interval is timed between scheduled, not actual, call times, so the call rate may
 * jitter but should not drift.
 */
extern void	hrt_call_every(struct hrt_call *entry, hrt_abstime delay, hrt_abstime interval, hrt_callout callout, void *arg);

/*
 * If this returns true, the entry has been invoked and removed from the callout list.
 *
 * Always returns false for repeating callouts.
 */
extern bool	hrt_called(struct hrt_call *entry);

/*
 * Remove the entry from the callout list.
 */
extern void	hrt_cancel(struct hrt_call *entry);

/*
 * Initialise the HRT.
 */
extern void	hrt_init(int timer);
