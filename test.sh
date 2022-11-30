#! /bin/bash

# default name for the program under test
prog="./bin/demo"

# default name for the directory containing input-data files for test cases
indir="tests/input"

# default name for the directory containing expected-output files for test cases
expdir="tests/expect"

# default name for the directory to store actual output files for test cases
resdir="tests/output"

# default limits on cpu seconds and chars of output to store
cpu=10
climit=100000

#set a count to count how many fail
count=0

# ----- test case processing -----

# run on each test file in the test directory
for tfile in ${indir}/* ; do

    # get the name of the expected output file
    fname=$(basename ${tfile} )

    # determine the names of the expected output and actual results files
    efile="${expdir}/${fname}"
    ofile="${resdir}/${fname}"

    # create a temp file to store the differences between actual and expected output
    dfile=$(mktemp)

    # run one test to check the exit status, don't store the output
    retv=0
    $(ulimit -t ${cpu}; ${prog} < ${tfile} &> /dev/null)
    retv=$?

    # check the return value, case failed if it crashed
    if [ $retv -ne 0 ] ; then
       echo "${tfile} failed: non-zero exit status"
        count=$((count+1));

    else
       # run the actual test, limiting the cpu time and output stored
       (ulimit -t  ${cpu}; (${prog} < ${tfile} 2>&1 | head -c ${climit} > ${ofile}))

       # use diff to check output correctness (exact match or not) and display passed or failed
       if (diff ${ofile} ${efile} 2>&1 > ${dfile}); then
            echo "${tfile} passed" 
            
       else
            echo "${tfile} faile" 
            count=$((count+1))
       fi
    fi

done
echo "Total ${count} cases fail "
