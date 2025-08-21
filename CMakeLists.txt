idf_component_register(
	  SRCS "library/zonedetect.c"
    INCLUDE_DIRS "library"
)


# Add model partition and flash srmodels.bin
if(CONFIG_PARTITION_TABLE_CUSTOM)
    partition_table_get_partition_info(size "--partition-name zonedb" "size")
    partition_table_get_partition_info(offset "--partition-name zonedb" "offset")

    if("${size}" AND "${offset}")
				get_filename_component(ZONEDB_BIN "${CMAKE_CURRENT_SOURCE_DIR}/database/v1/timezone16.bin" ABSOLUTE)
        esptool_py_flash_to_partition(flash "zonedb" "${ZONEDB_BIN}")
    else()
        set(message "Failed to find model in partition table file"
                    "Please add a line(Name=zonedb) to the partition file if you want to use esp-sr models.")
    endif()
endif()
