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

/**
 * Draw outlined text within a graphics context.
 *
 * @param GContext ctx: The destination graphics context in which to draw.
 * @param const char *text: The zero terminated UTF-8 string to draw.
 * @param GFont const font: The font in which the text should be set.
 * @param int x: The x-value of the top left corner of the box in which the text is drawn.
 * @param int y: The y-value of the top left corner of the box in which the text is drawn.
 * @param GSize text_size: The maximum size that the text will occupy; given by graphics_text_layout_get_content_size.
 * @param GColor text_color: Color of the inner portion of the text.
 * @param GColor text_outline_color: Color of the outline around the text.
 */
void draw_outlined_text(GContext *ctx, const char *text, GFont const font, int x, int y, 
						GSize text_size, GColor text_color, GColor text_outline_color) {
	
	graphics_context_set_text_color(ctx, text_outline_color);

	/* Draw offset by 1 pixel in each direction to create the outline. */
	graphics_draw_text(ctx, text, font, 
					   GRect(x + 1, y + 1, text_size.w, text_size.h), 
					   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	graphics_draw_text(ctx, text, font, 
					   GRect(x + 1, y - 1, text_size.w, text_size.h), 
					   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	graphics_draw_text(ctx, text, font, 
					   GRect(x - 1, y + 1, text_size.w, text_size.h), 
					   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	graphics_draw_text(ctx, text, font, 
					   GRect(x - 1, y - 1, text_size.w, text_size.h), 
					   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	/* Draw the inside of the text. */
	graphics_context_set_text_color(ctx, text_color);
	
	graphics_draw_text(ctx, text, font, 
					   GRect(x , y, text_size.w, text_size.h), 
					   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);	
}
