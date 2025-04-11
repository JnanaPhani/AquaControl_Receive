#include <utils.h>

// struct timeval tv;
extern char *TAG;
/* Signal Wi-Fi events on this event-group */
const int WIFI_CONNECTED_EVENT = BIT0;
EventGroupHandle_t wifi_event_group;

bool lora_connected;
extern bool wifi_connection_status;
extern bool gpio_status;
uint8_t buf[32];

int water_level_percentage;
extern unsigned char trigger_reprovision;
extern unsigned char trigger_switch;
unsigned char previous_state = 0;
unsigned char relay_state = 0;

// Declare system status global
int8_t system_status = 0;

bool setup(void)
{
    system_status = 0; // Reset at startup

    bool lora_status = lora_init();
    if (lora_status) {
        ESP_LOGI(TAG, "LoRa Initialised Successfully");
        lora_connected = true;
        system_status |= (1 << 1); // Set bit 1 for LoRa
    } else {
        ESP_LOGE(TAG, "LoRa Initialisation Failed");
        lora_connected = false;
    }

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    // GPIO Config
    aq_gpio_config();
    toggle_led(LED_01, 5, 100);
    toggle_led(LED_02, 5, 100);
    toggle_led(LED_03, 5, 100);
    toggle_led(LED_04, 5, 100);

    if (gpio_status) {
        system_status |= (1 << 0); // Bit 0 = GPIO
    } else {
        ESP_LOGE(TAG, "GPIOs are not initialised properly! Check connections");
    }
    #ifdef wifi
    // WiFi Init
    wifi_init();

    bool provision_status = provisioning_init();

    if (!provision_status) {
        handle_led(LED_01, OFF);
        do_provisioning();
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_EVENT, true, true, portMAX_DELAY);
    } else {
        ESP_LOGI(TAG, "Already provisioned, starting Wi-Fi STA");
        wifi_prov_mgr_deinit();
        wifi_init_sta();
        ESP_LOGI(TAG, "Waiting for Wi-Fi connection...");
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_EVENT, true, true, WIFI_CONNECTED_EVENT_TIMEOUT);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
    #endif
    handle_led(LED_02, ON);

     if (wifi_connection_status) {
        system_status |= (1 << 2); // Bit 2 = Wi-Fi
    } else {
        ESP_LOGE(TAG, "Wi-Fi is not connected / initialised");
    }

    if (system_status == 0b111) {
        ESP_LOGI(TAG, "Initialisation Success");
    } else {
        ESP_LOGE(TAG, "Initialisation Completed with Warnings. Status: 0x%02X", system_status);
    }
    bool success = (system_status == 0b111);
    return success;
}

void wifi_init_sta(void)
{
    /* Start Wi-Fi in station mode */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void wifi_init(void)
{
    /* Initialize TCP/IP */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Initialize the event loop */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_event_group = xEventGroupCreate();

    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &prov_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(PROTOCOMM_SECURITY_SESSION_EVENT, ESP_EVENT_ANY_ID, &prov_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &prov_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &prov_event_handler, NULL));

    /* Initialize Wi-Fi including netif with default config */
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void loop(void)
{

    unsigned long start_time = esp_timer_get_time();  // Start time in microseconds
    unsigned long elapsed_time = 0;

    while(1) {
        // Calculate elapsed time in seconds
        elapsed_time = (esp_timer_get_time() - start_time) / 1000000; 

        if (elapsed_time < 100) {
            #ifdef wifi
            // Run the remaining logic (e.g., LED toggling)
            ESP_LOGI(TAG, "Running for %d seconds", (int)elapsed_time);
            if(trigger_reprovision==1) {
                ESP_LOGI(TAG,"RESETTING PROVISIONING");
                toggle_led(LED_03, 5, 100);
                wifi_prov_mgr_reset_provisioning();
                trigger_reprovision =0;
                esp_restart();
            }
            if(trigger_switch != previous_state){
                relay_state = !relay_state;
                (relay_state == 1)? relay_on() : relay_off();
                handle_relay(relay_state);
                ESP_LOGI(TAG, "Manual override triggered. Relay state: %d", relay_state);
                previous_state = trigger_switch;
            }
            #endif
            // Example of toggling LED or other logic
            // toggle_led(LED_01, 10, 100);  // Placeholder for other functions
            
            // Sleep for a short time to avoid overwhelming the CPU
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1-second delay between operations

            if(lora_connected)
            {
                recieve_lora_data();
                process_lora_data();
                process_gpios();
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            else if(!(lora_connected) && elapsed_time > 60)
            {
                handle_led(LED_01, OFF);
                ESP_LOGE(TAG, "LoRa Initialising Error!!  Restarting ESP");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                // esp_restart();
            }
        }
        // Since this is reciever code deepsleep is commented out
        // else
        // {
        //     // After 30 seconds, go to deep sleep
        //     ESP_LOGI(TAG, "30 seconds elapsed, entering deep sleep...");
        //     deepsleep();
        // }
    }
}
#ifdef deepsleep
void deepsleep(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    const int wakeup_time_sec = CONFIG_WAKE_UP_TIME;
    ESP_LOGI(TAG,"Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    esp_set_deep_sleep_wake_stub(&wake_stub_example);

    ESP_LOGI(TAG,"Entering deep sleep");
    gettimeofday(&sleep_enter_time, NULL);

    esp_deep_sleep_start();
}
#endif

void task_rx(void *p)
    {
    int x;
    for(;;) {
        lora_receive();    // put into receive mode
        while(lora_received()) {
            x = lora_receive_packet(buf, sizeof(buf));
            if (x > 0) buf[x] = '\0'; // null terminate safely
            ESP_LOGI(TAG, "Received: %s", buf);
            lora_receive();
        } 
        vTaskDelay(1);
    }
}

// Send ACK 
void task_tx(void *p)
{
for(;;) {
    vTaskDelay(pdMS_TO_TICKS(5000));
    lora_send_packet((uint8_t*)"ACK", 3);
    ESP_LOGI(TAG,"ACK sent...");
}
}

void recieve_lora_data(void)
{
    //Receieve LoRa Data
    xTaskCreate(
        task_rx,           // Task function
        "LoRa RX Task",    // Task name (for debugging)
        4096,              // Stack size in words
        NULL,              // Task input parameter
        5,                 // Priority (higher number = higher priority)
        NULL               // Task handle (optional)
    );

    if (xTaskCreate(task_tx, "LoRa TX Task", 4096, NULL, 5, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to Send ACK task");
    }     
}

void process_lora_data() 
{
    int empty_space = 0;

    // Assuming buf[] has LoRa received data
    // Example: buf[0] is the empty space in centimeters
    empty_space = buf[0];  // Modify this according to your actual data format

    int water_level = TANK_SIZE_CM - empty_space;
    water_level_percentage = (water_level * 100) / TANK_SIZE_CM;

    ESP_LOGI(TAG, "Empty Space: %d cm", empty_space);
    ESP_LOGI(TAG, "Water Level: %d cm", water_level);
    ESP_LOGI(TAG, "Water Level Percentage: %d%%", water_level_percentage);

    // If you want, you can store or send this data further here
}

void process_gpios()
{
    if(water_level_percentage >= 85)
    {
        relay_off();
        handle_led(LED_01, OFF);      
    }
    else if (water_level_percentage > 40)
    {
        handle_led(LED_01, OFF);
    }
    else if (water_level_percentage <= 40)
    {
        handle_led(LED_01, ON);
    }
    else if (water_level_percentage <= 30)
    {       
        relay_on();
        handle_led(LED_01, ON);
    }
}

void relay_on()
{
    relay_state = 1;
    handle_relay(ON);
    handle_led(LED_04, ON);
}

void relay_off()
{
    relay_state = 0;
    handle_relay(OFF);
    handle_led(LED_04, OFF);
}