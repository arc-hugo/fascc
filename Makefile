GRM=main.y
LEX=main.l
BIN=main

CC=gcc
CFLAGS=-Wall -g

OBJ=symtab.o lex.yy.o y.tab.o

all: $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

y.tab.c: $(GRM)
	yacc -d -v -t $<

lex.yy.c: $(LEX)
	flex --debug $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm $(OBJ) y.tab.c y.tab.h lex.yy.c

test: all
	echo "main(){}" | ./$(BIN)
	echo "main({}" | ./$(BIN) || echo ERROR
