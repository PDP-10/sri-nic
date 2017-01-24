/*
 * Copyright (c) 1987,1988 Massachusetts Institute of Technology.
 *
 * Note that there is absolutely NO WARRANTY on this software.
 * See the file COPYRIGHT.NOTICE for details and restrictions.
 */

/* network stream code -- nopen, nclose, nsend, nrecv, nabort */

#include "domsym.h"
#include <macsym.h>
#include <monsym.h>

/* May not be in MONSYM.H yet. */
#ifndef JSYS
#define JSYS(name,acs) (jsys(monsym((name)),(acs)))
#endif

/*
 * Randomness for device types.  TCP is not standardized across
 * versions of TOPS-20, and Chaos may not be defined on a particular
 * system.  So we kludge things to get it at runtime if necessary.
 */

#define HAVE_TCP (monsymdefined("GTHST%")|monsymdefined("GTDOM%"))
#if HAVE_TCP
#if monsymdefined(".DVTCP")
#define _DVTCP	monsym(".DVTCP")
#define NEED_TCP 0
#else
#define _DVTCP	devtcp_runtime_value()
static int devtcp_runtime_value()
{
    static int stash = -1;
    return(stash == -1 ? (stash = devtyp("TCP:")) : stash);
}
#define NEED_TCP 1
#endif
#else
#define _DVTCP	(0)
#define NEED_TCP 0
#endif

#define HAVE_CHA monsymdefined("CHANM%")
#if HAVE_CHA
#if monsymdefined(".DVCHA")
#define _DVCHA	monsym(".DVCHA")
#define NEED_CHA 0
#else
#define _DVCHA	devcha_runtime_value()
static int devcha_runtime_value()
{
    static int stash = -1;
    return(stash == -1 ? (stash = devtyp("CHA:")) : stash);
}
#define NEED_CHA 1
#endif
#else
#define	_DVCHA	(0)
#define NEED_CHA 0
#endif

#if NEED_TCP | NEED_CHA
static int devtyp(dev)
    char *dev;
{
    int ac[5];
    ac[1] = (int)(dev-1);
    return(JSYS("STDEV%",ac) ? ((ac[2]>>18)&~monsym(".DVDES")) : -1);
}
#endif

/* This should be in MONSYM, but it may not be. */
#if	monsymdefined(".CSOPN")
#define _CSOPN monsym(".CSOPN")
#else
#define _CSOPN		(04)
#endif

/*
 * nopen(host,contact,protocol)
 *  host -- 32 or 16 bit host number
 *  contact -- contact name
 *  protocol -- XP_mumble (see INTERN.D)
 *
 * returns:  -1 on error
 *	      jfn on success
 */

/* Random parameters for TCP: OPENF% call */
#if HAVE_TCP
static char tcp_active[] = ";Connection:Active;Persist:30;Timeout:30";
#endif

int nopen(host, contact, protocol)
    int host;
    char *contact;
    int protocol;
{
    int ac[5];
    char open_name[170];

    switch(protocol) {

#if HAVE_CHA
	case XP_CHAOS:
	    sprintf(open_name,"CHA:%o.%s", host, contact);
	    ac[1] = monsym("GJ%SHT");
	    ac[2] = (int) (open_name-1);
	    if(!JSYS("GTJFN%",ac))
		return(-1);
	    ac[2] = FLD(8,monsym("OF%BSZ"))|monsym("OF%RD")|monsym("OF%WR");
	    if(!JSYS("OPENF%",ac))
		return(-1);
	    return(ac[1]);
#endif

#if HAVE_TCP
	case XP_TCP:
	    sprintf(open_name,"TCP:.%o-%d%s",
			host, tblook(ws_table,contact), tcp_active);
	    ac[1] = monsym("GJ%SHT");
	    ac[2] = (int) (open_name-1);
	    if(!JSYS("GTJFN%",ac))
		return(-1);
	    ac[2] = FLD(	       8,monsym("OF%BSZ")) | monsym("OF%RD")
		  | FLD(monsym(".TCMWH"),monsym("OF%MOD")) | monsym("OF%WR");
	    if(!JSYS("OPENF%",ac))
		return(-1);
	    return(ac[1]);
#endif

	default:
	    bugchk("nopen","bad protocol number %d", protocol);
    	    return(-1);
    }
}


/*
 * nsend(jfn,bytes,size)
 * sends size bytes from bytes down jfn.
 *
 * returns boolean indicating success.
 */

int nsend(jfn,bytes,size)
    int jfn, size;
    char *bytes;
{
    int ac[5];

    ac[1] = jfn;			/* output designator */
    ac[2] = size>>8;
    if(!JSYS("BOUT%",ac))	/* first two bytes are count */
	return(0);			/* of the remaining data */
    ac[2] = size&0xFF;
    if(!JSYS("BOUT%",ac))
	return(0);
    ac[2] = (int) (bytes-1);		/* now the data */
    ac[3] = -size;			/* exactly this much */
    if(!JSYS("SOUTR%",ac))	/* output and push */
	return(0);
    return(1);				/* return win */
}

/*
 * nrecv(jfn,bytes,size)
 * reads (at most) size bytes into bytes from jfn.
 * error if too many bytes to fit.
 * returns zero on error, else count of bytes read.
 */

int nrecv(jfn,bytes,size)
    int jfn,size;
    char *bytes;
{
    int ac[5], count;
    ac[1] = jfn;			/* net connection */
    if(!JSYS("BIN%",ac))	/* two bytes of length */
	return(0);
    count = ac[2]<<8;
    if(!JSYS("BIN%",ac))
	return(0);
    count += ac[2];
    if(count > size)			/* make sure will fit in buffer */
	return(0);
    ac[2] = (int) (bytes-1);		/* ok, read the string */
    ac[3] = -count;
    if(!JSYS("SIN%",ac))
	return(0);
    return(count);			/* return win */
}

/*
 * nclose(jfn)
 * close net connection, return boolean indicating success.
 */

int nclose(jfn)
    int jfn;
{
    int ac[5];

#if HAVE_TCP

/*
 * The following weirdness is to work around a long standing bug
 * in the Twenex TCPJFN code.  This code from SIERRA:<TCP>BOISE.MAC.
 */

    ac[1] = jfn;			/* only do this for TCP device */
    if(JSYS("DVCHR%",ac) && FLDGET(ac[2],monsym("DV%TYP")) == _DVTCP) {
	ac[1] = jfn;			/* trashed by DVCHR% */
	ac[2] = monsym(".TCSFN");	/* send a FIN down this jfn */
	if(!JSYS("TCOPR%",ac))		/* try handling errors, but */
	    return(0);			/* TCOPR% is pretty flakey... */
	while(JSYS("BIN%",ac))		/* suck bytes till EOF */
	    ;				/* (which gives JSYS error) */
    }					/* done crocking TCP: device */
#endif

    ac[1] = jfn;			/* in any case, close the jfn */
    return(JSYS("CLOSF%",ac));	/* we win if the jsys does */
}					/* th-th-that's all folks */

/* nabort(jfn) -- abort a network connection */

int nabort(jfn)
    int jfn;
{
    int ac[5];

    jfn &= 0777777;			/* paranoia */
    ac[1] = jfn|monsym("CZ%ABT");	/* abort the connnection */
    if(!JSYS("CLOSF%",ac)) {
	ac[1] = jfn;			/* lost, may still have jfn */
	JSYS("RLJFN%",ac);	/* so flush it, ignore errors */
    }
    return(1);				/* and claim we won */
}

/*
 * ncheck(jfn)
 * check status of net connection.  returns nonzero if connection live.
 */

int ncheck(jfn)
    int jfn;
{
    int ac[5], statwd, typewd;

    ac[1] = jfn;
    if(!JSYS("GTSTS%",ac)
      || (ac[2] & (monsym("GS%NAM")|monsym("GS%ERR"))) != monsym("GS%NAM"))
	return(0);			/* JFN invalid or in error state */
    if(!JSYS("GDSTS%",ac))	/* Get device dependent status */
	return(0);			/*  Timing screw */
    statwd = ac[2];			/* Isolate connection state word */
    if(!JSYS("DVCHR%",ac))	/* Get device designator */
	return(0);			/*  Timing screw */
    typewd = FLDGET(ac[2],monsym("DV%TYP")); /* Isolate device type */
    /*
     * We would use a switch() here except that we may not know the
     * device types for the various network devices until runtime.
     * Fortunately, KCC is smart enough to omit unreachable code.
     */
    if(HAVE_TCP && typewd == _DVTCP)
	return(statwd == XWD(monsym(".TCSYN"),monsym(".TCSYN")));
    if(HAVE_CHA && typewd == _DVCHA)
	return((statwd & 017) == _CSOPN);
    /* If we get here, it's an unknown device. */
    return(0);
}
