# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
#

asflags := -Iinclude/ -Ifx2lib/include/ -glos
ccflags := -Iinclude/ -Ifx2lib/include/ -mmcs51 -c
ldflags := -mmcs51 --code-size 0x1c00 --xram-size 0x0200 --xram-loc 0x1c00
ldflags += -Wl"-b DSCR_AREA=0x1e00" -Wl"-b INT2JT=0x1f00"
asheads := $(shell find -name "*.inc")
ccheads := $(shell find -name "*.h")
srcs := $(patsubst ./%,%,$(shell find -name "*.c" -or -name "*.a51"))
objs := $(addprefix build/,$(addsuffix .rel,$(basename $(srcs))))

ifeq ("$(origin V)", "command line")
BUILD_VERBOSE = $V
else
BUILD_VERBOSE =
endif

ifeq ($(BUILD_VERBOSE),1)
Q =
else
MAKEFLAGS += -s
Q = @
endif

all: image

build/%.rel:%.a51 $(asheads)
	$Q mkdir -p $(dir $@)
	$Q echo -e "\t\e[32mSDAS\e[0m\t" $@
	$Q sdas8051 $(asflags) -o $@ $<

build/%.rel:%.c $(ccheads)
	$Q mkdir -p $(dir $@)
	$Q echo -e "\t\e[32mSDCC\e[0m\t" $@
	$Q sdcc $(ccflags) -o $@ $<

build/firmware.ihx: $(objs)
	$Q echo -e "\t\e[35mLINK\e[0m\t" $@
	$Q sdcc $(ldflags) -o $@ $^
build: build/firmware.ihx FORCE
	@:

build/firmware.bin: build/firmware.ihx
	$Q echo -e "\t\e[34mOBJCOPY\e[0m\t" $@
	$Q objcopy -Iihex $< -Obinary $@
image: build/firmware.bin FORCE
	@:

clean: FORCE
	$Q echo -e "\t\e[31mCLEAN\e[0m\tbuild/"
	$Q rm -rf build/

FORCE:
.PHONY: FORCE
