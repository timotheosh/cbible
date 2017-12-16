```
      __          __       ___
     /\ \      __/\ \     /\_ \
  ___\ \ \____/\_\ \ \____\//\ \      __
 /'___\ \ '__`\/\ \ \ '__`\ \ \ \   /'__`\
/\ \__/\ \ \L\ \ \ \ \ \L\ \ \_\ \_/\  __/
\ \____\\ \_,__/\ \_\ \_,__/ /\____\ \____\
 \/____/ \/___/  \/_/\/___/  \/____/\/____/


```
[![Build Status](https://travis-ci.org/timotheosh/cbible.svg?branch=master)](https://travis-ci.org/timotheosh/cbible)

cbible

This program is a C/C++ interface to Crosswire's libsword, with
readline support, that includes an Emacs minor-mode for inserting
Scripture references and for saving selected text (or entire Emacs
buffer) as commentary text in the libsword Personal commentary.

Requires:
  * libsword from crosswire: http://www.crosswire.org
  * GNU readline library
  * Cmake 2.8 or greater

On Ubuntu 16.04, you can install these requirements with:
```
sudo apt-get install build-essentials libsword-dev libreadline-dev cmake
```

NOTE: If you are new to Crosswire's libsword, it is highly recommended
that you first install some software that will help you in installing
Bible versions that can be used, and a Personal commentary that you
can insert your own entries into.

This is currently developed on an Ubuntu 16.04 system with stock libs. It
relies on sys/ioctl.h and unistd.h for determining terminal (or screen)
size.

Current Features

* Uses a config file for setting the default Bible version (single
  user, only, in ~/.cbible.cfg). The config file is created when the
  program is used for the first time.

* The ability to write and retrieve entries in the Sword Project's
  Personal commentary.

* An elisp minor mode that works well with org-mode for inserting
  Scripture references, and commentary text.

Current possible goals:
* Have some or all the functionality as the KJV Bible program that
  had been sitting on the ancient Sunsite UNIX-C Archives FTP site.
  This program lives on on debian-based systems as the bible-kjv
  package.

* Commandline parameters that can be passed and output similar to
  the Sword Project's own diatheke program.

This program currently does not have a search feature nor does it use module options.
