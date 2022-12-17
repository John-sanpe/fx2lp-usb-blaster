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

    IBNIRQ = 0xff;
    NAKIRQ = bmIBN;
    SYNCDELAY();

    IBNIE = value;
    SYNCDELAY();
}

DEFINE_HANDLE(tf2_isr, TF2_ISR)
{
    static unsigned int count;

    if (!led_frequency)
        led_enable();
    else if (count++ > led_frequency) {
        led_toggle();
        count = 0;
    }

	TF2 = 0;
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
    (void)cmd;
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

    RCAP2L = -500 & 0xff;
    RCAP2H = (-500 & 0xff00) >> 8;
    T2CON = 0;
    TR2 = 1;
    ENABLE_TIMER2();

    EA = 1;
}

static void endpoints_setup(void)
{
    /* Setup ep1 input bulk mode */
    EP1INCFG = bmVALID | bmDIR | bmTYPE1 | bmSIZE;
    SYNCDELAY();

    /* Setup ep2 output bulk mode */
    EP2CFG = bmVALID | bmTYPE1 | bmSIZE;
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

    }
}
