#!/bin/bash

check_file ()
{
    local file_name=$(realpath "$1" 2>/dev/null)

    if [ -e ${file_name} ]; then
        echo "[log]: ─ file ${file_name} found"
        return 0
    else
        return 1
    fi
}

check_dir ()
{
    local dir_name=$(realpath "$1" 2>/dev/null)

    if [ -d ${dir_name} ]; then
        echo "[log]: ─ directory ${dir_name} found"
        return 0
    else
        return 1
    fi
}

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

    # Step 3: Check memory_manager.c - stop if failed
    echo "[log]: Look for source file (memory_manager.c)"
    if ! check_file "source_code/memory_manager.c"; then
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
