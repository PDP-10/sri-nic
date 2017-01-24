29-Sep-83 11:49:17-PDT,2762;000000000001
Return-Path: <MRC@SU-SCORE.ARPA>
Received: from SU-SCORE.ARPA by SRI-NIC with TCP; Thu 29 Sep 83 11:49:13-PDT
Date: Thu 29 Sep 83 11:48:56-PDT
From: Mark Crispin <MRC@SU-SCORE.ARPA>
Subject: Re: Some questions
To: KLH@SRI-NIC.ARPA
In-Reply-To: Message from "Ken Harrenstien <KLH@SRI-NIC>" of Wed 28 Sep 83 18:14:45-PDT
Postal-Address: 725 Mariposa Ave. #103; Mountain View, CA 94041
Phone: (415) 497-1407 (Stanford); (415) 968-1052 (residence)

Ken -

     .INSRT libraries should default to (in order) to device MID:, UNV:,
and SYS: on both TOPS-10 and TOPS-20.  WAITS should use SYS:.

     Yes, CCL doesn't work.  Either use the TOPS-10 TMPCOR UUO (which will
load in PA1050, ugh) or learn how the PRARG% JSYS works.  FAIL uses PRARG%
so you might want to look at FAIL's code.

     Bug: the command "R MIDAS" attempts to assemble MIDAS.MID.  It should
only assume a RSCAN% argument if it see "MIDAS", "NMIDAS", etc.  It should
NOT do it for "R", "RUN", "START", etc.  Strictly speaking, what you should
do is see if the first word is a command or filename so things like
OLD:MIDAS FOO would work, but it's hairy to do so considering all the hairy
ways to invoke programs.

     Bug: MIDAS does not write out the system symbol table along with the
program's.  RMS thinks this is the right thing, but DEC DDT disagrees and
is quite unlikely to change due to its limited address space -- every time
a bugfix is inserted something has to be rewritten or removed!  The answer
is not "use IDDT"; that translates into "don't use MIDAS".  It is alright
to do what MACRO does, e.g. just write out the system symbols which the
program actually uses.

     Consider the following bugs or features:

     MIDAS still doesn't write usable listing or CREF files.  Sometimes
@'s limitations are intolerable.  It should write output for DEC CREF,
and some thought should be given to listing control.  FAIL and especially
MACRO does much better.  This is especially the case when using the various
structured programming macros.  .DIRECTIVE FLBLST is especially useful to
have implemented.

      MIDAS should understand universal files, or the procedures for updating
MIDAS should be made clearer.  It is rather a crock that you have to rebuild
MIDAS every time MONSYM or MACSYM changes.

     It is a major limitation of MIDAS that it does not support PSECTs or
Polish expressions.  That's why I've pretty much abandoned programming in
MIDAS; most of my stuff uses PSECTs.

     It is possible that MIDAS doesn't run on TOPS-20 as well as it could;
are you using SIN%/SOUT% to do output or are you using PMAP%?  If you're
using SIN%/SOUT%/BIN%/BOUT% MIDAS probably runs slower than TOPS-10 MIDAS
with PA1050!
-------
