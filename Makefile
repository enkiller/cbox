##  SPDX-License-Identifier: Apache-2.0
##  Change Logs:
##  Date           Author       Notes
##  2022-03-19     tyx          first implementation

.PHONY:all clean debug

CC      = gcc
CPP     = g++
RM      = rm -rf

## Add Source
SRC_FILE   := ./main.c

## Add Source directory
SRC_PATH   := ./src/container/hashmap \
			  ./src/libc	\
			  ./test

## exe file name
TARGET     := cbox-test

## Add include path
INCLUDE_PATH := ./include

## get all source files
SRCS_PATH  += $(foreach dir, $(SRC_PATH), $(dir)/*.c)
SRCS       += $(wildcard $(SRCS_PATH))
SRCS       += $(foreach file, $(SRC_FILE), $(file))
## all .o based on all .c
OBJS        := $(SRCS:.c=.o)

## debug for debug info, when use gdb to debug
ifeq (debug, $(MAKECMDGOALS))
    CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1 -fsanitize=address -fno-omit-frame-pointer -fsanitize=address -fprofile-arcs -ftest-coverage
else
   CFLAGS += -O2
endif

## get all include path
CFLAGS  += -Werror -Wall $(foreach dir, $(INCLUDE_PATH), -I$(dir))

all:$(TARGET)

debug:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) $(OBJS) $(TARGET)
