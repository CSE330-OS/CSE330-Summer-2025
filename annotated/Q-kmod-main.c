#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/skbuff.h>
#include <linux/freezer.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>

/* File IO-related headers */
#include <linux/fs.h>
#include <linux/bio.h>
#include <linux/buffer_head.h>
#include <linux/blkdev.h>
#include <linux/version.h>
#include <linux/blkpg.h>     
#include <linux/namei.h>     

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("A Block Abstraction Read/Write for a USB device.");
MODULE_VERSION("1.0");

/* USB device name argument */
char* device = "/dev/sdb";
module_param(device, charp, S_IRUGO);


/* USB device system handler */
unsigned int cur_dev_sector = 0;
static struct block_device *bdevice = NULL;
static struct bio *usb_bio = NULL;
static struct file *usb_file = NULL;


bool kmod_ioctl_init(void);
void kmod_ioctl_teardown(void);
long rw_usb(char* data, unsigned int size, unsigned int  offset, bool flag);

static bool open_usb(void)
{
    /* Open a file for the path of the usb */
    usb_file = bdev_file_open_by_path(device, BLK_OPEN_READ | BLK_OPEN_WRITE, NULL, NULL);
    if (IS_ERR(usb_file)) {
        printk("error: failed to open the device (%s) using bdev_file_open_by_path().\n",
               device);
        usb_file = NULL;
        return false;
    }
    bdevice = file_bdev(usb_file);
    
    /* Perform various sanity checks to make sure the device works */
    if (IS_ERR(bdevice) || !bdevice) {
        printk("error: failed to open the device (%s) using lookup_bdev().\n", device);
        return false;
    }
    printk("success: opened %s as a block device.\n", bdevice->bd_disk->disk_name);

    usb_bio = bio_alloc(bdevice, 256, REQ_OP_WRITE, GFP_NOIO);
    if (!usb_bio || IS_ERR(usb_bio)) {
        printk("error: failed to allocate a bio structure.\n");
        fput(usb_file);
        return false;
    }
    printk("success: allocated usb_bio.\n");
    return true;
}

// API to write to the attached USB device
long rw_usb(
    char*         data,   /*  message buffer */ 
    unsigned int  size,   /*  message length */
    unsigned int  offset, /*  write location */
    bool          flag    /*  1=write, 0=read */
) 
{
    unsigned int remaining = size;
    unsigned int processed = 0;
    int len;

    unsigned int page_offset = 0;
    int bio_add_page_result = 0;

    /* Sanity check */
    if (IS_ERR(data)) {
        printk("error: message not correct.\n");
        return -1;
    }

    /* Break down writes into 512 bytes chunks */
    while (remaining > 0) {
        /* Set the BIO structure */

        // TODO: Use bio_set_dev() to assign usb_bio to bdevice

        if (offset == -1) {
            /* Use the previous offset */
            // TODO: Set bi_iter.bi_sector to cur_dev_sector
        
        } else {
            /* Use the provided offset */
            printk("offset is provided (bytes = %d, sector = %d)\n", offset, offset/512);
            // TODO: Set bi_iter.bi_sector

            // TODO: Update cur_dev_sector value for future tracking
            
            // TODO: Reset offset so it's not reused in next iteration
           
        }

        /* Set read/write op based on the flag */
        if (flag == true) {
            usb_bio->bi_opf = REQ_OP_WRITE;
            printk("WRITE (size = %d, offset = %lld)\n", 
                size, usb_bio->bi_iter.bi_sector);
        } else {
            usb_bio->bi_opf = REQ_OP_READ;
            printk("READ (size = %d, offset = %lld)\n", 
                size, usb_bio->bi_iter.bi_sector);
        }

        /* Check if the current sector is overflowing the device */

        // Add data to the bio in chunks of 512 bytes
        // TODO: Use bio_add_page() to add bytes at the correct offset
        // Use vmalloc_to_page(data) to get the page from the virtual address
      
        /* Submit BIO and wait for op completion */
        len = submit_bio_wait(usb_bio);
        if (len < 0) {
            printk("error: bio communication failed (ret = %d)\n", len);
            return processed;
        }
        bio_reset(usb_bio, bdevice, FMODE_WRITE);

        /* Decrement the chunk */
        // TODO: Increment cur_dev_sector
        
        // TODO: Calculate remaining bytes
        // TODO: Calculate processed bytes

        // TODO: Update page_offset
        
        // TODO: If page_offset reaches one page, increment 'data'

        // TODO: Reset page_offset
    }

    return processed;
}


static void close_usb(void)
{
    /* Close the file and device communication interface */

    // TODO: Check if the usb_file is valid
    // TODO: Use bdev_fput() to release the file reference
    
}

static int __init kmod_init(void)
{
    pr_info("Hello World!\n");
    if (!open_usb()) {
        pr_err("Failed to open USB block device\n");
        return -ENODEV;
    }
    kmod_ioctl_init();
    return 0;
}

static void __exit kmod_fini(void)
{
    close_usb();
    kmod_ioctl_teardown();
    printk("Goodbye, World!\n");
}

module_init(kmod_init);
module_exit(kmod_fini);
