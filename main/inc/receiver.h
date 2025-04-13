#ifndef __RECEIVER_H__
#define __RECEIVER_H__

// Standard libraries
#include <sys/time.h>

// ESP-IDF libraries
#include "esp_timer.h"
#include "esp_app_desc.h"
#include "esp_heap_caps.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project-specific
#include <utils.h>

// Function declarations
void app_main(void);

/**
 * @brief Prints current system memory status.
 */
void print_system_memory_status(void);

#endif /* __RECEIVER_H__ */
