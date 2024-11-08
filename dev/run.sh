#!/bin/bash
# Change to the directory where the script is located
cd "$(dirname "$0")" || exit

./battlesnake play -W 11 -H 11 --timeout 7000 --url http://0.0.0.0:8095 --url https://octiumlabs.com --name thunderous-soilder-dev --name thunderous-soilder

