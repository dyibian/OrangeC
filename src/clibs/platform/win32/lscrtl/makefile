# Software License Agreement
# 
#     Copyright(C) 1994-2019 David Lindauer, (LADSoft)
# 
#     This file is part of the Orange C Compiler package.
# 
#     The Orange C Compiler package is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     The Orange C Compiler package is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License
#     along with Orange C.  If not, see <http://www.gnu.org/licenses/>.
# 
#     As a special exception, if other files instantiate templates or
#     use macros or inline functions from this file, or you compile
#     this file and link it with other works to produce a work based
#     on this file, this file does not by itself cause the resulting
#     work to be covered by the GNU General Public License. However
#     the source code for this file must still be made available in
#     accordance with section (3) of the GNU General Public License.
#     
#     This exception does not invalidate any other reasons why a work
#     based on this file might be covered by the GNU General Public
#     License.
# 
#     contact information:
#         email: TouchStone222@runbox.com <David Lindauer>
# 

SHELL=cmd.exe

RCFILE=lscrtl

all: .\c0dll.o .\lscrtl.dll .\lscrtlil.dll

ifneq ($(shell type makefile),)
B = /b
endif

WINOBJS = $(shell dir $(B) $(WINOBJECT)\\*.o)
DLLOBJS = $(shell dir $(B) $(DLLOBJECT)\\*.o)

.\lscrtl.dll: .\lscrtl.o .\c0dll.o $(RCFILE).res
	-del $@
	$(LINK) $(LINKFLAGS) /T:DLL32 -o.\lscrtl .\lscrtl.o .\c0dll.o $(CLIBOBJECT)\srproc.o $(foreach dir, $(filter-out c0.o c0lsstartup.o, $(DLLOBJS)), $(DLLOBJECT)\$(dir)) $(SYSOBJECT)\climp.l $(RCFILE).res
	move lscrtl.dll $(LSCRTLOBJECT)
	olib lscrtl.l + $(DLLOBJECT)\shlguid.o + $(DLLOBJECT)\xmluuid.o
	move lscrtl.l $(LSCRTLOBJECT)

.\lscrtlil.dll: .\lscrtl.o .\c0dll.o $(RCFILE).res
	-del $@
	$(LINK) $(LINKFLAGS) /l /T:DLL32 -o.\lscrtlil .\lscrtl.o .\c0dll.o $(CLIBOBJECT)\srproc.o $(foreach dir, $(filter-out c0.o c0lsstartup.o, $(DLLOBJS)), $(DLLOBJECT)\$(dir)) $(SYSOBJECT)\climp.l $(RCFILE).res
#	move lscrtlil.dll \occil\bin

.\lscrtl.o: lscrtl.nas
	$(ASM) $(ASMFLAGS) -o$@ $^

.\c0dll.o: c0dll.c
	$(CC) /c $(CFLAGS) $(STDINCLUDE) -o$@ $^

$(RCFILE).res: $(RCFILE).rc ..\..\..\..\version.h
	orc -i$(ORANGEC)\src\clibs\stdinc -i$(ORANGEC)\src\clibs\platform\win32\wininc $(RCFILE).rc

#DISTRIBUTE:
#	$(ZIP) -porangec a $(DISTADDON)\lscrtl $(LSCRTLOBJECT)\*.*
