#!/bin/sh

red='\e[31;1m'
green='\e[32;1m'
blue='\e[34;1m'
normal='\e[0m'
bold='\e[1m'
bin='zombie'
FAILURE_text="$bold[ ${red}FAILURE$normal$bold ]$normal"
SUCCESS_text="$bold[ ${green}SUCCESS$normal$bold ]$normal"

print_FAILURE() {
    printf "$FAILURE_text "
}
print_SUCCESS() {
    printf "$SUCCESS_text "
}

echo >tests/stderr
echo >tests/stdout

printf "$blue\tBrainFuck Interpreter testsuite$normal\n"
printf "$blue\t===============================$normal\n\n"

total=0
success=0

compare_output_files() {
    received=$(cat "$1")
    if [ -f "$2" ]; then
        diff "$1" "$2"
        if [ "$?" -ne 0 ]; then
            passed_test=1
        fi
    elif [ -n "$received" ]; then
        passed_test=1
    fi
}

run_test() {
    expected_return="$1"; shift
    file="$1"; shift
    input="$1"; shift
    passed_test=0

    printf "$input" | ./"$bin" "$@" tests/"$file".bf 2> tests/stderr 1> tests/stdout

    if [ "$?" -ne "$expected_return" ]; then
        passed_test=1
    fi

    outputfile="$file"
    params_string=''
    if [ "$#" -gt 0 ]; then
        params_string=$(echo "$@" | sed 's/ //g')
        outputfile="${outputfile}-$params_string"
    fi
    compare_output_files tests/stdout tests/expected/"$outputfile".output
    compare_output_files tests/stderr tests/expected/"$outputfile".error

    if [ "$passed_test" -eq 0 ]; then
        print_SUCCESS
        success=$((success + 1))
    else
        print_FAILURE
    fi
    printf "$file.bf"
    if [ -n "$params_string" ]; then
        printf " ($*)"
    fi
    printf "\n"
    total=$((total + 1))
}

run_test 1 file_not_found
run_test 2 missing_left
run_test 2 missing_right
run_test 3 endless-add
run_test 3 incorrect_value_upper
run_test 3 incorrect_value_lower
run_test 3 pointer_out_of_bounds
run_test 3 multiple_errors
run_test 3 multiple_errors       ''             -a 4
run_test 3 repeat-forever       'out of bounds'

run_test 0 hello_world
run_test 0 trailing-comments
run_test 0 comments
run_test 0 nine
run_test 0 simple-loop

run_test 0 add                   '!#'
run_test 0 mul                   "\n\n"

run_test 0 ZiarZer

percent=$((success * 100 / total))
bars=$((percent * 2 / 10))
if [ "$percent" -eq 100 ]; then
    color="$green"
else
    color="$red"
fi
printf "$bold[$color"
for j in $(seq 20); do
    if [ "$j" -gt "$bars" ]; then
        printf ' '
    else
        printf '='
    fi
done
printf "$normal$bold]"
printf "$blue Total score: $color$percent%% ($success/$total)$normal\n"
