/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <usb-blaster.h>

SECTION_CODE uint8_t ftdi_eeprom[128] = {
	0x00, 0x00, 0xfb, 0x09,
    0x01, 0x60, 0x00, 0x04,
	0x80, 0xe1, 0x1c, 0x00,
    0x00, 0x02, 0x94, 0x0e,
	0xa2, 0x18, 0xba, 0x12,
    0x0e, 0x03, 0x41, 0x00,
	0x6c, 0x00, 0x74, 0x00,
    0x65, 0x00, 0x72, 0x00,
	0x61, 0x00, 0x18, 0x03,
    0x55, 0x00, 0x53, 0x00,
	0x42, 0x00, 0x2d, 0x00,
    0x42, 0x00, 0x6c, 0x00,
	0x61, 0x00, 0x73, 0x00,
    0x74, 0x00, 0x65, 0x00,
	0x72, 0x00, 0x12, 0x03,
    0x43, 0x00, 0x30, 0x00,
	0x42, 0x00, 0x46, 0x00,
    0x41, 0x00, 0x36, 0x00,
	0x44, 0x00, 0x37, 0x00,
    0x02, 0x03, 0x01, 0x00,
	0x52, 0x45, 0x56, 0x42,
    0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xb5, 0xb2
};
