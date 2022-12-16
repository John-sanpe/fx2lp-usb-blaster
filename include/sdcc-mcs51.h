/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _SDCC_MCS51_H_
#define _SDCC_MCS51_H_

typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int32_t;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef _Bool bool;

enum {
    false = 0,
    true = 1,
};

#define SECTION_CODE __code
#define SECTION_SFR __sfr
#define SECTION_SBIT __sbit
#define SECTION_IDATA __idata
#define SECTION_XDATA __xdata

#define REG_SFR(name, base) SECTION_SFR __at(base) name;
#define REG_BIT(name, base, shift) SECTION_SBIT __at(base + shift) name;

#define DECLARE_HANDLE(vector) extern void vector##_entry(void) __interrupt
#define DEFINE_HANDLE(vector) void vector##_entry(void) __interrupt vector

#endif /* _SDCC_MCS51_H_ */
