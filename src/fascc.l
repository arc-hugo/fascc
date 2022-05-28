%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"
#include "asmtab.h"
#include "fascc.tab.h"
%}

%option noyywrap noinput nounput yylineno

%%

[ \t\n]+                                {}
[0-9]+                                  {
                                          yylval.num = atoi(yytext);
                                          return tNB;
                                        }
[0-9]+"."[0-9]+                         {
                                          yylval.num = atoi(yytext); 
                                          return tNB;
                                        }
[0-9]+("."[0-9]+)?("E"|"e")[0-9]+       {
                                          yylval.num = strtold(yytext,NULL);
                                          return tNB;
                                        }
"main"                                  {
                                          yylval.string = "main";
                                          return tMAIN;
                                        }
"{"                                     { return tAO; }
"}"                                     { return tAF; }
"int"                                   { return tINT; }
"void" 		                        { return tVOID; }
"const"                                 { return tCONST; }
"if" 		                        { return tIF; }
"else"                                  { return tELSE; }
"while" 	                        { return tWHILE; }
"="	                                { return tEGAL; }
"-"	                                { return tSOU; }
"+"	                                { return tADD; }
"*"	                                { return tMUL; }
"/"	                                { return tDIV; }
"("	                                { return tPO; }
")"	                                { return tPF; }
"=="                                    { return tDEG; }
"!="                                    { return tDIF; }
">"                                     { return tSUP; }
"<"                                     { return tINF; }
">="                                    { return tSUE; }
"<="                                    { return tINE; }
"&&"                                    { return tAND; }
"||"                                    { return tOR; }
"print"		                        { return tPRINT; }
"return"                                { return tRET; }
";"           	                        { return tPV; }
","                                  { return tVIR; }
"/*"([^*]|\*+[^*/])*\*+"/"              {}
"//".*|"/*"(.*[\n].*)*"*/"              {}
([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|_)* 	{ 
                                          yylval.string = strdup(yytext); 
                                          return tID; 
                                        }
