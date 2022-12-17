;; SPDX-License-Identifier: GPL-2.0-or-later
;;
;; Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
;;

VID = 0xfb09 ; Manufacturer ID (0x04b4)
PID = 0x0160 ; Product ID (0x8613)

.include "dscr.inc"
string_descriptor_a 3,^"C0BFA6D7"
_dev_strings_end:
	.dw	0x0000
