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

**All fixes above included in [debdog-squeeze-mp1_0.0.1_i386.deb](https://github.com/DebianDog/Squeeze/releases/download/v.1.0/debdog-squeeze-mp1_0.0.1_i386.deb)

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

