# Xournal++ Windows Build

**Windows Build is working now, but Xournal++ is not yet fully supported
on Windows.**

See also [Linux Build](LinuxBuild.md)

Pull requests with fixes to the Code **and to this manual** are welcome!
This manual is not yet completed.


![Screenshot](main-win.png?raw=true "Xournal++ Screenshot on Win10")

## Preparation
Install MSYS2
Install NSIS to the standard Folder.

Start Mingw-w64 64bit. (Always Check **64bit** not 32bit and not MSYS2)

Update MSYS (do this multiple times,
close Terminal after each update)

```bash
pacman -Syuu
```

## Install GIT
```bash
pacman -S git
```

## Install Build tools
```bash
pacman -S mingw-w64-x86_64-cmake \
pacman -S make \
pacman -S mingw-w64-x86_64-toolchain \
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
```
(this is a duplicate of the lines above, probably only this line is needed.
Can anybody confirm this?)
-> press enter multiple times / confirm all default values

```bash
pacman -S mingw-w64-x86_64-gcc
```

## Install dependencies

```bash
pacman -S mingw-w64-x86_64-poppler \
pacman -S mingw-w64-x86_64-gtk3 \
pacman -S mingw-w64-x86_64-libsndfile \
pacman -S mingw-w64-x86_64-libzip
```

### PortAudio
Build/Install portaudio with
```bash
windows-setup/build-portaudio.sh
```

### Lua
Build/Install lua with
```bash
windows-setup/build-lua.sh
```

## Get sources and build

```bash
git clone https://github.com/xournalpp/xournalpp.git
cd xournalpp/
mkdir build
cd build/
```

```bash
cmake ..
make
```

## Packaging and Setup
Create the installer with
```bash
windows-setup/build-setup.sh
```

The installer will be located at `windows-setup/xournalpp-setup.exe`
