/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _USB_BLASTER_H_
#define _USB_BLASTER_H_

#include <sdcc-mcs51.h>
#include <fx2macros.h>
#include <fx2ints.h>
#include <autovector.h>

#define SYNCDELAY SYNCDELAY4

#define TMS_BIT bmBIT0
#define TCK_BIT bmBIT1
#define TDI_BIT bmBIT2
#define TDO_BIT bmBIT3
#define LED_BIT bmBIT1

#define TMS_PIN PB0
#define TCK_PIN PB1
#define TDI_PIN PB2
#define TDO_PIN PB3
#define LED_PIN PA1

#define LED_POLARITY    0 /* 1: active-high, 0: active-low */
#define led_init()      do { PORTACFG = 0; OEA = LED_BIT; } while (0)
#define led_enable()    do { LED_PIN = LED_POLARITY; } while (0)
#define led_disable()   do { LED_PIN = !LED_POLARITY; } while (0)
#define led_toggle()    do { LED_PIN = !LED_PIN; } while (0)

#endif /* _USB_BLASTER_H_ */
