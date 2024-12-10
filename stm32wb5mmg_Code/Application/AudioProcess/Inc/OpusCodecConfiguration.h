#ifndef __OPUSCODECCONFIG_HPP_
#define __OPUSCODECCONFIG_HPP_

#define FRAME_SIZE 960
#define MS_FRAME 20
#define SAMPLE_RATE 16000
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_AUDIO
#define BITRATE 256000
// #define MAX_FRAME_SIZE 
#define MAX_ENCODED_FRAME_SIZE (244)

uint8_t opusEncodeCreate();
uint8_t opusBitrateSet();
uint8_t opusEncodeFrame(int16_t *pcmData, uint8_t *opusData_out);

#endif