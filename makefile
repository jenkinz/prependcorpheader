# File: makefile (to build prependcorpheader)
#
# This is an Opus makefile.
#
# Author: Brian Jenkins <bj@jenkinz.com>
# Date: July 4, 2013
#

NAME = prependcorpheader

CC = g++
CFLAGS = -Wall -pedantic

LD = $(CC)
LFLAGS = $(CFLAGS)

SRCS = $(NAME).cpp
OBJS = $(NAME).o

$(NAME) : $(OBJS)
	$(LD) $(LFLAGS) -o $(NAME) $<

$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(NAME) $(NAME).o
	
