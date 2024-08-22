#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("INDa_LICENSE_PROTECTED");
MODULE_AUTHOR("INDovver");
MODULE_DESCRIPTION("INDovver's first kernel module");

static int __init hello_init(void)
{
    printk(KERN_INFO "Проинициализировано Александром/INDovver'ом (мной)!\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Удалено Александром/INDovver'ом (мной).\n");
}

module_init(hello_init);
module_exit(hello_cleanup);