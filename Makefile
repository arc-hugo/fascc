LIB=lib
COMP=fascc
INTER=fastinterp

TESTDIR=test
BUILDDIR=build

CC=gcc
CFLAGS=-Wall -g
YFALGS=-Wcounterexamples -d -v

OBJCOM=$(BUILDDIR)/asmtab.o
OBJCOMP=$(BUILDDIR)/symtab.o $(BUILDDIR)/fascc.tab.o $(BUILDDIR)/fascc.yy.o
OBJINTER=$(BUILDDIR)/fastinterp.tab.o $(BUILDDIR)/fastinterp.yy.o

all: $(COMP) $(INTER)

$(BUILDDIR)/%.o: $(LIB)/%.c
	if [[ ! -d ./$(BUILDDIR) ]]; then\
		mkdir $(BUILDDIR);\
	fi
	$(CC) -c $(CFLAGS) $< -o $@

$(LIB)/%.tab.c: $(LIB)/%.y
	yacc $(YFALGS) -t $< -o $@

$(LIB)/%.yy.c: $(LIB)/%.l
	flex -o $@ $<

$(COMP): $(OBJCOM) $(OBJCOMP)
	$(CC) $(CFLAGS) $^ -o $@

$(INTER): $(OBJCOM) $(OBJINTER)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJCOM) $(OBJCOMP) $(OBJINTER) $(LIB)/{*.tab.h,*.output}

test: all
	for f in $(shell find $(TESTDIR) -name '*.c'); do\
		echo "-------------------------------$$f--------------------------------";\
		cat $$f | ./$(COMP);\
		printf "\nCODE C:\n\n"; nl $$f;\
		printf "\nCODE ASM:\n\n"; nl -v 0 out;\
		printf "\nINTERPRETATION:\n\n"; cat out | ./$(INTER); echo; \
	done
	rm out
