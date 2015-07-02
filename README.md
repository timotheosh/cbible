      __          __       ___             
     /\ \      __/\ \     /\_ \            
  ___\ \ \____/\_\ \ \____\//\ \      __   
 /'___\ \ '__`\/\ \ \ '__`\ \ \ \   /'__`\ 
/\ \__/\ \ \L\ \ \ \ \ \L\ \ \_\ \_/\  __/ 
\ \____\\ \_,__/\ \_\ \_,__/ /\____\ \____\
 \/____/ \/___/  \/_/\/___/  \/____/\/____/
                                           
                                           
cbible

This program is a C/C++ interface to Crosswire's libsword, with readline
support.

Requires:
  libsword from crosswire: http://www.crosswire.org
  GNU readline library
  Cmake 2.8 or greater

This is currently developed on an Ubuntu 14.04 system with stock libs. It
relies on sys/ioctl.h and unistd.h for determining terminal (or screen)
size.

Current possible goals:
* Have some or all the functionality as the KJV Bible program that
  had been sitting on the ancient Sunsite UNIX-C Archives FTP site.
  This program lives on on debian-based systems as the bible-kjv
  package.

* Commandline parameters that can be passed and output similar to
  the Sword Project's own diatheke program.

* Use an ini configuration file for default settings (including setting
  module options and a default bible version).

Must have features that I want to obtain to:
* The ability to write and retrieve entries in the Sword Project's
  Personal commentary.

* An elisp minor mode that works well with org-mode for inserting
  Scripture references, and commentary text.

