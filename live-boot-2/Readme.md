Some changes in live-boot-2 (2.0.15-1) for DebianDog-Squeeze:

Source scripts from Debian Live team:

https://github.com/debian-live/live-boot/tree/debian-old-2.0

More for encrypted save file, ntfs support, webboot and fromiso= included changes read here:

https://github.com/MintPup/DebianDog-Wheezy/tree/master/live-boot-2

https://github.com/MintPup/DebianDog-Wheezy/blob/master/live-boot-2/bin/Readme-ntfs.txt

Support for persistent live-rw save file on NTFS partition needs replacing mount and umount with links to busybox v.1.21.1
and removing /lib/modules/3.2.0-4-486/kernel/fs/ntfs/ntfs.ko + some changes in /scripts/live-helpers functions posted here in 
DebianDog-Wheeze/live-boot-2/scripts/live-helpers file.

Some more testing shows using the official Squeeze BusyBox v1.17.1 (Debian 1:1.17.1-8+deb6u11) with links to mount and umount also works for ntfs boot and persistent. Instead links using included in the iso /bin/mount from util-linux-ng 2.17.2 (with libblkid and selinux support) and /bin/umount (util-linux-ng 2.17.2) also works.

More changes in both live-boot-2 (2.0.15-1) and later versions for netboot fetch=https://github.com/ support, save sessions on CD or DVD, **toram** to copy only /live content instead whole medium in RAM and more work in progress options will be posted [here](https://github.com/MintPup/DebianDog-Squeeze/blob/master/live-boot-2/Extra-options.md).
