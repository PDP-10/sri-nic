/*
 * CENDPS.C -- companion module to CPSECT.MAC (PSECT support for KCC)
 *
 * This module defines the tie-off symbols needed by CRT.C to
 * determine where the ends of the various psects are (the JOBDAT
 * info LINK provides isn't sufficient).
 *
 * This module is loaded via a /USERLIB switch by CPSECT to make
 * sure it is the very last module loaded.  See CPSECT.MAC for more
 * details.
 */

int `Z$DATA`;				/* Last word in "loseg" */

void `Z$CODE`() {}			/* Last word in "hiseg" */

/* That's all we need to do here. */
