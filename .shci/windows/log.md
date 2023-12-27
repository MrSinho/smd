
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

build ran for `27.94 s` and terminated with exit code `0`

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
    mkdir build/windows
    cd    build/windows

    cmake ../../ -DSMD_BUILD_EXAMPLE=ON
    cmake --build .
)

```

```bash
-- Configuring done (0.1s)
-- Generating done (0.1s)
-- Build files have been written to: D:/home/desktop/github/smd/build/windows
CMake is re-running because D:/home/desktop/github/smd/build/windows/CMakeFiles/generate.stamp is out-of-date.
  the file 'D:/home/desktop/github/smd/build/windows/CMakeFiles/3.26.4/CMakeCCompiler.cmake'
  is newer than 'D:/home/desktop/github/smd/build/windows/CMakeFiles/generate.stamp.depend'
  result='-1'
-- Configuring done (0.1s)
-- Generating done (0.2s)
-- Build files have been written to: D:/home/desktop/github/smd/build/windows
[ 25%] Building C object CMakeFiles/smd.dir/smd/src/smd.c.obj
[ 50%] Linking C static library D:\home\desktop\github\smd\bin\windows\libsmd.a
[ 50%] Built target smd
[ 75%] Building C object CMakeFiles/smd-example.dir/smd-example/src/smd-example.c.obj
[100%] Linking C executable D:\home\desktop\github\smd\bin\windows\smd-example.exe
[100%] Built target smd-example

```

---
    
build ran for `27.94 s` and terminated with exit code `0`

---

