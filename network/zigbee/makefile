#-*-Makefile-*- vim:syntax=make
CC:=gcc
AR:=ar
#CFLAGS:=-Wall -O1 -mmcu=msp430x1611 -mdisable-hwmul -nostdinc -nostdlib
#CFLAGS:=-Os -mmcu=msp430x1611 -mdisable-hwmul -nostdinc -nostdlib
CFLAGS:=$(CFLAGS) -fexpensive-optimizations -frename-registers -frerun-loop-opt -fstrength-reduce -pipe

DEPFILE = .depends.mf
INC = -I include -I ../../include
SRC = $(foreach dir,. nwk,$(wildcard $(dir)/*.c) )
OBJ = $(patsubst %.c,%.o, $(SRC) )
okernel:=zcall.o zfield.o zneighbor.o zpacket.o

.PHONY: all nwk dep clean_dep clean

all: nwk

ifeq ($(DEPFILE),$(wildcard $(DEPFILE)))
include $(DEPFILE)
endif

nwk: $(OBJ) 
	$(AR) -q -s libznet.a $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $(patsubst %.o,%.c,$@) -o $@

dep:
	$(shell > $(DEPFILE) )
	$(foreach a,$(SRC),$(shell $(CC) -M -MG -MT '$(patsubst %.c,%.o, $(a))' $(INC) $(a) >> $(DEPFILE) ))

clean_dep:
	rm -f $(DEPFILE)

clean: 
	rm -f *.o nwk/*.o *.a


