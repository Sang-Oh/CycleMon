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

#include "stdafx.h"
#include "ant_control.h"
#include "antmessage.h"
#include "ant_dll.h"
#include "types.h"
#include "config.h"

#define MESSAGE_TIMEOUT 3300
#define USER_NETWORK_KEY      {0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45} 
#define ANT_NETWORK     0       // public network
#define ANT_SCAN_CHAN   0       // Config of scanning mode
#define ANT_EXT_CHAN    1       // Channel resources to use for handling extended messages
                                // This demo uses a single channel to handle all extended messages
                                // It is also possible to use multiple channels, and have multiple messages pending to send to different devices

typedef struct
{
	UCHAR ucChannel;
	UCHAR ucChannelType;
	USHORT usDeviceNo;
	UCHAR ucDeviceType;
	UCHAR ucTransType;
	UCHAR ucRFFreq;
} ChannelDescriptor;

static UCHAR aucRxExtBuf[15];
static UCHAR aucResponseBuf[10];
static UCHAR aucTxBuf[12];
static volatile ANT_RESPONSE_FUNC pResponseFunc = NULL;
static UCHAR ucAntAckedMesg;
static UCHAR ucAck;

BOOL WaitAck(UCHAR ucAntMesg_, ULONG ulTimeOutMiliseconds_);
static void FormatExt(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_);

/////////////////////////
// Channel Definitions //
/////////////////////////

// Setup scanning mode
static ChannelDescriptor cdScanningMode = {ANT_SCAN_CHAN, 0, 0, 0, 0, 66}; 

////////////////////////
// Callback functions //
////////////////////////

void ANTProtocolEventProcess(UCHAR ucChannel_, UCHAR ucMessageCode_)
{
	switch(ucMessageCode_)
	{
	case MESG_RESPONSE_EVENT_ID:
			ucAntAckedMesg = aucResponseBuf[1];
			ucAck = aucResponseBuf[2];
			break;

		default:
			break;
	}
}

void ANTChannelEventProcess(UCHAR ucChannel_, UCHAR ucMessageCode_)
{
    if(ucChannel_ == ANT_SCAN_CHAN)
    {
	     switch(ucMessageCode_)
	     {
           case EVENT_RX_BROADCAST:
           case EVENT_RX_BURST_PACKET:
           case EVENT_RX_ACKNOWLEDGED:
	        case EVENT_RX_EXT_BROADCAST:
           case EVENT_RX_EXT_BURST_PACKET:
           case EVENT_RX_EXT_ACKNOWLEDGED:
           case EVENT_RX_FLAG_BROADCAST:
           case EVENT_RX_FLAG_BURST_PACKET:
           case EVENT_RX_FLAG_ACKNOWLEDGED:     // library only events
               pResponseFunc(ucMessageCode_, (char*) &aucRxExtBuf[1]);
               break;
           default:
               break;
        }
    }
    else if(ucChannel_ == ANT_EXT_CHAN)
    {
        switch(ucMessageCode_)
        {
            case EVENT_TX:
            case EVENT_TRANSFER_TX_COMPLETED:
                pResponseFunc(ucMessageCode_, NULL);
                break;
	         default:
		          break;
        }
	}
}

/////////////////////////////////////
// GUI Callback and USB connection //
/////////////////////////////////////
BOOL ANTControl_Connect(ANT_RESPONSE_FUNC pResponseFunc_, UCHAR ucUSBDeviceNum_, USHORT usBaudRate_)
{
	if (pResponseFunc_ == NULL)
		return FALSE;

	pResponseFunc = pResponseFunc_;

	if (!ANT_Load())
	{
		pResponseFunc(ANT_EVENT_DEBUG, "Failed to load ANT DLL\r\n");
		return FALSE;
	}
	if(!ANT_Init(ucUSBDeviceNum_, usBaudRate_))
	{
		pResponseFunc(ANT_EVENT_DEBUG, "Failed to initialize ANT\r\n");
		return FALSE;
	}

	// assign callback functions

	ANT_AssignResponseFunction( (RESPONSE_FUNC) ANTProtocolEventProcess, aucResponseBuf);
	ANT_AssignChannelEventFunction( cdScanningMode.ucChannel, (CHANNEL_EVENT_FUNC) ANTChannelEventProcess, aucRxExtBuf); // Scanning mode and incoming extended messages
    ANT_AssignChannelEventFunction( ANT_EXT_CHAN, (CHANNEL_EVENT_FUNC) ANTChannelEventProcess, aucRxExtBuf); // Outgoing extended messages 

	ANT_ResetSystem();
	Sleep(1000);
	pResponseFunc(ANT_EVENT_CONNECT, NULL);
	return(TRUE);

}

//////////////// Config Scanning Mode
BOOL ANTControl_ConfigScanningMode(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR ucRFFreq_, BOOL bRxOnly)
{
    BOOL bStatus = TRUE;

    cdScanningMode.usDeviceNo = usDeviceNo_;
    cdScanningMode.ucDeviceType = ucDeviceType_;
    cdScanningMode.ucTransType = ucTransType_;
    cdScanningMode.ucRFFreq = ucRFFreq_;
    if(bRxOnly)
        cdScanningMode.ucChannelType = 0x40;
    else
        cdScanningMode.ucChannelType = 0x00;

    // Config scanning mode
    // Using public network, so no need to set network key
	UCHAR ucNetKey[8] = USER_NETWORK_KEY;   
	ANT_SetNetworkKey(cdScanningMode.ucChannel, ucNetKey);
    ANT_AssignChannel(cdScanningMode.ucChannel, cdScanningMode.ucChannelType, ANT_NETWORK);
	 if(!WaitAck(MESG_ASSIGN_CHANNEL_ID, MESSAGE_TIMEOUT))
		  bStatus = FALSE;
    // Set channel ID mask
	 ANT_SetChannelId(cdScanningMode.ucChannel, cdScanningMode.usDeviceNo, cdScanningMode.ucDeviceType, cdScanningMode.ucTransType);
	 if(!WaitAck(MESG_CHANNEL_ID_ID, MESSAGE_TIMEOUT))
		  bStatus = FALSE;
    // Set RF frequency
	 ANT_SetChannelRFFreq(cdScanningMode.ucChannel, cdScanningMode.ucRFFreq);
	 if(!WaitAck(MESG_CHANNEL_RADIO_FREQ_ID, MESSAGE_TIMEOUT))
		  bStatus = FALSE;
    // Enable extended messages
    ANT_RxExtMesgsEnable(TRUE);
    if(!WaitAck(MESG_RX_EXT_MESGS_ENABLE_ID, MESSAGE_TIMEOUT))
        bStatus = FALSE;

	 if(!bStatus)
	 {
		  pResponseFunc(ANT_EVENT_DEBUG, "Config failed\r\n");
	 }

    return bStatus;
}

//////////////// Open Scanning Mode
BOOL ANTControl_OpenScanningMode()
{	
	BOOL bStatus = TRUE;

	ANT_OpenRxScanMode();
	if(!WaitAck(MESG_OPEN_RX_SCAN_ID, MESSAGE_TIMEOUT))
	{
		pResponseFunc(ANT_EVENT_DEBUG, "Open failed\r\n");
		bStatus = FALSE;
	}
	else
   {
		pResponseFunc(ANT_EVENT_OPEN, NULL);
   }

	return bStatus;
}

//////////////// Close Scanning Mode
BOOL ANTControl_CloseScanningMode()
{
  	ANT_CloseChannel(cdScanningMode.ucChannel);
	if(!WaitAck(MESG_CLOSE_CHANNEL_ID, MESSAGE_TIMEOUT))
	{
		pResponseFunc(ANT_EVENT_DEBUG, "Close  failed\r\n");
		return(FALSE);
	}
	else
	{
		pResponseFunc(ANT_EVENT_CLOSE, NULL);
	}
	ANT_UnAssignChannel(cdScanningMode.ucChannel);
	if(!WaitAck(MESG_UNASSIGN_CHANNEL_ID , MESSAGE_TIMEOUT))
	{
		pResponseFunc(ANT_EVENT_DEBUG, "Unassignment failed\r\n");
		return(FALSE);
	}
	return(TRUE);
}

//////////////// Disconnect
void ANTControl_Disconnect()
{
	ANT_ResetSystem();
	Sleep(1000);
	ANT_Close();
	pResponseFunc(ANT_EVENT_DISCONNECT, NULL);
}

//////////////// Send Extended Broadcast Data
BOOL ANTControl_SendExtBroadcast(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_)
{
	UCHAR ucChannel = ANT_EXT_CHAN;
	
	ANT_UnAssignChannel(ucChannel);
	
	// Setup channel parameters
	ANT_AssignChannel(ucChannel, cdScanningMode.ucChannelType, ANT_NETWORK);

	ANT_SetChannelId(ucChannel, usDeviceNo_, ucDeviceType_, ucTransType_);

	// Send data
	FormatExt(usDeviceNo_, ucDeviceType_, ucTransType_, pcBuffer_);
	ANT_SendExtBroadcastData(ucChannel, aucTxBuf);

	return(TRUE);
}

//////////////// Send Extended Ack Data
BOOL ANTControl_SendExtAcknowledged(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_)
{
	UCHAR ucChannel = ANT_EXT_CHAN;
	
	ANT_UnAssignChannel(ucChannel);
	
	// Setup channel parameters
	ANT_AssignChannel(ucChannel, cdScanningMode.ucChannelType, ANT_NETWORK);

	ANT_SetChannelId(ucChannel, usDeviceNo_, ucDeviceType_, ucTransType_);

	// Send data
	FormatExt(usDeviceNo_, ucDeviceType_, ucTransType_, pcBuffer_);
	ANT_SendExtAcknowledgedData(ucChannel, aucTxBuf);

	return(TRUE);
}

//////////////// Send Extended Burst Data Transfer
UCHAR ANTControl_SendExtBurst(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_, UCHAR ucNumPackets_)
{
	UCHAR ucChannel = ANT_EXT_CHAN;
   UCHAR i;
   UCHAR ucSeq = 0;
	
	ANT_UnAssignChannel(ucChannel);
	
	// Setup channel parameters
	ANT_AssignChannel(ucChannel, 0, ANT_NETWORK);

	ANT_SetChannelId(ucChannel, usDeviceNo_, ucDeviceType_, ucTransType_);

	// Format data
   FormatExt(usDeviceNo_, ucDeviceType_, ucTransType_, pcBuffer_);

   // Send data
   for(i = 0; i < ucNumPackets_; i++)
   {
      if(i == ucNumPackets_ - 1)
          ucSeq |= SEQUENCE_LAST_MESSAGE;
      if(!ANT_SendExtBurstTransferPacket(ucChannel | ucSeq, aucTxBuf))      // We are sending the same packet every time here, use ANT_SendExtBurstTransfer if sending a block of data     
          return (ucNumPackets_ - i);
      if (ucSeq == SEQUENCE_NUMBER_ROLLOVER)
         ucSeq = SEQUENCE_NUMBER_INC;
      else
         ucSeq += SEQUENCE_NUMBER_INC;
   }

	return(TRUE);
}

//////////////// Format extended data
static void FormatExt(USHORT usDeviceNo_, UCHAR ucDeviceType_, UCHAR ucTransType_, UCHAR* pcBuffer_)
{
   aucTxBuf[0] = (UCHAR) usDeviceNo_; // Device Number LSB
	aucTxBuf[1] = (UCHAR) (usDeviceNo_ >> 8);	// Device Number MSB
	aucTxBuf[2] = ucDeviceType_;
	aucTxBuf[3] = ucTransType_;
   memcpy(&aucTxBuf[4], pcBuffer_, 8);
}

//////////////// Wait for response from ANT
BOOL WaitAck(UCHAR ucAntMesg_, ULONG ulTimeOutMiliseconds_)
{
	BOOL bStatus = FALSE;
	ULONG ulStartTime = GetTickCount();

	do
	{
		if(ucAntMesg_ == ucAntAckedMesg)
		{
			if(ucAck == RESPONSE_NO_ERROR)
				bStatus = TRUE;
			else
				break;
		}
	} while((bStatus == FALSE) && ((GetTickCount()-ulStartTime) < ulTimeOutMiliseconds_));

	ucAck = 0;
	ucAntAckedMesg = 0;

	return bStatus;
}
