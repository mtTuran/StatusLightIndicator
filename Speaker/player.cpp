#include <Arduino.h>
#include <driver/i2s.h>
#include <SD.h>
#include "config.h"
#include "audio_globals.h"

#define I2S_NUM I2S_NUM_0 

extern File audioFile;
extern bool i2sDriverInstalled; 

i2s_config_t i2s_config_play = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

i2s_pin_config_t i2sSpkPins = {
    .bck_io_num = I2S_SPK_SCK,  
    .ws_io_num = I2S_SPK_WS,    
    .data_out_num = I2S_SPK_SD, 
    .data_in_num = I2S_PIN_NO_CHANGE
};

void startPlaying() {
  Serial.println("Starting playback setup...");
  if (i2sDriverInstalled) {
    Serial.println("I2S driver already installed, uninstalling...");
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    delay(20); 
  }

  esp_err_t err = i2s_driver_install(I2S_NUM, &i2s_config_play, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver for playback: %d\n", err);
    return;
  }
  i2s_set_pin(I2S_NUM, &i2sSpkPins);
  i2sDriverInstalled = true; 
  delay(20); 

  audioFile = SD.open(FILE_NAME, FILE_READ);
  if (!audioFile) {
    Serial.println("Failed to open file for playback");
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    return;
  }
  Serial.println("Playback setup complete.");
}

void stopPlaying() {
  Serial.println("Stopping playback...");
  if (audioFile) {
    audioFile.close();
  }
  if (i2sDriverInstalled) {
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    delay(20); 
  }
  Serial.println("Playback stopped.");
}

void playLoop() {
  const int bufferSize = 1024;
  uint8_t i2sData[bufferSize];

  if (!audioFile) return;

  size_t bytesRead = audioFile.read(i2sData, bufferSize);

  if (bytesRead > 0) {
    size_t bytesWritten;
    i2s_write(I2S_NUM, i2sData, bytesRead, &bytesWritten, portMAX_DELAY);
  } else {
  }
}