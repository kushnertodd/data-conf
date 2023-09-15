# Dconf Berkeley DB build tools and libraries installation <a id="build-tools-and-libraries-installation"/>
Dconf Berkeley DB may be built for Linux or Windows.
Instructions to build for these platforms follow.

# Table of Contents
- [Linux development tools and libraries](#linux-development)
  - [Linux C++ Development Tools](#linux-c++-development-tools)
    - [JetBrains CLion](#jetbrains-clion)
    - [WSL2 Linux distribution for Windows 10](#windows-10-wsl2-linux-distributions)
    - [Windows Terminal](#windows-terminal-tool)
    - [Linux Build Tools](linux-build-tools)
    - [Git](#linux-git-tool)
    - [Cmake](#linux-cmake-tool)
  - [Linux C++ Libraries](#linux-c++-libraries)
    - [pthreads Library](#linux-pthreads-library)
    - [OpenSSL Library](#linux-openssl-library)
    - [json-c Library](#linux-json-c-library)
    - [Berekley DB Library](#linux-berkeley-db-library)
- [Windows development tools and libraries installation](#windows-development)
  - [Windows C++ Development Tools](#windows-c++-development-tools)
    - [Visual Studio](#visual-studio-2022-development)
    - [Git](#windows-git-tool)
    - [Cmake](#windows-cmake-tool)
    - [vcpkg](#windows-vcpkg-tool)
  - [Windows C++ Libraries](#windows-c++-libraries)
    - [Berekley DB](#berekley-db-development)
    - [Json-c](#json-c-development)
- [Google Cloud](#google-cloud)
  - [Google Cloud account](#google-cloud-account)
  - [Google Cloud web server](#google-cloud-web-server)
  - [Google Cloud api server](#google-cloud-api-server)

# Linux development tools and libraries <a id="linux-development"/>
These are instructions to prepare the Linux build environment.

## Linux C++ Development Tools <a id="linux-c++-development-tools"/>
Linux development tools include:
- JetBrains CLion
- WSL2 Linux distribution for Windows 10
- Windows Terminal
- Linux Build Tools
- Git
- Cmake

### JetBrains CLion <a id="jetbrains-clion"/>
[JetBrains CLion](https://www.jetbrains.com/clion/download/#section=windows) is a free C++ IDE.
Dconf Berekeley DB is a Cmake project.
JetBrains has a 
[tutorial](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
on setting up CLion for a Cmake project.

### WSL2 Linux distribution for Windows 10 <a id="windows-10-wsl2-linux-distributions"/>
WSL2 is a native Linux running under Windows 10.
These are instructions for installing [Windows 10 WSL2](https://learn.microsoft.com/en-us/windows/wsl/).
WSL2 has specific [hardware requirements](https://learn.microsoft.com/en-us/windows/wsl/troubleshooting?source=recommendations),
including a 64-bit processor with second-level address translation (SLAT) and at least 4GB of RAM. Any new Intel Windows system should be adequate.
The command to [install](https://learn.microsoft.com/en-us/windows/wsl/install) WSL2 on Windows 10 is:
```
$ wsl --install
```
The install instructions are currently for Pengwin or Ubuntu.
The Linux build is currently tested on Windows 10 WSL2 with [Pengwin](https://www.whitewaterfoundry.com/),

#### Linux Distributions
Linux development was done under Windows 10 WSL2 with [Pengwin](https://www.whitewaterfoundry.com/), 
which may be obtained from the Microsoft Store.
[Ubuntu](https://ubuntu.com/tutorials/install-ubuntu-on-wsl2-on-windows-10#1-overview)
may also be obtained from the Microsoft Store.
The instructions likely work for other types of Linux,
such as CentOS, 
or other types of Unix, 
such as MacOS, possibly with modifications.
These are [instructions](https://learn.microsoft.com/en-us/windows/wsl/use-custom-distro)
for installing other WSL2 Linux distributions.

### Windows Terminal <a id="windows-terminal-tool"/>
You likely would like to use [Windows Terminal](https://aka.ms/terminal)
to develop using WSL2 Linux.
Windows Terminal may be obtained from the Microsoft Store.
This is a Microsoft [tutorial](https://learn.microsoft.com/en-us/windows/terminal/install) for starting with Windows Terminal.
Scott Hanselman has an excellent [video](https://www.youtube.com/watch?v=FC-gLkYWXLw) on using Windows Terminal.

### Linux Build Tools <a id="linux-build-tools"/>
There are several Linux C++ libraries and tools required for the build.
Pengwin and Ubuntu use the`apt`command to install many Linux library and tool packages:
```
$ sudo apt update
$ sudo apt install <package>
```
Always run`apt update`before`install`.
Some packages are not available under`apt`, or some`apt`packages
have versions that are too old to use.
In those cases, they can be directly downloaded from a web site or built from source.

The Linux`build-essential`package installs the C++ compiler, 
debugger, libraries, other development tools required for the build.
`build-essential`includes:
  - [g++, gcc](https://gcc.gnu.org/)
  - [make](https://www.gnu.org/software/make/)  
Using an IDE such as CLion for debugginer, you
likely want to load gdb. 
They are installed with:
```
$ sudo apt-get update
$ sudo apt install build-essential git
$ sudo apt install gdb
```

### Git <a id="linux-git-tool"/>
[git](https://git-scm.com/) is required for the build. 
It can be installed with this command:
```
$ sudo apt-get update
$ sudo apt install git
```
### Cmake <a id="linux-cmake-tool"/>
Dconf Berkeley DB is written in C++. 
[Cmake](https://cmake.org/) is used for building C++ applications.
The build requires the latest version, currently 3.26.1.
The version available from`apt`is too old to use, so it must be built from source.
Select the latest source from the`Unix/Linux Source`link on the [download website](https://cmake.org/download/),
e.g.,`cmake-3.26.3.tar.gz`, 
and [build](https://cmake.org/install/) with these instructions:
```
$ gunzip -c cmake-3.27.0-rc4.tar.gz | tar xvf -
$ cd cmake-3.27.0
$ ./configure --prefix=/usr
$ make
$ sudo make install
```
Cmake requires an additional tool be installed,`pkg-config`:
```
$ sudo apt-get update
$ sudo apt install pkg-config
```
* Developer note: cmake 3.27.0 has a dependency on `libjsoncpp25` and `libssl3` in Ubuntu.
## Linux C++ Libraries <a id="linux-c++-libraries"/>
Various C++ libraries are required for the Linux build.
### pthreads Library <a id="linux-pthreads-library"/>
Linux`pthreads`is a C++ threading library is required for the build.
It should already be installed in Linux. 
In the event it is not, it can be installed with this command:
```
$ sudo apt-get update
$ sudo apt install libpthread-stubs0-dev
```
### OpenSSL Library <a id="linux-openssl-library"/>
Linux [OpenSSL](https://www.openssl.org/) is a secure communications C++ library required for the build.
It can be [installed](https://learnubuntu.com/install-openssl/) these instructions:
```
$ sudo apt-get update
$ sudo apt install openssl
$ sudo apt install libssl-dev
```
### json-c Library <a id="linux-json-c-library"/> 
[json-c](https://github.com/json-c/json-c) is a C-based JSON manipulation library.
The following will download the latest version of`json-c`from the git repository and [build](https://github.com/json-c/json-c/blob/master/README.md)
static libraries and install them in`/usr/local`:
```
$ git clone git@github.com:json-c/json-c.git
```
At this point, manually edit`json-c/json_util.h`to change JSON_FILE_BUF_SIZE from 4096 to 65536.
```
#define JSON_FILE_BUF_SIZE 65536
```
Continue the build:
```
$ mkdir json-c-build
$ cd json-c-build
$ cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr ../json-c 
$ cmake --build .
$ sudo cmake --install . 
```
### Berekley DB Library <a id="linux-berkeley-db-library"/>
The [Berkeley DB](https://www.oracle.com/database/technologies/related/berkeleydb.html) library 
is an efficient, embedded non-SQL key-indexed database.
The latest version is required and can be built from source.
Download source for the latest version from:  
https://www.oracle.com/database/technologies/related/berkeleydb-downloads.html
- Select from the Berkeley DB *version* link. You may need to create an Oracle account. 
- Select the`Linux x86-64 platform,`read accept the license agreement, and select`Download`.
- Run the downloaded Windows executable named Oracle_SSN_DLM_*nnn*.exe to create a file named V*nnnnnn-nn*.zip. 
- Select Open Destination from the opened window to find the zip.
- Right-click/drag the zip using File Explorer and select Expand into a directory named V*nnnnnn-nn*.

[Build](https://docs.oracle.com/cd/E17276_01/html/installation/build_unix.html) using these instructions.
- In Linux, first install`autoconf`and other required development tools:
  - [autoconf](https://askubuntu.com/questions/290194/how-to-install-autoconf)
  - [libtool](https://www.gnu.org/software/make/)
  - [libssl-dev](https://packages.ubuntu.com/bionic/libssl-dev)
  - [libdb5.3-dev](https://packages.ubuntu.com/bionic/libdb-dev)

Install these with:
```
$ sudo apt-get update
$ sudo apt-get install autoconf
$ sudo apt-get install libtool
$ sudo apt install libssl-dev
$ sudo apt install libdb5.3-dev
```
To build, let BDB_HOME be the Berkeley DB installation directory, e.g.,V*nnnnnn-nn*/db-18.1.40:
```
$ cd $BDB_HOME/build_unix
$ ../dist/configure --prefix=/usr --enable-cxx --disable-shared 
$ make
$ sudo make install
```
There are some other`configure` arguments described in these files:  
- `$BDB_HOME/docs/installation/build_unix_conf.html`  
- `$BDB_HOME/docs/installation/build_unix_flags.html` 
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

## Google Cloud <a id="google-cloud"/>
This section describes how to set up the Google Cloud website.
### Google Cloud account <a id="google-cloud-account"/>
- Setting up [Google Workspace](https://support.google.com/a/answer/6365252?hl=en-NZ) (optional)  
You essentially need a Google Workspace account to set up Google Cloud.
It lets you set up a domain name.
It costs from $6 to $18 per user per month.
Business Standard for $12 is a good option.
To set up Google Workspace for one person, see [this](https://support.google.com/a/answer/9212585).
  - Set up Admin Console
  - Set up billing
  - Set up [domain name](https://support.google.com/domains/answer/3453651?hl=en)
  - Set up email
- Setting up [Cloud Identity](https://cloud.google.com/identity/docs/set-up-cloud-identity-admin)
- Setting up a [project](https://console.cloud.google.com/welcome/new?pli=1)
### Google Cloud web server <a id="google-cloud-web-server"/>
The Google Cloud website is a basic [Apache server](https://cloud.google.com/compute/docs/tutorials/basic-webserver-apache).
- Set up a [Compute Engine](https://cloud.google.com/endpoints/docs/openapi/get-started-compute-engine-docker) VM instance.
- Install the [Google Cloud command line](https://cloud.google.com/sdk/docs/install)
- Set up [ssh keys](https://cloud.google.com/sdk/gcloud/reference/compute/config-ssh)
```
$ gcloud computer config-ssh
```
- Open your Google Compute Engine console to see your VMs
- Copy the external IP address
- Select the SSH link next to your VM
- Install Apache:
```
$ sudo apt update && sudo apt -y install apache2
```
  - Create a test home web page:
```
$ echo '<!doctype html><html><body><h1>Hello World!</h1></body></html>' | sudo tee /var/www/html/index.html
```
 
  - Connect to the website and verify that you see a "Hello World!" page.
  - Select Upload File to upload files to the VM
  - Copy the files to the Apache web page directory
```
$ sudo cp [files] /var/www/html 
```
  - Connect to the external IP address to test that the web pages appear:
`http://`*external ip*
- Attach your [domain](![img_1.png](img_1.png)) to the site
## Google Cloud api server <a id="google-cloud-api-server"/>
Similarly:
- Create a VM
- Copy the external IP address
- Statically link the api server
- Create the api server as a docker image
- Connect to the VM with SSH
- Upload the api server executable and files required to build the docker image
- Build the docker image
- Run the docker image with the `-d` (daemonize) option
- Modify the javascript jquery ajax call to use the external IP address
- test the html web page using the javascript
















