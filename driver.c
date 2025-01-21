#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#include "driver.h"

// PLEASE IGNORE THE SCREEN,  TRIED :(

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xZist");
MODULE_DESCRIPTION("Char driver that implements dynamic circular queue");

static int dcq_vicharak_open(struct inode* device_file, struct file* instance){
  return 0;
}

static int dcq_vicharak_close(struct inode* device_file, struct file* instance){
  return 0;
}


// TODO: Make an actual queue, !!
//
// Welcome back !!

typedef struct dcq{
  data* items;
  int max_size;
  int size;
  int start;
  int end;
} dcq;

dcq queue = {.size = 0, .end = -1, .start = 0, .max_size = 0, .items = NULL};

static DEFINE_MUTEX(q_mutex);
static DECLARE_WAIT_QUEUE_HEAD(wait);

static long int driver(struct file* file, unsigned command, unsigned long arg){
  int new_size;
  data u_data;
  switch (command) {
  case SET_SIZE_OF_QUEUE:
      if(copy_from_user(&new_size, (int __user*)arg, sizeof(int))){
        return ERROR;
      }

      mutex_lock(&q_mutex);
      data* new_items = kmalloc(new_size * sizeof(data), GFP_KERNEL);
      if(!new_items){
        printk("vicharak.resize - kmalloc failed!\n");
        mutex_unlock(&q_mutex);
        return ERROR;
      }

      int i = 0;
      while(queue.size > 0 && i < new_size){
        new_items[i] = queue.items[queue.start];
        queue.start = ( queue.start + 1 ) % queue.max_size;
        queue.size--;
        i++;
      }

      queue.items = new_items;
      queue.max_size = new_size;
      queue.start = 0;
      queue.end = i - 1;
      queue.size = i;
      mutex_unlock(&q_mutex);
      printk("vicharak.resize - resized the dcq to %d\n", queue.max_size);
      return OK;
      break;

  case PUSH_DATA:
      if(copy_from_user(&u_data, (data __user*)arg, sizeof(data))){
        printk("vicharak.push - copy from user failed!\n");
        return ERROR;
      }
      if (u_data.length <= 0){
        return INVALID_IP;
      }

      if(!access_ok(u_data.data, u_data.length)){
        return ERROR;
      }

      char* buffer = kmalloc(u_data.length * sizeof(char), GFP_KERNEL);
      if(! buffer){
        printk("vicharak.push - kmalloc failed!!\n");
        return ERROR;
      }
      if(copy_from_user(buffer, u_data.data, u_data.length * sizeof(char))){
        kfree(buffer);
        printk("vicharak.push - copy from user failed!!\n");
        return ERROR;
      }

      mutex_lock(&q_mutex);
      if(queue.size == queue.max_size){
        printk("vicharak.push - dcq is full!!\n");
        kfree(buffer);
        mutex_unlock(&q_mutex);
        return FULL;
      }

      u_data.data = buffer;
      queue.end = (queue.end + 1) % queue.max_size;
      queue.items[queue.end] = u_data;
      queue.size++;
      wake_up_interruptible(&wait);
      mutex_unlock(&q_mutex);
      printk("vicharak.push - pushed to dcq: {.length: %d, .data: %s}\n", queue.items[queue.end].length, queue.items[queue.end].data); // This was the error, was accessing user space memory in kernel space, SORRY :(
      return OK;
      break;

  case POP_DATA:
    if (copy_from_user(&u_data, (data __user*)arg, sizeof(data))){
      printk("vicharak.pop - copy from user failed\n");
      return ERROR;
    }else{
      mutex_lock(&q_mutex);
      
      while (queue.size == 0) {
        mutex_unlock(&q_mutex);
        if (wait_event_interruptible(wait, queue.size > 0)){
            return ERROR;
        }
        mutex_lock(&q_mutex);
      }

      if (u_data.length < queue.items[queue.start].length){
        printk("vicharak.pop - not enough space in buffer!\n");
        mutex_unlock(&q_mutex);
        return INVALID_IP;
      } 

      if(copy_to_user(u_data.data, queue.items[queue.start].data, queue.items[queue.start].length)) {
        printk("vicharak.pop - copy to user failed!\n");
        mutex_unlock(&q_mutex);
        return ERROR;
      }

      if(copy_to_user(&(((data __user*)arg)->length), &(queue.items[queue.start].length), sizeof(int))) {
        printk("vicharak.pop - copy to user failed!!\n");
        mutex_unlock(&q_mutex);
        return ERROR;
      }
      
      u_data.length = queue.items[queue.start].length;
      kfree(queue.items[queue.start].data);
      queue.start = (queue.start + 1) % queue.max_size;
      queue.size--;

      mutex_unlock(&q_mutex);
      printk("vicharak.pop - popped an element!\n");
      return OK;
    }
    break;
  default:
      return INVALID_IP;
    break;
  }

  return OK;
}

static struct file_operations query_fops = {
  .owner = THIS_MODULE,
  .open = dcq_vicharak_open,
  .release = dcq_vicharak_close,
  .unlocked_ioctl = driver
};

static unsigned int MAJOR = 64;

static int __init ModuleInit(void){
  int retVal = 0;
  retVal = register_chrdev(MAJOR, "dcq_vicharak__", &query_fops);
  if(retVal == 0){
    printk("vicharak.init - registered, major: %d, minor: %d\n", MAJOR, 0);
  }else if(retVal > 0){
    printk("vicharak.init - registered, major: %d, minor: %d\n", retVal >> 20, retVal & 0xfffff);
    MAJOR =  retVal >> 20;
  }else{
    printk("vicharak.init - failed to register device\n");
    return -1;
  }
  return 0;
};

static void __exit ModuleExit(void){
  unregister_chrdev(MAJOR, "dcq_vicharak__");
  printk("vicharak.exit - unregistered device!\n");
};

module_init(ModuleInit);
module_exit(ModuleExit);
