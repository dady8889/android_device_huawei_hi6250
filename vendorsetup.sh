if [[ -e vendor/slim ]]; then
    add_lunch_combo slim_hi6250-userdebug
elif [[ -e vendor/omni ]]; then
    add_lunch_combo omni_hi6250-userdebug
elif [[ -e vendor/cm ]]; then
    add_lunch_combo lineage_hi6250-userdebug
elif [[ -e vendor/validus ]]; then
    add_lunch_combo validus_hi6250-userdebug
elif [[ -e vendor/du ]]; then
    add_lunch_combo du_hi6250-userdebug
else
    add_lunch_combo aosp_hi6250-userdebug
fi
