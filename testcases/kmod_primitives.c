#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdbool.h>
#include "common.h"

/* 
 * Description:
 * This file uses your kernel module to send/receive data from the USB device.
 * Temporarily, it stores all sent/received data in "kmod.txt" for comparison.
 */
int kmod_dev_fd; 

bool open_kmod(void) {
    kmod_dev_fd = open("/dev/kmod", O_RDWR);
    if (kmod_dev_fd < 0) {
        printf("error: couldn't open /dev/kmod\n");
        return false;
    }
    return true;
}

void close_kmod(void) {
    close(kmod_dev_fd);
}

void bread(char* buffer, unsigned int size) {
    struct block_rw_ops request;
    request.data   = buffer;
    request.size   = size;
    ioctl(kmod_dev_fd, BREAD, &request);
}

void breadoffset(char* buffer, unsigned int size, unsigned int offset) {
    struct block_rwoffset_ops request;
    request.data   = buffer;
    request.size   = size;
    request.offset  = offset;
    ioctl(kmod_dev_fd, BREADOFFSET, &request);
}

void bwrite(char* buffer, unsigned int size) {
    struct block_rw_ops request;
    request.data   = buffer;
    request.size   = size;
    ioctl(kmod_dev_fd, BWRITE, &request);
}

void bwriteoffset(char* buffer, unsigned int size, unsigned int offset) {
    struct block_rwoffset_ops request;
    request.data    = buffer;
    request.size    = size;
    request.offset  = offset;
    ioctl(kmod_dev_fd, BWRITEOFFSET, &request);
}