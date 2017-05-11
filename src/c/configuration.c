#include <pebble.h>
#include <string.h>
#include "configuration.h"

/*** Constants ***/
const int CURRENT_SCHEMA_VERSION = 5;

/*** Internal Functions ***/

/**
 * Loads the default settings, which are hardcoded here.
 * 
 * @param app_settings_t *settings: Pointer to the settings struct.
 */
static void load_default_settings(app_settings_t *settings) {
	/* Default colors */
	settings->background_color = GColorBlack;
	settings->text_color = GColorWhite;
	settings->text_outline_color = GColorBlack;
	
	settings->bar_colors[HOURS_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorOxfordBlue, GColorWhite);
	settings->bar_colors[MINUTES_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorDukeBlue, GColorWhite);
	settings->bar_colors[COMBINED_HOURS_MINUTES_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorDukeBlue, GColorWhite);
	settings->bar_colors[SECONDS_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorVividCerulean, GColorWhite);
	settings->bar_colors[WEEKDAY_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorRajah, GColorWhite);
	settings->bar_colors[MONTH_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorBrass, GColorWhite);
	settings->bar_colors[DAY_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite);
	settings->bar_colors[COMBINED_MONTH_DAY_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite);
	settings->bar_colors[TEMPERATURE_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorImperialPurple, GColorWhite);
	settings->bar_colors[STEPS_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorDarkGreen, GColorWhite);
	settings->bar_colors[BATTERY_BAR_IDX] = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
	
	/* Default display settings: show all bars with hours/mins and month/day combined. */
	settings->show_bar[HOURS_BAR_IDX] = false;
	settings->show_bar[MINUTES_BAR_IDX] = false;
	settings->show_bar[COMBINED_HOURS_MINUTES_BAR_IDX] = true;
	settings->show_bar[SECONDS_BAR_IDX] = true;
	settings->show_bar[WEEKDAY_BAR_IDX] = true;
	settings->show_bar[MONTH_BAR_IDX] = false;
	settings->show_bar[DAY_BAR_IDX] = false;	
	settings->show_bar[COMBINED_MONTH_DAY_BAR_IDX] = true;	
	settings->show_bar[TEMPERATURE_BAR_IDX] = true;
	settings->show_bar[STEPS_BAR_IDX] = true;	
	settings->show_bar[BATTERY_BAR_IDX] = true;
	
	settings->temperature_scale = FAHRENHEIT;
	settings->temperature_min = 32;		
	settings->temperature_max = 100;
	
	settings->bar_style = SOLID;
}

/**
 * Reads a value from the dictionary into the passed boolean.
 * 
 * @param DictionaryIterator *it: Iterator for the app message with the settings.
 * @param uint32_t message_key: Message key for the settings to be read.
 * @param bool *setting: Variable to hold the value.
 */
static void read_setting_bool (DictionaryIterator *it, uint32_t message_key, bool *setting) {
	Tuple *setting_tuple = dict_find(it, message_key);
	if(setting_tuple) {
		*setting = (setting_tuple->value->int16 == 1);
	}
}

/**
 * Reads a value from the dictionary into the passed GColor.
 * 
 * @param DictionaryIterator *it: Iterator for the app message with the settings.
 * @param uint32_t message_key: Message key for the settings to be read.
 * @param GColor *setting: Variable to hold the value.
 */
static void read_setting_gcolor (DictionaryIterator *it, uint32_t message_key, GColor *setting) {
	Tuple *setting_tuple = dict_find(it, message_key);
	if(setting_tuple) {
		*setting = GColorFromHEX(setting_tuple->value->int32);
	}
}

/**
 * Reads a value from the dictionary into the passed int.		
 * 		
 * @param DictionaryIterator *it: Iterator for the app message with the settings.		
 * @param uint32_t message_key: Message key for the settings to be read.		
 * @param int *setting: Variable to hold the value.		
 */		
static void read_setting_int (DictionaryIterator *it, uint32_t message_key, int *setting) {		
	Tuple *setting_tuple = dict_find(it, message_key);		
	if(setting_tuple) {		
		*setting = setting_tuple->value->int32;
	}		
}

/*** External Functions ***/

/**
 * Reads in the saved settings from persistent storage.
 * If stored settings are not available, loads the default settings.
 * 
 * @param app_settings_t *settings: Struct to store the settings.
 */
void load_settings(app_settings_t *settings) {
	/* Check if there are saved settings in persistent storage. */
	if (persist_exists(STORAGE_KEY_SETTINGS) && persist_exists(STORAGE_KEY_VERSION)) {
		int saved_version;
		persist_read_data(STORAGE_KEY_VERSION, &saved_version, sizeof(int));

		if (saved_version == CURRENT_SCHEMA_VERSION) {
			APP_LOG(APP_LOG_LEVEL_INFO, "Loaded saved settings with same version.");
			persist_read_data(STORAGE_KEY_SETTINGS, settings, sizeof(app_settings_t));
		}
		else {
			APP_LOG(APP_LOG_LEVEL_INFO, "Version has changed; previous saved settings replaced with new defaults.");
			persist_delete(STORAGE_KEY_SETTINGS);
			load_default_settings(settings);
		}
	}
	else {
		APP_LOG(APP_LOG_LEVEL_INFO, "No saved settings found, using defaults.");
		load_default_settings(settings);
	}
}

/**
 * Saves the settings to persistent storage.
 * 
 * @param app_settings_t *settings: Struct with the settings to save.
 */
void save_settings(app_settings_t *settings) {
	persist_write_data(STORAGE_KEY_VERSION, &CURRENT_SCHEMA_VERSION, sizeof(int));
	persist_write_data(STORAGE_KEY_SETTINGS, settings, sizeof(app_settings_t));
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Settings saved to persistent storage.");
}

/**
 * Parses the dictionary in the app message into the settings.
 *
 * @param app_settings_t *settings: Struct to hold the settings.
 & @param DictionaryIterator *it: Iterator for the app message with the settings.
 */
void read_settings_from_app_message(app_settings_t *settings, DictionaryIterator *it) {
	/* Read the colors settings. */
	read_setting_gcolor(it, MESSAGE_KEY_BackgroundColor, &(settings->background_color));
	read_setting_gcolor(it, MESSAGE_KEY_TextColor, &(settings->text_color));
	read_setting_gcolor(it, MESSAGE_KEY_TextOutlineColor, &(settings->text_outline_color));
	
	/* Loop to read whether each bar is enabled and read the color. */
	for (int i = 0; i < TOTAL_BARS; ++i) {
		read_setting_bool(it, MESSAGE_KEY_BarCheckboxes + i, &(settings->show_bar[i]));
		read_setting_gcolor(it, MESSAGE_KEY_BarColors + i, &(settings->bar_colors[i]));
	}
	
	Tuple *temperature_scale_tuple = dict_find(it, MESSAGE_KEY_TemperatureScale);
	if(temperature_scale_tuple) {
		if (temperature_scale_tuple->value->cstring[0] == 'C') {
			settings->temperature_scale = CELSIUS;
		}
		else if (temperature_scale_tuple->value->cstring[0] == 'F') {
			settings->temperature_scale = FAHRENHEIT;
		}
		else {
			APP_LOG(APP_LOG_LEVEL_ERROR, "Invalid value for MESSAGE_KEY_TemperatureScale: %s.", 
					temperature_scale_tuple->value->cstring);
		}
	}
	
	/* Read the correct temperature bounds, either C or F depending on what the user chose. */
	if (settings->temperature_scale == FAHRENHEIT) {		
		read_setting_int(it, MESSAGE_KEY_TemperatureMinF, &(settings->temperature_min));		
		read_setting_int(it, MESSAGE_KEY_TemperatureMaxF, &(settings->temperature_max));			
	}		
	else if (settings->temperature_scale == CELSIUS) {		
		read_setting_int(it, MESSAGE_KEY_TemperatureMinC, &(settings->temperature_min));		
		read_setting_int(it, MESSAGE_KEY_TemperatureMaxC, &(settings->temperature_max));			
	}	
	
	Tuple *bar_style_tuple = dict_find(it, MESSAGE_KEY_BarStyle);
	if(bar_style_tuple) {
		if (bar_style_tuple->value->cstring[0] == 'S') {
			settings->bar_style = SOLID;
		}
		else if (bar_style_tuple->value->cstring[0] == 'O') {
			settings->bar_style = OUTLINE;
		}
		else {
			APP_LOG(APP_LOG_LEVEL_ERROR, "Invalid value for MESSAGE_KEY_BarStyle: %s.", 
					bar_style_tuple->value->cstring);
		}
	}	
}

/**
 * Returns the count of bars that are visible.
 * 
 * @param app_settings_t *settings: The settings struct to check.
 * @retrun int: The count of visible bars.
 */
int count_enabled_bars(app_settings_t *settings) {
	int count = 0;

	for (int i = 0; i < TOTAL_BARS; ++i) {
		if (settings->show_bar[i])
			++count;
	}
	
	return count;
}
