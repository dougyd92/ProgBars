/**
 * JSON used to generate the configuration page.
 */
module.exports = [
	{
		"type": "heading",
		"id": "title",
		"defaultValue": "Settings"
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Colors"
			},
			{
				"type": "color",
				"messageKey": "BackgroundColor",
				"defaultValue": "0x000000",
				"label": "Background Color"
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Bars"
			},
			{
				"type": "checkboxgroup",
				"id": "barCheckboxesGroup",
				"messageKey": "BarCheckboxes",
				"description": "Choose what information will be displayed. Note: Displaying seconds will reduce battery life.", 
				"defaultValue": [true, true, true, true, true, true, true, true, true],
				"options": ["Hours", "Minutes", "Seconds", "Day of week", "Month", "Day of month", "Temperature", "Steps", "Battery"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "More options"
			},
			{
				"type": "toggle",
				"id": "combineHourMinToggle",
				"messageKey": "CombineHourMin",
				"defaultValue": true,
				"label": "Combine Hours and Minutes onto one line"
			},	
			{
				"type": "toggle",
				"id": "combineMonthDayToggle",
				"messageKey": "CombineMonthDay",
				"defaultValue": true,
				"label": "Combine Month and Day onto one line"
			},	  			
			{
				"type": "radiogroup",
				"messageKey": "TemperatureScale",
				"defaultValue": "F",
				"label": "Temperature Scale",
				"options": [
					{ 
						"label": "Fahrenheit",
						"value": "F"
					},
					{ 
						"label": "Celsius",
						"value": "C"
					}
				]
			}
		]
	},
	{
		"type": "submit",
		"defaultValue": "Save"
	},
	{
		"type": "text",
		"defaultValue": "<center> <b>ProgBars</b> created by Douglas de Jesus. " +
			"<br/> Quesions? Bug reports? Feature requests? Please send me an " +
			"<a href=\"mailto:codedbydoug@gmail.com?Subject=ProgBars Pebble Watchface\">email</a> " +
			"or submit an issue on <a href =\"https://github.com/dougyd92/ProgBars\">GitGub</a>!" +
			"<br/>Configuration page made using <a href=\"https://github.com/pebble/clay\">Clay</a>." +
			"<br/>Weather data supplied by <a href=\"https://openweathermap.org/\">OpenWeatherMap.org</a>." +
			"<br/><a href=\"https://fonts.google.com/specimen/Oxygen+Mono\">Oxygen Mono</a> font used under the " +
			"<a href=\"http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL_web\">Open Font License</a>." +
			"</center><br/>"
	}
];
