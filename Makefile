KERNEL := bin/kernel.elf
ISOFILE := arc.iso

LIMINECFG := limine.cfg
LIMINE_FILES := limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin
LIMINE_INSTALL := limine/limine-install

ISOROOT := isoroot

XORRISO_FLAGS := \
	-as mkisofs -b limine-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot limine-eltorito-efi.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	$(ISOROOT)

CC := cc
LD := ld

CFLAGS := -Wall -Wextra -O2 -pipe
INTERNALCFLAGS := \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pic \
	-mno-80387 \
	-mno-mmx \
	-mno-3dnow \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-MMD

LDFLAGS := 
INTERNALLDFLAGS := \
	-Tkernel/linker.ld \
	-nostdlib \
	-zmax-page-size=0x1000 \
	-static

CFILES := $(shell find kernel/ -name "*.c")
OBJFILES := $(addprefix /tmp/arc_inter_files/,$(CFILES:.c=.o))
HEADER_DEPS := $(OBJFILES:.o=.d)

MAKE_DIR = @mkdir -p $(@D)

# Build the iso file:
$(ISOFILE): $(KERNEL) $(LIMINE_INSTALL) $(LIMINECFG)
	@mkdir -p isoroot
	@cp -v $(KERNEL) $(LIMINECFG) $(LIMINE_FILES) $(ISOROOT)
	xorriso $(XORRISO_FLAGS) -o $(ISOFILE)

$(LIMINE_INSTALL): $(LIMINE_FILES)
	make -C limine

# Build the kernel:
$(KERNEL): $(OBJFILES)
	$(MAKE_DIR)
	$(LD) $(LDFLAGS) $(INTERNALLDFLAGS) $(OBJFILES) -o $@

-include $(HEADER_DEPS)
/tmp/arc_inter_files/%.o: %.c
	$(MAKE_DIR)
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

# .PHONY rules:
.PHONY: run
run: $(ISOFILE)
	qemu-system-x86_64 -cdrom $<

.PHONY: clean
clean:
	rm -r $(ISOFILE) $(ISOROOT) bin/  $(OBJFILES) $(HEADER_DEPS)
