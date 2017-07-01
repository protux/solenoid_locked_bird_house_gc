# Birdhouse Geocache

## Used Software
The JavaScript piano was written by [mrcoles (github)](https://github.com/mrcoles/javascript-piano). It is licenced with the MIT Licence and was slightly modified by me.

## How it works
You need to choose a code which needs to be played on the piano to open the solenoid. The code needs to be defined in piano.js into the variable "code" and in piano\_server.ino in the defined constant "CODE\_OPEN". The code consists of the numeric id of the key and the note which is represented by this key.  Example code: "-12c-12c-12c" which is three times the most left key. Additionally you must define the code length in the variable "code\_length" in piano.js.

This software was written for an ESP8266. Open the project e.g. in the Arduino IDE, install the esp-tools, select your board and click "Tools > ESP8266 Sketch Data Upload". The files in "data" directory will be uploaded. After that click "Upload". Now the IDE flashes the Firmware to your ESP8266.
