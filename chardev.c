#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>        
#include <linux/kernel.h>        
#include <linux/fs.h>   
#include <linux/kdev_t.h>        
#include <asm/uaccess.h>         
#define  DEVICE_NAME "chardev"   
#define  CLASS_NAME  "dev"       
 
MODULE_LICENSE("GPL");            
MODULE_AUTHOR("zahra ghaderi, mahdiye khademian, zahra madineh :)");
MODULE_DESCRIPTION("A simple Linux char driver for the dev"); 
MODULE_VERSION("0.1"); 
          
        
static struct class*  chardevClass  = NULL; 
static struct device* chardevDevice = NULL; 
static int    major_Number;                  
static char   mes[256] = {0};             
static int    number_Opens = 0;      
 
static int     my_open(struct inode *, struct file *);
static int     my_release(struct inode *, struct file *);
static ssize_t my_write(struct file *, const char *, size_t, loff_t *);
 
static struct file_operations fop =
{
   .open = my_open,
   .write = my_write,
   .release = my_release,
};
 
static int __init chardev_init(void){
   printk(KERN_INFO "mychardev: Initializing the chardev LKM\n");

   major_Number = register_chrdev(0, DEVICE_NAME, &fop);
   if (major_Number<0){
      printk(KERN_ALERT "chardev failed to register a major number\n");
      return major_Number;
   }
   printk(KERN_INFO "mychardev: The major number of chardev is %d\n", major_Number);
 
   // Register the device class
   chardevClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(chardevClass)){                // Check for error and clean up if there is
      unregister_chrdev(major_Number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(chardevClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "mychardev: Device class registered correctly!\n");
 
   // Register the device driver
   chardevDevice = device_create(chardevClass, NULL, MKDEV(major_Number, 0), NULL, DEVICE_NAME);
   if (IS_ERR(chardevDevice)){               // Clean up if there is an error
      class_destroy(chardevClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(major_Number, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(chardevDevice);
   }
   printk(KERN_INFO "mychardev: Device class created correctly!\n"); // Made it! device was initialized
   return 0;
}
 
static void __exit chardev_exit(void){
   device_destroy(chardevClass, MKDEV(major_Number, 0));    // remove the device
   class_unregister(chardevClass);                        // unregister the device class
   class_destroy(chardevClass);                          // remove the device class
   unregister_chrdev(major_Number, DEVICE_NAME);         // unregister the major number
   printk(KERN_INFO "mychardev: Goodbye from the LKM!\n");
}
 
static int my_open(struct inode *inodep, struct file *filep){
   number_Opens++;
   printk(KERN_INFO "mychardev: Device has been opened %d time(s)!\n", number_Opens);
   return 0;
}
static ssize_t my_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   printk("Massage received is : %s",mes);
   copy_from_user(mes, buffer, len);
   printk("%s",mes);  			      // write received string 
   return len;
}
 
static int my_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "mychardev: Device successfully closed!\n");
   return 0;
}
 
module_init(chardev_init);
module_exit(chardev_exit);
