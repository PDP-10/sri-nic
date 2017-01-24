/*
** ccsite.h - site-dependent declarations for KCC
*/

#include "c-env.h"

#ifndef HF_PREFIX
#if SYS_10X
#define HF_PREFIX "<C>"
#endif
#if SYS_ITS
#define HF_PREFIX "KC;"
#endif
#if SYS_T20
#define HF_PREFIX "C:"
#endif
#endif

#ifndef HF_POSTFIX
#if SYS_T20+SYS_10X+SYS_ITS
#define HF_POSTFIX ""
#endif
#if SYS_WAITS
#define HF_POSTFIX "[SYS,KCC]"	/* Just a guess - KLH */
#endif
#endif

/* Need to fix this for other assembler variants */
#ifndef ASM_HDR_FILE
#if SYS_T20
#define ASM_HDR_FILE "C:C-HDR.FAI"
#endif
#if SYS_10X
#define ASM_HDR_FILE "<C>C-HDR.FAI"
#endif
#if SYS_ITS
#define ASM_HDR_FILE "KC;C-HDR FAI"
#endif
#if SYS_WAITS
#define ASM_HDR_FILE "C-HDR.FAI[SYS,KCC]"
#endif
#endif
