#include <stdio.h>
#include <stddef.h>
#include "bars.h"

/*** Global Variables ***/
static Window *window_main;
	
/*** Functions ***/

/**
 * WindowHandler called when the main window is loaded.
 * Creates the display layer and adds it to the window.
 *
 * @param Window *a_window_main: The window that has just been pushed to the screen.
 */
static void main_window_load(Window *a_window_main) {
	layer_add_child(window_get_root_layer(a_window_main), bars_create_layer());
}

/**
 * WindowHandler called when the main window is deinited.
 * Destroys the display layer.
 *
 * @param Window *a_window_main: The window that has just unloaded.
 */
static void main_window_unload(Window *a_window_main) {
	bars_destroy_layer();
}

/**
 * AppMessageInboxReceived callback for when an incoming message is received.
 * Reads either settings or temperature from the app message.
 *
 * @param DictionaryIterator *it: The dictionary iterator to the received message.
 * @param void *context: Pointer to application data set by app_message_set_context.
 */
static void inbox_received_callback(DictionaryIterator *it, void *context)
{		
	APP_LOG(APP_LOG_LEVEL_INFO, "Inbox received.");

	/* Read the settings if available. */
	Tuple *settings_mesage_tuple = dict_find(it, MESSAGE_KEY_SettingsMessage);
	if (settings_mesage_tuple) {
		if(settings_mesage_tuple->value->int16 > 0) {
			bars_handle_settings_received(it, window_main);
		}
	}

	/* Read the temperature if available. */
	Tuple *temperature_tuple = dict_find(it, MESSAGE_KEY_Temperature);
	if(temperature_tuple) {
		int current_temperature = temperature_tuple->value->int32;
		bars_handle_temperature_received(current_temperature);
	}
}

/**
 * AppMessageInboxDropped callback for when an incoming message is dropped.
 * Logs the reason.
 *
 * @param AppMessageResult reason: The reason why the message was dropped.
 * @param void *context: Pointer to application data set by app_message_set_context.
 */
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped: %d.", reason);	
}

/**
 * AppMessageOutboxSent callback after an outbound message has been sent.
 * Logs the success.
 *
 * @param DictionaryIterator *it: The dictionary iterator to the sent message.
 * @param void *context: Pointer to application data set by app_message_set_context.
 */
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success.");
}

/**
 * AppMessageOutboxFailed callback after an outbound message has not been sent successfully.
 * Logs the reason.
 *
 * @param DictionaryIterator *it: The dictionary iterator to the sent message.
 * @param AppMessageResult reason: The reason why the message was dropped.
 * @param void *context: Pointer to application data set by app_message_set_context.
 */
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed: %d.", reason);
}

/**
 * Initializes main window and AppMessage connection.
 */
static void init() {	
	/* Inbox size must be large in order to receive the settings from Clay.
	In version 1.1, 130 bytes and below would consitently be dropped on iPhone 
	and emulator with error APP_MSG_BUSY. Outbox size can be smaller, as the 
	only message that gets sent is the fetch weather request. */
	const int inbox_size = 256;
	const int outbox_size = 64;

	/* Open AppMessage connection and register callbacks. */
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_register_outbox_failed(outbox_failed_callback);

	app_message_open(inbox_size, outbox_size);

	/* Create the main window. */
	window_main = window_create();

	window_set_window_handlers(window_main, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	window_stack_push(window_main, true);		

	/* Initialize resources and data for the bars display. */
	bars_init(window_main);
}

/**
 * Cleans up data and destroys the main window.
 */
static void deinit() {
	bars_deinit();
	window_destroy(window_main);
}

/**
 * Initializes and deinitializes the app and enters the Pebble main event loop.
 */
int main(void) {
	init();
	app_event_loop();
	deinit();
}
