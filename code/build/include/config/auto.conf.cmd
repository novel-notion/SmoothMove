deps_config := \
	/home/chris/Development/esp/esp-idf/components/app_trace/Kconfig \
	/home/chris/Development/esp/esp-idf/components/aws_iot/Kconfig \
	/home/chris/Development/esp/esp-idf/components/bt/Kconfig \
	/home/chris/Development/esp/esp-idf/components/driver/Kconfig \
	/home/chris/Development/esp/esp-idf/components/esp32/Kconfig \
	/home/chris/Development/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/chris/Development/esp/esp-idf/components/ethernet/Kconfig \
	/home/chris/Development/esp/esp-idf/components/fatfs/Kconfig \
	/home/chris/Development/esp/esp-idf/components/freertos/Kconfig \
	/home/chris/Development/esp/esp-idf/components/heap/Kconfig \
	/home/chris/Development/esp/esp-idf/components/libsodium/Kconfig \
	/home/chris/Development/esp/esp-idf/components/log/Kconfig \
	/home/chris/Development/esp/esp-idf/components/lwip/Kconfig \
	/home/chris/Development/esp/esp-idf/components/mbedtls/Kconfig \
	/home/chris/Development/esp/esp-idf/components/openssl/Kconfig \
	/home/chris/Development/esp/esp-idf/components/pthread/Kconfig \
	/home/chris/Development/esp/esp-idf/components/spi_flash/Kconfig \
	/home/chris/Development/esp/esp-idf/components/spiffs/Kconfig \
	/home/chris/Development/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/chris/Development/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/chris/Development/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/chris/Development/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/chris/Development/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/chris/Development/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
