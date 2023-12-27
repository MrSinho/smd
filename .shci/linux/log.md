
# Linux build logs

![](linux-exit-code.svg)

```
system        : Linux
version       : #1 SMP Thu Oct 5 21:02:42 UTC 2023
platform      : Linux-5.15.133.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `57.91 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Get:2 http://deb.debian.org/debian bullseye-updates InRelease [44.1 kB]
Get:3 http://security.debian.org/debian-security bullseye-security InRelease [48.4 kB]
Get:4 http://ftp.debian.org/debian bullseye-backports InRelease [49.0 kB]
Get:5 http://deb.debian.org/debian bullseye-updates/main amd64 Packages.diff/Index [25.2 kB]
Get:6 http://deb.debian.org/debian bullseye-updates/main amd64 Packages T-2023-12-26-1404.09-F-2023-12-26-1404.09.pdiff [198 B]
Get:6 http://deb.debian.org/debian bullseye-updates/main amd64 Packages T-2023-12-26-1404.09-F-2023-12-26-1404.09.pdiff [198 B]
Get:7 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages.diff/Index [63.3 kB]
Get:8 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages T-2023-12-25-1404.11-F-2023-12-25-1404.11.pdiff [1,401 B]
Get:8 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages T-2023-12-25-1404.11-F-2023-12-25-1404.11.pdiff [1,401 B]
Fetched 232 kB in 1s (177 kB/s)
Reading package lists...
Building dependency tree...
Reading state information...
39 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
make is already the newest version (4.3-4.1).
libc6-dev is already the newest version (2.31-13+deb11u7).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.

```

---
    
```bash
function BUILD {
	mkdir build
	mkdir build/linux
	cd build/linux

	cmake ../../ -DSMD_BUILD_EXAMPLE=ON
	cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/github/smd/build/linux
Scanning dependencies of target smd
[ 25%] Building C object CMakeFiles/smd.dir/smd/src/smd.c.o
[ 50%] Linking C static library ../../bin/linux/libsmd.a
[ 50%] Built target smd
Scanning dependencies of target smd-example
[ 75%] Building C object CMakeFiles/smd-example.dir/smd-example/src/smd-example.c.o
[100%] Linking C executable ../../bin/linux/smd-example
[100%] Built target smd-example

```

---
    
build ran for `57.91 s` and terminated with exit code `0`

---

