 1-Oct-87 19:10:28-PDT,487;000000000001
Mail-From: KLH created at  1-Oct-87 19:10:24
Date: Thu, 1 Oct 87 19:10:24 PDT
From: Ken Harrenstien <KLH@SRI-NIC.ARPA>
Subject: MIDAS eof bug
To: ian@SRI-NIC.ARPA
cc: klh@SRI-NIC.ARPA
Message-ID: <12339141818.22.KLH@SRI-NIC.ARPA>

The chan 22 int happens when the SEMIC comment flushing loop runs over EOF
and keeps going.  That happens when the last line in the file ends in
a semicolon comment which terminates in EOF rather than EOL.  A real
MIDAS bug.  Oh well.
-------
