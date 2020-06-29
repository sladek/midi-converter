/*******************************************************************************
  Easy Bootloader Library for PIC24/dsPIC33/PIC32MM Library API Source File

  Summary:
    Installs a .bl2 Application image file to the local flash memory from an
    EZBL_FIFO based remote communications interface (ex: UART or I2C).

 *******************************************************************************/

/*******************************************************************************
  Copyright (C) 2018 Microchip Technology Inc.

  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
  derivatives created by any person or entity by or on your behalf, exclusively
  with Microchip's products.  Microchip and its licensors retain all ownership
  and intellectual property rights in the accompanying software and in all
  derivatives here to.

  This software and any accompanying information is for suggestion only.  It
  does not modify Microchip's standard warranty for its products.  You agree
  that you are solely responsible for testing the software and determining its
  suitability.  Microchip has no obligation to modify, test, certify, or
  support the software.

  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
  TERMS.
*******************************************************************************/

#if defined(EZBL_LIB)
#include "../ezbl.h"
#else
#include "ezbl_integration/ezbl.h"
#endif


extern volatile unsigned int __attribute__((section("sfrs"))) IEC0CLR;
extern volatile unsigned int __attribute__((section("sfrs"))) IPC0;
extern volatile unsigned int __attribute__((section("sfrs"))) NVMKEY;
extern volatile unsigned int __attribute__((section("sfrs"))) NVMPWP;
extern volatile unsigned int __attribute__((section("sfrs"))) NVMBWP;


int __attribute__((weak)) EZBL_PreInstall(EZBL_FIFO *rxFromRemote, EZBL_FIFO *txToRemote, unsigned long fileSize, EZBL_APPID_VER *appIDVer);


//#define EZBL_DEBUG     // Uncomment to allow debugging messages to be printed to stdout
//#define VERBOSE        // Uncomment to have verbose printing of all flash write commands and data

#if !defined(EZBL_DEBUG)
#define EZBL_printf(...)        do{}while(0)
#define EZBL_DumpFlashDiff(...) do{}while(0)
#endif


/**
 * Reads a .bl2 file from the communications medium, checks that the offering
 * has a matching BOOTID_HASH, optionally checks that the APPID_VER field is
 * acceptable, erases all App-space flash pages, programs and read-back verifies
 * the .bl2 data records in App-space, verifies overall file and communications
 * integrity, and programs a flag indicating the new Application is valid and
 * available for execution.
 *
 * If the target device supports Dual Partition flash hardware and is configured
 * for a Dual Partition mode, the EZBL_Install2IP() function may be appropriate
 * instead of this one. EZBL_Install2IP() explicitly targets only the Inactive
 * Partition and excludes some code applicable to Single Partition bootloading.
 *
 * This function is non-blocking and will return, even if an active bootload
 * operation is underway. To ensure bootload operations are completely handled,
 * this function should be called periodically anytime bootloader functionality
 * is desired.
 *
 * If this function is invoked while an existing Application is executing and
 * the observed .bl2 header matches the installation requirements, then all
 * non-bootloader ISRs are disabled, the existing Application is erased, and
 * this function changes to a blocking function that never returns. Instead,
 * at bootload termination (success or failure), a device reset is done to
 * reenter the bootloader with a clean stack and device state to allow new
 * Application launch (or bootload retry if failure occurred).
 *
 * Because reception, decryption erase and programming of a whole .bl2 file
 * could take an extended period, this function may internally execute the
 * CLRWDT opcode before NVM erase/programming operations. As communications
 * operations generally do not have deterministic timing, you must NOT enable
 * the watchdog timer in a Windowed mode.
 *
 * @param *s Pointer to caller allocated EZBL_INSTALL_CTX structure that must
 *           remain within scope anytime a bootload session occurs. Before
 *           calling EZBL_Install2Flash() for the first time, set the following
 *           two fields:
 *              s.state = SM_SYNC_INIT;
 *              s.timeout = (application defined NOW timeout value);
 *           For the timeout, specify the timeout in some combination of
 *           NOW_sec and/or NOW_ms, ex: s.timeout = NOW_sec*1u; for a one second
 *           communications idle timeout. Higher latency communications mediums,
 *           such as wireless links, will likely require more than 1 second.
 *
 *           On subsequent calls to EZBL_Install2Flash(), pass the *s parameter
 *           back to the function with unchanged structure elements.
 *
 * @param BOOTID_HASH0, BOOTID_HASH1, BOOTID_HASH2, BOOTID_HASH3 project
 *        symbols. These are generated as a SHA-256 hash (truncated to 16-bytes)
 *        of the concatenated BOOTID_VENDOR, BOOTID_MODEL, BOOTID_NAME and
 *        BOOTID_OTHER strings defined in your EZBL makefile
 *        (i.e.: 'ezbl_boot.mk', which is included in 'Makefile').
 *
 * @param APPID_VER_MAJOR, APPID_VER_MINOR, APPID_VER_BUILD project symbols
 *        defined in the last installed Application's ezbl makefile (i.e.: 
 *        'ezbl_app.mk', which is included in 'Makefile'). These get stored in 
 *        the EZBL_appBootloadState flash address as part of the Application, so
 *        will not exist if the Application has been erased.
 *
 * @param EZBL_NO_APP_DOWNGRADE optional project symbol. Set to 1 using
 *        EZBL_SetSYM() if you wish to trigger pre-erase/programming abort when
 *        the offered .bl2 image file contains
 *        APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD version meta data that
 *        indicates the firmware is less than the presently executing
 *        application version (as defined by EZBL_appIDVer.appIDVer64).
 *
 * @param EZBL_FLOW_THRESHOLD optional project symbol. Provides loose control
 *        over flow control advertisements, as a byte count. Anytime the
 *        EZBL_COM_RX fifo contains >= EZBL_FLOW_THRESHOLD bytes worth of
 *        freespace available, a flow control message is generated, so smaller
 *        values for EZBL_FLOW_THRESHOLD cause more flow control advertisements
 *        to be generated while larger values can reduce overhead (at the
 *        possible expense of communications "burstiness").
 *
 *        If unspecified, the default value for EZBL_FLOW_THRESHOLD is 18 bytes.
 *
 *        To set this value, use the EZBL_SetSYM() macro in a project level
 *        source file.
 *
 * @param *EZBL_COM_TX Global pointer to a communications transmit FIFO for flow
 *        control and status messages. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_TX = &UART_TxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_TX = &I2C_TxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes.
 *
 * @param *EZBL_COM_RX Global pointer to a communications receive FIFO to read
 *        the .bl2 firmware image from. As an example, to use the default UART
 *        implementation, set:
 *          EZBL_COM_RX = &UART_RxFifo;
 *        To use an I2C slave medium, set:
 *          EZBL_COM_RX = &I2C_RxFifo;
 *
 *        By default, this parameter can be ignored as the associated
 *        communications RX ISR(s) contains a BOOTLOADER_WAKE_KEY definition,
 *        which causes the ISR to automatically set both EZBL_COM_TX and
 *        EZBL_COM_RX at the correct time based on an incoming data
 *        synchronization match of at least 8 bytes. After this initial 8 byte
 *        key match, this function internally requires a BOOT_ID match, ensuring
 *        at least 16 more bytes are valid EZBL communications data.
 *
 *        Upon termination of the bootload operation (due to idle timeout,
 *        bootload failure, offered version mismatch, successful reprogramming,
 *        or any other cause), EZBL_COM_RX is set to 0 (null) to place this
 *        EZBL_Install2Flash() function into a dormant/passive state (requires
 *        new ISR BOOTLOADER_WAKE_KEY match to wake up).
 *
 * @return <ul>
 *              <li>0 - normal operation. When in the passive state, this is the
 *                      immediate function response.</li>
 *              <li>1 - bootload-end event detected. A bootload-end event does
 *                      not necessarily indicate new firmware is available or
 *                      was attempted to be uploaded as COM RX idle timeouts are
 *                      also reported.</li>
 *         <ul>
 *
 *         When the return value is 1, read the s->bootCode parameter to
 *         determine the bootload-end cause.
 *
 *         For s->bootCode:
 *         Generally zero or greater is good, while negative values indicate a
 *         failure. Anything erased/programmed prior to a failure is left as it
 *         was upon failure detection (will automatically be cleaned up on next
 *         bootload attempt).
 *
 *         Specific return code meanings are:
 *         <ul>
 *          <li>EZBL_ERROR_SUCCESS (1): Operation completed successfully. The
 *              new application is ready for execution.</li>
 *          <li>EZBL_ERROR_COM_READ_TIMEOUT (-20): Bootloader signaled 
 *              communications timeout attempting to read firmware image.</li>

 *          <li>EZBL_ERROR_IMAGE_MALFORMED (-21): Bootloader rejected firmware
 *              as malformed or of unsupported type. Possible communications
 *              error.</li>
 *          <li>EZBL_ERROR_BOOTID_HASH_MISMATCH (-22): Bootloader rejected
 *              firmware as incompatible (with this Bootloader project).</li>
 *          </li>EZBL_ERROR_APPID_VER_MISMATCH (-23): Bootloader rejected
 *              firmware as out of the required programming order.
 *
 *              This error is only generated when the EZBL_NO_APP_DOWNGRADE
 *              symbol is non-zero and the received
 *              APPID_VER_MAJOR.APPID_VER_MINOR.APPID_VER_BUILD information in
 *              the file is less than the presently executing Application ID
 *              version. If no Application exists, this error will not be
 *              generated.</li>
 *          <li>EZBL_ERROR_HARD_VERIFY_ERROR (-24): Bootloader read-back
 *              verification failure.</li>
 *          <li>EZBL_ERROR_SOFT_VERIFY_ERROR (-25): Bootloader read-back
 *              verification mismatch in reserved address range.</li>
 *          <li>EZBL_ERROR_IMAGE_CRC (-26): Bootloader computed CRC mismatch
 *              with CRC contained in firmware image. Probable communications
 *              error.</li>
 *          <li>No return: Unhandled exception or unknown. (Image file may have
 *              records targeting unimplemented memory, causing an Address Error
 *              Trap upon verification, or electrical operating parameters are
 *              incorrect.) Ensure the image file was compiled specifically
 *              for this device and bootloader. For unhandled exceptions, the
 *              device will automatically reset back into the bootloader.</li>
 *         </ul>
 */
int EZBL_Install2Flash(EZBL_INSTALL_CTX *s)
{
    union
    {
        unsigned long long  u64[6];
        unsigned long       u32[12];
        unsigned short      u16[24];
        unsigned char        u8[48];
    } buf;                              // Can't be less than 48-bytes, preferably matches EZBL_FLOW_THRESHOLD: won't be better when bigger than necessary
    unsigned long now;
    unsigned int i;
    unsigned int dataCount;
    int ret;                            // Variable for temporary signed return codes
    EZBL_MISMATCH_STATUS verifyStatus;  // Not used, but useful to have available if debugging programming failures (doesn't really cost much since it is on the stack)
    int moreWork;                       // Flag indicating we should restart the state machine for another execution pass
    int workDone;                       // Flag indicating something useful happened such that we should reset the idle timeout
    int cantReturn;                     // Flag indicating if we were called while an App was running and now we've erased it
    EZBL_INSTALLED_FLAGS installFlags;



    // Return immediately if the Bootloader's RX communications channel has been
    // disabled (i.e. no UUUUUUUUMCUPHCME unlock sequence detected in ISR yet)
    if(!EZBL_COM_RX)
    {
        s->state = SM_SYNC_INIT;
        return 0;
    }


    cantReturn = 0;
    workDone = 0;
    do
    {
        moreWork = 0;
        dataCount = EZBL_COM_RX->dataCount;

        switch(s->state)
        {
            case SM_SYNC_INIT:  // Look for "BL2B" file start. Throws away any preceeding data, such as the "UUUUUUUUMCUPHCME" auto-baud/synchronization/bootloader wake up string decoded in the RX ISR.
                if(dataCount < sizeof(buf))                                 // Wait for at least 48 bytes to arrive. This will help give time for external code executing in main() to process incoming RX data if it falsely triggers bootloader wakeup.
                    return 0;
                workDone = 1;
                EZBL_FIFOPeek(&buf, EZBL_COM_RX, sizeof(buf));              // See if "BL2B" FILE_ID header sitting in FIFO
                for(i = 0; i < sizeof(buf)-4u; i++)
                {
                    if(buf.u8[i] != (unsigned char)'B')
                        continue;
                    if((buf.u8[i+1] ^ (unsigned char)'L') | (buf.u8[i+2] ^ (unsigned char)'2') | (buf.u8[i+3] ^ (unsigned char)'B'))
                        continue;
                    EZBL_printf("\n\n  Found BL2B header");
                    EZBL_FIFORead(0, EZBL_COM_TX, EZBL_COM_TX->dataCount);  // Throw all data in TX FIFO, if any, in the null bit bucket. This is needed for I2C which could have old failure codes laying around if the PC stopped polling us last time before we gave up.
                    s->state = SM_GET_HEADER;
                    moreWork = 1;
                    break;
                }
                EZBL_FIFORead(0, EZBL_COM_RX, i);                           // No match, eat all unmatchable locations before testing again
                break;

            case SM_GET_HEADER: // "BL2B" (4-byte), File Length (4-byte), BOOTID_HASH (16-byte), APPID_VER (8-byte), HMAC-SHA-256 (16-byte)
                if(dataCount < 48u)
                    break;
                workDone = 1;
                EZBL_FIFORead(&buf, EZBL_COM_RX, 48);
                s->crc32 = EZBL_CRC32(0x00000000, &buf, 48);                // Start CRC32 computation to validate the whole communications stream/whole file integrity
                s->bytesToEOF = buf.u32[1] - 16u - 8u - 16u;                // Get File Length; Subtract 16+8+16 bytes for size of BOOTID_HASH, APPID_VER, and HMAC-SHA-256 data fields in the header (done later after potentially using this value)
                s->bootCode = EZBL_ERROR_SUCCESS;                           // Default value
                EZBL_printf("\n  BOOTID_HASH offered: %016llX%016llX", buf.u64[2], buf.u64[1]);
                EZBL_printf("\n                 ours: %016llX%016llX", EZBL_bootIDHash.bootIDHash64[1], EZBL_bootIDHash.bootIDHash64[0]);
                if(EZBL_RAMCompare(&EZBL_bootIDHash, &buf.u64[1], 16))      // Check BOOTID_HASH field matches our Bootloader's bootIDHash
                {
                    EZBL_printf("\n  \x1B[31mBOOID_HASH mismatch\x1B[0m");
                    s->bootCode = EZBL_ERROR_BOOTID_HASH_MISMATCH;
                    s->state = SM_IGNORING;
                    break;
                }

                // Check that APPID_VER is >= APPID_VER that we are presently running
                s->appVer.appIDVer64 = buf.u64[3];                          // Get APPID_VER offering
                EZBL_printf("\n  APPID_VER: offered = %u.%02u.%04lu"
                            "\n             ours    = %u.%02u.%04lu%s", s->appVer.appIDVerMajor, s->appVer.appIDVerMinor, s->appVer.appIDVerBuild, EZBL_appBootloadState.appVer.appIDVerMajor, EZBL_appBootloadState.appVer.appIDVerMinor, EZBL_appBootloadState.appVer.appIDVerBuild, EZBL_appBootloadState.appInstalled == EZBL_APP_INSTALLED ? "" : " (no App present)");
                if(EZBL_WeakSYM(EZBL_NO_APP_DOWNGRADE))
                {
                    if(EZBL_appBootloadState.appInstalled == EZBL_APP_INSTALLED)             // Check that the structure is valid
                    {
                        if(s->appVer.appIDVer64 < EZBL_appBootloadState.appVer.appIDVer64)
                        {
                            EZBL_printf("\n  APPID_VER older than already installed (EZBL_NO_APP_DOWNGRADE flag enabled)");
                            s->bootCode = EZBL_ERROR_APPID_VER_MISMATCH;
                            s->state = SM_TERMINATE;
                            moreWork = 1;
                            break;
                        }
                    }
                }

                // Compute HMAC, if crypto is enabled
                // Compare 16-bytes of HMAC-SHA-256 data, if crypto is enabled
                // Use 16 extra bytes of HMAC-SHA-256 data as NONCE for AES-128 IV
                // If valid, begin AES-128 CTR decryption (actually encryption + XOR)
                // No crypto enabled, just verify HMAC field is all zero's
                for(i = 9; i < 12u; i++)
                {
                    buf.u32[8] |= buf.u32[i];
                }
                if(buf.u32[8])
                {   // Crypto appears to be enabled, check HMAC. NOTE: this
                    // should be done using a constant-time compare
                    // algorithm to avoid attack by sequentially sending
                    // HMACs that only increment the first byte until a
                    // compare duration is observed increasing.
                    s->bootCode = EZBL_ERROR_IMAGE_MALFORMED;
                    s->state = SM_TERMINATE;
                    moreWork = 1;
                    EZBL_printf("\n  HMAC invalid; terminating");
                    break;
                }

                if(EZBL_PreInstall)
                {
                    ret = EZBL_PreInstall(EZBL_COM_RX, EZBL_COM_TX, s->bytesToEOF + 64u, &s->appVer);
                    if(ret <= 0)
                    {
                        EZBL_printf("\n  EZBL_PreInstall() returned %i, so aborting update", ret);
                        s->bootCode = EZBL_ERROR_IMAGE_REJECTED;                // (-27)  // 0xFFE5 Bootloader or running application rejected the offered image;
                        s->state = SM_TERMINATE;
                        moreWork = 1;
                        break;
                    }
                }

                EZBL_printf("\n  Accepting offered firmware update: size = %lu", s->bytesToEOF + 64);   // 64 bytes for header (SYNC included)


                // If called from the running application, disable all but our own
                // interrupts since we are about to erase the previously running app
                cantReturn = EZBL_MFC0(EZBL_CP0_EBase) - (const unsigned int)&EZBL_BOOT_ebase_address;
                if(cantReturn)
                {
                    __builtin_disable_interrupts();
                    EZBL_MTC0(EZBL_CP0_EBase, (const unsigned int)&EZBL_BOOT_ebase_address);    // Change Ebase for the Bootloader to receive NMI, General Exceptions, and Interrupts
                    for(i = ((unsigned int)&IEC0CLR); i < (unsigned int)&IPC0; i += 0x10u)
                        *((volatile unsigned int*)i) = 0xFFFFFFFFu;                             // Clears every bit in all IECx Interrupt Enable registers
                    EZBL_ForwardAllIRQToBoot();

                    // Turn our interrupts back on
                    __builtin_enable_interrupts();
                    NOW_IntEnable();                                                            // NOW Timer/CCT Interrupt
                    EZBL_FIFOIntEnable(EZBL_COM_RX);                                            // Communications interrupts
                    EZBL_FIFOIntEnable(EZBL_COM_TX);
#if defined(EZBL_DEBUG)
                    EZBL_FIFOIntEnable(EZBL_STDOUT);
#endif
                }

                // Turn Application space hardware erase/write protect off and Bootloader space erase/write protect on
                NVMKEY = 0x00000000;    // Execute unlock sequence
                NVMKEY = 0xAA996655 ^ EZBL_bootIDHash.bootIDHash32[0] ^ buf.u32[1];;
                NVMKEY = 0x556699AA ^ EZBL_bootIDHash.bootIDHash32[1] ^ buf.u32[2];;
                NVMPWP = 0x80000000u | (((unsigned long)EZBL_Install2Flash) & 0x1FFFFFFFu);   // Don't lock further NVMPWP writes, set watermark to 0x1D002000 or the last page where the Bootloader sits

                // Also need this code if you want to erase/write Boot Flash (needed for Bootloader to be allowed to change Config words)
                NVMKEY = 0x00000000;    // Execute unlock sequence
                NVMKEY = 0xAA996655 ^ EZBL_bootIDHash.bootIDHash32[2] ^ buf.u32[3];
                NVMKEY = 0x556699AA ^ EZBL_bootIDHash.bootIDHash32[3] ^ buf.u32[4];
                NVMBWP = 0x00008400;    // Keep write protect on 0x1FC00000 to 0x1FC007FF (first Boot Flash page containing reset vector), unprotect Boot Flash page 2 and 3 (ICD Debug Executive + Flash Config Words pages)

                s->recHdr.len = 0;                  // Force first read to be a DATA_RECORD header and use this for Erase unlock keying
                s->recHdr.addr = 0;                 // Use this variable for erase state tracking
                s->getDataNextState = SM_IGNORING;  // Also used for Erase unlock keying
                s->state = SM_ERASE;
                // No break;
                EZBL_printf("\n  Erasing");

            case SM_ERASE:
                workDone = 1;                               // This state always does something
                if(EZBL_COM_TX->dataCount == 0u)            // Send -237 (0xFF13) keep-alive value while erasing
                    EZBL_FIFOWrite16(EZBL_COM_TX, 0xFF13);  // 0x13 is an XOFF character to allow rudimentary file upload via an EZBL protocol unaware application at slower baud rates
                if(cantReturn)
                    ClrWdt();
                EZBL_NVMKey += s->getDataNextState + (0xFC21u - SM_IGNORING) + ((unsigned int)s->recHdr.len);      // Generate required unlock value for EZBL_EraseAppSpace() of 0xFC21. Should evaluate as 0x0000 += SM_IGNORING + 0xFC21 - SM_IGNORING + 0x0000.
                if(EZBL_EraseAppSpace((unsigned int*)&s->recHdr.addr))  // Not done erasing yet, but in case if there is stuff to do elsewhere, return
                {
                    EZBL_printf(".");
                    break;                                  // execute without a long blocking delay, return periodically
                }
                EZBL_FIFOWrite16(EZBL_COM_TX, 0xFF11);      // 0x11 is an XON character to allow rudimentary file upload via a terminal application at slower baud rates
                dataCount = EZBL_COM_RX->dataCount;         // Potentially could have spent a long time linearly getting here from SM_GET_HEADER which ate bytes, so refresh the count available
                s->bytesRequested = 0;                      // Init flow control number
                //s->state = SM_SET_UP_GET;
                // No break
                EZBL_printf(" done");

            case SM_SET_UP_GET:
                workDone = 1;
                s->state = SM_GET_DATA;
                if(s->bytesToEOF <= 32u + 4u)                   // 32-byte SHA-256 file hash + 4-byte CRC32 just before EOF
                {
                    s->getDataBytes = 32u + 4u;
                    s->getDataNextState = SM_TEST_CHECKSUMS;
                    EZBL_printf("\n  Getting checksums");
                }
                else if(s->recHdr.len == 0u)
                {
                    s->getDataBytes = 8;                        // 8 bytes/record header
                    s->getDataNextState = SM_SET_UP_GET;
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                    EZBL_printf("\n  Getting header");
#endif
                }
                else                                            // Ordinary data record bytes
                {
                    EZBL_NVMKey += 0x03DF;                      // Unlock here with add so code must follow correct non-linear path to SM_PROG_REC_DATA in order to execute the NVM write
                    s->getDataBytes = 16;                       // Somewhat arbitrary read chunk size to suppress looping on overly tiny chunks of data
                    if(dataCount > 16u)
                        s->getDataBytes = (unsigned short)dataCount;
                    if(s->getDataBytes > sizeof(buf))
                        s->getDataBytes = sizeof(buf);
                    if((unsigned long)s->getDataBytes >= s->recHdr.len)
                    {   // Only fetch up to the end of this record
                        s->getDataBytes = s->recHdr.len16[0];
                    }
                    else
                    {   // When fetching less than the full record, make sure we always process data in 8 byte multiples so we can stay aligned to double word flash write size requirements
                        s->getDataBytes -= s->getDataBytes & 0x7u;
                    }
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                    EZBL_printf("\n  Getting %u bytes of prog data", (unsigned int)(s->getDataBytes));
#endif
                    s->getDataNextState = SM_PROG_REC_DATA;
                }
                // No break;

            case SM_GET_DATA:
                ret = EZBL_SYM(EZBL_FLOW_THRESHOLD);
                if(ret && (EZBL_COM_TX->fifoSize - EZBL_COM_TX->dataCount >= 6u))
                {
                    i = EZBL_COM_RX->fifoSize;
                    if((unsigned int)ret > i)   // Limit to real RX buffer space to ensure reliable results if multiple communications mediums (ex: UART + I2C) could be used at run-time and different FIFO sizes exist on them.
                        ret = (int)i;
                    i -= s->bytesRequested;
                    if((i >= (unsigned int)ret) || (s->getDataBytes > s->bytesRequested))
                    {
                        if(((unsigned char)i) == (unsigned char)0x13u)      // Adjust by one byte if a 0x13 XOFF character would appear in the stream
                            i--;
                        if(((unsigned char)(i>>8)) == (unsigned char)0x13u) // Also adjust by 0x100 bytes if a 0x13 XOFF character would appear in the second byte
                            i -= 0x0100u;
    #if defined(EZBL_DEBUG) && defined(VERBOSE)
                        EZBL_printf("\n  Requesting: %u bytes", i);
    #endif
                        s->bytesRequested += (unsigned short)i;
                        EZBL_FIFOWrite16(EZBL_COM_TX, i);                   // Transmit a 16-bit flow control data request (number of bytes of free space we have/can accept if sent to us)
                        workDone = 1;
                    }
                }
                if(dataCount < (unsigned int)s->getDataBytes)
                    break;
                workDone = 1;
                moreWork = 1;
                s->bytesRequested -= s->getDataBytes;
                s->bytesToEOF -= EZBL_FIFORead(&buf, EZBL_COM_RX, (unsigned int)s->getDataBytes);
                s->crc32 = EZBL_CRC32(s->crc32, &buf, (unsigned int)s->getDataBytes);
                s->state = s->getDataNextState;
                if(s->state == SM_SET_UP_GET)            // Copy buf data to recHdr if we came from the SM_SET_UP_GET state and are going back to it
                {
                    EZBL_RAMCopy(&s->recHdr, &buf, (unsigned int)s->getDataBytes);
                    if(s->recHdr.len8[3])      // Sanity check that length fits in 24-bits
                    {
                        s->bootCode = EZBL_ERROR_IMAGE_MALFORMED;
                        s->state = SM_TERMINATE;
                    }
                }
                break;

            case SM_PROG_REC_DATA:
                workDone = 1;
                moreWork = 1;
#if defined(EZBL_DEBUG) && defined(VERBOSE)
                EZBL_DumpFlashDiff("  Write: ", (void*)0, s->recHdr.addr, &buf, (void*)0, (unsigned int)s->getDataBytes);
                if(EZBL_NVMKey != 0x03DFu)
                    EZBL_printf("\n    EZBL_NVMKey MISMATCH = 0x%04X", EZBL_NVMKey);
#endif
                if(cantReturn)
                    ClrWdt();
                now = EZBL_WriteROMEx(s->recHdr.addr, &buf, (unsigned int)s->getDataBytes, 0, 0, 0);
                ret = EZBL_VerifyROM(s->recHdr.addr, &buf, (unsigned int)s->getDataBytes, &verifyStatus);
#if defined(EZBL_DEBUG)
                if(ret != 0)
                    EZBL_printf("\nEZBL_VerifyROM(%08lX, %08lX, %lu, %08lx) = 0x%08lX", s->recHdr.addr, (unsigned long)&buf, (unsigned long)s->getDataBytes, (unsigned long)&verifyStatus, (long)ret);
                unsigned char buf2[sizeof(buf)];
                EZBL_ReadPackedFlash(buf2, s->recHdr.addr, 0, s->getDataBytes);
                if(EZBL_RAMCompare(&buf, buf2, s->getDataBytes) != 0)
                {
                    EZBL_DumpFlashDiff("  Write: ", "  Read:  ", s->recHdr.addr, &buf, buf2, s->getDataBytes);
                    EZBL_printf("\n--------MISMATCH--------");
                }
#endif
                s->recHdr.addr = now;
                s->recHdr.len -= s->getDataBytes;
                s->state = SM_SET_UP_GET;
                if(ret < 0)
                {
                    s->bootCode = EZBL_ERROR_HARD_VERIFY_ERROR;                 // App-space data programmed does not match read-back value, even after adjusting for special addresses
                    s->state = SM_TERMINATE;
                }
                if(ret > 0)
                {
                    s->bootCode = EZBL_ERROR_SOFT_VERIFY_ERROR;                 // Bootloader reserved region data changed in image, but all App space data still checks out
                }
                break;

            case SM_TEST_CHECKSUMS:
                workDone = 1;
                EZBL_NVMKey += 0x03DF;
                if(s->crc32 == 0x2144DF1Cu)                                     // CRC32 computation includes the CRC32 at the very end, so magic constant 0x2144DF1C indicates validity of all data
                {   // All programming successful and .BL2 integrity verified, so program final EZBL_INSTALLED_FLAGS structure to mark the Application as valid/runnable
                    installFlags.appInstalled = EZBL_APP_INSTALLED;             // Set 0x12CD flag to indicate valid/runnable
                    installFlags.appVer.appIDVer64 = s->appVer.appIDVer64;      // Save 8 bytes of APPID_VER
                    installFlags.id.hash32 = buf.u32[7];                        // Save last 8 bytes of file: 4 bytes of Hash and CRC32
                    installFlags.id.CRC32  = buf.u32[8];
                    EZBL_printf("\n  CRC32 good: 0x%08lX\n\n", installFlags.id.CRC32);
                    EZBL_WriteROMEx((unsigned long)&EZBL_appBootloadState, &installFlags, sizeof(installFlags), 0, 0xDEAD, 0);
                    EZBL_WaitForNVMOpComplete();
                }
                else
                {   // CRC failure
                    s->bootCode = EZBL_ERROR_IMAGE_CRC;
                    EZBL_printf("\n  \x1B[31mCHECKSUM MISMATCH\x1B[0m"
                                "\n  File:     0x%08lX"
                                "\n  Expect:   0x2144DF1C"
                                "\n  Computed: 0x%08lX"
                                "\n"
                                "\n", buf.u32[8], s->crc32);
                }
                EZBL_COM_RX = 0;                                                // Stop processing data until reenabled by RX interrupt
                // no break; - need to send final status code shutdown immediately now that EZBL_COM_RX has been blanked

            case SM_TERMINATE:
                EZBL_NVMKey = 0;
                // Write protect all boot space, but do not hard lock it
                NVMKEY = 0x00000000;    // Execute unlock sequence
                NVMKEY = 0xAA996655;
                NVMKEY = 0x556699AA;
                NVMBWP = 0x00008700;
                // Write protect regular flash where bootloader exists
                NVMKEY = 0x00000000;
                NVMKEY = 0xAA996655;
                NVMKEY = 0x556699AA;
                NVMPWP = 0x80000000u | (((unsigned long)EZBL_Install2Flash) & 0x1FFFFFFFu);
                EZBL_printf("\n  Terminating: code = 0x%04X", (int)s->bootCode);
                workDone = 1;
                moreWork = 1;
                s->state = SM_IGNORING;
                EZBL_FIFOWrite32(EZBL_COM_TX, ((((unsigned long)s->bootCode)<<16)) | 0x0000u); // Send termination indicator value and reply back with EZBL_ERROR_SUCCESS, EZBL_ERROR_SUCCESS_VER_GAP, EZBL_ERROR_SOFT_VERIFY_ERROR or EZBL_ERROR_IMAGE_CRC
                EZBL_FIFOFlush(EZBL_COM_TX, NOW_ms);
                break;

            case SM_IGNORING:
                if(dataCount)
                {
                    workDone = 1;
                    if(dataCount >= s->bytesToEOF)
                    {
                        dataCount = s->bytesToEOF;
                        EZBL_FIFORead(0, EZBL_COM_RX, dataCount);               // Send RX data to the bit bucket until EOF is reached
                        EZBL_COM_RX = 0;                                        // Stop processing data until reenabled by RX interrupt
                    }
                    EZBL_FIFORead(0, EZBL_COM_RX, dataCount);                   // Send RX data to the bit bucket until EOF is reached. If we made EZBL_COM_RX null above, this function does nothing.
                    s->bytesToEOF -= dataCount;
                }
                break;

            default:    // Should not happen
                EZBL_ResetCPU();
                break;
        }

        // Track/handle timeout asynchronous reset
        now = NOW_32();
        if(workDone)
        {
            workDone = 0;
            s->lastActivity = now;
        }
        if(now - s->lastActivity > s->timeout)                                  // At any timeout, go to the sync initialization state and abort
        {
            s->bootCode = EZBL_ERROR_COM_READ_TIMEOUT;
            if((s->state > SM_GET_HEADER) && (s->state < SM_TEST_CHECKSUMS))
            {
                EZBL_FIFOWrite32(EZBL_COM_TX, ((((unsigned long)s->bootCode)<<16)) | 0x0000u); // Indicate termination and error code back to remote node
                EZBL_FIFOFlush(EZBL_COM_TX, NOW_ms);
                EZBL_FIFORead(0, EZBL_COM_RX, 0xFFFF);                          // Throw away any data sitting in the RX FIFO. If EZBL_COM_RX is null, this does nothing.
                EZBL_printf("\n--------TIMEOUT--------\n\n");
            }
            s->lastActivity = now;
            s->state = SM_SYNC_INIT;
            EZBL_COM_RX = 0;
        }
    } while(EZBL_COM_RX && (moreWork | cantReturn)); // Keep processing more tasks if we changed state to a non-linear flow down case

    if(cantReturn)
    {
        EZBL_FIFOFlush(EZBL_COM_TX, s->timeout);
#if defined(EZBL_DEBUG)
        EZBL_FIFOFlush(EZBL_STDOUT, NOW_sec);
#endif
        EZBL_ResetCPU();
    }

    // Restore starting state for SM_TEST_CHECKSUMS, SM_TERMINATE, SM_IGNORING states that may close the interface
    if(!EZBL_COM_RX)
        s->state = SM_SYNC_INIT;

    return (EZBL_COM_RX == 0);  // Return 1 if are signaling a bootload completion event (caller must check s->bootCode to see if we actually bootloaded something since failures will also return 1)
}
