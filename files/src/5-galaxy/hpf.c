10-May-89 14:36:24-PDT,845;000000000001
Return-Path: <geoff@fernwood.MPK.CA.US>
Received: from fernwood.MPK.CA.US by SRI-NIC.ARPA with TCP; Wed, 10 May 89 14:36:08 PDT
Received: by fernwood.MPK.CA.US at Wed, 10 May 89 14:35:39 -0700.
	(5.61a+/IDA-1.2.8-21) id AA14497 for mkl@sri-nic.arpa
Date: Wed, 10 May 89 14:35:39 -0700
From: Geoff Goodfellow <geoff@fernwood.mpk.ca.us>
Message-Id: <8905102135.AA14497@fernwood.MPK.CA.US>
To: mkl@sri-nic.arpa

/*

laserjet iid filter

*/

#include <stdio.h>

main()
{
  int c;

  /* reset */
  /* left column pos 5 */
  /* change pitch */
  /* wrap lines */
  /* convert lf to crlf */
  /* select lower tray */
  puts("\033E\033&a5L\033&k10.91H\033&s0C\033&k2G\033&l4H",stdout);
  fflush(stdout);

  while ((c = getchar()) != EOF) putchar(c);

  puts("\033&l0H",stdout);    /* eject page */
  fflush(stdout);
}
