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
 * Description: Similar to test-read-large, but the block size and iterations can be variable.  
 */

int main (int argc, char* argv[]) {
    printf("Testcase: read-variable\n");
    if (!open_kmod()) {
        printf("error: could not open kmod chardev.\n"); 
        return 0;
    }

    if (argc < 4) {
        printf("Usage: ./test.sh test-read-variable <blocksize> <iterations> <offset>\n");
        return -1;
    }

    char* device                    = argv[1];
    char* remaining;
    int   BLOCKSIZE                 = atoi(argv[2]);
    int   ITERATIONS                = atoi(argv[3]);
    unsigned long   OFFSET          = strtol(argv[4], &remaining, 10);
    char* msg_send = malloc(BLOCKSIZE);
    char* msg_recv = malloc(BLOCKSIZE);

    // Open two files 'kmod.txt' and 'dd.txt' for comparison
    int fd1, fd2;
    fd1 = open("dd.txt", O_CREAT | O_RDWR, 0644);
    fd2 = open("kmod.txt", O_CREAT | O_RDWR, 0644);
    if (fd1 == -1 || fd2 == -1) {printf("Error: could not open files.\n"); return -1;}

    // Write BLOCKSIZE*ITERATIONS worth of (random) data into the USB device using 'dd'
    // Write the random data to the 'dd.txt' file as well
    for (int i = 0; i < ITERATIONS; i++) {
        char* msg_send = (char*) generate_random_bytes(BLOCKSIZE);
        write(fd1, msg_send, BLOCKSIZE);
    }
    close(fd1);
    bwrite_file_range_using_dd(device, OFFSET, (OFFSET+(BLOCKSIZE*ITERATIONS)));

    // Read the data written into the USB device using your kernel module
    // Write this retrieved data into the 'kmod.txt' file
    unsigned long current_offset = OFFSET;
    for (int i = 0; i < ITERATIONS; i++) {
        if (OFFSET == 0) {
            bread(msg_recv, BLOCKSIZE);
        } else {
            breadoffset(msg_recv, BLOCKSIZE, current_offset);
            current_offset += BLOCKSIZE;
        }
        write(fd2, msg_recv, BLOCKSIZE);
    }

    // Compare the results of the two files (block-by-block) to make sure they match
    if (!compare_kmod_and_dd_files(device, BLOCKSIZE, BLOCKSIZE*ITERATIONS)) {
        printf("[X] FAILED; Output does not match 'dd'\n");
        return -1;
    }

    printf("-------------------------------------------------------------\n");
    printf("Parameters: BLOCKSIZE (%d), ITERATIONS (%d), OFFSET (%ld)\n", BLOCKSIZE, ITERATIONS, OFFSET);
    printf("[*] READ Tests Completed and Passed\n");
out:
    close_kmod();
    return 0;
}