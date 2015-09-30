CROSS_COMPILE = /opt/FriendlyARM/toolschain/4.5.1/bin/arm-linux-
CC  = $(CROSS_COMPILE)gcc
LD  = $(CROSS_COMPILE)ld
AR  =$(CROSS_COMPILE)ar
RM  := rm -rf
ALIB_NAME := libalgorithm.a

INCLUDE :=-I../inc
INCLUDE	+=-I../../platform/android/inc

FLAG := -O2 -mfloat-abi=softfp -mfpu=neon $(INCLUDE) 
FLAG += -DPRINT_LOG



SRC_PATH = .
SRCS += $(wildcard $(SRC_PATH)/*.c)
#OBJS := $(SRCS:.c=.o)
OBJS += $(wildcard $(SRC_PATH)/*.o)
OBJS += $(wildcard $(SRC_PATH)/algo/*.o)

#all:acm gt9p fasync test gen_data
all:libs
.PHONY:all clean

libs:
	$(CC) -c $(FLAG) $(SRCS)
	$(AR) -rcs $(ALIB_NAME) ./*.o ./algo/*.o
	$(RM) *.o

clean:
	$(RM) *.o *.a *~
	
