/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    Access = 258,
    AllocationConstraint = 259,
    And = 260,
    Application = 261,
    Array = 262,
    Assume = 263,
    Attribute = 264,
    Case = 265,
    CEX = 266,
    Class = 267,
    Colon = 268,
    Cofactor = 269,
    Div = 270,
    Dump = 271,
    Else = 272,
    Enum = 273,
    Esac = 274,
    Equal = 275,
    Equiv = 276,
    Exists = 277,
    False = 278,
    Fi = 279,
    Forall = 280,
    Frontier = 281,
    FunDef = 282,
    Greater = 283,
    Identifier = 284,
    If = 285,
    Implies = 286,
    Less = 287,
    Load = 288,
    Maximal = 289,
    Minimal = 290,
    Not = 291,
    NotEqual = 292,
    NotEquiv = 293,
    Number = 294,
    OnSetSize = 295,
    Or = 296,
    Print = 297,
    Prototype = 298,
    QuestionMark = 299,
    Quit = 300,
    Range = 301,
    Reset = 302,
    Seilpmi = 303,
    Show = 304,
    SimMinus = 305,
    SimPlus = 306,
    Size = 307,
    String = 308,
    Then = 309,
    Timer = 310,
    True = 311,
    Undefined = 312,
    Undump = 313,
    VarDef = 314,
    Verbose = 315,
    Visualize = 316,
    Witness = 317,
    WitnessConstraint = 318
  };
#endif
/* Tokens.  */
#define Access 258
#define AllocationConstraint 259
#define And 260
#define Application 261
#define Array 262
#define Assume 263
#define Attribute 264
#define Case 265
#define CEX 266
#define Class 267
#define Colon 268
#define Cofactor 269
#define Div 270
#define Dump 271
#define Else 272
#define Enum 273
#define Esac 274
#define Equal 275
#define Equiv 276
#define Exists 277
#define False 278
#define Fi 279
#define Forall 280
#define Frontier 281
#define FunDef 282
#define Greater 283
#define Identifier 284
#define If 285
#define Implies 286
#define Less 287
#define Load 288
#define Maximal 289
#define Minimal 290
#define Not 291
#define NotEqual 292
#define NotEquiv 293
#define Number 294
#define OnSetSize 295
#define Or 296
#define Print 297
#define Prototype 298
#define QuestionMark 299
#define Quit 300
#define Range 301
#define Reset 302
#define Seilpmi 303
#define Show 304
#define SimMinus 305
#define SimPlus 306
#define Size 307
#define String 308
#define Then 309
#define Timer 310
#define True 311
#define Undefined 312
#define Undump 313
#define VarDef 314
#define Verbose 315
#define Visualize 316
#define Witness 317
#define WitnessConstraint 318

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
