#!/bin/sh
cd $HOME
ls
cdstr="$( ( ls -d */; ls -d */*/; ls -d */*/*/; ls -d */*/*/*/ ) | rofi -dmenu -p Directories -i)"

if [ -z "$cdstr" ]
then
   exit
else
   $HOME/dwm/scripts/fishcmd.dash "$cdstr"
fi
