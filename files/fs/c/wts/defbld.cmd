! TOPS-20 Command file to set up cross-compiling for a WAITS target system.
! There are two things that need to be redefined:
!	C:  To specify the search path for include files and library rels.
!	KCC: To identify the C compiler binary to use.  This is normally a
!		version of XCC that calls SYS:CCX with the right switches.
define c:	ps:<KLH.PORT.WTS>, ps:<KLH.PORT.INCLUDE>
define kcc:	p:ccwts.exe
