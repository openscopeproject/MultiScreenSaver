#!/bin/bash
for sz in "16" "24" "32" "48" "64" "128"; do
    convert -resize "${sz}x${sz}" -background none appicon_256.png "$sz".png
done
convert -verbose "16.png" "24.png" "32.png" "48.png" "64.png" "128.png" "appicon_256.png" "appicon.ico"
for sz in "16" "24" "32" "48" "64" "128"; do
    rm "$sz.png"
done
