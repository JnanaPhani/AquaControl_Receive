idf_component_register(SRCS 
                    "src/receiver.c"
                    "src/utils.c"
                    "src/aq_gpio.c"
                    "src/softap-provision.c"
                    # "src/rtcwake.c"
                    "src/lora.c"
                    INCLUDE_DIRS "./inc" )
