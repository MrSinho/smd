
# Windows build logs

![](windows-exit-code.svg)

```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `33.67 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (
    choco install -y mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.0.0
Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
Installing the following packages:
mingw;cmake
By installing, you accept licenses for the packages.
mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/2 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

---
    
```bash
> .shci/windows/build-output.txt (
	mkdir build
	cd    build
	mkdir windows
	cd    windows

    cmake ../../ -DSMD_BUILD_EXAMPLE=ON
    cmake --build .
)

```

```bash
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- The C compiler identification is MSVC 19.37.32822.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- SMD_VERSION       : pre-release
-- SMD_ROOT_DIR      : D:/home/desktop/github/smd
-- SMD_BINARIES_DIR  : D:/home/desktop/github/smd/bin
-- SMD_BUILD_EXAMPLE : ON
-- Configuring done (6.1s)
-- Generating done (0.1s)
-- Build files have been written to: D:/home/desktop/github/smd/build/windows
Versione di MSBuild L17.7.2+d6990bcfa per .NET Framework

  Checking Build System
  Building Custom Rule D:/home/desktop/github/smd/CMakeLists.txt
  smd.c
  smd.vcxproj -> D:\home\desktop\github\smd\bin\windows\Debug\smd.lib
  Building Custom Rule D:/home/desktop/github/smd/CMakeLists.txt
  smd-example.c
  smd-example.vcxproj -> D:\home\desktop\github\smd\bin\windows\Debug\smd-example.exe
  Building Custom Rule D:/home/desktop/github/smd/CMakeLists.txt

```

---
    
build ran for `33.67 s` and terminated with exit code `0`

---

