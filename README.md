## CSE330 Project-1 VM Setup and Kernel Compilation

In this directory, there is a single script available for student testing convenience.

### [test_zip_contents.sh](https://github.com/visa-lab/CSE330-OS/blob/project-1/test_zip_contents.sh)

This script can be used to ensure the final submission adheres to the expected format specified in the project document. It will do the following:

1. Unzip your submission into a directory `unzip_<unix timestamp>/`
2. The script will check for all of the expected files within each directory
3. The script will ensure you did not include any binaries or kernel objects
4. The script will remove the directory it created `unzip_<unix timestamp>/`

Once the script is done running, it will inform you of the correctness of the submission by showing how many expected files it was able to locate.

Usage:
```bash
./test_zip_contents.sh <path to Project-1-ASUID.zip>
```

Expected Output:
```
Checking for files
 - File Found: lsb_release.png
 - File Found: uname.png
Summary --------------------------------------------------------
Got 2 out of 2 expected files
```

## Note: 
- Please do not make any changes in provided test code to pass the test cases.
- You can use print statements in case you want to debug and understand the logic of the test code.
- Please get in touch with the TAs if you face issues in using the test scripts.
