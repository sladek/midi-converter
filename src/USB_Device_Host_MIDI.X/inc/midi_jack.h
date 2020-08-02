/* 
 * File:   midi_jack.h
 * Author: sladekm
 *
 * Created on April 11, 2020, 4:36 PM
 */

#ifndef MIDI_JACK_H
#define	MIDI_JACK_H

/* Code Index Number (CIN) values */
/*   Table 4-1 of midi10.pdf      */
#define MIDI_CIN_MISC_FUNCTION_RESERVED         0x0
#define MIDI_CIN_CABLE_EVENTS_RESERVED          0x1
#define MIDI_CIN_2_uint8_t_MESSAGE              0x2
#define MIDI_CIN_MTC                            0x2
#define MIDI_CIN_SONG_SELECT                    0x2
#define MIDI_CIN_3_uint8_t_MESSAGE              0x3
#define MIDI_CIN_SSP                            0x3
#define MIDI_CIN_SYSEX_START                    0x4
#define MIDI_CIN_SYSEX_CONTINUE                 0x4
#define MIDI_CIN_1_uint8_t_MESSAGE              0x5
#define MIDI_CIN_SYSEX_ENDS_1                   0x5
#define MIDI_CIN_SYSEX_ENDS_2                   0x6
#define MIDI_CIN_SYSEX_ENDS_3                   0x7
#define MIDI_CIN_NOTE_OFF                       0x8
#define MIDI_CIN_NOTE_ON                        0x9
#define MIDI_CIN_POLY_KEY_PRESS                 0xA
#define MIDI_CIN_CONTROL_CHANGE                 0xB
#define MIDI_CIN_PROGRAM_CHANGE                 0xC
#define MIDI_CIN_CHANNEL_PREASURE               0xD
#define MIDI_CIN_PITCH_BEND_CHANGE              0xE
#define MIDI_CIN_SINGLE_uint8_t                 0xF

//Possible system command status uint8_ts
#define MIDI_STATUS_SYSEX_START                 0xF0
#define MIDI_STATUS_SYSEX_END                   0xF7
#define MIDI_STATUS_MTC_QFRAME                  0xF1
#define MIDI_STATUS_SONG_POSITION               0xF2
#define MIDI_STATUS_SONG_SELECT                 0xF3
#define MIDI_STATUS_TUNE_REQUEST                0xF6
#define MIDI_STATUS_MIDI_CLOCK                  0xF8
#define MIDI_STATUS_MIDI_TICK                   0xF9
#define MIDI_STATUS_MIDI_START                  0xFA
#define MIDI_STATUS_MIDI_STOP                   0xFC
#define MIDI_STATUS_MIDI_CONTINUE               0xFB
#define MIDI_STATUS_ACTIVE_SENSE                0xFE
#define MIDI_STATUS_RESET                       0xFF

#define MIDI_UART_BUFFER_SIZE               (uint8_t)64

void MIDI_Jack_Initialize();

//A circular buffer for storing received bytes from the host, out the UART to the attached device on the MIDI Out port
extern uint8_t        MIDIOutTxBuffer[MIDI_UART_BUFFER_SIZE];
extern uint8_t*       pTxBufReadLocation;
extern uint32_t*      pTxBufWriteLocation;        // This is a double word (4 bytes), since we will be writing a double word at a time
extern uint8_t        MIDIOutByteCount;

//A circular buffer for storing received bytes from the MIDI (UART), to be sent do attached USB MIDI device
extern uint8_t        MIDIInRxBuffer[MIDI_UART_BUFFER_SIZE];
extern uint8_t*       pRxUARTBufReadLocation;
extern uint8_t*       pRxUARTBufWriteLocation;
extern uint8_t        UARTInByteCount;

extern uint8_t        RxRunningStatusByte;
extern uint8_t        TxRunningStatusByte;

#endif	/* MIDI_JACK_H */

