#ifndef __AUDIOPROCESS_HPP_
#define __AUDIOPROCESS_HPP_

// #include "arm_math.h"

#define AUDIO_DRV 0x4C
#define AUDIO_BUFFER_SIZE 488
#define NUM_STAGES 4

class Audio
{
public:
    void playbackInit(void);
    void audioProcess(void);
    void bleDataSend(void);
    void saiDmaInit(void);
    void filterProcess_audio(int16_t *buffOut, int16_t *buffIn);
    // void audioProcess_movingBuffer(void);
    // void i2cRead(uint8_t val1, uint8_t len);
    // void i2cWrite(uint8_t val1, uint8_t val2, uint8_t len);
    // uint8_t Rx_Data[5];
    int16_t PCM_OUT_Buff[AUDIO_BUFFER_SIZE], convertedAudio[AUDIO_BUFFER_SIZE];
    int16_t coeffs[6 * NUM_STAGES];

    // q15_t biquadCoeffs[5 * NUM_STAGES] = {
    //     // Adjusting as per corrected interpretation, ensure values are within the Q15 range
    //     0, 0, 0, 0, -64155,                  // First stage coefficients (seems to be an error with one too many initializers here)
    //     32767, 0, 32767, -64843, 32123,      // Second stage coefficients (corrected, assumed 0 for missing b1 coefficient)
    //     32767, -32767, 32767, -65013, 32250, // Third stage coefficients
    //     32767, -32767, 32767, -65394, 32629  // Fourth stage coefficients
    // };
    // q15_t biquadState[4 * NUM_STAGES] = {0};

private:
    uint8_t lowerData, upperData, Tx_Data[5], Rx_Data[5];
    int16_t ble_buff[AUDIO_BUFFER_SIZE / 4]; // PCM_OUT_Buff[AUDIO_BUFFER_SIZE], convertedAudio[10][AUDIO_BUFFER_SIZE];
    int32_t PCM_IN_Buff[AUDIO_BUFFER_SIZE];
    volatile uint8_t head{0};
    volatile int16_t bellMicData{0}, ancMicData{0}, filterData{0}, ancData{0};

    void copyBuffer(int16_t *Buff_Out, int32_t *Buff_In, int16_t *Ble, int16_t *convPcm, uint16_t size);
    void i2cWrite(uint8_t val1, uint8_t val2, uint8_t len);
    void i2cRead(uint8_t val1, uint8_t len);
    void volumeFunction(void);
};

#endif