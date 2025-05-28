#!/bin/bash

# !!! DO NOT MOVE THIS FILE !!!
source utils.sh

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

run_test ()
{
    local FINAL_TOTAL=0
    local FINAL_PTS=0

    local zip_file=$(realpath "$1")
    local unzip_dir="unzip_$(date +%s)"

    mkdir -p ${unzip_dir}
    pushd ${unzip_dir} 1>/dev/null
    unzip ${zip_file} 1>/dev/null
    echo "Unzipping to \"${unzip_dir}\""

    if check_system_call; then
        echo -e "[my_syscall]: Passed with ${SYSCALL_PTS} out of ${SYSCALL_TOTAL}"
    else
        echo -e "[my_syscall]: Failed with ${SYSCALL_PTS} out of ${SYSCALL_TOTAL} because:${SYSCALL_ERR}"
    fi

    let FINAL_TOTAL=SYSCALL_TOTAL
    let FINAL_PTS=SYSCALL_PTS
    echo "[Total Score]: ${FINAL_PTS} out of ${FINAL_TOTAL} (this is NOT your final grade)"

    popd 1>/dev/null
    rm -r ${unzip_dir}
}

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

if [ "$#" -ne 1 ]; then
    echo "Zip using this command: zip -r Project-2-ASUID.zip kernel_module/ kernel_syscall/ userspace/ screenshots/"
    echo "Run the script using this command: ./test_my_name.sh /path/to/your/submission.zip"
    exit 1
fi

if [ -e "$1" ]; then
    run_test "$1"
else
    echo "File $1 does not exist"
    exit 1
fi

echo "NOTE: This script does not check the content of your screenshot."
echo "      Check grading rubric for details, the grader will inspect"
echo "      the text within the screenshots."
