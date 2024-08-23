#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/stat.h>

#define FILE_NAME "module_procced_file" // Файл для взаимодействия
#define BUFFER_SIZE 10 // Размер буфера для сообщений (теперь это не маг число)
#define FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH // Права доступа к файлу (теперь это не маг число)

static int len, temp;  //С делал статиком (изоляция)
static char *msg; // Сделал статиком (изоляция)
// Все функци также сделал статиком (изоляция)
static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) {
    if (count > temp) {
        count = temp;
    }
    temp = temp - count;
    if(copy_to_user(buf, msg, count)!=0) return -EFAULT; // Если функция возвращает не ноль, то это ошибка,
    if (count == 0)                                      //  где EFAULT - ошибка сегментации, таким образом и ошибка обработана, и варнинга не будет
        temp = len;
    return count;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    if(copy_from_user(msg, buf, count)!=0) return -EFAULT; // Если функция возвращает не ноль, то это ошибка, 
    len = count;                                           //  где EFAULT - ошибка сегментации, таким образом и ошибка обработана, и варнинга не будет
    temp = len;
    return count;
}

static const struct proc_ops proc_fops = {
    proc_read : read_proc,
    proc_write : write_proc,
};

static void create_new_proc_entry(void) {  
    proc_create(FILE_NAME, FILE_MODE, NULL, &proc_fops); 
    msg = kmalloc(BUFFER_SIZE * sizeof(char), GFP_KERNEL);
}

static int __init proc_init(void) {  // Добавил __init, т.к. вроде помогает с оптимизацией
    create_new_proc_entry();
    return 0;
}

static void __exit proc_cleanup(void) {  // Добавил __exit, т.к. тоже вроде помогает с оптимизацией
    remove_proc_entry(FILE_NAME, NULL);
    kfree(msg);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("INDovver");
module_init(proc_init);
module_exit(proc_cleanup);
