;; SPDX-License-Identifier: GPL-2.0-or-later
;;
;; Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
;;

VID = 0xa908 ; Manufacturer ID (0x08a9)
PID = 0x1500 ; Product ID (0x0015)

.include "dscr.inc"
string_descriptor_a 3,^"CWAV USBee DX"
_dev_strings_end:
	.dw	0x0000
