# SPDX-License-Identifier: GPL-2.0-or-later
asflags := -Iinclude/ -Ifx2lib/include/ -glos
ccflags := -Iinclude/ -Ifx2lib/include/ -c
ldflags := --code-size 0x1c00 --xram-size 0x0200 --xram-loc 0x1c00 -Wl"-b DSCR_AREA=0x1e00" -Wl"-b INT2JT=0x1f00"
heads := $(shell find -name "*.h" -or -name "*.inc")
srcs := $(patsubst ./%,%,$(shell find -name "*.c" -or -name "*.a51"))
objs := $(addprefix build/,$(addsuffix .rel,$(basename $(srcs))))

build/%.rel:%.a51 $(heads)
	@ mkdir -p $(dir $@)
	@ echo -e "\t\e[32mCC\e[0m\t" $@
	@ sdas8051 $(asflags) -o $@ $<

build/%.rel:%.c $(heads)
	@ mkdir -p $(dir $@)
	@ echo -e "\t\e[32mCC\e[0m\t" $@
	@ sdcc $(ccflags) -o $@ $<

build/firmware.ihx: $(objs)
	@ echo -e "\t\e[34mMKIHX\e[0m\t" $@
	@ sdcc $(ldflags) -o $@ $^

clean: FORCE
	@ echo -e "\t\e[31mCLEAN\e[0m build/\t"
	@ rm -rf build/

FORCE:
.PHONY: FORCE
