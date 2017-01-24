enable
cd src:<mm-new.binaries>
def dsk: dsk:,src:<mm-new>
load %"noinitial" dmmlr+mmailr
reset
close
rename mmailr.*.0 dmailr.*.-1
