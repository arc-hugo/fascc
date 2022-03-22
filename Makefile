GRM=main.y
LEX=main.l
BIN=main

CC=gcc
CFLAGS=-Wall -g

OBJ=asmtab.o symtab.o y.tab.o lex.yy.o

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
	rm $(OBJ) y.tab.c lex.yy.c

test: all
	echo "main(){}" | ./$(BIN)
	echo "main({}" | ./$(BIN) || echo ERROR
	echo "main(){int a=0;a+=10;print(a);}" | ./$(BIN)
	cat test/testOpeManyVar.c | ./$(BIN)