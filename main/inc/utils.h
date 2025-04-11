#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "esp_sleep.h"
#include "esp_wake_stub.h"
#include "driver/rtc_io.h"
#include "rtcwake.h"
#include <softap-provision.h>
#include <aq_gpio.h>
#include <rtcwake.h>
#include "lora.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <receiver.h>

#undef wifi
#undef deepsleep

#define WIFI_CONNECTED_EVENT_TIMEOUT    (3000/portTICK_PERIOD_MS)
#define WATER_TANK_SIZE                 (140)
#define WATER_LOWER_THRESHOLD           (30)
#define WATER_UPPER_THRESHOLD           (90)

#define TANK_SIZE_CM 200  // Example: 200 cm tank height

// // sleep_enter_time stored in RTC memory
// static RTC_DATA_ATTR struct timeval sleep_enter_time;

bool setup(void);
void loop(void);
void wifi_init_sta(void);
void wifi_init(void);   
void application_logic(void);
void deepsleep(void);
void recieve_lora_data(void);
void relay_on();
void relay_off();
void process_lora_data() ;
void process_gpios();


#endif