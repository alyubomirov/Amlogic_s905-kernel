# Amlogic_s905-kernel

This repo is a clone of 'https://github.com/150balbes/Amlogic_s905-kernel.git'
Branch 'S905X' and commit 66e9aae29c8d30240740d7d69f43eabf922591e0 from 10/25/2016.
I use it with 'https://github.com/150balbes/lib.git' to build Armbian for MXQ-Pro TV box.

Armbian doesn't support checking git commits for it's sources. Only branches and tags.
I built the latest kernel on 105balbes repo but the performance was very poor.
This is the reason for creating this repo.
As you can see, I added aml_nftl_dev.ko, wifi.SDIO drivers and OverlayFS support.
The OverlayFS is using '-t overlayfs' mount option instead of '-t overlay'.  

All credits go to '150balbes'. Thanks and keep up the good work ! 
