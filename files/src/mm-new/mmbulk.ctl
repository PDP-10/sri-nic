enable
cd src:<mm-new.binaries>
def dsk: dsk:,src:<mm-new>
load %"noinitial" bmmlr+mmailr
reset
close
rename mmailr.*.0 mmbulk.*.-1
