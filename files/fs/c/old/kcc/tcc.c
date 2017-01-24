/* TCC - Hack to provide cross-compiler for TENEX (on T20) */

main(argc,argv)
int argc;
char **argv;
{	char **newv, **nv;
	int pid;

	if(argc < 2)
	  {	writez(2,"Use TCC like CC.  See C:CC.DOC for info.\n");
		exit(0);
	  }
	nv = newv = (char **)calloc(argc+6, sizeof(char *));
	if(!newv)
	  {	writez(2,"TCC: cannot allocate arg space.\n");
		exit(0);
	  }
	*nv++ = "CC";		/* Fake program name */
	*nv++ = "-a";		/* Compile for KA-10 */
	*nv++ = "-DSYS_10X=1";	/* C-ENV def: TENEX */
	*nv++ = "-DCPU_KA=1";	/* C-ENV def: KA-10 CPU */
	*nv++ = "-HC:C-H10X.FAI";	/* Special header file
					that points to C:CLB10X */
	while(--argc > 0)
		*nv++ = *++argv;
	*nv = 0;
#ifdef COMMENT
	if(!setlnm("C","PS:<KCC.TENEX>,C:"))
	  {	perror("TCC: Could not set C: logical name");
		exit(0);
	  }
	if((pid = fork())==0)
	  {
#endif COMMENT
		execv("SYS:CC.EXE", newv);	/* Invoke it! */
		perror("TCC: Could not invoke CC");
#ifdef COMMENT
		exit(0);
	  }
	wait(0);		/* Wait for compilation to terminate */
	if(!setlnm("C", (char *)0))
	  {	perror("TCC: Could not reset C: logical name");
		exit(0);
	  }
#endif COMMENT
}

writez(fd,str)
{	write(fd,str,strlen(str));
}

/* Set job-wide logical name */
setlnm(name, def)
char *name,*def;
{	int acs[5], ret;
	if(def)
	  {	acs[1] = 4;	/* .CLNJB */
		acs[2] = (int)(name-1);
		acs[3] = (int)(def-1);
		ret = jsys(0502, acs);	/* CRLNM */
		return(ret > 0 ? 1 : 0);
	  }
	acs[1] = 0;	/* .CLNJ1 */
	acs[2] = (int)(name-1);
	ret = jsys(0502, acs);	/* CRLNM */
	return(ret > 0 ? 1 : 0);
}
#ifdef COMMENT
/* Save current definition of C: for restoring later */
char *dirnam = "C";
char dirdef[200];
int dirflg = 0;
savecdir()
{	int acs[5];
	int ret;
	acs[1] = 0;	/* .LNSJB */
	acs[2] = (int)(dirnam-1);
	acs[3] = (int)(dirdef-1);
	ret = jsys(0504,acs);	/* LNMST */
	if(ret > 0) dirflg++;
}

/* Restore old definition of C: */
restcdir()
{	int acs[5];
	if(!dirflg) return;
	acs[1] = 0;
}
#endif COMMENT
