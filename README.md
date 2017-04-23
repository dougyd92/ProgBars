# ProgBars
ProgBars is watchface app for the Pebble smartwatch.

ProgBars displays all the information you would ever need on a watch in a compact, vibrant, easy to read fashion. 
It includes time and date, temperature, step count, and battery life. 
ProgBars can be customized to show only exactly the information you want to see!

The app consists of two components with bidirectional communication:

* A C application running natively on the watch itself using the Pebble C API. This holds most of the runtime logic and is responsible for rendering the display and reading time, health, and battery data from the watch.

* A JavaScript component running on the phone within the Pebble mobile app. This is written using the PebbleKit JS framework. The JavaScript component creates the dynamic configuration page and sends the user preferences to the watch. It also accesses the web through the phone's connection in order to retrieve weather information.

https://apps.getpebble.com/applications/58ce72e56ca3876a5e001569

<img src="http://i.imgur.com/SGeGW1w.png" alt="Watchface app" width="250">
<img src="http://i.imgur.com/FuEOn4M.png" alt="Configuration page" width="480">
