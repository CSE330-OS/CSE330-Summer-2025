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
 * Description: Similar to test-write-large, but with variable block sizes and iterations.
 */

int main (int argc, char* argv[]) {
    printf("Testcase: write-variable\n");
    if (!open_kmod()) {
        printf("error: could not open kmod chardev.\n"); 
        return -1;
    }
    
    if (argc < 3) {
        printf("Usage: ./test.sh test-write-variable <blocksize> <iterations>\n");
        return -1;
    }

    char* device                    = argv[1];
    char* remaining;
    int   BLOCKSIZE                 = atoi(argv[2]);
    int   ITERATIONS                = atoi(argv[3]);
    unsigned long   OFFSET          = strtol(argv[4], &remaining, 10);
    char* msg_send = malloc(BLOCKSIZE);
    char* msg_recv = malloc(BLOCKSIZE);

    int fd = open("kmod.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        printf("Error: could not open \"dd.txt\" file.\n"); 
        return -1;
    }

    // Write BLOCKSIZE*ITERATIONS worth of (random) data into the USB device using your module
    // Write the random data to the 'kmod.txt' file as well
    unsigned long current_offset = OFFSET;
    for (int i = 0; i < ITERATIONS; i++) {
        char* msg_send = (char*) generate_random_bytes(BLOCKSIZE);
        if (OFFSET == 0) {
            bwrite(msg_send, BLOCKSIZE);
        } else {
            bwriteoffset(msg_send, BLOCKSIZE, current_offset);
            current_offset += BLOCKSIZE;
        }
        write(fd, msg_send, BLOCKSIZE);
    }
    close_kmod();
    close(fd);
    
    // Read the data written into the USB device using the 'dd' command
    // Write this retrieved data into the 'dd.txt' file
    bread_file_range_using_dd(device, OFFSET, (OFFSET+(BLOCKSIZE*ITERATIONS)));

    // Compare the results (block-by-block) to make sure it matches
    if (!compare_kmod_and_dd_files(device, BLOCKSIZE, BLOCKSIZE*ITERATIONS)) {
        printf("[X] FAILED; Output does not match 'dd'\n");
        return -1;
    }

    printf("-------------------------------------------------------------\n");
    printf("Parameters: BLOCKSIZE (%d), ITERATIONS (%d), OFFSET (%ld)\n", BLOCKSIZE, ITERATIONS, OFFSET);
    printf("[*] WRITE Tests Completed and Passed\n");
    return 0;
out:
    close_kmod();
    return -1;
}
