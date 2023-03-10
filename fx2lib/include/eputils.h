// Copyright (C) 2009 Ubixum, Inc.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <http://www.gnu.org/licenses/>.

/*! \file
 *  Functions and macros for working with endpoints.
 * */

#ifndef EPUTILS_H
#define EPUTILS_H

#include "fx2types.h"

/**
 * NOTE you can't use these unless you define SYNCDELAY
 * as a macro or function.  The reason is that SYNCDELAY
 * needs to be longer or shorter depending on your IFCONFIG
 * settings.
 * See delay.h
 *
 * Example:
 * \code
 *  #define SYNCDELAY() SYNCDELAY4 // SYNCDELAY4 from delay.h
 * \endcode
 *
 *
 **/

/**
 * Stalls EP0.
 **/
#define STALLEP0() EP0CS |= bmEPSTALL

/**
 * \brief Reset the toggle on an endpoint.
 * To use this, the endpoint needs bit 8 to be IN=1,OUT=0
 **/
#define RESETTOGGLE(ep) TOGCTL = (ep&0x0F) + ((ep&0x80)>>3); TOGCTL |= bmRESETTOGGLE


/**
 * RESETFIFO should not use 0x80|epnum for IN endpoints
 * Only use 0x02, 0x04, 0x06, 0x06 for ep value
 **/
#define RESETFIFO(ep) {FIFORESET=0x80; SYNCDELAY();\
                       FIFORESET=ep; SYNCDELAY();\
                       FIFORESET=0x00; SYNCDELAY();}
/**
 * Quickly reset all endpoint FIFOS.
 **/
#define RESETFIFOS() {FIFORESET=0x80; SYNCDELAY();\
                     FIFORESET=0x02; SYNCDELAY();\
                     FIFORESET=0x04; SYNCDELAY();\
                     FIFORESET=0x06; SYNCDELAY();\
                     FIFORESET=0x08; SYNCDELAY();\
                     FIFORESET=0x00; SYNCDELAY();}

#endif
