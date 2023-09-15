# Dconf Berkeley DB
This project implements an Dconf search website using Berkeley DB.
- this repo:  
 https://github.com/kushnertodd/dconf-berkeley-db

# Table of Contents
- [Usage](#usage)
  - [Web interface](#web-interface-usage)
- [Architecture](#architecture)
  - [Linux](#linux-architecture)
  - [Berekley DB](#berekley-db-architecture)
  - [Json-c](#json-c-architecture)
  - [Dconf database](#dconf-database-architecture)

## Usage <a name="usage"/>
### Web interface <a name="web-interface-usage"/>

The web interface can be tested as follows (from git branch `vx.0` on).
Run from separate shells in `dconf-berkeley-db/src/scripts`.
The curl command  `src/scripts/request.sh` runs a 
`load_name/sample database request.
```sh
$ cd src/scripts
$ ./cmake-build-debug/bin/bdb_unix_lookup   -d json/bdb-databases.json  -h ../dconf-data/db-sample
$ ./cmake-build-debug/bin/dconf_lookup
$ curl -d '@request.json' -X POST http://localhost:8000/dconf-request-dto
```
## Architecture <a name="architecture"/>
It will consist of:
- a back-end feeding data from a database and web APIs
- a web server to serve it up on the web
Other featuers are:
- For now, the project will cross-compile on Windows 10 and WSL2 Linux, running under Windows.
- Berkeley DB, a key-indexed database, much faster than a relational DB if we don't need full SQL functionality 
It interfaces with this [Organizer page](Organizer.md)
### Linux <a name="linux-architecture">

Useful resources for learning and using Linux are:
- [Linux.org](https://www.linux.org/)
- [The Linux Documentation Prject](https://tldp.org)
- [The Linux command line for beginners](https://ubuntu.com/tutorials/command-line-for-beginners#1-overview)
- [Learn Linux, 101: A roadmap for LPIC-1](https://developer.ibm.com/tutorials/l-lpic1-map/?mhsrc=ibmsearch_a&mhq=lpic-1%20exam)
- [The Linux Command Line](http://linuxclass.heinz.cmu.edu/doc/tlcl.pdf)
- [TutorialsPoint UNIX / LINUX Tutorial](https://www.tutorialspoint.com/unix/index.htm)
- [Javatpoint Linux/Unix Tutorial](https://www.javatpoint.com/linux-tutorial)
- [Vim Tips Wiki](https://vim.fandom.com/wiki/Vim_Tips_Wiki)

### Berekley DB <a name="berekley-db-architecture"/>
- Resources
Various websites have useful information and tutorials for using Berkeley DB:
- http://web.mit.edu/ghudson/dev/third/rpm/db/docs/reftoc.html
- https://github.com/SerenadeEXE/BerkeleyDBTutorial
The latter can be downloaded with:
```
$ git clone github.com/SerenadeEXE/BerkeleyDBTutorial
```
The `examples` directory has some C examples and instructions in `examples/README`.

### Json-c <a name="json-c-architecture"/>
These are useful tutorials and resources for using json-c:  
- [A simple and complete json parser](https://linuxprograms.wordpress.com/category/json-c])  
- [json-c / libjson Tutorial with Examples](https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/)
- [Json-c Tutorial](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/index.md)  
- [JSON-C Tutorial: How to Parse JSON in C](https://progur.com/2018/12/how-to-parse-jffson-in-c.html)
- [json-c Tutorial introduction & outline](https://alan-mushi.github.io/2014/10/28/json-c-tutorial-intro-outline.html)
- [json-c Documentation](https://json-c.github.io/json-c/json-c-0.16/doc/html/index.html)
- API Documentation
json-c API documentation can be found at:
- [json-c Documentation](https://json-c.github.io/json-c/json-c-0.16/doc/html/index.html)

### Dconf database <a name="dconf-database-architecture"/>


