The new MM prints 'No such local user as ""' if you type in
a garbage address.  Someone changed the code at GETUSR+nn
from:
	      MOVE A,[POINT 7,ADRSTR(U)] ;Local addr, not user, try forwarding
	      CALL CHKFWD	;Did we find it?
	       IFNSK.
		 MOVEI A,ADRSTR(U) ;Return error
		 ERROR <No such local user as "%1R">
	       ENDIF.
to:
	      MOVE A,[POINT 7,ADRSTR(U)] ;Local addr, not user, try forwarding
	      CALL CHKFWD	;Did we find it?
		 ERROR <No such local user as "%1R">

but the ETYPE routine doesn't like indexed pointers...
