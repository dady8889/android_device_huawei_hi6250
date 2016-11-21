#!/bin/bash

THISDIR=$PWD
TOPDIR="$THISDIR/../../../../"
CLEARED=""

clear_set() {
cd $1
for LINE in $(echo $(find -name *.patch); echo $(find -name *.apply))
do
  	REPO=$(dirname $LINE)
	if [[ $(echo $CLEARED | grep -c "$REPO") -gt 0 ]]; then
		continue
	fi 

	cd $TOPDIR
	if [[ ! -e $REPO ]]; then
		echo "WARNING: $REPO does not exist; skipping..."
		CLEARED="$CLEARED|$REPO|"
		cd $THISDIR
		continue
	fi
	echo "Clearing: $REPO"
	cd $REPO
	git add . > /dev/null
	git stash > /dev/null
	find -name *.orig | while read LINE; do rm $LINE; done
	find -name *.rej | while read LINE; do rm $LINE; done
	git clean -f > /dev/null
	git stash clear > /dev/null
	CLEARED="$CLEARED|$REPO|"
	cd $THISDIR
done
}

clear_set pa
clear_set common
