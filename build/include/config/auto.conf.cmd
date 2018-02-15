deps_config := \
	/Users/zhy/ESP32/src/esp-idf/components/app_trace/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/aws_iot/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/bt/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/esp32/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/ethernet/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/fatfs/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/freertos/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/log/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/lwip/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/mbedtls/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/openssl/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/spi_flash/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/wear_levelling/Kconfig \
	/Users/zhy/ESP32/src/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/zhy/ESP32/src/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/zhy/ESP32/demo/oled_test/main/Kconfig.projbuild \
	/Users/zhy/ESP32/src/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/zhy/ESP32/src/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
