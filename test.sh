#!/bin/bash
COUNTER=0
TEST=1

if [ "$#" -eq  "0" ]; then
    TEST=0
fi;

do_test () {
    let COUNTER++

    if [ $TEST -eq 1 ]; then
        echo -e "$1" | ./main
    fi;

    echo -e "$1" | ./main | grep -q "$2" && echo "Test ${COUNTER} Ok" || (echo "Test ${COUNTER} Failed" && echo -e "$1" | ./main)
}

do_test "1994 \n 2 \n 28 \n m \n 91823 \n 91823 \n 91823" "geschikt voor een exacte studie" 
do_test "1994 \n 2 \n 28 \n m \n 1 \n b" "u bent geschikt voor een beta studie"
do_test "2000 \n 2 \n 29 \n d \n i \n 1 \n b" "je bent geschikt voor een beta studie."
do_test "2023" "Je bent te jong"
do_test "a" "gegeven informatie is niet valide" 
do_test "1994 \n 2 \n 28 \nm \n 1 \n c" "u bent waarschijnlijk niet geschikt voor een opleiding aan de universiteit"
