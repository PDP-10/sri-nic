25-Jul-88 03:17:11-PDT,5760;000000000005
Return-Path: <LARSON@KL.SRI.COM>
Received: from KL.SRI.COM by SRI-NIC.ARPA with TCP; Mon, 25 Jul 88 03:17:05 PDT
Date: Mon, 25 Jul 88 03:17:11 PDT
From: Alan Larson <LARSON@KL.SRI.COM>
Subject: kcc strangeness
To: klh@sri-nic.arpa
Message-ID: <12417087202.12.LARSON@KL.SRI.COM>

  I have a C program that I was trying to move to the KL, and it seems
to get rather different results than it did on the vax/vms or the pyramid
unix systems.  The program is kl:<larson>o23.c and kl:<larson>or.c, and
requires the files wa6azp.sit, mode.dat, and kepler.dat.  (These are all
in the same directory.)

  I am including a few lines from the run on KL and the pyramid.  If you
have any ideas, I would be interested.  By the way, on the Macintosh, it
doesn't even come as close as the KL, but I think there are problems with
lightspeed C.

	Alan


UNIX:1> o23
N3EMO Orbit Simulator  v2.3
Available satellites:
        a) oscar-13
Letter or satellite name :a
Site name :wa6azp
Start date (UTC) (Month Day Year) :7 23 88
Starting Hour (UTC) :24
Duration (Days) :1
Time Step (Minutes) :5
Output file (RETURN for TTY) :
oscar-13 Element Set 0
WA6AZP (Grid CM87  Menlo Park)

Doppler calculated for freq = 145.812500 MHz
Saturday  23 Jul 88  ----Orbit # 83-----
 P.D.T.   Az   El  Doppler  Range  Height  Lat  Long  Phase(256)
1705:00  111   10    -191   39080   34279   -4    60    95    B
1710:00  111   11    -170   39185   34503   -4    61    97    B
1715:00  111   12    -149   39277   34712   -3    62    99    L
1720:00  111   13    -128   39356   34908   -2    62   101    L
1725:00  112   14    -107   39422   35091   -2    63   102    L
1730:00  112   16     -87   39475   35260   -1    64   104    L
1735:00  112   17     -67   39516   35416   -0    65   106    L
1740:00  112   18     -47   39545   35558    0    66   108    L
1745:00  112   19     -27   39561   35687    1    67   110    L
1750:00  112   20      -7   39566   35803    1    68   112    L
1755:00  112   21      13   39558   35906    2    68   114    L
1800:00  112   22      32   39538   35996    3    69   115    L
1805:00  113   24      51   39507   36073    3    70   117    L
1810:00  113   25      70   39463   36137    4    71   119    L
1815:00  113   26      89   39408   36188    4    72   121    L
1820:00  113   27     108   39342   36226    5    73   123    L
1825:00  113   28     127   39263   36251    6    74   125    L
1830:00  113   29     146   39173   36263    6    74   127    L
1832:12  113   30     159   39130   36264    7    75   128    L    Apogee
1835:00  114   30     168   39072   36262    7    75   129    L
1840:00  114   31     183   38959   36249    8    76   130    L
1845:00  114   33     202   38835   36222    8    77   132    L
1850:00  114   34     220   38699   36183    9    78   134    L
1855:00  114   35     239   38552   36130    9    79   136    L
1900:00  114   36     257   38393   36065   10    80   138    L
1905:00  115   37     276   38223   35986   11    80   140    L
1910:00  115   38     294   38042   35895   11    81   142    L
1915:00  115   39     313   37849   35791   12    82   143    L
1920:00  115   40     331   37645   35673   12    83   145    L
1925:00  115   41     350   37429   35542   13    84   147    L


------------------------------------

!orbit
N3EMO Orbit Simulator  v2.3
Available satellites:
        a) oscar-13
Letter or satellite name :a
Site name :wa6azp
Start date (UTC) (Month Day Year) :7 23 88
Starting Hour (UTC) :24
Duration (Days) :1
Time Step (Minutes) :5
Output file (RETURN for TTY) :
oscar-13 Element Set 0
WA6AZP (Grid CM87  Menlo Park)

Doppler calculated for freq = 145.812500 MHz
Saturday  23 Jul 88  ----Orbit # 83-----
 P.D.T.   Az   El  Doppler  Range  Height  Lat  Long  Phase(256)
1700:00  110   15    -345   38318   34042    0    63    93    B
1705:00  110   15    -321   38516   34279    0    63    95    B
1710:00  110   16    -298   38700   34503    0    63    97    B
1715:00  111   16    -276   38870   34712    0    64    99    L
1720:00  111   16    -253   39026   34908    0    64   101    L
1725:00  111   17    -231   39168   35091    0    65   102    L
1730:00  112   17    -209   39297   35260    0    65   104    L
1735:00  112   18    -187   39412   35416    0    66   106    L
1740:00  112   18    -165   39514   35558    0    66   108    L
1745:00  113   19     -95   39573   35687    0    67   110    L
1750:00  113   20      -7   39577   35803    1    68   112    L
1755:00  113   21      13   39569   35906    2    69   114    L
1800:00  113   22      32   39549   35996    2    70   115    L
1805:00  113   23      51   39518   36073    3    70   117    L
1810:00  113   25      70   39474   36137    3    71   119    L
1815:00  114   26      89   39419   36188    4    72   121    L
1820:00  114   27     108   39353   36226    5    73   123    L
1825:00  114   28     127   39274   36251    5    74   125    L
1830:00  114   29     146   39184   36263    6    75   127    L
1832:12  114   30     159   39141   36264    6    75   128    L    Apogee

1845:00  113   33  -51296   38867   36264    9    77   132    L
1850:00  113   34     168   38763   36264   10    77   134    L
1855:00  112   35     164   38662   36264   11    78   136    L
1900:00  112   36     161   38563   36264   12    78   138    L
1905:00  111   37     157   38466   36264   13    79   140    L
1910:00  110   39     153   38372   36264   14    80   142    L
1915:00  110   40     149   38280   36264   15    80   143    L
1920:00  109   41     145   38190   36264   16    81   145    L
1925:00  108   42     140   38104   36264   17    81   147    L


Strange.
-------
