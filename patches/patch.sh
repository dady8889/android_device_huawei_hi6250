#!/bin/bash
THISDIR=$PWD
ROM=${1}
CLEAR=${2}
TOPDIR="$THISDIR/../../../../"
if [[ "$ROM" == "" ]]; then
	echo "ROM not specified, assuming pa!"
	sleep 1
	ROM="pa"
fi
if [[ "$CLEAR" == "" ]]; then
	./clearpatches.sh $ROM
fi

cd $ROM
for LINE in $(find -name *.patch | sort )
do

	
	PATCH=$THISDIR/$ROM/$LINE
	REPO=$(dirname $LINE)

	echo "Applying patch: $ROM - $LINE"	

	cd $TOPDIR
	if [[ ! -e $REPO ]]; then
		echo "WARNING: $REPO does not exist; skipping..."
		cd $THISDIR
		continue
	fi
	cd $REPO
	RESULT=$(patch -p1 --follow-symlinks --no-backup-if-mismatch < $PATCH)
	#echo -e "${RESULT}"
	if [[ $(echo $RESULT | grep -c FAILED) -gt 0 ]] ; then
		echo ""
		echo "Fail!"
		if [[ $UNATTENDED -eq 1 ]]; then
			exit 9
		else
			read -p "Patch Failed!" yn
			break;
		fi
	fi
	if [[ $(echo $RESULT | grep -c "saving rejects to file") -gt 0 ]] ; then
		echo ""
		echo "Fail!"
		echo "Fix the patch!"
		if [[ $UNATTENDED -eq 1 ]]; then
			exit 9
		else
			read -p "Patch Rejects!" yn
			break;
		fi
	fi
	if [[ $(echo $RESULT | grep -c "Skip this patch") -gt 0 ]] ; then
		echo ""
		echo "Fail!"
		echo "Fix the patch!"
		if [[ $UNATTENDED -eq 1 ]]; then
			exit 9
		else
			read -p "Patch Skipped!" yn
			break;
		fi
	fi
	cd $THISDIR
done

cd $ROM
for LINE in $(find -name *.apply | sort )
do
	PATCH=$THISDIR/$ROM/$LINE
	REPO=$(dirname $LINE)
	echo "Applying patch: $ROM - $LINE"	
	
	cd $TOPDIR
	if [[ ! -e $REPO ]]; then
		echo "WARNING: $REPO does not exist; skipping..."
		cd $THISDIR
		continue
	fi
	cd $REPO
	RESULT=$(git apply --whitespace=nowarn -v $PATCH 2>&1)
	
	if [[ $(echo $RESULT | grep -c error:) -gt 0 ]] ; then
		echo ""
		echo "Fail!"
		echo "Fix the patch!"
		if [[ $UNATTENDED -eq 1 ]]; then
			exit 9
		else
			read -p "Patch Error!" yn
			break;
		fi
	fi
	cd $THISDIR
done
cd $THISDIR
if [[ "$ROM" == "pa" ]];then
	./patch.sh common false
elif [[ "$ROM" == "meticulus" ]]; then
	./patch.sh pa false
fi
