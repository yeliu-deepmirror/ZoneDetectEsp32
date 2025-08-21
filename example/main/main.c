// Copyright 2025 MOBILI Inc. All rights reserved.

#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "zonedetect.h"

#define DEBUG_MODE

#if defined(DEBUG_MODE)
#define DMLOG(...) printf(__VA_ARGS__)
#else
#define DMLOG(...) ((void)0) // do nothing
#endif

void PrintMemoryInfo() {
  DMLOG("[MEM] Internal Free Heap: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  DMLOG("[MEM] Largest Free Block: %d bytes, DMA : %d bytes \n",
        heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        heap_caps_get_largest_free_block(MALLOC_CAP_DMA));
  DMLOG("[MEM] PSRAM Free Heap: %d / %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
}


void ZoneonError(int errZD, int errNative) {
  DMLOG("[GPS] ZD error: %s (0x%08X)\n", ZDGetErrorString(errZD), (unsigned)errNative);
}

void ZonePrintResults(ZoneDetect* cd, ZoneDetectResult* results, float safezone) {
  if (!results) {
    DMLOG("[GPS] No results\n");
    return;
  }

  unsigned int index = 0;
  while (results[index].lookupResult != ZD_LOOKUP_END) {
    DMLOG("%s:\n", ZDLookupResultToString(results[index].lookupResult));
    DMLOG("  meta: %lu\n", results[index].metaId);
    DMLOG("  polygon: %lu\n", results[index].polygonId);
    if (results[index].data) {
      for (unsigned int i = 0; i < results[index].numFields; i++) {
        if (results[index].fieldNames[i] && results[index].data[i]) {
          DMLOG("  %s: %s\n", results[index].fieldNames[i], results[index].data[i]);
        }
      }
    }

    index++;
  }
  ZDFreeResults(results);

  if (index) {
    DMLOG("Safezone: %f\n", safezone);
  }
}

void SetAndTransformUnixTime(int64_t unit_time_int, const char* time_zone) {
  // test to set time zone and convert unix time
  setenv("TZ", time_zone, 1);  // set timezone environment variable
  tzset();                      // apply it

  // Thu Aug 21 2025 07:02:13 GMT+0000
  time_t unix_time = unit_time_int;  // example GPS UTC timestamp
  struct tm local_tm;

  localtime_r(&unix_time, &local_tm);

  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", &local_tm);
  DMLOG("Local time: %s\n", buf);
}

void app_main(void) {

  PrintMemoryInfo();

  ZDSetErrorHandler(ZoneonError);

  ZoneDetect* const cd = ZDOpenDatabase("zonedb");
  if (!cd) {
    DMLOG("[GPS] open data base failed.\n");
  } else {
    ZDOpenDatabaseMap(cd, "tzcsv");
    char buffer_zone[64];
    {
      const float lat = 47.5098014;
      const float lon = 0.485065422;
      DMLOG("\n[GPS] test %f %f\n", lat, lon);

      float safezone = 0;
      ZoneDetectResult* results = ZDLookup(cd, lat, lon, &safezone);
      ZonePrintResults(cd, results, safezone);

      char* simple_str = ZDHelperSimpleLookupString(cd, lat, lon);
      DMLOG("The simple string is [%s]\n", ZDHelperSimpleLookupString(cd, lat, lon));
      if (CsvQueryTimeZone(cd, simple_str, buffer_zone, 64)) {
        DMLOG("Time zone %s\n", buffer_zone);
        SetAndTransformUnixTime(1755759733, buffer_zone);
      }
      free(simple_str);
    }
    {
      const float lat = 50.257067746810;
      const float lon = 5.065349821;
      DMLOG("\n[GPS] test %f %f\n", lat, lon);

      float safezone = 0;
      ZoneDetectResult* results = ZDLookup(cd, lat, lon, &safezone);
      ZonePrintResults(cd, results, safezone);

      char* simple_str = ZDHelperSimpleLookupString(cd, lat, lon);
      DMLOG("The simple string is [%s]\n", ZDHelperSimpleLookupString(cd, lat, lon));
      if (CsvQueryTimeZone(cd, simple_str, buffer_zone, 64)) {
        DMLOG("Time zone %s\n", buffer_zone);
        SetAndTransformUnixTime(1755759733, buffer_zone);
      }
      free(simple_str);
    }

    // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.json
    {
      // test to set time zone and convert unix time
      const float lat = 22.781663;
      const float lon = 113.513604;
      DMLOG("\n[GPS] test %f %f\n", lat, lon);

      char* simple_str = ZDHelperSimpleLookupString(cd, lat, lon);
      DMLOG("The simple string is [%s]\n", ZDHelperSimpleLookupString(cd, lat, lon));
      if (CsvQueryTimeZone(cd, simple_str, buffer_zone, 64)) {
        DMLOG("Time zone %s\n", buffer_zone);
        SetAndTransformUnixTime(1755759733, buffer_zone);
      }
      free(simple_str);
    }
  }
  PrintMemoryInfo();

  if (cd) ZDCloseDatabase(cd);
}
