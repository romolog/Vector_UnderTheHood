#! /bin/bash

# if args not empty(empty: -z, not empty: $# -gt 0) - exit failure
if [[ $# -gt 0 ]] ; then
    echo -e "Usage: $0 no args"
    exit 1
fi

### CONFIG VARIABLES ######################################################################

BUILD_="build";
# PROG_="./$BUILD_/webserv";


COMPILER_="clang++";
SET_COMPILER_=""; 

VALGRIND_FLAGS_=" --leak-check=full --show-leak-kinds=all ";
LEAK_CHECK_="./$BUILD_/leak_check";

### UTILITY VARIABLES ######################################################################

BLK_BOLD_="\e[1;30m";
RED_="\033[0;31m";
RED_BOLD_="\033[1;31m";
GRN_="\033[0;92m";
GRN_BOLD_="\e[1;33m";
YEL_="\e[0;93m";
YEL_BOLD_="\e[1;93m";
MAG_="\e[0;35m";
CYN_="\e[0;36m";
WHT_BOLD_="\e[1;37m";

BG_BLK_="\e[40m";
BG_GRY_="\e[100m";
BG_GRN_="\e[42m";

ULN_="\033[4m";

DFT_="\033[0m";

ERR_="$RED_BOLD_\tERROR: $RED_" 
ERR_END_="$DFT_\n";


### BUILD ######################################################################
echo -e "\n$WHT_BOLD_$BG_GRY_\t CMAKE GTEST MyVECTOR $DFT_\n";

((cmake -S . -B $BUILD_  -DCMAKE_CXX_COMPILER=$COMPILER_ && cmake --build $BUILD_ ) || (echo -e "$ERR_ cmake failed $ERR_END_" && false)) || exit 1;
($COMPILER_ -std=c++23 -I. -g -O1 leak_check.cpp -o $LEAK_CHECK_ || (echo -e "$ERR_ compile leak_check failed $ERR_END_" && false)) || exit 1;

### RUN TESTS ######################################################################
ctest --test-dir $BUILD_;
valgrind $VALGRIND_FLAGS_ $LEAK_CHECK_;