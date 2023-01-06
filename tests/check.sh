#!/bin/sh

red='\e[31;1m'
green='\e[32;1m'
blue='\e[34;1m'
normal='\e[0m'
bold='\e[1m'
bin='zombie'

echo >tests/stderr
echo >tests/stdout

printf "$blue\tBrainFuck Interpreter testsuite$normal\n"
printf "$blue\t===============================$normal\n\n"

total=0
success=0

compare_results() {
    if [ "$?" -ne "$1" ]; then
        text="$bold[ ${red}FAILURE$normal$bold ]$normal"
    else
        diff tests/stdout tests/expected/"$file"
        if [ "$?" -ne 0 ]; then
            text="$bold[ ${red}FAILURE$normal$bold ]$normal"
        else
            text="$bold[ ${green}SUCCESS$normal$bold ]$normal"
            success=$((success + 1))
        fi
    fi
    printf "$text $file.bf\n"
    total=$((total + 1))
}

simple_test() {
    file="$2"
    ./"$bin" tests/"$file".bf 2> tests/stderr 1> tests/stdout

    compare_results "$1"
}

input_test() {
    file="$2"
    input="$3"
    printf "$input" | ./"$bin" tests/"$file".bf 2> tests/stderr 1> tests/stdout

    compare_results "$1"
}

error_test() {
    file="$2"
    if [ "$#" -eq 2 ]; then
        ./"$bin" tests/"$file".bf 2> tests/stderr 1> tests/stdout
    else
        error_test "$1" "$2"
        ./"$bin" tests/"$file".bf "$3" 2> tests/stderr 1> tests/stdout
    fi

    if [ "$?" -ne "$1" ]; then
        text="$bold[ ${red}FAILURE$normal$bold ]$normal"
    else
        expected_output_file=tests/expected/"$file"
        if [ "$#" -gt 2 ]; then
            expected_output_file="${expected_output_file}_with_size"
        fi
        diff tests/stderr "$expected_output_file"
        if [ "$?" -ne 0 ]; then
            text="$bold[ ${red}FAILURE$normal$bold ]$normal"
        else
            text="$bold[ ${green}SUCCESS$normal$bold ]$normal"
            success=$((success + 1))
        fi
    fi

    if [ "$#" -gt 2 ]; then
        printf "$text $file.bf (with array size equal to $3)\n"
    else
        printf "$text $file.bf\n"
    fi

    total=$((total + 1))
}

error_test 1 file_not_found
error_test 2 missing_left
error_test 2 missing_right
error_test 3 incorrect_value_upper
error_test 3 incorrect_value_lower
error_test 3 pointer_out_of_bounds
error_test 3 multiple_errors 4

simple_test 0 hello_world
simple_test 0 comments
simple_test 0 nine

input_test 0 add '!#'
input_test 0 mul "\n\n"

simple_test 0 ZiarZer

echo

percent=$((success * 100 / total))
bars=$((percent * 2 / 10))
if [ "$percent" -eq 100 ]; then
    color="$green"
else
    color="$red"
fi
printf "$bold[$color"
for j in $(seq "$bars"); do
    printf "="
done
for j in $(seq $((20 - bars))); do
    printf ' '
done
printf "$normal$bold]"
printf "$blue Total score: $color$percent%% ($success/$total)$normal\n"
