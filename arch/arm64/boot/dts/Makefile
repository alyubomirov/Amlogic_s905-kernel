dtb-$(CONFIG_ARCH_VEXPRESS) += gxbb_p200.dtb gxbb_p200_2G.dtb \
				gxl_p212_1g.dtb gxl_p212_2g.dtb \
				gxbb_p200_2G_vegas95.dtb

targets += dtbs

DTB_NAMES := $(subst $\",,$(CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE_NAMES))
ifneq ($(DTB_NAMES),)
DTB_LIST := $(addsuffix .dtb,$(DTB_NAMES))
else
DTB_LIST := $(dtb-y)
endif
targets += $(DTB_LIST)

dtbs: $(addprefix $(obj)/, $(DTB_LIST))

clean-files := dts/*.dtb *.dtb
