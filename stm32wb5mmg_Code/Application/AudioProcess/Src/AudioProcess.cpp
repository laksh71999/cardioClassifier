#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include <main.h>
#include <AudioProcess.hpp>
#include <tas2562_params.hpp>
#include <LowpassFilter.hpp>
// #include <BandpassFilter.h>
// #include "filter.h"
#include <LMS.hpp>

extern I2C_HandleTypeDef hi2c1;

extern IPCC_HandleTypeDef hipcc;

extern RTC_HandleTypeDef hrtc;

extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;

extern uint8_t NotifyCharData[10][244], fullReady;

extern uint8_t state, volume;

extern bool halfFill, fullFill, volUpFlag, ble_recFlag, volDownFlag, NotificationTransferReq, DtFlowStatus;

// extern int32_t PCM_IN_Buff[AUDIO_BUFFER_SIZE], PCM_CPY_Buff[20][AUDIO_BUFFER_SIZE/2];

uint8_t cntTest{0}, bleDataSend_Flag{0}, loopEven{0}, opusOut[MAX_ENCODED_FRAME_SIZE], inc{0};

char buffer[50];

// vTitan::iirButterworth::ButterworthBandPass<4> bandpass;
vTitan::ButterworthLowPass<4> lowpass;
LMSClass lms;

// arm_biquad_casd_df1_inst_q15 biquadInstance;

void Audio::saiDmaInit()
{
  if (HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)&PCM_IN_Buff, AUDIO_BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)&PCM_OUT_Buff, AUDIO_BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}

void Audio::i2cWrite(uint8_t val1, uint8_t val2, uint8_t len)
{
  uint8_t ret{0};
  Tx_Data[0] = val1;
  Tx_Data[1] = val2;
  ret = HAL_I2C_Master_Transmit(&hi2c1, AUDIO_DRV << 1, &Tx_Data[0], len, HAL_MAX_DELAY);
  if (ret != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
  }
}

void Audio::i2cRead(uint8_t val1, uint8_t len)
{
  uint8_t ret{0};
  Tx_Data[0] = val1;
  ret = HAL_I2C_Master_Transmit(&hi2c1, AUDIO_DRV << 1, &Tx_Data[0], len, HAL_MAX_DELAY);
  if (ret != HAL_OK)
    Error_Handler();
  ret = HAL_I2C_Master_Receive(&hi2c1, AUDIO_DRV << 1, &Rx_Data[0], len, HAL_MAX_DELAY);
  if (ret != HAL_OK)
    Error_Handler();
}

void Audio::playbackInit(void)
{
  i2cWrite(PAGE, PAGE_SET0, 2);
  i2cWrite(BOOK, BOOK_SET, 2);
  i2cWrite(SW_RESET, SWRE_SET, 2);
  HAL_Delay(10);

  i2cWrite(PB_CFG1, PB_CFG0_SET, 2);
  i2cWrite(TDM_CFG0, TDM_CFG0_SET, 2);
  i2cWrite(TDM_CFG1, TDM_CFG1_SET, 2);
  i2cWrite(TDM_CFG2, TDM_CFG2_SET, 2);
  i2cWrite(PWR_CTL, PWR_CTL_RESET, 2);

  i2cRead(PWR_CTL,1);
  // SEGGER_RTT_printf(0,"TAS2652 Init \nPWR_CTL = %d\n",Rx_Data[0]);
  i2cRead(INT_LTCH0, 1);
  // SEGGER_RTT_printf(0,"int_ltch = %d\n",Rx_Data[0]);

  i2cWrite(0X03, volume << 1, 2);

  /* Filter Setup */
  lowpass.Setup(16000,120);
  // bandpass.Setup(16000, 60, 80); // 20-100
  bleDataSend_Flag = 1;
}

void Audio::volumeFunction(void)
{
  if ((true == volUpFlag) || (true == volDownFlag))
  {
    i2cWrite(0X03, volume << 1, 2);
    if (true == volUpFlag)
      volUpFlag = false;
    else
      volDownFlag = false;
  }
  else
  {
  }
  // SEGGER_RTT_printf(0,"Volume = %d\n",volume);
}

void Audio::copyBuffer(int16_t *Buff_Out, int32_t *Buff_In, int16_t *Ble, int16_t *convPcm, uint16_t size)
{
  volumeFunction();

  cntTest++;

  // arm_biquad_cascade_df1_init_q15(&biquadInstance, NUM_STAGES, biquadCoeffs, biquadState, 1);

  SEGGER_RTT_Write(0, "{", 1);

  for (int i = 0, k = 0; i < size; i += 2, k++)
  {
    bellMicData = Buff_In[i] >> 16;
    ancMicData = Buff_In[i + 1] >> 16;

    filterData = lowpass.filter((int16_t)bellMicData);  //without ANC
    // filterData = bandpass.filter((int16_t)bellMicData);

    SEGGER_RTT_printf(0, "%d,%d,%d ", bellMicData, cntTest, i);

    // ancData = lms.computeLMS(filterData, ancMicData); // LMS for ANC

    // if (ble_recFlag==1){
    //   Buff_Out[i + 1] = ancData;
    // }
    // else{
    //   Buff_Out[i + 1] = filterData;
    // }

    Buff_Out[i + 1] = bellMicData;
    // convPcm[i+1] = bellMicData;
    Ble[k] = bellMicData;
  }  

  // arm_biquad_cascade_df1_q15(&biquadInstance, convPcm, Buff_Out, AUDIO_BUFFER_SIZE/2);

  SEGGER_RTT_Write(0, "}\n", 2);

  bleDataSend();
  // bleDataSend_Flag = 1;

  if(cntTest==10)
    cntTest = 0;
}

// void Audio::audioProcess_movingBuffer(void){
//   volumeFunction();

//   cntTest++;

//   if (fullReady == 1){

//   SEGGER_RTT_Write(0, "{", 1);

//   for (int i = 0, k = 0; i < 244; i += 2, k++)
//   {
//     bellMicData = PCM_CPY_Buff[inc][i] >> 16;
//     ancMicData = PCM_CPY_Buff[inc][i + 1] >> 16;

//     filterData = lowpass.filter((int16_t)bellMicData);  //without ANC
//     // filterData = bandpass.filter((int16_t)bellMicData);

//     SEGGER_RTT_printf(0, "%d,%d,%d ", bellMicData, filterData, i);

//     // ancData = lms.computeLMS(filterData, ancMicData); // LMS for ANC

//     // if (ble_recFlag==1){
//     //   Buff_Out[i + 1] = ancData;
//     // }
//     // else{
//     //   Buff_Out[i + 1] = filterData;
//     // }

//     ble_buff[k] = bellMicData;
//     PCM_OUT_Buff[i + 1] = bellMicData;
//   }

//   SEGGER_RTT_Write(0, "}\n", 2);

//   // bleDataSend();
//   bleDataSend_Flag = 1;

//   if(inc >= 20)
//     inc = 0;
//   }
// }

// void Audio::filterProcess_audio(int16_t *buffOut, int16_t *buffIn){
//   for (int i = 0; i < AUDIO_BUFFER_SIZE; i++){
//     buffOut[i] = lowpass.filter((int16_t)buffIn[i]);;
//   }
// }

void Audio::bleDataSend(void)
{
  opusEncodeFrame(ble_buff, opusOut);
  memcpy(NotifyCharData[head++], opusOut, sizeof(opusOut));

  // Copying data to notification array
  // memcpy(NotifyCharData[head++], ble_buff, sizeof(ble_buff));

  // BLE Notification
  if (DtFlowStatus != DTS_APP_FLOW_OFF)
  {
    UTIL_SEQ_SetTask(1 << CFG_TASK_AUDIO_STREAM_ID, CFG_SCH_PRIO_0);
  }

  if (head >= 10)
    head = 0;
}

void Audio::audioProcess(void)
{
  switch (state)
  {
  case 0:
    if (halfFill == true)
    {
      copyBuffer(&PCM_OUT_Buff[0], &PCM_IN_Buff[0], &ble_buff[0], &convertedAudio[0], AUDIO_BUFFER_SIZE / 2);

      halfFill = false;
      state = 1;
    }
    break;
  case 1:
    if (fullFill == true)
    {
      copyBuffer(&PCM_OUT_Buff[AUDIO_BUFFER_SIZE / 2], &PCM_IN_Buff[AUDIO_BUFFER_SIZE / 2], &ble_buff[0], &convertedAudio[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);

      fullFill = false;
      state = 0;
    }
    break;
  }
}