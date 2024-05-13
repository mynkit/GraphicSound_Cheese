#!/bin/bash

# Initialize .jackdrc
echo '/usr/bin/jackd -P75 -p16 -dalsa -dhw:1 -r44100 -p1024 -n3' > ~/.jackdrc

# Get the card number for the device containing "IQaudIODAC"
card_number=$(aplay -l | grep -B 2 "IQaudIODAC" | grep "card" | awk '{print $2}' | tr -d ":")

# Check if card_number is not empty
if [ -n "$card_number" ]; then
    # Update ~/.jackdrc with the new card number
    sed -i "s/-dhw:[0-9]*/-dhw:$card_number/" ~/.jackdrc
    echo "Updated ~/.jackdrc with card $card_number"
else
    echo "Error: Unable to find card number for IQaudIODAC device"
fi

