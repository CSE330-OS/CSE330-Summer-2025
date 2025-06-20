#ifndef __IOCTL_DEFINES_H__
#define __IOCTL_DEFINES_H__

/* Operations to support */
// READ         (data, size);               // reads from the current offset
// WRITE        (data, size);               // writes to the current offset
// READOFFSET   (data, size, offset);       // reads from the specified offset
// WRITEOFFSET  (data, size, offset);       // writes to the specified offset

/* READ/WRITE operation definitions */
struct block_rw_ops {
    char*         data;
    unsigned int  size;
};
#define BREAD    _IOW('a', 'a', struct block_rw_ops)
#define BWRITE   _IOW('a', 'b', struct block_rw_ops)

/* READOFFSET/WRITEOFFSET operation definitions */
struct block_rwoffset_ops {
    char*         data;
    unsigned int  size;
    unsigned int  offset;
};
#define BREADOFFSET    _IOW('a', 'c', struct block_rwoffset_ops)
#define BWRITEOFFSET   _IOW('a', 'd', struct block_rwoffset_ops)

#endif