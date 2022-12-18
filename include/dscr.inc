;; SPDX-License-Identifier: GPL-2.0-or-later
;;
;; Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
;;

.include "common.inc"
.module DEV_DSCR

; Descriptor types
DSCR_DEVICE_TYPE        = 1
DSCR_CONFIG_TYPE        = 2
DSCR_STRING_TYPE        = 3
DSCR_INTERFACE_TYPE     = 4
DSCR_ENDPOINT_TYPE      = 5
DSCR_DEVQUAL_TYPE       = 6

; Descriptor lengths
DSCR_INTERFACE_LEN      = 9
DSCR_ENDPOINT_LEN       = 7

; Endpoint types
ENDPOINT_TYPE_CONTROL   = 0
ENDPOINT_TYPE_ISO       = 1
ENDPOINT_TYPE_BULK      = 2
ENDPOINT_TYPE_INT       = 3

.globl _dev_dscr, _dev_qual_dscr, _highspd_dscr, _fullspd_dscr, _dev_strings, _dev_strings_end
.area DSCR_AREA (CODE)

; -----------------------------------------------------------------------------
; Device descriptor
; -----------------------------------------------------------------------------

_dev_dscr:
    .db     dev_dscr_end - _dev_dscr
    .db     DSCR_DEVICE_TYPE
    .dw     0x0002              ; USB 2.0
    .db     0x00                ; Class (vendor specific)
    .db     0x00                ; Subclass (vendor specific)
    .db     0x00                ; Protocol (vendor specific)
    .db     0x08                ; Max. EP0 packet size
    .dw     VID                 ; Manufacturer ID
    .dw     PID                 ; Product ID
    .dw     0x0100              ; Product version (0.01)
    .db     1                   ; Manufacturer string index
    .db     2                   ; Product string index
    .db     3                   ; Serial number string index (none)
    .db     1                   ; Number of configurations

dev_dscr_end:

; -----------------------------------------------------------------------------
; Device qualifier (for "other device speed")
; -----------------------------------------------------------------------------

_dev_qual_dscr:
    .db     dev_qualdscr_end - _dev_qual_dscr
    .db     DSCR_DEVQUAL_TYPE
    .dw     0x0002              ; USB 2.0
    .db     0xff                ; Class (vendor specific)
    .db     0xff                ; Subclass (vendor specific)
    .db     0xff                ; Protocol (vendor specific)
    .db     64                  ; Max. EP0 packet size
    .db     1                   ; Number of configurations
    .db     0                   ; Extra reserved byte

dev_qualdscr_end:

; -----------------------------------------------------------------------------
; High-Speed configuration descriptor
; -----------------------------------------------------------------------------

_highspd_dscr:
    .db     highspd_dscr_end - _highspd_dscr
    .db     DSCR_CONFIG_TYPE

    ; Total length of the configuration (1st line LSB, 2nd line MSB)
    .db     (highspd_dscr_realend - _highspd_dscr) % 256
    .db     (highspd_dscr_realend - _highspd_dscr) / 256
    .db     1                   ; Number of interfaces
    .db     1                   ; Configuration number
    .db     0                   ; Configuration string (none)
    .db     0x80                ; Attributes (bus powered, no wakeup)
    .db     0x32                ; Max. power (100mA)

highspd_dscr_end:

    ; Interfaces (only one in our case)
    .db     DSCR_INTERFACE_LEN
    .db     DSCR_INTERFACE_TYPE
    .db     0                   ; Interface index
    .db     0                   ; Alternate setting index
    .db     2                   ; Number of endpoints
    .db     0xff                ; Class (vendor specific)
    .db     0xff                ; Subclass (vendor specific)
    .db     0xff                ; Protocol (vendor specific)
    .db     0                   ; String index (none)

    ; Endpoint 1 (IN)
    .db     DSCR_ENDPOINT_LEN
    .db     DSCR_ENDPOINT_TYPE
    .db     0x81                ; EP number (1), direction (IN)
    .db     ENDPOINT_TYPE_BULK  ; Endpoint type (bulk)
    .db     0x40                ; Max. packet size, LSB (64 bytes)
    .db     0x00                ; Max. packet size, MSB (64 bytes)
    .db     0x01                ; Polling interval (ignored for bulk)

    ; Endpoint 2 (OUT)
    .db     DSCR_ENDPOINT_LEN
    .db     DSCR_ENDPOINT_TYPE
    .db     0x02                ; EP number (2), direction (OUT)
    .db     ENDPOINT_TYPE_BULK  ; Endpoint type (bulk)
    .db     0x40                ; Max. packet size, LSB (64 bytes)
    .db     0x00                ; Max. packet size, MSB (64 bytes)
    .db     0x01                ; Polling interval (ignored for bulk)

highspd_dscr_realend:
    .even

; -----------------------------------------------------------------------------
; Full-Speed configuration descriptor
; -----------------------------------------------------------------------------

_fullspd_dscr:
    .db     fullspd_dscr_end - _fullspd_dscr
    .db     DSCR_CONFIG_TYPE

    ; Total length of the configuration (1st line LSB, 2nd line MSB)
    .db     (fullspd_dscr_realend - _fullspd_dscr) % 256
    .db     (fullspd_dscr_realend - _fullspd_dscr) / 256
    .db     1                   ; Number of interfaces
    .db     1                   ; Configuration number
    .db     0                   ; Configuration string (none)
    .db     0x80                ; Attributes (bus powered, no wakeup)
    .db     0x32                ; Max. power (100mA)

fullspd_dscr_end:

    ; Interfaces (only one in our case)
    .db     DSCR_INTERFACE_LEN
    .db     DSCR_INTERFACE_TYPE
    .db     0                   ; Interface index
    .db     0                   ; Alternate setting index
    .db     1                   ; Number of endpoints
    .db     0xff                ; Class (vendor specific)
    .db     0xff                ; Subclass (vendor specific)
    .db     0xff                ; Protocol (vendor specific)
    .db     0                   ; String index (none)

    ; Endpoint 2 (IN)
    .db     DSCR_ENDPOINT_LEN
    .db     DSCR_ENDPOINT_TYPE
    .db     0x82                ; EP number (2), direction (IN)
    .db     ENDPOINT_TYPE_BULK  ; Endpoint type (bulk)
    .db     0x40                ; Max. packet size, LSB (64 bytes)
    .db     0x00                ; Max. packet size, MSB (64 bytes)
    .db     0x00                ; Polling interval (ignored for bulk)

fullspd_dscr_realend:
    .even

; -----------------------------------------------------------------------------
; Strings
; -----------------------------------------------------------------------------

_dev_strings:

; See http://www.usb.org/developers/docs/USB_LANGIDs.pdf for the full list.
string_descriptor_lang 0,0x0409 ; Language code 0x0409 (English, US)

string_descriptor_a 1,^"Altera"
string_descriptor_a 2,^"USB-Blaster"
