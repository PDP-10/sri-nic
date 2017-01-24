!This command file will startup the necessay Batch Streams, Printers,
! and Readers allows all messages to appear on the console.
! The objects all will use system default limits which can be changed
! by adding various Set commands.
! If you wish to suppress Job Messages for all objects insert the following
! command into this file.
!
DISABLE OUTPUT-DISPLAY ALL-MESSAGES /JOB
!
SET PRINTER GRUMPY PAGE-LIMIT 20000
SET PRINTER SMOKEY PAGE-LIMIT 20000
SET PRINTER SPEEDY PAGE-LIMIT 20000
SET PRINTER PIPPIN PAGE-LIMIT 20000
SET PRINTER HP PAGE-LIMIT 20000
SET PRINTER DDNPMO PAGE 20000
SET BATCH-STREAM 0:2 TIME-LIMIT 720	!Set Time Limit in minutes for Batch
!
START BATCH-STREAM 0:2
START PRINTER GRUMPY
START PRINTER SMOKEY
START PRINTER SPEEDY
START PRINTER PIPPIN
START PRINTER HP
START PRINTER DDNPMO
