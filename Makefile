THUMB_COMPILE_FLAGS = \
	-r \
	-march=armv5t \
	-mthumb \

EMBEDDED_CPP_COMPILE_FLAGS = \
	-fno-rtti \
	-fno-exceptions

INCLUDE_FLAGS = \
	-Iinclude/ \
	-Iinclude/swan \
	-Ilib \
	-Isrc

ALL_TARGETS = \
	demo3d \
	demo3d_narc

all : $(ALL_TARGETS)

demo3d : src/demo3d/demo3d.cpp
	@echo "Building $@"
	@mkdir -p out/demo3d/$(dir $@)
	@arm-none-eabi-gcc $(THUMB_COMPILE_FLAGS) $(EMBEDDED_CPP_COMPILE_FLAGS) $(INCLUDE_FLAGS) $^ -o out/demo3d/demo3d_reloc.elf

demo3d_narc :
	@echo "Removing old cutscene NARC."
	@rm -f $(VFS_ROOT)/data/demo3d_seq_data.narc
	@echo "Packing new cutscene NARC."
	@tools/knarc -d resource/demo3d/bin -p resource/demo3d/narc/demo3d_seq_data.narc
	@mv resource/demo3d/narc/demo3d_seq_data.narc $(VFS_ROOT)/data/demo3d_seq_data.narc

clean :
	@rm -rf out/
	@rm -rf resource/demo3d/narc