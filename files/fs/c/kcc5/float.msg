
ANSI EXTENSION - "long double" type

	The ANSI draft standard (X3J11) proposes a new float data type
called "long double" which is expected to have precision/range at
least as great as "double": float <= double <= long double.  The need
to assign some hardware format to this new type raises the issue of
the most suitable mapping for all three types, and I'd like some comments
on this.

The PDP-10 floating point formats are:

			Words	Precision (digits)	Range
Single-precision	1	8		1.5e-39 to 1.7e38
Double-precision	2	18		1.5e-39 to 1.7e38
"G"-format		2	17		2.8e-309 to 9.0e307

The first two are the normal hardware formats that have been around since
the KI-10 (the KA-10 uses a software format for double precision).  The
third is a new one only available in KL10s with microcode version 271
or up; KS10s (DEC-2020) don't have this.  Note the loss of significance
is exchanged for a gain in range.

The plausible mappings appear to be:
	    Float  Double  Long Double
	A. single  single  double	Fast arith, all CPUs (or non-KL)
	B. single  single  G-fmt	Fast arith, KL only
	C. single  double  double	All CPUs (or non-KL)
	D. single  double  G-fmt	KL only
	E. single  G-fmt   G-fmt	KL only

C considerations:
	C according to H&S requires that all operands of type "float"
be converted to "double".  ANSI C will permit them to remain "float",
however.  All C library routines, both now and in the future, require
"double" arguments and return "double" results.

Timing considerations:
	I ran some timing tests on SRI-NIC (a DEC-2065) and have summarized
the results in this table (times are in usec):

		ADD	SUB	MUL	DIV
	Single	1.9	2.2	2.5	4.6
	Double	2.5	3.2	4.2	8.7
	G-fmt	7.5	8.2	5.8	9.8

Exact times will depend on the operands used; the values chosen were
picked to give a reasonable number of bit transitions.
