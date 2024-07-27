#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define pin GPIO_NUM_15
#define freq 50
#define pulsemin 544
#define pulsemax 2400
#define duty_max 65535

void setup_ledc(){
  ledc_timer_config_t timer_conf = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_16_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = freq,
    .clk_cfg=LEDC_AUTO_CLK
  };

  ledc_timer_config(&timer_conf);

  ledc_channel_config_t channel_conf = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0 ,
    .timer_sel=LEDC_TIMER_0,
    .intr_type=INTR_DISABLE,
    .gpio_num=pin,
    .duty=0,
    .hpoint=0
  };

  ledc_channel_config(&channel_conf);
}

int to_duty(int pulse){
  return (float)pulse/20000 * duty_max;
}

void set_angle(int channel,int angle){
  if (angle > 180){
    angle=180;
  }
  else if (angle < 0){
    angle=0;
  }
  int pulse=pulsemin+(angle/180.0)*(pulsemax-pulsemin);
  int duty=to_duty(pulse);
  ledc_set_duty(LEDC_HIGH_SPEED_MODE,channel,duty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE,channel);
}


void app_main(void)
{
  setup_ledc();
  for(int i=0;i<=180;i++){
    set_angle(LEDC_CHANNEL_0,i);
    vTaskDelay(500/portTICKS_PERIOD_MS);
  }
}
