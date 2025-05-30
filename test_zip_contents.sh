#!/bin/bash

# !!! DO NOT MOVE THIS FILE !!!
source utils.sh

check_zip_content ()
{
    # Step 1: Check for `source_code` directory - stop if failed
    echo "[log]: Look for directory (source_code)"
    if ! check_file "source_code"; then
        KERNEL_MODULE_ERR="${KERNEL_MODULE_ERR}"
        return 1
    fi

    # Step 2: Check Makefile - stop if failed
    echo "[log]: Look for Makefile"
    if ! check_file "source_code/Makefile"; then
        KERNEL_MODULE_ERR="${KERNEL_MODULE_ERR}"
        return 1
    fi

    # Step 3: Check producer_consumer.c - stop if failed
    echo "[log]: Look for source file (producer_consumer.c)"
    if ! check_file "source_code/producer_consumer.c"; then
        KERNEL_MODULE_ERR="${KERNEL_MODULE_ERR}"
        return 1
    fi

    return 0
}

run_test ()
{
    local zip_file=$(realpath "$1")
    local unzip_dir="unzip_$(date +%s)"

    mkdir -p ${unzip_dir}
    pushd ${unzip_dir} 1>/dev/null

    unzip ${zip_file} 1>/dev/null
    if check_zip_content; then
        echo -e "[test_zip_contents]: Passed"
    else
        echo -e "[test_zip_contents]: Failed. Please read the document carefully."
    fi

    popd 1>/dev/null
    rm -r ${unzip_dir}
}

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

if [ "$#" -ne 1 ]; then
    echo "Usage: ./test_zip_contents.sh </path/to/your/submission.zip>"
    exit 1
fi

if [ -e "$1" ]; then
    run_test "$1"
else
    echo "File $1 does not exist"
    exit 1
fi
