#!/bin/sh

# turns a png file into a 32x32 image for Discord
url=$(rofi -dmenu)
magick "$url" -resize 32x32 - | xclip -t image/png -sel clip
