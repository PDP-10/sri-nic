@enable
@cd system:
@noerror
;kill of all hosts except net 10 and 26
@nichost
;
@teco
*;yhosts.txt
*jr; DoD Internet Host Table; ******** SRI-NIC Local Host Table ********
*jr, 10.0.0.51, 10.0.0.51, 192.33.33.20
*jr, 10.4.0.11 : :
*jr10.4.0.2,
*exhosts.txt.0

@append hosts.time hosts.txt
@init hosts
@delete hosts.txt.*,
@keep 1
@
@
