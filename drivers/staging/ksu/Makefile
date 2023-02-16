obj-$(CONFIG_KSU) += ksu.o
obj-$(CONFIG_KSU) += allowlist.o
kernelsu-objs := apk_sign.o
obj-$(CONFIG_KSU) += kernelsu.o
obj-$(CONFIG_KSU) += module_api.o
obj-$(CONFIG_KSU) += sucompat.o
obj-$(CONFIG_KSU) += uid_observer.o
obj-$(CONFIG_KSU) += manager.o
obj-$(CONFIG_KSU) += core_hook.o
obj-$(CONFIG_KSU) += ksud.o
obj-$(CONFIG_KSU) += embed_ksud.o
obj-$(CONFIG_KSU) += kernel_compat.o

obj-$(CONFIG_KSU) += selinux/

ifndef EXPECTED_SIZE
EXPECTED_SIZE := 0x033b
endif

ifndef EXPECTED_HASH
EXPECTED_HASH := 0xb0b91415
endif

ccflags-y += -DEXPECTED_SIZE=$(EXPECTED_SIZE)
ccflags-y += -DEXPECTED_HASH=$(EXPECTED_HASH)
ccflags-y += -Wno-implicit-function-declaration -Wno-strict-prototypes -Wno-int-conversion -Wno-gcc-compat
ccflags-y += -Wno-macro-redefined -Wno-declaration-after-statement
