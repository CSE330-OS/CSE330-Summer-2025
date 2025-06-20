#ifndef __COMMON_H__
#define __COMMON_H__

#include "../ioctl-defines.h"

bool open_kmod(void);
void close_kmod(void);

/* kmod_primitives.c */
void bread(char* buffer, unsigned int size);
void bwrite(char* buffer, unsigned int size);
void breadoffset(char* buffer, unsigned int size, unsigned int offset);
void bwriteoffset(char* buffer, unsigned int size, unsigned int offset);

/* dd_primitives.c */
bool bwrite_using_dd(char* device, char* buffer, unsigned long size);
bool bread_using_dd(char* device, char* buffer, unsigned long size);
void bwriteoffset_using_dd(char* device, char* buffer, unsigned long size, unsigned long offset);
void breadoffset_using_dd(char* device, char* buffer, unsigned long size, unsigned long offset);
void bread_file_range_using_dd(char* device, unsigned long start, unsigned long end);
void bwrite_file_range_using_dd(char* device, unsigned long start, unsigned long end);
bool compare_kmod_and_dd_files(char* device, unsigned long blocksize, unsigned long totalsize);

static unsigned char *generate_random_bytes (unsigned int size)
{
    srand ((unsigned int) time (NULL));
    unsigned int i;
    unsigned char *stream = malloc (size);
    for (i = 0; i < size; i++) stream[i] = rand();
    return stream;
}

static bool verify_data (char* sent, char* received, unsigned int size) {
    for (int i = 0; i < size; i++) {
        if (sent[i] != received[i])
            return false;
    }
    return true;
}

#endif