SRCS	:= sheet.c fileio.c interactive.c
HDRS	:= sheet.h fileio.h interactive.h

OBJS	= $(SRCS:.c=.o)

CC	= gcc
CFLAGS	= -Wall -Werror -Wmissing-prototypes -Wstrict-prototypes -O2 -std=gnu89

vsv : $(OBJS)
	$(CC)  $(CFLAGS)  -o $@	 $(OBJS)

sheet.o :
	$(CC)  $(CFLAGS)  -o $@ -c $(@:.o=.c)
fileio.o :
	$(CC)  $(CFLAGS)  -o $@ -c $(@:.o=.c)
interactive.o :
	$(CC)  $(CFLAGS)  -o $@ -c $(@:.o=.c)

.PHONY:	clean debug

debug : clean
	CFLAGS += -Og -g
	vsv

clean :
	rm -f $(OBJS) vsv
