###############################################################################
## Makefile|advice --- Makefile for Advice's chunk store
## Author          : Francisco J. Ballesteros
## Created On      : Thu May  2 20:32:31 1996
## Last Modified By: Francisco J. Ballesteros
## Last Modified On: Mon Aug 19 10:33:37 1996
## Update Count    : 44
## VC Id           : $Id: Makefile,v 1.1.1.1 1996/08/27 09:55:40 nemo Exp $ 
## Usage           : You need GNU make to use this.
##                 : By now this is a standalone Makefile, it'll be included
## Status          : Keep it Going.
###############################################################################

include ~/lib/skel/make.impl

TEXFILES= chunk.tex 
FIGURES=  stkmaps.fig surf.fig regions.fig
PSFIGURES=  stkmaps.eps surf.eps regions.eps
SRCFILES= chunk.c chunk.h 
OBJFILES= chunk.o
TARGET=chunksto

CC=gcc
CFLAGS= -g -DDEBUG=1

all: sources dvi

dvi: $(TARGET).dvi

test: testchunk

sources: $(SRCFILES)

obj: $(OBJFILES)

install: 

clean: 
	-rm -f *.aux *.dvi *.log *.o $(SRCFILES) $(PSFIGURES) $(TEXFILES) \
           $(TARGET).dvi $(TARGET).ps testchunk

$(TARGET).tex:  $(TEXFILES) $(PSFIGURES)

chunk.c: chunk.nw chunk.h seg.h
	notangle -L -Rchunk.c\*  chunk.nw >chunk.c
chunk.h: chunk.nw seg.h
	notangle -L -Rchunk.h\*  chunk.nw >chunk.h
chunk.tex: chunk.nw chunk.defs
	noweave -n -latex  -indexfrom chunk.defs chunk.nw >chunk.tex

$(TARGET).dvi: $(TARGET).tex $(TARGET).aux
	noindex $(TARGET)
	latex   $(TARGET)

$(TARGET).aux: $(TARGET).tex $(TEXFILES)
	latex $(TARGET)

testchunk.c: chunk.h 
testchunk.o: testchunk.c
testchunk: testchunk.o chunk.o
	$(CC) $(CFLAGS) -o testchunk testchunk.o chunk.o
