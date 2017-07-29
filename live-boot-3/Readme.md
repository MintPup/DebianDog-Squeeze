Some changes in live-boot-3 for DebianDog-Squeeze.

I will use newer live-boot-(4.0.2-1) this time with the encrypted save problems fixed upstream.

Source scripts from Debian Live team:

https://github.com/debian-live/live-boot/tree/debian-old-4.0/components

More for encrypted save file, ntfs support, webboot and fromiso= included changes read here:

https://github.com/MintPup/DebianDog-Wheezy/tree/master/live-boot-3/

https://github.com/MintPup/DebianDog-Wheezy/blob/master/live-boot-3/bin/Readme-ntfs.txt

Support for persistence save file on NTFS partition needs replacing mount and umount with links to busybox v.1.21.1
and removing /lib/modules/3.2.0-4-486/kernel/fs/ntfs/ntfs.ko
And some changes in /bin/boot/9990-misc-helpers.sh functions. Copy the changed script also in /lib/live/boot/
I don't know why all boot scripts exist in both /bin/boot and /lib/live/boot but this is the way update-initramfs 
command generates the initrd.img

Some more testing shows using the official Squeeze BusyBox v1.17.1 (Debian 1:1.17.1-8+deb6u11) with links to mount and umount also works for ntfs boot and persistence. Instead links using included in the iso /bin/mount from util-linux-ng 2.17.2 (with libblkid and selinux support) and /bin/umount (util-linux-ng 2.17.2) also works.

