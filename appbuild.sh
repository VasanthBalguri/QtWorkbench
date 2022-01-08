#!/bin/bash 
mkdir appdir
mkdir appdir/usr
mkdir appdir/usr/lib
mkdir appdir/usr/bin

cp build/Qtworkbench appdir/usr/bin
cp QtWorkbench.desktop appdir
cp QtWorkbench.png appdir
ln -s $PWD/appdir/usr/bin/Qtworkbench appdir/AppRun

ARCH=x86_64 ./linuxdeploy-x86_64.AppImage --appdir appdir --output appimage
