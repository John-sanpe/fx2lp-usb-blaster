/**
 * Copyright (C) 2009 Ubixum, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 **/

#include <fx2regs.h>
#include <fx2macros.h>
#include <eputils.h>
#include <setupdat.h>

extern BOOL handle_vendorcommand(BYTE reqtyp, BYTE cmd);
extern BOOL handle_set_configuration(BYTE cfg);
extern BOOL handle_get_interface(BYTE ifc, BYTE* alt_ifc);
extern BOOL handle_set_interface(BYTE ifc,BYTE alt_ifc);
extern BYTE handle_get_configuration();
extern BOOL handle_set_configuration(BYTE cfg);
extern void handle_reset_ep(BYTE ep);

BOOL handle_get_status(void);
BOOL handle_clear_feature(void);
BOOL handle_set_feature(void);
void handle_get_descriptor(void);

void handle_setupdata(void) {
    BYTE alt_ifc;

    if ((SETUPDAT[0] & USB_REQ_TYP_MASK) == USB_REQ_TYP_STANDARD) {
        switch (SETUPDAT[1]) {
            case GET_STATUS:
                if (!handle_get_status())
                    STALLEP0();
                break;

            case CLEAR_FEATURE:
                if (!handle_clear_feature()) {
                    STALLEP0();
                }
                break;

            case SET_FEATURE:
                if (!handle_set_feature()) {
                    STALLEP0();
                }
                break;

            case GET_DESCRIPTOR:
                handle_get_descriptor();
                break;

            case GET_CONFIGURATION:
                EP0BUF[0] = handle_get_configuration();
                EP0BCH=0;
                EP0BCL=1;
                break;

            case SET_CONFIGURATION:
                if( !handle_set_configuration(SETUPDAT[2])) {
                    STALLEP0();
                }
                break;

            case GET_INTERFACE:
                if (!handle_get_interface(SETUPDAT[4],&alt_ifc))
                    STALLEP0();
                else {
                    EP0BUF[0] = alt_ifc;
                    EP0BCH=0;
                    EP0BCL=1;
                }
                break;

            case SET_INTERFACE:
                if ( !handle_set_interface(SETUPDAT[4],SETUPDAT[2])) {
                    STALLEP0();
                }
                break;

            default:
                if (!handle_vendorcommand(SETUPDAT[0], SETUPDAT[1]))
                    STALLEP0();
                break;
        }
    } else {
        if (!handle_vendorcommand(SETUPDAT[0], SETUPDAT[1]))
            STALLEP0();
    }

    EP0CS |= bmHSNAK;
}

__xdata BYTE* ep_addr(BYTE ep) { // bit 8 of ep_num is the direction
    BYTE ep_num = ep&~0x80; // mask the direction
    switch (ep_num) {
        case 0: return &EP0CS;
        case 1: return ep&0x80? &EP1INCS : &EP1OUTCS;
        case 2: return &EP2CS;
        case 4: return &EP4CS;
        case 6: return &EP6CS;
        case 8: return &EP8CS;
        default: return NULL;
    }
}

// Get status has three request types
#define GS_DEVICE 0x80
#define GS_INTERFACE 0x81
#define GS_ENDPOINT 0x82

volatile BOOL self_powered=FALSE;
volatile BOOL remote_wakeup_allowed=FALSE;

BOOL handle_get_status(void)
{
    switch (SETUPDAT[0]) {
        case GS_INTERFACE:
            EP0BUF[0] = 0;
            EP0BUF[1] = 0;
            EP0BCH=0;
            EP0BCL=2;
            break;

        case GS_DEVICE:
            EP0BUF[0] = (remote_wakeup_allowed << 1) | self_powered;
            EP0BUF[1] = 0;
            EP0BCH = 0;
            EP0BCL = 2;
            break;

        case GS_ENDPOINT: {
                __xdata BYTE* pep=ep_addr(SETUPDAT[4]);
                if (!pep)
                    return FALSE;
                EP0BUF[0] = *pep & bmEPSTALL ? 1 : 0;
                EP0BUF[1] = 0;
                EP0BCH=0;
                EP0BCL=2;
            }
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

#define GF_DEVICE 0
#define GF_ENDPOINT 2

BOOL handle_clear_feature(void)
{
    switch (SETUPDAT[0]) {
        case GF_DEVICE:
            if (SETUPDAT[2] == 1) {
                remote_wakeup_allowed=FALSE;
                break;
            }
            return FALSE;

        case GF_ENDPOINT:
            if (SETUPDAT[2] == 0) { // ep stall feature
                __xdata BYTE* pep=ep_addr(SETUPDAT[4]);
                *pep &= ~bmEPSTALL;
            } else
                return FALSE;
            break;

        default:
            return handle_vendorcommand(SETUPDAT[0], SETUPDAT[1]);
    }

    return TRUE;
}

BOOL handle_set_feature(void)
{
    switch (SETUPDAT[0]) {
        case GF_DEVICE:
            if (SETUPDAT[2] == 2)
                break;
            if (SETUPDAT[2] == 1) {
                remote_wakeup_allowed=TRUE;
                break;
            }
            return FALSE;

        case GF_ENDPOINT:
            if (SETUPDAT[2] != 0)
                return FALSE;
            else {
                __xdata BYTE* pep = ep_addr(SETUPDAT[4]);
                if (!pep)
                    return FALSE;
                *pep |= bmEPSTALL;
                RESETTOGGLE(SETUPDAT[4]);
            }
            break;

        default:
            return handle_vendorcommand(SETUPDAT[0], SETUPDAT[1]);
    }

    return TRUE;
}

extern __code WORD dev_dscr;
extern __code WORD dev_qual_dscr;
extern __code WORD highspd_dscr;
extern __code WORD fullspd_dscr;
extern __code WORD dev_strings;

WORD pDevConfig = (WORD)&fullspd_dscr;
WORD pOtherConfig = (WORD)&highspd_dscr;

void handle_hispeed(BOOL highspeed)
{
    __critical{
        if (highspeed) {
            pDevConfig=(WORD)&highspd_dscr;
            pOtherConfig=(WORD)&fullspd_dscr;
        } else {
            pDevConfig=(WORD)&fullspd_dscr;
            pOtherConfig=(WORD)&highspd_dscr;
        }
    }
}

void handle_get_descriptor(void)
{
    switch (SETUPDAT[3]) {
        case DSCR_DEVICE_TYPE:
            SUDPTRH = MSB((WORD)&dev_dscr);
            SUDPTRL = LSB((WORD)&dev_dscr);
            break;

        case DSCR_CONFIG_TYPE:
            SUDPTRH = MSB(pDevConfig);
            SUDPTRL = LSB(pDevConfig);
            break;

        case DSCR_STRING_TYPE: {
                STRING_DSCR* pStr = (STRING_DSCR*)&dev_strings;
                // pStr points to string 0
                BYTE idx = SETUPDAT[2];
                BYTE cur=0; // current check
                do {
                    if (idx==cur++) break;
                    pStr = (STRING_DSCR*)((BYTE*)pStr + pStr->dsc_len);
                    if (pStr->dsc_type != DSCR_STRING_TYPE) pStr=NULL;
                } while ( pStr && cur<=idx);

                if (pStr) {
                    SUDPTRH = MSB((WORD)pStr);
                    SUDPTRL = LSB((WORD)pStr);
                } else {STALLEP0();}
            }
            break;

        case DSCR_DEVQUAL_TYPE:
            SUDPTRH = MSB((WORD)&dev_qual_dscr);
            SUDPTRL = LSB((WORD)&dev_qual_dscr);
            break;

        case DSCR_OTHERSPD_TYPE:
            SUDPTRH = MSB(pOtherConfig);
            SUDPTRL = LSB(pOtherConfig);
            break;

        default:
            STALLEP0();
            break;
    }
}

