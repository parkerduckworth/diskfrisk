#!/bin/bash

# source ~/diskfrisk/bin/exec.sh

# Run the application recursively
function f() {
    echo "DISKFRISK INITIATED"
    while true; do
        echo "____________________________________________"
        echo "Enter search, or \"settings\" for more options"
        printf "DISKFRISK$ "
        read search
        if [ "$search" = "q" ]; then
            echo "exiting..."
            return 0
        fi
        ~/diskfrisk/bin/find $search
    done
}

# Runs makefile from any dir, useful for development
function dfmake() {
    echo "Making DISKFRISK..."
    curr=$(pwd)
    cd ~/diskfrisk
    make find
    echo "Make complete."
    cd $curr
}
