while true
    set string (amixer get Master | grep '..%' -o | head -n 1) "|"
    set string $string (date "+%a %m/%d")
    xsetroot -name (echo $string)
    sleep 1m
end
