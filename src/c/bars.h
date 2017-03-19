#pragma once

#include <math.h>
#include "configuration.h"
#include "utilities.h"

#ifndef PBL_DISPLAY_WIDTH
#define PBL_DISPLAY_WIDTH 144
#endif

#ifndef PBL_DISPLAY_HEIGHT
#define PBL_DISPLAY_HEIGHT 168
#endif

/*** Functions ***/
void bars_init(Window* win_main);
void bars_deinit();
Layer* bars_create_layer();
void bars_destroy_layer();
void bars_handle_temperature_received(int new_temperature);
void bars_handle_settings_received(DictionaryIterator *it, Window* win_main);
