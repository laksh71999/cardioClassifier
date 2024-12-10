#include <dlogger.hpp>
#include <cstdint>
#include <main.h>
#include <AudioProcess.hpp>

Audio Aud;

uint8_t irqz = 0, incIn = 0;
extern uint8_t volume;
uint8_t bleIndicationCnt = 0;
extern uint8_t bleIndicationCntFlag, bleDataSend_Flag;

int main(void)
{
  bsp_main();
  HAL_Delay(10);
  Aud.playbackInit();
  Aud.saiDmaInit();

  while (1)
  {
    MX_APPE_Process();
    Aud.audioProcess();
    // Aud.audioProcess_movingBuffer();

    // Aud.filterProcess_audio(&Aud.PCM_OUT_Buff[0], &Aud.convertedAudio[incIn][0]);
    // memcpy(&Aud.PCM_OUT_Buff[0],&Aud.convertedAudio[incIn][0],976);
    // incIn++;
    // if (incIn >= 10)
    //   incIn = 0;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // runs every 5ms for audio
{
  bleIndicationCnt++;

  // if(bleDataSend_Flag == 1){
  //   Aud.bleDataSend();
  //   bleDataSend_Flag = 0;
  // }
  // else{}

  // if (bleDataSend_Flag == 1)
  // {
  //   Aud.filterProcess_audio(&Aud.PCM_OUT_Buff[0], &Aud.convertedAudio[incIn][0]);
  //   // memcpy(&Aud.PCM_OUT_Buff[0], &Aud.convertedAudio[incIn][0], 976);
  //   incIn++;
  //   if (incIn >= 10)
  //     incIn = 0;
  // }
  // else
  // {
  // }

  if ((bleIndicationCnt == 100) && (bleIndicationCntFlag == 1))
  { // blinks led for every 500ms - ble indication
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
    bleIndicationCnt = 0;
  }
  else
  {
  }
}