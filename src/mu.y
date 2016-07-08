%{

#include "pterm.h"

extern int yyerror(const char*);
extern int yylex();

#define YYSTYPE pterm

extern const char * token_to_str(int);
extern pterm parseTree;

%}

%token Access
%token AllocationConstraint
%token And
%token Application
%token Array
%token Assume
%token Attribute
%token Case
%token CEX
%token Class
%token Colon
%token Cofactor
%token Div
%token Dump
%token Else
%token Enum
%token Esac
%token Equal
%token Equiv
%token Exists
%token False
%token Fi
%token Forall
%token Frontier
%token FunDef
%token Greater
%token Identifier
%token If
%token Implies
%token Less
%token Load
%token Maximal
%token Minimal
%token Not
%token NotEqual
%token NotEquiv
%token Number
%token OnSetSize
%token Or
%token Print
%token Prototype
%token QuestionMark
%token Quit
%token Range
%token Reset
%token Seilpmi
%token Show
%token SimMinus
%token SimPlus
%token Size
%token String
%token Then
%token Timer
%token True
%token Undefined
%token Undump
%token VarDef
%token Verbose
%token Visualize
%token Witness
%token WitnessConstraint

%%

start
:
commands
;

commands
:
command
{
  YYACCEPT;
}
|
Quit
{
  YYACCEPT;
}
;

command
:
typeDef ';'
{
  parseTree = $1; 
}
|
expr ';'
{
  parseTree = $1;
}
|
varDef
{
  parseTree = $1;
}
|
funDef
{
  parseTree = $1;
}
|
directive ';'
{
  parseTree = $1;
}
;

directive
:
directiveKW directiveParams
{
  $$ = pterm_cons($1, $2);
}
|
directiveKW
{
  $$ = pterm_cons($1, 0);
}
|
unaryExprDirective expr
{
  $$ = pterm_cons($1, $2);
}
|
maxOneParamDirective
{
  $$ = pterm_cons($1, 0);
}
|
maxOneParamDirective expr
{
  $$ = pterm_cons($1, $2);
}
;

unaryExprDirective
:
CEX
|
Visualize
|
Witness
|
OnSetSize
|
Dump
|
Undump
|
Reset
|
Size
;

maxOneParamDirective
:
Timer
|
Frontier
|
Verbose
;


directiveParams
:
directivePara
{
  $$ = pterm_cons($1,0);
}
|
directivePara ',' directiveParams
{
  $$ = pterm_cons($1,$3);
}
;

directiveKW
:
Print
|
Load
;

directivePara
:
string
|
expr
;

string
:
String
{
  $$ = pterm_create_Keyword(String);
  $$ = pterm_cons($$, $1);
}
;

basicExpr
:
number
|
functionApplication
|
case
|
'(' expr ')'
{
  $$ = $2;
}
|
variable
|
basicExpr '{' exprAttribute '}'
{
  if($3!=0)
    {
      pterm tmp = pterm_create_Keyword(Attribute);
      $$ = pterm_cons($1,$3);
      $$ = pterm_cons(tmp, $$);
    }
  else $$ = $1;
}
;

exprAttribute
:
WitnessConstraint witcslist
{
  $$ = pterm_cons($1,$2);
}
|
Assume expr
{
  $$ = pterm_cons($1,$2);
}
|
Cofactor expr
{
  $$ = pterm_cons($1,$2);
}
|
AllocationConstraint allocConstraintList
{
  $$ = pterm_cons($1,$2);
}
|
Show showAttrList
{
  $$ = pterm_cons($1,$2);
}
;

witcslist
:
witcsitem
{
  $$ = pterm_cons($1,0);
}
|
witcsitem ',' witcslist
{
  $$ = pterm_cons($1,$3);
}
;

witcsitem
:
witlabel ',' expr
{
  $$ = pterm_cons($1,$3);
}
;

witlabel
:
number
{
  $$ = pterm_cons($1,0);
}
|
number Greater witlabel
{
  $$ = pterm_cons($2, $3);
  $$ = pterm_cons($1,$$);
}
|
number Less witlabel
{
  $$ = pterm_cons($2, $3);
  $$ = pterm_cons($1,$$);
}
;

showAttrList
:
string
{
  $$ = pterm_cons($1, 0);
}
|
string ',' showAttrVarList
{
  $$ = pterm_cons($1, $3);
}
|
showAttrVarList
;

showAttrVarList
:
variable
{
  $$ = pterm_cons($1, 0);
}
|
variable ',' showAttrVarList
{
  $$ = pterm_cons($1, $3);
}
;

number
:
True
|
False
|
Number
;

variable
:
basicExprWithComponents components
{
  if($2)
    {
      pterm tmp = pterm_create_Keyword(Access);
      $$ = pterm_cons($1, $2);
      $$ = pterm_cons(tmp, $$);
    }
  else $$ = $1;
}
;

basicExprWithComponents
:
Identifier
|
arrayApplication
;

components
:
/* epsilon */
{
  $$ = 0;
}
|
'.' basicExprWithComponents components
{
  $$ = pterm_cons($2,$3);
}
;

functionApplication
:
Identifier '(' kommalist ')'
{
  pterm tmp = pterm_create_Keyword(Application);
  $$ = pterm_cons($1, $3);
  $$ = pterm_cons(tmp, $$);
}
;

arrayApplication
:
Identifier '[' expr ']'
{
  pterm tmp = pterm_create_Keyword(Array);
  $$ = pterm_cons($1, $3);
  $$ = pterm_cons(tmp, $$);
}
;

case
:
Case case_body Esac
{
  $$ = $2;
}
;

case_body
:
expr Colon expr ';'
{
  pterm tmp = pterm_create_Keyword(And);
  $$ = pterm_cons($1,$3);
  $$ = pterm_cons(tmp,$$);
}
|
expr Colon expr ';' case_body
{
  pterm tmp = pterm_create_Keyword(If);
  $$ = pterm_cons($3,$5);
  $$ = pterm_cons($1,$$);
  $$ = pterm_cons(tmp,$$);
}
;


kommalist
:
expr
{
  $$ = pterm_cons($1, 0);
}
|
expr ',' kommalist
{
  $$ = pterm_cons($1, $3);
}
;

expr
:
rvalue
;

rvalue
:
matched_ite
|
unmatched_ite
;

matched_ite
:
If '(' quantifier_expr ')' matched_ite Else matched_ite
{
  $$ = pterm_cons($5,$7);
  $$ = pterm_cons($3,$$);
  $$ = pterm_cons($1,$$);
}
|
quantifier_expr
;

unmatched_ite
:
If '(' quantifier_expr ')' matched_ite Else unmatched_ite
{
  $$ = pterm_cons($5,$7);
  $$ = pterm_cons($3,$$);
  $$ = pterm_cons($1,$$);
}
|
If '(' quantifier_expr ')' quantifier_expr
{
  pterm tmp = pterm_create_Keyword(Then);
  $$ = pterm_cons($3,$5);
  $$ = pterm_cons(tmp,$$);
}
;

quantifier_expr
:
quantifier variableDeclarations quantifierBodySeperator quantifier_expr
{
  $$ = pterm_cons($2,$4);
  $$ = pterm_cons($1,$$);
}
|
equivs
;

quantifier
:
Exists
|
Forall
;

quantifierBodySeperator
:
'.'
|
Colon
;

equivs
:
ors equiv_operator ors
{
  $$ = pterm_cons($1,$3);
  $$ = pterm_cons($2,$$);
}
|
ors Seilpmi ors
{
  pterm tmp = pterm_create_Keyword(Implies);
  $$ = pterm_cons($3,$1);
  $$ = pterm_cons(tmp,$$);
}
|
ors
;

equiv_operator
:
Equiv
|
NotEquiv
|
Implies
;

ors
:
ands Or ors
{
  $$ = pterm_cons($1,$3);
  $$ = pterm_cons($2,$$);
}
|
ands
;

ands
:
unary And ands
{
  $$ = pterm_cons($1,$3);
  $$ = pterm_cons($2,$$);
}
|
unary
;

unary
:
unary_operator unary
{
  $$ = pterm_cons($1,$2);
}
|
assume
|
assume eq_operator assume
{
  $$ = pterm_cons($1,$3);
  $$ = pterm_cons($2,$$);
}
;

assume
:
basicExpr
|
basicExpr Assume basicExpr
{
  pterm tmp = pterm_create_Keyword(Attribute);
  $$ = pterm_cons($2,$3);
  $$ = pterm_cons($1,$$);
  $$ = pterm_cons(tmp, $$);
}
|
basicExpr Cofactor basicExpr
{
  pterm tmp = pterm_create_Keyword(Attribute);
  $$ = pterm_cons($2,$3);
  $$ = pterm_cons($1,$$);
  $$ = pterm_cons(tmp, $$);
}
;

eq_operator
:
Equal
|
NotEqual
;

unary_operator
:
Not
;


variableDeclarations
:
variableDeclaration
{
  $$ = pterm_cons($1,0);
}
|
variableDeclaration ',' variableDeclarations
{
  $$ = pterm_cons($1,$3);
}
;

variableDeclaration
:
TypeName Identifier ArrayDef
{
  if($3)
    $$ = pterm_cons($2,$3);
  else
    $$ = $2;
  $$ = pterm_cons($1,$$);
}
;

typeDef
:
Class Identifier '{' classDefComps '}' compoundAllocCS
{
  pterm tmp = pterm_create_Keyword(Class);
  $$ = pterm_cons($4,$6);
  $$ = pterm_cons($2,$$);
  $$ = pterm_cons(tmp,$$);
}
|
Enum Identifier '{' enumList '}'
{
  pterm tmp = pterm_create_Keyword(Enum);
  $$ = pterm_cons($2,$4);
  $$ = pterm_cons(tmp,$$);
}
|
Enum Identifier '{' range '}'
{
  pterm tmp = pterm_create_Keyword(Range);
  $$ = pterm_cons($2,$4);
  $$ = pterm_cons(tmp,$$);
}
;

compoundAllocCS
:
/* epsilon */
{
  $$ = 0;
}
|
allocConstraintList
;


allocConstraintList
:
allocConstraint
|
allocConstraint ',' allocConstraintList
{
  $$ = pterm_cons($1,$3);
}
;

allocConstraint
:
Identifier allocBinaryRelation Identifier
{
  $$ = pterm_cons($1, $3);
  $$ = pterm_cons($2, $$);
}
|
allocSingleRelation Identifier
{
  $$ = pterm_cons($1, $2);
}
;

allocBinaryRelation
:
SimPlus
|
SimMinus
|
Less
|
Greater
;

allocSingleRelation
:
SimMinus
|
SimPlus
;

classDefComps
:
/* epsilon */
{
  $$ = 0;
}
|
classDefComp ';' classDefComps
{
  $$ = pterm_cons($1,$3);
}
;

classDefComp
:
TypeName variableDefsInClassComp
{
  $$ = pterm_cons($1,$2);
}
;

enumList
:
/* epsilon */
{
  $$ = 0;
}
|
nonEmptyEnumList
;

nonEmptyEnumList
:
enumListComp ',' nonEmptyEnumList
{
  $$ = pterm_cons($1,$3);
}
|
enumListComp
{
  $$ = pterm_cons($1,0);
}
;

enumListComp
:
Identifier
;

range
:
Number dots Number
{
  $$ = pterm_cons($1,$3);
}
;

dots
:
'.' '.'
|
'.' '.' '.'
;

ArrayDef
:
'[' Number ']'
{
  $$ = $2;
}
|
/* epsilon */
{
  $$ = 0;
}
;

TypeName
:
Identifier
;

variableDefsInClassComp
:
variableDefInClassComp
{
  $$ = pterm_cons($1, 0);
}
|
variableDefInClassComp ',' variableDefsInClassComp
{
  $$ = pterm_cons($1,$3);
}
;

variableDefInClassComp
:
Identifier ArrayDef
{
  if($2)
    $$ = pterm_cons($1,$2);
  else
    $$ = $1;
}
;

varDef
:
classDefComp ';'
{
  pterm tmp = pterm_create_Keyword(VarDef);
  $$ = pterm_cons(tmp,$1);
}
;

funDef
:
funDefHead expr ';'
{
  pterm tmp = pterm_create_Keyword(FunDef);
  $$ = pterm_cons($1,$2);
  $$ = pterm_cons(tmp,$$);
}
|
funDefHead allocConstraintList expr ';'
{
  pterm tmp = pterm_create_Keyword(AllocationConstraint);
  $$ = pterm_cons(tmp,$2);
  tmp = pterm_create_Keyword(Attribute);
  $$ = pterm_cons($3,$$);
  $$ = pterm_cons(tmp,$$);
  tmp = pterm_create_Keyword(FunDef);
  $$ = pterm_cons($1,$$);
  $$ = pterm_cons(tmp,$$);
}
|
funDefHead ';'
{
  $$ = pterm_create_Keyword(Prototype);
  $$ = pterm_cons($$,$1);
}
;

funDefHead
:
minMax TypeName Identifier '(' funArgs ')'
{
  $$ = pterm_cons($1,0);
  $$ = pterm_cons($5,$$);
  $$ = pterm_cons($3,$$);
  $$ = pterm_cons($2,$$);
}
|
TypeName Identifier '(' funArgs ')'
{
  $$ = pterm_cons(0,0);
  $$ = pterm_cons($4,$$);
  $$ = pterm_cons($2,$$);
  $$ = pterm_cons($1,$$);
}
;

minMax
:
Minimal
|
Maximal
;

funArgs
:
/* epsilon */
{
  $$ = 0;
}
|
nonEmptyFunArgs
;

nonEmptyFunArgs
:
funArgComp
{
  $$ = pterm_cons($1, 0);
}
|
funArgComp ',' nonEmptyFunArgs
{
  $$ = pterm_cons($1,$3);
}
;

funArgComp
:
TypeName variableDefInClassComp
{
  $$ = pterm_cons($1,$2);
}
;

%%
