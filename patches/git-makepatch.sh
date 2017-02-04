#!/bin/bash
# Copyright (C) Daniel Múčka 2017 (dady8889)
# Create git patch and save it with selected name to desktop
# This will ease patch creation a bit

NC='\033[0m' # No Color
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'

filename=$1
homedir=${HOME}
fullpath="$homedir/Desktop/$filename.patch"

if [ $# -eq 1 ]; then
    echo -e "${YELLOW}Creating patch...${NC}"

    #echo "$filename"
    #echo "$homedir"
    #echo "$fullpath"
    #echo "$patchcommand"

    output=$(git format-patch -1 --stdout 2>&1 > $fullpath) 

    if [[ $output ]]; then
    	echo -e "${RED}$output${NC}"
    	filetext=$(<$fullpath)
    	if [ -z "$filetext" -o "$filetext" == " " -o "$filetext" == "\n" ]; then
    		rm $fullpath
    	fi
    else
		echo -e "${BLUE}Patch created in $fullpath${NC}"
	fi
else
    echo "Usage: git-makepatch filename"
fi