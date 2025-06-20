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
 * This file uses 'dd' to send/receive data from the USB device
 */

int ddfd = -1;
unsigned long dd_current_offset = 0;

/* Writes data (stored in the dd.txt file) at the current offset in the 
 * USB device, using the 'dd' Linux command. */
bool bwrite_using_dd(char* device, char* message, unsigned long size) {
    // 1. write message into the 'dd.txt' file
    if (ddfd == -1) {
        ddfd = open("dd.txt", O_CREAT | O_RDWR, 0644);
        if (ddfd == -1) {printf("Error: could not open \"dd.txt\" file.\n"); return false;}
    }
    if (size != write(ddfd, message, size)) {
        printf("Error: could not write provided message to the \"dd.txt\" file.\n");
        return false;
    }

    // 2. run a 'dd' command to sync dd.txt with the USB drive
    char* command = (char*) malloc(512);
    if (!command) {printf("Error: could not allocate command buffer\n"); return false;}
    sprintf(command, 
           "dd if=dd.txt count=1 bs=%ld of=%s oflag=seek_bytes seek=%ld > /dev/null 2>&1", 
           size, device, dd_current_offset);
    // printf("Command: %s\n", command);
    system(command);
    system("sync");

    // 3. update the current offset dd is operating on
    dd_current_offset += size;
    return true;
}

/* Reads data at the current offset in the USB device to the dd.txt file, 
 * using the 'dd' Linux command. */
bool bread_using_dd(char* device, char* buffer, unsigned long size) {
    // 1. read from USB device to dd.txt
    char* command = (char*) malloc(512);
    if (!command) {printf("Error: could not allocate command buffer\n"); return false;}
    sprintf(command, 
            "sudo dd if=%s iflag=skip_bytes count=1 bs=%ld skip=%ld of=dd.txt > /dev/null 2>&1", 
            device, size, dd_current_offset);
    // printf("Command: %s\n", command);
    system("sync");
    system(command);

    // 2. read data from dd.txt into buffer
    if (ddfd == -1) {
        ddfd = open("dd.txt", O_RDONLY, 0644);
        if (ddfd == -1) {printf("Error: could not open \"dd.txt\" file.\n"); return false;}
    }
    if (read(ddfd, buffer, size) != size) {
        printf("Error: read back incorrect number of bytes from \"dd.txt\" \n"); 
        return false;
    }
    close(ddfd);

    // 3. update the current offset dd is operating on
    dd_current_offset += size;
    return true;
}

/* Specify offset and execute BWRITE using 'dd' */
void bwriteoffset_using_dd(char* device, char* buffer, unsigned long size, unsigned long offset) {
    dd_current_offset = offset;
    bwrite_using_dd(device, buffer, size);
}

/* Specify offset and execute BREAD using 'dd' */
void breadoffset_using_dd(char* device, char* buffer, unsigned long size, unsigned long offset) {
    dd_current_offset = offset;
    bread_using_dd(device, buffer, size);
}

/* Reads data from the USB device at the byte range between start and end using the 'dd' Linux 
 * command, and stores it into dd.txt. */
void bread_file_range_using_dd(char* device, unsigned long start, unsigned long end) {
    char* command = (char*) malloc(512);
    if (!command) {printf("Error: could not allocate command buffer\n"); return;}
    // sprintf(command, 
    //         "dd if=%s count=1 bs=%ld oflag=seek_bytes seek=%ld of=dd.txt > /dev/null 2>&1", 
    //         device, (end-start), start);
    sprintf(command, 
            "dd if=%s count=1 bs=%ld iflag=skip_bytes skip=%ld of=dd.txt > /dev/null 2>&1", 
            device, (end-start), start);
    // printf("Command: %s\n", command);
    system(command);
    system("sync");
}

/* Writes data (stored in the dd.txt file) into the byte range between start and end 
 * in the USB device, using the 'dd' Linux command. */
void bwrite_file_range_using_dd(char* device, unsigned long start, unsigned long end) {
    char* command = (char*) malloc(512);
    if (!command) {printf("Error: could not allocate command buffer\n"); return;}
    sprintf(command, 
           "dd if=dd.txt count=1 bs=%ld of=%s oflag=seek_bytes seek=%ld > /dev/null 2>&1", 
           (end-start), device, start);
    // printf("Command: %s\n", command);
    system(command);
    system("sync");
}

/* Compares the outputs from your kernel module (stored in kmod.txt) and the dd command
 * (stored in dd.txt) in a block-by-block manner. */
bool compare_kmod_and_dd_files(char* device, unsigned long blocksize, unsigned long totalsize) {
    int fd1,fd2;
    fd1 = open("dd.txt", O_RDONLY, 0644);
    fd2 = open("kmod.txt", O_RDONLY, 0644);
    char* msg_send = malloc(blocksize);
    char* msg_recv = malloc(blocksize);
    unsigned long current = 0;

    while (current < totalsize) {
        read(fd1, msg_send, blocksize);
        read(fd2, msg_recv, blocksize);
        if (!verify_data(msg_send, msg_recv, blocksize)) {
            printf("[X] FAILED (iteration: %ld) \n", (current/blocksize));
            printf("[.]     Expected: %s\n", msg_send);
            printf("[.]     Received: %s\n", msg_recv);
            return false;
        } else {
            printf("[*] PASSED (iteration: %ld) \n", (current/blocksize));
        }
        current += blocksize;
    }

    close(fd1);
    close(fd2);
    return true;
}