while true
    set string (acpi | grep '..%' -o | head -n 1) "|"
    set string $string (date "+%a %m/%d %I:%M")
    xsetroot -name (echo $string)
    sleep 1m
end
