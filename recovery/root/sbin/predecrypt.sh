#!/sbin/sh

ln -s /sbin/android.hardware.gatekeeper@1.0-impl.so /vendor/lib64/hw/android.hardware.gatekeeper@1.0-impl.so

setprop vendor.predecrypt Ready
