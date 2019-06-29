#!/bin/fish
set string "⚡"(acpi | grep '..%' -o | head -n 1) "|"
set string $string 🔈(pamixer --get-volume-human) "|"
set string $string (date "+%a %m/%d %I:%M")
xsetroot -name (echo $string)
