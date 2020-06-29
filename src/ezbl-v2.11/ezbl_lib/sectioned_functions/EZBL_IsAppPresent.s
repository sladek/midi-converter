;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Library Source File
;
;  Summary:
;    Identifies if a valid Application is present in the flash by reading a
;    marker written only after full verification passes durring last
;    programming.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2017 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
;// DOM-IGNORE-END

	.equ	SR_C, 0x0	; SR<C> is bit 0
	.equ	SR_Z, 0x1	; SR<Z> is bit 1
	.equ	SR_OV, 0x2	; SR<OV> is bit 2
	.equ	SR_N, 0x3	; SR<N> is bit 3


/**
 * Checks if a valid Application has been flagged as existent in flash.
 *
 * This function reads the EZBL_appBootloadState flash address and returns the
 * (EZBL_INSTALLED_FLAGS.appInstalled == EZBL_APP_INSTALLED) evaluation. It does
 * not do comprehensive/pessimistic CRC/hash testing of the Application space.
 * I.e. use this function to determine if the last bootloading attempt completed
 * successfully with a non-corrupt Application image that was read-back
 * verified, not to test if the Application has self-destructed or been tampered
 * with.
 *
 * When EZBL_appBootloadState does not exist in the project (such as when 
 * building a Dual Parition project), this function instead checks the FBTSEQ
 * validity on the inactive partition. An App is said to be present if
 * FBTSEQ<11:0> == ~FBTSEQ<23:12>.
 *
 * @return 1 if an Application is fully installed and ready to be executed.
 *         0 if an Application is absent or corrupt.
 */
; int EZBL_IsAppPresent(void);
	.pushsection    .text.EZBL_IsAppPresent, code
	.global         _EZBL_IsAppPresent
    .type           _EZBL_IsAppPresent, @function
    .weak           _EZBL_appBootloadState
    .weak           __FBTSEQ
    .extern         _EZBL_ReadFlash
    .extern         SR
_EZBL_IsAppPresent:
    mov         #packed_lo(_EZBL_appBootloadState+0xA), w0
    mov         #packed_hi(_EZBL_appBootloadState+0xA), w1
    sub         w0, #0xA, w2
    subb        w1, #0, w3
    bra         Z, dualPartitionCriteria

    call       _EZBL_ReadFlash              ; w1:w0 = unsigned long EZBL_ReadFlash(&EZBL_appBootloadState); // Reads 24-bits of flash at given program space address
    lsr         w0, #8, w0
    sl          w1, #8, w1
    ior         w0, w1, w0
    mov         #0x12CD, w2
1:  sub         w2, w0, w0                  ; ((EZBL_INSTALLED_FLAGS*)&EZBL_appBootloadState)->appInstalled - EZBL_APP_INSTALLED == 0? (EZBL_APP_INSTALLED == 0x12CD)
    btss        SR, #SR_Z
    retlw       #0, w0                      ; Non-zero, no flag isn't 0x12CD, app is absent or corrupt
    retlw       #1, w0                      ; Zero, yes flag matches 0x12CD, app is present

dualPartitionCriteria:                      ; Checks if Inactive Partition FBTSEQ Config word contains a valid 12-bit + 12-bit complemented number
    mov         #packed_lo(__FBTSEQ), w0
    mov         #packed_hi(__FBTSEQ), w1 
    bset        w1, #6                      ; Make 0x400000 address for Inactive Partition
    call       _EZBL_ReadFlash              ; w1:w0 = unsigned long EZBL_ReadFlash(&EZBL_appBootloadState); // Reads 24-bits of flash at given program space address
    lsr         w0, #12, w2
    sl          w1, #4, w3
    ior         w2, w3, w2
    com         w0, w0
    mov         #0x0FFF, w4
    and         w0, w4, w0
    bra         1b
    .size   _EZBL_IsAppPresent, . - _EZBL_IsAppPresent
    .popsection
