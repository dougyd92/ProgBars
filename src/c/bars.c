#include <pebble.h>
#include "bars.h"

/*** Constants ***/
const float BAR_SPACING = 8.0;
const int CORNER_RADIUS = 4;
const int LABEL_HORIZ_SPACING = 1;
const int LABEL_VERT_OFFSET = 2;
const int LABEL_WIDTH = 8;
const int WEATHER_UPDATE_FREQUENCY_MS = 900000; //15 minutes

/*** Internal Global Variables ***/
static float bar_height;
static GFont font_for_text;
static Layer *layer_bars;
static float progress[TOTAL_BARS];
static char *labels[TOTAL_BARS];
static app_settings_t settings;

/*** Internal Functions ***/

/** 
 * Draws a single horizontal bar.
 * 
 * @param GRect bounds: Bounds of the graphics layer. 
 * @param Gcontext *ctx: Graphics context to draw in. Passed along from the LayerUpdateProc.
 * @param float progress: Number describing how much the bar should be filled. 
 *	0.0 is empty, 1.0 fills though whole screen.
 * @param char *label: The text to be drawn at the end of the bar.
 * @param GColor fill_color: Color used to draw the bar.
 * @param float height: Height of the bar.
 * @param float *next_bar_start_y: y-position to start the bar at. Will be updated to be
 *	the height at which to start the next bar, based on this one's height. 
 */
static void draw_a_bar(GRect bounds, GContext *ctx, float progress, char *label, 
					   GColor fill_color, float height, float *next_bar_start_y) {

	int bar_filled_width = PBL_DISPLAY_WIDTH * progress;

	/* Space each bar vertically. */
	*next_bar_start_y += BAR_SPACING;

	graphics_context_set_fill_color(ctx, fill_color);

	/* Draw the rectangle that is the bar. */
	graphics_fill_rect(ctx, 
					   GRect(0, round(*next_bar_start_y), bar_filled_width, round(height)), 
					   CORNER_RADIUS, 
					   GCornersRight);

	GSize text_size = graphics_text_layout_get_content_size(label, font_for_text, bounds,
															GTextOverflowModeWordWrap, GTextAlignmentCenter);

	/* Draw the label at the end of the bar, but don't go off the screen when the bar
	is all the way full or off the chart (e.g. in extreme temperature, for example). */
	int label_x = bar_filled_width + LABEL_HORIZ_SPACING;
	if (label_x < 0)
		label_x = 0;
	else if (label_x > PBL_DISPLAY_WIDTH - text_size.w)
		label_x = PBL_DISPLAY_WIDTH - text_size.w;

		/* Draw the text label. */
		graphics_draw_text(ctx, label, font_for_text, 
						   GRect(label_x, *next_bar_start_y - LABEL_VERT_OFFSET, 
								 text_size.w, text_size.h), 
						   GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

	/* Update the starting y-position based on this one's height. The variable can then
	be passed in subsequent calls without the caller needing to update it. */
	*next_bar_start_y += height;
}

/**
 * LayerUpdateProc render function callback for the bars graphics layer.
 * Draws all the bars that are enabled in settings.  
 * 
 * @param Layer *layer: The layer that needs to be rendered.
 * @param GContext *ctx: The destination graphics context to draw into.
 */
static void redraw_bars(Layer *layer, GContext *ctx) {
	float next_bar_start_y = 0;
	GRect l_grect_bounds = layer_get_bounds(layer);

	/* The text color does need to get set every time, as there
	is no way to set the color outside of the graphics context. */
	graphics_context_set_text_color(ctx, GColorWhite);

	for (int i = 0; i < TOTAL_BARS; ++i) {
		if (settings.show_bar[i])
			draw_a_bar(l_grect_bounds, ctx, progress[i], labels[i], settings.bar_colors[i], 
					   bar_height, &next_bar_start_y);
	}
}

/**
 * TickHandler callback for the TickTimerService API.
 * Recalculates each bar's progress and label based on the new time. 
 *
 * @param struct tm *tick_time: The time at which the tick event was triggered.
 * @param TimeUnits units_changed: Which unit change triggered this tick event.
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	/* Update the seconds. */
	if (units_changed & SECOND_UNIT) {
		if (settings.show_bar[SECONDS_BAR_IDX]) {
			progress[SECONDS_BAR_IDX] = tick_time->tm_sec / 60.0;
			strftime(labels[SECONDS_BAR_IDX], LABEL_WIDTH, "%Ss", tick_time);
		}	
	}

	/* Update the minutes. */
	if (units_changed & MINUTE_UNIT) {
		if (settings.show_bar[MINUTES_BAR_IDX]) {
			/* When the setting to combine hours and minutes into one bar is enabled,
			the minutes bar is used for that. So, either update the hour and minute
			together (taking into account 12/24 hour time), or just the minutes. */
			if (settings.combine_hour_min) {
				if (clock_is_24h_style()) {
					progress[MINUTES_BAR_IDX] = (tick_time->tm_hour * 60 + tick_time->tm_min) / (24.0 * 60.0);
					strftime(labels[MINUTES_BAR_IDX], LABEL_WIDTH, "%H:%M", tick_time);
				}
				else {
					progress[MINUTES_BAR_IDX] = ((tick_time->tm_hour % 12) * 60 + tick_time->tm_min) / (12.0 * 60.0);
					strftime(labels[MINUTES_BAR_IDX], LABEL_WIDTH, "%I:%M%P", tick_time);
				}	
			}
			else {
				progress[MINUTES_BAR_IDX] = tick_time->tm_min / 60.0; 
				strftime(labels[MINUTES_BAR_IDX], LABEL_WIDTH, "%Mm", tick_time);
			}
		}	
	}

	/* Update the hours. */
	if (units_changed & HOUR_UNIT && !(settings.combine_hour_min)) {
		if (settings.show_bar[HOURS_BAR_IDX]) {		
			if (clock_is_24h_style()) {
				progress[HOURS_BAR_IDX] = tick_time->tm_hour / 24.0;
				strftime(labels[HOURS_BAR_IDX], LABEL_WIDTH, "%Hh", tick_time);
			}
			else {
				progress[HOURS_BAR_IDX] = (tick_time->tm_hour % 12) / 12.0;
				strftime(labels[HOURS_BAR_IDX], LABEL_WIDTH, "%I%P", tick_time);
			}
		}
	}

	/* Update the day of the week and the day of the month. */
	if (units_changed & DAY_UNIT) {
		if (settings.show_bar[WEEKDAY_BAR_IDX]) {
			progress[WEEKDAY_BAR_IDX] = tick_time->tm_wday / 7.0;
			strftime(labels[WEEKDAY_BAR_IDX], LABEL_WIDTH, "%a", tick_time);
		}	

		/* When the setting to combine days and months into one bar is enabled,
		the days bar is used for that. So, either update the day and month
		together or just theday of the month. */
		if (settings.show_bar[DAY_BAR_IDX]) {
			if (settings.combine_month_day) {
				progress[DAY_BAR_IDX] = tick_time->tm_yday / 365.0;
				strftime(labels[DAY_BAR_IDX], LABEL_WIDTH, "%b %d", tick_time);
			}
			else {
				progress[DAY_BAR_IDX] = (float) tick_time->tm_mday / get_days_in_month(tick_time);
				strftime(labels[DAY_BAR_IDX], LABEL_WIDTH, "%d", tick_time);
			}
		}	
	}

	/* Update the months. */
	if (units_changed & MONTH_UNIT && !(settings.combine_month_day)) {
		if (settings.show_bar[MONTH_BAR_IDX]) {
			progress[MONTH_BAR_IDX] = tick_time->tm_mon / 12.0;
			strftime(labels[MONTH_BAR_IDX], LABEL_WIDTH, "%b", tick_time);
		}	
	}

	/* Mark the display dirty. Only update each second if displaying seconds, 
	else update every minute. */
	if (((units_changed & SECOND_UNIT) && settings.show_bar[SECONDS_BAR_IDX])
		|| (units_changed & MINUTE_UNIT)) {
		layer_mark_dirty(layer_bars);
	}
}

/**
 * Sends a message to the phone to tell it to fetch the weather. 
 */
static void fetch_weather() { 
	/* Begin dictionary. */
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	/* Set the FetchTemperature message key to indicate that 
	the phone should retrieve the temperature. */
	dict_write_uint8(iter, MESSAGE_KEY_FetchTemperature, 1);

	/* Send the message. */
	app_message_outbox_send();
}

/**
 * Starts a timer to retrieve the weather periodically.
 */
static void fetch_weather_timer() {
	if (settings.show_bar[TEMPERATURE_BAR_IDX]) {
		fetch_weather(); 
	}

	/* Restart the timer. */
	app_timer_register(WEATHER_UPDATE_FREQUENCY_MS, fetch_weather_timer, NULL);
}

/**
 * Recalculates the temperature bar's progress and label based on the new temperature. 
 *
 * @param int new_temperature: The new temperature value in Fahrenheit.
 */
static void update_temperature(int new_temperature) {
	/* The bounds for the bar are arbitrarily from 32F to 100F, since those 
	seem like reasonable temperatures. Out of that range is "off the charts". */
	progress[TEMPERATURE_BAR_IDX] = (new_temperature - 32) / 68.0;	

	/* Write the correct label depending on the user's settings.
	"\u00B0" is the degree symbol. */
	if (settings.temperature_scale == FAHRENHEIT) {
		snprintf(labels[TEMPERATURE_BAR_IDX], LABEL_WIDTH, "%d\u00B0F", 
				 new_temperature);
	}
	else {
		/* Convert the temperature to Celsius from Fahrenheit. */
		snprintf(labels[TEMPERATURE_BAR_IDX], LABEL_WIDTH, "%d\u00B0C", 
				 (int)((new_temperature - 32) / 1.8));
	}
	layer_mark_dirty(layer_bars);
}

/**
 * BatteryStateHandler callback for the BatteryStateService API.
 * Recalculates the battery bar's progress and label based on the charge percent.
 * Note that the charge percent is returned by the API in 10% increments.
 *
 * @param BatteryChargeState state: The state of the battery BatteryChargeState
 */
static void battery_callback(BatteryChargeState state) {
	int battery_percent = state.charge_percent;

	progress[BATTERY_BAR_IDX] = battery_percent / 100.0;
	snprintf(labels[BATTERY_BAR_IDX], LABEL_WIDTH, "%d%%", battery_percent);

	layer_mark_dirty(layer_bars);
}

/**
 * Recalculates the step bar's progress and label based on the step count. 
 *
 * @param int new_steps: The number of steps.
 */
static void update_steps(int new_steps) {
	progress[STEPS_BAR_IDX] = new_steps / 10000.0;
	snprintf(labels[STEPS_BAR_IDX], LABEL_WIDTH, "%d", new_steps);

	layer_mark_dirty(layer_bars);
}
 
/**
 * HealthEventHandler callback for the HealthService API.
 * Reads the daily step count from the health data.
 *
 * @param HealthEventType event: The type of health-related event that occured. 
 * @param void *context: The context pointer.
 */
static void health_event_callback(HealthEventType event, void *context) {
	if (settings.show_bar[STEPS_BAR_IDX] &&
		(event == HealthEventSignificantUpdate ||
		 event == HealthEventMovementUpdate)) {

		HealthMetric metric = HealthMetricStepCount;
		time_t start = time_start_of_today();
		time_t end = time(NULL);
		int steps_today = 0;

		/* Check the metric has data available for today. */
		HealthServiceAccessibilityMask mask;
		mask = health_service_metric_accessible(metric, start, end);

		if(mask & HealthServiceAccessibilityMaskAvailable) {
			/* Get the total step coutn for today. */
			steps_today = health_service_sum_today(metric);
		}

		update_steps(steps_today);

		/* Save the step count to storage. This is so it can be read when the app loads, 
		avoiding having a blank display while waiting for the first health event. */
		persist_write_data(STORAGE_KEY_STEPS, &steps_today, sizeof(int));
	}
}

/**
 * Handles all the updates needed when settigs are changed, such as 
 * showing and hiding bars, updating supscriptions to event services 
 * when those bars are turned on or off, changing the temperature display 
 * from Fahrenheit to Celsius, etc.
 *
 * @param Win *win_main: Pointer to the main window. Needed to set the 
 *	window background color.
 */
static void settings_changed(Window *win_main) {
	/* Turn the light on briefly to highlight the new display. */
	light_enable_interaction();

	/* Calculate the bar height based on the number of visible bars. */
	int bar_count = count_enabled_bars(&settings);
	bar_height = (float)(PBL_DISPLAY_HEIGHT - (bar_count + 1) * BAR_SPACING) / bar_count;

	/* Update subscription to battery state service. */
	if (settings.show_bar[BATTERY_BAR_IDX]) {
		battery_callback(battery_state_service_peek());
		battery_state_service_subscribe(battery_callback);
	}
	else {
		battery_state_service_unsubscribe();
	}

	/* Update subscription to health tracking service. */
	if (settings.show_bar[STEPS_BAR_IDX]) {
		/* Initialize step count with saved value. */
		if (persist_exists(STORAGE_KEY_STEPS)) {	
			int saved_steps;
			persist_read_data(STORAGE_KEY_STEPS, &saved_steps, sizeof(int));
			update_steps(saved_steps);
		}

		health_service_events_subscribe(health_event_callback, NULL);
	}
	else {
		health_service_events_unsubscribe();
	}	

	/* Update subscription to tick timer service to use minutes or seconds. 
	Subsequent calls override the previous subscription, so there is no 
	need to explicilty unsubscribe from seconds, for instance. */
	if (settings.show_bar[SECONDS_BAR_IDX])
		tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	else
		tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	/* Force an update of all time units. */
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);	
	tick_handler(tick_time, SECOND_UNIT|MINUTE_UNIT|HOUR_UNIT|DAY_UNIT|MONTH_UNIT|YEAR_UNIT);

	/* Initialize temperature with saved value */
	if (settings.show_bar[TEMPERATURE_BAR_IDX] && persist_exists(STORAGE_KEY_TEMPERATURE)) {	
		int new_temperature;
		persist_read_data(STORAGE_KEY_TEMPERATURE, &new_temperature, sizeof(int));
		update_temperature(new_temperature);
	}	

	/* Set the background color. */
	window_set_background_color(win_main, settings.background_color);

	/* Trigger a redraw. */
	layer_mark_dirty(layer_bars);
}

/*** External Functions ***/

/**
 * Initializes resources, settings and data needed.
 *
 * @param Window* win_main: Pointer to the main window.
 */
void bars_init(Window *win_main) {
	/* Allocate memory for label strings. */
	for (int i = 0; i < TOTAL_BARS; ++i) {
		labels[i] = malloc(LABEL_WIDTH);
	}

	/* Load resources. */
	font_for_text = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OXYGEN_MONO_19));

	/* Load the settings, either from storage or from defaults. */
	load_settings(&settings);
	settings_changed(win_main);

	/* Start timer to fetch weather periodically. */
	app_timer_register(WEATHER_UPDATE_FREQUENCY_MS, fetch_weather_timer, NULL);
}

/**
 * Deallocates data and unloads resources.
 */
void bars_deinit() {
	/* Unload resources. */
	fonts_unload_custom_font(font_for_text);

	/* Deallocate memory for label strings. */
	for (int i = 0; i < TOTAL_BARS; ++i) {
		free(labels[i]);
	}
}

/**
 * Creates the bars graphics layer. 
 *
 * @return Layer*: Pointer to the bars graphics layer.
 */
Layer* bars_create_layer() {
	layer_bars = layer_create(GRect(0,0, PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT));
	layer_set_update_proc(layer_bars, redraw_bars);	
	return layer_bars;
}

/**
 * Destroys the bars graphics layer. 
 * Does not need to be passed a pointer to that layer;
 * as it is stored statically.
 */
void bars_destroy_layer() {
	layer_destroy(layer_bars);
}

/**
 * Updates and stores temperature data when received from the app message.
 *
 * @param int new_temperature: The new temperature value in Fahrenheit.
 */
void bars_handle_temperature_received(int new_temperature) {
	if (settings.show_bar[TEMPERATURE_BAR_IDX]) {
		update_temperature(new_temperature);
	}

	/* Save the temperature to storage. This is so it can be read when the app loads
	or when the bar is truned on, thus avoiding having a blank display while waiting 
	for the first response. */
	persist_write_data(STORAGE_KEY_TEMPERATURE, &new_temperature, sizeof(int));
}

/**
 * Reads and stores settings when received from the app message.
 *
 * @param int new_temperature: The new temperature value in Fahrenheit.
 */
void bars_handle_settings_received(DictionaryIterator *it, Window* win_main) {
	read_settings_from_app_message(&settings, it);
	settings_changed(win_main);
	save_settings(&settings);
}
