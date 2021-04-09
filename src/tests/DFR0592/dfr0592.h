#pragma once
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DFR0592
    #define DFR0592_PUBLIC __declspec(dllexport)
  #else
    #define DFR0592_PUBLIC __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_DFR0592
      #define DFR0592_PUBLIC __attribute__ ((visibility ("default")))
  #else
      #define DFR0592_PUBLIC
  #endif
#endif

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

int DFR0592_PUBLIC dfr_func();

