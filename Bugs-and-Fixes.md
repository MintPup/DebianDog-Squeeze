Fixes for older version **DebianDog-Squeeze-hybrid-29.08.2015.iso**
(All included in **DebianDog-Squeeze-hybrid-30.04.2016.iso**)

**1.** The installer scripts bugs fixing created new problem and the last two versions removed from the repository. 
Recommended to make sure you have this version on your system by reinstalling this package: [debdoginstallscripts_1.0.5_i386-squeeze.deb](http://smokey01.com/saintless/DebianDog-Squeeze/Packages/Included/debdoginstallscripts_1.0.5_i386-squeeze.deb)

Or by running in terminal:
```
sudo apt-get update
sudo apt-get remove debdoginstallscripts
sudo apt-get install debdoginstallscripts
```

**2.** Some fixes from Fred for fixdepinstall (install deb and dependencies right click option).
[More information read here.](http://murga-linux.com/puppy/viewtopic.php?p=871384#871384)
Install fixed version from the link above or from terminal:
```
sudo apt-get update
sudo apt-get install fixdepinstall
```

**3.** Fix for squashfs module loading scripts from Fred. [More information read here.](http://murga-linux.com/puppy/viewtopic.php?p=878996#878996)
```
sudo apt-get update
sudo apt-get install sfsload portablesfs-loadsfs-fuse
```

**4.** Small fix for frisbee. [More information read here.](http://murga-linux.com/puppy/viewtopic.php?p=883158&sid=3588429564754e676ce49df134d930a8#883158)
```
sudo apt-get update
sudo apt-get install frisbee
```

**5.** Small fix for apt2sfs. [More information read here.](http://murga-linux.com/puppy/viewtopic.php?p=885536&sid=e09b92e591e85bcc4632168abdb32e5b#885536)
```
sudo apt-get update
sudo apt-get install apt2sfs
```

**6.** With porteus-boot and only when using save on exit code upgrading libc6 could create some issues. More information and workarownd read [here](http://murga-linux.com/puppy/viewtopic.php?p=889934&sid=00f59036fe7b1df6f8bc7168fe1df597#889934) and the fix [here.](http://murga-linux.com/puppy/viewtopic.php?p=890342&sid=00f59036fe7b1df6f8bc7168fe1df597#890342)
Install this package to fix the problem:
```
sudo apt-get update
sudo apt-get install porteusbootscripts

```

**7.** Debian Squeeze main, contrib and non-free repositories are available only in Debian archive repository now.
Run these commands in terminal to fix the problem:
```
sudo apt-get update
sudo apt-get install debdog-repo-updater
sudo apt-get update

```
Or change the line in /etc/apt/sources.list to archive.debian.org (installing debdog-repo-updater package will do the same change):
```
deb http://archive.debian.org/debian/ squeeze main contrib non-free
#deb http://ftp.debian.org/debian/ squeeze main contrib non-free
```

**8.** The packages **sfs-get-smokey-get** and **debdog-repo-updater** upgraded becuse Debian Squeeze security and backports repositories are available only from Debian archive now. Only squeeze-multimedia repo is active outside the archive but I'm not sure for how long. I will try to mirror squeeze-multimedia i386 repo while it is still available but I need to make some space by removing modules from kazzascorner.com.au and smokey01.com first and make some script changes. In order to do that sfs-get script will download the extra modules from github.com in the future for all DD versions and MintPup.

**It is recommended to install both packages by running these commands to keep sfs-get and dd-repo up to date:**
```
sudo apt-get update
sudo apt-get install debdog-repo-updater sfs-get-smokey-get
sudo apt-get update

```

**9.** One more upgrade for **debdog-repo-updater** (to version 0.0.3). 
The reason is because Squeeze-LTS repo in Debian archive gives expired Release message the last day and installing package from LTS will ask twice for confirmation. [More information read here.](http://murga-linux.com/puppy/viewtopic.php?p=894389&sid=bae7a5267901157c24f9041a65b5542f#894389)
Not a big deal but to skip the warning in the future install the upgraded debdog-repo-updater running the same commands again: 
```
sudo apt-get update
sudo apt-get install debdog-repo-updater
sudo apt-get update

```
It will not change sources.list but /etc/apt/apt.conf will have line to skip this check in the future:
```
Acquire::Check-Valid-Until "false";

```

**10.** **Only for DebianDog-Jwm version** fix for XDM login manager. In case XDM is activated using porteus-boot save on Exit and typing reboot command in terminal (instead using the Reboot-Shutdown menu) does not give option **not to save** changes. [More information read here.](https://github.com/DebianDog/Jessie/issues/2)
To fix this just reisntall XDM (make sure to install debdog-repo-updater first and run apt-get update again - the previous fix):
```
sudo apt-get update
sudo apt-get install --reinstall xdm

```
Or download and install the package [xdm_1.1.11-2_i386.deb](http://smokey01.com/saintless/DebianDog-Squeeze/Packages/Included/xdm_1.1.11-2_i386.deb)

**11.** After some more testing the frugal debdog-install script changes available for upgrade by running in terminal:
```
sudo apt-get update
sudo apt-get install debdoginstallscripts
```
