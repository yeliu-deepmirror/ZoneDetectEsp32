idf_component_register(
	  SRCS "library/zonedetect.c"
    INCLUDE_DIRS "library"
		REQUIRES esp_partition
		PRIV_REQUIRES spi_flash
)


# Add model partition and flash timezone16.bin
if(CONFIG_PARTITION_TABLE_CUSTOM)
    partition_table_get_partition_info(size "--partition-name zonedb" "size")
    partition_table_get_partition_info(offset "--partition-name zonedb" "offset")

    if("${size}" AND "${offset}")
				get_filename_component(ZONEDB_BIN "${CMAKE_CURRENT_SOURCE_DIR}/database/v1/timezone16.bin" ABSOLUTE)
        esptool_py_flash_to_partition(flash "zonedb" "${ZONEDB_BIN}")
    else()
        set(message "Failed to find model in partition table file"
                    "Please add a line(Name=zonedb) to the partition file.")
    endif()
endif()

# Add model partition and flash zone map csv
if(CONFIG_PARTITION_TABLE_CUSTOM)
    partition_table_get_partition_info(size "--partition-name tzcsv" "size")
    partition_table_get_partition_info(offset "--partition-name tzcsv" "offset")

    if("${size}" AND "${offset}")
				get_filename_component(ZONEDB_BIN "${CMAKE_CURRENT_SOURCE_DIR}/database/v1/zones.csv" ABSOLUTE)
        esptool_py_flash_to_partition(flash "tzcsv" "${ZONEDB_BIN}")
    else()
        set(message "Failed to find model in partition table file"
                    "Please add a line(Name=tzcsv) to the partition file.")
    endif()
endif()
