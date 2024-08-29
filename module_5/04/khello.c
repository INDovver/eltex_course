#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h> // Для copy_to_user и copy_from_user

#define SUCCESS 0
#define DEVICE_NAME "chardevasd" /* Имя устройства, как оно будет отображаться в /proc/devices */
#define BUF_LEN 80 /* Максимальная длина сообщения от устройства */

// Прототипы функций
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

// Глобальные переменные 
static int major; /* Главный номер, присвоенный нашему драйверу устройства */
static char msg[BUF_LEN]; /* Сообщение, которое устройство отдаст при запросе */
static int msg_size = 0; /* Текущий размер сообщения */
static atomic_t already_open = ATOMIC_INIT(0); /* Флаг открытия устройства */
static struct class *cls; /* Класс устройства */

// Операции с файлом для символьного устройства 
static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

// Инициализация модуля 
static int __init chardev_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
    if (major < 0) {
        pr_alert("Регистрация символьного устройства не удалась с кодом %d\n", major);
        return major;
    }
    pr_info("Устройство зарегистрировано с главным номером %d.\n", major);

    cls = class_create(DEVICE_NAME); 
    if (IS_ERR(cls)) {
        unregister_chrdev(major, DEVICE_NAME);
        pr_alert("Не удалось создать класс устройства.\n");
        return PTR_ERR(cls);
    }

    if (IS_ERR(device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME))) {
        class_destroy(cls);
        unregister_chrdev(major, DEVICE_NAME);
        pr_alert("Не удалось создать устройство.\n");
        return PTR_ERR(cls);
    }

    pr_info("Устройство создано в /dev/%s\n", DEVICE_NAME);
    return SUCCESS;
}

// Выход из модуля 
static void __exit chardev_exit(void) {
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("Символьное устройство успешно удалено.\n");
}

// Функция открытия устройства 
static int device_open(struct inode *inode, struct file *file) {
    if (atomic_cmpxchg(&already_open, 0, 1)) {
        pr_alert("Устройство уже открыто!\n");
        return -EBUSY; /* Устройство уже используется */
    }

    try_module_get(THIS_MODULE); // Увеличение счетчика использования модуля 
    return SUCCESS;
}

// Функция закрытия устройства 
static int device_release(struct inode *inode, struct file *file) {
    atomic_set(&already_open, 0); // Устанавливаем флаг в 0 (не используется) 
    module_put(THIS_MODULE); // Уменьшение счетчика использования модуля 
    return SUCCESS;
}

// Функция чтения из устройства 
static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    int bytes_to_read;
    
    if (*offset >= msg_size) {
        return 0; // Конец файла 
    }

    // Определяем, сколько байтов копировать 
    bytes_to_read = min(length, (size_t)(msg_size - *offset));

    if (copy_to_user(buffer, msg + *offset, bytes_to_read)) {
        return -EFAULT;
    }

    *offset += bytes_to_read;
    return bytes_to_read;
}

// Функция записи в устройство 
static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
    int bytes_to_write;

    if (len > BUF_LEN) {
        pr_alert("Превышение максимальной длины сообщения.\n");
        return -EINVAL;
    }

    // Копируем данные из пользовательского пространства в пространство ядра 
    bytes_to_write = len;
    if (copy_from_user(msg, buff, bytes_to_write)) {
        return -EFAULT;
    }

    msg_size = bytes_to_write;
    pr_info("Получено сообщение: %s\n", msg);
    return bytes_to_write;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("INDovver");
MODULE_DESCRIPTION("Обмен информацией с ядром через chardev");
