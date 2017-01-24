;This is the NFS access control file

;only allow request from these nets or hosts (netnum, mask)
NET 192.33.33.0 377 377 377 0	;NICNET
NET 10.7.0.2 377 377 377 377	;SUN.COM
NET 130.93.1.2 377 377 377 377  ;fernwood

;directory access controls
192.33.33.101		R	PS:<VIVIAN>
192.33.33.102		R	PS:<KLH>
192.33.33.103		R	PS:<FRED>
192.33.33.104		R	PS:<MKL>	
192.33.33.105		RW	PS:<IAN>
192.33.33.106		R	PS:<MIMI>
192.33.33.106		R	PS:<SKAHN>
192.33.33.107		R	PS:<LIM>
192.33.33.108		R	PS:<HSS>
192.33.33.105		RW	PS:<NICPROG>
