 main()
	{  enum { a };
	   int  a();
	}
#if 0
will be correctly flagged as a duplicate symbol; but then the compiler will
go into an infinite loop generating internal error messages from ridlsym().
What's happened is, dodecl() calls funchk() calls uniqsym() calls symqcreat(),
and symqcreat(), assuming that the ident symbol returned by the lexer is
linked into the global symbol list, calls makelsym() to unlink it and put
it on the local list.  However, the ident symbol returned by the lexer is
in fact already on the local symbol list, so makelsym() in fact screws up
the linkage entirely, returning with the local symbol tail pointing back
to itself.  Then when ridlsym tries to traverse that list, it doesn't
have an end.  I'm sure some no-longer-safe assumption is being made about
the symbol created by the lexer, but I'm not sure what assumptions can in
fact be made there.  Would you be able to take a look?
				Thanks,
				Michael

#endif
