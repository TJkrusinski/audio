
#define MINIAUDIO_IMPLEMENTATION
#include "./miniaudio.h"

#include <stdlib.h>
#include <stdio.h>

#define AUDIO_SIZE 441

typedef struct {
  uint32_t sequence_number;
  uint64_t timestamp;
  uint8_t audio_data[AUDIO_SIZE];
} Audio_Packet;

int *bufferLoc; 
static int writeLocation = 0;
static int readLocation = 0;
static bool passed = false;


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  MA_ASSERT(pDevice->capture.format == pDevice->playback.format);
  MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);

  uint32_t sizeToAllocate;

  sizeToAllocate = frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels);

  writeLocation += sizeToAllocate;

  if (writeLocation > 300 * AUDIO_SIZE) {
    writeLocation = 0;
    printf("%i, write location: ", writeLocation);
  };

  if (writeLocation > (300 * AUDIO_SIZE - sizeToAllocate)) {
    passed = true;
  }

  memcpy((char *)bufferLoc + writeLocation, pInput, sizeToAllocate);

  if (passed) {
    memcpy(pOutput, (char *)bufferLoc + readLocation, sizeToAllocate);

    readLocation += sizeToAllocate;

    if (readLocation > 300 * AUDIO_SIZE) {
      readLocation = 0;
    };
  }
}

int main(int argc, char** argv)

{
    bufferLoc= malloc(300 * AUDIO_SIZE);

    if (bufferLoc == NULL) {
      printf("unable to allocate main buffer");
      exit(0);
    }

    ma_result result;

    ma_device_config deviceConfig;
    ma_device device;

    deviceConfig = ma_device_config_init(ma_device_type_duplex);

    deviceConfig.capture.pDeviceID = NULL;
    deviceConfig.capture.format    = ma_format_u8;
    deviceConfig.capture.channels  = 1;
    deviceConfig.capture.shareMode = ma_share_mode_shared;

    deviceConfig.playback.pDeviceID = NULL;
    deviceConfig.playback.format = ma_format_u8;
    deviceConfig.playback.channels = 1;

    deviceConfig.dataCallback     = data_callback;

    result = ma_device_init(NULL, &deviceConfig, &device);

    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    result = ma_device_start(&device);

    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");
    getchar();

    ma_device_uninit(&device);

    (void)argc;
    (void)argv;
    return 0;
}
