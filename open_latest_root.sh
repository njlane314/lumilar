#!/bin/bash
current_dir=$PWD

cd $current_dir/system/data
latest_file=$(ls -t *.root | head -1)
root --web=off $latest_file