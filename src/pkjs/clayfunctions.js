/**
 * Custom code that is injected into the configuration page generated by Clay.
 */
module.exports = function(minified) {	
	var clayConfig = this;
	
	var HOURS_BAR_IDX = 0;
	var MINUTES_BAR_IDX = 1;
	var COMBINED_HOURS_MINUTES_BAR_IDX = 2;
	var MONTH_BAR_IDX = 5;
	var DAY_BAR_IDX = 6;
	var COMBINED_MONTH_DAY_BAR_IDX = 7;
	var TEMPERATURE_BAR_IDX = 8;
	var TOTAL_BARS = 11;
		
	var barCheckboxesSaved;
	
	/**
	 * Shows or hides the options as needed based on which bars are enabled.
	 */
	function handleBarCheckboxesChanged() {
		var i;
		var barCheckboxesNew = this.get();
		var messageKey ;
		var colorPicker;
		
		/* Make sure that the combo bars (e.g. "Combined Hours and Minutes")
		and the corresponding individuals bars (e.g. "Hours" and "Minutes")
		are not both enabled. */
		if ( (barCheckboxesNew[HOURS_BAR_IDX] || barCheckboxesNew[MINUTES_BAR_IDX]) && 
			barCheckboxesNew[COMBINED_HOURS_MINUTES_BAR_IDX]) {
			/* Compare the previous settings to the new settings to see which option was
			changed; whatever was changed most recently will take priority. */
			if (barCheckboxesSaved[HOURS_BAR_IDX] || barCheckboxesSaved[MINUTES_BAR_IDX]) {
				barCheckboxesNew[HOURS_BAR_IDX] = false;
				barCheckboxesNew[MINUTES_BAR_IDX] = false;
			}
			else {
				barCheckboxesNew[COMBINED_HOURS_MINUTES_BAR_IDX] = false;
			}
		} 
		
		if ( (barCheckboxesNew[MONTH_BAR_IDX] || barCheckboxesNew[DAY_BAR_IDX]) && 
			barCheckboxesNew[COMBINED_MONTH_DAY_BAR_IDX]) {
			/* Compare the previous settings to the new settings to see which option was
			changed; whatever was changed most recently will take priority. */
			if (barCheckboxesSaved[MONTH_BAR_IDX] || barCheckboxesSaved[DAY_BAR_IDX]) {
				barCheckboxesNew[MONTH_BAR_IDX] = false;
				barCheckboxesNew[DAY_BAR_IDX] = false;
			}
			else {
				barCheckboxesNew[COMBINED_MONTH_DAY_BAR_IDX] = false;
			}
		} 
		
		this.set(barCheckboxesNew);
		barCheckboxesSaved = barCheckboxesNew;
		
		/* Show or hide the color pickers for each bar. */
		for (i = 0; i < TOTAL_BARS; ++i) {
			messageKey = 'BarColors[n]'.replace('n', i);
			colorPicker = clayConfig.getItemByMessageKey(messageKey);
			if (barCheckboxesNew[i]) {
				colorPicker.show();
			}
			else {
				colorPicker.hide();
			}
		}
		
		/* Show or hide the Temperature options. */
		if (barCheckboxesNew[TEMPERATURE_BAR_IDX]) {
			showTemperatureOptions();
		}
		else {
			hideTemperatureOptions();
		}
	}	
	
	/**		
	 * Keeps the two sets of temperature bounds sliders in sync.
	 */		
	function syncTemperatureSliders() {
		var tempLowerBoundSliderF = clayConfig.getItemById('tempLowerBoundSliderF');		
		var tempUpperBoundSliderF = clayConfig.getItemById('tempUpperBoundSliderF');		
		var tempLowerBoundSliderC = clayConfig.getItemById('tempLowerBoundSliderC');		
		var tempUpperBoundSliderC = clayConfig.getItemById('tempUpperBoundSliderC');			
		var tempScaleRadio = clayConfig.getItemById('tempScaleRadio');
		
		var tempScale = tempScaleRadio.get();
		
		var tempLowerBoundF;
		var tempUpperBoundF;
		var tempLowerBoundC;
		var tempUpperBoundC;

		if (tempScale == "F") {	
			tempLowerBoundF = tempLowerBoundSliderF.get();
			tempUpperBoundF = tempUpperBoundSliderF.get();
				
			tempLowerBoundSliderC.set((tempLowerBoundF - 32) / 1.8);
			tempUpperBoundSliderC.set((tempUpperBoundF - 32) / 1.8);
		}
		else if (tempScale == "C"){
			tempLowerBoundC = tempLowerBoundSliderC.get();
			tempUpperBoundC = tempUpperBoundSliderC.get();
			
			tempLowerBoundSliderF.set(tempLowerBoundC * 1.8 + 32);
			tempUpperBoundSliderF.set(tempUpperBoundC * 1.8 + 32);			
		}
	}
	
	/**
	 * Shows the Temperature Scale option and the correct sliders. 
	 */
	function showTemperatureOptions() {
		var tempLowerBoundSliderF = clayConfig.getItemById('tempLowerBoundSliderF');		
		var tempUpperBoundSliderF = clayConfig.getItemById('tempUpperBoundSliderF');		
		var tempLowerBoundSliderC = clayConfig.getItemById('tempLowerBoundSliderC');		
		var tempUpperBoundSliderC = clayConfig.getItemById('tempUpperBoundSliderC');		

		var tempScaleRadio = clayConfig.getItemById('tempScaleRadio');	
		
		var tempScale = tempScaleRadio.get();
		
		tempScaleRadio.show();		
		
		if (tempScale == "F") {	
			tempLowerBoundSliderF.show();
			tempUpperBoundSliderF.show();
			tempLowerBoundSliderC.hide();
			tempUpperBoundSliderC.hide();
		}
		else if (tempScale == "C"){
			tempLowerBoundSliderF.hide();
			tempUpperBoundSliderF.hide();			
			tempLowerBoundSliderC.show();
			tempUpperBoundSliderC.show();
		}	
	}
	
	/**
	 * Hides the Temperature Scale option and the temperature boundary sliders. 
	 */	
	function hideTemperatureOptions() {
		var tempLowerBoundSliderF = clayConfig.getItemById('tempLowerBoundSliderF');		
		var tempUpperBoundSliderF = clayConfig.getItemById('tempUpperBoundSliderF');		
		var tempLowerBoundSliderC = clayConfig.getItemById('tempLowerBoundSliderC');		
		var tempUpperBoundSliderC = clayConfig.getItemById('tempUpperBoundSliderC');		

		var tempScaleRadio = clayConfig.getItemById('tempScaleRadio');	
		
		tempLowerBoundSliderF.hide();
		tempUpperBoundSliderF.hide();
		tempLowerBoundSliderC.hide();
		tempUpperBoundSliderC.hide();
		tempScaleRadio.hide();	
	}	
	
	/* Initialize the page and add all the event handlers.. */
	clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
		var barCheckboxesGroup = clayConfig.getItemById('barCheckboxesGroup');
		var tempScaleRadio = clayConfig.getItemById('tempScaleRadio');			
		var tempLowerBoundSliderF = clayConfig.getItemById('tempLowerBoundSliderF');		
		var tempUpperBoundSliderF = clayConfig.getItemById('tempUpperBoundSliderF');		
		var tempLowerBoundSliderC = clayConfig.getItemById('tempLowerBoundSliderC');		
		var tempUpperBoundSliderC = clayConfig.getItemById('tempUpperBoundSliderC');	
		
		barCheckboxesSaved = barCheckboxesGroup.get();

		handleBarCheckboxesChanged.call(barCheckboxesGroup);
		barCheckboxesGroup.on('change', handleBarCheckboxesChanged);

		tempScaleRadio.on('change', showTemperatureOptions);		
		tempLowerBoundSliderF.on('change', syncTemperatureSliders);	
		tempUpperBoundSliderF.on('change', syncTemperatureSliders);	
		tempLowerBoundSliderC.on('change', syncTemperatureSliders);	
		tempUpperBoundSliderC.on('change', syncTemperatureSliders);	
	});
};
