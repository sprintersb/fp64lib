#!/bin/bash

for f in *.S; do
    fils="$fils@@$f"
done

echo "### Auto-generated from Files.tpl by running $(basename $0)"
echo ""

awk '{ if ($0 ~ /@@@/) print "\t'${fils:2}'"; else print $0; }' Files.tpl \
    | sed -e 's:@@: \\\n\t:g'
