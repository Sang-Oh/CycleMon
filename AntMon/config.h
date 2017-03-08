/*
 * Dynastream Innovations Inc.
 * Cochrane, AB, CANADA
 *
 * Copyright © 1998-2007 Dynastream Innovations Inc.
 * All rights reserved. This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 */ 

#if !defined(CONFIG_H)
#define CONFIG_H

#if defined(_WIN32_WINNT)
   #if (_WIN32_WINNT < 0x501)
      #undef _WIN32_WINNT
      #define _WIN32_WINNT             0x0501
   #endif
#else
   #define _WIN32_WINNT                0X0501
#endif

// Version Information
#define SW_VER_PPP                     "AGV"
#define SW_VER_NUM                     "0.0"
#define SW_VER_DEV                     "40"
#define SW_VER                         SW_VER_PPP SW_VER_NUM SW_VER_DEV

// Serial configuration.  Choose ONE of the following:
//---
#define SERIAL_SI
//#define SERIAL_COM
//---

#if defined(SERIAL_SI)
   #define RECURSE_ON_BAD_CRC
#endif

// Debug configuration
#define DEBUG_FILE
#define DEBUG_VERBOSE                                     // Creates big debug file!

#define GARMIN_BUILD
#pragma warning(disable: 4127)  // constant conditional expression

#endif // !defined(CONFIG_H)
