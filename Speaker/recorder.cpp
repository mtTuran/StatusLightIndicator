#include <Arduino.h>
#include <driver/i2s.h>
#include <SD.h>
#include "config.h"
#include "audio_globals.h"

#define I2S_NUM I2S_NUM_0 

File audioFile;.
bool i2sDriverInstalled = false; 

i2s_config_t i2s_config_record = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
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

i2s_pin_config_t i2sMicPins = {
    .bck_io_num = I2S_MIC_SCK,
    .ws_io_num = I2S_MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SD
};

void startRecording() {
  Serial.println("Starting recording setup...");
  if (i2sDriverInstalled) {
    Serial.println("I2S driver already installed, uninstalling...");
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    delay(20);
  }

  esp_err_t err = i2s_driver_install(I2S_NUM, &i2s_config_record, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver for recording: %d\n", err);
    return;
  }
  i2s_set_pin(I2S_NUM, &i2sMicPins);
  i2sDriverInstalled = true;
  delay(20);

  audioFile = SD.open(FILE_NAME, FILE_WRITE);
  if (!audioFile) {
    Serial.println("Failed to open file for recording");
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    return;
  }
  Serial.println("Recording setup complete.");
}

void stopRecording() {
  Serial.println("Stopping recording...");
  if (audioFile) {
    audioFile.close();
  }
  if (i2sDriverInstalled) {
    i2s_driver_uninstall(I2S_NUM);
    i2sDriverInstalled = false;
    delay(20);
  }
  Serial.println("Recording stopped.");
}

void recordLoop() {
  const int bufferSize = 1024;
  int16_t i2sData[bufferSize / sizeof(int16_t)];

  size_t bytesRead;
  i2s_read(I2S_NUM, (void*)i2sData, bufferSize, &bytesRead, portMAX_DELAY);

  if (audioFile && bytesRead > 0) {
    int amplification_factor = 32;
    for (int i = 0; i < bytesRead / sizeof(int16_t); i++) {
      int32_t amplifiedValue = (int32_t)i2sData[i] * amplification_factor;
      if (amplifiedValue > 32767) amplifiedValue = 32767;
      else if (amplifiedValue < -32768) amplifiedValue = -32768;
      i2sData[i] = (int16_t)amplifiedValue;
    }

    audioFile.write((uint8_t*)i2sData, bytesRead);

  
  }
}