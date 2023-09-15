# Dconf Berkeley DB build tools and libraries installation <a id="build-tools-and-libraries-installation"/>
Dconf Berkeley DB may be built for Linux or Windows.
Instructions to build for these platforms follow.

# Table of Contents
- [Windows development tools and libraries installation](#windows-development)
  - [Windows C++ Development Tools](#windows-c++-development-tools)
    - [Visual Studio](#visual-studio-2022-development)
    - [Git](#windows-git-tool)
    - [Cmake](#windows-cmake-tool)
    - [vcpkg](#windows-vcpkg-tool)
  - [Windows C++ Libraries](#windows-c++-libraries)
    - [Berekley DB](#berekley-db-development)
    - [Json-c](#json-c-development)

# Windows development tools and libraries installation <a id="windows-development"/>
- Windows development was done under Windows 10
  - Windows 11 is claimed compatible but is reportedly bloatware

## Windows C++ Development Tools <a id="windows-c++-development-tools"/>
Following are Windows tools used for development. 

### Visual Studio 2022 <a id="visual-studio-2022-development"/>
This is the Windows C++ development environment used.
#### Required Visual Studio components
This section includes Visual Studio components required to build the project.
- Download [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/)
- Select the following Modules:
    - .NET desktop development
    - Desktop development with C++
    - Universal Windows Platform development
- Select the following Individual Component:
    - Class Designer
- Install the following extensions:
    - Select Extensions->Manage Extensions
    - Install these extensions
        - Search for`markdown`and select`Markdown editor (64-bit)`
        - Search for`json`and select`Json formatter`
        - Search for 'doxygen' and select 'Doxygen Comments' by Finn Gegenmantel
    - Restart Visual Studio

### Git <a id="windows-git-tool"/>
Download and run the 64-bit Git for Windows installer
from the [Git download website](https://git-scm.com/download/win).
### Cmake <a id="windows-cmake-tool"/>
Dconf Berkeley DB is written in C++. 
[Cmake](https://cmake.org/) is used for building C++ applications.
The build requires the latest version, currently 3.26.1.
Select the latest Windows version from the`Windows x64 Installer`
link on the [download website](https://cmake.org/download/),
e.g.,`cmake-3.26.3-windows-x86_64.msi`, and run the installer.

### vcpkg <a id="windows-vcpkg-tool"/>
`vcpkg`is useful to install Windows tools without building them from source.
Instructions to install`vcpkg`are:
```
C:>git clone https://github.com/microsoft/vcpkg
C:>vcpkg
C:>bootstrap-vcpkg.bat
```
(Optional) If using with Visual Studio run:
```
C:>vcpkg integrate install
```
## Windows C++ Libraries <a id="windows-c++-libraries"/>
Various C++ libraries are required for the Linux build.
### Berekley DB <a id="windows-berekley-db-development"/>
Berkeley DB is an efficient, embedded non-SQL key-indexed database.
#### Download
The latest Berkeley DB version can be downloaded from:
- https://www.oracle.com/database/technologies/related/berkeleydb-downloads.html
- Select the`Download`link under 'Berkeley DB *version*', e.g.,`Berkeley DB 18.1 (18.1.40)`
  - You may need to create an Oracle account. 
- On the next screen:
  - Check next to 'Oracle Berkeley DB *version*', e.g.,`Oracle Berkeley DB 18.1.401`
  - Select Platform`Linux x86-64`
  - Accept the license agreements
  - Select`Download`
- It will download a Windows executable named like 'Oracle_SSN_DLM_*nnn*.exe'
- Run the executable, it will create a file named, e.g.,`V997917-01.zip`.
- Select`Open Destination`from the window that appears to find the zip.
- Right-click the zip in File Explorer and select`Extract All...`into a directory, e.g.,`V997917-01`.

Let`BDB_HOME`be the Berkeley DB installation directory inside the directory above, e.g.,`db-18.1.40`.

- Linux installation
These are instructions to build [Berkeley DB for UNIX/POSIX](https://docs.oracle.com/cd/E17276_01/html/installation/build_unix.html)
- In Linux, first install the following tools, including`autoconf`and`libtool`:
    - [autoconf](https://askubuntu.com/questions/290194/how-to-install-autoconf)
    - [libtool](https://www.gnu.org/software/make/)
    - [libdb5.3-dev](https://packages.ubuntu.com/bionic/libdb-dev)
    - [libssl-dev](https://packages.ubuntu.com/bionic/libssl-dev)
```
$ sudo apt update
$ sudo apt install autoconf
$ sudo apt install libtool
$ sudo apt install libssl-dev
$ sudo apt install libdb5.3-dev
```
Instructions to build are:
```
$ cd $BDB_HOME/build_unix
$ ../dist/configure <arguments>
$ make
$ sudo make install
```
`configure`takes a few arguments.
They are described at these files:
- `$BDB_HOME/docs/installation/build_unix_conf.html`
- `$BDB_HOME/docs/installation/build_unix_flags.html`

Useful ones include:
- `--prefix=`*directory* determines the final installation directory, default is`/usr/local`
- `--enable-cxx`to build the C++ library
- `--disable-shared`is useful to build with static rather than dynamic libraries
- `--enable-debug` to build the library for debugging
- `CC=`*c compiler* is the C compiler to use, e.g.,`cc`or`gcc`, default is`gcc`
- `CFLAGS=`*flags* are the C compiler flags
- `CXXFLAGS=`*flags* are the C++ compiler flags  

A good set of commands to [build Berkeley DB](https://github.com/SerenadeEXE/BerkeleyDBTutorial)
to install C++ and C static libraries under`/usr/local`are:
```
$ cd $BDB_HOME/build_unix
$ LDFLAGS="-R/usr/lib/x86_64-linux-gnu"
$ ../dist/configure --prefix=/usr --enable-cxx 
$ make
$ sudo make install
```
### Windows installation
These are instructions to build [Berkeley DB for Windows](https://docs.oracle.com/cd/E17076_05/html/installation/build_win.html)
- Choose File -> Open -> Project/Solution.... and select the [file](https://docs.oracle.com/cd/E17076_05/html/installation/build_win.html#win_build32)
`%BDB_HOME%\build_windows\BDB_dotNet_vs2015.sln`
- Choose the desired project configuration from the drop-down menu on the tool bar (either Debug or Release).
- Choose the desired platform configuration from the drop-down menu on the tool bar (usually Win32 or x64).
- To build, right-click on the Berkeley_DB_vs2015 solution and select Build Solution.
- The library files are in`%BDB_HOME%\build_windows\x64\Debug`:
    - libdb181d.dll
    - libdb181d.lib
- The include files are in`%BDB_HOME%`

### Json-c <a id="windows-json-c-development"/>
You may install`jcon-c`from source or load it using`vcpkg`.
[json-c](https://github.com/json-c/json-c) <a id="json-c-development"/>
json-c is a C-based JSON manipulation library.
#### Download
json-c can be downloaded by cloning the git repository:
```
$ git clone git@github.com:json-c/json-c.git
```
Let JSONC_HOME be the json-c installation directory.

#### Building from source
Steps to building on Windows are to . 
- Run these command in the Visual Studio command window using Tools->Command Line->Developer Command Prompt
```
$ cd %JSONC_HOME%
$ mkdir build
$ cd build
$ cmake ..
```
- open the solution`build/json-c.sln`in Visual Studio and build solution
- library files are in`json-c/build/Debug`:
  - json-c-static.lib
  - json-c.lib
- include files are in`json-c`and`json-c/build`
#### Installing`json-c`using`vcpkg`
The command to install`json-c`using`vcpkg`are:
```
C:>vcpkg  install json-c
```

The`examples`directory has some C examples and instructions in`examples/README`.allation <a id="Windows-build-tools-and-libraries"/>
These are instructions to prepare the Windows build environment.
- [json-c](https://github.com/json-c/json-c) <a id="json-c-development"/>
json-c is a C-based JSON manipulation library.
- Download
json-c can be downloaded by cloning the git repository:
```
$ git clone git@github.com:json-c/json-c.git
```
Let JSONC_HOME be the json-c installation directory.
- Installing`json-c`from source
Steps to building on Windows from source are:
- Run these command in the Visual Studio command window using Tools->Command Line->Developer Command Prompt
```
$ cd %JSONC_HOME%_
$ mkdir build
$ cd build
$ cmake ..
```
- open the solution`build/json-c.sln`in Visual Studio and build solution
- library files are in`json-c/build/Debug`:
    - json-c-static.lib
    - json-c.lib
- include files are in`json-c`and`json-c/build`


## Dconf database <a id="dconf-database"/>

- Windows installation
    - Environment variables
        - BDB_HOME   db-18.1.40\build_windows
        - JSONC_HOME json\json-c
    - Visual Studio
        - Additional Include Directories
            - $(BDB_HOME)
            - $(JSONC_HOME)
            - $(JSONC_HOME)\build
        - Additional Library Directories
            - %BDB_HOME%\x64\Debug
            - %JSONC_HOME%\build\Debug
    - Required tools
- Linux installation
    - Environment variables
    - Required tools
- API


#### Windows build
Soft link Berkeley DB and json-c installs to the directory above the Dconf Berkeley home:
```
cd dconf-berkeley-db/..
ln -s ../installs/linux/berkeleydb/db-18.1.40 .
ln -s ../installs/linux/json/json-c .
```
Copy these files from`db-18.1.40\build_windows\x64\Debug`
to`dconf-berkeley-db\src\cxx\bdb_win\db_database_load`:
- `libdb181d.dll`
- `libdb181d.exp`
- `libdb181d.lib`

