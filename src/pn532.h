#ifndef __PN532_H__
#define __PN532_H__

/*============================================================================
 * Include files
 */

/*============================================================================
 * Macros
 */
#define PN532_TEST                  1

#define PN532_IRQ_PIN               P0_6

#define PN532_I2C_ADDRESS           0x24

// MIN of frame length is - the length of ACK/NACK frame - 6
// FIXME:
// Use 5 as MIN instead of 6, as it seems POSTAMBLE byte can be ignore
#define PN532_FRAME_LEN_MIN         5

/**
 * Frame Byte
 */
#define PN532_PREAMBLE              0x00
#define PN532_STARTCODE_0           0x00
#define PN532_STARTCODE_1           0xFF
#define PN532_POSTAMBLE             0x00

#define PN532_TFI_HOST2PN           0xD4
#define PN532_TFI_PN2HOST           0xD5

/**
 * ACK/NACK/ERROR frame is special frame (without TFI) defined in PN532 user manual.
 * Here define code for these frame to be consistent with normal frame.
 */
#define PN532_TFI_ACK               0x00
#define PN532_TFI_NACK              0xFF
#define PN532_TFI_ERROR             0x7F

#define PN532_ACK_WAIT_TIME         10      // ms, timeout of waiting for ACK

/**
 * Status / Result
 */
#define PN532_GOOD                  0x00    //
#define PN532_ACK                   0x00    //
#define PN532_NACK                  0x01    //

#define PN532_ERR                   0xFF    //
#define PN532_BUS_BUSY              0xFE    //
#define PN532_NOT_SUPPORT           0xFD    //
#define PN532_INVALID_PARAM         0xFC    //
#define PN532_INVALID_ACK           0xFB    //
#define PN532_INVALID_FRAME         0xFA    //
#define PN532_INVALID_RESP          0xF9    //
#define PN532_TIMEOUT               0xF8    //

/**
 * Commands
 *
 * The frame from Host Controller to PN532 in the following format
 * (CC in two represented frame, CC = Command Code)
 * +----+----+----+-----+-----+----+------+----------------------+-----+-----+
 * | 00 | 00 | FF | LEN | LCS | D4 | CC   | Optional Input Data  | DCS | 00 |
 * +----+----+----+-----+-----+----+------+----------------------+-----+-----+
 * or
 * +----+----+----+-----+-----+----+------+----------------------+-----+-----+
 * | 00 | 00 | FF | LEN | LCS | D5 | CC+1 | Optional output Data | DCS | 00 |
 * +----+----+----+-----+-----+----+------+----------------------+-----+-----+
 *
 * Commands for Initiator Mode
 * - Initialization / Activation:
 *   - InJumpForDEP
 *   - InJumpForPSL
 *   - InListPassiveTarget • InAutoPoll
 *   - InATR
 *   - InPSL
 * - Data Exchange:
 *   - InDataExchange
 *   - InCommunicateThru
 * - Selection / De-Selection / Release:
 *   - InSelect
 *   - InDeselect • InRelease
 *
 * Commands for Target mode
 * - Initialization:
 *   - TgInitAsTarget
 *   - TgSetGeneralBytes
 * - Data Exchange:
 *   - TgGetData
 *   - TgSetData
 *   - TgSetMetaData
 *   - TgGetInitiatorCommand
 *   - TgResponseToInitiator
 */
#define PN532_CMD_DIAGNOSE                  0x00
#define PN532_CMD_GETFIRMWAREVERSION        0x02
#define PN532_CMD_GETGENERALSTATUS          0x04
#define PN532_CMD_READREGISTER              0x06
#define PN532_CMD_WRITEREGISTER             0x08
#define PN532_CMD_READGPIO                  0x0C
#define PN532_CMD_WRITEGPIO                 0x0E
#define PN532_CMD_SETSERIALBAUDRATE         0x10
#define PN532_CMD_SETPARAMETERS             0x12
#define PN532_CMD_SAMCONFIGURATION          0x14
#define PN532_CMD_POWERDOWN                 0x16
#define PN532_CMD_RFCONFIGURATION           0x32
#define PN532_CMD_RFREGULATIONTEST          0x58
#define PN532_CMD_INJUMPFORDEP              0x56
#define PN532_CMD_INJUMPFORPSL              0x46
#define PN532_CMD_INLISTPASSIVETARGET       0x4A
#define PN532_CMD_INATR                     0x50
#define PN532_CMD_INPSL                     0x4E
#define PN532_CMD_INDATAEXCHANGE            0x40
#define PN532_CMD_INCOMMUNICATETHRU         0x42
#define PN532_CMD_INDESELECT                0x44
#define PN532_CMD_INRELEASE                 0x52
#define PN532_CMD_INSELECT                  0x54
#define PN532_CMD_INAUTOPOLL                0x60
#define PN532_CMD_TGINITASTARGET            0x8C
#define PN532_CMD_TGSETGENERALBYTES         0x92
#define PN532_CMD_TGGETDATA                 0x86
#define PN532_CMD_TGSETDATA                 0x8E
#define PN532_CMD_TGSETMETADATA             0x94
#define PN532_CMD_TGGETINITIATORCOMMAND     0x88
#define PN532_CMD_TGRESPONSETOINITIATOR     0x90
#define PN532_CMD_TGGETTARGETSTATUS         0x8A

#define PN532_RSP_INDATAEXCHANGE            0x41
#define PN532_RSP_INLISTPASSIVETARGET       0x4B


#define PN532_FWVER_IC                      0x32
#define PN532_FWVER_SUP_ISO14443A           (1 << 0)
#define PN532_FWVER_SUP_ISO14443B           (1 << 1)
#define PN532_FWVER_SUP_ISO18092            (1 << 2)


/*============================================================================
 * Enums
 */


/*============================================================================
 * Type Definitions
 */
typedef signed   char   int8;     //!< Signed 8 bit integer
typedef unsigned char   uint8;    //!< Unsigned 8 bit integer
typedef signed   short  int16;    //!< Signed 16 bit integer
typedef unsigned short  uint16;   //!< Unsigned 16 bit integer
typedef signed   long   int32;    //!< Signed 32 bit integer
typedef unsigned long   uint32;   //!< Unsigned 32 bit integer
typedef unsigned char   bool;     //!< Boolean data type

/**
 * Communication between the host controller and the PN532 is performed through frames,
 * in a half-duplex mode.
 * Four different types of frames are used in one or both directions (host controller
 * to the PN532 and PN532 to the host controller).
 */

// Normal Information Frame
// +----+----+----+-----+-----+-----+------------+-----+-----+
// | 00 | 00 | FF | LEN | LCS | TFI | PD0...PNn  | DCS | 00  |
// +----+----+----+-----+-----+-----+------------+-----+-----+
//
// The amount of data that can be exchanged using this frame structure is limited to
// 255 bytes (including TFI).
//
// Packet Length Checksum (LCS) byte satisfies the relation:
// Lower byte of [LEN + LCS] = 0x00
//
// Data Checksum (DCS) byte satisfies the relation:
// Lower byte of [TFI + PD0 + PD1 + ... + PDn + DCS] = 0x00
typedef struct {
  uint8 preamble;                   // 0x00
  uint8 spc[2];                     // Start of Packet Code, 0x00 FF
  uint8 len;                        // Packet Length (TFI and PD0 to PDn)
  uint8 lcs;                        // Packet Length Checksum
  uint8 tfi;                        // Specific PN532 Frame Identifier, 0xD4: Host -> PN532, 0xD5: PN532 -> Host
  uint8 *data;                      // PD0~PDn, PD0 is the Command Code
  uint8 dcs;                        // Packet Data Checksum
  uint8 postamble;                  // 0x00
} PN532_InfoFrame_t;

// Extended Information Frame
// +----+----+----+----+----+------+------+-----+-----+------------+-----+-----+
// | 00 | 00 | FF | FF | FF | LENM | LENL | LCS | TFI | PD0...PNn  | DCS | 00  |
// +----+----+----+----+----+------+------+-----+-----+------------+-----+-----+
//
// In the firmware implementation of the PN532, the maximum length of the packet data
// is limited to 264 bytes (265 bytes with TFI included).
//
// LENGTH = LENM x 256 + LENL coding the number of bytes in the data field
// (TFI and PD0 to PDn)
//
// The host controller, for sending frame whose length is less than 255 bytes, can also
// use this type of frame.
// But, the PN532 always uses the suitable type of frame, depending on the length
// (Normal Information Frame for frame <= 255 bytes and Extended Information Frame for
// frame > 255 bytes).
typedef struct {
  uint8 preamble;                   // 0x00
  uint8 spc[2];                     // Start of Packet Code, 0x00 FF
  uint8 len0;                       // Normal Packet Length, Fixed to be 0xFF
  uint8 lcs0;                       // Normal Packet Length Checksum, Fixed to be 0xFF
  uint8 lenM;                       // Packet Length High Byte (MSB)
  uint8 lenL;                       // Packet Length Lower Byte (LSB)
  uint8 lcs;                        // Packet Length Checksum
  uint8 tfi;                        // Specific PN532 Frame Identifier, 0xD4: Host -> PN532, 0xD5: PN532 -> Host
  uint8 *data;                      // PD0~PDn, PD0 is the Command Code
  uint8 dcs;                        // Packet Data Checksum
  uint8 postamble;                  // 0x00
} PN532_ExtInfoFrame_t;

// The specific ACK frame is used for the synchronization of the packets and
// also for the abort mechanism.
// +----+----+----+----+----+----+
// | 00 | 00 | FF | 00 | FF | 00 |
// +----+----+----+----+----+----+
// The specific NACK frame is used for the synchronization of the packets.
// +----+----+----+----+----+----+
// | 00 | 00 | FF | FF | 00 | 00 |
// +----+----+----+----+----+----+
typedef struct {
  uint8 preamble;                   // 0x00
  uint8 spc[2];                     // Start of Packet Code, 0x00 FF
  uint8 code[2];                    // Packet Code, ACK: 0x00 FF, NACK: 0xFF 00
  uint8 postamble;                  // 0x00
} PN532_AckFrame_t;

// The syntax error frame is used to inform the host controller that the PN532
// has detected an error at the application level.
// +----+----+----+----+----+----+----+----+
// | 00 | 00 | FF | 01 | FF | 7F | 81 | 00 |
// +----+----+----+----+----+----+----+----+
typedef struct {
  uint8 preamble;                   // 0x00
  uint8 spc[2];                     // Start of Packet Code, 0x00 FF
  uint8 len;                        // Packet Length, 0x01
  uint8 lcs;                        // Packet Length Checksum, Fixed to be 0xFF
  uint8 code;                       // Specific Application Level Error Code, 0x7F
  uint8 dcs;                        // Packet Data Checksum, 0x81
  uint8 postamble;                  // 0x00
} PN532_ErrorFrame_t;

// +----+----+----+-----+-----+---------+
// | D5 | 03 | IC | Ver | Rev | Support |
// +----+----+----+-----+-----+---------+
typedef struct {
  uint8 ic;
  uint8 ver;
  uint8 rev;
  uint8 support;
} PN532_FirmwareVersion_t;

// +----+----+-------+------+------------+
// | D4 | 4A | MaxTg | BrTy | [InitData] |
// +----+----+-------+------+------------+
typedef struct {
  uint8 maxTg;
  uint8 brTy;
  uint8 *initData;
  uint8 dataLen;
} PN532_InListPassiveTarget_Cmd_t;

// +----+----+------+----------------+----------------+
// | D5 | 4B | NbTg | [TgtData1 [] ] | [TgtData2 [] ] |
// +----+----+------+----------------+----------------+
typedef struct {
  uint8 nbTg;
  uint8 brTy;
  uint8 *tgtData;
  uint8 dataLen;
} PN532_InListPassiveTarget_Resp_t;

/*============================================================================
 * Prototypes
 */
uint8 PN532_Write(uint8 *data, uint8 len);
uint8 PN532_Read(uint8 *data, uint8 len);

uint8 PN532_SendCmd(uint8 cmd, uint8 *pCmdData, uint8 dataLen, uint8 waitAck);
uint8 PN532_SendAck(void);
int8  PN532_ReadAck(void);
int8  PN532_ReadRsp(uint8 *pResp);
uint8 PN532_RespHandler(void);
uint8 PN532_FrameParser(const uint8 *pFrame, uint8 frmLen, void **ppPacket, uint8 *pLen);

uint8 PN532_GetFirmwareVersion(PN532_FirmwareVersion_t *pVer);
uint8 PN532_InListPassiveTarget(PN532_InListPassiveTarget_Cmd_t *pCmd, PN532_InListPassiveTarget_Resp_t *pResp);

void PN532_WakeUp(void);

void PN532_Test(void);

void PN532_Init(void);

#endif /*  __PN532_H__  */

/* vim: set ts=2 sw=2 tw=0 list : */
