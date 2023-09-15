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
- [Google Cloud](#google-cloud)
  - [Google Cloud account](#google-cloud-account)
  - [Google Cloud web server](#google-cloud-web-server)
  - [Google Cloud api server](#google-cloud-api-server)

# Linux development tools and libraries <a id="linux-development"/>
These are instructions to prepare the Linux build environment.

## Linux development tools for Ubuntu 
To develop under native Ubuntu, 
- Install latest Ubuntu version ISO from the [Ubuntu download](https://ubuntu.com/download/desktop) page
- Install [Rufus](https://rufus.ie/en/,install) or other tool to burn the ISO to a USB formatted as FAT32
- Burn ISO to a USB and boot the new system from the USB
- Select Install Ubuntu
- Accept system updates

## Linux C++ Development Tools for Windows 10 <a id="linux-c++-development-tools"/>
To develop under Windows 10, linux development tools for Windows 10 include:
- JetBrains CLion
- WSL2 Linux distribution for Windows 10
- Windows Terminal

### JetBrains CLion <a id="jetbrains-clion"/>
[JetBrains CLion](https://www.jetbrains.com/clion/download/#section=windows) is a free C++ IDE.
Dconf Berekeley DB is a Cmake project.
JetBrains has a 
[tutorial](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
on setting up CLion for a Cmake project.

### WSL2 Linux distribution<a id="windows-10-wsl2-linux-distributions"/>
WSL2 is a native Linux running under Windows 10.
These are instructions for installing [Windows 10 WSL2](https://learn.microsoft.com/en-us/windows/wsl/).
WSL2 has specific [hardware requirements](https://learn.microsoft.com/en-us/windows/wsl/troubleshooting?source=recommendations),
including a 64-bit processor with second-level address translation (SLAT) and at least 4GB of RAM. Any new Intel Windows system should be adequate.
The command to [install](https://learn.microsoft.com/en-us/windows/wsl/install) 
WSL2 on Windows 10 with the Ubuntu distribution is:
```bash
$ wsl --install -d Ubuntu
```
To enable changing file owners & permissions, you need to add configuration options to`/etc/wsl.conf`:
```
$ sudo vi /etc/wsl.conf
[automount]
options = "metadata"
```
Add these lines to the end:
```
[automount]
options = "metadata"
```
#### Linux Distributions for WSL2
Linux development was done under Windows 10 WSL2 with 
[Ubuntu](https://ubuntu.com/tutorials/install-ubuntu-on-wsl2-on-windows-10#1-overview)
and
[Pengwin](https://www.whitewaterfoundry.com/), 
both of which may be obtained from the Microsoft Store.
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

## Linux Build Tools <a id="linux-build-tools"/>
There are several Linux C++ libraries and tools required for the build.
There are some installation differences between native Ubuntu
and Windows 10 WSL2 Linux. They will be noted below.


Pengwin and Ubuntu use the`apt`command to install many Linux library and tool packages:
```bash
$ sudo apt update
$ sudo apt install <package>
```
Always run`apt update`before`install`.
Some packages are not available under`apt`, or some`apt`packages
have versions that are too old to use.
In those cases, they can be directly downloaded from a web site or built from source,
as indicated below.

### Linux compilers and utilities
The Linux`build-essential`package installs the C++ compiler, 
debugger, libraries, other development tools required for the build.
`build-essential`includes:
  - [g++, gcc](https://gcc.gnu.org/)
  - [make](https://www.gnu.org/software/make/)  

Other utilities to install are:
- [git](https://git-scm.com/)
- [OpenSSL](https://www.openssl.org/)
- doxygen
- curl
- unzip
- vim  

They are installed with:
```bash
$ sudo apt-get update
$ sudo apt install build-essential 
$ sudo apt install git 
$ sudo apt install openssl
$ sudo apt install libssl-dev
$ sudo apt install doxygen
$ sudo apt install curl
$ sudo apt install unzip
$ sudo apt install vim
```

### Cmake <a id="linux-cmake-tool"/>
Dconf Berkeley DB is written in C++. 
[Cmake](https://cmake.org/) is used for building C++ applications.
The build requires the latest version, currently 3.27.4.
#### Ubuntu
The latest cmake version can be installed with:
```bash
$ sudo snap install --classic cmake  
```
#### WSL
On WSL2, the version available from`apt`is too old to use, 
so it must be built from source.
Select the latest source from the`Unix/Linux Source`link on the [cmake download website](https://cmake.org/download/),
e.g.,`cmake-3.26.3.tar.gz`, 
and [build](https://cmake.org/install/) with these instructions:
```bash
$ gunzip -c cmake-3.27.0-rc4.tar.gz | tar xvf -
$ cd cmake-3.27.0
$ ./configure --prefix=/usr
$ make
$ sudo make install
```
## Linux C++ Libraries <a id="linux-c++-libraries"/>
Various C++ libraries are required for the Linux build.

### `json-c` Library <a id="linux-json-c-library"/> 
[json-c](https://github.com/json-c/json-c) is a C-based JSON manipulation library.
The following will download the latest version of`json-c`from the git repository and [build](https://github.com/json-c/json-c/blob/master/README.md)
static libraries and install them in`/usr/local`:
```bash
$ git clone git@github.com:json-c/json-c.git
$ git clone https://github.com/json-c/json-c.git

```
At this point, manually edit`json-c/json_util.h`to change JSON_FILE_BUF_SIZE from 4096 to 65536.
```bash
#define JSON_FILE_BUF_SIZE 65536
```
Continue the build:
```bash
$ mkdir json-c-build]
$ cd json-c-build
$ cmake -DBUILD_SHARED_LIBS=OFF ../json-c 
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
$ unzip V997917-01.zip

[Build](https://docs.oracle.com/cd/E17276_01/html/installation/build_unix.html) using these instructions.

To build, let BDB_HOME be the Berkeley DB installation directory, e.g.,V*nnnnnn-nn*/db-18.1.40:
```bash
$ unzip V997917-01.zip
$ cd db-18.1.40/build_unix
$ ../dist/configure --prefix=/usr --enable-cxx --disable-shared 
$ make
$ sudo make install
```

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

raw instructions:
```shell
2023-09-08
Deploying dconf-berkeley=db to Google Cloud

There are two instances:

- tkllc-lucy-game-design-instance-1 
Apache web server for web files
- app-bdb-inet-lookup-instance-1
Docker container for api server

First step:
- open page https://console.cloud.google.com/compute/instances?project=tkllc-lucy-game-design
- start instances tkllc-lucy-game-design-instance-1 and app-bdb-inet-lookup-instance-1 if not already started

For web files or if src/web files change:

- on tkllc-lucy-game-design-instance-1:
  - open ssh
  - remove files from home directory:
    $ rm *.js *.css *.html
  - upload:
    *.js
    *.css
    *.html
  - modify request.js to select the request_url to 34.106.93.238:8000 instead of localhost

->  static request_url = "http://34.106.93.238:8000";
    //static request_url = "http://localhost:8000";
    
  - run:
    $ sudo cp *.js *.css *.html /var/www/html
  - to be sure they copied okay, in /var/www/html run:
    $ for f in *.js *.css *.html; do echo $f; diff $f ~/$f; done
    (ensure the diff is clean)

For api server or if any of these files change:

- src/apps/dconf_app.cpp
- src/bdb-lib
- src/docker
- src/dconf-lib
- src/util-lib

- locally:
  - build dconf_app clean
  - in src/scripts
    + in one window run:
      $ sh dconf_app.sh
      (make sure there are no connect errors, else see section at the end)
    + in another window run:
      $ sh rm-db-samples.sh
      $ sh ls-db-samples.sh
      (ensure src/data/db is clean)
      $ sh check.sh
      (ensure all requests succeed)
      $ sh dump_db_samples.sh
      $ diff dump_sample.txt dump_sample_init.txt
      (make sure diff is clean)
  - stop dconf_app.sh
  - in src/docker run:
    (start docker for windows if using)
    $ sh stop_images.sh
    $ sh remove_images.sh
    $ docker ps
    $ docker images
    (ensure images are gone)
  + note that the docker uses the data files in data/
    $ rm *db dconf_app
    $ sh build.sh
    $ sh run_api.sh
    (see container-id)
    $ docker logs --follow <container-id>
    (ensure you see 'server socket opened (3)')
    (open src/weblocalhost/dconf-home.html web page in chrome, select a user and order to make sure is working)
    (run '$ sh stop_images.sh' is passes)
    
- on app-bdb-inet-lookup-instance-1:
 + open ssh
 + delete files in home directory:
   $ rm *db *json *sh dconf_app Dockerfile
 + if you want fresh db files, upload files from src/docker to instance:
   account.db
   account_email.sdb
   account_username.sdb
   account_username_triplets.db
   bdb_databases.json
   build_api.sh
   item.db
   item_name.sdb
   item_name_triplets.db
   item_cost.sdb
   order_account_id.sdb
   order_item_item_id.sdb
   order_item.db
   order_item_order_id.sdb
   order.db
   order_name.sdb
   order_name_triplets.db
   Dockerfile
   dconf_app
   dconf_app.sh
   remove_images.sh
   run_api.sh
   stop_images.sh
   
 + in the home directory run:
  = first time:
    $ mkdir data
    $ cp *db data
  = if you want to start with fresh data files:
    $ cp *db data
  $ sh stop_images.sh
  $ sh remove_images.sh
  $ docker ps
  $ docker images
  (ensure images are gone)
  $ sh build_api.sh
  $ sh run_api.sh

You should be able to connect from http://35.192.73.70/dconf-home.html and see the 'Welcome to the Magic Online Item Game!' page.

2023-09-09 v0.3 instructions
- connect from http://35.192.73.70/dconf-home.html and see the 'Welcome to the Magic Online Item Game!' page
- select a user
  + the order list should appear
- select a order
  + the order row should be highlighted
  + the Edit Order button appears in blue (enabled)
  + the item table appears below
- select the other order, those order items should appear
- click Edit Order
  + the Edit order button should become grey (disabled)
  + a Remove item button should appear over the items as grey
  + a table of available items should appear on the right with an Add item button as grey
- select a item in the left item list
  + the item row should be highlighted
  + the Remove item button should change to blue
- click Remove item
  + the item should move to the available item list
  + the Remove Item button should become grey
- select a item in the available item list
  + the item row should be highlighted
  + the Add item button should change to blue
- click Add item
  + the carAddd should move to the left item list
  + the Remove Item button should become grey
- keep clicking items in the left and available list and move them back and forth
  + try emptying one of the lists and make sure you can move items back into it
- select another order 
  + the order row should be highlighted
  + the Edit Order button appears in blue
  + the item table appears below
  + the available item list disappears
- try editing that order 
- select another user
  + the order list should appear
  + all other tables should disappear
  
You may need some tools to check ports if you can't start dconf_app:
- netstat
  + install with:
    $ sudo apt install net-tools
  + check port 8000 in use:
    $ netstat -ntlp 

```
















