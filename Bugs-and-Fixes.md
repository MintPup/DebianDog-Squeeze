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

**3.**

