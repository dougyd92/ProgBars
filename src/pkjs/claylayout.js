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
				"defaultValue": "Bars"
			},
			{
				"type": "checkboxgroup",
				"id": "barCheckboxesGroup",
				"messageKey": "BarCheckboxes",
				"description": "Choose what information will be displayed. Note: Displaying seconds will reduce battery life.", 
				"defaultValue": [false, false, true, true, true, false, false, true, true, true, true],
				"options": ["Hours", "Minutes", "Hours and Minutes Combined", "Seconds", "Day of week", 
							"Month", "Day of month", "Month and Day Combined", "Temperature", "Steps", "Battery"]
			}
		]
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
				"defaultValue": "0x000000", //GColorBlack
				"sunlight": false,
				"label": "Background Color",
				"allowGray": true
			},		
			{
				"type": "color",
				"messageKey": "TextColor",
				"defaultValue": "0xFFFFFF", //GColorWhite
				"sunlight": false,
				"label": "Text Color"
			},			
			{
				"type": "color",
				"messageKey": "TextOutlineColor",
				"defaultValue": "0x000000", //GColorBlack
				"sunlight": false,
				"label": "Text Outline Color"
			},				
			{
				"type": "color",
				"messageKey": "BarColors[0]",
				"defaultValue": "0x000055", //GColorOxfordBlue
				"sunlight": false,
				"label": "Hours Bar Color",
				"allowGray": true
			},		
			{
				"type": "color",
				"messageKey": "BarColors[1]",
				"defaultValue": "0x0000AA", //GColorDukeBlue
				"sunlight": false,
				"label": "Minutes Bar Color",
				"allowGray": true
			},
			{
				"type": "color",
				"messageKey": "BarColors[2]",
				"defaultValue": "0x0000AA", //GColorDukeBlue
				"sunlight": false,
				"label": "Hours and Minutes Combined Bar Color",
				"allowGray": true
			},				
			{
				"type": "color",
				"messageKey": "BarColors[3]",
				"defaultValue": "0x00AAFF", //GColorVividCerulean
				"sunlight": false,
				"label": "Seconds Bar Color",
				"allowGray": true
			},
			{
				"type": "color",
				"messageKey": "BarColors[4]",
				"defaultValue": "0xFFAA55", //GColorRajah
				"sunlight": false,
				"label": "Day of Week Bar Color",
				"allowGray": true
			},		
			{
				"type": "color",
				"messageKey": "BarColors[5]",
				"defaultValue": "0xAAAA55", //GColorBrass
				"sunlight": false,
				"label": "Month Bar Color",
				"allowGray": true
			},	
			{
				"type": "color",
				"messageKey": "BarColors[6]",
				"defaultValue": "0xFFFF00", //GColorYellow
				"sunlight": false,
				"label": "Day of Month Bar Color",
				"allowGray": true
			},	
			{
				"type": "color",
				"messageKey": "BarColors[7]",
				"defaultValue": "0xFFFF00", //GColorYellow
				"sunlight": false,
				"label": "Month and Day Combined Bar Color",
				"allowGray": true
			},				
			{
				"type": "color",
				"messageKey": "BarColors[8]",
				"defaultValue": "0x550055", //GColorImperialPurple
				"sunlight": false,
				"label": "Temperature Bar Color",
				"allowGray": true
			},	
			{
				"type": "color",
				"messageKey": "BarColors[9]",
				"defaultValue": "0x005500", //GColorDarkGreen
				"sunlight": false,
				"label": "Steps Bar Color",
				"allowGray": true
			},	
			{
				"type": "color",
				"messageKey": "BarColors[10]",
				"defaultValue": "0xFF0000", //GColorRed
				"sunlight": false,
				"label": "Battery Bar Color",
				"allowGray": true
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
				"type": "radiogroup",
				"messageKey": "BarStyle",
				"defaultValue": "S",
				"label": "Bar Style",
				"options": [
					{ 
						"label": "Solid",
						"value": "S"
					},
					{ 
						"label": "Outline",
						"value": "O"
					}
				]
			},			
			{
				"type": "radiogroup",
				"messageKey": "TemperatureScale",
				"id": "tempScaleRadio",
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
			},
			{
				"type": "slider",		
				"messageKey": "TemperatureMinF",		
				"id": "tempLowerBoundSliderF",		
				"defaultValue": 32,		
				"label": "Temperature Bar Lower Bound (\u00B0F)", //\u00B0 is the degree symbol
				"min": -100,		
				"max": 60,		
			},		
			{		
				"type": "slider",		
				"messageKey": "TemperatureMaxF",		
				"id": "tempUpperBoundSliderF",		
				"defaultValue": 100,		
				"label": "Temperature Bar Upper Bound (\u00B0F)",		
				"min": 70,		
				"max": 135,		
			},		
			{		
				"type": "slider",		
				"messageKey": "TemperatureMinC",		
				"id": "tempLowerBoundSliderC",		
				"defaultValue": 0,		
				"label": "Temperature Bar Lower Bound (\u00B0C)",		
				"min": -73,		
				"max": 15,		
			},		
			{		
				"type": "slider",		
				"messageKey": "TemperatureMaxC",		
				"id": "tempUpperBoundSliderC",		
				"defaultValue": 38,		
				"label": "Temperature Bar Upper Bound (\u00B0C)",		
				"min": 21,		
				"max": 57,		
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
			"or submit an issue on <a href =\"https://github.com/dougyd92/ProgBars\">GitHub</a>!" +
			"<br/>Configuration page made using <a href=\"https://github.com/pebble/clay\">Clay</a>." +
			"<br/>Weather data supplied by <a href=\"https://openweathermap.org\">OpenWeatherMap.org</a>." +
			"<br/><a href=\"https://fonts.google.com/specimen/Oxygen+Mono\">Oxygen Mono</a> font used under the " +
			"<a href=\"http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL_web\">Open Font License</a>." +
			"<br/>Footprints icon made by Freepik from <a href=\"http://www.flaticon.com\">flaticon.com</a>." +
			"</center><br/>"
	}
];
