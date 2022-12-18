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
static unsigned int led_frequency;

static SECTION_XDATA uint8_t receive_buffer[64];
static SECTION_XDATA uint8_t transmit_buffer[128];
static SECTION_XDATA uint8_t receive_length;

DEFINE_HANDLE(usbreset_isr, USBRESET_ISR)
{
    handle_hispeed(false);
    CLEAR_USBRESET();
}

DEFINE_HANDLE(hispeed_isr, HISPEED_ISR)
{
    handle_hispeed(true);
    CLEAR_HISPEED();
}

DEFINE_HANDLE(sudav_isr, SUDAV_ISR)
{
    sudav_handled = true;
    CLEAR_SUDAV();
}

DEFINE_HANDLE(ibn_isr, IBN_ISR)
{
    uint8_t value;

    value = IBNIE;
    IBNIE = 0;

    CLEAR_USBINT();
    led_enable();

    IBNIRQ = 0xff;
    NAKIRQ = bmIBN;
    SYNCDELAY();

    IBNIE = value;
    SYNCDELAY();
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

bool handle_vendorcommand(uint8_t reqtyp, uint8_t cmd)
{
    uint16_t length, addr, value;
    bool retval = true;

    if ((reqtyp & USB_REQ_TYP_MASK) != USB_REQ_TYP_VENDOR)
        return false;

    value = MAKEWORD(EP0BUF[3], EP0BUF[2]);
    addr = MAKEWORD(EP0BUF[5], EP0BUF[4]);
    length = MAKEWORD(EP0BUF[7], EP0BUF[6]);

    if (!length) {
        switch (cmd) {
            case FTDI_REQ_RESET:
                /* Nothing */
                break;

            case FTDI_REQ_SET_BAUDRATE:
                /* Nothing */
                break;

            case FTDI_REQ_SET_DATA_CHAR:
                /* Nothing */
                break;

            case FTDI_REQ_SET_FLOW_CTRL:
                /* Nothing */
                break;

            case FTDI_REQ_SET_MODEM_CTRL:
                /* Nothing */
                break;

            default:
                /* Nothing */
                break;
        }
    } else {
        switch (cmd) {
            case FTDI_REQ_RD_EEPROM:
                addr <<= 1;
                EP0BUF[0] = ftdi_eeprom[addr];
                EP0BUF[1] = ftdi_eeprom[addr + 1];
                length = 2;
                break;

            case FTDI_REQ_GET_MODEM_STA:
                EP0BUF[0] = FTDI_MODEM_DUMMY0;
                EP0BUF[1] = FTDI_MODEM_DUMMY1;
                length = FTDI_MODEM_LENGTH;
                break;

            default:
                EP0BUF[0] = 0x00;
                EP0BUF[1] = 0x00;
                length = 2;
                break;
        }
    }

    EP0BCH = MSB(length);
    SYNCDELAY();
    EP0BCL = LSB(length);
    SYNCDELAY();

    return true;
}

static void fx2lp_hwinit(void)
{
    REVCTL = bmNOAUTOARM | bmSKIPCOMMIT;
    RENUMERATE_UNCOND();

    SETCPUFREQ(CLK_48M);
    USE_USB_INTS();

    ENABLE_SUDAV();
    NAKIE |= bmEP1IBN;
    ENABLE_HISPEED();
    ENABLE_USBRESET();

    EA = 1;
}

static void endpoints_setup(void)
{
    /* Setup ep1 input bulk mode */
    EP1INCFG = bmVALID | bmDIR | bmTYPE1;
    SYNCDELAY();

    /* Setup ep2 output bulk mode */
    EP2CFG = bmVALID | bmTYPE1;
    SYNCDELAY();

    /* Disable all other eps */
    EP4CFG &= ~bmVALID;
    SYNCDELAY();
    EP6CFG &= ~bmVALID;
    SYNCDELAY();
    EP8CFG &= ~bmVALID;
    SYNCDELAY();

    /* Reset the fifos. */
    RESETFIFO(1);
    RESETFIFO(2);
}

void main(void)
{
    unsigned int buff_index;
    unsigned int bit_shift;
    uint8_t value;

    fx2lp_hwinit();
    endpoints_setup();

    led_init();
    led_disable();

    /* Poll loop */
    for (;;) {
        if (sudav_handled) {
            handle_setupdata();
            sudav_handled = false;
            continue;
        }







    }
}
