/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "FS.h"

/* Set these to your desired credentials. */
#define SSID "O pulchram avem!"
#define LED_PIN 2 // Internal LED
#define SOLENOID_PIN 5 // Pin D1

#define ROOT_PATH "/"
#define OPEN_PATH "/open"
#define AUDIO_JS_PATH "/audio.js"
#define JQUERY_PATH "/jquery.js"
#define PIANO_JS_PATH "/piano.js"
#define STYLE_CSS_PATH "/piano.css"
#define INDEX_HTML_PATH "/index.html"
#define PIANO_PNG_PATH "/piano.png"
#define AUDIO_PATH "/mm.mp3"

#define CONTENT_TYPE_JS "application/javascript"
#define CONTENT_TYPE_HTML "text/html"
#define CONTENT_TYPE_CSS "text/css"
#define CONTENT_TYPE_PNG "image/png"
#define CONTENT_TYPE_MP3 "audio/mpeg"

#define CODE_ARG "code"
#define CODE_OPEN "<YOUR CODE>"
#define OPEN_DURATION 15000 // 15 seconds
#define SOLENOID_OPEN HIGH
#define SOLENOID_CLOSED LOW

ESP8266WebServer server(80);

String readFile(String path) {
  if (SPIFFS.exists(AUDIO_JS_PATH)) {
    File file = SPIFFS.open(path, "r");
    return file.readString();
  }

  return "";
}

void deliver(String path, String contentType) {
  File file = SPIFFS.open(path, "r");
  server.streamFile(file, contentType);
  file.close();
}

void deliverIndex() {
  String content = readFile(INDEX_HTML_PATH); 
  server.send(200, CONTENT_TYPE_HTML, content);
}

void deliverJavaScript(String path) {
  deliver(path, CONTENT_TYPE_JS);
}

void deliverAudioJS() {
  deliverJavaScript(AUDIO_JS_PATH);
}

void deliverJQuery() {
  deliverJavaScript(JQUERY_PATH);
}

void pianoJS() {
  deliverJavaScript(PIANO_JS_PATH);
}

void deliverCSS(String path) {
  deliver(path, CONTENT_TYPE_CSS);
}

void deliverPianoCSS() {
  deliverCSS(STYLE_CSS_PATH);
}

void deliverPianoImage() {
  deliver(PIANO_PNG_PATH, CONTENT_TYPE_PNG);
}

void deliverMorningMoodAudio() {
  deliver(AUDIO_PATH, CONTENT_TYPE_MP3);
}

void openCache() {
  Serial.println("open requested");
  if (server.method() == HTTP_POST) {
    for (uint8_t i = 0; i < server.args(); i++) {
      Serial.println("received code: " + server.arg(i));
      if (server.argName(i) == CODE_ARG && server.arg(i) == CODE_OPEN) {
        Serial.println("code is correct");
        digitalWrite(SOLENOID_PIN, SOLENOID_OPEN);
        delay(OPEN_DURATION);
        digitalWrite(SOLENOID_PIN, SOLENOID_CLOSED);
      }
    }
  }
}

void setup() {
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  pinMode(SOLENOID_PIN, OUTPUT);
  Serial.begin(115200);
  if (SPIFFS.begin()) {
    Serial.println("Filesystem successfully mounted");
  } else {
    Serial.println("Error while mounting filesystem.");
  }
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(SSID);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on(ROOT_PATH, deliverIndex);
  server.on(AUDIO_JS_PATH, deliverAudioJS);
  server.on(PIANO_JS_PATH, pianoJS);
  server.on(JQUERY_PATH, deliverJQuery);
  server.on(STYLE_CSS_PATH, deliverPianoCSS);
  server.on(PIANO_PNG_PATH, deliverPianoImage);
  server.on(AUDIO_PATH, deliverMorningMoodAudio);
  server.on(OPEN_PATH, openCache);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
