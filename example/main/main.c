// Copyright 2025 MOBILI Inc. All rights reserved.

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

void app_main(void) {

  PrintMemoryInfo();

  ZDSetErrorHandler(ZoneonError);

  ZoneDetect* const cd = ZDOpenDatabase("zonedb");
  if (!cd) {
    DMLOG("[GPS] open data base failed.\n");
  } else {
    // Latitude: 22.781663 / N 22° 46' 53.988''
    // Longitude: 113.513604 / E 113° 30' 48.972''
    const float lat = 22.781663;
    const float lon = 113.513604;

    float safezone = 0;
    ZoneDetectResult* results = ZDLookup(cd, lat, lon, &safezone);
    ZonePrintResults(cd, results, safezone);

    DMLOG("The simple string is [%s]\n", ZDHelperSimpleLookupString(cd, lat, lon));

    ZDCloseDatabase(cd);
  }

  PrintMemoryInfo();
}
