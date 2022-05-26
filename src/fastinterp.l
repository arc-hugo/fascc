%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fastinterp.tab.h"
%}

%option noyywrap

%%

[ \t\n]+  {}
"ADD"   { return tADD; }
"MUL"   { return tMUL; }
"SOU"   { return tSOU; }
"DIV"   { return tDIV; }
"COP"   { return tCOP; }
"AFC"   { return tAFC; }
"JMP"   { return tJMP; }
"JMF"   { return tJMF; }
"INF"   { return tINF; }
"SUP"   { return tSUP; }
"EQU"   { return tEQU; }
"PRI"   { return tPRI; }
"AND"   { return tAND; }
"OR"    { return tOR;  }
"NOT"   { return tNOT; }
"CLL"   { return tCLL; }
"RET"   { return tRET; }

[0-9]+    {
            yylval.num = atoi(yytext);
            return tNB;
          }
"@"[0-9]+ {
            yylval.num = atoi(yytext+1);
            return tADDR;
          }
"l"[0-9]+ {
            yylval.num = atoi(yytext+1);
            return tLINE;
          }
