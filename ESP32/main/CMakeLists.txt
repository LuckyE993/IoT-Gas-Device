idf_component_register(
    SRCS "uart_main.c" 
    SRCS "../src/wireless.c"
    SRCS "../src/wifi.c"
    SRCS "../src/cJSON.c"
    SRCS "../src/cJSON_Utils.c"
    SRCS "../src/server.c"

    INCLUDE_DIRS "../inc"
    INCLUDE_DIRS ".")
