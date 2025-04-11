#include <receiver.h>
#include "esp_log.h"

#define TAG_2 "APP"
char *TAG = "RECEIVER_V1";
char *TAG_1 = "SYSTEM_INFO";

struct timeval tv;
extern uint8_t system_status;

void app_main(void)
{


    print_system_memory_status();

    bool setup_status = setup();

    if(setup_status)
    {
        ESP_LOGI(TAG, "Setup Successfull!! Every Thing Initialised Successfully");
    }
    else
    {
        bool wifi_failed = !(system_status & 0b100);
        bool lora_failed = !(system_status & 0b010);
        bool gpio_failed = !(system_status & 0b001);

        if (!wifi_failed && lora_failed && !gpio_failed) {
            ESP_LOGW(TAG, "Wi-Fi Failed, but continuing to retry Wi-Fi connection...");
        } else {
            ESP_LOGE(TAG, "Critical failure detected (LoRa or GPIO). Restarting...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            esp_restart();
        }
    }

    loop();

}



void print_system_memory_status() {
    ESP_LOGI(TAG_1, "========== Chip Information ===========================================");
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    const char *chip_model;
    switch (chip_info.model) {
        case CHIP_ESP32: chip_model = "ESP32"; break;
        case CHIP_ESP32S2: chip_model = "ESP32-S2"; break;
        case CHIP_ESP32S3: chip_model = "ESP32-S3"; break;
        case CHIP_ESP32C3: chip_model = "ESP32-C3"; break;
        case CHIP_ESP32H2: chip_model = "ESP32-H2"; break;
        default: chip_model = "Unknown"; break;
    }
    ESP_LOGI(TAG_1, "Chip model: %s", chip_model);

    ESP_LOGI(TAG_1,"CPU cores: %d", chip_info.cores);
    ESP_LOGI(TAG_1,"Silicon revision: %d", chip_info.revision);

    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);  // <-- updated function
    ESP_LOGI(TAG_1,"Flash size: %lu MB", (unsigned long) flash_size / (1024 * 1024));

    const esp_app_desc_t *app_desc = esp_app_get_description();
    gettimeofday(&tv, NULL);
    ESP_LOGI(TAG_1, "========== Program Version ============================================");
    ESP_LOGI(TAG_1, "[APP] Name: %s", app_desc->project_name);
    
    ESP_LOGI(TAG_1, "[APP] Version: %s", APP_VERSION);
    ESP_LOGI(TAG_1, "[APP] Compile Date: %s", app_desc->date);
    ESP_LOGI(TAG_1, "[APP] Compile Time: %s", app_desc->time);
    // ESP_LOGI(TAG_1, "=======================================================================");
    ESP_LOGI(TAG_1, "========== Heap Information ===========================================");
    ESP_LOGI(TAG_1,"Total free heap: %lu bytes", (unsigned long) heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    ESP_LOGI(TAG_1,"Minimum free heap since boot: %lu bytes", (unsigned long) heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
    ESP_LOGI(TAG_1,"Internal RAM free: %lu bytes", (unsigned long) heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

    ESP_LOGI(TAG_1, "========== Stack Information ==========================================");
    ESP_LOGI(TAG_1,"Current task stack high water mark: %lu bytes", (unsigned long) uxTaskGetStackHighWaterMark(NULL));

    ESP_LOGI(TAG_1, "========== Flash Partition Information ================================");
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
    if (part != NULL) {
        ESP_LOGI(TAG_1,"App partition size: %lu bytes", (unsigned long) part->size);
    } else {
        ESP_LOGI(TAG_1,"App partition not found!");
    }

    ESP_LOGI(TAG_1, "=======================================================================");
}