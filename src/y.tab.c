/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "mu.y" /* yacc.c:339  */


#include "pterm.h"

extern int yyerror(const char*);
extern int yylex();

#define YYSTYPE pterm

extern const char * token_to_str(int);
extern pterm parseTree;


#line 80 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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

/* Copy the second part of user declarations.  */

#line 257 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  71
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  149
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  249

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      66,    67,     2,     2,    65,     2,    70,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    64,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    68,     2,    69,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    81,    81,    86,    91,    99,   104,   109,   114,   119,
     127,   132,   137,   142,   147,   155,   157,   159,   161,   163,
     165,   167,   169,   174,   176,   178,   184,   189,   197,   199,
     204,   206,   211,   220,   222,   224,   226,   231,   233,   247,
     252,   257,   262,   267,   275,   280,   288,   296,   301,   307,
     316,   321,   326,   331,   336,   344,   346,   348,   353,   367,
     369,   375,   379,   387,   397,   407,   415,   422,   434,   439,
     447,   452,   454,   459,   466,   471,   478,   488,   494,   499,
     501,   506,   508,   513,   519,   526,   531,   533,   535,   540,
     546,   551,   557,   562,   567,   569,   578,   580,   588,   599,
     601,   606,   612,   617,   625,   637,   645,   652,   663,   667,
     673,   675,   683,   689,   697,   699,   701,   703,   708,   710,
     716,   720,   728,   737,   741,   746,   751,   759,   764,   772,
     774,   779,   785,   792,   797,   802,   810,   821,   830,   837,
     849,   858,   866,   877,   879,   885,   889,   894,   899,   907
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "Access", "AllocationConstraint", "And",
  "Application", "Array", "Assume", "Attribute", "Case", "CEX", "Class",
  "Colon", "Cofactor", "Div", "Dump", "Else", "Enum", "Esac", "Equal",
  "Equiv", "Exists", "False", "Fi", "Forall", "Frontier", "FunDef",
  "Greater", "Identifier", "If", "Implies", "Less", "Load", "Maximal",
  "Minimal", "Not", "NotEqual", "NotEquiv", "Number", "OnSetSize", "Or",
  "Print", "Prototype", "QuestionMark", "Quit", "Range", "Reset",
  "Seilpmi", "Show", "SimMinus", "SimPlus", "Size", "String", "Then",
  "Timer", "True", "Undefined", "Undump", "VarDef", "Verbose", "Visualize",
  "Witness", "WitnessConstraint", "';'", "','", "'('", "')'", "'{'", "'}'",
  "'.'", "'['", "']'", "$accept", "start", "commands", "command",
  "directive", "unaryExprDirective", "maxOneParamDirective",
  "directiveParams", "directiveKW", "directivePara", "string", "basicExpr",
  "exprAttribute", "witcslist", "witcsitem", "witlabel", "showAttrList",
  "showAttrVarList", "number", "variable", "basicExprWithComponents",
  "components", "functionApplication", "arrayApplication", "case",
  "case_body", "kommalist", "expr", "rvalue", "matched_ite",
  "unmatched_ite", "quantifier_expr", "quantifier",
  "quantifierBodySeperator", "equivs", "equiv_operator", "ors", "ands",
  "unary", "assume", "eq_operator", "unary_operator",
  "variableDeclarations", "variableDeclaration", "typeDef",
  "compoundAllocCS", "allocConstraintList", "allocConstraint",
  "allocBinaryRelation", "allocSingleRelation", "classDefComps",
  "classDefComp", "enumList", "nonEmptyEnumList", "enumListComp", "range",
  "dots", "ArrayDef", "TypeName", "variableDefsInClassComp",
  "variableDefInClassComp", "varDef", "funDef", "funDefHead", "minMax",
  "funArgs", "nonEmptyFunArgs", "funArgComp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,    59,    44,    40,    41,   123,   125,
      46,    91,    93
};
# endif

#define YYPACT_NINF -169

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-169)))

#define YYTABLE_NINF -134

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     122,    15,  -169,   -13,  -169,    -5,  -169,  -169,  -169,  -169,
     -10,   -44,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,    15,    50,
    -169,  -169,   -11,    15,    15,   189,    -2,  -169,  -169,     3,
    -169,  -169,  -169,    12,  -169,  -169,  -169,  -169,    53,  -169,
      56,    38,    79,    37,   229,    25,    44,    57,  -169,  -169,
     171,    53,   -48,    90,    97,    45,    46,    15,    15,   150,
      48,  -169,  -169,  -169,  -169,  -169,  -169,    47,  -169,  -169,
      19,    19,     6,    88,  -169,  -169,  -169,     2,    58,    92,
    -169,  -169,  -169,   229,   229,   229,   229,  -169,  -169,    19,
    -169,  -169,  -169,    -7,  -169,    60,   -19,  -169,  -169,  -169,
      62,    15,    65,    93,    98,  -169,    15,    53,   -18,    64,
      70,    71,    74,  -169,   189,    68,    68,   -24,    15,    15,
     -12,     7,    73,    66,     3,  -169,  -169,   150,    53,    75,
    -169,  -169,  -169,  -169,  -169,    53,   110,  -169,   121,  -169,
    -169,  -169,  -169,   124,  -169,    95,   -24,  -169,   100,    99,
      85,   104,   121,  -169,   101,   107,  -169,   120,   118,  -169,
      15,  -169,   221,  -169,    69,  -169,  -169,  -169,   125,  -169,
    -169,   126,  -169,   127,   130,    11,  -169,  -169,  -169,  -169,
    -169,   121,   131,  -169,   132,   136,    75,  -169,  -169,  -169,
    -169,    53,    15,   -24,    53,   133,   163,  -169,   175,  -169,
    -169,   143,   196,   198,    88,    88,     7,    15,     7,     7,
    -169,  -169,    53,  -169,   153,  -169,  -169,  -169,  -169,   154,
    -169,  -169,   150,    15,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,   156,  -169,  -169,   221,   209,   221
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    15,     0,    19,     0,    79,    56,    80,    24,
      59,     0,    29,   144,   143,   101,    57,    18,    28,     4,
      21,    22,    23,    55,    20,    25,    16,    17,     0,     0,
       2,     3,     0,     0,    13,    11,    96,    33,    37,    61,
      34,    60,    35,     0,    70,    71,    72,    74,     0,    78,
      85,    90,    92,    94,     0,     0,     0,     0,     7,     8,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     9,    12,    14,    32,    10,    26,    30,    31,
       0,     0,     0,     0,    58,     6,   133,     0,   102,     0,
      86,    88,    87,     0,     0,     0,     0,    99,   100,     0,
      93,     5,   137,   132,   122,   134,    59,   118,   119,   140,
       0,     0,   110,     0,     0,    65,     0,   120,   123,     0,
      68,     0,     0,    36,     0,    97,    98,     0,     0,     0,
       0,     0,     0,    59,    61,    82,    81,     0,     0,   132,
      84,    83,    89,    91,    95,   145,     0,   136,     0,   117,
     116,   115,   114,     0,   138,     0,     0,   113,     0,     0,
       0,     0,     0,   127,     0,     0,   124,   126,     0,    63,
       0,    64,     0,    27,     0,    42,    40,    41,    50,    43,
      52,    53,    39,    44,     0,    47,    38,    62,    77,   103,
     104,     0,     0,   146,   147,     0,   132,   135,   112,   139,
     111,   145,    66,   108,   120,     0,     0,   106,     0,   107,
      69,     0,     0,    76,     0,     0,     0,     0,     0,     0,
     149,   142,     0,   131,     0,    67,   105,   109,   121,   129,
     128,   125,     0,     0,    51,    54,    45,    46,    48,    49,
     148,   141,   130,     0,    73,    75,     0,     0,     0
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,  -169,  -169,  -169,  -169,  -169,   105,  -169,  -169,
     102,    22,  -169,    14,  -169,  -123,  -169,  -108,  -128,  -122,
     151,   106,  -169,  -169,  -169,    31,    77,     0,  -169,  -168,
       8,   -67,  -169,  -169,  -169,  -169,     5,   140,   184,   149,
    -169,  -169,   111,  -169,  -169,  -169,  -120,  -169,  -169,  -169,
      49,   254,  -169,    51,  -169,  -169,  -169,   117,     1,   113,
      72,  -169,  -169,  -169,  -169,    61,    42,  -169
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,    33,    34,    76,    35,    77,
      78,    36,   132,   182,   183,   184,   179,   180,    37,    38,
      39,    84,    40,    41,    42,    63,   119,    64,    44,    45,
      46,    47,    48,   137,    49,    94,    50,    51,    52,    53,
      99,    54,    87,    88,    55,   226,   111,   112,   153,   113,
     160,   161,   165,   166,   167,   168,   206,   147,   191,   104,
     105,    58,    59,    60,    61,   192,   193,   194
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,    57,   122,   185,   212,   174,    80,   175,   181,   149,
     127,   163,    81,   150,   128,   135,    65,   133,    67,  -133,
     129,   164,    69,    68,    66,     1,   107,   108,    70,     1,
       7,   151,   152,    73,    74,    79,   200,     6,     7,   218,
       8,    75,     7,   219,    62,    11,    16,    67,    62,    89,
      71,    15,    68,    72,    16,   130,    67,    97,    16,   145,
     110,    68,   114,    23,   146,   244,    82,   120,   121,   131,
     188,    23,   136,    83,    98,    23,    85,    90,   247,    95,
     244,    28,    86,   227,    96,    28,   103,    91,   185,   101,
     185,   185,   181,   181,    92,   238,   239,   149,   140,   141,
     142,   150,   125,   126,    93,   213,   234,   235,   102,   115,
     116,   155,   124,   117,   118,   123,   159,   133,   162,   151,
     152,   139,   157,   138,    79,   148,   154,   158,   176,   177,
     156,   169,     1,     2,     3,   170,    82,    68,     4,    89,
       5,   172,   186,   171,     6,     7,   146,     8,     9,   195,
     196,    10,    11,   198,   203,    12,    13,    14,    15,   199,
       1,    16,    17,   202,    18,   243,   201,    19,   204,    20,
     120,   205,     6,     7,    21,     8,   207,    22,    23,    62,
      24,     1,    25,    26,    27,   208,    15,   209,    28,    16,
     214,   215,   216,     6,     7,   217,     8,   222,   221,     1,
     106,    11,   230,   229,   163,   162,    23,    15,   223,   232,
      16,     6,     7,   233,     8,   -74,    28,   237,    62,    11,
     241,   107,   108,   246,   242,    15,   248,    23,    16,   173,
     236,     1,   178,   225,   134,   109,   143,    28,   100,     1,
     187,   245,    75,     6,     7,    23,     8,   210,   144,   189,
      62,   211,     7,   228,    56,    28,   190,    15,    62,   231,
      16,   197,   224,   220,   240,    15,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,    23,     0,    28,     0,     0,
       0,     0,     0,     0,     0,    28
};

static const yytype_int16 yycheck[] =
{
       0,     0,    69,   131,   172,    29,     8,   127,   130,    28,
       4,    29,    14,    32,     8,    13,    29,    29,    66,    29,
      14,    39,    66,    71,    29,    10,    50,    51,    28,    10,
      23,    50,    51,    33,    34,    35,   156,    22,    23,    28,
      25,    53,    23,    32,    29,    30,    39,    66,    29,    48,
       0,    36,    71,    64,    39,    49,    66,    20,    39,    66,
      60,    71,    61,    56,    71,   233,    68,    67,    68,    63,
     137,    56,    70,    70,    37,    56,    64,    21,   246,    41,
     248,    66,    29,   203,     5,    66,    29,    31,   216,    64,
     218,   219,   214,   215,    38,   218,   219,    28,    93,    94,
      95,    32,    80,    81,    48,   172,   214,   215,    64,    19,
      13,   111,    65,    68,    68,    67,   116,    29,   117,    50,
      51,    29,    29,    65,   124,    65,    64,    29,   128,   129,
      65,    67,    10,    11,    12,    65,    68,    71,    16,   138,
      18,    67,    69,    72,    22,    23,    71,    25,    26,    39,
      29,    29,    30,    29,    69,    33,    34,    35,    36,    64,
      10,    39,    40,    64,    42,   232,    66,    45,    64,    47,
     170,    70,    22,    23,    52,    25,    69,    55,    56,    29,
      58,    10,    60,    61,    62,    65,    36,    69,    66,    39,
      65,    65,    65,    22,    23,    65,    25,    65,    67,    10,
      29,    30,    39,    70,    29,   204,    56,    36,    72,    66,
      39,    22,    23,    17,    25,    17,    66,   217,    29,    30,
      67,    50,    51,    67,    70,    36,    17,    56,    39,   124,
     216,    10,   130,   202,    83,    64,    96,    66,    54,    10,
     134,   233,    53,    22,    23,    56,    25,   170,    99,   138,
      29,    30,    23,   204,     0,    66,   139,    36,    29,   208,
      39,   148,   201,   191,   222,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    11,    12,    16,    18,    22,    23,    25,    26,
      29,    30,    33,    34,    35,    36,    39,    40,    42,    45,
      47,    52,    55,    56,    58,    60,    61,    62,    66,    74,
      75,    76,    77,    78,    79,    81,    84,    91,    92,    93,
      95,    96,    97,   100,   101,   102,   103,   104,   105,   107,
     109,   110,   111,   112,   114,   117,   124,   131,   134,   135,
     136,   137,    29,    98,   100,    29,    29,    66,    71,    66,
     100,     0,    64,   100,   100,    53,    80,    82,    83,   100,
       8,    14,    68,    70,    94,    64,    29,   115,   116,   131,
      21,    31,    38,    48,   108,    41,     5,    20,    37,   113,
     111,    64,    64,    29,   132,   133,    29,    50,    51,    64,
     100,   119,   120,   122,   131,    19,    13,    68,    68,    99,
     100,   100,   104,    67,    65,    84,    84,     4,     8,    14,
      49,    63,    85,    29,    93,    13,    70,   106,    65,    29,
     109,   109,   109,   110,   112,    66,    71,   130,    65,    28,
      32,    50,    51,   121,    64,   100,    65,    29,    29,   100,
     123,   124,   131,    29,    39,   125,   126,   127,   128,    67,
      65,    72,    67,    80,    29,   119,   100,   100,    83,    89,
      90,    92,    86,    87,    88,    91,    69,    94,   104,   115,
     130,   131,   138,   139,   140,    39,    29,   132,    29,    64,
     119,    66,    64,    69,    64,    70,   129,    69,    65,    69,
      99,    30,   102,   104,    65,    65,    65,    65,    28,    32,
     133,    67,    65,    72,   138,    98,   118,   119,   123,    70,
      39,   126,    66,    17,    90,    90,    86,   100,    88,    88,
     139,    67,    70,   104,   102,   103,    67,   102,    17
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    75,    75,    76,    76,    76,    76,    76,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    79,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    84,    84,    84,    84,    84,    84,    85,
      85,    85,    85,    85,    86,    86,    87,    88,    88,    88,
      89,    89,    89,    90,    90,    91,    91,    91,    92,    93,
      93,    94,    94,    95,    96,    97,    98,    98,    99,    99,
     100,   101,   101,   102,   102,   103,   103,   104,   104,   105,
     105,   106,   106,   107,   107,   107,   108,   108,   108,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   112,   113,
     113,   114,   115,   115,   116,   117,   117,   117,   118,   118,
     119,   119,   120,   120,   121,   121,   121,   121,   122,   122,
     123,   123,   124,   125,   125,   126,   126,   127,   128,   129,
     129,   130,   130,   131,   132,   132,   133,   134,   135,   135,
     135,   136,   136,   137,   137,   138,   138,   139,   139,   140
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     2,
       2,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     4,     2,
       2,     2,     2,     2,     1,     3,     3,     1,     3,     3,
       1,     3,     1,     1,     3,     1,     1,     1,     2,     1,
       1,     0,     3,     4,     4,     3,     4,     5,     1,     3,
       1,     1,     1,     7,     1,     7,     5,     4,     1,     1,
       1,     1,     1,     3,     3,     1,     1,     1,     1,     3,
       1,     3,     1,     2,     1,     3,     1,     3,     3,     1,
       1,     1,     1,     3,     3,     6,     5,     5,     0,     1,
       1,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       0,     3,     2,     0,     1,     3,     1,     1,     3,     2,
       3,     3,     0,     1,     1,     3,     2,     2,     3,     4,
       2,     6,     5,     1,     1,     0,     1,     1,     3,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 87 "mu.y" /* yacc.c:1646  */
    {
  YYACCEPT;
}
#line 1546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 92 "mu.y" /* yacc.c:1646  */
    {
  YYACCEPT;
}
#line 1554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 100 "mu.y" /* yacc.c:1646  */
    {
  parseTree = (yyvsp[-1]); 
}
#line 1562 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 105 "mu.y" /* yacc.c:1646  */
    {
  parseTree = (yyvsp[-1]);
}
#line 1570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 110 "mu.y" /* yacc.c:1646  */
    {
  parseTree = (yyvsp[0]);
}
#line 1578 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 115 "mu.y" /* yacc.c:1646  */
    {
  parseTree = (yyvsp[0]);
}
#line 1586 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 120 "mu.y" /* yacc.c:1646  */
    {
  parseTree = (yyvsp[-1]);
}
#line 1594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 128 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
}
#line 1602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 133 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 1610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 138 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
}
#line 1618 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 143 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 1626 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 148 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
}
#line 1634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 185 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]),0);
}
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 190 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 1650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 212 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_create_Keyword(String);
  (yyval) = pterm_cons((yyval), (yyvsp[0]));
}
#line 1659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 227 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = (yyvsp[-1]);
}
#line 1667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 234 "mu.y" /* yacc.c:1646  */
    {
  if((yyvsp[-1])!=0)
    {
      pterm tmp = pterm_create_Keyword(Attribute);
      (yyval) = pterm_cons((yyvsp[-3]),(yyvsp[-1]));
      (yyval) = pterm_cons(tmp, (yyval));
    }
  else (yyval) = (yyvsp[-3]);
}
#line 1681 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 248 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 253 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 258 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1705 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 263 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 268 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 276 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]),0);
}
#line 1729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 281 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 1737 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 289 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 1745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 297 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]),0);
}
#line 1753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 302 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-2]),(yyval));
}
#line 1762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 308 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-2]),(yyval));
}
#line 1771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 317 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 1779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 322 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]), (yyvsp[0]));
}
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 332 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 1795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 337 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]), (yyvsp[0]));
}
#line 1803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 354 "mu.y" /* yacc.c:1646  */
    {
  if((yyvsp[0]))
    {
      pterm tmp = pterm_create_Keyword(Access);
      (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
      (yyval) = pterm_cons(tmp, (yyval));
    }
  else (yyval) = (yyvsp[-1]);
}
#line 1817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 375 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 1825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 380 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 388 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Application);
  (yyval) = pterm_cons((yyvsp[-3]), (yyvsp[-1]));
  (yyval) = pterm_cons(tmp, (yyval));
}
#line 1843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 398 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Array);
  (yyval) = pterm_cons((yyvsp[-3]), (yyvsp[-1]));
  (yyval) = pterm_cons(tmp, (yyval));
}
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 408 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = (yyvsp[-1]);
}
#line 1861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 416 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(And);
  (yyval) = pterm_cons((yyvsp[-3]),(yyvsp[-1]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 1871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 423 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(If);
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 1882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 435 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 440 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]), (yyvsp[0]));
}
#line 1898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 460 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
  (yyval) = pterm_cons((yyvsp[-6]),(yyval));
}
#line 1908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 472 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
  (yyval) = pterm_cons((yyvsp[-6]),(yyval));
}
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 479 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Then);
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 489 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-3]),(yyval));
}
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 514 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
}
#line 1946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 520 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Implies);
  (yyval) = pterm_cons((yyvsp[0]),(yyvsp[-2]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 1956 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 541 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
}
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 552 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
}
#line 1974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 563 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 1982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 570 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
}
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 581 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Attribute);
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-2]),(yyval));
  (yyval) = pterm_cons(tmp, (yyval));
}
#line 2002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 589 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Attribute);
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-2]),(yyval));
  (yyval) = pterm_cons(tmp, (yyval));
}
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 613 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]),0);
}
#line 2021 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 618 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2029 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 626 "mu.y" /* yacc.c:1646  */
    {
  if((yyvsp[0]))
    (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
  else
    (yyval) = (yyvsp[-1]);
  (yyval) = pterm_cons((yyvsp[-2]),(yyval));
}
#line 2041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 638 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Class);
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 2052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 646 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Enum);
  (yyval) = pterm_cons((yyvsp[-3]),(yyvsp[-1]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 2062 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 653 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(Range);
  (yyval) = pterm_cons((yyvsp[-3]),(yyvsp[-1]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 663 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 676 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 684 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]), (yyvsp[0]));
  (yyval) = pterm_cons((yyvsp[-1]), (yyval));
}
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 690 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]), (yyvsp[0]));
}
#line 2105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 716 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 2113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 721 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 729 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 2129 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 737 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 2137 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 747 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 752 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]),0);
}
#line 2153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 765 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2161 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 780 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = (yyvsp[-1]);
}
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 785 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 798 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 2185 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 803 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 811 "mu.y" /* yacc.c:1646  */
    {
  if((yyvsp[0]))
    (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
  else
    (yyval) = (yyvsp[-1]);
}
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 822 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(VarDef);
  (yyval) = pterm_cons(tmp,(yyvsp[-1]));
}
#line 2213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 831 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(FunDef);
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[-1]));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 838 "mu.y" /* yacc.c:1646  */
    {
  pterm tmp = pterm_create_Keyword(AllocationConstraint);
  (yyval) = pterm_cons(tmp,(yyvsp[-2]));
  tmp = pterm_create_Keyword(Attribute);
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
  (yyval) = pterm_cons(tmp,(yyval));
  tmp = pterm_create_Keyword(FunDef);
  (yyval) = pterm_cons((yyvsp[-3]),(yyval));
  (yyval) = pterm_cons(tmp,(yyval));
}
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 850 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_create_Keyword(Prototype);
  (yyval) = pterm_cons((yyval),(yyvsp[-1]));
}
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 859 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-5]),0);
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
  (yyval) = pterm_cons((yyvsp[-3]),(yyval));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
}
#line 2258 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 867 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons(0,0);
  (yyval) = pterm_cons((yyvsp[-1]),(yyval));
  (yyval) = pterm_cons((yyvsp[-3]),(yyval));
  (yyval) = pterm_cons((yyvsp[-4]),(yyval));
}
#line 2269 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 885 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = 0;
}
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 895 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[0]), 0);
}
#line 2285 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 900 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-2]),(yyvsp[0]));
}
#line 2293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 908 "mu.y" /* yacc.c:1646  */
    {
  (yyval) = pterm_cons((yyvsp[-1]),(yyvsp[0]));
}
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2305 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 913 "mu.y" /* yacc.c:1906  */

