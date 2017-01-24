/* TPP1.C - Test level 1 and 2 input (PP trigraph processing and EOL quoting)
**	Compile with -E to get results on standard output.
**	There should always be an error message about the file not ending
**	in EOL.
*/

"Testing all trigraphs:	??=??(??/??)??'??<??!??>??- "
"Prev string should be: #[\]^{|}~ "
"Checking long runs ending in trigraphs: 1???= 2????= 3?????="
"Testing non-trigraph seqs: 1? 2?? 3??? 4???? 5?????"
"More non-trigraph seqs:  1?= 2??$ 3???* 4????@"

"Testing line quoting, this li\
ne should be unbroken"
"\This\\ \line\\ \has\\ \one&two\\ \backslashes\\ \around\\ \each\\ \word\\"
"This line contains 4 backslashes '\\\\' and\
		is broken with two tabs"

"Now generate error msg about EOF"\
