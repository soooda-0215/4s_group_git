## PROG.c
##
## edit3 by y.katsuta  2019.07.22
## edit2 by y.katsuta  2018.06.01
## edit  by y.katsuta  2013.10.02
##
## make PROG=h8sample clean     <-- 1 KAI ME NOMI
## make PROG=h8sample
##
## make PROG=h8sample LDF=-lm OBJSA=rs232c.o
## make PROG=h8sample LDF="-lm rs232c.a"
## make PROG=h8sample MINICOM=1 LDF="-lm rs232c.a"
##
## make pre PROG=h8sample
##
##

ifndef PROG
PROG=h8sample
endif

ifdef MINICOM
LDFMINICOM=-DMINICOM
endif

H8DIR = $(HOME)/.wine/drive_c/H8
MKLOCAL = $(H8DIR)/local
CBPATH = $(H8DIR)/app

CC = wine $(MKLOCAL)/bin/h8300-elf-gcc.exe $(LDFMINICOM)
CFLAGS = -Wall -B $(MKLOCAL)/h8300-elf -I$(CBPATH)/. -mint32 -mh -fPIC -O2
LDFA = -Wl,--section-start=.stack=0x400 -B $(MKLOCAL)/h8300-elf -L$(CBPATH)/ -mint32 -mh -T $(CBPATH)/h8pic.x -nostartfiles $(CBPATH)/h8piccrt0.S
LDFLAGS = -lmes_h8e

OBJSA = timer.o
OBJS = $(PROG).o $(OBJSA) 

All : $(PROG).elf

$(PROG).elf : $(OBJS)
	$(CC) $(LDFA) -o $(PROG).elf  $(OBJS) $(LDFLAGS) $(LDF)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(PROG).elf $(OBJS)

pre:
	$(CC) $(CFLAGS) -E $(PROG).c -o $(PROG).i
	$(CC) $(CFLAGS) -S $(PROG).i -o $(PROG).s
