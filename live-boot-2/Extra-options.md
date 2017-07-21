**1.** Save on Exit in save file or directory:

This function in /scripts/live (live-boot-2) and /bin/boot/9990-misk-helpers.sh (live-boot-3 and 4) caught my eye:
```
get_backing_device ()
{
	case "${1}" in
		*.squashfs|*.ext2|*.ext3|*.ext4|*.jffs2)
			echo $(setup_loop "${1}" "loop" "/sys/block/loop*" '0' "${LIVE_MEDIA_ENCRYPTION}" "${2}")
			;;

		*.dir)
			echo "directory"
			;;

		*)
			panic "Unrecognized live filesystem: ${1}"
			;;
	esac
}
```
Seems live-boot gives option to save changes on Exit in save file or in save directory from day one (over 10 years ago) and this option is missing in live-boot documentation.

In short if you have in "live" save file ending with .ext2 .ext3, .ext4 or directory ending with .dir it will be loaded on boot in alphabetical order.
For example with frugal install on ext partition if you have in "live" the main module 01-filesystem.squashfs and directory changes.dir this directory will be loaded like second module on boot. The changes will not be auto saved in changes.dir but you can do that any time before reboot with simple command:
`cp -af /live/cow/* /live/image/live/changes.dir`
And all changes will be saved in /live/image/live/changes.dir

I will add some examples in the boot methods posts and live-boot documentation in the wiki later.

Edit: 2016-07-14: The problem is the above method doesn't support deleted files. Since changes.dir is loaded as second read-only module all files deleted from changes.dir will be marked as .wh files in /live/cow
Changing the paths in snapmergepuppy script to point /live/image/live/changes.dir works to preserve deleted files. Quick mod in directory names and locations for live-boot in snapmergepuppy script (should be easy to make it support live-boot and  porteus-boot in the same script). I will fix this when I have time if I can't find simpler way to update deleted files in changes.dir.

```
#!/bin/bash

#2007 Lesser GPL licence v2 (http://www.fsf.org/licensing/licenses/lgpl.html)
#Barry Kauler www.puppylinux.com
#Edited for 'porteus-boot' on Debiandog for the "save on exit" boot option, by fredx181
#2016-02-26 Change; Line 89 "--remove-destination" instead of "-f", workaround possible crashing when copying files from upgraded libc6
#2016-07-14 saintless - just quick changes in directory location to make it work for live-boot in /live/image/live/changes.dir

export LANG=C #110206 Dougal: I **think** this should not cause problems with filenames

PATH="/bin:/sbin:/usr/bin:/usr/sbin:/usr/X11R7/bin"

SNAP="/live/cow"
cd $SNAP || exit 1

BASE="/live/image/live/changes.dir"

echo "Merging $SNAP onto $BASE..."

SFSPoints=$( ls -d -1 /live/* |sort -u ) #110206 Dougal: get a list of the sfs mountpoints

#Handle Whiteouts...
find . -mount \( -regex '.*/\.wh\.[^/]*' -type f \) | sed -e 's/\.\///;s/\.wh\.//' |
while read N
do
 BN="`basename "$N"`"
 DN="`dirname "$N"`"
 [ "$BN" = ".wh.aufs" ] && continue #w003 aufs has file .wh..wh.aufs in /initrd/pup_rw.
 #[ "$DN" = "." ] && continue
 #110212 unionfs and early aufs: '.wh.__dir_opaque' marks ignore all contents in lower layers...
 if [ "$BN" = "__dir_opaque" ];then #w003
  #'.wh.__dir_opaque' marks ignore all contents in lower layers...
  rm -rf "${BASE}/${DN}" 2>/dev/null #wipe anything in save layer. 110212 delete entire dir.
  mkdir -p "${BASE}/${DN}" #jemimah: files sometimes mysteriously reappear if you don't delete and recreate the directory, aufs bug? 111229 rerwin: need -p, may have to create parent dir.
  #also need to save the whiteout file to block all lower layers (may be readonly)...
  touch "${BASE}/${DN}/.wh.__dir_opaque" 2>/dev/null
  rm -f "$SNAP/$DN/.wh.__dir_opaque" #should force aufs layer "reval".
  continue
 fi
 #110212 recent aufs: .wh.__dir_opaque name changed to .wh..wh..opq ...
 if [ "$BN" = ".wh..opq" ] ; then
  rm -rf "${BASE}/${DN}" 2>/dev/null  #wipe anything in save layer.
  mkdir -p "${BASE}/${DN}" #jemimah: files sometimes mysteriously reappear if you don't delete and recreate the directory, aufs bug? 111229 rerwin: need -p, may have to create parent dir.
  #also need to save the whiteout file to block all lower layers (may be readonly)...
  touch "${BASE}/${DN}/.wh..wh..opq" 2>/dev/null 
  rm -f "$SNAP/$DN/.wh..wh..opq"  #should force aufs layer "reval".
  continue
 fi
 #comes in here with the '.wh.' prefix stripped off, leaving actual filename...
 rm -rf "$BASE/$N"
 #if file exists on a lower layer, have to save the whiteout file...
 #110206 Dougal: speedup and refine the search...
 for P in $SFSPoints
 do
   if [ -e "$P/$N" ] ; then
     [ ! -d "${BASE}/${DN}" ] && mkdir -p "${BASE}/${DN}"
     touch "${BASE}/${DN}/.wh.${BN}"
     break
   fi
 done #110206 End Dougal.
done

#Directories... v409 remove '^var'. w003 remove aufs .wh. dirs.
#w003 /dev/.udev also needs to be screened out... 100820 added var/tmp #110222 shinobar: remove all /dev
find . -mount -type d | busybox tail +2 | sed -e 's/\.\///' | grep -v -E '^mnt|^initrd|^proc|^sys|^tmp|^root/tmp|^\.wh\.|/\.wh\.|^dev/|^run|^var/run/udev|^run/udev|^var/tmp|^etc/blkid-cache' |
#110224 BK revert, leave save of /dev in for now, just take out some subdirs... 110503 added dev/snd
#find . -mount -type d | busybox tail +2 | sed -e 's/\.\///' | grep -v -E '^mnt|^initrd|^proc|^sys|^tmp|^root/tmp|^\.wh\.|/\.wh\.|^dev/\.|^dev/fd|^dev/pts|^dev/shm|^dev/snd|^var/tmp' |
while read N
do

 mkdir -p "$BASE/$N"
 #i think nathan advised this, to handle non-root user:
 chmod "$BASE/$N" --reference="$N"
 OWNER="`stat --format=%U "$N"`"
 chown $OWNER "$BASE/$N"
 GRP="`stat --format=%G "$N"`"
 chgrp $GRP "$BASE/$N"
 touch "$BASE/$N" --reference="$N"
done

#Copy Files... v409 remove '^var'. w003 screen out some /dev files. 100222 shinobar: more exclusions. 100422 added ^root/ftpd. 100429 modify 'trash' exclusion. 100820 added var/tmp #110222 shinobar: remove all /dev
find . -mount -not \( -regex '.*/\.wh\.[^/]*' -type f \) -not -type d |  sed -e 's/\.\///' | grep -v -E '^mnt|^initrd|^proc|^sys|^tmp|^pup_|^zdrv_|^root/tmp|_zdrv_|^dev/|^\.wh\.|^run|^var/run/udev|^run/udev|^root/ftpd|^var/tmp' | grep -v -E -i '\.thumbnails|\.trash|trash/|^etc/blkid-cache|\.part$'  |
#110224 BK: revert, leave save of /dev in for now... 120103 rerwin: add .XLOADED
#find . -mount -not \( -regex '.*/\.wh\.[^/]*' -type f \) -not -type d |  sed -e 's/\.\///' | grep -v -E '^mnt|^initrd|^proc|^sys|^tmp|^run|^pup_|^zdrv_|^root/tmp|_zdrv_|^dev/\.|^dev/fd|^dev/pts|^dev/shm|^\.wh\.|^var/run|^root/ftpd|^var/tmp|\.XLOADED$' | grep -v -E -i '\.thumbnails|\.trash|trash/|\.part$'  |
while read N
do

[ -L "$BASE/$N" ] && rm -f "$BASE/$N"

# Finally, copy files unconditionally.
cp -a --remove-destination "$N" "$BASE/$N"


 BN="`basename "$N"`" #111229 rerwin: bugfix for jemimah code (110212).
 DN="`dirname "$N"`" #111229  "
 [ -e "$BASE/$DN/.wh.${BN}" ] && rm "$BASE/$DN/.wh.${BN}" #110212 jemimah bugfix - I/O errors if you don't do this

done

# Remove files, corresponding with .wh files, from /live/image/live/changes.dir
# Taken from 'cleanup' script included in the official Porteus initrd.xz 
MNAME="/live/image/live/changes.dir"; NAME="basename "$MNAME""
find $MNAME -name ".wh.*" 2>/dev/null | while IFS= read -r NAME; do wh=`echo "$NAME" | sed -e 's^$MNAME^^g' -e 's/.wh.//g'`; test -e "$wh" && rm -rf "$NAME"; done

sync
exit 0

###END###

```
I'm not sure if we need to copy in changes.dir also /live/cow/.wh..wh.aufs, /live/cow/.wh..wh.plnk and /live/cow/.wh..wh.orph but it works without them. These files and folders are auto-generated on boot in /live/cow anyway. I guess we don't need them in changes.dir.
Otherwise it is easy to copy them once only with:
`cp -af /live/cow/.wh* /live/image/live/changes.dir`


The option to load directories is much more important as I thought. Live-boot loads up to 7 squashfs modules on boot inside "live" and it fails to boot if you add more. But this is not the case if you load directories from "live".  Probabaly because directories do not use loop device according to mount command output. Tested to load one squashfs module, one .ext2 module and 13 directories inside "live" and the system boots without problem. Each .dir containes inside empty text file with the directory number and all are loaded after boot:

```
root@debian:~# ls -l /
total 28
-rw-r--r--  1 root root     0 Jul 16 14:12 1.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 10.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 11.txt
-rw-r--r--  1 root root     0 Jul 15 18:41 111.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 12.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 2.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 3.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 4.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 5.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 6.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 7.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 8.txt
-rw-r--r--  1 root root     0 Jul 16 14:12 9.txt
-rw-r--r--  1 root root     0 Jul 13 09:38 Dir-load-OK.txt
drwxr-xr-x  2 root root  4096 Jul 14 10:02 bin
drwxr-xr-x  2 root root    41 May 14  2014 boot
drwxr-xr-x 14 root root  2980 Jul 16 14:17 dev
drwxr-xr-x 75 root root    60 Jul 14 11:06 etc
drwxr-xr-x  3 root root    28 Mar 19  2014 home
drwxr-xr-x 17 root root  4096 Jun  5 22:01 lib
drwxrwxrwt 19 root root   380 Jul 16 14:17 live
drwxr-xr-x  2 root root     3 Mar 17  2014 live-rw-backing
-rw-r--r--  1 root root     0 Jul 16 14:16 live-sn.ext2.txt
drwxr-xr-x  3 root root  4096 Jul 14 09:06 media
drwxr-xr-x  2 root root     3 Jul 11 16:36 mnt
dr-xr-xr-x 70 root root     0 Jul 16 14:17 proc
drwx------ 17 root root    80 Jul 16 14:17 root
drwxr-xr-x 10 root root   280 Jul 16 14:17 run
drwxr-xr-x  2 root root  2548 Jun 16 09:59 sbin
drwxr-sr-x  4 root staff  974 Jul 11 17:43 scripts
drwxr-xr-x  2 root root     3 Jan 26  2014 selinux
drwxr-xr-x  2 root root     3 Jan 26  2014 srv
drwxr-xr-x 12 root root     0 Jul 16 14:17 sys
drwxrwxrwt  4 root root   120 Jul 16 14:18 tmp
drwxr-xr-x 13 root root  4096 May 28 08:50 usr
drwxr-xr-x 16 root root    80 Jan 23  2015 var
```
```
root@debian:~# ls -l /live
total 5
drwxr-xr-x 23 root root  346 Jul 11 16:42 01-filesystem.squashfs
drwxr-xr-x  2 root root   40 Jul 16 14:17 a1.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a10.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a11.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a12.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a2.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a3.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a4.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a5.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a6.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a7.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a8.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 a9.dir
drwxr-xr-x  2 root root   40 Jul 16 14:17 changes.dir
drwxr-xr-x  8 root root  180 Jul 16 14:17 cow
drwxr-xr-x 34 root root 4096 Jul 15 09:28 image
drwxr-xr-x  2 root root 1024 Jul 16 14:16 live-sn.ext2
```
```

root@debian:~# mount
sysfs on /sys type sysfs (rw,nosuid,nodev,noexec,relatime)
proc on /proc type proc (rw,nosuid,nodev,noexec,relatime)
udev on /dev type devtmpfs (rw,relatime,size=10240k,nr_inodes=30177,mode=755)
devpts on /dev/pts type devpts (rw,nosuid,noexec,relatime,gid=5,mode=620,ptmxmode=000)
tmpfs on /run type tmpfs (rw,nosuid,noexec,relatime,size=25372k,mode=755)
/dev/sda1 on /live/image type ext3 (rw,noatime,errors=continue,barrier=1,data=ordered)
/dev/loop0 on /live/01-filesystem.squashfs type squashfs (ro,noatime)
/dev/loop1 on /live/live-sn.ext2 type ext2 (ro,noatime)
tmpfs on /live/cow type tmpfs (rw,noatime,mode=755)
aufs on / type aufs (rw,relatime,si=39b7f5f5,noxino)
tmpfs on /live type tmpfs (rw,relatime)
tmpfs on /run/lock type tmpfs (rw,nosuid,nodev,noexec,relatime,size=5120k)
tmpfs on /run/shm type tmpfs (rw,nosuid,nodev,noexec,relatime,size=50720k)
tmpfs on /tmp type tmpfs (rw,nosuid,nodev,relatime)
```

Live-boot has many persistent options missing from the official documentation. Multiple directories loading on boot could make possible saving sessions on CD/DVD like in Puppy linux.  This should be possible also with Ubuntu based live systems using casper-boot. Casper also loads directory content on boot from limited testing.


**2.** Save file fsck:

In live-boot-2 /scripts/live we have one more option missing from the official documentation:
```
	forcepersistentfsck)
FORCEPERSISTENTFSCK="Yes"
export FORCEPERSISTENTFSCK
```
Save file fsck works after including e2fsck and the libs for it in initrd1.img
The option is active after adding **forcepersistentfsck** to the boot code.
But this option is missing in live-boot-3 and later.

**3.** In live-boot-3 and 4 Copy on Write option booting with no-persistence boot code is located in /lib/live/mount/overlay but it seems empty at first. For DebianDog I made a script called [cow-nosave](http://murga-linux.com/puppy/viewtopic.php?p=798823#798823) over two years ago. It is included in all Jwm versions (in the last iso in /opt/bin/special/old) and in [new-kernel-scripts.tar.gz](https://9eb8f45ca0acc9dd68fbe8a604cd7299aa432000.googledrive.com/host/0B8P7qC27sushWHg2VFB6QTRJLW8/DebianDog-Wheezy/Old-Versions/Packages/).

```
#!/bin/bash

/opt/bin/remount-rw
rm -fr /live/cow
rm -fr /live/image
umount /lib/live/mount/overlay
ln -s /lib/live/mount/overlay /live/cow
ln -s  /lib/live/mount/medium /live/image
```

The important command is umount /lib/live/mount/overlay to make the files visible. Later improved version /opt/bin/cowsave from Fred replaced cow-nosave, cow-save-file and cow-save-part scripts.

**4.** The perfect full install.

I've described this method using save file or save partition in [How to reduce the size of Debian Live image 
](http://www.murga-linux.com/puppy/viewtopic.php?p=741783&sid=8dddf480959eaeee388f8c9dfc40d390#741783) and [here.](http://murga-linux.com/puppy/viewtopic.php?p=771639#771639) But now the option to save changes on exit in directory with live-boot makes possible to have perfect full install.

In short using live-boot (2 or 3 or 4) **without** persistent boot code (only save file or save partition needs persistent boot code) and having in "live" **empty** 01-filesystem.squashfs and the system (the content of 01-filesystem.squashfs)  extracted in changes.dir folder you will boot in frugal mode with read-only system in changes.dir. Any changes you make will be lost after reboot if you dont save on exit. But when you save changes in changes.dir there will be no duplicated files in the empty 01-filesystem.squashfs and changes.dir and there is no need to remaster the system anymore. The remastering or backup process could be simple archive of changes.dir folder (after some cleaning) portable to install (extract) on different drive, usbflash or sdcard. And the system boots uncomressed changes.dir content much faster compared to squashfs module. You get all advantages of full install in frugal mode keeping the option to save or not your changes and to load extra squashfs modules.
There is no problem to combine this boot method with real save file or partition adding persistent (persistence) boot code in case you have low RAM machine. The difference is all changes will be preserved in save file but you can remove this file or boot without persistent code any time after saving the changes you need in changes.dir.

Live-boot gives unique persistent options. [Again my thanks to Daniel Baumann!](https://lists.debian.org/debian-live/2015/11/msg00024.html)

This type of boot solves also the kernel upgrades. Since all system files are in changes.dir and can be updated/replaced all we need to do is moving initrd1.img or initrd.img and vmlinuz1 in /live/changes.dir/boot with the original names (vmlinuz-3.2.0-4-486 and initrd.img-3.2.0-4-486). Then the boot code example will be:

```
title DebianDog perfect full install on sda1 (ext3) inside "live"
root=(hd0,0) 
kernel /live/changes.dir/boot/vmlinuz-3.2.0-4-486 boot=live showmounts
initrd /live/changes.dir/boot/initrd.img-3.2.0-4-486
```
Now we can remove the kernel pinning in /live/changes.dir/etc/apt/preferences and reboot. The kernel can be upgraded without duplicating files using save on exit after the upgrade. The initrd.img and vmlinuz in /live/changes.dir/boot will be replaced with updated versions after the upgrade.

This works well with live-boot-3,4 without persistence save file. Unfortunately adding persistence save file (in case you don't have much RAM) fails the boot process (works fine with live-boot-2). I suspect the problem is related with the wrong modules loading in live-boot-3,4 using persistence with more than one squashfs module (from Z to A instead from A to Z). Maybe I will try to fix this for live-boot-3,4 but I feel it has many disadvantages compared to live-boot-2, especially when live-boot-2 supports now [encrypted save file](https://github.com/MintPup/DebianDog-Wheezy/commit/c124d939f31415e2ef3c641c36ddd55e42431695#diff-5ecdb4a03b89fc9471978e363fef3f1b) and [persistent save file on NTFS boot partition](https://github.com/MintPup/DebianDog-Wheezy/commit/a9d2875fffda59f81940629fb9825b93a599779e#diff-5ecdb4a03b89fc9471978e363fef3f1b).
Live-boot-2 doesn't have any of the problems in later versions and I think it is the most flexible boot and persistent method. I feel patched live-boot-2 deb package with ntfs and encrypted save support is the best option to boot the system with new generated initrd.img after installing the deb. Probabaly universal solution to add flexible persistent options in official Debian/Ubuntu live systems.
