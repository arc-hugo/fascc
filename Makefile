LIB=lib
GRM=main.y
LEX=main.l
BIN=main

TESTDIR=test
BUILDDIR=build

CC=gcc
CFLAGS=-Wall -g
YFALGS=-Wcounterexamples -d -v

OBJ=$(BUILDDIR)/asmtab.o $(BUILDDIR)/symtab.o $(BUILDDIR)/y.tab.o $(BUILDDIR)/lex.yy.o

all: $(BIN)

$(BUILDDIR)/%.o: $(LIB)/%.c
	if [[ ! -d ./$(BUILDDIR) ]]; then\
		mkdir $(BUILDDIR);\
	fi
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(LIB)/y.tab.c: $(LIB)/$(GRM)
	yacc $(YFALGS) -t $< -o $(LIB)/y.tab.c

$(LIB)/lex.yy.c: $(LIB)/$(LEX)
	flex -o $(LIB)/lex.yy.c $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm $(OBJ) $(LIB)/{y.tab.c,y.tab.h,lex.yy.c,y.output}

test: all
	for f in $(shell find $(TESTDIR) -name '*.c'); do\
		echo "-------------------------------$$f--------------------------------";\
		cat $$f | ./$(BIN); cat $$f; echo; \
	done
