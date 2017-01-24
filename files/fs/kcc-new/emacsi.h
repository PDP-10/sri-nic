/* EMACSI.H - header file for EMACS Interface module
**	Copyright (c) 1989 by Ken Harrenstien, SRI International
*/

struct emacsi {		/* Structure passed to emacsi() */
	int em_fun;	/* Function to perform */
	int em_arg;	/* General-purpose numerical arg */
	char *em_cp;	/* Char ptr to string arg */
	int em_len;	/* Length of string arg */
	char *em_error;	/* Ret: Error string pointer, if not NULL */
	int em_pt;	/* Ret: Char # of "point"; 0 means before 1st char */
	int em_z;	/* Ret: # chars in current buffer */
	int em_modiff;	/* Ret: Non-zero if buffer has been modified */
	int em_rdonly;	/* Ret: Non-zero if buffer is read-only */
};

#define EMACS_INIT 0	/* Initialize fork and mem (em_arg, em_cp, em_len) */
#define EMACS_QUIT 1	/* Kill editor fork (no args) */
#define EMACS_EDIT 2	/* Give control to EMACS, start editing (no args) */
#define EMACS_MOVE 3	/* Move point relatively (by em_arg) */
#define EMACS_JUMP 4	/* Move point absolutely (to em_arg) */
#define EMACS_INSERT 5	/* Insert text (em_cp, em_len) at point */
#define EMACS_DELETE 6	/* Delete em_arg chars starting at point */
#define EMACS_READ 7	/* Read buffer (into em_cp, em_len) */
#define EMACS_CLEAR 8	/* Clear buffer (no args) */

extern int emacsi(/* struct emacsi *em */);
