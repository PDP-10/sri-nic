/*  */
/*	list - ASCII source lister  */
/*		Gerry Barksdale (August 1975)  */
/*  */
int FF 014;	/* Form Feed  */
char *modtime;
int status[18];
int wbuf[259] 1;
int rbuf[259];
char margin[50] "    ";
int mval 4;	/*margin value  */
int line, page, col;
int head, number;
int lineperpage 56;
int tabv 4;
int printer 1;	/*output is intended for printer (FF at end of page)  */
int lineno 1;	/*line numbering desired  */
int maxcol 74;	/*text width - margin - number  */
int lines;	/*number of lines printed on this page  */
int beginpg 1;
int endpg 32767;
int print, more, fold;
char *ttyfil "/dev/ttyx";
int filter 0;   /* if 1 means used as filter */
int browse 0;   /* if 1 means we want to browse text */

main(argc,argv)
char **argv;
{
	register int c, com, n;

	if (argv[0][0] == 'b' ){
		browse = 1;
		if ( argc > 0) {
			printer = 0;
			lineperpage = 22;
			margin[0] = 0;
			printf("Press <return> to see next page.\n");
			maxcol = 72; 
		};
	}

	while (--argc >  0) {
		argv++;
		if(**argv == '-') {
			while(*++*argv) {
				com = **argv;	/*save command  */
				c = 0;
				while((n = *++*argv) >= '0' && (n <= '9'))
					c = c*10 + n - '0';
				*--*argv;

				switch (com) {

				case 'm':	/* set left margin  */
					if(c > 49)
						c = 49;
					maxcol =+ (c-mval);
					mval = c;
					margin[c] = 0;
					while (c-- > 0)
						margin[c] = ' ';
					break;

				case 't':	/* set tab value  */
					if( (tabv = c) < 1 )
						tabv = 1;
					break;

				case 'c':	/* CRT display mode  */
					printer = 0;
					lineperpage = 22;
					margin[0] = 0;
					break;

				case 'l':	/* set page length  */
					if( (lineperpage = c) < 1)
						lineperpage = 60;
					break;

				case 'n':	/* no line numbers  */
					maxcol =+ 4;
					lineno = 0;
					break;

				case 'w':	/* page width (cols)  */
					maxcol = c - 4*lineno - mval;
					break;

				case '+':	/* start printing with this page  */
					beginpg = c;
					break;

				case ':':	/* stop printing after this page  */
					endpg = c;
					break;

				case 'p':	/* direct to /dev/lp  */
					if((wbuf[0] = open("/dev/lp",1)) < 0) {
						wbuf[0] = 1;
						printf("Cannot open printer\n");
						fflush(wbuf);
						return;
					}
					break;

				case 'f':	/*	fold output  */
					fold++;
					break;

				}
			}
		}   
		else {
			goto list;
		}
	}
list:   
	if ( argc == 0 ) {
		/* input is from standard input i.e. filter */
		if ( ttyn(0) != 'x') exit();
		time(&status[16]);
		rbuf[0] = dup(0);  /* filter input  */
		close(0);
		ttyfil[8] = ttyn(1);
		if (ttyfil[8] == 'x')  filter = 1;
		if(!filter) open(ttyfil,0);
		if( browse ) {
			printer = 0;
			lineperpage = 22;
			maxcol = 72;
			margin[0] = 0;          
		}
		doit(*argv, 0);
		fflush(wbuf);
		exit();
	}

	while ( argc--  > 0 ){
		if((rbuf[0]=open(*argv,0)) < 0)
			goto next;
		fstat(rbuf[0],status);
		doit(*argv, 1);
		fflush(wbuf);
	next:   argv++;
	};
	fflush(wbuf);
}
doit(argv,argc) char *argv; int argc;
{
	register int c, com, n;
	modtime = ctime(&status[16]);
	fflush(wbuf);
	for(c=1;c<4;c++)
		rbuf[c] = 0;	/*initialize getc pointers  */
	line=1;
	lines = 0;
	page=0;
	head=1;	/*force heading on first line  */
	more = print = 1;
	number = 1;
	while (((c=getc(rbuf)) >= 0) && more) {
		while(head) {
			lines = head = 0;
			print = ((++page < beginpg) ? 0 : 1);
			if(page > endpg)
				more = print = 0;
			if(!print)
				break;
			if(printer) {
				putc(FF,wbuf);
			} 
			else {
				fflush(wbuf);
				if(!filter) read(0,status,1);       /*wait for a char to release page  */
			}
			if ( argc )
			printf("%s%s    Page %d    %s\n",
			margin, argv,page,modtime);
			else
			printf("%s    Page %d    %s\n",
			margin, page,modtime);
		}
		if(print && number) {
			printf("%s",margin);
			number = 0;
			if(lineno) {
				printf("%3d ",line);
			}
		}
		if(!print && (c != '\n'))
gobble:
			while((c=getc(rbuf)) != '\n') ; /*gobble  */
		switch (c) {

		case '\t':
			do
			    putc(' ',wbuf);
			while(col++ % tabv -tabv +1);
			break;

		case '\n':
			if(print)	putc(c,wbuf);
			col = 0;
			line++;
			if(++lines >= lineperpage)
				head++;
			number++;
			break;

		default:
			if((c  =& 0177) >= ' ') {	/*it's printable  */
				putc(c,wbuf);
				if(++col >= maxcol) {
					if( ! fold) {
						putc(0177,wbuf);
						goto gobble;
					}
					col = 0;
					printf("\n%s    ",margin);	/*fold line  */
					lines++;
				}
			}
		}
	}
	close(rbuf[0]);
	if(printer)
		putc(FF,wbuf);
	fflush(wbuf);
}
putchar(c){

	putc(c,wbuf);	
}	/*redirect printf() output  */
