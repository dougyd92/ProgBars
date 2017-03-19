#include <pebble.h>
#include "utilities.h"

/*** External Functions ***/

/**
 * Returns the number of days in a given month.
 *
 * @param struct tm *tick_time: Time used to determine the month.
 * @return int: The number of days in the month.
 */
int get_days_in_month(struct tm *tick_time) {
	/* Month is in the range [0-11]. */
	int month = tick_time->tm_mon;
	int year = tick_time->tm_year;
	
	if (month == 3 || month == 5 || month == 8 || month == 10) {
		return 30;
	}
	else if (month == 1) {
		/* Determine if this is a leap year. A year is a leap year if it is
		evenly divided by 4 and it is not evenly divided by 100, unless it is 
		also divisble by 400.
    	*/
		if (year % 4 == 0 && 
			(!(year % 100 == 0) || (year % 400 == 0))) {
			return 29;
		}
		else {
			return 28;
		}
	}
	else {
		return 31;
	}
}
