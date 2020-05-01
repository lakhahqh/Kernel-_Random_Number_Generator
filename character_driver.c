//Head
#include<linux/module.h>
#include<linux/fs.h>
//Device init
#include<linux/device.h>

// cap nhat bo nho cau truc du lieu cua driver va khoi tao//
#include<linux/slab.h>
#include"character_driver.h"
#include <linux/random.h>
#include <linux/init.h>   
#include <linux/kernel.h> 
#include<linux/cdev.h>
#include <linux/sched.h> 
#include <linux/workqueue.h> 
#include <linux/interrupt.h> 
#define DRIVER_AUTHOR "NGUYEN TAT HUNG - NGUYEN MINH DUC - NGUYEN HOANG ANH TU"
#define DRIVER_DESC "TEST MODULE LINUX"
#define DRIVER_VERSION "Released"
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);

static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

/* Day la ham xu ly ngat can duoc dang ki vao duong ngat danh cho ban phim */
irqreturn_t irq_handler(int irq, void * dev_id, struct pt_regs * regs) 
{  
    /*     * * This variables are static because they need to be     * * accessible (through pointers) to the bottom half routine.     * */  
    static unsigned char scancode;  
    unsigned char status;  
    /*    * * Read keyboard status    * */  
    status = inb(0x64);  
    scancode = inb(0x60);  
    switch (scancode) {  
        case 0x01:  
            printk(KERN_EMERG "! You pressed Esc ...\n");  
            break;  
        case 0x3B:  
            printk(KERN_EMERG "! You pressed F1 ...\n");  
            break;  
        case 0x3C:  
            printk(KERN_EMERG "! You pressed F2 ...\n");  
            break;  
        default:  
            break;  
    }  
    return IRQ_HANDLED;  
}  

 

typedef struct vchar_dev
{
	unsigned char* control_regs;
	unsigned char* status_regs;
	unsigned char* data_regs;
}vchar_dev_t;

struct _vchar_drv
{
	dev_t dev_number;
	struct class *dev_class;
	struct device *dev;
	vchar_dev_t* vchar_hw;
	struct cdev *vcdev;
}vchar_drv;


//khoi tao thiet bi vat ly//
int vchar_hw_init(vchar_dev_t*hw)
{
	char*buf;
	buf=kzalloc(NUM_DEV_REGS*REG_SIZE, GFP_KERNEL);
	if(!buf)
	{
		return -ENOMEM;
	}
	hw->control_regs=buf;
	hw->status_regs=hw->control_regs + NUM_CTRL_REGS;
	hw->data_regs=hw->status_regs + NUM_STS_REGS;
	


	//KHOI TAO GIA TRI THANH GHI//
	hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
	hw->status_regs[DEVICE_STATUS_REG]=0x03;
	return 0;
}



//XOA THIET BI VAT LY
void vchar_hw_exit(vchar_dev_t*hw)
{
	kfree(hw->control_regs);
}

///CAc ham entry POINT
static int dev_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "character_driver: Device is opening....\n");
    return 0;
}
static ssize_t dev_read(struct file *filp, char *buffer, size_t len, loff_t *offset)
{
	unsigned int randomnumber;
	get_random_bytes(&randomnumber, sizeof(randomnumber));
	printk("character_driver: Random number is : %u\n", randomnumber);
	return randomnumber;
}
static int dev_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "character_driver: Device successfully closed\n");
    return 0;
}
static struct file_operations fops =
{
        .open = dev_open,
        .read = dev_read,
        .release = dev_release,
};
//Init
static int __init character_driver_init(void)
{
	//cap nhat device number//
	int ret=0;
	vchar_drv.dev_number = 0;
	ret = alloc_chrdev_region(&vchar_drv.dev_number,0, 1, "character_device");
	if(ret<0)
	{
		printk("Failed to register device number. \n");
		return ret;
	}
	printk("Allocated device number (%d,%d) \n", MAJOR(vchar_drv.dev_number),MINOR(vchar_drv.dev_number));
	




	//tao device file//
	vchar_drv.dev_class = class_create(THIS_MODULE,"class_vchar_dev");
	if(vchar_drv.dev_class==NULL)
	{
		printk("Failed to create device file");
		unregister_chrdev_region(vchar_drv.dev_number, 1);
		return 0;
	}
	vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_number, NULL, "vchar_dev");
	if(IS_ERR(vchar_drv.dev))
	{
		printk("Failed to create device file");
		class_destroy(vchar_drv.dev_class);
		return 0;
	}
	printk("Device file has been created");

	//cap phat bo nho cau truc du lieu cua driver va khoi tao//
	vchar_drv.vchar_hw=kzalloc(sizeof(vchar_dev_t), GFP_KERNEL);
	if(!vchar_drv.vchar_hw){
		printk("Failed to allocate data structure of the device\n");
		ret=-ENOMEM;
		device_destroy(vchar_drv.dev_class, vchar_drv.dev_number);
		return 0;
	}
	printk("Data structure allocated successfully");


	//khoi tao thiet bi vat ly
	ret=vchar_hw_init(vchar_drv.vchar_hw);
	if(ret<0)
	{
		printk("Failed to initialize a virtual character device");
		kfree(vchar_drv.vchar_hw);
		return 0;
	}
	printk("Virtual character device initialized successfully");
	//dang ky cac entry point//
	vchar_drv.vcdev=cdev_alloc();
	if(vchar_drv.vcdev==NULL){
		printk("Failed to allocate cdev structure");
		vchar_hw_exit(vchar_drv.vchar_hw);
		return 0;
	}
	cdev_init(vchar_drv.vcdev, &fops);
	ret=cdev_add(vchar_drv.vcdev, vchar_drv.dev_number, 1);
	if(ret<0){
		printk("Failed to add entry point!");
		vchar_hw_exit(vchar_drv.vchar_hw);
		return 0;
	}
	printk("Entry points added succesfully");
	
	//dang ky ham xu ly ngat//
	  
    	ret = request_irq(1,(irq_handler_t)irq_handler,IRQF_SHARED,"character_driver_interrupt", (void * )(irq_handler));  
    	
   	if (ret<0){ 
	printk(KERN_EMERG "Can't initialize character_driver_interrupt\n"); 
	}


	printk("Itnitialized character driver successully \n");
	return 0;
}


//end
 static void __exit character_driver_exit(void)
{	
	//HUy dang ky xu ly ngat//
	free_irq(1, (void * )(irq_handler));
	//huy dang ky entry point vs kernel//
	cdev_del(vchar_drv.vcdev);

	//giai phong thiet bi vat ly//
	vchar_hw_exit(vchar_drv.vchar_hw);	



	//giai phong bu nho da cap phat cau truc du lieu cua driver//
	kfree(vchar_drv.vchar_hw);

	//xoa bo device file//
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_number);
	class_destroy(vchar_drv.dev_class);
	

	//giai phong device number//
	unregister_chrdev_region(vchar_drv.dev_number, 1);



	printk("Character_driver exited\n");
}

//GOIHAM
module_init(character_driver_init);
module_exit(character_driver_exit);

//GIAYPHEP
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_SUPPORTED_DEVICE("TEST DEVICE");
