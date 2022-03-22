GRM=main.y
LEX=main.l
BIN=main

TESTDIR=test/

CC=gcc
CFLAGS=-Wall -g

OBJ=asmtab.o symtab.o y.tab.o lex.yy.o

all: $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

y.tab.c: $(GRM)
	yacc -d -v -t $<

lex.yy.c: $(LEX)
	flex $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm $(OBJ) y.tab.c lex.yy.c

test: all
	for f in $(shell find $(TESTDIR) -name '*.c'); do\
		echo "-------------------------------$$f--------------------------------";\
		cat $$f | ./$(BIN); cat $$f; echo; \
	done
