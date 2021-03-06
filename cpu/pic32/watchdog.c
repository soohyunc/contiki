/*
 * Contiki PIC32 Port project
 *
 * Copyright (c) 2012,
 *  Scuola Superiore Sant'Anna (http://www.sssup.it) and
 *  Consorzio Nazionale Interuniversitario per le Telecomunicazioni
 *  (http://www.cnit.it).
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \addtogroup pic32 PIC32 Contiki Port
 *
 * @{
 */

/**
 * \file   cpu/pic32/watchdog.c
 * \brief  PIC32MX Watchdog routines
 * \author Giovanni Pellerano <giovanni.pellerano@evilaliv3.org>
 * \date   2012-03-23
 */

#include "dev/watchdog.h"

#include <pic32_irq.h>

#include <p32xxxx.h>

/*---------------------------------------------------------------------------*/
void
watchdog_init(void)
{
  WDTCON = 0;
  OSCCONSET = 0x10; /* Set power mode saving to Idle */
}
/*---------------------------------------------------------------------------*/
void
watchdog_start(void)
{
  WDTCONSET = _WDTCON_ON_MASK;
}
/*---------------------------------------------------------------------------*/
void
watchdog_periodic(void)
{
  WDTCONSET = _WDTCON_WDTCLR_MASK;
}
/*---------------------------------------------------------------------------*/
void
watchdog_stop(void)
{
  WDTCONCLR = _WDTCON_ON_MASK;
}
/*---------------------------------------------------------------------------*/
void
watchdog_reboot(void)
{
  volatile int *p = (int *)&RSWRST;

  /* Unlock sequence */
  ASM_DIS_INT;
  if(!(DMACONbits.SUSPEND)){
    DMACONSET=_DMACON_SUSPEND_MASK; // suspend
    while((DMACONbits.DMABUSY)) {
      ; // wait to be actually suspended
    }
  }

  SYSKEY = 0;
  SYSKEY = 0xaa996655;
  SYSKEY = 0x556699aa;

  RSWRSTSET=_RSWRST_SWRST_MASK;
  *p;

  while(1) {
    ;
  }
}
/*---------------------------------------------------------------------------*/

/** @} */
