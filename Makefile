# /*******************************************************************************\
# * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
# *                                                                               *
# * Copyright (C) Gong Han, Chen Tingjun                                          *
# \*******************************************************************************/
# 
#/*
# * This program is modified from MyStar, the original author is netxray@byhh.
# *
# * Many thanks to netxray@byhh
# *
# * AUTHORS:
# *   Gong Han  <gong@fedoraproject.org> from CSE@FJNU CN
# *   Chen Tingjun <chentingjun@gmail.com> from POET@FJNU CN
# *
# * This program is free software; you can redistribute it and/or
# * modify it under the terms of the GNU Lesser General Public
# * License as published by the Free Software Foundation; either
# * version 2 of the License, or (at your option) any later version.
# *
# * This library is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
# * Lesser General Public License for more details.
# *
# * You should have received a copy of the GNU Lesser General Public
# * License along with this library; if not, write to the
# * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# * Boston, MA 02111-1307, USA.
#*/

CC=gcc
Flags=-O2 -Wall
BinPath=/usr/local/bin
ConfPath=/etc/ruijieclient

all:ruijieclient

# DO NOT ever intend to complie it static. It's HARMFUL
ruijieclient: ruijieclient.o myerr.o blog.o sendpacket.o codeconv.o
	$(CC) $(Flags) -o $@  $^ -lpcap -lnet -lssl

myerr.o: myerr.c myerr.h
	$(CC) $(Flags) -o $@ -c $< 

blog.o:  blog.c blog.h  myerr.h
	$(CC) $(Flags) -o $@ -c $< 

sendpacket.o: sendpacket.c sendpacket.h global.h blog.h
	$(CC) $(Flags) -o $@ -c $<

ruijieclient.o: ruijieclient.c ruijieclient.h sendpacket.h myerr.h blog.h global.h
	$(CC) $(Flags) -o $@ -c $<

codeconv.o: codeconv.c codeconv.h
	$(CC) $(Flags) -o $@ -c $<

clean:
	rm -f *.o ruijieclient

rebuild:
	make clean all

install:
cp ruijieclient $(BinPath)
if [ ! -d $(ConfPath) ] then
mkdir -p $(ConfPath)
fi
cp ruijie.conf $(ConfPath)

uninstall:
if [ -f "$(BinPath)/ruijieclient" ]; then
rm -f $(BinPath)/ruijieclient
fi
if [ -d "$(ConfPath)" ]; then
if [ -f "$(ConfPath)/ruijie.conf" ]; then
rm -f $(ConfPath)/ruijie.conf
fi
rm -f $(ConfPath)
fi
