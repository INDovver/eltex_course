#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h> /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_DESCRIPTION("Example module illustrating the use of Keyboard LEDs.");
MODULE_LICENSE("GPL");

static struct timer_list my_timer;
static struct tty_driver *my_driver;
static struct kobject *example_kobject;

static int _kbledstatus = 0;
static int test = 0; // Значение по умолчанию (для иллюстрации)

#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", test);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int val;
    if (sscanf(buf, "%d", &val) == 1) {
        if (val >= 0 && val <= ALL_LEDS_ON) {
            test = val;
        } else {
            pr_debug("Invalid value: %d\n", val);
        }
    } else {
        pr_debug("Failed to parse input: %s\n", buf);
    }
    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(test, 0660, foo_show, foo_store);

static void my_timer_func(struct timer_list *ptr)
{
    int *pstatus = &_kbledstatus;
    if (*pstatus == test)
        *pstatus = RESTORE_LEDS;
    else
        *pstatus = test;

    (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *pstatus);

    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);
}

static int __init kbleds_init(void)
{
    int i;
    printk(KERN_INFO "kbleds: loading\n");
    printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);

    example_kobject = kobject_create_and_add("systest", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    if (sysfs_create_file(example_kobject, &foo_attribute.attr)) {
        pr_debug("Failed to create the foo file in /sys/kernel/systest\n");
    }

    for (i = 0; i < MAX_NR_CONSOLES; i++) {
        if (!vc_cons[i].d)
            break;
        printk(KERN_INFO "kbleds: console[%i/%i] #%i, tty %lx\n", i,
               MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
               (unsigned long)vc_cons[i].d->port.tty);
    }

    printk(KERN_INFO "kbleds: finished scanning consoles\n");

    my_driver = vc_cons[fg_console].d->port.tty->driver;
    timer_setup(&my_timer, my_timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    return 0;
}

static void __exit kbleds_cleanup(void)
{
    printk(KERN_INFO "kbleds: unloading...\n");

    // Остановить таймер
    del_timer(&my_timer);

    // Восстановить исходное состояние светодиодов клавиатуры
    (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);

    // Удалить файл из sysfs
    kobject_put(example_kobject);

    printk(KERN_INFO "kbleds: module unloaded\n");
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("INDovver");
MODULE_DESCRIPTION("A simple module to control keyboard LEDs based on input from sysfs.");