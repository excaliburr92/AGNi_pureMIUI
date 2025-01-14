#!/system/bin/sh
# Please don't hardcode /magisk/modname/... ; instead, please use $MODDIR/...
# This will make your scripts compatible even if Magisk change its mount point in the future
MODDIR=${0%/*}

# This script will be executed in post-fs-data mode
# More info in the main Magisk thread

if ([ ! "` uname -r | grep AGNi`" ] || [ ! -f /sys/module/lpm_levels/parameters/agni_present ]); then
	rm -rf /data/adb/modules/AGNiSupport;
fi;

# Disable some crashing MIUI R services
stop vendor.cnss_diag

