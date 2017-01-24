Arrrgh.  I suppose I should have expected that the first bug would show
up just after I made the first tape dump of the distribution.  Fortunately
I hadn't mailed it yet, so I got a chance to do it all over again.

Thanks for the simple test case; that bug is fixed as of KCC 601.
(cregok() in CCCREG was being unduly optimistic.  The code it now
produces wins by changing all of the regs to 5 (or whatever) instead
of stopping halfway.)

The KCC-5 source distribution has been updated to reflect the fix.
