#include <Arduino.h>
#include <SD.h>
#include "config.h"
#include "audio_globals.h"
#include "audio_functions.h"

extern File audioFile;

enum State { IDLE, RECORDING, PLAYING };
State currentState = IDLE;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; 

bool lastButtonReading = HIGH;
bool buttonStableState = HIGH; 

unsigned long buttonDownTime = 0; 

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS); 

  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed!");
    while (true); 
  }
  Serial.println("SD Card initialized successfully.");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonStableState) { 
      buttonStableState = reading; 

      if (buttonStableState == LOW) {
        buttonDownTime = millis(); 
      } else { 
        unsigned long pressDuration = millis() - buttonDownTime; 

        if (currentState == RECORDING) {
          stopRecording();
          currentState = IDLE;
          Serial.println("Recording stopped"); 
        }
        else if (currentState == IDLE) {
          
          if (pressDuration > debounceDelay && pressDuration < 500) {
            if (SD.exists(FILE_NAME)) {
              startPlaying();
              currentState = PLAYING;
              Serial.println("Playback started"); 
            } else {
              Serial.println("No audio file to play");
            }
          }
        }
      }
    }
  }

  lastButtonReading = reading; 

  switch (currentState) {
    case IDLE:
      
      if (buttonStableState == LOW) {
        unsigned long heldTime = millis() - buttonDownTime;
        if (heldTime > 500) { 
          startRecording();
          currentState = RECORDING;
          Serial.println("Recording started (held)"); 
        }
      }
      break;

    case RECORDING:
      recordLoop();
      
      break;

    case PLAYING:
      playLoop();
      
      if (!audioFile || audioFile.available() == 0) { 
        stopPlaying();
        currentState = IDLE;
        Serial.println("Playback stopped"); 
      }
      break;
  }
}