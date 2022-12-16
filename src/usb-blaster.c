/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <usb-blaster.h>
#include <command.h>
#include <delay.h>
#include <eputils.h>
#include <setupdat.h>

static bool sudav_handled;
static uint8_t ftdi_command;

DEFINE_HANDLE(USBRESET_ISR)
{
    handle_hispeed(false);
    CLEAR_USBRESET();
}

DEFINE_HANDLE(HISPEED_ISR)
{
    handle_hispeed(true);
    CLEAR_HISPEED();
}

DEFINE_HANDLE(SUDAV_ISR)
{
    sudav_handled = true;
    CLEAR_SUDAV();
}

DEFINE_HANDLE(IBN_ISR)
{
    uint8_t value;

    value = IBNIE;
	IBNIE = 0;

    CLEAR_USBINT();

	IBNIRQ = 0xff;
	NAKIRQ = bmIBN;
	SYNCDELAY;

	IBNIE = value;
	SYNCDELAY;
}

bool handle_get_descriptor(void)
{
    return true;
}

bool handle_get_interface(uint8_t ifc, uint8_t *alt_ifc)
{
    if (ifc != 0)
        return false;

    *alt_ifc = 0;
    return true;
}

bool handle_set_interface(uint8_t ifc, uint8_t alt_ifc)
{
    if (ifc || alt_ifc)
        return false;

    RESETTOGGLE(0x82);
    RESETFIFO(0x02);

    return true;
}

uint8_t handle_get_configuration(void)
{
    return 1;
}

bool handle_set_configuration(uint8_t cfg)
{
    return cfg == 1;
}

bool handle_vendorcommand(uint8_t cmd)
{
    return false;
}

static void fx2lp_hwinit(void)
{
    REVCTL = bmNOAUTOARM | bmSKIPCOMMIT;
    RENUMERATE_UNCOND();

    SETCPUFREQ(CLK_48M);
    USE_USB_INTS();

    ENABLE_SUDAV();
    ENABLE_HISPEED();
    ENABLE_USBRESET();

    EA = 1;
}

static void endpoints_setup(void)
{
    /* Setup ep1 input bulk mode */
    EP1INCFG = bmVALID | bmDIR | bmTYPE1 | bmSIZE;
    SYNCDELAY;

    /* Setup ep2 output bulk mode */
    EP2CFG = bmVALID | bmTYPE1 | bmSIZE;
    SYNCDELAY;

    /* Disable all other eps */
    EP4CFG &= ~bmVALID;
    SYNCDELAY;
    EP6CFG &= ~bmVALID;
    SYNCDELAY;
    EP8CFG &= ~bmVALID;
    SYNCDELAY;

    /* Reset the fifos. */
    RESETFIFO(1);
    RESETFIFO(2);
}

void main(void)
{
    fx2lp_hwinit();
    endpoints_setup();

    led_init();
    led_enable();

    /* Poll loop */
    for (;;) {
        if (sudav_handled) {
            handle_setupdata();
            sudav_handled = false;
        }

        switch (ftdi_command) {
            case FTDI_REQ_RESET:
                break;

            case FTDI_REQ_SET_BAUDRATE:
                break;

            case FTDI_REQ_SET_DATA_CHAR:
                break;

            case FTDI_REQ_SET_FLOW_CTRL:
                break;

            case FTDI_REQ_SET_MODEM_CTRL:
                break;

            case FTDI_REQ_GET_MODEM_STA:
                break;

            case FTDI_REQ_SET_EVENT_CHAR:
                break;

            case FTDI_REQ_SET_ERR_CHAR:
                break;

            case FTDI_REQ_SET_LAT_TIMER:
                break;

            case FTDI_REQ_GET_LAT_TIMER:
                break;

            case FTDI_REQ_SET_BITMODE:
                break;

            case FTDI_REQ_RD_PINS:
                break;

            case FTDI_REQ_RD_EEPROM:
                break;

            case FTDI_REQ_WR_EEPROM:
                break;

            case FTDI_REQ_ES_EEPROM:
                break;

            default:
                break;
        }
    }
}
