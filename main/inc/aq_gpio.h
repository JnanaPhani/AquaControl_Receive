#ifndef __GPIO_H_
#define __GPIO_H_

#include <esp_log.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <receiver.h>

extern bool gpio_status;

// GPIO State
#define ON                              (1)
#define OFF                             (0)

// Switch GPIO's
#define MANNUAL_SWITCH                  (27)        //SW1 - MANNUAL ON & OFF SWITCH
#define TRIGGER_REPROVISION_GPIO        (16)        //SW3 - REPROVISION SWITCH

// LED GPIO's
#define LED_01                          (22)        //RED    (WATER LEVEL INDICATIONN) -> (WATER LEVEL ABOVE 40% => OFF, WATER LEVEL BELOW 40% => ON)
#define LED_02                          (21)        //BLUE   (LoRa Connection) -> (CONNECTION OK => ON, CONNECTION NOT OK => OFF)
#define LED_03                          (19)        //BLUE   (NETWORK REGISTRATION) -> (UNPROVISIONED => OFF, PROVISIONED => ON)
#define LED_04                          (18)        //GREEN  (MOTOR STATE) - (MOTOR ON => TOOGLE, MOTOR OFF => OFF)

// Relay GPIO
#define RELAY                           (04)        //Relay PIN 

void aq_gpio_config(void);
void gpio_isr_handler(void* arg);
void handle_led(unsigned char led_no, unsigned char state);
void toggle_led(unsigned char led_no, unsigned char count, unsigned int delay);
void handle_relay(unsigned char state);

#endif /*__GPIO_H__*/