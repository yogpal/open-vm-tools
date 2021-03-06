/*********************************************************
 * Copyright (C) 1998 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/

/*
 * timeutil.h --
 *
 *   Miscellaneous time related utility functions.
 *
 */

#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#include <time.h>

#define INCLUDE_ALLOW_USERLEVEL
#include "includeCheck.h"

#include "vm_basic_types.h"
#include "vm_assert.h"


#define MAX_DAYSLEFT     4096

struct timeval;

#ifdef _WIN32
struct timespec {
   time_t tv_sec;
   long   tv_nsec;
};
#endif

/* Similar to a struct tm but with slightly less weird semantics. */
typedef struct TimeUtil_Date {
   unsigned int year;   /* e.g. 1970 */
   unsigned int month;  /* [1, 12] */
   unsigned int day;    /* [1, 31] */
   unsigned int hour;   /* [0, 23] */
   unsigned int minute; /* [0, 59] */
   unsigned int second; /* [0, 61] (for leap seconds) */
} TimeUtil_Date;

typedef struct TimeUtil_TimeOfDay {
   unsigned long seconds;
   unsigned long useconds;
} TimeUtil_TimeOfDay;

typedef struct TimeUtil_Expiration {
   /*
    * Does it expire?
    */

   Bool expires;

   /*
    * When does it expire? (valid only if 'expires' == TRUE)
    *
    * Note: TimeUtil_Expiration only uses the 'year', 'month'
    * and 'day' fields of 'when'.
    */

   TimeUtil_Date when;

   /*
    * Compute this once for all, to avoid problems when the current day changes
    * (valid only if 'expires' == TRUE).
    */

   unsigned int daysLeft;
} TimeUtil_Expiration;


EXTERN time_t TimeUtil_MakeTime(const TimeUtil_Date *d);

EXTERN Bool TimeUtil_StringToDate(TimeUtil_Date *d,    // IN/OUT
                                  char const *date);   // IN: 'YYYYMMDD' or 'YYYY/MM/DD' or 'YYYY-MM-DD'

EXTERN Bool TimeUtil_DaysSubtract(TimeUtil_Date *d,  // IN/OUT
                                  unsigned int nr);  // IN

EXTERN int TimeUtil_DeltaDays(TimeUtil_Date const *left,   // IN
                              TimeUtil_Date const *right); // IN

EXTERN void TimeUtil_DaysAdd(TimeUtil_Date *d, // IN/OUT
                             unsigned int nr); // IN

EXTERN void TimeUtil_PopulateWithCurrent(Bool local,        // IN
                                         TimeUtil_Date *d); // OUT

EXTERN void TimeUtil_GetTimeOfDay(TimeUtil_TimeOfDay *d); // OUT

EXTERN unsigned int TimeUtil_DaysLeft(TimeUtil_Date const *d); // IN

EXTERN Bool TimeUtil_ExpirationLowerThan(TimeUtil_Expiration const *left,   // IN
                                         TimeUtil_Expiration const *right); // IN

EXTERN Bool TimeUtil_DateLowerThan(TimeUtil_Date const *left,   // IN
                                   TimeUtil_Date const *right); // IN

EXTERN void TimeUtil_ProductExpiration(TimeUtil_Expiration *e); // OUT

EXTERN char * TimeUtil_GetTimeFormat(int64 utcTime,  // IN
                                     Bool showDate,  // IN
                                     Bool showTime); // IN

EXTERN int TimeUtil_NtTimeToUnixTime(struct timespec *unixTime, // OUT
                                     VmTimeType ntTime);        // IN

EXTERN VmTimeType TimeUtil_UnixTimeToNtTime(struct timespec unixTime); // IN


#ifdef _WIN32
EXTERN Bool TimeUtil_UTCTimeToSystemTime(const __time64_t utcTime,    // IN
                                         SYSTEMTIME *systemTime);     // OUT
#endif

EXTERN int TimeUtil_GetLocalWindowsTimeZoneIndexAndName(char **ptzName);

#endif // _TIMEUTIL_H_
