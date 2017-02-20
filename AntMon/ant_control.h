/*******************************************************************************
 * THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
 * 
 * Please forward all questions regarding this code to ANT Technical Support.
 * 
 * Dynastream Innovations Inc.
 * 228 River Avenue
 * Cochrane, Alberta, Canada
 * T4C 2C1
 * 
 * (P) (403) 932-9292
 * (F) (403) 932-6521
 * (TF) 1-866-932-9292
 * (E) support@thisisant.com
 * 
 * www.thisisant.com
 *
 * Reference Design Disclaimer
 *
 * The references designs and codes provided may be used with ANT devices only and remain the copyrighted property of 
 * Dynastream Innovations Inc. The reference designs and codes are being provided on an "as-is" basis and as an accommodation, 
 * and therefore all warranties, representations, or guarantees of any kind (whether express, implied or statutory) including, 
 * without limitation, warranties of merchantability, non-infringement,
 * or fitness for a particular purpose, are specifically disclaimed.
 *
 * ?009 Dynastream Innovations Inc. All Rights Reserved
 * This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 *
 *******************************************************************************/

#ifndef __ANT_CONTROL__
#define __ANT_CONTROL__

#include "config.h"
#include "types.h"

#if defined(__cplusplus)
   extern "C" {
#endif

// Application events	   
typedef enum
{
	ANT_EVENT_DEBUG = 0xF0,
	ANT_EVENT_CONNECT,
	ANT_EVENT_OPEN,
	ANT_EVENT_CLOSE,
	ANT_EVENT_DISCONNECT,
} AntResponseEnum;

typedef void(*ANT_RESPONSE_FUNC)(UCHAR ucEvent_, char* pcBuffer_);

///////////////////////////////////////////////////////////////////////////
// Function Prototypes 
///////////////////////////////////////////////////////////////////////////
BOOL ANTControl_Connect(ANT_RESPONSE_FUNC pResponseFunc_, UCHAR ucUSBDeviceNum_, USHORT usBaudRate_);
BOOL ANTControl_ConfigScanningMode(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR ucRFFreq_, BOOL bRxOnly);
BOOL ANTControl_OpenScanningMode();
BOOL ANTControl_CloseScanningMode();
void ANTControl_Disconnect();
BOOL ANTControl_SendExtBroadcast(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_);
BOOL ANTControl_SendExtAcknowledged(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_);
UCHAR ANTControl_SendExtBurst(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_, UCHAR ucNumPackets_);

#if defined(__cplusplus)
   }
#endif

#endif // ANT_CONTROL