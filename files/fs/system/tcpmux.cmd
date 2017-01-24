; This file is used by the TCPMUX program on TCP port 1
;
; Format is a service name, and the .exe to run for it.
; Services are started with primary i/o set to the tcp connection.
; If the first character is a "+", then the server returns a positive
; reply before starting service, otherwise the service must provide it.
; 
+finger		sys:finger.exe
+hostname	ps:<namser>tcphst.exe
+whois		sys:whois.exe
+nicname	sys:whois.exe
+tstats		sys:tstats.exe
+cookie		sys:cookie.exe
+snigle		sys:snigle.exe
+wwv		sys:wwv.exe
