#For creating Debian packages
Format: 1.0
Source: cybrinth
Version: 0.1
Binary: cybrinth
Maintainer: James Dearing <dearingj@lifetime.oregonstate.edu>
Architecture: any
Build-Depends: debhelper (>= 4.1.16), gimp, pkg-config, libfreetype6-dev, libtag1-dev, libsdl2-mixer-dev, libirrlicht-dev, libboost-filesystem-dev, automake, autoconf-archive
Files:
 d57283ebb8157ae919762c58419353c8 133282 %name-%version.tar.xz #The numbers are meaningless: https://en.opensuse.org/openSUSE:Build_Service_Debian_builds says OpenSUSE Build Service ignores them