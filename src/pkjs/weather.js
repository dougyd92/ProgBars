var openweathermap = require('./openweathermapkey');

/**
 * Gets weather data from the OpenWeatherMap API.
 *
 * @param pos: Position object with the current GPS coordinates.
 */
function getOpenWeatherData(pos) {
	/* Construct the URL. */
	var url = 'http://api.openweathermap.org/data/2.5/weather?' +
		'lat=' +pos.coords.latitude + 
		'&lon=' + pos.coords.longitude + 
		'&units=imperial' + 
		'&appid=' + openweathermap.api_key;

	var xhr = new XMLHttpRequest();

	xhr.onload = function() {
		/* On load, parse the temperature from the response and send it to the watch. */
		
		/* responseText will contain a JSON object with data info. */
		var json = JSON.parse(this.responseText);

		/* Assemble dictionary to send. */
		var dictionary = {
			'Temperature': json.main.temp
		};

		/* Send the message to the watch. */
		Pebble.sendAppMessage(dictionary, function(e) {
			console.log('Weather info sent to Pebble successfully.');
		}, function(e) {
			console.log('Error sending weather info to Pebble.');
		});
	};
	
	/* Send the request. */
	xhr.open('GET', url);
	xhr.send();
}

/**
 * Logs an error if GPS is unavailable.
 */
function locationError(err) {
	console.log('Error requesting location.');
}

/**
 * Gets the weather data for the current location. 
 */
function getWeather() {
	console.log('Getting weather data for current location.');

	/* Get the current position and, if successful, the weather data. */
	navigator.geolocation.getCurrentPosition(
		getOpenWeatherData,
		locationError,
		{timeout: 15000, maximumAge: 60000}
	);
}

module.exports.getWeather = getWeather;
