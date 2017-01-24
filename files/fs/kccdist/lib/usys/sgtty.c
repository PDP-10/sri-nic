/*
 *	SGTTY
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "sgtty.h"

/*
 *	Get TTY characteristics
 */

gtty(fd, buf)
int fd;
struct sgttyb *buf;
{
    return ioctl(fd, TIOCGETP, buf);
}

/*
 *	Set TTY characteristics
 */

stty(fd, buf)
int fd;
struct sgttyb *buf;
{
    return ioctl(fd, TIOCSETP, buf);
}
