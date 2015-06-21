CC = mpic++ -O3 
RM = rm -f 
SRC = $(wildcard $(srcdir)*.cc)# files .cc
HEAD =$(wildcard $(srcdir)*.h)# files .h
OBJ = $(subst $(srcdir), $(objdir),$(SRC:.cc=.o))  # objects
PROG = $(bindir)resolve #executable name
srcdir = ./src/
bindir = ./bin/
objdir = ./obj/


all:mkdir $(PROG) 

$(PROG):$(OBJ)
	$(CC) $^ -o $@

$(objdir)%.o:$(srcdir)%.cc $(srcdir)%.h
	$(CC)   -c $< -o $@

$(objdir)%.o:$(srcdir)%.cc
	$(CC)   -c $< -o $@

.PHONY:clean

clean:
	$(RM) $(PROG) $(objdir)*.o 

.PHONY:mkdir

mkdir:	
	$(shell mkdir -p $(bindir) $(objdir))

.PHONY:very-clean

very-clean:
	$(RM) $(srcdir)*.*~ $(srcdir)*.*\# 
	$(RM) -R $(bindir)
	$(RM) -R $(objdir)
	$(RM) out.txt
	$(RM) puzzle.txt

