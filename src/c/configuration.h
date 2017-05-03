#pragma once

#include<pebble.h>

/**
 * Keys for persistent storage. 
 */
enum {
	STORAGE_KEY_VERSION,
	STORAGE_KEY_SETTINGS,
	STORAGE_KEY_TEMPERATURE,
	STORAGE_KEY_STEPS
};

/**
 * Indexes used to iterate over each bar.
 */
enum {
	HOURS_BAR_IDX,
	MINUTES_BAR_IDX,
	COMBINED_HOURS_MINUTES_BAR_IDX,
	SECONDS_BAR_IDX,
	WEEKDAY_BAR_IDX,
	MONTH_BAR_IDX,
	DAY_BAR_IDX,
	COMBINED_MONTH_DAY_BAR_IDX,
	TEMPERATURE_BAR_IDX,
	STEPS_BAR_IDX,
	BATTERY_BAR_IDX,
	TOTAL_BARS
};

/**
 * Temperature scales. Kelvin not supported.
 */
typedef enum {
	CELSIUS,
	FAHRENHEIT
} temperature_scale_e;

/**
 * Holds all the user settings for the app.
 */
typedef struct {
	GColor background_color;
	GColor text_color;
	GColor bar_colors[TOTAL_BARS];
	bool show_bar[TOTAL_BARS];
	temperature_scale_e temperature_scale;
	int temperature_min;		
	int temperature_max;	
} app_settings_t;

/*** Functions ***/
void load_settings(app_settings_t *settings);
void read_settings_from_app_message(app_settings_t *settings, DictionaryIterator *it);
void save_settings(app_settings_t *settings);
int count_enabled_bars(app_settings_t *settings);
