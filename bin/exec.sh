#!/bin/bash

# source ~/diskfrisk/bin/exec.sh

# Run the application recursively
function f() {
    echo "DISKFRISK INITIATED"
    while true; do
        echo "____________________________________________"
        echo "Enter search, or \"settings\" for more options"
        printf "DISKFRISK$ "
        read input
        if [ "$input" = "q" ]
        then
            echo "exiting..."
            return 0
        elif [ "$input" = "settings" ]
        then
            ~/diskfrisk/bin/settings
        else
            ~/diskfrisk/bin/find $input
        fi
    done
}

# Runs makefile from any dir, useful for development
function dfmake() {
    echo "Making DISKFRISK..."
    curr=$(pwd)
    cd ~/diskfrisk
    make
    echo "Make finished."
    cd $curr
}
