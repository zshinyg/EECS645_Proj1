IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./
LDIR =./

LIBS=-lm

_DEPS = ALUSimulator.h  RegisterFile_01.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ALUSimulator_Main.o ALUSimulator.o  RegisterFile_01.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ALUSim: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
