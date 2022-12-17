;; SPDX-License-Identifier: GPL-2.0-or-later
;;
;; Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
;;

.macro string_descriptor_a n,str
_string'n:
	.nchr	len,"'str"
	.db	len * 2 + 2
	.db	3
	.irpc	i,^"'str"
		.db	''i, 0
	.endm
.endm

.macro string_descriptor_lang n,l
_string'n:
	.db	4
	.db	3
	.dw	>l + (<l * 0x100)
.endm
