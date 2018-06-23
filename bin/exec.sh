#!/bin/bash

# source ~/diskfrisk/bin/exec.sh

# Run the application
function dfstart() {
    printf "DISKFRISK INITIATED\n"
    while true; do
        printf "________________________________________________\n"
        printf "Enter search, or \"options\" for more information.\n"
        printf ":"
        read search
        if [ "$search" = "q" ]; then
            echo "exiting..."
            return 0
        fi
        ~/diskfrisk/bin/$search
    done
}

# Update changes to source
function dfupdate() {
    printf "Updating DISKFRISK...\n"
    cc ~/diskfrisk/src/main.c ~/diskfrisk/src/diskfrisk.c ~/diskfrisk/src/display.c 
        -o ~/diskfrisk/bin/frisk
    printf "Update complete.\n"
}
