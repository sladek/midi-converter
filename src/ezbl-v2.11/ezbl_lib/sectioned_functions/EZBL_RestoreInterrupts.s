;/*******************************************************************************
;  Easy Bootloader for PIC24 and dsPIC33 Core Source File
;
;  Summary:
;    Assembly language optimized helper routines required by the EZBL core
;
;  Description:
;    Assembly language optimized helper routines required by the EZBL core
;    and optionally also callable from EZBL optional submodules.
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2016 Microchip Technology Inc.
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

    .equ    SR_C, 0x0   ; SR<C> is bit 0
    .equ    SR_Z, 0x1   ; SR<Z> is bit 1
    .equ    SR_OV, 0x2  ; SR<OV> is bit 2
    .equ    SR_N, 0x3   ; SR<N> is bit 3

    
/**
 * void EZBL_RestoreInterrupts(unsigned int previousSRSave);
 *
 * Restores the prior CPU IPL setting returned from EZBL_DisableInterrupts(), 
 * enabling any interrupts that were previously enabled. CORCON<IPL3> is 
 * cleared.
 *   
 * This function executes in 12 cycles (dsPIC33E/PIC24E/dsPIC33C devices) or
 * 7 cycles (dsPIC30F/dsPIC33F/PIC24H/PIC24F devices), including the call and 
 * return.
 *
 * @param previousSRSave Original SR<IPL2:0> encoded value returned from 
 *                       EZBL_DisableInterrupts();
 *
 * @return CORCON<IPL3> and SR<IPL> bits revert back to their original states.
 *    
 *         This function only uses w0. When calling from assembly, it is not 
 *         necessary to preserve the w1-w7 registers. w0 is destroyed though, so 
 *         don't call EZBL_RestoreInterrupts() a second time expecting 
 *         idempotent results unless you externally reprovide the same 
 *         previousSRSave data.
 */
    .pushsection    .text.EZBL_RestoreInterrupts, code
    .global         _EZBL_RestoreInterrupts
    .type           _EZBL_RestoreInterrupts, @function
_EZBL_RestoreInterrupts:
    sl              w0, #8, w0      ; Move SR<IPL> bits to the high byte of the high word of our return address
    ior             w0, [--w15], [w15++]
    retfie                          ; Again fake an interrupt return to restore the original IPL
    .size           _EZBL_RestoreInterrupts, . - _EZBL_RestoreInterrupts
    .popsection
    