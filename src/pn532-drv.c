#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "log.h"

#include "OnBoard.h"

#include "hal_assert.h"
#include "hal_i2c.h"

#include "mua.h"
#include "pn532.h"

#define LOG_TAG             "PN5"

#define DEBUG_PN532         1
#define DEBUG_PN532_BYTE    1
#define DEBUG_PN532_PACKET  1

#define PN532_DATABUF_MAX   32

static uint8 sDataBuf[PN532_DATABUF_MAX];
static uint8 sWaitAck = 0;          // Flag set after sending cmd, send ACK on the coming IRQ from PN532

static uint8 PN532_ACK_FRAME[] = { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00 };

uint8 PN532_Write(uint8 *data, uint8 len)
{
  uint8 res = PN532_BUS_BUSY;

  if (!data || !len) {
    return PN532_INVALID_PARAM;
  }

  res = HalI2CWrite(PN532_I2C_ADDRESS, len, data);

#if DEBUG_PN532_BYTE
  LOGD(LOG_TAG, "W - %02d - 0x", res);
  for (uint8 i = 0; i < res; i++) {
    LOG("%02x ", *(data + i));
  }
  LOG("\r\n");
#endif


  return (res == len ? PN532_GOOD : PN532_BUS_BUSY);
}

uint8 PN532_Read(uint8 *data, uint8 len)
{
  uint8 res = PN532_BUS_BUSY;

  if (!data || !len) {
    return PN532_INVALID_PARAM;
  }

  res = HalI2CRead(PN532_I2C_ADDRESS, len, data);

#if DEBUG_PN532_BYTE
  LOGD(LOG_TAG, "R - %02d - 0x", res);
  for (uint8 i = 0; i < res; i++) {
    LOG("%02x ", *(data + i));
  }
  LOG("\r\n");
#endif

  return (res == len) ? PN532_GOOD : PN532_BUS_BUSY;
}

/**
 * +-----+-----+-----------+
 * | TFI | PD0 | PD1...PDn |
 * +-----+-----+-----------+
 * | D4  | cmd | cmd data  |
 * +-----+-----+-----------+
 */
uint8 PN532_SendCmd(uint8 cmd, uint8 *pCmdData, uint8 dataLen, uint8 waitAck)
{
  uint8 res = PN532_BUS_BUSY;
  uint8 idx = 0;
  uint8 sum = 0;

  if (!pCmdData && dataLen) {
    return PN532_INVALID_PARAM;
  }

  sDataBuf[idx++] = PN532_PREAMBLE;
  sDataBuf[idx++] = PN532_STARTCODE_0;
  sDataBuf[idx++] = PN532_STARTCODE_1;
  sDataBuf[idx++] = dataLen + 2;         // Length of (TFI, PD0...PDn)
  sDataBuf[idx++] = ~(dataLen + 2) + 1;  // Length Checksum
  sDataBuf[idx++] = PN532_TFI_HOST2PN;   // Frame Identifier
  sDataBuf[idx++] = cmd;                 // PD0

  sum = PN532_TFI_HOST2PN + cmd;
  // PD1...PDn (Optional Input Data)
  for (uint8 i = 0; i < dataLen; i++) {
    sDataBuf[idx++] = pCmdData[i];
    sum += pCmdData[i];
  }
  sDataBuf[idx++] = (~sum) + 1;          // Data Checksum
  sDataBuf[idx++] = PN532_POSTAMBLE;

  res = HalI2CWrite(PN532_I2C_ADDRESS, idx, sDataBuf);
#if DEBUG_PN532_BYTE
  LOGD(LOG_TAG, "W - %02d - 0x", res);
  for (uint8 i = 0; i < res; i++) {
    LOG("%02x ", *(sDataBuf + i));
  }
  LOG("\r\n");
#endif

  sWaitAck = (res == idx ? waitAck : 0);

  return (res == idx ? PN532_GOOD : PN532_BUS_BUSY);
}

uint8 PN532_SendAck(void)
{
  uint8 res = PN532_BUS_BUSY;

  res = HalI2CWrite(PN532_I2C_ADDRESS, sizeof(PN532_ACK_FRAME), PN532_ACK_FRAME);

  return (res == sizeof(PN532_ACK_FRAME) ? PN532_GOOD : PN532_BUS_BUSY);
}

/**
 * Return
 * 0      - ACK
 * 1      - NACK
 * Others - ERROR
 */
int8 PN532_ReadAck(void)
{
  uint8 res = PN532_BUS_BUSY;

  res = HalI2CRead(PN532_I2C_ADDRESS, PN532_DATABUF_MAX, sDataBuf);

#if DEBUG_PN532_BYTE
  LOGD(LOG_TAG, "R - %02d - 0x", res);
  for (int8 i = 0; i < res; i++) {
    LOG("%02x ", *(sDataBuf + i));
  }
  LOG("\r\n");
#endif

  res = PN532_FrameParser(sDataBuf, res, NULL, NULL);

  if (res == PN532_TFI_ACK) {
    return PN532_ACK;
  }
  if (res == PN532_TFI_NACK) {
    return PN532_NACK;
  }

  return res;
}

/**
 * Return the number of bytes successfully read.
 */
int8 PN532_ReadRsp(uint8 *pResp)
{
  int8 res = PN532_BUS_BUSY;

  if (!pResp) {
    return PN532_INVALID_PARAM;
  }

  res = HalI2CRead(PN532_I2C_ADDRESS, PN532_DATABUF_MAX, pResp);

#if DEBUG_PN532_BYTE
  LOGD(LOG_TAG, "R - %02d - 0x", res);
  for (int8 i = 0; i < res; i++) {
    LOG("%02x ", *(pResp + i));
  }
  LOG("\r\n");
#endif

  return res;
}

/**
 * PN532 generates IRQ signal to inform that the answer is ready
 * (both ACK frame and answer response frame).
 * \code{.c}
 * IRQ -----------------+  +------------------------+        +--------------------------------
 *                      |  |                        |        |
 *                      +--+                        +--------+
 * SDA -------+-------+---+------------+--------------------+-------------+-------------------
 *            |  CMD  |   | Read + ACK |                    | Read + RESP |
 *            +-------+   +------------+                    +-------------+
 * SCL -------+-------+---+------------+--------------------+-------------+-------------------
 *            |  CLK  |   |     CLK    |                    |     CLK     |
 *            +-------+   +------------+                    +-------------+
 * \endcode
 *
 * However, if the host controller starts an exchange asserting the H_REQ signal,
 * PN532 will react as described below.
 * \code{.c}
 * REQ ---+ +---------------------------------------------------------------------------------
 *        | |
 *        +-+
 * IRQ ------+ +--------+  +------------------------+        +--------------------------------
 *           | |        |  |                        |        |
 *           +-+        +--+                        +--------+
 * SDA -------+-------+---+------------+--------------------+-------------+-------------------
 *            |  CMD  |   | Read + ACK |                    | Read + RESP |
 *            +-------+   +------------+                    +-------------+
 * SCL -------+-------+---+------------+--------------------+-------------+-------------------
 *            |  CLK  |   |     CLK    |                    |     CLK     |
 *            +-------+   +------------+                    +-------------+
 * \endcode
 */

/**
 * Input:
 * +----+----+
 * | D4 | 02 |
 * +----+----+
 * Output:
 * +----+----+----+-----+-----+---------+
 * | D5 | 03 | IC | Ver | Rev | Support |
 * +----+----+----+-----+-----+---------+
 */
uint8 PN532_GetFirmwareVersion(PN532_FirmwareVersion_t *pVer)
{
  int8 res = PN532_BUS_BUSY;
  uint8 tfi = 0;
  uint8 len = 0;
  uint8 *pPacket = NULL;

  res = PN532_SendCmd(PN532_CMD_GETFIRMWAREVERSION, NULL, 0, 0);
  if (res != PN532_GOOD) {
    return res;
  }

  if (PN532_ReadAck() != PN532_ACK) {
    return PN532_INVALID_ACK;
  }

  memset(sDataBuf, 0, sizeof(sDataBuf));    // Clean buffer for sure
  res = PN532_ReadRsp(sDataBuf);
  if (res <= 0) {
    return PN532_ERR;
  }

  // Decode response frame
  tfi = PN532_FrameParser(sDataBuf, res, (void **)&pPacket, &len);
  if (tfi != PN532_TFI_PN2HOST) {
    return PN532_INVALID_FRAME;
  }
  if (!pPacket
      || pPacket[0] != PN532_CMD_GETFIRMWAREVERSION + 1
      || len != sizeof(PN532_FirmwareVersion_t) + 1
     ) {
    return PN532_INVALID_FRAME;
  }

  if (!pVer) {
    return PN532_INVALID_PARAM;
  }

  pVer->ic      = pPacket[1];
  pVer->ver     = pPacket[2];
  pVer->rev     = pPacket[3];
  pVer->support = pPacket[4];

#if DEBUG_PN532_PACKET
  LOGD(LOG_TAG, "IC : %02x\r\n", pPacket[1]);
  LOGD(LOG_TAG, "VER: %02x\r\n", pPacket[2]);
  LOGD(LOG_TAG, "REV: %02x\r\n", pPacket[3]);
  LOGD(LOG_TAG, "SUP: %02x\r\n", pPacket[4]);
#endif

  return PN532_GOOD;
}

/* Input
 * +----+----+-------+------+------------+
 * | D4 | 4A | MaxTg | BrTy | [InitData] |
 * +----+----+-------+------+------------+
 * Output
 * +----+----+------+----------------+----------------+
 * | D5 | 4B | NbTg | [TgtData1 [] ] | [TgtData2 [] ] |
 * +----+----+------+----------------+----------------+
 * Return
 * Success or Error
 */
uint8 PN532_InListPassiveTarget(PN532_InListPassiveTarget_Cmd_t *pCmd, PN532_InListPassiveTarget_Resp_t *pResp)
{
  int8 res = PN532_BUS_BUSY;
  uint8 tfi = 0;
  uint8 len = 0;
  uint8 *pPacket = NULL;
  uint8 cmdData[] = { 0x01, 0x00 };     // MaxTg, BrTy

  // Input pCmd -> cmdData
  // TODO
#if 0
  if (!pCmd) {
    return PN532_INVALID_PARAM;
  }
#endif

  res = PN532_SendCmd(PN532_CMD_INLISTPASSIVETARGET, cmdData, sizeof(cmdData), 0);
  if (res != PN532_GOOD) {
    return res;
  }

  if (PN532_ReadAck() != PN532_ACK) {
    return PN532_INVALID_ACK;
  }

  delayMS(5);                               // FIXME: necessary for POLL? No need for IRQ

  memset(sDataBuf, 0, sizeof(sDataBuf));    // Clean buffer for sure
  res = PN532_ReadRsp(sDataBuf);
  if (res <= 0) {
    return PN532_ERR;
  }

  // Decode response frame
  tfi = PN532_FrameParser(sDataBuf, res, (void **)&pPacket, &len);
  if (tfi != PN532_TFI_PN2HOST) {
    return PN532_INVALID_FRAME;
  }
  if (!pPacket
      || pPacket[0] != PN532_CMD_INLISTPASSIVETARGET + 1
     ) {
    return PN532_INVALID_FRAME;
  }

#if 0
  // TODO
  // Copy response data to output
  if (!pResp) {
    return PN532_INVALID_PARAM;
  }
#endif

#if DEBUG_PN532_PACKET
  LOGD(LOG_TAG, "I - %02d - 0x", len);
  for (uint8 i = 0; i < len; i++) {
    LOG("%02x ", *(pPacket + i));
  }
  LOG("\r\n");
#endif

  return PN532_GOOD;
}

uint8 PN532_RespHandler(void)
{
  // TODO
  return 0;
}

/**
 * Input
 * - Frame bytes
 * Output
 * - Pointer to Packet Data (position in input frame)
 * - Packet Data Length (Not include TFI)
 * Return
 * - TFI (frame identifier) for valid frame (0 for unknown)
 * - Other for invlid frame or error
 */
uint8 PN532_FrameParser(const uint8 *pFrame, uint8 frmLen, void **ppPacket, uint8 *pLen)
{
  uint8 len = 0;
  uint8 sum = 0;
  uint8 idx = 0;
  uint8 checksum = 0;
  const uint8 *p = pFrame;

  if (!pFrame || frmLen < PN532_FRAME_LEN_MIN) {
    return PN532_INVALID_FRAME;
  }

  // Find Frame Start
  for (idx = 0; idx < frmLen - PN532_FRAME_LEN_MIN; idx++) {
    if (p[0] == PN532_PREAMBLE
        && p[1] == PN532_STARTCODE_0
        && p[2] == PN532_STARTCODE_1
       ) {
      break;
    }
    p += 1;
  }
  if (idx == frmLen - PN532_FRAME_LEN_MIN) {
    return PN532_INVALID_FRAME;
  }

  // Special Frame - ACK/NACK
  if (p[3] == 0x00
      && p[4] == 0xFF
     ) {
    return PN532_TFI_ACK;
  }
  if (p[3] == 0xFF
      && p[4] == 0x00
     ) {
    return PN532_TFI_NACK;
  }
  // Extended Frame
  if (p[3] == 0xFF
      && p[4] == 0xFF
     ) {
    return PN532_NOT_SUPPORT;
  }

  // Length and Checksum
  len = p[3];
  if (!len
      || len > PN532_DATABUF_MAX    // FIXME: PN532 support 255 as MAX. Extra limitation here!
     ) {
    return PN532_INVALID_FRAME;
  }
  checksum = ~len + 1;
  if (checksum != p[4]) {
    return PN532_INVALID_FRAME;
  }
  // Data Checksum
  sum = p[5];                       // TFI
  idx = 6;                          // Position of the next byte data to be processed
  for (uint8 i = 0; i < len - 1; i++) {
    sum += p[6 + i];
    idx += 1;
  }
  checksum = ~sum + 1;
  if (checksum != p[idx]) {
    return PN532_INVALID_FRAME;
  }

  // Postamble
  // No need to check, just ignore it

  if (ppPacket
      && len >= 1                   // Length include TFI, thus 1 for no data
     ) {
    *ppPacket = (void *)&p[6];
  }
  if (pLen) {
    *pLen = len - 1;                // Not include TFI
  }

  return p[5];
}

void PN532_WakeUp(void)
{
  // TODO
}

void PN532_Test(void)
{
  uint8 buf[8];
  uint8 res = 0;
  PN532_FirmwareVersion_t fwVer;
  PN532_InListPassiveTarget_Cmd_t cmd = { 0x01, 0x00, NULL, 0 };
  PN532_InListPassiveTarget_Resp_t resp = { 0x00, 0x00, buf, 0 };

  res = PN532_GetFirmwareVersion(&fwVer);
  LOGD(LOG_TAG, "Get_FW_Ver: 0x%02x\r\n", res);

  res = PN532_InListPassiveTarget(&cmd, &resp);
  LOGD(LOG_TAG, "InLstPasTg: 0x%02x\r\n", res);
}

void PN532_Init(void)
{
  // TODO
}

/* vim: set ts=2 sw=2 tw=0 list : */
