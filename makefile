# File: makefile (to build prependcorpheader)
#
# This is an Opus makefile.
#
# Author: Brian Jenkins <bj@jenkinz.com>
# Date: July 4, 2013
#

XCFLAGS ?=

%if "$(OS,LC)" == "nt"
%setenv PATH=c:\bcc55\bin;$(PATH)
E = .exe
O = obj
CC = bcc32
DEBUG = -v
LDFLAGS = $(DEBUG)
XCLEANMASK = *.tr2 *.tds
LP = .\\

%elif "$(OS,LC)" == "unix"
E = 
O = o
CC = g++
LD = $(CC)
DEBUG = -DDEBUG=1
LDFLAGS = $(DEBUG) -o $@
XCLEANMASK =
LP = ./

%else
%error OS=$(OS) is not recognized!
%endif

IMAGE = $(LP)prependcorpheader

CFLAGS = $(DEBUG) -pedantic -Wall
LD = $(CC)
OBJS = $(IMAGE,R,>.$O)

$(IMAGE)$E : $(OBJS)  
	%echo Linking $@
	>$(LD) $(LDFLAGS) $^

.cpp.$O :
	%echo Compiling $<
	>$(CC) $(CFLAGS) $(XCFLAGS) -c $<	

compile .ALWAYS :
	$(MAKE) $(tgt).$O

clean .ALWAYS :
	--rm *.$O $(IMAGE)$E 

$(OBJS) : $(MAKEFILE)
	
