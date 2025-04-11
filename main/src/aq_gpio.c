#include <aq_gpio.h>
#include <receiver.h>

extern char *TAG;
unsigned char trigger_switch = 0;
unsigned char trigger_reprovision = 1;  
bool gpio_status = false;

void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    switch(gpio_num) {
        case TRIGGER_REPROVISION_GPIO:
            trigger_reprovision = 1;
        break;
        case MANNUAL_SWITCH:
            trigger_switch = !trigger_switch;
        break;
    }
}

void aq_gpio_config(void)
{
    /*MANNUAL_SWITCH*/
    gpio_reset_pin(MANNUAL_SWITCH);
    gpio_set_direction(MANNUAL_SWITCH, GPIO_MODE_INPUT);
    gpio_pullup_dis(MANNUAL_SWITCH);
    gpio_pulldown_en(MANNUAL_SWITCH);
    gpio_set_intr_type(MANNUAL_SWITCH, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(MANNUAL_SWITCH, gpio_isr_handler, (void*)MANNUAL_SWITCH);
    gpio_intr_enable(MANNUAL_SWITCH);

    /*TRIGGER_REPROVISION_GPIO*/
    gpio_reset_pin(TRIGGER_REPROVISION_GPIO);
    gpio_set_direction(TRIGGER_REPROVISION_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_dis(TRIGGER_REPROVISION_GPIO);
    gpio_pulldown_en(TRIGGER_REPROVISION_GPIO);
    gpio_set_intr_type(TRIGGER_REPROVISION_GPIO, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(TRIGGER_REPROVISION_GPIO, gpio_isr_handler, (void*)TRIGGER_REPROVISION_GPIO);
    gpio_intr_enable(TRIGGER_REPROVISION_GPIO);

    /*LED GPIO CONFIGURATION*/
    gpio_reset_pin(LED_01);
    gpio_set_direction(LED_01, GPIO_MODE_OUTPUT);
    gpio_pullup_en(LED_01);
    gpio_pulldown_dis(LED_01);

    gpio_reset_pin(LED_02);
    gpio_set_direction(LED_02, GPIO_MODE_OUTPUT);
    gpio_pullup_en(LED_02);
    gpio_pulldown_dis(LED_02);

    gpio_reset_pin(LED_03);
    gpio_set_direction(LED_03, GPIO_MODE_OUTPUT);
    gpio_pullup_en(LED_03);
    gpio_pulldown_dis(LED_03);

    gpio_reset_pin(LED_04);
    gpio_set_direction(LED_04, GPIO_MODE_OUTPUT);
    gpio_pullup_en(LED_04);
    gpio_pulldown_dis(LED_04);

    gpio_status = true;
}

void handle_led(unsigned char led_no, unsigned char state)
{
    gpio_set_level(led_no, state);
}

void toggle_led(unsigned char led_no, unsigned char count, unsigned int delay)
{
    unsigned int i = 0;
    for(i=0;i<count;i++) {
        handle_led(led_no, ON);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        handle_led(led_no, OFF);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}

void handle_relay(unsigned char state)
{
    gpio_set_level(RELAY, state);
}
