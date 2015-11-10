#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x75722d02, "module_layout" },
	{ 0x91766c09, "param_get_ulong" },
	{ 0x799c50a, "param_set_ulong" },
	{ 0xfdc91274, "platform_driver_register" },
	{ 0x8efb9c01, "platform_device_register" },
	{ 0x3c97a268, "input_event" },
	{ 0xa7f44650, "add_timer" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xb6867ec3, "del_timer" },
	{ 0xf0efc300, "input_register_device" },
	{ 0x313341a3, "_set_bit_le" },
	{ 0xbdc991f2, "input_allocate_device" },
	{ 0xf229e057, "init_timer_key" },
	{ 0xc5893c6c, "input_free_device" },
	{ 0x8fea26a3, "input_unregister_device" },
	{ 0x7d56ee75, "platform_device_unregister" },
	{ 0x932e75a5, "platform_driver_unregister" },
	{ 0xea147363, "printk" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0x7d11c268, "jiffies" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

