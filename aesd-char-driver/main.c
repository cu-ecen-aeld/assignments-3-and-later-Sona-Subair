/**
 * @file aesdchar.c
 * @brief Functions and data related to the AESD char driver implementation
 *
 * Based on the implementation of the "scull" device driver, found in
 * Linux Device Drivers example code.
 *
 * @author Dan Walkes
 * @date 2019-10-22
 * @copyright Copyright (c) 2019
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h> // file_operations
#include "aesdchar.h"
int aesd_major =   0; // use dynamic major
int aesd_minor =   0;

MODULE_AUTHOR("Sona Subair");
MODULE_LICENSE("Dual BSD/GPL");

struct aesd_dev aesd_device;


int find_new_line(char* buffer, int count){
    for(int i=0;i<count;i++){
        if(buffer[i]=='/n'){
            return(i);
        }
    }
    return(-1);
}


int aesd_open(struct inode *inode, struct file *filp)
{
    struct aesd_dev *dev;
    dev=container_of(inode->i_cdev, struct aesd_dev, cdev);
    flip->private_data=dev;
    if ( (filp->f_flags & O_ACCMODE) =  = O_WRONLY) {
        scull_trim(dev); /* ignore errors */
    }
    PDEBUG("open");  
    return 0;
}

int aesd_release(struct inode *inode, struct file *filp)
{
    PDEBUG("release");
    return 0;
}

ssize_t aesd_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
    ssize_t retval = 0,ret,entry_size;
    struct aesd_buffer_entry *aesd_entry;
    struct aesd_dev *driver_data = filp->private_data;
    size_t entry_offset_byte_rtn;
    PDEBUG("read %zu bytes with offset %lld",count,*f_pos);
    ret= mutex_lock_interruptible(&driver_data->char_dev_mutex_lock);
    if(ret!=0){
        PDEBUG("Error while locking mutex");
        return ret;
    }
    if(aesd_circular_buffer_find_entry_offset_for_fpos(&driver_data->circular_buffer, *f_pos, &entry_offset_byte_rtn)!=NULL){
        data_size=(entry_offset_byte_rtn < count)? entry_offset_byte_rtn:count;
        ret=copy_to_user(buf, (entry_loc->buffptr + entry_offset_byte_rtn), data_size);
        if(ret){
            retval=-EFAULT;
            PDEBUG("Copying to user space failed");
        }
        else{
            retval=data_size;
            *f_pos+=data_size;
        }
    }
    ret= mutex_unlock(&driver_data->char_dev_mutex_lock);
    if(ret!=0){
    PDEBUG("Error while unlocking mutex");
    return ret;
    }
    else{
        retval=0;
    }
    return retval;
}

ssize_t aesd_write(struct file?
 *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
    ssize_t retval = -ENOMEM,ret;
    int delimiter_index;
    struct aesd_dev *driver_data = filp->private_data; 
    struct aesd_buffer_entry entry_data;
    size_t packet_start=0,allocation_size=0,packet_buffer_size=0;
    char* kernel_buffer=NULL,packet_buffer,return_buffer;
    struct aesd_buffer_entry buf;

    PDEBUG("write %zu bytes with offset %lld",count,*f_pos);
    ret= mutex_lock_interruptible(&driver_data->char_dev_mutex_lock);
    if(ret!=0){
        PDEBUG("Error while locking mutex");
        return ret;
    }
    kernel_buffer=kmalloc(count,GFP_KERNEL);
    if(kernel_buffer!=NULL){
        ret=copy_from_user(kernel_buffer,buf,count);
        if(ret!=0){
            ret=-EFAULT;
            PDEBUG("Copying from user failed");
            goto exit_mutex_unlock;
        }
        while(delimiter_index!=-1){
        delimiter_index=find_new_line(kernel_buffer,count);
        allocation_size=delimiter_index==-1?(count-packet_start):(delimiter-packet_start);
        if(packet_buffer_size==0){
            packet_buffer=kmalloc(allocation_size,GFP_KERNEL;)
            if(packet_buffer==NULL){
                PDEBUG("Mallocation failed");
                goto exit_mutex_unlock;
            }
        }
        if(packet_buffer_size>0){
            packet_buffer=krealloc(packet_buffer,allocation_size);
            if(packet_buffer==NULL){
                PDEBUG("Reallocation failed");
                goto exit_mutex_unlock;
            }         
        }
        memcpy((packet_buffer+packet_buffer_size),kernel_buffer,allocation_size);
        packet_buffer_size+=allocation_size;
        if(delimiter_index!=-1){
            buf.buffptr=packet_buffer;
            buf.size=packet_buffer_size;
            return_buffer=aesd_circular_buffer_add_entry(&driver_data->c_buffer,&buf);
            if(return_buffer!=NULL){
                free(return_buffer);
            }
        }
        retval+=allocation_size;
        }
    
    }
    exit_mutex_unlock:
    ret= mutex_unlock(&driver_data->char_dev_mutex_lock);
    if(ret!=0){
    PDEBUG("Error while unlocking mutex");
    return ret;
    }    
    
    return retval;
}
struct file_operations aesd_fops = {
    .owner =    THIS_MODULE,
    .read =     aesd_read,
    .write =    aesd_write,
    .open =     aesd_open,
    .release =  aesd_release,
};

static int aesd_setup_cdev(struct aesd_dev *dev)
{
    int err, devno = MKDEV(aesd_major, aesd_minor);

    cdev_init(&dev->cdev, &aesd_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &aesd_fops;
    err = cdev_add (&dev->cdev, devno, 1);
    if (err) {
        printk(KERN_ERR "Error %d adding aesd cdev", err);
    }
    return err;
}



int aesd_init_module(void)
{
    dev_t dev = 0;
    int result;
    result = alloc_chrdev_region(&dev, aesd_minor, 1,
            "aesdchar");
    aesd_major = MAJOR(dev);
    if (result < 0) {
        printk(KERN_WARNING "Can't get major %d\n", aesd_major);
        return result;
    }
    memset(&aesd_device,0,sizeof(struct aesd_dev));
    mutex_init(&aesd_device.char_dev_mutex_lock);
    result = aesd_setup_cdev(&aesd_device);

    if( result ){
        unregister_chrdev_region(dev, 1);
    }
    return result;

}

void aesd_cleanup_module(void)
{
    dev_t devno = MKDEV(aesd_major, aesd_minor);

    cdev_del(&aesd_device.cdev);
    offset_in=aesd_device.circular_buffer.in_offs;
    offset_out=aesd_device.circular_buffer.out_offs;
    while(aesd_device.circular_buffer.full!=false){
        kfree(aesd_device.circular_buffer.entry[offset_in].buffptr);
    }
    mutex_destroy(&aesd_device.char_dev_mutex_lock);
    unregister_chrdev_region(devno, 1);
}



module_init(aesd_init_module);
module_exit(aesd_cleanup_module);
