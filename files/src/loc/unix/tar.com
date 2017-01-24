		[ TAR.COM - 29-Jan-83/JLR ]
Commands
CREATE	[name ,...]		Write files at beginning of tape
EXTRACT	[name ...]		Extract the named file(s) from tape
HELP				Type this text
QUIT				Exit from Tar
REWIND				Rewind tape drive
TAKE	[file]			Take commands from named file
TAPE	device:			Declare tape drive to use
TYPE	[name ...]		Directory contents of tape
WRITE	[name ,...]		Write files at end of tape

ASK	Command			Ask for comfirmation of Command actions
	valid commands: EXTRACT, CREATE, WRITE
VERBOSE	Command			list Command actions as they take place
	valid commands: EXTRACT, CREATE, WRITE, TYPE
NOSUPERSEDE Command		Do not overwrite existing disk files
	vaild command:  EXTRACT
NOMTIME Command			Do not restore the modification time
	vaild command:	EXTRACT

Example of usage
The following sequence writes your CONNECTed directory to tape
(destroying the previous contents of the tape)

	@TAR
	TAR>TAPE MTA0:
	TAR>CREATE *.*.*
	TAR>EXIT
		
You could then login on a UNIX system and use

	% tar xv

which would restore all the files on the tape into your current working
directory.

For more help, see <Unix.Man>Tar.Hlp and Tar.Doc.
