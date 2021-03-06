Fixes for previous iso version read [here.](https://github.com/DebianDog/Squeeze/blob/921f30c938455cd1f162f439d8208058c34c9927/Bugs-and-Fixes.md)

**Fixes for DebianDog-Squeeze-hybrid-30.04.2016.iso:**

**1.** Change /etc/apt/sources.list to:

```
deb http://archive.debian.org/debian/ squeeze main contrib non-free
deb http://archive.debian.org/debian/ squeeze-lts main contrib non-free
deb http://archive.debian.org/debian/ squeeze-proposed-updates main contrib non-free
deb http://archive.debian.org/debian-security/ squeeze/updates main contrib non-free
deb http://archive.debian.org/backports.org squeeze-backports main contrib non-free
deb http://archive.debian.org/backports.org squeeze-backports-sloppy main contrib non-free
```

**2.** Some scripts changes:
https://github.com/DebianDog/Squeeze/tree/master/scripts

**3.** Remove gdrive-get (doesn't work anymore).

All fixes above included in [debdog-squeeze-mp1_0.0.1_i386.deb](https://github.com/DebianDog/Squeeze/releases/download/v.1.0/debdog-squeeze-mp1_0.0.1_i386.deb)

I don't see any reason to update the iso image yet. It is useful the way it is. Just install the above maintain pack deb. What it does you can read inside the postinst scripts:

```
#!/bin/sh

mv -f -t /etc/apt /opt/temp/sources.list
mv -f -t /usr/local/bin/ /opt/temp/apt2sfs-cli-fullinst
mv -f -t /opt/bin /opt/temp/ffmpeg2sfs
mv -f -t /usr/share/applications /opt/temp/ffmpeg2sfs.desktop
mv -f -t /opt/bin /opt/temp/sfs-get
rmdir /opt/temp

rm -f /usr/share/menu/gdrive-get
rm -f /usr/share/applications/gdrive-get.desktop
rm -f /opt/bin/gdrive-get

if [ -x "`which update-menus 2>/dev/null`" ]; then
	update-menus
fi

```
Probably any new fixes will be provided the same way.

**4.** Remove programs and scripts from [**mcewanw**](http://murga-linux.com/puppy/viewtopic.php?p=960161#960161)

[His wish will be respected as usual.](https://github.com/MintPup/DebianDog-Wheezy/commits/master/scripts)

Remove chpupsocket, make-xhippo-playlist, exec_desktopfile.awk, precord, pavrecord, domycommand, domyfile, ffconvert (maybe I will make alternative mods to make ffconvert work with debiandog but I don't use it and it is available as deb package for other users install anyway), remove xhippo (here I have to replace the broken default audio-video-radio-player links to alternative scripts).

Installing this package [debdog-squeeze-mp1_0.0.1-1_i386.deb](https://github.com/MintPup/DebianDog-Squeeze/releases/download/v.2.1/debdog-squeeze-mp1_0.0.1-1_i386.deb) will remove most scripts and xhippo files replacing the broken default links in /usr/local/bin with alternative scripts.
The postinstall script content:

```
#!/bin/sh

mv -f -t /etc/apt /opt/temp/sources.list
mv -f -t /usr/local/bin/ /opt/temp/apt2sfs-cli-fullinst
mv -f -t /opt/bin /opt/temp/ffmpeg2sfs
mv -f -t /usr/share/applications /opt/temp/ffmpeg2sfs.desktop
mv -f -t /opt/bin /opt/temp/sfs-get

mv -f -t /opt/bin /opt/temp/exec_desktopfile.awk-2
rm -f /opt/bin/exec_desktopfile.awk
ln -sf /opt/bin/exec_desktopfile.awk-2 /opt/bin/exec_desktopfile.awk

mv -f -t /opt/bin /opt/temp/chpupsocket2
rm -f /opt/bin/chpupsocket
ln -sf /opt/bin/chpupsocket2 /opt/bin/chpupsocket

mv -f -t /opt/bin /opt/temp/audio-player
ln -sf /opt/bin/audio-player /usr/local/bin/default_audio-player

mv -f -t /opt/bin /opt/temp/video-player
ln -sf /opt/bin/video-player /usr/local/bin/default_video-player
ln -sf /opt/bin/video-player /usr/local/bin/defaultmediaplayer

mv -f -t /opt/bin /opt/temp/radio-player


mv -f -t /usr/share/mime/packages /opt/temp/freedesktop.org.xml
update-mime-database /usr/share/mime

rmdir /opt/temp

rm -f /usr/share/menu/gdrive-get
rm -f /usr/share/applications/gdrive-get.desktop
rm -f /opt/bin/gdrive-get

xhippo_list () {
echo "/opt/bin/make-xhippo-playlist
/opt/bin/run-ddi
/opt/bin/xhippo
/opt/bin/xhplay
/opt/bin/xhrecord
/opt/bin/xrecord
/usr/share/menu/xhippo
/usr/share/menu/xrecord
/usr/share/applications/xhippo.desktop
/usr/share/applications/xrecord.desktop
/usr/share/pixmaps/xhippo.xpm
/usr/share/pixmaps/mini-record.xpm
/usr/share/pixmaps/minimize.png
/usr/share/pixmaps/next.png
/usr/share/pixmaps/pause.png
/usr/share/pixmaps/play.png
/usr/share/pixmaps/prev.png
/usr/share/pixmaps/refresh.png
/usr/share/pixmaps/shuffle.png
/usr/share/pixmaps/stop.png
/home/puppy/.xhippo
/etc/skel/.xhippo
/root/.xhippo
/root/.xhrecord
/root/.xrecord
/opt/apps/xhippo
" > /tmp/xhippo.txt
}
xhippo_list

rm -fr $(cat /tmp/xhippo.txt)

rm -f /tmp/xhippo.txt

if [ -x "`which update-menus 2>/dev/null`" ]; then
	update-menus
fi

```

The rest I will remove from terminal:

```
apt-get purge precord pavrecord domycommand domyfile ffconvert
apt-get autoremove
```

Reminder to myself - always make deb package for complicated scripts in the future. Saves much time to remove them properly by uninstalling the deb.

**5.** Keep xfe with the old file associations but change them in rox to use the command line scripts from [retro-debian](https://github.com/MintPup/Retro-Debian-Sources/tree/master/scripts) modified for dd-squeeze. Make possible to remove gsu, yad, gtkdialog without breaking DebianDog (it is impossible at the moment). This will keep most community work included with option to use CLI alternatives for most scripts.

**6.** Include puppy-boot initrd.gz as option and some changes in [porteus-boot](https://github.com/MintPup/DebianDog-Wheezy/commits/master/porteus-boot/linuxrc) and from [here](https://github.com/MintPup/Puppy-Linux/commits/master/Debian-kernel/init) and [here.](https://github.com/MintPup/DebianDog-Wheezy/commits/master/puppy-boot/init) The installer scripts will need some changes for puppy-boot. Maybe also the remastering scripts.

**7.** Many changes in live-boot-2 shared a year ago [here](https://github.com/MintPup/DebianDog-Wheezy/tree/master/live-boot-2) and continued [here](https://github.com/MintPup/DebianDog-Squeeze/tree/master/live-boot-2). For example downloading at top of sda1 (ntfs, ext or vfat) the new [initrd1.img](https://github.com/MintPup/DebianDog-Squeeze/releases/download/v.2.1/initrd1.img) and [vmlinuz1](https://github.com/MintPup/DebianDog-Squeeze/releases/download/v.2.1/vmlinuz1) is enough to webboot DebianDog-Squeeze last iso from github using this code (initrd1.img and vmlinuz not inside subfolder example but could be inside live or any name using live-media-path=):

```
title DD-Squeeze fetch=https://github.com/... (example at top of sda1 - no subfolders)
root (hd0,0)
kernel /vmlinuz1 boot=live fetch=https://github.com/DebianDog/Squeeze/releases/download/v.1.0/DebianDog-Squeeze-hybrid-30.04.2016.iso
initrd /initrd1.img
```
Or already downloaded iso from the same partition:

```
title DebianDog fromiso= (example at top of sda1 - no subfolders)
root (hd0,0)
kernel /vmlinuz1 boot=live fromiso=/dev/sda1/DebianDog-Squeeze-hybrid-30.04.2016.iso
initrd /initrd1.img
```

**8.** Live-boot-3 now based on newer version [4.0.2-1](https://github.com/debian-live/live-boot/tree/debian-old-4.0/) with all previous version problems fixed upstream. Included changes shared a year ago [here](https://github.com/MintPup/DebianDog-Wheezy/tree/master/live-boot-3) and continued [here](https://github.com/MintPup/DebianDog-Squeeze/tree/master/live-boot-3). For example downloading at top of sda1 (ntfs, ext or vfat) the new [initrd.img](https://github.com/MintPup/DebianDog-Squeeze/releases/download/v.2.1/initrd.img) and [vmlinuz1](https://github.com/MintPup/DebianDog-Squeeze/releases/download/v.2.1/vmlinuz1) is enough to webboot DebianDog-Squeeze last iso from github.com:

```
title DD-Squeeze fetch=https://github.com/... (example at top of sda1 - no subfolders)
root (hd0,0)
kernel /vmlinuz1 boot=live fetch=https://github.com/DebianDog/Squeeze/releases/download/v.1.0/DebianDog-Squeeze-hybrid-30.04.2016.iso
initrd /initrd.img
```
Or already downloaded iso from the same partition:

```
title DebianDog fromiso= (example at top of sda1 - no subfolders)
root (hd0,0)
kernel /vmlinuz1 boot=live fromiso=/dev/sda1/DebianDog-Squeeze-hybrid-30.04.2016.iso
initrd /initrd.img
```
