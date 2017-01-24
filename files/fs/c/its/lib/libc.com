kl:
del *.rel.*
xcc -m -c cpu
xcc -c crt ctype memstr onexit string syscal malloc abort strerr
ku:
del *.rel.*
xcc -c urt urtsud exit sbrk sigdat uiodat stat open dup fcntl lseek read write close wait 
kio:
del *.rel.*
xcc -c fopen freope setbuf ungetc fflush fgetc fputc fputs scanf printf ftell fseek fclose sopen cleanu
km:
del *.rel.*
xcc -c modf floor
kits:
del *.rel.*
xcc -c humble
