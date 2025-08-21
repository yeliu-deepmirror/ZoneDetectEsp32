# ZoneDetect

This is a C library that allows you to find an area a point belongs to using a database file. A typical example would be looking up the country or timezone given a latitude and longitude. The timezone database also contains the country information.

The API should be self-explanatory from zonedetect.h. A small demo is included (demo.c). You can build the demo with `make demo` and run it like this: `./demo timezone21.bin 35.0715 -82.5216`.

The databases are obtained from [here](https://github.com/evansiroky/timezone-boundary-builder) and converted to the format used by this library.

### Online API
You can test the library using an online API: [https://timezone.bertold.org/timezone](https://timezone.bertold.org/timezone)
It takes the following GET parameters:

* lat: Latitude.
* lon: Longitude.
* c: Set to one (c=1) to produce compact JSON.
* s: Set to one (s=1) to get only the timezone.

For example: [https://timezone.bertold.org/timezone?lat=51&lon=5](https://timezone.bertold.org/timezone?lat=51&lon=5)  
You are free to use this API for any application, but I am not responsible for the quality of service. Please contact me if your application requires reliability.


### Demo
An online demo is available here: [https://cdn.bertold.org/demo/timezone.html](https://cdn.bertold.org/demo/timezone.html). Simple click anywhere on the map and see the result of the query.

![CodeBuild](https://codebuild.eu-central-1.amazonaws.com/badges?uuid=eyJlbmNyeXB0ZWREYXRhIjoiVmtHd1UwSThmbkZ4RlkyRVZMQUgyRVFKdjhyRzJ6N29ZQUNUVU4wdFpEcUdZQ29ITzlHQlFEU0VZd1dXcEtJUlRvcUFLSDJLTnp5V2lQUGkxNGs1ZUdJPSIsIml2UGFyYW1ldGVyU3BlYyI6ImxxUnFTTVBreUNrK2xPeHYiLCJtYXRlcmlhbFNldFNlcmlhbCI6MX0%3D&branch=master)


### Demo - ESP32

```
cd example
idf.py build flash monitor
```

```
[MEM] Internal Free Heap: 389372 bytes
[MEM] Largest Free Block: 327680 bytes, DMA : 327680 bytes
[MEM] PSRAM Free Heap: 0 / 0 bytes
I (279) ZoneDetect: Mapped 6041600 partition to data memory address 0x3c030000
I (286) ZoneDetect: tableType: 84, version: 1, precision: 16, numFields: 4
I (293) ZoneDetect: Mapped 32768 csv partition to data memory address 0x3c600000

[GPS] test 47.509800 0.485065
In zone:
  meta: 7097
  polygon: 1600
  TimezoneIdPrefix: Europe/
  TimezoneId: Paris
  CountryAlpha2: FR
  CountryName: France
Safezone: 2.152767
The simple string is [Europe/Paris]
Time zone CET-1CEST,M3.5.0,M10.5.0/3
Local time: 2025-08-21 09:02:13 CEST

[GPS] test 50.257069 5.065350
In zone:
  meta: 6581
  polygon: 1692
  TimezoneIdPrefix: Europe/
  TimezoneId: Brussels
  CountryAlpha2: BE
  CountryName: Belgium
Safezone: 0.207235
The simple string is [Europe/Brussels]
Time zone CET-1CEST,M3.5.0,M10.5.0/3
Local time: 2025-08-21 09:02:13 CEST

[GPS] test 22.781664 113.513603
The simple string is [Asia/Shanghai]
Time zone CST-8
Local time: 2025-08-21 15:02:13 CST
[MEM] Internal Free Heap: 387984 bytes
[MEM] Largest Free Block: 327680 bytes, DMA : 327680 bytes
[MEM] PSRAM Free Heap: 0 / 0 bytes
I (675) ZoneDetect: Unmapped partition from data memory
I (680) main_task: Returned from app_main()
```
