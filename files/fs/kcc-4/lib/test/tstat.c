#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"

main(argc, argv)
int argc;
char *argv[];
{
    struct stat foo;

    if (argc < 2) {
	fputs("usage is: stest filename\n", stderr);
	exit(0);
    }

    if (stat(argv[1], &foo) < 0) {
	fputs("couldn't stat", stderr);
	exit(0);
    } else {
	printf("file: %s\n", argv[1]);
	printf("device (st_dev): %o\n", foo.st_dev);
	printf("disk address (st_ino): %o\n", foo.st_ino);
	printf("mode/protection (st_mode): %o\n", foo.st_mode);
	printf("number of links (st_nlink): %o\n", foo.st_nlink);
	printf("user id of owner (st_uid): %o\n", foo.st_uid);
	printf("group id of owner (st_gid): %o\n", foo.st_gid);
	printf("whatever this is (st_rdev): %o\n", foo.st_rdev);
	printf("size of file (st_size): %o\n", foo.st_size);
	printf("last access file (st_atime): %o\n", foo.st_atime);
	printf("last modify time (st_mtime): %o\n", foo.st_mtime);
	printf("last status change time (st_ctime): %o\n", foo.st_ctime);
    }
}
