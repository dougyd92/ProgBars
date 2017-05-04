#pragma once

/*** Functions ***/
int get_days_in_month(struct tm *tick_time);
void draw_outlined_text(GContext *ctx, const char *text, GFont const font, int x, int y, 
						GSize text_size, GColor text_color, GColor text_outline_color);
