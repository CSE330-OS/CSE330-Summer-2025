# CSE330 Project-4 Memory Manager

In this directory, there are two scripts available for students testing convenience.

## test_module.py

Before using this script, you must compile a C source, `testp5.c` file into a binary, `testp5`. This binary
takes a scalar value to denote how much memory to allocate. The `testp5` program will allocate a large chunk
of memory on the heap and then access each page to bring it present into memory. Since the memory allocation
is large, some of the pages accessed will be moved to disk. This binary is invoked by `test_module.py` so you
do not need to run it yourself but you are welcome to do so.
- Note, you ***MUST*** configure your virtual machine to use 2GB-4GB. For some test cases, the value of the `scalar`
  argument will change based on how much RAM your virtual machine has.
     - When you are testing virtual addresses mapped to physical addresses belonging to pages in *swap*, the value
       for `scalar` scales with your RAM. If you are using 2GB, use 2. If you are using 3GB, use 3. And so on.
     - For all other test cases, set the value of `scalar` to 1.
  
  This is to ensure the script will be able to generate swap pages when needed so it can choose virtual addresses belonging to
  swapped pages to test your kernel module.

You can compile `testp5` using the provided `Makefile` by simply running the following command:
```bash
make
```

This script can be used to test the kernel module. It will do the following when provided the path to your
compiled kernel module (i.e., a .ko file kernel object), a scalar value to denote how much memory to allocate,
and the number of present, swapped, and invalid pages to test:
  1. If the script is going to test virtual addresses mapped to physical addresses in swap, then the script will make sure swap is enabled. Otherwise it will temporarily disable swap.
  2. The script will start the `testp5` program.
  3. The script will wait to give the OS some time to either bring pages present into memory or move pages to swap. The script will start by waiting for five seconds. If it still has not found the necessary pages to test your code, it will wait another five seconds. This repeats until the necessary pages exist to test your code.
  4. We will test a number of random virtual addresses based on the arguments provided. It is reccomended to test addresses present in memory, present in swap, and invalid addresses seperately.

Your kernel module will be loaded and unloaded for each address tested. If for any reason the kernel module either
fails to load or unload, the script will stop testing and you will be left only with the total points you have
accumulated so far.

To troubleshoot:

- If you are correctly providing all the commandline arguments and are still finding that the script cannot find any **pages present in main memory**, the `testp5` process may be getting killed by the OS due to it's high memory usage. In this scenario, you should increase the amount of memory for your VM.

- If you are correctly providing all the commandline arguments and are still finding that the script cannot find any **pages present in swap**, make sure your machine is configured to support swap and make sure your usage of the `scalar` argument is correct. If you do not have enough swap, the OS can still kill the `testp5` process if there is not enough.

- Since this script reads `/var/log/kern.log` and multiple files from procfs to validate your output, it ***MUST*** be run with `sudo`.

### Usage and expected output:

Usage: Replace `/path/to/memory_manager.ko` with the path to your compiled kernel module:
```bash
Usage: sudo ./test_module.py /path/to/memory_manager.ko <scalar> <present> <swapped> <invalid>
 - scalar  : This should change based on RAM size and test case.
 - present : The number of present addresses to test.
 - swapped : The number of swapped addresses to test.
 - invalid : The number of invalid addresses to test.
This script MUST be run as root and you MUST have compiled your kernel module before running.
```

Expected output for test case 1:
```
[log]: Disable swap
[log]: Waiting for 5 seconds to allow pages to be present and/or to be moved to swap
[log]: Checking 100 random present pages
[log]: - 100/100 correct
[log]: Enabling swap
[memory_manager]: Passed (100.0/100)
```
Expected output for test case 2:
```
[log]: Enabling swap
[log]: Waiting for 5 seconds to allow pages to be present and/or to be moved to swap
[log]: Checking 100 random swapped pages
[log]: - 100/100 correct
[memory_manager]: Passed (100.0/100)
```
Expected output for test case 3:
```
[log]: Disable swap
[log]: Waiting for 5 seconds to allow pages to be present and/or to be moved to swap
[log]: Checking 100 random invalid pages
[log]: - 100/100 correct
[log]: Enabling swap
[memory_manager]: Passed (100.0/100)
```

## test_zip_contents.sh

This script is to be used to ensure the final submission adheres to the expected format specified in the project codument. It will do the following:

1. Unzip your submission into a directory `unzip_<unix_timestamp>/`
2. The script will check for all of the expected files within the `source_code` directory
3. The script will remove the directory it created `unzip_<unix_timestamp>`

Once the script is done running, it will inform you of the correctness of the submission by showing you anything it could not find.

Usage:
```bash
./test_zip_contents.sh /path/to/zip/file
```

Expected output:
```
[log]: Look for directory (source_code)
[log]: ─ file /home/vboxuser/git/GTA-CSE330-Fall2024/Project5/test/unzip_1730664481/source_code found
[log]: Look for Makefile
[log]: ─ file /home/vboxuser/git/GTA-CSE330-Fall2024/Project5/test/unzip_1730664481/source_code/Makefile found
[log]: Look for source file (memory_manager.c)
[log]: ─ file /home/vboxuser/git/GTA-CSE330-Fall2024/Project5/test/unzip_1730664481/source_code/memory_manager.c found
[test_zip_contents]: Passed
```
