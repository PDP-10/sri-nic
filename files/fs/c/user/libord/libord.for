      PROGRAM LIBORD
C  Program for determining order dependence for a one pass library
C  coded by Neil Maron, modified heavily by Norm Samuelson
C
      INCLUDE 'LIBORD.COM'
C
C version 2.2 put listing after ordering, add info to listing.
C version 2.1 change list file prompt to one line
C version 2.0 extract into .TMP files, then use APPEND
C version 1.9 use 2-D structures, not 1-D
C version 1.8 use CHARACTER variables where needed
C UPD ID= 18, PS:<SUPPORT.UTILITIES>LIBORD.FOR.2,  18-Nov-83 05:26:30 by
C  Samuelson
C minor mods for TOPS-20
C   VERSION DATE COMMENTS
C     1    12JUN80 NEW.
C     1.1  22Dec81 increased CB() to 3000.
C     1.2  05May83 fix up rel block types for Fortran-77.
C     1.3  09May83 increased datsiz from 5000 to 10000 for DATAPLOT.
C     1.4  10May83 write out duplicate name when multiply defined.
C     1.5  19May83 fixed bug that came when a 0 count was seen for entry
C                  block which are the result of .UNV files.
C     1.6  15Jun83 Type 1004 really has long counts contrary to current
C                  link manual (April 1982) so fixed SKIPBL for this.
C
C  LIMIT NUMBER OF .REL FILES TO 20
C  THE .REL FILES MUST ALL BE IN YOUR PATH AND
C  HAVE THE EXTENSION ".REL"
C
      DATA TOPS20/.TRUE./
C
      IF (TOPS20) THEN
         WRITE (5, 10) '20'
   10    FORMAT (' LIBORD-',A,' ver 2.2  8/20/84')
       ELSE
         WRITE (5, 10) '10'
       END IF
      OPEN (UNIT=MICFIL, FILE='LIBORD.MIC', MODE='ASCII')
C  GATHER DATA...
      CALL GATHER
C  Order all the modules
      CALL ORDMOD
C  PRINT DATA...
      CALL DISPLA
C  All done
      CLOSE (UNIT=MICFIL)
      CALL EXIT
      END
      SUBROUTINE BUILDF (IMOD,NEWCLS,NCLUST)
      INCLUDE 'LIBORD.COM'
      INTEGER IMOD, NEWCLS, NCLUST, LP
      CHARACTER*80 LINE, COMENT
      CHARACTER MODNUM*4, MNAME*6, CMNT*11
      LOGICAL INPROG
      DATA INPROG/.FALSE./,LP/0/
C
      IF (NEWCLS.NE.0) THEN
C  if we have a line open close it
         LP=LP-1
         IF (INPROG) THEN
            IF (TOPS20) THEN
               WRITE (MICFIL, 10) COMENT(:LP),LINE(:LP),LINE(:LP+1)
   10 FORMAT (';       ',A,/                                            
     1        '@APPEND ',A,' LMASTR.TMP',/                              
     2        '@DELETE ',A,/'@EXPUNGE',/'@')
             ELSE
               WRITE (MICFIL, 20) COMENT(:LP),LINE(:LP),LINE(:LP)
   20          FORMAT (';                           ',A/                
     1                 '.COPY LMASTR.TMP=LMASTR.TMP,',A/'.DELETE ',A)
             END IF
            INPROG=.FALSE.
          END IF
         IF (NEWCLS.LT.0) THEN
            IF (TOPS20) THEN
               WRITE (MICFIL, 30)
   30          FORMAT ('@R MAKLIB'/'*WORK.REL=LMASTR.TMP/INDEX'/        
     1            '@DELETE L*.TMP,'/'@EXPUNGE'/'@'/'@VD WORK.REL')
             ELSE
               WRITE (MICFIL, 40)
   40          FORMAT ('.R MAKLIB'/'*WORK.REL=LMASTR.TMP/INDEX'/        
     1            '.DELETE L*.TMP',/,'.DIR WORK.REL')
             END IF
            RETURN
          END IF
         WRITE (MICFIL, 50) NCLUST
   50    FORMAT (';Cluster ',i4)
         NEWCLS=0
       END IF
      IF (INPROG) THEN
         IF (((TOPS20).AND.(LP .GE. 80-9-11-11)).OR.                    
     1       ((.NOT.TOPS20).AND. (LP.GT.80-6-11-11-11))) THEN
C  lose...not enough room, end the line and try for another
            LP=LP-1
            IF (TOPS20) THEN
               WRITE (MICFIL, 10) COMENT(:LP),LINE(:LP),LINE(:LP+1)
             ELSE
               WRITE (MICFIL, 20) COMENT(:LP),LINE(:LP),LINE(:LP)
             END IF
            LINE = ' '
            LP = 0
          END IF
       ELSE
         LINE = ' '
         LP = 0
       END IF
C  now install the module .tmp file name into line
      WRITE (MODNUM, 60) IMOD
   60 FORMAT (O4)
      WRITE (CMNT, 70) MODNAM(IMOD),CB(SPCLEV,IMOD)
   70 FORMAT (A,I4,',')
      LINE(LP+1:)='LM'//MODNUM//'.TMP,'
      COMENT(LP+1:)=CMNT
      LP = LP+11
      INPROG=.TRUE.
      RETURN
      END
      SUBROUTINE C50TO6 (IWORD,MNAME)
C  CONVERT RADIX50 INPUT (IWORD) TO CHARACTER*6 (MNAME)
      CHARACTER MNAME*6, IC*1
      INTEGER IWORD, IW, JW, I, J
C
      JW=IWORD.AND."037777777777
      DO 10 I=1,6
         J=7-I
         IW=MOD(JW,40)
         JW=JW/40
         IF (IW.EQ.0) THEN
C  NULL
            IC=' '
         ELSE IF (IW.GE.1 .AND. IW.LE."12) THEN
C  NUMBER
            IC=CHAR(IW-1+ICHAR('0'))
         ELSE IF (IW.EQ."45) THEN
            IC='.'
         ELSE IF (IW.EQ."46) THEN
            IC='$'
         ELSE IF (IW.EQ."47) THEN
            IC='%'
          ELSE
C  A LETTER
            IC=CHAR(IW-"13+ICHAR('A'))
          END IF
         MNAME(J:J)=IC
   10    CONTINUE
      CALL TRIM (MNAME,I)
      RETURN
      END
      SUBROUTINE DISPLA
      INCLUDE 'LIBORD.COM'
      CHARACTER FILNAM*80, MNAME*6
      INTEGER IFILE, IMOD, JMOD, SCNT, WORD, IWRD, DCNT
C
      WRITE (5, 10)
   10 FORMAT (' Listing file(<cr> for none): ',$)
      READ (5, 20) FILNAM
   20 FORMAT (A)
      IF (FILNAM.EQ.' ') RETURN
      OPEN (UNIT=LSTFIL, FILE=FILNAM, MODE='ASCII', ACCESS='SEQOUT')
      DO 100 IFILE=1,NRELS
         WRITE (LSTFIL, 30) RELFS(IFILE)
   30    FORMAT (//,' =========File: ',A)
         IMOD=RELMOD(MODPTR,IFILE)
         SCNT=RELMOD(MODCNT,IFILE)
         DO 90 JMOD=IMOD,IMOD+SCNT-1
            WRITE (LSTFIL, 40) JMOD, MODNAM(JMOD),                      
     1           CB(SPCCNM,JMOD), CB(SPCLEV,JMOD)
   40       FORMAT (/,' Module ',O4': ',A,/' Cluster:',I4,' Level: ',I4)
C  WRITE ENTRY POINTS
            CALL LISTIN
            WORD=CB(SPCENT,JMOD)
            DCNT=CB(SPCEPC,JMOD)
            IF (DCNT.NE.0) THEN
               WRITE (LSTFIL, 50)
   50          FORMAT (' Entries--')
               DO 60 IWRD=WORD,WORD+DCNT-1
                  CALL C50TO6 (DATBUF(IWRD),MNAME)
                  CALL LISTSE (MNAME)
   60             CONTINUE
               CALL LISTCU
             END IF
C  WRITE EXTERNALS
            CALL LISTIN
            DCNT=CB(SPCERC,JMOD)
            IF (DCNT.NE.0) THEN
               WORD=CB(SPCEXT,JMOD)
               WRITE (LSTFIL, 70)
   70          FORMAT (' Externals---')
               DO 80 IWRD=WORD,WORD+DCNT-1
                  CALL C50TO6 (DATBUF(IWRD),MNAME)
                  CALL LISTSE (MNAME)
   80             CONTINUE
               CALL LISTCU
             END IF
   90       CONTINUE
  100    CONTINUE
      CLOSE (UNIT=LSTFIL)
      RETURN
      END
      SUBROUTINE ENTADD (IWORD,ERRFLG)
C  Add the entry point in IWORD to the sorted ENTRY table
      INCLUDE 'LIBORD.COM'
      INTEGER IWORD, I, ILOC, MOD1
      LOGICAL ERRFLG, FOUND
      CHARACTER*6 EPNAM
C  Find where it belongs
      CALL ENTFND (IWORD,ILOC,FOUND)
      IF (FOUND) THEN
         CALL C50TO6 (IWORD, EPNAM)
         MOD1 = ENTTAB(ENTMOD,ILOC)
         TYPE 10, EPNAM, MODNAM(MOD1), RELFS(CB(SPCFIL,MOD1)),          
     1                   MODNAM(NMOD), RELFS(CB(SPCFIL,NMOD))
   10    FORMAT (' ?Duplicate ENTRY name ',A,' found in:',              
     1           2(/'  Module: ',A,' File: ',A))
         ERRFLG = .TRUE.
         RETURN
       END IF
C  Make room for it (if needed)
      DO 20 I=NUMENT,ILOC,-1
         ENTTAB(ENTNAM,I+1)=ENTTAB(ENTNAM,I)
         ENTTAB(ENTMOD,I+1)=ENTTAB(ENTMOD,I)
   20    CONTINUE
C  Insert this word
      NUMENT = NUMENT+1
      ENTTAB(ENTNAM,ILOC)=IWORD
      ENTTAB(ENTMOD,ILOC)=NMOD
      RETURN
      END
      SUBROUTINE ENTFND (IWORD, ILOC, FOUND)
C  Find entry point matching IWORD
C  If found, return location in ILOC
C  If not found, return in ILOC the place where it will go.
      INCLUDE 'LIBORD.COM'
      INTEGER IWORD, ILOC
      LOGICAL FOUND
C
      FOUND = .FALSE.
      DO 10 ILOC=1,NUMENT
         IF (IWORD.LE.ENTTAB(ENTNAM,ILOC)) THEN
            IF (IWORD.EQ.ENTTAB(ENTNAM,ILOC)) FOUND = .TRUE.
            RETURN
          END IF
   10    CONTINUE
      ILOC = NUMENT + 1
      FOUND = .FALSE.
      RETURN
      END
      SUBROUTINE ERROR (N)
      INTEGER N
C
      GO TO (10, 30, 50, 70, 90, 110), N
   10 WRITE (5, 20)
   20 FORMAT (' ?Too many modules (MODSIZ)')
      GO TO 130
   30 WRITE (5, 40)
   40 FORMAT (' ?Too many global symbols (DATSIZ)')
      GO TO 130
   50 WRITE (5, 60)
   60 FORMAT (' ?Bad order for ENTRY block')
      GO TO 130
   70 WRITE (5, 80)
   80 FORMAT (' ?Bad order for NAME block')
      GO TO 130
   90 WRITE (5, 100)
  100 FORMAT (' ?Bad order for COMMON block')
      GO TO 130
  110 WRITE (5, 120)
  120 FORMAT (' ?Bad order for EXT-REQ block')
  130 CALL EXIT
      END
      SUBROUTINE GATHER
      INCLUDE 'LIBORD.COM'
      LOGICAL ERRFLG
      INTEGER IFILE, I, J, K, IWORD, ITYPE, ICOUNT, MYPNT, MYCNT,       
     1     ORDER, IWHAT, LEN
      CHARACTER LINE*150, FILNAM*80, MNAME*6
C
      WRITE (5, 10)
   10 FORMAT (' Input rel file spec''s 1 per line, extra cr when done')
C We dont want to clobber any important .TMP files here...
      IF (TOPS20) THEN
         WRITE (MICFIL, 20)
   20    FORMAT ('@ERROR %'/'@VD L*.TMP'/'@IF(NOERROR)@KMIC'/           
     1           '@R MAKLIB')
       ELSE
         WRITE (MICFIL, 30)
   30    FORMAT ('.ERROR %'/'.DIR L*.TMP'/'.IF(NOERROR).MIC ABORT'/     
     1           '.COPY LMASTR.REL=NUL:'/'.R MAKLIB')
       END IF
      ERRFLG = .FALSE.
      NMOD = 0
      DATPTR = 1
C
      DO 120 I=1,MAXRLS
   40    WRITE (5, 50)
   50    FORMAT (' *',$)
         READ (5, 60) FILNAM
   60    FORMAT (A)
         IF (FILNAM.EQ.' ') GO TO 130
         CALL NAMFIX (FILNAM)
         DO 80 J=1,NRELS
            IF (FILNAM.EQ.RELFS(J)) THEN
               TYPE 70
   70          FORMAT (' %Duplicate filename ignored')
               GO TO 40
             END IF
   80       CONTINUE
         OPEN (UNIT=RELFIL, FILE=FILNAM, ACCESS='SEQIN', ERR=90)
         GO TO 110
   90    TYPE 100, FILNAM
  100    FORMAT (' ?Not found: ',A)
         ERRFLG=.TRUE.
         GO TO 40
  110    CONTINUE
         CLOSE (UNIT=RELFIL)
         NRELS=I
         RELFS(NRELS)= FILNAM
  120    CONTINUE
  130 IF (ERRFLG) PAUSE '%One or more files not found'
      IF (NRELS.EQ.0) STOP '?No input files'
C
      ERRFLG = .FALSE.
      DO 2060 IFILE=1,NRELS
         OPEN (UNIT=RELFIL, FILE=RELFS(IFILE), MODE='IMAGE', ACCESS='SEQ
     1IN')
         CALL FINIT
         RELMOD(MODPTR,IFILE)=NMOD+1
         RELMOD(MODCNT,IFILE)=0
  140    ORDER=0
  150    CALL RWORD (IWORD)
         IF (EOFF) GO TO 2050
         ICOUNT=IWORD.AND.HWORD
         ITYPE=IROT(IWORD,18).AND.HWORD
         IF (ITYPE.EQ.2) GO TO 200
         IF (ITYPE.EQ.4) GO TO 400
         IF (ITYPE.EQ.5) GO TO 500
         IF (ITYPE.EQ.6) GO TO 600
         IF (ITYPE.EQ."20) GO TO 2000
         CALL SKIPBL (ITYPE,ICOUNT)
         IF (EOFF) GO TO 2050
         GO TO 150
C
C SYMBOL BLOCK
C
*NEXT=200
  200    IF (ORDER.NE.4) THEN
C  FIRST TIME SO DO SETUP
            CB(SPCEXT,NMOD)=DATPTR
            ORDER=4
          END IF
  210    K=MIN(18,ICOUNT)
         CALL RWORD (IWORD)
         IF (EOFF) GO TO 2050
         DO 240 I=1,K
            CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
            IF (MOD(I,2).EQ.0) GO TO 240
            IWHAT=IWORD.AND."740000000000
            IF (IWHAT.NE."600000000000) GO TO 240
C  NOW JUST SAVE RADIX50 PART
            IWORD=IWORD.AND."037777777777
C  IF IT EXIST IN THE COMMON BLOCK TABLE DONT PUT IT IN DATBUF
            MYCNT=CB(SPCCBC,NMOD)
            IF (MYCNT.GT.0) THEN
               MYPNT=CB(SPCCOM,NMOD)
               DO 220 J=MYPNT,MYPNT+MYCNT-1
                  IF (IWORD.EQ.DATBUF(J)) GO TO 240
  220             CONTINUE
             END IF
C  NOW SEE IF ALREADY IN EXT-REQ FOR THIS MODULE
            MYCNT=CB(SPCERC,NMOD)
            IF (MYCNT.GT.0) THEN
               MYPNT=CB(SPCEXT,NMOD)
               DO 230 J=MYPNT,MYPNT+MYCNT-1
                  IF (IWORD.EQ.DATBUF(J)) GO TO 240
  230             CONTINUE
             END IF
C  SINCE WE GOT HERE, MUST PUT IT IN DATBUF
            IF (DATPTR.GT.DATSIZ) CALL ERROR (2)
            DATBUF(DATPTR)=IWORD
            DATPTR=DATPTR+1
C  INCREMENT COUNT
            CB(SPCERC,NMOD)=CB(SPCERC,NMOD)+1
CD      CALL C50TO6(IWORD,MNAME)
CD      WRITE(5,1020) MNAME
CD 1020 FORMAT(' GLOBAL REQ:',A)
  240       CONTINUE
         ICOUNT=ICOUNT-K
         IF (ICOUNT.LE.0) GO TO 150
         GO TO 210
C
C  ENTRY BLOCK
C
*NEXT=400
  400    IF (ORDER.GT.1) CALL ERROR (3)
         IF (ORDER.EQ.0) THEN
            NMOD=NMOD+1
C  ARE WE OUT OF BUFFER SPACE
            IF (NMOD.GT.MODSIZ) STOP '?Too many modules (MODSIZ)'
            DO 410 I=1,11
  410          CB(I,NMOD)=0
            MODNAM(NMOD)='?'
            CB(SPCENT,NMOD)=DATPTR
            CB(SPCFIL,NMOD)=IFILE
            ORDER=1
          END IF
         IF (ICOUNT.EQ.0) THEN
C  This module has no entry points.
C  just read and ignore the Relocation Word.
            CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
C  now eat the module until an End block(type=5) is seen.
  420       CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
            ICOUNT=IWORD.AND.HWORD
            ITYPE=IROT(IWORD,18).AND.HWORD
            CALL SKIPBL (ITYPE,ICOUNT)
            IF (EOFF) GO TO 2050
            IF (ITYPE.EQ.5) GO TO 150
            GO TO 420
          END IF
  430    K=MIN(18,ICOUNT)
         CALL RWORD (IWORD)
         IF (EOFF) GO TO 2050
         DO 440 I=1,K
            CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
            CALL ENTADD (IWORD,ERRFLG)
            IF (DATPTR.GT.DATSIZ) CALL ERROR (2)
            DATBUF(DATPTR)=IWORD
            DATPTR=DATPTR+1
C  INCREMENT COUNT
            CB(SPCEPC,NMOD)=CB(SPCEPC,NMOD)+1
CD      CALL C50TO6(IWORD,MNAME)
CD      WRITE(5,2020) MNAME
CD 2020 FORMAT(' ENTRY:',A)
  440       CONTINUE
         ICOUNT=ICOUNT-K
         IF (ICOUNT.LE.0) GO TO 150
         GO TO 430
C
C  END BLOCK
C
*NEXT=500
  500    DO 510 I=1,ICOUNT+1
            CALL RWORD (IWORD)
C      IF (EOFF) GO TO 998
  510       CONTINUE
         RELMOD(MODCNT,IFILE)=RELMOD(MODCNT,IFILE)+1
         GO TO 140
C
C  NAME BLOCK
C
*NEXT=600
  600    IF (ORDER.GT.1) CALL ERROR (4)
         IF (ORDER.EQ.0) THEN
            NMOD=NMOD+1
C  ARE WE OUT OF BUFFER SPACE
            IF (NMOD.GT.MODSIZ) STOP '?Too many modules (MODSIZ)'
            DO 610 I=1,11
  610          CB(I,NMOD)=0
            CB(SPCENT,NMOD)=DATPTR
            CB(SPCFIL,NMOD)=IFILE
            ORDER=1
          END IF
         DO 630 I=1,ICOUNT+1
            CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
            IF (I.EQ.2) THEN
C  Write the MAKLIB command to move this module into LMnnnn.TMP
               CALL C50TO6 (IWORD,MNAME)
               MODNAM(NMOD)= MNAME
               WRITE (LINE, 620) NMOD, RELFS(IFILE), MNAME
  620          FORMAT ('*LM',O4,'.TMP=',A,'/EXTRACT:(',A,')')
               CALL TRIM (LINE, LEN)
               WRITE (MICFIL, 60) LINE(:LEN)
CD      WRITE(5,3020) MNAME
CD 3020 FORMAT(' MODULE:',A)
             END IF
  630       CONTINUE
         GO TO 150
C
C  COMMON BLOCK
C
*NEXT=2000
 2000    IF (ORDER-3) 2020, 2030, 2010
 2010    CALL ERROR (5)
C  FIRST TIME SO SET UP CB
 2020    CB(SPCCOM,NMOD)=DATPTR
         ORDER=3
C   PRESS ON...
 2030    K=MIN(18,ICOUNT)
         CALL RWORD (IWORD)
         IF (EOFF) GO TO 2050
         DO 2040 I=1,K
            CALL RWORD (IWORD)
            IF (EOFF) GO TO 2050
            IF (MOD(I,2).EQ.0) GO TO 2040
            IF (DATPTR.GT.DATSIZ) CALL ERROR (2)
            DATBUF(DATPTR)=IWORD.AND."037777777777
            DATPTR=DATPTR+1
C  INCREMENT COUNT
            CB(SPCCBC,NMOD)=CB(SPCCBC,NMOD)+1
CD      CALL C50TO6(IWORD,MNAME)
CD      WRITE(5,5020) MNAME
CD 5020 FORMAT(' COMMON:',A)
 2040       CONTINUE
         ICOUNT=ICOUNT-K
         IF (ICOUNT.LE.0) GO TO 150
         GO TO 2030
 2050    CLOSE (UNIT=RELFIL)
 2060    CONTINUE
      IF (ERRFLG) CALL EXIT
      IF (NMOD.EQ.0) STOP '?No modules found'
      TYPE 2070, NMOD
 2070 FORMAT (1X,I4,' Modules found')
      RETURN
      END
      SUBROUTINE INCLEV (LEV,CLST)
      INCLUDE 'LIBORD.COM'
      INTEGER LEV, CLST, LEVI, IMOD
C
C  INCREASE CHAIN LEVEL OF EVERYBODY .GE. LEV
      DO 10 IMOD=1,NMOD
C  IGNORE EVERYBODY BUT THIS CLUSTER
         IF (CB(SPCCNM,IMOD).EQ.CLST) THEN
C  GET LEVEL
            LEVI=CB(SPCLEV,IMOD)
            IF (LEVI.GE.LEV) CB(SPCLEV,IMOD)=LEVI+1
          END IF
   10    CONTINUE
      RETURN
      END
      SUBROUTINE LISTO
      INCLUDE 'LIBORD.COM'
      CHARACTER MNAME*6, LBUF*80
      INTEGER K
C
C  INIT
      ENTRY LISTIN
   10 K=1
      LBUF = ' '
      RETURN
C  STORE
      ENTRY LISTSE (MNAME)
      LBUF(K:)=MNAME
      K=K+10
      IF (K.LT.81) RETURN
      WRITE (LSTFIL, 20) LBUF
   20 FORMAT (X,A)
      GO TO 10
C
C  CLEAN UP
      ENTRY LISTCU
      IF (K.EQ.1) RETURN
      WRITE (LSTFIL, 20) LBUF(:K-1)
      GO TO 10
      END
      SUBROUTINE NAMFIX (FILNAM)
C
C  ADD THE EXTENTION ".REL" IF THE EXTENTION IS NOT SPECIFIED
C
      CHARACTER*80 FILNAM
      INTEGER ISTART, IDOT, ILEN
C
      ISTART = INDEX (FILNAM,'>')+ 1
      IF (ISTART.GT.1) THEN
         TYPE 10
   10    FORMAT ('%DIRECTORY cant be handled by MAKLIB')
         FILNAM = FILNAM(ISTART:)
       END IF
      IDOT = INDEX(FILNAM,'.')
      IF (IDOT.EQ.0) THEN
         ILEN = INDEX(FILNAM,' ')
         FILNAM(ILEN:)='.REL'
       END IF
      RETURN
      END
      SUBROUTINE ORDMOD
C  ORDER MODULES
C
C  1.  GET CALLERS AND CALLEE--THIS ELIMINATES UNSATISFIED
C       EXTERNS FROM OTHER LIBRARIES
C  2.  ASSIGN CLUSTER NUMBERS TO EACH CLUSTER
C  3.  SORT EACH CLUSTER
      INCLUDE 'LIBORD.COM'
CD    CHARACTER MNAME*6
      INTEGER LINE(80), I, J, MODULH, MODULS, MFORCE, JFORCE,           
     1   CNTH, CNTS, CPTR, CCNT, IMOD, DATPH, DATPS, NCLUST,            
     2   NEWCLS, CLST, LEVL, MAXLEV
      DATA CHDPTR/1/
C
      WRITE (5, 10)
   10 FORMAT (' Now ordering modules',/)
C  1.  GET CALLERS AND CALLEES
C
C  SET UP THE "HOST" MODULE
      DO 80 MODULH=1,NMOD
C...DEBUG CODE
C      WRITE(5,101) MODNAM(MODULH)
C  101 FORMAT(X,A)
C...END DEBUG CODE
         CB(SPCCHD,MODULH)=CHDPTR
         CB(SPCCHC,MODULH)=0
         CB(SPCCNM,MODULH)=0
         CB(SPCLEV,MODULH)=0
C  NOW SCAN OTHER MODULES
         DO 70 MODULS=1, NMOD
C  DONT SELF SCAN
            IF (MODULS.EQ.MODULH) GO TO 70
C  CHECK FOR CALLERS
            CNTH=CB(SPCEPC,MODULH)
            CNTS=CB(SPCERC,MODULS)
            IF ((CNTH.GT.0).AND.(CNTS.GT.0)) THEN
               DATPH=CB(SPCENT,MODULH)
               DATPS=CB(SPCEXT,MODULS)
               DO 30 I=DATPH,DATPH+CNTH-1
                  DO 20 J=DATPS,DATPS+CNTS-1
                     IF (DATBUF(I).EQ.DATBUF(J)) THEN
C  WE HAVE A MATCH
                        IF (CHDPTR.GT.CHDSIZ) STOP '?CHDAT overflow'
                        CB(SPCCHC,MODULH)=CB(SPCCHC,MODULH)+1
                        CHDAT(CHDMOD,CHDPTR)=MODULS
                        CHDAT(CHDFLG,CHDPTR)=1
                        CHDPTR=CHDPTR+1
                        GO TO 40
                      END IF
   20                CONTINUE
   30             CONTINUE
             END IF
C
C  CHECK FOR CALLEES
   40       CNTH=CB(SPCERC,MODULH)
            CNTS=CB(SPCEPC,MODULS)
            IF ((CNTH.GT.0).AND.(CNTS.GT.0)) THEN
               DATPH=CB(SPCEXT,MODULH)
               DATPS=CB(SPCENT,MODULS)
               DO 60 I=DATPH,DATPH+CNTH-1
                  DO 50 J=DATPS,DATPS+CNTS-1
                     IF (DATBUF(I).EQ.DATBUF(J)) THEN
C  WE HAVE A MATCH
                        IF (CHDPTR.GT.CHDSIZ) STOP '?CHDAT overflow'
                        CB(SPCCHC,MODULH)=CB(SPCCHC,MODULH)+1
                        CHDAT(CHDMOD,CHDPTR)=MODULS
                        CHDAT(CHDFLG,CHDPTR)=2
                        CHDPTR=CHDPTR+1
                        GO TO 70
                      END IF
   50                CONTINUE
   60             CONTINUE
             END IF
   70       CONTINUE
   80    CONTINUE
C
C  2.  ASSIGN CLUSTER NUMBERS
C
*NEXT=200
      NCLUST=0
      DO 230 I=1,NMOD
         IMOD=I
C  IF COUNT=0 JUST ASSIGN CURRENT CLUSTER NUM, INC NCLUST AND GO ON
C  IF CLUSTER NUM NOT EQUAL TO ZERO GO TO NEXT MOD
C  IF CLUSTER NUM EQUAL 0 THEN THIS BEGINS A NEW CLUSTER
         CNTH=CB(SPCCHC,IMOD)
         IF (CNTH.EQ.0) THEN
C  WE HAVE A ZERO COUNT SO JUST ASSIGN NCLUST
            NCLUST=NCLUST+1
            CB(SPCCNM,IMOD)=NCLUST
            MAXLEV = MAX(1,MAXLEV)
            CB(SPCLEV,IMOD)=1
C  CHECK FOR PREVIOUSLY ASSIGNED TO A CLUSTER
          ELSE
            ICLUST=CB(SPCCNM,IMOD)
            IF (ICLUST.EQ.0) THEN
               NCLUST=NCLUST+1
               ICLUST=NCLUST
             END IF
C  NEW CLUSTER OR ONE IN THIS CLUSTER,
C  SWEEP THROUGH ALL MODULES CONNECTED TO THIS ONE ASSIGNING
C  NCLUST AND LEVL
            LEVL = 1
  200       CB(SPCCNM,IMOD)=ICLUST
            IF (CB(SPCLEV,IMOD).LT.LEVL) THEN
               CB(SPCLEV,IMOD)=LEVL
               MAXLEV = MAX(LEVL,MAXLEV)
               CPTR=CB(SPCCHD,IMOD)
               CCNT=CB(SPCCHC,IMOD)
               IF (CCNT.GT.0) THEN
  210             CALL PUSH (CPTR)
                  CALL PUSH (CCNT)
                  CALL PUSH (LEVL)
                  CALL PUSH (IMOD)
C  POINT TO NEXT MODULE
                  IMOD=CHDAT(CHDMOD,CPTR)
C IF NOT ASSIGNED TO CLUSTER TREAT AS A NEW MODULE TO SCAN
C IF GOING DOWN, INCREMENT LEVEL, ELSE ASSUME TOP LEVEL
                  IF (CHDAT(CHDFLG,CPTR).EQ.2) THEN
                     LEVL = LEVL + 1
                   ELSE
                     LEVL = 1
                   END IF
                  IF (CB(SPCCNM,IMOD).EQ.0) GO TO 200
                  IF ((CB(SPCLEV,IMOD).LT.LEVL).AND.(LEVL.LE.MODSIZ))   
     1               GO TO 200
  220             CALL POP (IMOD)
                  CALL POP (LEVL)
                  CALL POP (CCNT)
                  CALL POP (CPTR)
                END IF
             END IF
            IF (CPTR.NE.0) THEN
               CPTR=CPTR+1
               CCNT=CCNT-1
               IF (CCNT.GT.0) GO TO 210
               GO TO 220
             END IF
          END IF
  230    CONTINUE
      WRITE (5, 240) NCLUST
  240 FORMAT (X,I4,' Cluster(s)')
      IF (MAXLEV.GT.NMOD) THEN
         TYPE 250
  250    FORMAT (' %Possible recursion detected')
       ELSE
         TYPE 260, MAXLEV
  260    FORMAT (' Maximum nesting level:',i4)
       END IF
      IF (NCLUST.LE.0) STOP '?No clusters found'
C
C  3.  SORT CLUSTERS
*NEXT=300
C
C  GETNEXTMODULE;
C  ML=1;
C  IF MORE THEN
C  BEGIN
C    IF LEVEL=0 THEN
C    BEGIN
C      INCREASE ALL LEVELS > ML BY 1;
C      SET LEVEL=ML; CPTR=POINTER;
C   1: IF MORE_CALLEES THEN
C      BEGIN
C        IF CALLEE_LEVEL=0 THEN
C        BEGIN
C          PUSH(CPTR); PUSH(ML)
C          INCREASE ALL LEVELS > ML BY 1; ML=ML+1;
C          SET LEVEL=ML;
C          CPTR=THIS MOD. POINTER;
C          GOTO 1
C        END ELSE
C        BEGIN
C          IF CALLEE_LEVEL > ML THEN (*DONE*)
C          ELSE BEGIN
C            WRITE('%RECURSIVE CALL DETECTED');
C            GOTO 1
C          END
C        END
C      END ELSE
C      BEGIN
C        POP(ML); POP(CPTR); IF CPTR <> 0 THEN GOTO 1
C      END
C    END
C
C  CHECK FOR MODULES TO FORCE
      IF (MAXLEV.GT.NMOD) THEN
C  DISCARD PREVIOUS LEVEL DATA, FIND IT ALL THE OLD WAY.
         DO 300 IMOD=1,NMOD
  300       CB(SPCLEV,IMOD)=0
C  ASK USER WHICH MODULES TO FORCE
         CALL RFORCE
         DO 340 CLST=1,NCLUST
            MFORCE=0
            DO 310 JFORCE=1,NFORCE
               IF (FORCE(FRCCNM,JFORCE).EQ.CLST) THEN
                  MFORCE=MFORCE+1
                  LINE(MFORCE)=FORCE(FRCCNT,JFORCE)
                END IF
  310          CONTINUE
            DO 320 I=1,MFORCE
  320          CALL ORDWRK (LINE(I),CLST+0,.TRUE.)
            DO 330 I=1,NMOD
  330          CALL ORDWRK (I+0,CLST+0,.FALSE.)
  340       CONTINUE
       END IF
C  NOW HAVE THINGS SORTED WRITE OUT LIST
C  optimize passes by noting original order for extraction in BUILDF
*NEXT=400
      DO 420 CLST=1,NCLUST
         NEWCLS=1
         DO 410 LEVL=1,MAXLEV
            DO 400 IMOD=1,NMOD
               IF ((CB(SPCCNM,IMOD).EQ.CLST).AND.                       
     1             (CB(SPCLEV,IMOD).EQ.LEVL)) THEN
                  CALL BUILDF (IMOD+0,NEWCLS,CLST+0)
CCC                  GO TO 410
                END IF
  400          CONTINUE
  410       CONTINUE
  420    CONTINUE
C  flag buildf to end
      NEWCLS=-1
      CALL BUILDF (0,NEWCLS,0)
      WRITE (5, 430)
  430 FORMAT ('   Done')
      RETURN
      END
      SUBROUTINE ORDWRK (IMOD,CLST,FRCFLG)
C  THE WORK HORSE ROUTINE FOR ORDER SECTION 3.
      INCLUDE 'LIBORD.COM'
      INTEGER IMOD, JMOD, KMOD, CLST, LEVL, CPTR, CCNT
      LOGICAL FRCFLG
C
      LEVL=1
      IF ((CB(SPCCNM,IMOD).NE.CLST).OR.(CB(SPCLEV,IMOD).NE.0)) RETURN
      CALL INCLEV (LEVL,CLST)
      CB(SPCLEV,IMOD)=LEVL
C  LOOK AT CALLEES
      CPTR=CB(SPCCHD,IMOD)
      CCNT=CB(SPCCHC,IMOD)
   10 IF (CCNT.LE.0) GO TO 50
      IF (CHDAT(CHDFLG,CPTR).NE.2) GO TO 40
      KMOD=JMOD
      JMOD=CHDAT(CHDMOD,CPTR)
      IF (CB(SPCLEV,JMOD).EQ.0) THEN
C  UNTOUCHED CALLEE
         CALL PUSH (CPTR)
         CALL PUSH (CCNT)
         CALL PUSH (LEVL)
         CALL PUSH (JMOD)
         LEVL=LEVL+1
         CALL INCLEV (LEVL,CLST)
         CB(SPCLEV,JMOD)=LEVL
         CPTR=CB(SPCCHD,JMOD)
         CCNT=CB(SPCCHC,JMOD)
         GO TO 10
       END IF
      IF (LEVL.LT.CB(SPCLEV,JMOD)) GO TO 40
C  IF "FORCE"ING THEN SEE IF WE KNOW ABOUT THIS
      IF ((JMOD.EQ.IMOD).AND.FRCFLG) GO TO 40
C  RECURSIVE CALL DETECTED
      WRITE (5, 20) CLST
   20 FORMAT (' %Recursive CALL detected; Cluster #',I4)
      WRITE (5, 30) MODNAM(KMOD),RELFS(CB(SPCFIL,KMOD)),                
     1              MODNAM(JMOD),RELFS(CB(SPCFIL,JMOD))
   30 FORMAT ('  Module:',X,A,' File:',A)
   40 CPTR=CPTR+1
      CCNT=CCNT-1
      GO TO 10
C
   50 CALL POP (JMOD)
      CALL POP (LEVL)
      CALL POP (CCNT)
      CALL POP (CPTR)
      IF (CPTR.NE.0) GO TO 40
      RETURN
      END
      SUBROUTINE PUSH (I)
      INCLUDE 'LIBORD.COM'
      INTEGER I, ISTACK(STKSIZ), STKPTR
      DATA STKPTR/0/
C
      STKPTR=STKPTR+1
      IF (STKPTR.GT.STKSIZ) STOP '?STACK overflow'
      ISTACK(STKPTR)=I
      RETURN
C
      ENTRY POP(I)
      IF (STKPTR.GT.0) THEN
         I=ISTACK(STKPTR)
         STKPTR=STKPTR-1
       ELSE
         I=0
         STKPTR=0
       END IF
      RETURN
      END
      SUBROUTINE RFORCE
C  READ MODULES TO FORCE TO FIX UP RECURSION PROBLEMS
      INCLUDE 'LIBORD.COM'
      CHARACTER MNAME*6, CC*1
      INTEGER I, IMOD, CLST
      DATA NFORCE/0/
C
C  FORMAT FOR FORCE ENTRIES...
C  ORDER NUMBER IN CLUSTER(6)MODULE NUMBER(12)CLUSTER NUMBER(18)
C
      WRITE (5, 10)
   10 FORMAT (' Module names to force. 1 per line, extra cr when done')
   20 WRITE (5, 30)
   30 FORMAT (' *',$)
      READ (5, 40) MNAME
   40 FORMAT (A)
      IF (MNAME.EQ.' ') RETURN
      DO 50 I=1,6
         CC = MNAME(I:I)
         IF ((CC.GE.'a').AND.(CC.LE.'z')) MNAME(I:I)=                   
     1        CHAR(ICHAR(CC)-ICHAR('a')+ICHAR('A'))
   50    CONTINUE
C  SWEEP THROUGH MODULES LOOKING FOR THIS ONE
      DO 60 IMOD=1,NMOD
         IF (MODNAM(IMOD).EQ.MNAME) GO TO 80
   60    CONTINUE
      WRITE (5, 70)
   70 FORMAT (' ?Module name not found')
      GO TO 20
C  FOUND
   80 CLST=CB(SPCCNM,IMOD)
      NFORCE=NFORCE+1
      IF (NFORCE.GT.FRCSIZ) STOP '?Too many FORCE modules'
      FORCE(FRCCNT,NFORCE)=1
      FORCE(FRCCNM,NFORCE)=CLST
      FORCE(FRCMOD,NFORCE)=IMOD
C  NOW PUT IN ORDER NUMBER
      DO 90 I=1,NFORCE-1
         IF (FORCE(FRCCNM,I).EQ.CLST)                                   
     1       FORCE(FRCCNT,NFORCE)=FORCE(FRCCNT,NFORCE)+1
   90    CONTINUE
      GO TO 20
      END
      SUBROUTINE RWORD (IWORD)
      INCLUDE 'LIBORD.COM'
      INTEGER IWORD, I, IBUF(BUFSIZ), IPTR
      LOGICAL EOFN
      DATA IPTR,EOFN/BUFSIZ,.TRUE./
C
      IF (IPTR.GE.BUFSIZ) THEN
C
         DO 10 I=1,BUFSIZ
   10       IBUF(I)=0
C
         EOFF=EOFN
         IF (EOFF) RETURN
         READ (RELFIL, END=20) IBUF
         GO TO 30
C EOF (next time)
   20    EOFN=.TRUE.
   30    CONTINUE
         IPTR=0
       END IF
      IPTR=IPTR+1
      IWORD=IBUF(IPTR)
      RETURN
C
      ENTRY FINIT
      IPTR=BUFSIZ
      EOFN=.FALSE.
      RETURN
      END
      SUBROUTINE SKIPBL (ITYPE,ICOUNT)
C  SKIP A BLOCK
C  MAP ITYPE TO SHORT,LONG,ASCII OR ERROR
C  DIE ON ERROR
      INCLUDE 'LIBORD.COM'
      PARAMETER (NUMTYP=17)
      INTEGER I, J, K, ITYPE, JTYPE, ICOUNT, IWORD, KTYPE(3,NUMTYP)
      DATA KTYPE/"0000,"0012,1, "0014,"0014,2, "0015,"0023,1,           
     1           "0024,"0024,1, "0037,"0037,1, "0100,"0100,1,           
     2           "0776,"0777,2, "1000,"1004,2, "1010,"1037,2,           
     3           "1042,"1043,1, "1044,"1045,2, "1050,"1052,2,           
     4           "1060,"1060,2, "1070,"1070,2, "1072,"1072,2,           
     5           "1100,"1107,2, "1120,"1130,2/
C
      IF (ITYPE.GT."3777) THEN
         JTYPE=3
       ELSE
         DO 10 I=1,NUMTYP
            IF ((ITYPE.GE.KTYPE(1,I))                                   
     1     .AND.(ITYPE.LE.KTYPE(2,I))) GO TO 30
   10       CONTINUE
         WRITE (5, 20) ITYPE
   20    FORMAT (' ?Bad REL type: ',O6)
         CALL EXIT
C
   30    JTYPE = KTYPE(3,I)
       END IF
      GO TO (100, 200, 300), JTYPE
*NEXT=100
C  SHORT TYPE
  100 IF (ICOUNT.EQ.0) RETURN
      I=ICOUNT
  110 K=MIN(18,I)
      DO 120 J=1,K+1
         CALL RWORD (IWORD)
         IF (EOFF) RETURN
  120    CONTINUE
      I=I-K
      IF (I.GT.0) GO TO 110
      RETURN
*NEXT=200
C  LONG TYPE
  200 DO 210 I=1,ICOUNT
         CALL RWORD (IWORD)
         IF (EOFF) RETURN
  210    CONTINUE
      RETURN
*NEXT=300
C  ASCIZ TYPE
  300 CALL RWORD (IWORD)
      IF (EOFF) RETURN
C  SEE IF WE HAVE A NULL IN THE WORD
CCC      IF ((IWORD.AND."774000000000).EQ.0) RETURN
CCC      IF ((IWORD.AND."003760000000).EQ.0) RETURN
CCC      IF ((IWORD.AND."000017700000).EQ.0) RETURN
CCC      IF ((IWORD.AND."000000077400).EQ.0) RETURN
      IF ((IWORD.AND."000000000376).EQ.0) RETURN
      GO TO 300
      END
      SUBROUTINE TRIM (STRING,LENGTH)
      CHARACTER*(*) STRING
      INTEGER LENGTH,CUR,NEXT,LAST
C remove all blanks from input string, return length
      LAST = LEN(STRING)
      DO 20 CUR=1,LAST
         IF (STRING(CUR:CUR).EQ.' ') THEN
            DO 10 NEXT = CUR+1, LAST
               IF (STRING(NEXT:NEXT).NE.' ') THEN
                  STRING(CUR:)= STRING(NEXT:)
                  GO TO 20
                END IF
   10          CONTINUE
            LENGTH = CUR-1
            RETURN
          END IF
   20    CONTINUE
      LENGTH = LAST
      RETURN
      END
