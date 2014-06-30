CPP = g++
CPPFLAGS = -O2 -Wall
CPPLINK = -ltcl8.4 -ltk8.4 -lBLT24
OBJ =	fourier.o volumes.o scaling.o output.o input.o analysis.o SIMtoEXP.o


SIMtoEXP:	$(OBJ) SIMtoEXP.h
		$(CPP) $(OBJ) $(CPPFLAGS) $(CPPLINK) -o ../$@.lnx


clean: 
	rm -f *.o core *~ a.out

%.o:	%.cpp
	$(CPP) -c $(CPPFLAGS) $<
