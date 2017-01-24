/*
** cccreg.h - definitions for changereg() and friends
** David Eppstein / Stanford University / 8-Jul-85
*/


/*
** Data type for storing effect of op on reg.
**
** This describes the return value of rchange(), and says in what manner the
** op changes the register: whether it is unaffected, set solely by the op,
** changed by both the op and its previous contents, or whether all registers
** were changed unpredictably; and, if the register was changed, whether this
** was as a single reg or a register pair.
*/

enum rmod {
    REG_SAME,		/* op affects memory or it skips but changes nothing */
    REG_SET,		/* op changes reg based only on the other operand */
    REG_SET_DBL,	/* op changes reg based on doubleword operand */
    REG_CHANGED,	/* op changes reg based on it and other operand */
    REG_CHG_DBL,	/* op changes reg based on it and double operand */
    DBL_SAME,		/* op uses reg pair but doesn't change it */
    DBL_SET,		/* op changes reg pair based only on other operand */
    DBL_SET_SGL,	/* op changes reg pair based on singleword operand */
    DBL_CHANGED,	/* op changes reg pair based also on it */
    DBL_CHG_SGL,	/* op changes pair based on it and single operand */
    UNKNOWN_CHANGES	/* all registers changed in unknown fashion */
} rchange();
