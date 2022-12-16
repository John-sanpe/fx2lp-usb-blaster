/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

enum fidi_request {
    FTDI_REQ_RESET              = 0x00,
    FTDI_REQ_SET_BAUDRATE       = 0x01,
    FTDI_REQ_SET_DATA_CHAR      = 0x02,
    FTDI_REQ_SET_FLOW_CTRL      = 0x03,
    FTDI_REQ_SET_MODEM_CTRL     = 0x04,
    FTDI_REQ_GET_MODEM_STA      = 0x05,
    FTDI_REQ_SET_EVENT_CHAR     = 0x06,
    FTDI_REQ_SET_ERR_CHAR       = 0x07,
    FTDI_REQ_SET_LAT_TIMER      = 0x09,
    FTDI_REQ_GET_LAT_TIMER      = 0x0A,
    FTDI_REQ_SET_BITMODE        = 0x0B,
    FTDI_REQ_RD_PINS            = 0x0C,
    FTDI_REQ_RD_EEPROM          = 0x90,
    FTDI_REQ_WR_EEPROM          = 0x91,
    FTDI_REQ_ES_EEPROM          = 0x92,
};

#define FTDI_MODEM_LENGTH 2
#define FTDI_MODEM_DUMMY0 0x01
#define FTDI_MODEM_DUMMY1 0x60

#endif /* _COMMAND_H_ */
