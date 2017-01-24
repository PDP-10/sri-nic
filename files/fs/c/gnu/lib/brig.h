 5-May-84 14:23:38-EDT,869;000000000001
Received: from MIT-MC by MIT-OZ via Chaosnet; 5 May 84 14:23-EDT
Received: From bostonu.csnet by csnet-relay;  5 May 84 14:13 EDT
Received: by csvaxa.ARPA (4.12/4.7)
	id AA23844; Sat, 5 May 84 13:56:15 edt
Date: Sat, 5 May 84 13:56:15 edt
From: God <root%bostonu.csnet@csnet-relay.arpa>
Message-Id: <8405051756.AA23844@csvaxa.ARPA>
To: rms@mit-mc.arpa
Subject: brig.h

/*
 *	The bucket structure. bfirst is the index that the first element
 *	in bdata refers to in absolute terms.
 */
#define buck_t struct buck_s
struct buck_s {
	buck_t	*bnext ;
	unsigned int bfirst ;
	char	*bdata ;
} ;
/*
 *	The head structure as returned by bopen()
 */
struct bhead_s {
	buck_t	*bhead ;		/* first bucket			*/
	unsigned short	bperb ;		/* bytes per bucket item	*/
	unsigned short	nperb ;		/* bucket items per bucket	*/
} ;
#define bhead_t struct bhead_s
