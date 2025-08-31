#Compile the code
# if the file ${cmd} exits then remove it
cmd=hw
if [ -f ${cmd} ]; then
    rm ${cmd}
fi

g++ -std=c++11 *.cpp -o ${cmd}
# if the file ${cmd} does not exit then exit the test
if [ ! -f ${cmd} ]; then
    echo -e "\033[1;91mCompile FAILED.\033[0m"
    exit
fi

# clean folder
for casenum in `seq 1 1 3`; do
    if [ -f ${casenum}.out ]; then
        rm ${casenum}.out
    fi
    if [ -f ${casenum}.stderr ]; then
        rm ${casenum}.stderr
    fi
    if [ -f ${casenum}.stdcout ]; then
        rm ${casenum}.stdcout
    fi
done

# Run the code
for casenum in `seq 1 1 3`; do
	./${cmd} "input=input${casenum}.txt;command=command${casenum}.txt;output=output${casenum}.txt" 1>${casenum}.stdcout 2>${casenum}.stderr
# compare output1.txt with ans1.txt, output the difference to 1.diff
	diff -iBwu ans${casenum}.txt output${casenum}.txt > ${casenum}.diff
# if diff returns nothing, it means you pass the test case (Your output file 1.out is correct)
	if [ $? -ne 0 ]; then
    # display "test case 1 FAILED" to the screen. The word "FAILED" is highlighted in Red color
    		echo -e "Test case ${casenum}    \033[1;91mFAILED.\033[0m"
	else
    # display "test case 1 PASSED" to the screen. The word "PASSED" is highlighted in Green color
    		echo -e "Test case ${casenum}    \033[1;92mPASSED.\033[0m"
    # remove the file 1.diff
    		rm -f ${casenum}.diff
            rm -f ${casenum}.stderr
            rm -f ${casenum}.stdcout
	fi
done
