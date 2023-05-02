#!/bin/bash

current_dir=$PWD

cd $current_dir/system
latest_file=$(ls -t *.root | head -1)

read -p "Enter a new name for the latest root file: " new_name
mv "$latest_file" "$new_name"

echo "The latest root file has been renamed to: $new_name"