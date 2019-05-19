
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         VLparse
#define yylex           VLlex
#define yyerror         VLerror
#define yylval          VLlval
#define yychar          VLchar
#define yydebug         VLdebug
#define yynerrs         VLnerrs
#define yylloc          VLlloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 2 "parse.y"

/*
 * Copyright (c) 1998-2004 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: parse.y,v 1.201.2.8 2007/04/26 00:50:54 steve Exp $"
#endif

# include "config.h"

# include  "parse_misc.h"
# include  "compiler.h"
# include  "pform.h"
# include  <sstream>

extern void lex_start_table();
extern void lex_end_table();

static svector<PExpr*>* active_range = 0;
static bool active_signed = false;

/* Port declaration lists use this structure for context. */
static struct {
      NetNet::Type port_net_type;
      NetNet::PortType port_type;
      bool sign_flag;
      svector<PExpr*>* range;
} port_declaration_context;

/* Later version of bison (including 1.35) will not compile in stack
   extension if the output is compiled with C++ and either the YYSTYPE
   or YYLTYPE are provided by the source code. However, I can get the
   old behavior back by defining these symbols. */
# define YYSTYPE_IS_TRIVIAL 1
# define YYLTYPE_IS_TRIVIAL 1

/* Recent version of bison expect that the user supply a
   YYLLOC_DEFAULT macro that makes up a yylloc value from existing
   values. I need to supply an explicit version to account for the
   text field, that otherwise won't be copied. */
# define YYLLOC_DEFAULT(Current, Rhs, N)  do {       \
  (Current).first_line   = (Rhs)[1].first_line;      \
  (Current).first_column = (Rhs)[1].first_column;    \
  (Current).last_line    = (Rhs)[N].last_line;       \
  (Current).last_column  = (Rhs)[N].last_column;     \
  (Current).text         = (Rhs)[1].text;   } while (0)

/*
 * These are some common strength pairs that are used as defaults when
 * the user is not otherwise specific.
 */
const static struct str_pair_t pull_strength = { PGate::PULL,  PGate::PULL };
const static struct str_pair_t str_strength = { PGate::STRONG, PGate::STRONG };


/* Line 189 of yacc.c  */
#line 152 "parse.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     SYSTEM_IDENTIFIER = 259,
     STRING = 260,
     PATHPULSE_IDENTIFIER = 261,
     BASED_NUMBER = 262,
     DEC_NUMBER = 263,
     REALTIME = 264,
     K_LE = 265,
     K_GE = 266,
     K_EG = 267,
     K_EQ = 268,
     K_NE = 269,
     K_CEQ = 270,
     K_CNE = 271,
     K_LS = 272,
     K_RS = 273,
     K_RSS = 274,
     K_SG = 275,
     K_PO_POS = 276,
     K_PO_NEG = 277,
     K_PSTAR = 278,
     K_STARP = 279,
     K_LOR = 280,
     K_LAND = 281,
     K_NAND = 282,
     K_NOR = 283,
     K_NXOR = 284,
     K_TRIGGER = 285,
     K_always = 286,
     K_and = 287,
     K_assign = 288,
     K_begin = 289,
     K_buf = 290,
     K_bufif0 = 291,
     K_bufif1 = 292,
     K_case = 293,
     K_casex = 294,
     K_casez = 295,
     K_cmos = 296,
     K_deassign = 297,
     K_default = 298,
     K_defparam = 299,
     K_disable = 300,
     K_edge = 301,
     K_else = 302,
     K_end = 303,
     K_endcase = 304,
     K_endfunction = 305,
     K_endmodule = 306,
     K_endprimitive = 307,
     K_endspecify = 308,
     K_endtable = 309,
     K_endtask = 310,
     K_event = 311,
     K_for = 312,
     K_force = 313,
     K_forever = 314,
     K_fork = 315,
     K_function = 316,
     K_highz0 = 317,
     K_highz1 = 318,
     K_if = 319,
     K_initial = 320,
     K_inout = 321,
     K_input = 322,
     K_integer = 323,
     K_join = 324,
     K_large = 325,
     K_localparam = 326,
     K_macromodule = 327,
     K_medium = 328,
     K_module = 329,
     K_nand = 330,
     K_negedge = 331,
     K_nmos = 332,
     K_nor = 333,
     K_not = 334,
     K_notif0 = 335,
     K_notif1 = 336,
     K_or = 337,
     K_output = 338,
     K_parameter = 339,
     K_pmos = 340,
     K_posedge = 341,
     K_primitive = 342,
     K_pull0 = 343,
     K_pull1 = 344,
     K_pulldown = 345,
     K_pullup = 346,
     K_rcmos = 347,
     K_real = 348,
     K_realtime = 349,
     K_reg = 350,
     K_release = 351,
     K_repeat = 352,
     K_rnmos = 353,
     K_rpmos = 354,
     K_rtran = 355,
     K_rtranif0 = 356,
     K_rtranif1 = 357,
     K_scalared = 358,
     K_signed = 359,
     K_small = 360,
     K_specify = 361,
     K_specparam = 362,
     K_strong0 = 363,
     K_strong1 = 364,
     K_supply0 = 365,
     K_supply1 = 366,
     K_table = 367,
     K_task = 368,
     K_time = 369,
     K_tran = 370,
     K_tranif0 = 371,
     K_tranif1 = 372,
     K_tri = 373,
     K_tri0 = 374,
     K_tri1 = 375,
     K_triand = 376,
     K_trior = 377,
     K_trireg = 378,
     K_vectored = 379,
     K_wait = 380,
     K_wand = 381,
     K_weak0 = 382,
     K_weak1 = 383,
     K_while = 384,
     K_wire = 385,
     K_wor = 386,
     K_xnor = 387,
     K_xor = 388,
     K_Shold = 389,
     K_Speriod = 390,
     K_Srecovery = 391,
     K_Srecrem = 392,
     K_Ssetup = 393,
     K_Swidth = 394,
     K_Ssetuphold = 395,
     KK_attribute = 396,
     K_pll = 397,
     K_mem = 398,
     K_square = 399,
     K_mult = 400,
     K_mult_add = 401,
     K_mult_accum = 402,
     K_TAND = 403,
     UNARY_PREC = 404,
     less_than_K_else = 405
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 72 "parse.y"

      bool flag;

      char letter;

	/* text items are C strings allocated by the lexor using
	   strdup. They can be put into lists with the texts type. */
      char*text;
      list<perm_string>*perm_strings;

      hname_t*hier;

      list<string>*strings;

      struct str_pair_t drive;

      PCase::Item*citem;
      svector<PCase::Item*>*citems;

      lgate*gate;
      svector<lgate>*gates;

      Module::port_t *mport;
      svector<Module::port_t*>*mports;

      named_pexpr_t*named_pexpr;
      svector<named_pexpr_t*>*named_pexprs;
      struct parmvalue_t*parmvalue;

      PExpr*expr;
      svector<PExpr*>*exprs;

      svector<PEEvent*>*event_expr;

      NetNet::Type nettype;
      PGBuiltin::Type gatetype;
      NetNet::PortType porttype;

      PWire*wire;
      svector<PWire*>*wires;

      PEventStatement*event_statement;
      Statement*statement;
      svector<Statement*>*statement_list;

      PTaskFuncArg function_type;

      struct { svector<PExpr*>*range; svector<PExpr*>*delay; } range_delay;
      net_decl_assign_t*net_decl_assign;

      verinum* number;

      verireal* realtime;



/* Line 214 of yacc.c  */
#line 395 "parse.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 420 "parse.cc"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5370

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  196
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  133
/* YYNRULES -- Number of rules.  */
#define YYNRULES  539
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1271

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   405

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   171,     2,   168,     2,   160,   153,     2,
     166,   167,   158,   156,   163,   157,   176,   159,   177,   178,
       2,     2,     2,     2,     2,     2,     2,     2,   150,   165,
     154,   164,   155,   149,   169,     2,   185,     2,     2,     2,
     182,     2,     2,     2,     2,     2,     2,   188,   190,     2,
     192,   193,   187,     2,     2,     2,     2,     2,     2,     2,
       2,   172,     2,   173,   152,   195,     2,     2,   180,     2,
       2,     2,   181,     2,   184,     2,     2,     2,   183,     2,
     189,     2,   191,   194,   186,     2,     2,     2,     2,     2,
     179,     2,     2,   174,   151,   175,   170,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   161,   162
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    11,    13,    15,    18,
      22,    25,    26,    30,    32,    34,    38,    44,    49,    53,
      57,    61,    65,    69,    73,    77,    81,    85,    89,    93,
      97,   101,   103,   106,   108,   109,   113,   117,   120,   124,
     127,   129,   133,   137,   141,   143,   144,   148,   150,   153,
     157,   160,   165,   168,   173,   180,   189,   191,   192,   194,
     200,   202,   204,   206,   208,   210,   219,   225,   231,   237,
     243,   249,   255,   257,   258,   260,   262,   264,   266,   268,
     270,   272,   274,   277,   282,   287,   289,   293,   297,   300,
     303,   305,   307,   310,   313,   316,   319,   322,   325,   328,
     331,   334,   337,   340,   343,   347,   351,   355,   359,   363,
     367,   371,   375,   379,   383,   387,   391,   395,   399,   403,
     407,   411,   415,   419,   423,   427,   431,   435,   439,   445,
     447,   453,   457,   459,   460,   463,   465,   467,   469,   471,
     473,   478,   485,   490,   495,   499,   503,   510,   515,   521,
     526,   532,   537,   543,   546,   548,   551,   556,   562,   566,
     569,   574,   578,   580,   582,   584,   586,   588,   590,   592,
     594,   596,   598,   600,   602,   604,   606,   608,   610,   612,
     614,   616,   618,   620,   622,   624,   626,   628,   630,   632,
     636,   638,   642,   644,   648,   650,   654,   658,   665,   672,
     679,   686,   695,   697,   698,   700,   701,   703,   708,   711,
     715,   717,   722,   729,   733,   737,   741,   743,   744,   745,
     756,   758,   760,   763,   767,   771,   772,   773,   778,   781,
     785,   790,   797,   804,   811,   817,   823,   830,   837,   844,
     851,   857,   860,   864,   868,   873,   879,   885,   892,   896,
     900,   907,   914,   919,   924,   930,   937,   944,   951,   958,
     965,   969,   973,   974,   982,   983,   992,   995,   999,  1003,
    1006,  1012,  1016,  1020,  1030,  1036,  1039,  1041,  1043,  1044,
    1048,  1052,  1054,  1056,  1058,  1060,  1062,  1064,  1066,  1068,
    1070,  1072,  1074,  1076,  1080,  1082,  1083,  1087,  1088,  1093,
    1095,  1099,  1103,  1105,  1106,  1110,  1111,  1116,  1118,  1122,
    1127,  1132,  1135,  1138,  1139,  1145,  1150,  1152,  1156,  1158,
    1164,  1168,  1176,  1178,  1179,  1181,  1188,  1193,  1198,  1200,
    1204,  1210,  1216,  1221,  1225,  1227,  1229,  1231,  1233,  1239,
    1241,  1242,  1244,  1246,  1248,  1250,  1252,  1253,  1255,  1259,
    1266,  1268,  1272,  1276,  1279,  1282,  1289,  1296,  1307,  1316,
    1327,  1338,  1351,  1364,  1375,  1383,  1385,  1389,  1391,  1394,
    1400,  1404,  1412,  1424,  1432,  1444,  1452,  1464,  1472,  1484,
    1486,  1488,  1490,  1496,  1500,  1506,  1513,  1520,  1524,  1526,
    1531,  1535,  1542,  1546,  1554,  1558,  1566,  1568,  1572,  1574,
    1576,  1577,  1580,  1583,  1588,  1593,  1597,  1599,  1600,  1602,
    1604,  1607,  1610,  1614,  1621,  1623,  1629,  1633,  1639,  1643,
    1647,  1648,  1656,  1659,  1664,  1665,  1673,  1676,  1681,  1685,
    1689,  1692,  1696,  1702,  1709,  1716,  1723,  1730,  1737,  1744,
    1750,  1758,  1764,  1772,  1786,  1798,  1812,  1818,  1824,  1830,
    1833,  1837,  1842,  1849,  1854,  1859,  1865,  1871,  1877,  1887,
    1893,  1903,  1909,  1915,  1918,  1924,  1927,  1930,  1934,  1937,
    1939,  1941,  1943,  1948,  1954,  1959,  1965,  1970,  1976,  1979,
    1981,  1983,  1984,  1985,  1990,  1992,  1994,  1999,  2001,  2004,
    2006,  2009,  2016,  2022,  2024,  2025,  2027,  2030,  2032,  2034,
    2036,  2038,  2040,  2042,  2044,  2046,  2048,  2050,  2052,  2054,
    2056,  2058,  2060,  2062,  2064,  2066,  2068,  2070,  2072,  2074,
    2076,  2078,  2080,  2082,  2084,  2088,  2092,  2096,  2101,  2103,
    2106,  2108,  2112,  2114,  2115,  2118,  2119,  2122,  2127,  2138
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     197,     0,    -1,   198,    -1,    -1,   217,    -1,   198,   217,
      -1,     7,    -1,     8,    -1,     8,     7,    -1,    23,   201,
      24,    -1,    23,    24,    -1,    -1,   201,   163,   202,    -1,
     202,    -1,     3,    -1,     3,   164,   225,    -1,    95,   240,
     282,   286,   165,    -1,    95,   240,   286,   165,    -1,    68,
     286,   165,    -1,   114,   286,   165,    -1,    93,   235,   165,
      -1,    94,   235,   165,    -1,    84,   263,   165,    -1,    71,
     268,   165,    -1,    95,     1,   165,    -1,    68,     1,   165,
      -1,   114,     1,   165,    -1,    93,     1,   165,    -1,    94,
       1,   165,    -1,    84,     1,   165,    -1,    71,     1,   165,
      -1,   203,    -1,   204,   203,    -1,   204,    -1,    -1,   227,
     150,   306,    -1,    43,   150,   306,    -1,    43,   306,    -1,
       1,   150,   306,    -1,   207,   206,    -1,   206,    -1,   166,
     105,   167,    -1,   166,    73,   167,    -1,   166,    70,   167,
      -1,   208,    -1,    -1,   234,   164,   225,    -1,   210,    -1,
     282,   210,    -1,   211,   163,   210,    -1,   168,   216,    -1,
     168,   166,   215,   167,    -1,   168,   216,    -1,   168,   166,
     215,   167,    -1,   168,   166,   215,   163,   215,   167,    -1,
     168,   166,   215,   163,   215,   163,   215,   167,    -1,   213,
      -1,    -1,   225,    -1,   225,   150,   225,   150,   225,    -1,
       8,    -1,     9,    -1,     3,    -1,   245,    -1,   328,    -1,
     141,   166,     3,   163,     5,   163,     5,   167,    -1,   166,
     220,   163,   221,   167,    -1,   166,   221,   163,   220,   167,
      -1,   166,   220,   163,    63,   167,    -1,   166,   221,   163,
      62,   167,    -1,   166,    63,   163,   220,   167,    -1,   166,
      62,   163,   221,   167,    -1,   218,    -1,    -1,   110,    -1,
     108,    -1,    88,    -1,   127,    -1,   111,    -1,   109,    -1,
      89,    -1,   128,    -1,   169,   234,    -1,   169,   166,   223,
     167,    -1,   169,   166,     1,   167,    -1,   224,    -1,   223,
      82,   224,    -1,   223,   163,   224,    -1,    86,   225,    -1,
      76,   225,    -1,   225,    -1,   228,    -1,   156,   228,    -1,
     157,   228,    -1,   170,   228,    -1,   153,   228,    -1,   171,
     228,    -1,   151,   228,    -1,   152,   228,    -1,    27,   228,
      -1,    28,   228,    -1,    29,   228,    -1,   171,     1,    -1,
     152,     1,    -1,   225,   152,   225,    -1,   225,   158,   225,
      -1,   225,   159,   225,    -1,   225,   160,   225,    -1,   225,
     156,   225,    -1,   225,   157,   225,    -1,   225,   153,   225,
      -1,   225,   151,   225,    -1,   225,    27,   225,    -1,   225,
      28,   225,    -1,   225,    29,   225,    -1,   225,   154,   225,
      -1,   225,   155,   225,    -1,   225,    17,   225,    -1,   225,
      18,   225,    -1,   225,    19,   225,    -1,   225,    13,   225,
      -1,   225,    15,   225,    -1,   225,    10,   225,    -1,   225,
      11,   225,    -1,   225,    14,   225,    -1,   225,    16,   225,
      -1,   225,    25,   225,    -1,   225,    26,   225,    -1,   225,
     149,   225,   150,   225,    -1,   225,    -1,   225,   150,   225,
     150,   225,    -1,   227,   163,   225,    -1,   225,    -1,    -1,
     227,   163,    -1,   199,    -1,     9,    -1,     5,    -1,   234,
      -1,     4,    -1,   234,   172,   225,   173,    -1,   234,   172,
     225,   150,   225,   173,    -1,   234,   166,   227,   167,    -1,
       4,   166,   227,   167,    -1,   166,   226,   167,    -1,   174,
     227,   175,    -1,   174,   225,   174,   227,   175,   175,    -1,
      67,   283,   235,   165,    -1,    67,   104,   283,   235,   165,
      -1,    83,   283,   235,   165,    -1,    83,   104,   283,   235,
     165,    -1,    66,   283,   235,   165,    -1,    66,   104,   283,
     235,   165,    -1,   200,   203,    -1,   229,    -1,   230,   229,
      -1,     3,   166,   227,   167,    -1,     3,   282,   166,   227,
     167,    -1,   166,   227,   167,    -1,     3,   282,    -1,     3,
     166,   280,   167,    -1,   232,   163,   231,    -1,   231,    -1,
      32,    -1,    75,    -1,    82,    -1,    78,    -1,   133,    -1,
     132,    -1,    35,    -1,    36,    -1,    37,    -1,    79,    -1,
      80,    -1,    81,    -1,    77,    -1,    98,    -1,    85,    -1,
      99,    -1,    41,    -1,    92,    -1,   115,    -1,   100,    -1,
     116,    -1,   117,    -1,   101,    -1,   102,    -1,   142,    -1,
       3,    -1,   234,   176,     3,    -1,     3,    -1,   235,   163,
       3,    -1,   276,    -1,   236,   163,   276,    -1,   238,    -1,
     237,   163,   238,    -1,   237,   163,     3,    -1,   200,    67,
     239,   240,   283,     3,    -1,   200,    66,   239,   240,   283,
       3,    -1,   200,    83,   239,   240,   283,     3,    -1,   200,
      83,   261,   240,   283,     3,    -1,   200,    83,   261,   240,
     283,     3,   164,   225,    -1,   260,    -1,    -1,   104,    -1,
      -1,   234,    -1,   234,   172,   225,   173,    -1,   234,   282,
      -1,   174,   227,   175,    -1,   234,    -1,   234,   172,   225,
     173,    -1,   234,   172,   225,   150,   225,   173,    -1,   174,
     227,   175,    -1,   241,   164,   225,    -1,   244,   163,   243,
      -1,   243,    -1,    -1,    -1,   200,   248,     3,   246,   251,
     250,   165,   247,   257,    51,    -1,    74,    -1,    72,    -1,
     283,   214,    -1,   166,   236,   167,    -1,   166,   237,   167,
      -1,    -1,    -1,   168,   166,   252,   167,    -1,    84,   262,
      -1,   252,   163,   262,    -1,   252,   163,    84,   262,    -1,
     200,   260,   240,   249,   235,   165,    -1,   200,   260,   240,
     249,   259,   165,    -1,   200,   260,   240,   218,   259,   165,
      -1,   123,   209,   249,   235,   165,    -1,   281,   240,   249,
     235,   165,    -1,   281,   260,   240,   283,   235,   165,    -1,
      83,   261,   240,   283,   235,   165,    -1,    67,   261,   240,
     283,   235,   165,    -1,    66,   261,   240,   283,   235,   165,
      -1,   281,   240,   249,     1,   165,    -1,   200,   203,    -1,
      44,   211,   165,    -1,    56,   235,   165,    -1,   200,   233,
     232,   165,    -1,   200,   233,   213,   232,   165,    -1,   200,
     233,   218,   232,   165,    -1,   200,   233,   218,   213,   232,
     165,    -1,    91,   232,   165,    -1,    90,   232,   165,    -1,
      91,   166,   221,   167,   232,   165,    -1,    90,   166,   220,
     167,   232,   165,    -1,     3,   272,   232,   165,    -1,     3,
     272,     1,   165,    -1,    33,   219,   214,   244,   165,    -1,
      33,   143,   166,   227,   167,   165,    -1,    33,   144,   166,
     227,   167,   165,    -1,    33,   145,   166,   227,   167,   165,
      -1,    33,   146,   166,   227,   167,   165,    -1,    33,   147,
     166,   227,   167,   165,    -1,   200,    31,   302,    -1,   200,
      65,   302,    -1,    -1,   113,     3,   165,   254,   309,   306,
      55,    -1,    -1,    61,   284,     3,   165,   255,   230,   302,
      50,    -1,   106,    53,    -1,   106,   289,    53,    -1,   106,
       1,    53,    -1,     1,   165,    -1,    33,     1,   164,   225,
     165,    -1,    33,     1,   165,    -1,    61,     1,    50,    -1,
     141,   166,     3,   163,     5,   163,     5,   167,   165,    -1,
     141,   166,     1,   167,   165,    -1,   256,   253,    -1,   253,
      -1,   256,    -1,    -1,     3,   164,   225,    -1,   259,   163,
     258,    -1,   258,    -1,   130,    -1,   118,    -1,   120,    -1,
     110,    -1,   126,    -1,   121,    -1,   119,    -1,   111,    -1,
     131,    -1,   122,    -1,    95,    -1,     3,   164,   225,    -1,
     266,    -1,    -1,   282,   264,   266,    -1,    -1,   104,   282,
     265,   266,    -1,   262,    -1,   266,   163,   262,    -1,     3,
     164,   225,    -1,   271,    -1,    -1,   282,   269,   271,    -1,
      -1,   104,   282,   270,   271,    -1,   267,    -1,   271,   163,
     267,    -1,   168,   166,   227,   167,    -1,   168,   166,   274,
     167,    -1,   168,     8,    -1,   168,     1,    -1,    -1,   176,
       3,   166,   225,   167,    -1,   176,     3,   166,   167,    -1,
     273,    -1,   274,   163,   273,    -1,   277,    -1,   176,     3,
     166,   277,   167,    -1,   174,   278,   175,    -1,   176,     3,
     166,   174,   278,   175,   167,    -1,   275,    -1,    -1,     3,
      -1,     3,   172,   225,   150,   225,   173,    -1,     3,   172,
     225,   173,    -1,     3,   172,     1,   173,    -1,   277,    -1,
     278,   163,   277,    -1,   176,     3,   166,   225,   167,    -1,
     176,     3,   166,     1,   167,    -1,   176,     3,   166,   167,
      -1,   280,   163,   279,    -1,   279,    -1,    67,    -1,    83,
      -1,    66,    -1,   172,   225,   150,   225,   173,    -1,   282,
      -1,    -1,   282,    -1,    68,    -1,    93,    -1,    94,    -1,
     114,    -1,    -1,     3,    -1,     3,   164,   225,    -1,     3,
     172,   225,   150,   225,   173,    -1,   285,    -1,   286,   163,
     285,    -1,   107,   297,   165,    -1,   293,   165,    -1,   290,
     165,    -1,    64,   166,   225,   167,   293,   165,    -1,    64,
     166,   225,   167,   290,   165,    -1,   134,   166,   299,   163,
     299,   163,   215,   300,   167,   165,    -1,   135,   166,   299,
     163,   215,   300,   167,   165,    -1,   136,   166,   299,   163,
     299,   163,   215,   300,   167,   165,    -1,   138,   166,   299,
     163,   299,   163,   215,   300,   167,   165,    -1,   140,   166,
     299,   163,   299,   163,   215,   163,   215,   300,   167,   165,
      -1,   137,   166,   299,   163,   299,   163,   215,   163,   215,
     300,   167,   165,    -1,   139,   166,   299,   163,   215,   163,
     225,   300,   167,   165,    -1,   139,   166,   299,   163,   215,
     167,   165,    -1,   215,    -1,   288,   163,   215,    -1,   287,
      -1,   289,   287,    -1,   291,   164,   166,   288,   167,    -1,
     291,   164,   216,    -1,   166,    86,   295,   298,    12,     3,
     167,    -1,   166,    86,   295,   298,    12,   166,   228,   292,
     225,   167,   167,    -1,   166,    86,   295,   298,    20,     3,
     167,    -1,   166,    86,   295,   298,    20,   166,   228,   292,
     225,   167,   167,    -1,   166,    76,   295,   298,    12,     3,
     167,    -1,   166,    76,   295,   298,    12,   166,   228,   292,
     225,   167,   167,    -1,   166,    76,   295,   298,    20,     3,
     167,    -1,   166,    76,   295,   298,    20,   166,   228,   292,
     225,   167,   167,    -1,    21,    -1,    22,    -1,   150,    -1,
     294,   164,   166,   288,   167,    -1,   294,   164,   216,    -1,
     294,   164,   166,     1,   167,    -1,   166,   295,   298,    12,
     295,   167,    -1,   166,   295,   298,    20,   295,   167,    -1,
     166,     1,   167,    -1,     3,    -1,     3,   172,   228,   173,
      -1,   295,   163,     3,    -1,   295,   163,     3,   172,   228,
     173,    -1,     3,   164,   225,    -1,     3,   164,   225,   150,
     225,   150,   225,    -1,     6,   164,   225,    -1,     6,   164,
     166,   225,   163,   225,   167,    -1,   296,    -1,   297,   163,
     296,    -1,   156,    -1,   157,    -1,    -1,    86,   225,    -1,
      76,   225,    -1,    86,   228,   148,   225,    -1,    76,   228,
     148,   225,    -1,   228,   148,   225,    -1,   228,    -1,    -1,
     301,    -1,   163,    -1,   163,   234,    -1,   301,   163,    -1,
     301,   163,   234,    -1,   301,   163,   234,   172,   228,   173,
      -1,     3,    -1,    33,   241,   164,   225,   165,    -1,    42,
     241,   165,    -1,    58,   241,   164,   225,   165,    -1,    96,
     241,   165,    -1,    34,   305,    48,    -1,    -1,    34,   150,
       3,   303,   205,   305,    48,    -1,    34,    48,    -1,    34,
     150,     3,    48,    -1,    -1,    60,   150,     3,   304,   205,
     305,    69,    -1,    60,    69,    -1,    60,   150,     3,    69,
      -1,    45,   234,   165,    -1,    30,   234,   165,    -1,    59,
     302,    -1,    60,   305,    69,    -1,    97,   166,   225,   167,
     302,    -1,    38,   166,   225,   167,   207,    49,    -1,    39,
     166,   225,   167,   207,    49,    -1,    40,   166,   225,   167,
     207,    49,    -1,    38,   166,   225,   167,     1,    49,    -1,
      39,   166,   225,   167,     1,    49,    -1,    40,   166,   225,
     167,     1,    49,    -1,    64,   166,   225,   167,   306,    -1,
      64,   166,   225,   167,   306,    47,   306,    -1,    64,   166,
       1,   167,   306,    -1,    64,   166,     1,   167,   306,    47,
     306,    -1,    57,   166,   242,   164,   225,   165,   225,   165,
     242,   164,   225,   167,   302,    -1,    57,   166,   242,   164,
     225,   165,   225,   165,     1,   167,   302,    -1,    57,   166,
     242,   164,   225,   165,     1,   165,   242,   164,   225,   167,
     302,    -1,    57,   166,     1,   167,   302,    -1,   129,   166,
     225,   167,   302,    -1,   129,   166,     1,   167,   302,    -1,
     212,   306,    -1,   222,   200,   306,    -1,   169,   158,   200,
     306,    -1,   169,   166,   158,   167,   200,   306,    -1,   242,
     164,   225,   165,    -1,   242,    10,   225,   165,    -1,   242,
     164,   212,   225,   165,    -1,   242,    10,   212,   225,   165,
      -1,   242,   164,   222,   225,   165,    -1,   242,   164,    97,
     166,   225,   167,   222,   225,   165,    -1,   242,    10,   222,
     225,   165,    -1,   242,    10,    97,   166,   225,   167,   222,
     225,   165,    -1,   125,   166,   225,   167,   306,    -1,     4,
     166,   227,   167,   165,    -1,     4,   165,    -1,   234,   166,
     227,   167,   165,    -1,   234,   165,    -1,     1,   165,    -1,
     305,   200,   302,    -1,   200,   302,    -1,   302,    -1,   165,
      -1,   203,    -1,    67,   283,   235,   165,    -1,    67,   104,
     283,   235,   165,    -1,    83,   283,   235,   165,    -1,    83,
     104,   283,   235,   165,    -1,    66,   283,   235,   165,    -1,
      66,   104,   283,   235,   165,    -1,   308,   307,    -1,   307,
      -1,   308,    -1,    -1,    -1,   112,   311,   312,    54,    -1,
     314,    -1,   315,    -1,   319,   150,   321,   165,    -1,   313,
      -1,   314,   313,    -1,   316,    -1,   315,   316,    -1,   319,
     150,   320,   150,   321,   165,    -1,    65,     3,   164,   199,
     165,    -1,   317,    -1,    -1,   320,    -1,   319,   320,    -1,
     177,    -1,   178,    -1,   179,    -1,   149,    -1,   180,    -1,
     158,    -1,   160,    -1,   181,    -1,   182,    -1,   183,    -1,
     184,    -1,   185,    -1,   186,    -1,   187,    -1,   188,    -1,
     189,    -1,   190,    -1,   191,    -1,   192,    -1,   193,    -1,
     194,    -1,   195,    -1,   156,    -1,   177,    -1,   178,    -1,
     179,    -1,   157,    -1,    67,   235,   165,    -1,    83,     3,
     165,    -1,    95,     3,   165,    -1,    95,    83,     3,   165,
      -1,   322,    -1,   323,   322,    -1,     3,    -1,   324,   163,
       3,    -1,    95,    -1,    -1,   164,   225,    -1,    -1,    67,
       3,    -1,   327,   163,    67,     3,    -1,    87,     3,   166,
     324,   167,   165,   323,   318,   310,    52,    -1,    87,     3,
     166,    83,   325,     3,   326,   163,   327,   167,   165,   310,
      52,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   243,   243,   243,   246,   247,   250,   252,   254,   262,
     263,   264,   268,   274,   283,   290,   313,   316,   319,   322,
     325,   328,   331,   332,   337,   341,   345,   349,   353,   357,
     361,   368,   369,   373,   374,   378,   385,   390,   395,   402,
     408,   416,   417,   418,   422,   423,   427,   441,   442,   446,
     450,   455,   463,   468,   473,   479,   489,   490,   494,   498,
     504,   515,   526,   536,   537,   538,   547,   551,   555,   559,
     563,   567,   574,   575,   579,   580,   581,   582,   586,   587,
     588,   589,   593,   604,   611,   618,   620,   626,   635,   643,
     651,   662,   664,   666,   672,   678,   684,   690,   696,   702,
     708,   714,   720,   725,   730,   736,   742,   748,   754,   760,
     766,   772,   778,   784,   790,   796,   802,   808,   814,   820,
     826,   832,   838,   844,   850,   856,   862,   868,   874,   883,
     885,   911,   916,   922,   927,   935,   942,   948,   954,   961,
     968,   976,   985,   992,   998,  1000,  1015,  1036,  1043,  1050,
    1058,  1066,  1074,  1082,  1091,  1093,  1110,  1120,  1133,  1144,
    1160,  1172,  1180,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,
    1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1221,  1225,
    1237,  1243,  1269,  1275,  1284,  1290,  1296,  1317,  1332,  1347,
    1362,  1377,  1403,  1404,  1407,  1407,  1413,  1420,  1435,  1446,
    1458,  1465,  1474,  1483,  1493,  1502,  1508,  1518,  1521,  1517,
    1530,  1530,  1532,  1537,  1538,  1539,  1545,  1547,  1551,  1552,
    1553,  1557,  1566,  1575,  1583,  1589,  1597,  1601,  1609,  1614,
    1619,  1630,  1635,  1636,  1643,  1647,  1651,  1655,  1662,  1666,
    1671,  1675,  1683,  1689,  1697,  1700,  1703,  1706,  1709,  1712,
    1717,  1723,  1736,  1735,  1756,  1755,  1773,  1776,  1780,  1789,
    1795,  1801,  1807,  1816,  1823,  1829,  1830,  1834,  1835,  1847,
    1857,  1863,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,
    1877,  1878,  1882,  1886,  1903,  1904,  1904,  1909,  1909,  1917,
    1918,  1928,  1945,  1946,  1946,  1951,  1951,  1959,  1960,  1980,
    1986,  1992,  2004,  2010,  2014,  2021,  2031,  2036,  2059,  2067,
    2078,  2087,  2096,  2097,  2113,  2121,  2143,  2160,  2176,  2178,
    2191,  2198,  2206,  2216,  2222,  2230,  2231,  2232,  2236,  2253,
    2254,  2259,  2260,  2261,  2262,  2263,  2264,  2273,  2278,  2287,
    2300,  2306,  2315,  2316,  2319,  2322,  2325,  2328,  2332,  2336,
    2340,  2344,  2349,  2354,  2359,  2365,  2366,  2370,  2371,  2375,
    2376,  2380,  2381,  2382,  2383,  2384,  2385,  2386,  2387,  2391,
    2392,  2393,  2397,  2398,  2399,  2406,  2409,  2412,  2419,  2425,
    2431,  2437,  2446,  2458,  2464,  2468,  2476,  2477,  2481,  2482,
    2483,  2487,  2489,  2491,  2495,  2499,  2503,  2509,  2510,  2514,
    2516,  2518,  2520,  2522,  2526,  2538,  2545,  2556,  2562,  2575,
    2583,  2582,  2595,  2601,  2616,  2615,  2628,  2634,  2642,  2649,
    2656,  2662,  2669,  2675,  2681,  2687,  2693,  2695,  2697,  2699,
    2705,  2711,  2715,  2719,  2726,  2731,  2736,  2740,  2744,  2748,
    2756,  2768,  2777,  2786,  2792,  2798,  2806,  2814,  2820,  2828,
    2835,  2843,  2852,  2860,  2868,  2876,  2884,  2892,  2899,  2909,
    2910,  2915,  2917,  2924,  2931,  2938,  2945,  2952,  2962,  2968,
    2973,  2976,  2980,  2980,  2986,  2987,  2991,  3004,  3010,  3019,
    3025,  3034,  3048,  3060,  3061,  3065,  3071,  3083,  3084,  3085,
    3086,  3087,  3088,  3089,  3090,  3091,  3092,  3093,  3094,  3095,
    3096,  3097,  3098,  3099,  3100,  3101,  3102,  3103,  3104,  3105,
    3109,  3110,  3111,  3112,  3116,  3118,  3124,  3130,  3139,  3141,
    3150,  3156,  3164,  3164,  3167,  3168,  3172,  3178,  3191,  3206
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "SYSTEM_IDENTIFIER",
  "STRING", "PATHPULSE_IDENTIFIER", "BASED_NUMBER", "DEC_NUMBER",
  "REALTIME", "K_LE", "K_GE", "K_EG", "K_EQ", "K_NE", "K_CEQ", "K_CNE",
  "K_LS", "K_RS", "K_RSS", "K_SG", "K_PO_POS", "K_PO_NEG", "K_PSTAR",
  "K_STARP", "K_LOR", "K_LAND", "K_NAND", "K_NOR", "K_NXOR", "K_TRIGGER",
  "K_always", "K_and", "K_assign", "K_begin", "K_buf", "K_bufif0",
  "K_bufif1", "K_case", "K_casex", "K_casez", "K_cmos", "K_deassign",
  "K_default", "K_defparam", "K_disable", "K_edge", "K_else", "K_end",
  "K_endcase", "K_endfunction", "K_endmodule", "K_endprimitive",
  "K_endspecify", "K_endtable", "K_endtask", "K_event", "K_for", "K_force",
  "K_forever", "K_fork", "K_function", "K_highz0", "K_highz1", "K_if",
  "K_initial", "K_inout", "K_input", "K_integer", "K_join", "K_large",
  "K_localparam", "K_macromodule", "K_medium", "K_module", "K_nand",
  "K_negedge", "K_nmos", "K_nor", "K_not", "K_notif0", "K_notif1", "K_or",
  "K_output", "K_parameter", "K_pmos", "K_posedge", "K_primitive",
  "K_pull0", "K_pull1", "K_pulldown", "K_pullup", "K_rcmos", "K_real",
  "K_realtime", "K_reg", "K_release", "K_repeat", "K_rnmos", "K_rpmos",
  "K_rtran", "K_rtranif0", "K_rtranif1", "K_scalared", "K_signed",
  "K_small", "K_specify", "K_specparam", "K_strong0", "K_strong1",
  "K_supply0", "K_supply1", "K_table", "K_task", "K_time", "K_tran",
  "K_tranif0", "K_tranif1", "K_tri", "K_tri0", "K_tri1", "K_triand",
  "K_trior", "K_trireg", "K_vectored", "K_wait", "K_wand", "K_weak0",
  "K_weak1", "K_while", "K_wire", "K_wor", "K_xnor", "K_xor", "K_Shold",
  "K_Speriod", "K_Srecovery", "K_Srecrem", "K_Ssetup", "K_Swidth",
  "K_Ssetuphold", "KK_attribute", "K_pll", "K_mem", "K_square", "K_mult",
  "K_mult_add", "K_mult_accum", "K_TAND", "'?'", "':'", "'|'", "'^'",
  "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY_PREC",
  "less_than_K_else", "','", "'='", "';'", "'('", "')'", "'#'", "'@'",
  "'~'", "'!'", "'['", "']'", "'{'", "'}'", "'.'", "'0'", "'1'", "'x'",
  "'b'", "'f'", "'F'", "'l'", "'h'", "'B'", "'r'", "'R'", "'M'", "'n'",
  "'N'", "'p'", "'P'", "'Q'", "'q'", "'_'", "$accept", "main",
  "source_file", "number", "attribute_list_opt", "attribute_list",
  "attribute", "block_item_decl", "block_item_decls",
  "block_item_decls_opt", "case_item", "case_items", "charge_strength",
  "charge_strength_opt", "defparam_assign", "defparam_assign_list",
  "delay1", "delay3", "delay3_opt", "delay_value", "delay_value_simple",
  "description", "drive_strength", "drive_strength_opt", "dr_strength0",
  "dr_strength1", "event_control", "event_expression_list",
  "event_expression", "expression", "expr_mintypmax", "expression_list",
  "expr_primary", "function_item", "function_item_list", "gate_instance",
  "gate_instance_list", "gatetype", "identifier", "list_of_identifiers",
  "list_of_ports", "list_of_port_declarations", "port_declaration",
  "net_type_opt", "signed_opt", "lavalue", "lpvalue", "assign",
  "assign_list", "module", "$@1", "$@2", "module_start", "range_delay",
  "module_port_list_opt", "module_parameter_port_list_opt",
  "module_parameter_port_list", "module_item", "$@3", "$@4",
  "module_item_list", "module_item_list_opt", "net_decl_assign",
  "net_decl_assigns", "net_type", "var_type", "parameter_assign",
  "parameter_assign_decl", "$@5", "$@6", "parameter_assign_list",
  "localparam_assign", "localparam_assign_decl", "$@7", "$@8",
  "localparam_assign_list", "parameter_value_opt",
  "parameter_value_byname", "parameter_value_byname_list", "port",
  "port_opt", "port_reference", "port_reference_list", "port_name",
  "port_name_list", "port_type", "range", "range_opt",
  "function_range_or_type_opt", "register_variable",
  "register_variable_list", "specify_item", "specify_delay_value_list",
  "specify_item_list", "specify_edge_path_decl", "specify_edge_path",
  "polarity_operator", "specify_simple_path_decl", "specify_simple_path",
  "specify_path_identifiers", "specparam", "specparam_list",
  "spec_polarity", "spec_reference_event", "spec_notifier_opt",
  "spec_notifier", "statement", "$@9", "$@10", "statement_list",
  "statement_opt", "task_item", "task_item_list", "task_item_list_opt",
  "udp_body", "$@11", "udp_entry_list", "udp_comb_entry",
  "udp_comb_entry_list", "udp_sequ_entry_list", "udp_sequ_entry",
  "udp_initial", "udp_init_opt", "udp_input_list", "udp_input_sym",
  "udp_output_sym", "udp_port_decl", "udp_port_decls", "udp_port_list",
  "udp_reg_opt", "udp_initial_expr_opt", "udp_input_declaration_list",
  "udp_primitive", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,    63,
      58,   124,    94,    38,    60,    62,    43,    45,    42,    47,
      37,   404,   405,    44,    61,    59,    40,    41,    35,    64,
     126,    33,    91,    93,   123,   125,    46,    48,    49,   120,
      98,   102,    70,   108,   104,    66,   114,    82,    77,   110,
      78,   112,    80,    81,   113,    95
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   196,   197,   197,   198,   198,   199,   199,   199,   200,
     200,   200,   201,   201,   202,   202,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   205,   206,   206,   206,   206,   207,
     207,   208,   208,   208,   209,   209,   210,   211,   211,   211,
     212,   212,   213,   213,   213,   213,   214,   214,   215,   215,
     216,   216,   216,   217,   217,   217,   218,   218,   218,   218,
     218,   218,   219,   219,   220,   220,   220,   220,   221,   221,
     221,   221,   222,   222,   222,   223,   223,   223,   224,   224,
     224,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   226,
     226,   227,   227,   227,   227,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   229,
     229,   229,   229,   229,   230,   230,   231,   231,   231,   231,
     231,   232,   232,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   234,   234,
     235,   235,   236,   236,   237,   237,   237,   238,   238,   238,
     238,   238,   239,   239,   240,   240,   241,   241,   241,   241,
     242,   242,   242,   242,   243,   244,   244,   246,   247,   245,
     248,   248,   249,   250,   250,   250,   251,   251,   252,   252,
     252,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   254,   253,   255,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   256,   256,   257,   257,   258,
     259,   259,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   262,   263,   264,   263,   265,   263,   266,
     266,   267,   268,   269,   268,   270,   268,   271,   271,   272,
     272,   272,   272,   272,   273,   273,   274,   274,   275,   275,
     275,   275,   276,   276,   277,   277,   277,   277,   278,   278,
     279,   279,   279,   280,   280,   281,   281,   281,   282,   283,
     283,   284,   284,   284,   284,   284,   284,   285,   285,   285,
     286,   286,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   288,   288,   289,   289,   290,
     290,   291,   291,   291,   291,   291,   291,   291,   291,   292,
     292,   292,   293,   293,   293,   294,   294,   294,   295,   295,
     295,   295,   296,   296,   296,   296,   297,   297,   298,   298,
     298,   299,   299,   299,   299,   299,   299,   300,   300,   301,
     301,   301,   301,   301,   301,   302,   302,   302,   302,   302,
     303,   302,   302,   302,   304,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   305,   305,   306,
     306,   307,   307,   307,   307,   307,   307,   307,   308,   308,
     309,   309,   311,   310,   312,   312,   313,   314,   314,   315,
     315,   316,   317,   318,   318,   319,   319,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     321,   321,   321,   321,   322,   322,   322,   322,   323,   323,
     324,   324,   325,   325,   326,   326,   327,   327,   328,   328
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     2,     3,
       2,     0,     3,     1,     1,     3,     5,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     1,     0,     3,     3,     2,     3,     2,
       1,     3,     3,     3,     1,     0,     3,     1,     2,     3,
       2,     4,     2,     4,     6,     8,     1,     0,     1,     5,
       1,     1,     1,     1,     1,     8,     5,     5,     5,     5,
       5,     5,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     1,     3,     3,     2,     2,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     5,     1,
       5,     3,     1,     0,     2,     1,     1,     1,     1,     1,
       4,     6,     4,     4,     3,     3,     6,     4,     5,     4,
       5,     4,     5,     2,     1,     2,     4,     5,     3,     2,
       4,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     6,     6,     6,
       6,     8,     1,     0,     1,     0,     1,     4,     2,     3,
       1,     4,     6,     3,     3,     3,     1,     0,     0,    10,
       1,     1,     2,     3,     3,     0,     0,     4,     2,     3,
       4,     6,     6,     6,     5,     5,     6,     6,     6,     6,
       5,     2,     3,     3,     4,     5,     5,     6,     3,     3,
       6,     6,     4,     4,     5,     6,     6,     6,     6,     6,
       3,     3,     0,     7,     0,     8,     2,     3,     3,     2,
       5,     3,     3,     9,     5,     2,     1,     1,     0,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     0,     3,     0,     4,     1,
       3,     3,     1,     0,     3,     0,     4,     1,     3,     4,
       4,     2,     2,     0,     5,     4,     1,     3,     1,     5,
       3,     7,     1,     0,     1,     6,     4,     4,     1,     3,
       5,     5,     4,     3,     1,     1,     1,     1,     5,     1,
       0,     1,     1,     1,     1,     1,     0,     1,     3,     6,
       1,     3,     3,     2,     2,     6,     6,    10,     8,    10,
      10,    12,    12,    10,     7,     1,     3,     1,     2,     5,
       3,     7,    11,     7,    11,     7,    11,     7,    11,     1,
       1,     1,     5,     3,     5,     6,     6,     3,     1,     4,
       3,     6,     3,     7,     3,     7,     1,     3,     1,     1,
       0,     2,     2,     4,     4,     3,     1,     0,     1,     1,
       2,     2,     3,     6,     1,     5,     3,     5,     3,     3,
       0,     7,     2,     4,     0,     7,     2,     4,     3,     3,
       2,     3,     5,     6,     6,     6,     6,     6,     6,     5,
       7,     5,     7,    13,    11,    13,     5,     5,     5,     2,
       3,     4,     6,     4,     4,     5,     5,     5,     9,     5,
       9,     5,     5,     2,     5,     2,     2,     3,     2,     1,
       1,     1,     4,     5,     4,     5,     4,     5,     2,     1,
       1,     0,     0,     4,     1,     1,     4,     1,     2,     1,
       2,     6,     5,     1,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     4,     1,     2,
       1,     3,     1,     0,     2,     0,     2,     4,    10,    13
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      11,     0,     0,     0,     0,    11,     0,     4,    63,    64,
      14,    10,     0,    13,     0,     0,     1,     5,   221,   220,
       0,     0,     9,     0,     0,     0,   217,   188,   139,   137,
       6,     7,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,   135,    15,    91,   138,    12,
     530,   533,     0,     0,   226,   133,     8,    99,   100,   101,
      97,   103,    98,    95,    92,    93,   129,     0,    94,   102,
      96,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,   532,     0,     0,     0,     0,     0,   225,   132,     0,
       0,   144,   133,   134,   145,   122,   123,   120,   124,   121,
     125,   117,   118,   119,   126,   127,   112,   113,   114,     0,
     111,   104,   110,   115,   116,   108,   109,   105,   106,   107,
       0,     0,   189,   535,   531,     0,     0,     0,    11,     0,
     143,     0,     0,   131,     0,   142,     0,   140,     0,     0,
       0,     0,     0,   528,   494,     0,     0,     0,   324,     0,
       0,     0,     0,     0,   194,   322,   192,   318,   218,     0,
       0,   128,     0,   534,     0,   190,     0,     0,     0,     0,
       0,   493,     0,   529,    65,     0,   228,     0,   227,     0,
     328,     0,     0,   203,   203,   203,   323,   223,    11,   224,
       0,   130,   146,   141,     0,     0,     0,   524,   525,   526,
       0,     0,   482,     0,     0,     0,   229,     0,     0,     0,
     320,     0,   285,   289,   283,   288,   284,   287,   291,   286,
     282,   290,   205,   202,   205,   292,   205,   205,   193,   196,
     195,     0,   313,     0,     0,     0,     0,   337,   335,   336,
       0,     0,     0,     0,    45,     0,     0,   276,     0,     0,
     205,   536,     0,     0,   191,   527,     0,     0,   538,   293,
     230,   327,     0,   326,   329,     0,     0,   204,   340,   340,
     340,   340,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,    57,     0,    47,     0,     0,     0,     0,
       0,   342,   343,   344,   345,   341,     0,   205,   205,   205,
       0,   133,   162,     0,   133,     0,     0,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   367,     0,
       0,     0,     0,     0,     0,     0,    44,   340,     0,     0,
     163,   169,   170,   171,   179,     0,     0,     0,   164,   175,
     166,   172,   173,   174,   165,     0,   177,   180,     0,     0,
       0,   176,   178,   182,   185,   186,     0,   181,   183,   184,
     168,   167,   187,   241,     0,   205,   275,   219,   340,   205,
       0,     0,     0,   500,   519,   502,   503,   497,   498,   499,
     501,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,     0,   487,   484,   485,
     489,     0,   495,     0,     0,   319,   339,     0,     0,     0,
       0,   312,   311,   133,     0,   133,     0,     0,   271,   133,
     133,   133,   133,   133,     0,     0,    76,    80,    75,    79,
      74,    78,    77,    81,     0,     0,     0,    56,     0,     0,
       0,   242,     0,    48,   243,   272,     0,   340,   340,   340,
     133,   159,     0,     0,     0,   249,     0,   248,   268,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,     0,   400,   267,   368,   354,     0,
     353,     0,   262,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,     0,     0,    11,     0,     0,     0,     0,     0,     0,
       0,   133,     0,    11,   210,     0,   260,   261,     0,   347,
     350,     0,     0,     0,     0,   307,     0,   302,   303,     0,
       0,   299,     0,   294,   295,     0,     0,     0,     0,     0,
       0,     0,     0,   133,     0,     0,     0,   340,     0,   340,
     537,     0,   492,   483,   488,     0,   490,     0,     0,   496,
     325,     0,   198,   197,   199,   200,     0,     0,   316,     0,
     253,   252,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    60,    61,     0,    52,   133,   206,     0,
     216,     0,     0,    49,    46,   264,     0,     0,     0,     0,
       0,   334,     0,   133,     0,   158,   161,     0,     0,     0,
       0,     0,   352,     0,     0,   406,     0,     0,     0,     0,
       0,     0,     0,   387,     0,   400,   400,   398,   399,     0,
       0,     0,   370,     0,   383,   481,    43,    42,    41,     0,
     222,     0,     0,   466,   463,   133,     0,     0,   422,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,   430,
     426,     0,    11,     0,     0,     0,     0,     0,     0,    50,
      11,     0,    82,     0,   470,   469,   449,     0,   465,   133,
       0,     0,     0,    25,     0,     0,     0,    18,    30,     0,
     305,    23,     0,     0,    29,   297,    22,     0,     0,    27,
      20,    28,    21,    24,     0,     0,    26,    19,     0,     0,
       0,   244,     0,     0,     0,     0,     0,   539,     0,     0,
     523,   497,   498,   499,     0,     0,   321,     0,     0,   309,
       0,   310,   270,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,   208,     0,
       0,   254,     0,    11,     0,     0,     0,     0,   156,     0,
     160,     0,     0,     0,     0,   392,     0,   394,   397,   402,
      91,   401,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   390,     0,     0,   365,     0,     0,
       0,   340,   340,   340,   471,   479,   480,     0,   234,   274,
       0,     0,   429,     0,   420,   468,   419,     0,     0,     0,
       0,   416,   428,     0,   210,     0,     0,   424,   431,     0,
       0,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    90,   213,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
     351,   301,     0,   308,   304,     0,   300,   296,     0,    17,
     245,     0,   246,     0,   281,     0,   190,     0,     0,   240,
     235,     0,   520,   521,   522,     0,   486,   201,     0,   317,
     255,   256,   257,   258,   259,    71,    70,    68,    66,    69,
      67,     0,    53,     0,   209,     0,   214,   215,   338,   340,
     340,   340,     0,   154,     0,   239,   238,   237,     0,   333,
     157,   251,   250,     0,     0,     0,   129,     0,     0,   405,
       0,   407,     0,     0,     0,     0,     0,   389,     0,     0,
       0,     0,     0,     0,     0,     0,   369,   384,   382,   340,
       0,   340,     0,   340,     0,   478,     0,     0,     0,     0,
     423,    34,   467,     0,     0,     0,     0,     0,     0,   427,
      34,     0,     0,     0,     0,     0,     0,    51,   451,    84,
      89,    88,    11,     0,     0,    83,     0,     0,   211,     0,
       0,     0,     0,   454,     0,     0,     0,   453,     0,   306,
     298,    16,   247,     0,     0,   233,   231,   232,   236,     0,
     315,     0,     0,     0,   207,   340,     0,   340,     0,   340,
       0,   153,   155,     0,     0,   332,     0,   356,   355,     0,
       0,   404,   403,     0,   414,   409,     0,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   385,   386,   366,     0,     0,     0,     0,
       0,     0,   263,     0,   462,   415,    31,    33,    11,     0,
       0,    40,     0,     0,     0,     0,     0,     0,   446,     0,
     417,    11,   441,   439,   432,   461,   448,   447,     0,    86,
      87,   464,     0,     0,   456,   459,     0,   455,   457,     0,
     279,   280,   491,   314,     0,    54,     0,     0,     0,     0,
       0,     0,     0,   265,   331,   330,     0,     0,   407,   410,
       0,   411,   407,     0,   407,   407,   364,     0,   375,     0,
     377,     0,   371,     0,   373,     0,   391,     0,   476,     0,
     472,     0,   474,     0,    32,    11,   436,     0,     0,    37,
       0,   433,    39,     0,   437,   434,   438,   435,     0,    11,
       0,     0,   452,   212,     0,     0,   349,     0,    59,     0,
     151,     0,   147,     0,   149,   393,   395,     0,   358,   412,
       0,     0,     0,     0,     0,   379,   380,   381,     0,     0,
       0,     0,   477,   473,   475,   273,   421,    38,    36,    35,
       0,     0,   425,   442,   440,     0,     0,    55,   152,   148,
     150,     0,     0,     0,   407,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   357,     0,   359,     0,
     360,   363,     0,     0,     0,     0,     0,     0,     0,     0,
     460,   458,   413,     0,     0,   376,   378,   372,   374,     0,
       0,     0,   362,   361,     0,   444,     0,     0,     0,   445,
     443
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,    45,   670,    12,    13,   814,  1077,  1078,
    1081,  1082,   346,   347,   305,   306,   532,   457,   458,   807,
     606,     7,   302,   303,   454,   455,   533,   852,   853,   108,
      67,  1083,    47,   923,   924,   322,   323,   384,    48,   186,
     172,   173,   174,   242,   288,   609,   535,   610,   611,     8,
      54,   210,    20,   506,   149,   107,   167,   267,   655,   773,
     268,   269,   884,   885,   243,   247,   551,   552,   718,   875,
     553,   545,   546,   713,   872,   547,   294,   588,   589,   175,
     176,   177,   201,   621,   622,   270,   426,   507,   316,   540,
     541,   338,   808,   339,   340,   341,  1198,   342,   343,   495,
     482,   483,   650,   636,  1046,  1047,   695,   971,   980,   671,
     696,   815,   816,   817,   223,   277,   416,   417,   418,   419,
     420,   191,   192,   421,   422,   745,   163,   164,    52,   102,
     159,   215,     9
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -757
static const yytype_int16 yypact[] =
{
     249,   327,    47,   -61,   112,   279,   143,  -757,  -757,  -757,
     -39,  -757,    53,  -757,   -12,   156,  -757,  -757,  -757,  -757,
     196,  2203,  -757,   243,    77,   101,  -757,  -757,   145,  -757,
    -757,   317,  -757,   225,   225,   225,   225,   135,   225,   225,
     225,  2203,   225,   166,  2203,  -757,  4696,  -757,   444,  -757,
    -757,   278,   174,   398,   283,  2203,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  4254,   316,  -757,  -757,
    -757,  2401,   -48,  2203,  2203,  2203,  2203,  2203,  2203,  2203,
    2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,
    2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,  2203,
     461,  -757,   517,   527,   439,   408,   445,   468,  4696,   254,
    2203,  -757,  2203,  2203,  -757,   489,   489,   604,   604,   604,
     604,   542,   542,   542,  4716,  4736,   922,  2244,  2244,  4274,
    1046,  2244,   922,   489,   489,   658,   658,  -757,  -757,  -757,
     356,  2427,  -757,   473,  -757,   406,   653,   579,   182,   510,
    -757,  4294,   237,  4696,  2203,  -757,  2203,  -757,  2203,   541,
     716,   719,   144,  -757,   339,   560,   733,   390,   626,   754,
     777,   376,   418,   492,  -757,  -757,  -757,  -757,  -757,  2203,
     639,  4696,  2653,  4696,   744,  -757,   464,   673,   677,   849,
     869,  -757,   764,  -757,  -757,   749,  -757,   335,  -757,  1600,
    -757,   290,   714,  1389,  1389,   923,    19,  -757,   100,  -757,
    4876,  4696,  -757,  -757,   881,   493,   885,  -757,  -757,  -757,
     730,   795,  -757,   911,  2203,   733,  -757,   752,  2452,   754,
    -757,    40,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,   799,  -757,   799,  -757,   799,   799,  -757,  -757,
    -757,   806,   813,   123,    18,   716,    87,   879,   879,   879,
     115,   118,   360,   990,   832,   865,  5103,  -757,  5000,   953,
    1613,  -757,   950,   882,  -757,  -757,   610,  5175,  -757,  4696,
    -757,  -757,  2203,  -757,  -757,   754,   884,  -757,   883,   883,
     883,   883,  -757,   108,   128,   662,   892,   901,   906,   921,
     924,   466,  -757,   920,  2203,  -757,   524,   361,  1086,   529,
    1041,  -757,  -757,  -757,  -757,  -757,  1089,   799,   799,   799,
     216,  1946,  -757,   580,  2067,   670,  1043,  -757,   928,   636,
     932,   935,   936,   937,   938,   945,   946,   354,  -757,   685,
     951,   954,   952,   955,   956,   462,  -757,   883,   585,  1272,
    -757,  -757,  -757,  -757,  -757,  1272,   833,    83,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,   116,  -757,  -757,   836,   846,
     131,  -757,  -757,  -757,  -757,  -757,   861,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,   127,   799,  -757,  -757,   883,   799,
    1112,   764,   960,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  1068,  -757,  5175,  5175,
    -757,  1748,  -757,  2676,   340,  -757,  -757,  1129,  1130,  1134,
    1136,  -757,  -757,  1629,   975,  2203,   700,  2203,  -757,  2203,
    2203,  2203,  2203,  2203,   979,   980,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,   981,   982,   153,  -757,    76,  4445,
    1086,  -757,  2203,  -757,  -757,  -757,   983,   883,   883,   883,
    1663,   984,   985,   505,   150,  -757,   992,  -757,  -757,  2203,
     987,   997,  -757,   703,   236,   236,   236,   236,   236,   236,
     236,   995,   974,  1146,  1146,   534,  -757,  -757,  -757,   180,
    -757,   228,  -757,   996,   998,   999,   716,   920,  1000,  1001,
    1003,   602,  1086,    76,   273,  1005,  1008,  1009,    76,  1086,
    1010,    76,  1272,   275,  1015,    76,  1016,  1020,  1024,   296,
     107,  2203,  1096,  1168,   478,   136,  -757,  -757,  1004,   195,
    -757,   706,  1044,  1047,   883,  -757,  1045,  1049,  -757,  1050,
     883,  -757,  1051,  1056,  -757,  1055,   720,  1062,   724,  1063,
      46,  1065,   728,  1957,   150,   142,   731,   400,   897,   883,
    -757,  1156,  -757,  -757,  -757,  4610,  -757,  5081,  5128,  -757,
    -757,  1064,  -757,  -757,  -757,  1069,  1229,   519,  -757,   547,
    -757,  -757,  3708,   554,   561,   562,   567,   572,   381,   414,
     363,   423,  -757,  -757,  -757,  2203,  -757,  2203,   565,  1070,
    -757,   739,  2203,  -757,  4696,  -757,   716,   716,   716,  1232,
     587,  -757,   588,  2203,   150,  -757,  -757,   150,  2923,  2203,
    2239,   636,  -757,  2203,  2203,  1088,  1074,  1076,  1082,  1083,
    1084,  1090,  1091,  -757,   225,   534,   534,  -757,  -757,  1246,
     434,  2203,  -757,  1675,  -757,  1367,  -757,  -757,  -757,   743,
    -757,  1092,  1251,  -757,  -757,  2203,    -8,  1098,  -757,  1257,
    1272,   175,  2203,  2203,  2203,  1101,   413,    35,  1099,  -757,
    -757,  1266,    81,  1847,  1106,  2203,  2203,  1858,  2203,  -757,
    1168,  1371,  1107,   371,  -757,  -757,  -757,  1096,  -757,  2203,
    2203,  2130,  2137,  -757,  2203,  2203,  1269,  -757,  -757,  2203,
    -757,  -757,  1271,  1271,  -757,  -757,  -757,   733,   733,  -757,
    -757,  -757,  -757,  -757,  1269,   746,  -757,  -757,   747,   150,
     755,  -757,  1279,  1281,  1120,   756,   716,  -757,   386,  5175,
    -757,  1122,  1123,  1124,  1140,  1126,  -757,  2203,  1131,  -757,
    1116,  -757,  -757,  1135,  1138,  1139,  1142,  1143,  1128,  1132,
    1148,  1149,  1152,  1160,   589,  4465,   377,  2203,  -757,  2203,
      76,  -757,  2699,   366,   761,   765,   766,  1157,  -757,  1125,
    -757,   603,   779,   782,  1162,  4485,  2203,  4696,  -757,  4696,
    1185,  4696,  1186,  2203,   236,  2203,   236,   236,   236,  2203,
     236,  1164,   570,   616,  1163,  1146,  1146,  -757,   606,  1171,
     608,   202,   256,   276,  -757,  -757,  1367,  1096,  -757,  -757,
    1133,   614,  -757,  2203,  1291,  -757,  -757,  1272,  2943,  2963,
    2983,  -757,  -757,  1173,   607,  1177,  2203,  1273,  -757,  1176,
    3003,  -757,  3023,  3178,  1180,  3198,  1181,  1096,  1182,  2203,
    2203,  1183,   251,  -757,  4696,  -757,  -757,   627,  2477,  1188,
     169,  2203,  2203,  3728,  1189,  2203,  2203,  3748,  4696,  4505,
    -757,  4696,  1271,  -757,  1049,   733,  -757,  1056,   783,  -757,
    -757,   786,  -757,  1187,  -757,   792,  1187,   804,   805,  -757,
    -757,   812,  -757,  -757,  -757,   386,  -757,  4696,  2176,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  2203,  -757,  2203,  -757,  2502,  4696,  -757,  -757,   289,
     291,   297,   512,  -757,  1184,  -757,  -757,  -757,  1555,  -757,
    -757,  -757,  -757,  1191,  1192,  2203,  4234,  2203,  2203,  4696,
    1196,    84,  1198,  1199,  1200,   630,  1201,  -757,   173,   176,
     197,   204,   225,   632,   637,  2203,  -757,  -757,  -757,   883,
     716,   883,   716,   883,   716,  -757,  1310,  1362,  1205,  3768,
    -757,   512,  -757,  1383,  1415,  1489,  1272,  2203,  3788,  -757,
     512,  1096,  1096,  1272,  1096,  1272,  1272,  -757,  -757,  -757,
    4696,  4696,  1168,  2164,  2164,  -757,  1206,  2203,  -757,  2203,
    1589,  3941,  3961,  -757,  2203,  3981,  4001,  -757,  2203,  1049,
    1056,  -757,  -757,  2203,  1279,  -757,  -757,  -757,  -757,  1208,
    -757,  3218,   638,  4525,  -757,   883,   716,   883,   716,   883,
     716,  -757,  -757,  1327,  1214,  -757,  3238,  -757,  -757,  4545,
    2203,  4696,  4696,  2203,  -757,  1086,  1215,  1220,  2203,  2203,
    2203,  2203,  1224,  2203,  1218,   225,  1226,   225,  1227,   225,
    1228,   225,  1223,  -757,  -757,  -757,   716,   817,   716,   825,
     716,   826,  -757,  1235,  -757,  -757,  -757,   512,  1168,    58,
     218,  -757,   704,   -57,    62,   957,   103,  1317,  -757,  4021,
    -757,  1168,  1356,  1357,  -757,  -757,  -757,  -757,  1096,  -757,
    -757,  -757,  2722,  3258,  -757,  -757,  3278,  -757,  -757,  2745,
    4696,  -757,  -757,  -757,  2203,  -757,  2203,   716,   831,   716,
     834,   716,   838,  -757,  -757,  -757,  2203,  3433,    84,  1107,
    1240,  1086,    84,  1243,    84,  2772,  -757,  1244,  -757,   343,
    -757,   343,  -757,   343,  -757,   343,  -757,   850,  -757,   851,
    -757,   858,  -757,  1248,  -757,   329,  -757,  1096,  1096,  -757,
    1258,  -757,  -757,  1096,  -757,  -757,  -757,  -757,  1887,   346,
    1096,  1096,  -757,  -757,  1245,  1245,  -757,  1242,  4696,   872,
    -757,   873,  -757,   903,  -757,  4696,  -757,  1250,  -757,   631,
    1254,  2203,  1260,  1261,  2203,  -757,  -757,  -757,  2203,  2203,
    2203,  2203,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    1264,  4041,  -757,  -757,  -757,  2203,  2203,  -757,  -757,  -757,
    -757,  1265,   225,  1267,    84,  1274,  1280,    84,  3453,  3473,
    3493,  3513,    88,    36,  4194,  4214,  -757,  1252,  -757,  1270,
    -757,  -757,  1282,  1285,  1286,  1287,  1288,  1284,  1289,  1295,
    -757,  -757,  -757,  1298,  1299,  -757,  -757,  -757,  -757,  2203,
    1272,  2203,  -757,  -757,  3533,  -757,  3688,  1272,  1272,  -757,
    -757
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -757,  -757,  -757,  1155,     3,  -757,  1392,  -265,  -757,   456,
    -485,  -116,  -757,  -757,  -206,  -757,   324,  -373,   958,  -593,
      13,  1461,  -371,  -757,  -314,  -308,  -687,  -757,    90,   -21,
    -757,    -4,    -9,   548,  -757,  1002,  -180,  -757,    26,  -220,
    -757,  -757,  1263,   835,   635,    74,  -671,   705,  -757,  -757,
    -757,  -757,  -757,  -370,  -757,  -757,  -757,  1209,  -757,  -757,
    -757,  -757,   465,   745,   543,   573,   -84,  -757,  -757,  -757,
    -633,   770,  -757,  -757,  -757,  -681,  -757,   734,  -757,  -757,
    1283,   -47,  1210,   707,  -757,  -757,  -237,  -243,  -757,   796,
    -374,  1165,   848,  -757,   701,  -757,  -472,   721,  -757,  -452,
     875,  -757,   424,  -389,  -756,  -757,  -345,  -757,  -757,  -518,
    -653,   687,  -757,  -757,  1121,  -757,  -757,  1095,  -757,  -757,
    1102,  -757,  -757,   667,  -412,   619,  1361,  -757,  -757,  -757,
    -757,  -757,  -757
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -523
static const yytype_int16 yytable[] =
{
      46,   383,   562,     6,   536,   682,   835,   472,     6,   579,
     537,   564,   764,   565,   862,   866,   476,   308,   568,   315,
      66,    27,   168,    71,    57,    58,    59,    60,    62,    63,
      64,    65,   874,    68,    70,   309,   833,  1248,    27,    27,
      72,   645,   646,   168,   856,   427,   428,   429,   430,   539,
      14,   109,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    22,   141,    27,
      50,   325,   196,   471,   542,   877,   543,  1044,   310,   151,
    -346,    27,   153,  1163,   140,   846,   637,   638,   639,   640,
     641,   642,   463,   249,     1,    15,   113,  1156,   152,   431,
      27,  1164,    16,   226,   436,   113,   432,   549,   320,   195,
     548,   320,   200,     1,   295,    21,   -73,   114,   554,   434,
     320,   320,   559,   181,  -205,   182,    61,   183,    27,    28,
      29,   280,    30,    31,    32,   320,   701,   188,   556,   558,
     838,   171,  1166,   320,    24,   311,   602,   822,   211,    25,
      51,   603,   604,   579,   966,   579,   744,    69,   100,    27,
      28,    29,    27,    30,    31,    32,  1054,   679,   228,  1056,
     312,   313,   284,   602,   286,   168,   725,   544,   603,   604,
     304,  1009,   729,   169,   988,   170,   732,   733,     1,    26,
    1058,   314,   941,   279,   566,     1,   945,  1060,  1157,   531,
     531,   171,  1157,   266,   285,    18,    23,    19,   304,   510,
     550,    27,   511,   826,   616,   617,   618,   189,    27,    28,
      29,   602,    30,    31,    32,   287,   603,   604,   200,    27,
      28,    29,  1010,    30,    31,    32,    10,  1045,   512,    -3,
     607,   513,   514,  1157,   613,   304,   515,   516,   517,   304,
     518,   423,   531,   519,    53,   690,   296,   297,   298,   299,
     300,   266,     1,   691,   433,   520,   521,   522,   523,    -2,
     307,   321,   524,   459,   324,   759,   659,   763,   304,   301,
     758,   -73,   761,   563,   435,   456,     1,   -73,     1,   602,
     702,    41,     1,  -205,   603,   604,   959,   710,   435,    44,
     456,    55,   633,   715,   525,   526,   435,   473,  1022,   605,
     473,   668,   634,   724,    56,   825,   736,   744,  1092,  1093,
      10,  1095,    41,   993,   307,  1000,     2,   103,   195,  1055,
      44,   104,  1057,   527,   680,  -323,   651,   528,   735,  -323,
     878,    11,     1,   953,   954,   491,   169,   492,   170,   704,
     961,   326,  1065,  1059,  1195,  1196,     2,   705,  1158,     1,
    1061,   768,  1187,   101,   304,   534,  1190,  1206,  1192,  1193,
     963,   534,   470,   694,   728,   730,   529,   530,   304,     1,
       3,    41,   531,  1025,   653,  1027,   774,   775,   776,    44,
     113,  1029,    41,   105,   190,   940,   160,   942,   943,   944,
      44,   946,   180,   327,   994,  1212,   592,   113,   995,   225,
       3,   150,   161,   669,   328,   681,   760,  1159,   304,   587,
     493,   473,   919,   920,   162,   593,   594,   595,   596,   597,
     494,   614,   203,   204,   782,  1172,   805,   783,   304,   921,
    1128,   106,   447,   229,   806,  1132,  1133,  1134,   628,   205,
    1137,   304,   688,   304,   142,   230,   620,   329,  1239,   304,
     447,  1242,   449,   160,   451,   635,   635,   635,   635,   635,
     635,   635,   972,   111,   608,   762,   307,  1215,  1216,   161,
     449,   453,   451,  1197,   330,   331,   332,   333,   334,   335,
     336,   162,   446,   229,  1207,  1208,    79,    80,    81,   453,
    1209,   446,   652,   887,   654,   581,   891,  1213,  1214,   113,
     143,  1177,   448,   155,   450,   462,   337,   693,   444,   445,
     144,   448,   503,   450,   113,   504,   697,   100,   666,   608,
     113,   452,   689,   740,   608,   676,   855,   608,   534,   881,
     452,   608,   914,   197,   446,   447,   692,   198,   534,   473,
    1155,  1247,  1249,   892,   893,   894,   301,   505,   960,   962,
     964,   146,   304,  1169,   448,   449,   450,   451,   832,  1033,
     356,   206,   948,   357,   765,   207,   508,   667,   509,   100,
     949,   772,   675,   452,   453,   678,   365,  1162,  1224,   684,
    1162,  1227,  1162,   766,   145,   368,   369,   370,   785,   787,
      98,   147,   789,   791,    73,    74,    99,    30,    31,   781,
     100,    79,    80,    81,   790,   792,   376,   216,   950,   217,
     765,  1088,   765,   876,   148,   801,   951,   158,  1094,   480,
    1096,  1097,   481,   698,   699,    93,    94,    95,    96,    97,
     700,   828,   829,   830,   100,   208,   272,  1031,   165,   209,
     273,   821,   840,   166,   842,   843,   845,   765,   113,  1199,
     854,  1200,   625,  1201,   827,   178,  1026,  1028,  1030,   858,
     863,   867,   113,   868,   869,   827,   749,   460,   871,   461,
     647,   648,   216,   847,   464,   857,   534,   649,    93,    94,
      95,    96,    97,   834,   184,  1160,  1076,    27,    28,    29,
     750,    30,    31,    32,   751,  1076,  1066,   113,  1068,   185,
    1070,   753,   187,   534,   113,   113,   897,   194,   754,   755,
     113,    33,    34,    35,   756,   113,   195,   767,   496,   757,
    1067,   100,  1069,   474,  1071,   475,   915,  1080,   916,   328,
     113,   779,   911,  1161,   778,   780,   912,   168,    91,    92,
      93,    94,    95,    96,    97,   936,   113,   664,   665,   955,
     930,   955,   939,   956,   765,   958,   922,   113,   765,   700,
     202,   968,  1117,   100,  1119,   635,  1121,   635,   635,   635,
     113,   635,   329,  1051,   996,   649,   608,  1052,   199,  1063,
     649,  1114,   969,  1222,  1064,  1115,  1118,   100,  1120,   385,
    1122,   214,  1154,   389,   212,   978,    95,    96,    97,   330,
     331,   332,   333,   334,   335,   336,   437,   438,   990,   991,
     317,   318,   319,   474,   538,   477,   539,   555,   218,   185,
    1001,  1002,   219,   534,  1005,  1006,  1147,   557,  1149,   185,
    1151,   337,   220,   534,  -133,    36,    37,    38,  1085,  1087,
      39,    40,   561,   474,   539,   591,   631,  -133,   632,   706,
      41,   707,   221,   534,    42,    43,   222,  1021,    44,   289,
     231,   290,   291,   216,   271,   720,   692,   216,   274,   722,
     765,   706,  1023,   727,   474,   275,   731,  1179,   734,  1181,
     185,  1183,   770,   287,   771,   388,   216,  1036,   818,   706,
     474,   879,   880,   224,  1039,  1265,  1041,  1042,   474,   216,
     882,   890,  1269,  1270,   216,   281,   925,   922,   216,   216,
     926,   927,    73,    74,   765,    75,    76,    77,    78,    79,
      80,    81,   474,  1062,   931,   474,   706,   932,  1011,   474,
     534,  1012,   467,   468,   469,  1014,  1089,  1015,  1160,   276,
      27,    28,    29,   278,    30,    31,    32,   216,  1014,  1016,
    1017,   292,   854,   854,   245,   216,  1102,  1018,  1103,   854,
     216,   293,  1148,  1106,    33,    34,    35,  1109,   216,   216,
    1150,  1152,  1110,   344,   216,  1098,  1180,   216,   345,  1182,
    1080,   216,   534,  1184,   387,   560,  1165,   534,   534,   534,
     534,   534,   534,   216,   216,  1202,  1203,   390,   245,  1127,
     567,   216,   765,  1204,   569,   861,   865,   765,   765,   765,
    1135,   348,   765,   232,   233,   216,   216,  1218,  1219,   244,
     246,   234,   235,   236,   237,   238,  1139,   391,  1141,   239,
    1143,   425,  1145,   240,   241,   304,    73,    74,   439,    75,
      76,    77,    78,    79,    80,    81,   216,   440,  1220,   802,
     803,  1129,   441,    84,    85,    86,    91,    92,    93,    94,
      95,    96,    97,  1099,  1100,   575,   577,   442,   456,    27,
     443,   465,   466,   765,   479,  1178,   478,   510,   484,    27,
     511,   485,   486,   487,   488,  1185,   534,  -133,    36,    37,
      38,   489,   490,    39,    40,   570,   498,   500,   499,   501,
    -133,   502,   573,    41,   534,   572,   512,    42,    43,   513,
     514,    44,   582,   583,   515,   516,   517,   584,   518,   585,
     590,   519,   598,   599,   600,   601,   644,  1211,   615,   492,
     623,   629,   624,   520,   521,   522,   523,  1189,   827,   627,
     524,   630,   643,   656,   662,   657,   658,   661,   663,   703,
     765,   672,   827,   765,   673,   674,   677,  1228,  1229,  1230,
    1231,   683,   685,   534,   534,   510,   686,    27,   511,   534,
     687,     1,   525,   526,  1234,  1235,   534,   534,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     1,   737,   708,
     711,   709,   712,  1237,   512,   714,   716,   513,   514,   717,
     719,   527,   515,   516,   517,   528,   518,   721,   723,   519,
     726,   746,   748,   747,   769,   777,   793,   794,  1264,   795,
    1266,   520,   521,   522,   523,   796,   797,   798,   524,   804,
     919,   920,   -11,   799,   800,   -11,   820,   819,   834,   834,
     824,   694,   823,   836,   529,   530,   831,   921,   -11,   837,
     531,   841,   539,   510,   543,    27,   511,   -11,   -11,   -11,
     525,   526,   883,   100,   886,   889,   534,  -520,  -521,  -522,
     895,   896,   586,   534,   534,   905,   967,   898,   -11,   906,
     900,   619,   512,   901,   902,   513,   514,   903,   904,   527,
     515,   516,   517,   528,   518,   907,   908,   519,  1160,   909,
      27,    28,    29,   928,    30,    31,    32,   910,   337,   520,
     521,   522,   523,   937,   938,   952,   524,   947,   957,   970,
     976,   977,   979,   981,    33,    34,    35,   985,   987,   989,
     992,  1013,   529,   530,   999,  1004,  1037,  1038,   531,  1043,
    1080,  1048,  1049,  1050,  1053,  1072,  1167,  1073,   525,   526,
    1074,  1101,   848,  1112,    27,    28,    29,  1123,    30,    31,
      32,  1124,  1130,  1131,  1079,  1138,    27,    28,    29,  1136,
      30,    31,    32,  1140,  1142,  1144,  1146,   527,    33,    34,
      35,   528,  1153,  1170,  1171,  1188,  1191,  1194,  1157,  1217,
      33,    34,    35,  1205,   860,    49,  1084,  1221,    27,    28,
      29,  1223,    30,    31,    32,  1252,  1080,  1225,  1226,  1232,
    1236,   392,  1238,   811,   812,   356,  1091,  1253,   357,  1240,
     529,   530,    33,    34,    35,  1241,   531,   849,  1259,  1254,
     813,   365,  1255,  1256,  1257,  1258,  1260,   850,  1080,  1261,
     368,   369,   370,  1262,  1263,   660,    17,  -133,    36,    37,
      38,   250,  1032,    39,    40,   917,   626,   386,   888,  1111,
    -133,   376,   873,    41,   899,   933,   929,    42,    43,   248,
    1086,    44,    27,    28,    29,   424,    30,    31,    32,   232,
     233,   810,   870,   965,   497,   934,   788,   234,   235,   236,
     237,   238,   571,   574,  1019,   239,    33,    34,    35,   240,
     241,   576,    36,    37,    38,   193,     0,    39,    40,   851,
       0,     0,  1080,  -133,    36,    37,    38,    41,     0,    39,
      40,    42,    43,     0,     0,    44,  -133,     0,     0,    41,
       0,     0,     0,    42,    43,     0,  1034,    44,    27,    28,
      29,     0,    30,    31,    32,  -133,    36,    37,    38,     0,
       0,    39,    40,     0,     0,     0,     0,     0,  -133,     0,
       0,    41,    33,    34,    35,    42,    43,     0,     0,    44,
     848,     0,    27,    28,    29,     0,    30,    31,    32,     0,
       0,   227,     0,    27,    28,    29,     0,    30,    31,    32,
       0,     0,     0,     0,     0,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       0,     0,    27,    28,    29,     0,    30,    31,    32,  -133,
      36,    37,    38,     0,     0,    39,    40,     0,     0,     0,
       0,     0,  -133,     0,     0,    41,    33,    34,    35,    42,
      43,     0,     0,    44,     0,   849,    27,    28,    29,     0,
      30,    31,    32,     0,     0,   850,   809,     0,    27,    28,
      29,     0,    30,    31,    32,     0,     0,     0,     0,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    34,    35,     0,    36,    37,    38,     0,
       0,    39,    40,     0,     0,     0,     0,   287,     0,     0,
       0,    41,  1035,   232,   233,    42,    43,     0,     0,    44,
       0,   234,   235,   236,   237,   238,     0,     0,     0,   239,
      36,    37,    38,   240,   241,    39,    40,     0,     0,     0,
       0,    36,    37,    38,     0,    41,    39,    40,     0,    42,
      43,     0,     0,    44,     0,     0,    41,     0,     0,     0,
      42,    43,     0,     0,    44,     0,     0,     0,     0,     0,
      36,    37,    38,     0,     0,    39,    40,     0,     0,     0,
       0,     0,     0,     0,     0,    41,     0,     0,     0,    42,
      43,     0,     0,    44,     0,   586,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,    39,
      40,     0,     0,     0,     0,     0,    36,    37,    38,    41,
       0,    39,    40,    42,    43,     0,     0,    44,     0,   619,
       0,    41,     0,     0,     0,    42,    43,     0,   839,    44,
      27,    28,    29,     0,    30,    31,    32,     0,     0,   844,
       0,    27,    28,    29,     0,    30,    31,    32,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,  1210,     0,
      27,    28,    29,     0,    30,    31,    32,   393,   578,     0,
       0,     0,     0,     0,   394,     0,   395,     0,   396,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,     0,     0,     0,     0,     0,    27,
      28,    29,     0,    30,    31,    32,     0,     0,     0,     0,
      27,    28,    29,     0,    30,    31,    32,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,    39,    40,     0,     0,     0,     0,    36,
      37,    38,     0,    41,    39,    40,     0,    42,    43,   444,
     445,    44,     0,     0,    41,     0,     0,     0,    42,    43,
       0,     0,    44,     0,   446,     0,     0,     0,    36,    37,
      38,     0,     0,    39,    40,   446,   447,     0,     0,     0,
       0,     0,     0,    41,   448,     0,   450,    42,    43,     0,
       0,    44,     0,     0,     0,   448,   449,   450,   451,     0,
      27,    28,    29,   452,    30,    31,    32,     0,     0,     0,
       0,     0,     0,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
       0,     0,    39,    40,     0,     0,     0,     0,    36,    37,
      38,     0,    41,    39,    40,     0,    42,    43,     0,     0,
      44,     0,     0,    41,     0,     0,     0,    42,    43,     0,
       0,    44,     0,    27,    28,    29,     0,    30,    31,    32,
      27,    28,    29,     0,    30,    31,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,    33,    34,    35,
       0,     0,     0,     0,    33,    34,    35,    27,    28,    29,
       0,    30,    31,    32,     0,     0,   449,     0,   451,    27,
      28,    29,     0,    30,    31,    32,     0,     0,     0,     0,
       0,    33,    34,    35,     0,   453,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,    27,    28,    29,     0,
      30,    31,    32,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,    39,    40,     0,     0,   859,     0,     0,
      33,    34,    35,    41,   864,     0,     0,    42,    43,     0,
     849,    44,    27,    28,    29,     0,    30,    31,    32,     0,
     850,     0,     0,     0,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    33,    34,    35,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,    39,    40,    36,    37,
      38,     0,     0,    39,    40,     0,    41,     0,   529,   860,
      42,    43,     0,    41,    44,   529,   860,    42,    43,     0,
       0,    44,     0,     0,     0,    36,    37,    38,     0,     0,
      39,    40,     0,     0,     0,     0,     0,    36,    37,    38,
      41,     0,    39,    40,    42,    43,     0,     0,    44,     0,
       0,     0,    41,  1020,     0,     0,    42,    43,     0,     0,
      44,     0,     0,     0,    36,    37,    38,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,     0,    41,
       0,     0,     0,    42,    43,     0,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,     0,    39,    40,    90,    91,    92,
      93,    94,    95,    96,    97,   786,     0,     0,     0,    42,
      43,    73,    74,    44,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,     0,     0,     0,     0,     0,     0,    73,    74,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,     0,
       0,     0,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,    73,    74,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,     0,     0,     0,    73,    74,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,     0,
       0,     0,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,    73,    74,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,    87,   156,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     157,    87,   282,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   283,    87,   997,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     998,    87,   612,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,  1024,     0,     0,    82,    83,
      84,    85,    86,     0,     0,     0,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,     0,     0,     0,    73,
      74,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,     0,
       0,     0,    73,    74,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,     0,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,  1044,     0,     0,     0,     0,
       0,     0,    73,    74,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,   213,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,   580,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,   918,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1173,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1176,     0,
       0,    87,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    73,    74,  1045,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
     784,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
     973,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
     974,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
     975,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
     982,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    73,    74,
     983,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,   984,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,   986,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,  1113,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,  1125,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,  1174,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    73,    74,  1175,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,    82,    83,
      84,    85,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
    1186,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
    1243,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
    1244,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
    1245,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
    1246,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    73,    74,
    1267,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,  1268,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,   752,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1003,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1007,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1075,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,    73,    74,  1090,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,  1104,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,  1105,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,  1107,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,  1108,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     0,     0,  1168,     0,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,    73,    74,  1233,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,  1250,
       0,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,  1251,
       0,     0,     0,    87,   110,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,  1040,     0,     0,
       0,     0,     0,    87,   110,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,    87,   154,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,    87,   179,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,   612,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,   913,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,   935,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1008,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1116,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1126,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,   393,
     738,     0,     0,    84,    85,    86,   394,     0,   395,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   251,     0,   252,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,     0,     1,
       0,     0,     0,     0,     0,     0,     0,   -11,   -11,   253,
       0,   -11,   -11,   -11,     0,     0,     0,   -11,     0,     0,
     254,     0,     0,     0,     0,     0,     0,  -278,     0,     0,
       0,     0,   255,     0,     0,     0,     0,   256,     0,     0,
       0,   -11,   257,   258,   -11,     0,     0,   -11,     0,     0,
       0,   -11,     0,   -11,   -11,   -11,   -11,   -11,   -11,   259,
     -11,   -11,     0,     0,     0,     0,   260,   261,   -11,   -11,
     -11,   -11,     0,     0,   -11,   -11,   -11,   -11,   -11,     0,
       0,     0,   262,     0,     0,     0,   -11,   -11,     0,   263,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   264,
       0,   251,   -11,   252,     0,     0,   -11,   -11,   -11,   -11,
       0,     0,     0,     0,     0,     0,     0,   265,   -11,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,   -11,   -11,   253,     0,   -11,   -11,   -11,     0,     0,
       0,   -11,     0,     0,   254,     0,     0,     0,     0,     0,
       0,  -277,     0,     0,     0,     0,   255,     0,     0,     0,
       0,   256,     0,     0,     0,   -11,   257,   258,   -11,     0,
       0,   -11,     0,     0,     0,   -11,     0,   -11,   -11,   -11,
     -11,   -11,   -11,   259,   -11,   -11,     0,     0,     0,     0,
     260,   261,   -11,   -11,   -11,   -11,     0,     0,   -11,   -11,
     -11,   -11,   -11,     0,     0,     0,   262,     0,     0,     0,
     -11,   -11,     0,   263,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   264,     0,     0,   -11,     0,     0,     0,
     -11,   -11,   -11,   -11,   349,   350,     0,     0,   351,   352,
     353,   265,   -11,     0,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   355,     0,
       0,   356,     0,     0,   357,     0,     0,     0,   358,     0,
     359,   360,   361,   362,   363,   364,     0,   365,   366,     0,
       0,     0,     0,     0,     0,   367,   368,   369,   370,     0,
       0,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,   232,   233,     0,     0,   376,   377,   378,
     379,   234,   235,   236,   237,   238,     0,     0,     0,   239,
     393,   739,     0,   240,   241,   380,   381,   394,     0,   395,
       0,   396,     0,     0,     0,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   393,     0,     0,
       0,     0,     0,     0,   394,   740,   395,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   741,   742,   743,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   393,     0,     0,     0,     0,     0,
       0,   394,     0,   395,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415
};

static const yytype_int16 yycheck[] =
{
      21,   266,   376,     0,   349,   523,   677,   321,     5,   421,
     355,   384,   605,   384,   701,   702,   324,   254,   388,   256,
      41,     3,     3,    44,    33,    34,    35,    36,    37,    38,
      39,    40,   713,    42,    43,   255,     1,     1,     3,     3,
      44,   493,   494,     3,   697,   288,   289,   290,   291,     3,
       3,    55,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    24,    99,     3,
       3,   261,   166,   320,     1,   718,     3,     3,     1,   110,
       3,     3,   113,   150,    98,   688,   485,   486,   487,   488,
     489,   490,   308,     3,    23,   166,   163,    49,   112,     1,
       3,    49,     0,   197,   294,   163,     8,     1,     3,     3,
     357,     3,   169,    23,     1,   164,     3,   175,   365,     1,
       3,     3,     1,   154,     3,   156,     1,   158,     3,     4,
       5,   225,     7,     8,     9,     3,    10,     3,   368,   369,
      69,   148,    49,     3,   166,    68,     3,   165,   179,     3,
      83,     8,     9,   575,   817,   577,   578,     1,   176,     3,
       4,     5,     3,     7,     8,     9,     3,   522,   199,     3,
      93,    94,   229,     3,   231,     3,   560,   104,     8,     9,
     172,   872,   565,   174,   847,   176,   567,   567,    23,     3,
       3,   114,   795,   224,   384,    23,   799,     3,   150,   174,
     174,   208,   150,   210,   174,    72,   163,    74,   172,     1,
     104,     3,     4,    48,   467,   468,   469,    83,     3,     4,
       5,     3,     7,     8,     9,   104,     8,     9,   285,     3,
       4,     5,   875,     7,     8,     9,     3,   163,    30,     0,
     174,    33,    34,   150,   460,   172,    38,    39,    40,   172,
      42,   282,   174,    45,   163,   158,   143,   144,   145,   146,
     147,   268,    23,   166,   166,    57,    58,    59,    60,     0,
     254,   166,    64,   304,   166,   599,   506,   601,   172,   166,
     598,   168,   600,   166,   166,   168,    23,   174,    23,     3,
     164,   166,    23,   172,     8,     9,   104,   544,   166,   174,
     168,   166,    76,   550,    96,    97,   166,   321,   911,   166,
     324,    48,    86,   560,     7,   670,   569,   739,   981,   982,
       3,   984,   166,    82,   308,   166,    87,   163,     3,   166,
     174,   167,   166,   125,    69,   163,   166,   129,   568,   167,
     724,    24,    23,   805,   806,     1,   174,     3,   176,   164,
     104,     1,   955,   166,    21,    22,    87,   172,   150,    23,
     166,   608,  1128,    95,   172,   349,  1132,    48,  1134,  1135,
     104,   355,   166,   165,   564,   565,   168,   169,   172,    23,
     141,   166,   174,   104,   166,   104,   616,   617,   618,   174,
     163,   104,   166,     5,    65,   794,    67,   796,   797,   798,
     174,   800,   175,    53,   163,    69,   437,   163,   167,    84,
     141,   167,    83,   150,    64,   150,    63,  1080,   172,   433,
      76,   435,    66,    67,    95,   439,   440,   441,   442,   443,
      86,   462,    66,    67,   624,  1098,    12,   627,   172,    83,
    1043,   168,    89,   163,    20,  1048,  1049,  1050,   479,    83,
    1053,   172,   166,   172,     3,   175,   470,   107,  1224,   172,
      89,  1227,   109,    67,   111,   484,   485,   486,   487,   488,
     489,   490,   827,   167,   458,    62,   460,  1174,  1175,    83,
     109,   128,   111,   150,   134,   135,   136,   137,   138,   139,
     140,    95,    88,   163,  1157,  1158,    17,    18,    19,   128,
    1163,    88,   499,   733,   501,   175,   736,  1170,  1171,   163,
       3,  1114,   108,   167,   110,   164,   166,   531,    62,    63,
       3,   108,    70,   110,   163,    73,   533,   176,   512,   513,
     163,   127,   529,   157,   518,   519,   175,   521,   522,   729,
     127,   525,   175,   163,    88,    89,   530,   167,   532,   563,
    1078,  1232,  1233,   177,   178,   179,   166,   105,   811,   812,
     813,   163,   172,  1091,   108,   109,   110,   111,   165,   924,
      68,   163,    12,    71,   605,   167,     1,   513,     3,   176,
      20,   612,   518,   127,   128,   521,    84,  1082,  1191,   525,
    1085,  1194,  1087,   607,   165,    93,    94,    95,   629,   630,
     166,   166,   633,   634,    10,    11,   172,     7,     8,   623,
     176,    17,    18,    19,   633,   634,   114,   163,    12,   165,
     651,   976,   653,   717,   166,   644,    20,   164,   983,     3,
     985,   986,     6,   165,   166,   156,   157,   158,   159,   160,
     172,   672,   673,   674,   176,   163,   163,   922,     5,   167,
     167,   665,   683,    84,   685,   686,   687,   688,   163,  1141,
     691,  1143,   167,  1145,   671,   165,   919,   920,   921,   700,
     701,   702,   163,   704,   705,   682,   167,   163,   709,   165,
     156,   157,   163,   690,   165,   699,   670,   163,   156,   157,
     158,   159,   160,   677,   163,     1,   971,     3,     4,     5,
     163,     7,     8,     9,   167,   980,   959,   163,   961,     3,
     963,   167,     3,   697,   163,   163,   747,   167,   167,   167,
     163,    27,    28,    29,   167,   163,     3,   172,    53,   167,
     960,   176,   962,   163,   964,   165,   767,    43,   769,    64,
     163,   163,   163,    49,   167,   167,   167,     3,   154,   155,
     156,   157,   158,   159,   160,   786,   163,   165,   166,   163,
     167,   163,   793,   167,   795,   167,   773,   163,   799,   172,
       3,   167,  1025,   176,  1027,   794,  1029,   796,   797,   798,
     163,   800,   107,   163,   167,   163,   770,   167,   172,   167,
     163,   163,   823,   172,   167,   167,  1026,   176,  1028,   266,
    1030,    67,  1077,   270,   175,   836,   158,   159,   160,   134,
     135,   136,   137,   138,   139,   140,   164,   165,   849,   850,
     257,   258,   259,   163,     1,   165,     3,     1,   165,     3,
     861,   862,   165,   817,   865,   866,  1066,     1,  1068,     3,
    1070,   166,     3,   827,   150,   151,   152,   153,   974,   975,
     156,   157,     1,   163,     3,   165,   163,   163,   165,   163,
     166,   165,     3,   847,   170,   171,   112,   898,   174,   244,
     166,   246,   247,   163,     3,   165,   860,   163,     3,   165,
     911,   163,   913,   165,   163,   165,   165,  1117,     1,  1119,
       3,  1121,   163,   104,   165,   270,   163,   928,   165,   163,
     163,   165,   165,   164,   935,  1260,   937,   938,   163,   163,
     165,   165,  1267,  1268,   163,   173,   165,   924,   163,   163,
     165,   165,    10,    11,   955,    13,    14,    15,    16,    17,
      18,    19,   163,   952,   165,   163,   163,   165,   165,   163,
     924,   165,   317,   318,   319,   163,   977,   165,     1,   164,
       3,     4,     5,    52,     7,     8,     9,   163,   163,   165,
     165,   165,   993,   994,    95,   163,   997,   165,   999,  1000,
     163,   168,   165,  1004,    27,    28,    29,  1008,   163,   163,
     165,   165,  1013,     3,   163,   992,   165,   163,   166,   165,
      43,   163,   976,   165,    51,   370,    49,   981,   982,   983,
     984,   985,   986,   163,   163,   165,   165,    67,    95,  1040,
     385,   163,  1043,   165,   389,   701,   702,  1048,  1049,  1050,
    1051,   166,  1053,   110,   111,   163,   163,   165,   165,   204,
     205,   118,   119,   120,   121,   122,  1055,   165,  1057,   126,
    1059,   167,  1061,   130,   131,   172,    10,    11,   166,    13,
      14,    15,    16,    17,    18,    19,   163,   166,   165,   645,
     646,  1045,   166,    27,    28,    29,   154,   155,   156,   157,
     158,   159,   160,   993,   994,   418,   419,   166,   168,     3,
     166,    50,     3,  1114,   166,  1116,    53,     1,   166,     3,
       4,   166,   166,   166,   166,  1126,  1080,   150,   151,   152,
     153,   166,   166,   156,   157,     3,   165,   165,   164,   164,
     163,   165,    54,   166,  1098,   165,    30,   170,   171,    33,
      34,   174,     3,     3,    38,    39,    40,     3,    42,     3,
     165,    45,   163,   163,   163,   163,   172,  1168,   165,     3,
     166,   164,   167,    57,    58,    59,    60,  1131,  1155,   167,
      64,   164,   167,   167,   163,   167,   167,   167,   165,   165,
    1191,   166,  1169,  1194,   166,   166,   166,  1198,  1199,  1200,
    1201,   166,   166,  1157,  1158,     1,   166,     3,     4,  1163,
     166,    23,    96,    97,  1215,  1216,  1170,  1171,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    23,    52,   165,
     165,   164,   163,  1222,    30,   165,   165,    33,    34,   163,
     165,   125,    38,    39,    40,   129,    42,   165,   165,    45,
     165,   167,     3,   164,   164,     3,   148,   163,  1259,   163,
    1261,    57,    58,    59,    60,   163,   163,   163,    64,     3,
      66,    67,    68,   163,   163,    71,     5,   165,  1232,  1233,
       3,   165,   164,   164,   168,   169,   165,    83,    84,     3,
     174,   165,     3,     1,     3,     3,     4,    93,    94,    95,
      96,    97,     3,   176,     3,   165,  1260,   165,   165,   165,
     150,   165,   176,  1267,  1268,   167,   163,   166,   114,   167,
     165,   176,    30,   165,   165,    33,    34,   165,   165,   125,
      38,    39,    40,   129,    42,   167,   167,    45,     1,   167,
       3,     4,     5,   166,     7,     8,     9,   167,   166,    57,
      58,    59,    60,   148,   148,   172,    64,   173,   167,    48,
     167,   164,    69,   167,    27,    28,    29,   167,   167,   167,
     167,   164,   168,   169,   166,   166,   165,   165,   174,   163,
      43,   163,   163,   163,   163,    55,    49,     5,    96,    97,
     165,   165,     1,   165,     3,     4,     5,    50,     7,     8,
       9,   167,   167,   163,     1,   167,     3,     4,     5,   165,
       7,     8,     9,   167,   167,   167,   173,   125,    27,    28,
      29,   129,   167,    47,    47,   165,   163,   163,   150,   167,
      27,    28,    29,   165,   169,    23,     1,   167,     3,     4,
       5,   167,     7,     8,     9,   173,    43,   167,   167,   165,
     165,   276,   165,    66,    67,    68,   980,   167,    71,   165,
     168,   169,    27,    28,    29,   165,   174,    76,   164,   167,
      83,    84,   167,   167,   167,   167,   167,    86,    43,   164,
      93,    94,    95,   165,   165,   507,     5,   150,   151,   152,
     153,   208,   924,   156,   157,   770,   474,   268,   733,  1014,
     163,   114,   712,   166,   750,   784,   779,   170,   171,   206,
       1,   174,     3,     4,     5,   285,     7,     8,     9,   110,
     111,   653,   706,   816,   339,   784,   631,   118,   119,   120,
     121,   122,   391,   418,   895,   126,    27,    28,    29,   130,
     131,   419,   151,   152,   153,   164,    -1,   156,   157,   158,
      -1,    -1,    43,   150,   151,   152,   153,   166,    -1,   156,
     157,   170,   171,    -1,    -1,   174,   163,    -1,    -1,   166,
      -1,    -1,    -1,   170,   171,    -1,     1,   174,     3,     4,
       5,    -1,     7,     8,     9,   150,   151,   152,   153,    -1,
      -1,   156,   157,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,   166,    27,    28,    29,   170,   171,    -1,    -1,   174,
       1,    -1,     3,     4,     5,    -1,     7,     8,     9,    -1,
      -1,     1,    -1,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,     3,     4,     5,    -1,     7,     8,     9,   150,
     151,   152,   153,    -1,    -1,   156,   157,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,   166,    27,    28,    29,   170,
     171,    -1,    -1,   174,    -1,    76,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    86,     1,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,   151,   152,   153,    -1,
      -1,   156,   157,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,   166,   167,   110,   111,   170,   171,    -1,    -1,   174,
      -1,   118,   119,   120,   121,   122,    -1,    -1,    -1,   126,
     151,   152,   153,   130,   131,   156,   157,    -1,    -1,    -1,
      -1,   151,   152,   153,    -1,   166,   156,   157,    -1,   170,
     171,    -1,    -1,   174,    -1,    -1,   166,    -1,    -1,    -1,
     170,   171,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,   170,
     171,    -1,    -1,   174,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   166,
      -1,   156,   157,   170,   171,    -1,    -1,   174,    -1,   176,
      -1,   166,    -1,    -1,    -1,   170,   171,    -1,     1,   174,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,     1,
      -1,     3,     4,     5,    -1,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,     1,    -1,
       3,     4,     5,    -1,     7,     8,     9,   149,   150,    -1,
      -1,    -1,    -1,    -1,   156,    -1,   158,    -1,   160,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,   166,   156,   157,    -1,   170,   171,    62,
      63,   174,    -1,    -1,   166,    -1,    -1,    -1,   170,   171,
      -1,    -1,   174,    -1,    88,    -1,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,   166,   108,    -1,   110,   170,   171,    -1,
      -1,   174,    -1,    -1,    -1,   108,   109,   110,   111,    -1,
       3,     4,     5,   127,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,   151,   152,   153,
      -1,    -1,   156,   157,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,   166,   156,   157,    -1,   170,   171,    -1,    -1,
     174,    -1,    -1,   166,    -1,    -1,    -1,   170,   171,    -1,
      -1,   174,    -1,     3,     4,     5,    -1,     7,     8,     9,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    27,    28,    29,
      -1,    -1,    -1,    -1,    27,    28,    29,     3,     4,     5,
      -1,     7,     8,     9,    -1,    -1,   109,    -1,   111,     3,
       4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,    -1,    -1,    97,    -1,    -1,
      27,    28,    29,   166,    97,    -1,    -1,   170,   171,    -1,
      76,   174,     3,     4,     5,    -1,     7,     8,     9,    -1,
      86,    -1,    -1,    -1,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    27,    28,    29,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,    -1,    -1,   156,   157,   151,   152,
     153,    -1,    -1,   156,   157,    -1,   166,    -1,   168,   169,
     170,   171,    -1,   166,   174,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     166,    -1,   156,   157,   170,   171,    -1,    -1,   174,    -1,
      -1,    -1,   166,   167,    -1,    -1,   170,   171,    -1,    -1,
     174,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,   170,   171,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,   153,   154,   155,
     156,   157,   158,   159,   160,   166,    -1,    -1,    -1,   170,
     171,    10,    11,   174,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,   173,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   173,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   173,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   173,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   173,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    10,    11,   163,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    10,    11,
     167,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    10,    11,   167,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    10,    11,
     167,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    10,    11,   165,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    10,    11,   165,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,   149,
     150,    -1,    -1,    27,    28,    29,   156,    -1,   158,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    -1,    -1,    71,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,     1,   126,     3,    -1,    -1,   130,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    36,    37,    -1,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    -1,
      -1,    71,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,   131,   132,   133,    31,    32,    -1,    -1,    35,    36,
      37,   141,   142,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,    -1,   126,
     149,   150,    -1,   130,   131,   132,   133,   156,    -1,   158,
      -1,   160,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   149,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   149,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,   158,    -1,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    23,    87,   141,   197,   198,   200,   217,   245,   328,
       3,    24,   201,   202,     3,   166,     0,   217,    72,    74,
     248,   164,    24,   163,   166,     3,     3,     3,     4,     5,
       7,     8,     9,    27,    28,    29,   151,   152,   153,   156,
     157,   166,   170,   171,   174,   199,   225,   228,   234,   202,
       3,    83,   324,   163,   246,   166,     7,   228,   228,   228,
     228,     1,   228,   228,   228,   228,   225,   226,   228,     1,
     228,   225,   227,    10,    11,    13,    14,    15,    16,    17,
      18,    19,    25,    26,    27,    28,    29,   149,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   166,   172,
     176,    95,   325,   163,   167,     5,   168,   251,   225,   227,
     150,   167,   174,   163,   175,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     227,   225,     3,     3,     3,   165,   163,   166,   166,   250,
     167,   225,   227,   225,   150,   167,   150,   173,   164,   326,
      67,    83,    95,   322,   323,     5,    84,   252,     3,   174,
     176,   200,   236,   237,   238,   275,   276,   277,   165,   150,
     175,   225,   225,   225,   163,     3,   235,     3,     3,    83,
      65,   317,   318,   322,   167,     3,   262,   163,   167,   172,
     277,   278,     3,    66,    67,    83,   163,   167,   163,   167,
     247,   225,   175,   173,    67,   327,   163,   165,   165,   165,
       3,     3,   112,   310,   164,    84,   262,     1,   225,   163,
     175,   166,   110,   111,   118,   119,   120,   121,   122,   126,
     130,   131,   239,   260,   239,    95,   239,   261,   276,     3,
     238,     1,     3,    33,    44,    56,    61,    66,    67,    83,
      90,    91,   106,   113,   123,   141,   200,   253,   256,   257,
     281,     3,   163,   167,     3,   165,   164,   311,    52,   225,
     262,   173,   150,   173,   277,   174,   277,   104,   240,   240,
     240,   240,   165,   168,   272,     1,   143,   144,   145,   146,
     147,   166,   218,   219,   172,   210,   211,   234,   282,   235,
       1,    68,    93,    94,   114,   282,   284,   261,   261,   261,
       3,   166,   231,   232,   166,   232,     1,    53,    64,   107,
     134,   135,   136,   137,   138,   139,   140,   166,   287,   289,
     290,   291,   293,   294,     3,   166,   208,   209,   166,    31,
      32,    35,    36,    37,    41,    65,    68,    71,    75,    77,
      78,    79,    80,    81,    82,    84,    85,    92,    93,    94,
      95,    98,    99,   100,   101,   102,   114,   115,   116,   117,
     132,   133,   142,   203,   233,   260,   253,    51,   240,   260,
      67,   165,   199,   149,   156,   158,   160,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   312,   313,   314,   315,
     316,   319,   320,   225,   278,   167,   282,   283,   283,   283,
     283,     1,     8,   166,     1,   166,   232,   164,   165,   166,
     166,   166,   166,   166,    62,    63,    88,    89,   108,   109,
     110,   111,   127,   128,   220,   221,   168,   213,   214,   225,
     163,   165,   164,   210,   165,    50,     3,   240,   240,   240,
     166,   282,   220,   227,   163,   165,   221,   165,    53,   166,
       3,     6,   296,   297,   166,   166,   166,   166,   166,   166,
     166,     1,     3,    76,    86,   295,    53,   287,   165,   164,
     165,   164,   165,    70,    73,   105,   249,   283,     1,     3,
       1,     4,    30,    33,    34,    38,    39,    40,    42,    45,
      57,    58,    59,    60,    64,    96,    97,   125,   129,   168,
     169,   174,   212,   222,   234,   242,   302,   302,     1,     3,
     285,   286,     1,     3,   104,   267,   268,   271,   282,     1,
     104,   262,   263,   266,   282,     1,   235,     1,   235,     1,
     240,     1,   286,   166,   213,   218,   232,   240,   249,   240,
       3,   310,   165,    54,   313,   319,   316,   319,   150,   320,
     173,   175,     3,     3,     3,     3,   176,   227,   273,   274,
     165,   165,   225,   227,   227,   227,   227,   227,   163,   163,
     163,   163,     3,     8,     9,   166,   216,   174,   234,   241,
     243,   244,   150,   210,   225,   165,   283,   283,   283,   176,
     227,   279,   280,   166,   167,   167,   231,   167,   225,   164,
     164,   163,   165,    76,    86,   228,   299,   299,   299,   299,
     299,   299,   299,   167,   172,   295,   295,   156,   157,   163,
     298,   166,   216,   166,   216,   254,   167,   167,   167,   235,
     214,   167,   163,   165,   165,   166,   234,   241,    48,   150,
     200,   305,   166,   166,   166,   241,   234,   166,   241,   302,
      69,   150,   305,   166,   241,   166,   166,   166,   166,   216,
     158,   166,   234,   227,   165,   302,   306,   200,   165,   166,
     172,    10,   164,   165,   164,   172,   163,   165,   165,   164,
     282,   165,   163,   269,   165,   282,   165,   163,   264,   165,
     165,   165,   165,   165,   282,   286,   165,   165,   232,   213,
     232,   165,   218,   249,     1,   235,   283,    52,   150,   150,
     157,   177,   178,   179,   320,   321,   167,   164,     3,   167,
     163,   167,   165,   167,   167,   167,   167,   167,   221,   220,
      63,   221,    62,   220,   215,   225,   227,   172,   282,   164,
     163,   165,   225,   255,   235,   235,   235,     3,   167,   163,
     167,   227,   232,   232,   167,   225,   166,   225,   296,   225,
     228,   225,   228,   148,   163,   163,   163,   163,   163,   163,
     163,   228,   298,   298,     3,    12,    20,   215,   288,     1,
     288,    66,    67,    83,   203,   307,   308,   309,   165,   165,
       5,   227,   165,   164,     3,   302,    48,   200,   225,   225,
     225,   165,   165,     1,   234,   242,   164,     3,    69,     1,
     225,   165,   225,   225,     1,   225,   215,   200,     1,    76,
      86,   158,   223,   224,   225,   175,   306,   227,   225,    97,
     169,   212,   222,   225,    97,   212,   222,   225,   225,   225,
     285,   225,   270,   267,   271,   265,   262,   266,   286,   165,
     165,   232,   165,     3,   258,   259,     3,   235,   259,   165,
     165,   235,   177,   178,   179,   150,   165,   225,   166,   273,
     165,   165,   165,   165,   165,   167,   167,   167,   167,   167,
     167,   163,   167,   150,   175,   225,   225,   243,   173,    66,
      67,    83,   200,   229,   230,   165,   165,   165,   166,   279,
     167,   165,   165,   290,   293,   150,   225,   148,   148,   225,
     299,   215,   299,   299,   299,   215,   299,   173,    12,    20,
      12,    20,   172,   295,   295,   163,   167,   167,   167,   104,
     283,   104,   283,   104,   283,   307,   306,   163,   167,   225,
      48,   303,   302,   167,   167,   167,   167,   164,   225,    69,
     304,   167,   167,   167,   167,   167,   167,   167,   306,   167,
     225,   225,   167,    82,   163,   167,   167,   150,   173,   166,
     166,   225,   225,   165,   166,   225,   225,   165,   150,   271,
     266,   165,   165,   164,   163,   165,   165,   165,   165,   321,
     167,   225,   215,   225,   173,   104,   283,   104,   283,   104,
     283,   203,   229,   302,     1,   167,   225,   165,   165,   225,
     163,   225,   225,   163,     3,   163,   300,   301,   163,   163,
     163,   163,   167,   163,     3,   166,     3,   166,     3,   166,
       3,   166,   228,   167,   167,   215,   283,   235,   283,   235,
     283,   235,    55,     5,   165,   165,   203,   204,   205,     1,
      43,   206,   207,   227,     1,   207,     1,   207,   302,   225,
     165,   205,   306,   306,   302,   306,   302,   302,   200,   224,
     224,   165,   225,   225,   165,   165,   225,   165,   165,   225,
     225,   258,   165,   167,   163,   167,   150,   283,   235,   283,
     235,   283,   235,    50,   167,   167,   150,   225,   215,   234,
     167,   163,   215,   215,   215,   225,   165,   215,   167,   228,
     167,   228,   167,   228,   167,   228,   173,   235,   165,   235,
     165,   235,   165,   167,   203,   305,    49,   150,   150,   306,
       1,    49,   206,   150,    49,    49,    49,    49,   165,   305,
      47,    47,   306,   173,   167,   167,   173,   215,   225,   235,
     165,   235,   165,   235,   165,   225,   167,   300,   165,   234,
     300,   163,   300,   300,   163,    21,    22,   150,   292,   292,
     292,   292,   165,   165,   165,   165,    48,   306,   306,   306,
       1,   225,    69,   306,   306,   222,   222,   167,   165,   165,
     165,   167,   172,   167,   215,   167,   167,   215,   225,   225,
     225,   225,   165,   165,   225,   225,   165,   228,   165,   300,
     165,   165,   300,   167,   167,   167,   167,   242,     1,   242,
     165,   165,   173,   167,   167,   167,   167,   167,   167,   164,
     167,   164,   165,   165,   225,   302,   225,   167,   167,   302,
     302
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

/* Line 1455 of yacc.c  */
#line 251 "parse.y"
    { (yyval.number) = (yyvsp[(1) - (1)].number); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 253 "parse.y"
    { (yyval.number) = (yyvsp[(1) - (1)].number); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 255 "parse.y"
    { (yyval.number) = pform_verinum_with_size((yyvsp[(1) - (2)].number),(yyvsp[(2) - (2)].number), (yylsp[(2) - (2)]).text, (yylsp[(2) - (2)]).first_line); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 262 "parse.y"
    { (yyval.named_pexprs) = (yyvsp[(2) - (3)].named_pexprs); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 263 "parse.y"
    { (yyval.named_pexprs) = 0; ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 264 "parse.y"
    { (yyval.named_pexprs) = 0; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 269 "parse.y"
    { svector<named_pexpr_t*>*tmp =
			new svector<named_pexpr_t*>(*(yyvsp[(1) - (3)].named_pexprs),(yyvsp[(3) - (3)].named_pexpr));
		  delete (yyvsp[(1) - (3)].named_pexprs);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 275 "parse.y"
    { svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].named_pexpr);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 284 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(1) - (1)].text));
		  tmp->parm = 0;
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 291 "parse.y"
    { PExpr*tmp = (yyvsp[(3) - (3)].expr);
		  if (tmp && !pform_expression_is_constant(tmp)) {
			yyerror((yylsp[(3) - (3)]), "error: attribute value "
			            "expression must be constant.");
			delete tmp;
			tmp = 0;
		  }
		  named_pexpr_t*tmp2 = new named_pexpr_t;
		  tmp2->name = lex_strings.make((yyvsp[(1) - (3)].text));
		  tmp2->parm = tmp;
		  delete (yyvsp[(1) - (3)].text);
		  (yyval.named_pexpr) = tmp2;
		;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 314 "parse.y"
    { pform_set_net_range((yyvsp[(4) - (5)].perm_strings), (yyvsp[(3) - (5)].exprs), (yyvsp[(2) - (5)].flag));
		;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 317 "parse.y"
    { pform_set_net_range((yyvsp[(3) - (4)].perm_strings), 0, (yyvsp[(2) - (4)].flag));
		;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 320 "parse.y"
    { pform_set_reg_integer((yyvsp[(2) - (3)].perm_strings));
		;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 323 "parse.y"
    { pform_set_reg_time((yyvsp[(2) - (3)].perm_strings));
		;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 326 "parse.y"
    { pform_make_reals((yyvsp[(2) - (3)].perm_strings), (yylsp[(1) - (3)]).text, (yylsp[(1) - (3)]).first_line);
		;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 329 "parse.y"
    { pform_make_reals((yyvsp[(2) - (3)].perm_strings), (yylsp[(1) - (3)]).text, (yylsp[(1) - (3)]).first_line);
		;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 338 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in reg variable list.");
		  yyerrok;
		;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 342 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in integer variable list.");
		  yyerrok;
		;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 346 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in time variable list.");
		  yyerrok;
		;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 350 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in real variable list.");
		  yyerrok;
		;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 354 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in realtime variable list.");
		  yyerrok;
		;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 358 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in parameter list.");
		  yyerrok;
		;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 362 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error localparam list.");
		  yyerrok;
		;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 379 "parse.y"
    { PCase::Item*tmp = new PCase::Item;
		  tmp->expr = *(yyvsp[(1) - (3)].exprs);
		  tmp->stat = (yyvsp[(3) - (3)].statement);
		  delete (yyvsp[(1) - (3)].exprs);
		  (yyval.citem) = tmp;
		;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 386 "parse.y"
    { PCase::Item*tmp = new PCase::Item;
		  tmp->stat = (yyvsp[(3) - (3)].statement);
		  (yyval.citem) = tmp;
		;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 391 "parse.y"
    { PCase::Item*tmp = new PCase::Item;
		  tmp->stat = (yyvsp[(2) - (2)].statement);
		  (yyval.citem) = tmp;
		;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 396 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: Incomprehensible case expression.");
		  yyerrok;
		;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 403 "parse.y"
    { svector<PCase::Item*>*tmp;
		  tmp = new svector<PCase::Item*>(*(yyvsp[(1) - (2)].citems), (yyvsp[(2) - (2)].citem));
		  delete (yyvsp[(1) - (2)].citems);
		  (yyval.citems) = tmp;
		;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 409 "parse.y"
    { svector<PCase::Item*>*tmp = new svector<PCase::Item*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].citem);
		  (yyval.citems) = tmp;
		;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 428 "parse.y"
    { PExpr*tmp = (yyvsp[(3) - (3)].expr);
		  if (!pform_expression_is_constant(tmp)) {
			yyerror((yylsp[(3) - (3)]), "error: parameter value "
			            "must be constant.");
			delete tmp;
			tmp = 0;
		  }
		  pform_set_defparam(*(yyvsp[(1) - (3)].hier), (yyvsp[(3) - (3)].expr));
		  delete (yyvsp[(1) - (3)].hier);
		;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 443 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: defparam may not include a range.");
		  delete (yyvsp[(1) - (2)].exprs);
		;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 451 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = (yyvsp[(2) - (2)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 456 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = (yyvsp[(3) - (4)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 464 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = (yyvsp[(2) - (2)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 469 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = (yyvsp[(3) - (4)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 474 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(2);
		  (*tmp)[0] = (yyvsp[(3) - (6)].expr);
		  (*tmp)[1] = (yyvsp[(5) - (6)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 480 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(3);
		  (*tmp)[0] = (yyvsp[(3) - (8)].expr);
		  (*tmp)[1] = (yyvsp[(5) - (8)].expr);
		  (*tmp)[2] = (yyvsp[(7) - (8)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 489 "parse.y"
    { (yyval.exprs) = (yyvsp[(1) - (1)].exprs); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 490 "parse.y"
    { (yyval.exprs) = 0; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 495 "parse.y"
    { PExpr*tmp = (yyvsp[(1) - (1)].expr);
		  (yyval.expr) = tmp;
		;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 499 "parse.y"
    { (yyval.expr) = pform_select_mtm_expr((yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].expr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 505 "parse.y"
    { verinum*tmp = (yyvsp[(1) - (1)].number);
		  if (tmp == 0) {
			yyerror((yylsp[(1) - (1)]), "internal error: delay.");
			(yyval.expr) = 0;
		  } else {
			(yyval.expr) = new PENumber(tmp);
			(yyval.expr)->set_file((yylsp[(1) - (1)]).text);
			(yyval.expr)->set_lineno((yylsp[(1) - (1)]).first_line);
		  }
		;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 516 "parse.y"
    { verireal*tmp = (yyvsp[(1) - (1)].realtime);
		  if (tmp == 0) {
			yyerror((yylsp[(1) - (1)]), "internal error: delay.");
			(yyval.expr) = 0;
		  } else {
			(yyval.expr) = new PEFNumber(tmp);
			(yyval.expr)->set_file((yylsp[(1) - (1)]).text);
			(yyval.expr)->set_lineno((yylsp[(1) - (1)]).first_line);
		  }
		;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 527 "parse.y"
    { PEIdent*tmp = new PEIdent(hname_t((yyvsp[(1) - (1)].text)));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		  delete (yyvsp[(1) - (1)].text);
		;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 539 "parse.y"
    { perm_string tmp3 = lex_strings.make((yyvsp[(3) - (8)].text));
		  pform_set_type_attrib(tmp3, (yyvsp[(5) - (8)].text), (yyvsp[(7) - (8)].text));
		  delete (yyvsp[(3) - (8)].text);
		  delete (yyvsp[(5) - (8)].text);
		;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 548 "parse.y"
    { (yyval.drive).str0 = (yyvsp[(2) - (5)].drive).str0;
		  (yyval.drive).str1 = (yyvsp[(4) - (5)].drive).str1;
		;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 552 "parse.y"
    { (yyval.drive).str0 = (yyvsp[(4) - (5)].drive).str0;
		  (yyval.drive).str1 = (yyvsp[(2) - (5)].drive).str1;
		;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 556 "parse.y"
    { (yyval.drive).str0 = (yyvsp[(2) - (5)].drive).str0;
		  (yyval.drive).str1 = PGate::HIGHZ;
		;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 560 "parse.y"
    { (yyval.drive).str0 = PGate::HIGHZ;
		  (yyval.drive).str1 = (yyvsp[(2) - (5)].drive).str1;
		;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 564 "parse.y"
    { (yyval.drive).str0 = (yyvsp[(4) - (5)].drive).str0;
		  (yyval.drive).str1 = PGate::HIGHZ;
		;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 568 "parse.y"
    { (yyval.drive).str0 = PGate::HIGHZ;
		  (yyval.drive).str1 = (yyvsp[(4) - (5)].drive).str1;
		;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 574 "parse.y"
    { (yyval.drive) = (yyvsp[(1) - (1)].drive); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 575 "parse.y"
    { (yyval.drive).str0 = PGate::STRONG; (yyval.drive).str1 = PGate::STRONG; ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 579 "parse.y"
    { (yyval.drive).str0 = PGate::SUPPLY; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 580 "parse.y"
    { (yyval.drive).str0 = PGate::STRONG; ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 581 "parse.y"
    { (yyval.drive).str0 = PGate::PULL; ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 582 "parse.y"
    { (yyval.drive).str0 = PGate::WEAK; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 586 "parse.y"
    { (yyval.drive).str1 = PGate::SUPPLY; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 587 "parse.y"
    { (yyval.drive).str1 = PGate::STRONG; ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 588 "parse.y"
    { (yyval.drive).str1 = PGate::PULL; ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 589 "parse.y"
    { (yyval.drive).str1 = PGate::WEAK; ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 594 "parse.y"
    { PEIdent*tmpi = new PEIdent(*(yyvsp[(2) - (2)].hier));
		  tmpi->set_file((yylsp[(2) - (2)]).text);
		  tmpi->set_lineno((yylsp[(2) - (2)]).first_line);
		  delete (yyvsp[(2) - (2)].hier);
		  PEEvent*tmpe = new PEEvent(PEEvent::ANYEDGE, tmpi);
		  PEventStatement*tmps = new PEventStatement(tmpe);
		  tmps->set_file((yylsp[(1) - (2)]).text);
		  tmps->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.event_statement) = tmps;
		;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 605 "parse.y"
    { PEventStatement*tmp = new PEventStatement(*(yyvsp[(3) - (4)].event_expr));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(3) - (4)].event_expr);
		  (yyval.event_statement) = tmp;
		;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 612 "parse.y"
    { yyerror((yylsp[(1) - (4)]), "error: Malformed event control expression.");
		  (yyval.event_statement) = 0;
		;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 619 "parse.y"
    { (yyval.event_expr) = (yyvsp[(1) - (1)].event_expr); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 621 "parse.y"
    { svector<PEEvent*>*tmp = new svector<PEEvent*>(*(yyvsp[(1) - (3)].event_expr), *(yyvsp[(3) - (3)].event_expr));
		  delete (yyvsp[(1) - (3)].event_expr);
		  delete (yyvsp[(3) - (3)].event_expr);
		  (yyval.event_expr) = tmp;
		;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 627 "parse.y"
    { svector<PEEvent*>*tmp = new svector<PEEvent*>(*(yyvsp[(1) - (3)].event_expr), *(yyvsp[(3) - (3)].event_expr));
		  delete (yyvsp[(1) - (3)].event_expr);
		  delete (yyvsp[(3) - (3)].event_expr);
		  (yyval.event_expr) = tmp;
		;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 636 "parse.y"
    { PEEvent*tmp = new PEEvent(PEEvent::POSEDGE, (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  (yyval.event_expr) = tl;
		;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 644 "parse.y"
    { PEEvent*tmp = new PEEvent(PEEvent::NEGEDGE, (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  (yyval.event_expr) = tl;
		;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 652 "parse.y"
    { PEEvent*tmp = new PEEvent(PEEvent::ANYEDGE, (yyvsp[(1) - (1)].expr));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  svector<PEEvent*>*tl = new svector<PEEvent*>(1);
		  (*tl)[0] = tmp;
		  (yyval.event_expr) = tl;
		;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 663 "parse.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 665 "parse.y"
    { (yyval.expr) = (yyvsp[(2) - (2)].expr); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 667 "parse.y"
    { PEUnary*tmp = new PEUnary('-', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 673 "parse.y"
    { PEUnary*tmp = new PEUnary('~', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 679 "parse.y"
    { PEUnary*tmp = new PEUnary('&', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 685 "parse.y"
    { PEUnary*tmp = new PEUnary('!', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 691 "parse.y"
    { PEUnary*tmp = new PEUnary('|', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 697 "parse.y"
    { PEUnary*tmp = new PEUnary('^', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 703 "parse.y"
    { PEUnary*tmp = new PEUnary('A', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 709 "parse.y"
    { PEUnary*tmp = new PEUnary('N', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 715 "parse.y"
    { PEUnary*tmp = new PEUnary('X', (yyvsp[(2) - (2)].expr));
		  tmp->set_file((yylsp[(2) - (2)]).text);
		  tmp->set_lineno((yylsp[(2) - (2)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 721 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: Operand of unary ! "
			  "is not a primary expression.");
		  (yyval.expr) = 0;
		;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 726 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: Operand of reduction ^ "
			  "is not a primary expression.");
		  (yyval.expr) = 0;
		;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 731 "parse.y"
    { PEBinary*tmp = new PEBinary('^', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 737 "parse.y"
    { PEBinary*tmp = new PEBinary('*', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 743 "parse.y"
    { PEBinary*tmp = new PEBinary('/', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 749 "parse.y"
    { PEBinary*tmp = new PEBinary('%', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 755 "parse.y"
    { PEBinary*tmp = new PEBinary('+', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 761 "parse.y"
    { PEBinary*tmp = new PEBinary('-', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 767 "parse.y"
    { PEBinary*tmp = new PEBinary('&', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 773 "parse.y"
    { PEBinary*tmp = new PEBinary('|', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 779 "parse.y"
    { PEBinary*tmp = new PEBinary('A', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 785 "parse.y"
    { PEBinary*tmp = new PEBinary('O', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 791 "parse.y"
    { PEBinary*tmp = new PEBinary('X', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 797 "parse.y"
    { PEBinary*tmp = new PEBinary('<', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 803 "parse.y"
    { PEBinary*tmp = new PEBinary('>', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 809 "parse.y"
    { PEBinary*tmp = new PEBinary('l', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 815 "parse.y"
    { PEBinary*tmp = new PEBinary('r', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 821 "parse.y"
    { PEBinary*tmp = new PEBinary('R', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 827 "parse.y"
    { PEBinary*tmp = new PEBinary('e', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 833 "parse.y"
    { PEBinary*tmp = new PEBinary('E', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 839 "parse.y"
    { PEBinary*tmp = new PEBinary('L', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 845 "parse.y"
    { PEBinary*tmp = new PEBinary('G', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 851 "parse.y"
    { PEBinary*tmp = new PEBinary('n', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 857 "parse.y"
    { PEBinary*tmp = new PEBinary('N', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 863 "parse.y"
    { PEBinary*tmp = new PEBinary('o', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 869 "parse.y"
    { PEBinary*tmp = new PEBinary('a', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 875 "parse.y"
    { PETernary*tmp = new PETernary((yyvsp[(1) - (5)].expr), (yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].expr));
		  tmp->set_file((yylsp[(2) - (5)]).text);
		  tmp->set_lineno((yylsp[(2) - (5)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 884 "parse.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 886 "parse.y"
    { switch (min_typ_max_flag) {
		      case MIN:
			(yyval.expr) = (yyvsp[(1) - (5)].expr);
			delete (yyvsp[(3) - (5)].expr);
			delete (yyvsp[(5) - (5)].expr);
			break;
		      case TYP:
			delete (yyvsp[(1) - (5)].expr);
			(yyval.expr) = (yyvsp[(3) - (5)].expr);
			delete (yyvsp[(5) - (5)].expr);
			break;
		      case MAX:
			delete (yyvsp[(1) - (5)].expr);
			delete (yyvsp[(3) - (5)].expr);
			(yyval.expr) = (yyvsp[(5) - (5)].expr);
			break;
		  }
		;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 912 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(*(yyvsp[(1) - (3)].exprs), (yyvsp[(3) - (3)].expr));
		  delete (yyvsp[(1) - (3)].exprs);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 917 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 922 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(1);
		  (*tmp)[0] = 0;
		  (yyval.exprs) = tmp;
		;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 928 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(*(yyvsp[(1) - (2)].exprs), 0);
		  delete (yyvsp[(1) - (2)].exprs);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 936 "parse.y"
    { assert((yyvsp[(1) - (1)].number));
		  PENumber*tmp = new PENumber((yyvsp[(1) - (1)].number));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 943 "parse.y"
    { PEFNumber*tmp = new PEFNumber((yyvsp[(1) - (1)].realtime));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 949 "parse.y"
    { PEString*tmp = new PEString((yyvsp[(1) - (1)].text));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 955 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (1)].hier));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		  delete (yyvsp[(1) - (1)].hier);
		;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 962 "parse.y"
    { PECallFunction*tmp = new PECallFunction(hname_t((yyvsp[(1) - (1)].text)));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  (yyval.expr) = tmp;
		  delete (yyvsp[(1) - (1)].text);
		;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 969 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (4)].hier));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  tmp->msb_ = (yyvsp[(3) - (4)].expr);
		  delete (yyvsp[(1) - (4)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 977 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (6)].hier));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  tmp->msb_ = (yyvsp[(3) - (6)].expr);
		  tmp->lsb_ = (yyvsp[(5) - (6)].expr);
		  delete (yyvsp[(1) - (6)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 986 "parse.y"
    { PECallFunction*tmp = new PECallFunction(*(yyvsp[(1) - (4)].hier), *(yyvsp[(3) - (4)].exprs));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(1) - (4)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 993 "parse.y"
    { PECallFunction*tmp = new PECallFunction(hname_t((yyvsp[(1) - (4)].text)), *(yyvsp[(3) - (4)].exprs));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.expr) = tmp;
		;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 999 "parse.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1001 "parse.y"
    { PEConcat*tmp = new PEConcat(*(yyvsp[(2) - (3)].exprs));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  for (unsigned idx = 0 ;  idx < (*(yyvsp[(2) - (3)].exprs)).count() ;  idx += 1) {
			PExpr*ex = (*(yyvsp[(2) - (3)].exprs))[idx];
			if (ex == 0) {
			      yyerror((yylsp[(1) - (3)]), "error: Null arguments not allowed"
				      " in repeat expressions.");
			      break;
			}
		  }
		  delete (yyvsp[(2) - (3)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1016 "parse.y"
    { PExpr*rep = (yyvsp[(2) - (6)].expr);
		  if (!pform_expression_is_constant((yyvsp[(2) - (6)].expr))) {
			yyerror((yylsp[(2) - (6)]), "error: Repeat expression "
			            "must be constant.");
			delete rep;
			rep = 0;
		  }
		  PEConcat*tmp = new PEConcat(*(yyvsp[(4) - (6)].exprs), rep);
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  delete (yyvsp[(4) - (6)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1037 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1044 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1051 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		  yyerror((yylsp[(1) - (4)]), "Functions may not have output ports.");
		;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1059 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		  yyerror((yylsp[(1) - (5)]), "Functions may not have output ports.");
		;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1067 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		  yyerror((yylsp[(1) - (4)]), "Functions may not have inout ports.");
		;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1075 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		  yyerror((yylsp[(1) - (5)]), "Functions may not have inout ports.");
		;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1083 "parse.y"
    { (yyval.wires) = 0; ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1092 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1094 "parse.y"
    { if ((yyvsp[(1) - (2)].wires) && (yyvsp[(2) - (2)].wires)) {
		        svector<PWire*>*tmp = new svector<PWire*>(*(yyvsp[(1) - (2)].wires), *(yyvsp[(2) - (2)].wires));
			delete (yyvsp[(1) - (2)].wires);
			delete (yyvsp[(2) - (2)].wires);
			(yyval.wires) = tmp;
		  } else if ((yyvsp[(1) - (2)].wires)) {
			(yyval.wires) = (yyvsp[(1) - (2)].wires);
		  } else {
			(yyval.wires) = (yyvsp[(2) - (2)].wires);
		  }
		;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1111 "parse.y"
    { lgate*tmp = new lgate;
		  tmp->name = (yyvsp[(1) - (4)].text);
		  tmp->parms = (yyvsp[(3) - (4)].exprs);
		  tmp->file  = (yylsp[(1) - (4)]).text;
		  tmp->lineno = (yylsp[(1) - (4)]).first_line;
		  delete (yyvsp[(1) - (4)].text);
		  (yyval.gate) = tmp;
		;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1121 "parse.y"
    { lgate*tmp = new lgate;
		  svector<PExpr*>*rng = (yyvsp[(2) - (5)].exprs);
		  tmp->name = (yyvsp[(1) - (5)].text);
		  tmp->parms = (yyvsp[(4) - (5)].exprs);
		  tmp->range[0] = (*rng)[0];
		  tmp->range[1] = (*rng)[1];
		  tmp->file  = (yylsp[(1) - (5)]).text;
		  tmp->lineno = (yylsp[(1) - (5)]).first_line;
		  delete (yyvsp[(1) - (5)].text);
		  delete rng;
		  (yyval.gate) = tmp;
		;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1134 "parse.y"
    { lgate*tmp = new lgate;
		  tmp->name = "";
		  tmp->parms = (yyvsp[(2) - (3)].exprs);
		  tmp->file  = (yylsp[(1) - (3)]).text;
		  tmp->lineno = (yylsp[(1) - (3)]).first_line;
		  (yyval.gate) = tmp;
		;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1145 "parse.y"
    { lgate*tmp = new lgate;
		  svector<PExpr*>*rng = (yyvsp[(2) - (2)].exprs);
		  tmp->name = (yyvsp[(1) - (2)].text);
		  tmp->parms = 0;
		  tmp->range[0] = (*rng)[0];
		  tmp->range[1] = (*rng)[1];
		  tmp->file  = (yylsp[(1) - (2)]).text;
		  tmp->lineno = (yylsp[(1) - (2)]).first_line;
		  delete (yyvsp[(1) - (2)].text);
		  delete rng;
		  (yyval.gate) = tmp;
		;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1161 "parse.y"
    { lgate*tmp = new lgate;
		  tmp->name = (yyvsp[(1) - (4)].text);
		  tmp->parms_by_name = (yyvsp[(3) - (4)].named_pexprs);
		  tmp->file  = (yylsp[(1) - (4)]).text;
		  tmp->lineno = (yylsp[(1) - (4)]).first_line;
		  delete (yyvsp[(1) - (4)].text);
		  (yyval.gate) = tmp;
		;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1173 "parse.y"
    { svector<lgate>*tmp1 = (yyvsp[(1) - (3)].gates);
		  lgate*tmp2 = (yyvsp[(3) - (3)].gate);
		  svector<lgate>*out = new svector<lgate> (*tmp1, *tmp2);
		  delete tmp1;
		  delete tmp2;
		  (yyval.gates) = out;
		;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1181 "parse.y"
    { svector<lgate>*tmp = new svector<lgate>(1);
		  (*tmp)[0] = *(yyvsp[(1) - (1)].gate);
		  delete (yyvsp[(1) - (1)].gate);
		  (yyval.gates) = tmp;
		;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1189 "parse.y"
    { (yyval.gatetype) = PGBuiltin::AND; ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1190 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NAND; ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1191 "parse.y"
    { (yyval.gatetype) = PGBuiltin::OR; ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1192 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOR; ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1193 "parse.y"
    { (yyval.gatetype) = PGBuiltin::XOR; ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1194 "parse.y"
    { (yyval.gatetype) = PGBuiltin::XNOR; ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1195 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUF; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1196 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUFIF0; ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1197 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUFIF1; ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1198 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOT; ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1199 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOTIF0; ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1200 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOTIF1; ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1201 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NMOS; ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1202 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RNMOS; ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1203 "parse.y"
    { (yyval.gatetype) = PGBuiltin::PMOS; ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1204 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RPMOS; ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1205 "parse.y"
    { (yyval.gatetype) = PGBuiltin::CMOS; ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1206 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RCMOS; ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1207 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRAN; ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1208 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRAN; ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1209 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRANIF0; ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1210 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRANIF1; ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1211 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRANIF0; ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1212 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRANIF1; ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1213 "parse.y"
    { (yyval.gatetype) = PGBuiltin::PLL; ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1222 "parse.y"
    { (yyval.hier) = new hname_t((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1226 "parse.y"
    { hname_t * tmp = (yyvsp[(1) - (3)].hier);
		  tmp->append((yyvsp[(3) - (3)].text));
		  delete (yyvsp[(3) - (3)].text);
		  (yyval.hier) = tmp;
		;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1238 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1244 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1270 "parse.y"
    { svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].mport);
		  (yyval.mports) = tmp;
		;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1276 "parse.y"
    { svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*(yyvsp[(1) - (3)].mports), (yyvsp[(3) - (3)].mport));
		  delete (yyvsp[(1) - (3)].mports);
		  (yyval.mports) = tmp;
		;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1285 "parse.y"
    { svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].mport);
		  (yyval.mports) = tmp;
		;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1291 "parse.y"
    { svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*(yyvsp[(1) - (3)].mports), (yyvsp[(3) - (3)].mport));
		  delete (yyvsp[(1) - (3)].mports);
		  (yyval.mports) = tmp;
		;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1297 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(3) - (3)].text), (yylsp[(3) - (3)]).text,
						     (yylsp[(3) - (3)]).first_line);
		  svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*(yyvsp[(1) - (3)].mports), ptmp);

		    /* Get the port declaration details, the port type
		       and what not, from context data stored by the
		       last port_declaration rule. */
		  pform_module_define_port((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].text),
					port_declaration_context.port_type,
					port_declaration_context.port_net_type,
					port_declaration_context.sign_flag,
					port_declaration_context.range, 0);
		  delete (yyvsp[(1) - (3)].mports);
		  (yyval.mports) = tmp;
		;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1319 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(6) - (6)].text), (yylsp[(2) - (6)]).text,
						     (yylsp[(2) - (6)]).first_line);
		  pform_module_define_port((yylsp[(2) - (6)]), (yyvsp[(6) - (6)].text), NetNet::PINPUT,
					   (yyvsp[(3) - (6)].nettype), (yyvsp[(4) - (6)].flag), (yyvsp[(5) - (6)].exprs), (yyvsp[(1) - (6)].named_pexprs));
		  port_declaration_context.port_type = NetNet::PINPUT;
		  port_declaration_context.port_net_type = (yyvsp[(3) - (6)].nettype);
		  port_declaration_context.sign_flag = (yyvsp[(4) - (6)].flag);
		  port_declaration_context.range = (yyvsp[(5) - (6)].exprs);
		  delete (yyvsp[(1) - (6)].named_pexprs);
		  delete (yyvsp[(6) - (6)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1334 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(6) - (6)].text), (yylsp[(2) - (6)]).text,
						     (yylsp[(2) - (6)]).first_line);
		  pform_module_define_port((yylsp[(2) - (6)]), (yyvsp[(6) - (6)].text), NetNet::PINOUT,
					   (yyvsp[(3) - (6)].nettype), (yyvsp[(4) - (6)].flag), (yyvsp[(5) - (6)].exprs), (yyvsp[(1) - (6)].named_pexprs));
		  port_declaration_context.port_type = NetNet::PINOUT;
		  port_declaration_context.port_net_type = (yyvsp[(3) - (6)].nettype);
		  port_declaration_context.sign_flag = (yyvsp[(4) - (6)].flag);
		  port_declaration_context.range = (yyvsp[(5) - (6)].exprs);
		  delete (yyvsp[(1) - (6)].named_pexprs);
		  delete (yyvsp[(6) - (6)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1349 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(6) - (6)].text), (yylsp[(2) - (6)]).text,
						     (yylsp[(2) - (6)]).first_line);
		  pform_module_define_port((yylsp[(2) - (6)]), (yyvsp[(6) - (6)].text), NetNet::POUTPUT,
					   (yyvsp[(3) - (6)].nettype), (yyvsp[(4) - (6)].flag), (yyvsp[(5) - (6)].exprs), (yyvsp[(1) - (6)].named_pexprs));
		  port_declaration_context.port_type = NetNet::POUTPUT;
		  port_declaration_context.port_net_type = (yyvsp[(3) - (6)].nettype);
		  port_declaration_context.sign_flag = (yyvsp[(4) - (6)].flag);
		  port_declaration_context.range = (yyvsp[(5) - (6)].exprs);
		  delete (yyvsp[(1) - (6)].named_pexprs);
		  delete (yyvsp[(6) - (6)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1364 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(6) - (6)].text), (yylsp[(2) - (6)]).text,
						     (yylsp[(2) - (6)]).first_line);
		  pform_module_define_port((yylsp[(2) - (6)]), (yyvsp[(6) - (6)].text), NetNet::POUTPUT,
					   (yyvsp[(3) - (6)].nettype), (yyvsp[(4) - (6)].flag), (yyvsp[(5) - (6)].exprs), (yyvsp[(1) - (6)].named_pexprs));
		  port_declaration_context.port_type = NetNet::POUTPUT;
		  port_declaration_context.port_net_type = (yyvsp[(3) - (6)].nettype);
		  port_declaration_context.sign_flag = (yyvsp[(4) - (6)].flag);
		  port_declaration_context.range = (yyvsp[(5) - (6)].exprs);
		  delete (yyvsp[(1) - (6)].named_pexprs);
		  delete (yyvsp[(6) - (6)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1379 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(6) - (8)].text), (yylsp[(2) - (8)]).text,
						     (yylsp[(2) - (8)]).first_line);
		  pform_module_define_port((yylsp[(2) - (8)]), (yyvsp[(6) - (8)].text), NetNet::POUTPUT,
					   (yyvsp[(3) - (8)].nettype), (yyvsp[(4) - (8)].flag), (yyvsp[(5) - (8)].exprs), (yyvsp[(1) - (8)].named_pexprs));
		  port_declaration_context.port_type = NetNet::POUTPUT;
		  port_declaration_context.port_net_type = (yyvsp[(3) - (8)].nettype);
		  port_declaration_context.sign_flag = (yyvsp[(4) - (8)].flag);
		  port_declaration_context.range = (yyvsp[(5) - (8)].exprs);

		  if (! pform_expression_is_constant((yyvsp[(8) - (8)].expr)))
			yyerror((yylsp[(8) - (8)]), "error: register declaration assignment"
				" value must be a constant expression.");
		  pform_make_reginit((yylsp[(6) - (8)]), (yyvsp[(6) - (8)].text), (yyvsp[(8) - (8)].expr));

		  delete (yyvsp[(1) - (8)].named_pexprs);
		  delete (yyvsp[(6) - (8)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1403 "parse.y"
    { (yyval.nettype) = (yyvsp[(1) - (1)].nettype); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1404 "parse.y"
    { (yyval.nettype) = NetNet::IMPLICIT; ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1407 "parse.y"
    { (yyval.flag) = true; ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1407 "parse.y"
    {(yyval.flag) = false; ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1414 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (1)].hier));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1421 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (4)].hier));
		  PExpr*sel = (yyvsp[(3) - (4)].expr);
		  if (! pform_expression_is_constant(sel)) {
			yyerror((yylsp[(2) - (4)]), "error: Bit select in lvalue must "
			        "contain a constant expression.");
			delete sel;
		  } else {
			tmp->msb_ = sel;
		  }
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(1) - (4)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1436 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (2)].hier));
		  assert((yyvsp[(2) - (2)].exprs)->count() == 2);
		  tmp->msb_ = (*(yyvsp[(2) - (2)].exprs))[0];
		  tmp->lsb_ = (*(yyvsp[(2) - (2)].exprs))[1];
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  delete (yyvsp[(1) - (2)].hier);
		  delete (yyvsp[(2) - (2)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1447 "parse.y"
    { PEConcat*tmp = new PEConcat(*(yyvsp[(2) - (3)].exprs));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1459 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (1)].hier));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1466 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (4)].hier));
		  tmp->msb_ = (yyvsp[(3) - (4)].expr);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(1) - (4)].hier);

		  (yyval.expr) = tmp;
		;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1475 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (6)].hier));
		  tmp->msb_ = (yyvsp[(3) - (6)].expr);
		  tmp->lsb_ = (yyvsp[(5) - (6)].expr);
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  delete (yyvsp[(1) - (6)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1484 "parse.y"
    { PEConcat*tmp = new PEConcat(*(yyvsp[(2) - (3)].exprs));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1494 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(2);
		  (*tmp)[0] = (yyvsp[(1) - (3)].expr);
		  (*tmp)[1] = (yyvsp[(3) - (3)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1503 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(*(yyvsp[(1) - (3)].exprs), *(yyvsp[(3) - (3)].exprs));
		  delete (yyvsp[(1) - (3)].exprs);
		  delete (yyvsp[(3) - (3)].exprs);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1509 "parse.y"
    { (yyval.exprs) = (yyvsp[(1) - (1)].exprs); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1518 "parse.y"
    { pform_startmodule((yyvsp[(3) - (3)].text), (yylsp[(2) - (3)]).text, (yylsp[(2) - (3)]).first_line, (yyvsp[(1) - (3)].named_pexprs)); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1521 "parse.y"
    { pform_module_set_ports((yyvsp[(6) - (7)].mports)); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1524 "parse.y"
    { pform_endmodule((yyvsp[(3) - (10)].text));
		  delete (yyvsp[(3) - (10)].text);
		;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1533 "parse.y"
    { (yyval.range_delay).range = (yyvsp[(1) - (2)].exprs); (yyval.range_delay).delay = (yyvsp[(2) - (2)].exprs); ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1537 "parse.y"
    { (yyval.mports) = (yyvsp[(2) - (3)].mports); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1538 "parse.y"
    { (yyval.mports) = (yyvsp[(2) - (3)].mports); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1539 "parse.y"
    { (yyval.mports) = 0; ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1558 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), (yyvsp[(4) - (6)].range_delay).range, (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype),
				 NetNet::NOT_A_PORT, (yyvsp[(1) - (6)].named_pexprs));
		  if ((yyvsp[(4) - (6)].range_delay).delay != 0) {
			yyerror((yylsp[(4) - (6)]), "sorry: net delays not supported.");
			delete (yyvsp[(4) - (6)].range_delay).delay;
		  }
		  if ((yyvsp[(1) - (6)].named_pexprs)) delete (yyvsp[(1) - (6)].named_pexprs);
		;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1567 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), (yyvsp[(4) - (6)].range_delay).range, (yyvsp[(3) - (6)].flag), (yyvsp[(4) - (6)].range_delay).delay,
				 str_strength, (yyvsp[(5) - (6)].net_decl_assign), (yyvsp[(2) - (6)].nettype));
		  if ((yyvsp[(1) - (6)].named_pexprs)) {
			yyerror((yylsp[(3) - (6)]), "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete (yyvsp[(1) - (6)].named_pexprs);
		  }
		;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1576 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), 0, (yyvsp[(3) - (6)].flag), 0, (yyvsp[(4) - (6)].drive), (yyvsp[(5) - (6)].net_decl_assign), (yyvsp[(2) - (6)].nettype));
		  if ((yyvsp[(1) - (6)].named_pexprs)) {
			yyerror((yylsp[(4) - (6)]), "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete (yyvsp[(1) - (6)].named_pexprs);
		  }
		;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1584 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "sorry: trireg nets not supported.");
		  delete (yyvsp[(3) - (5)].range_delay).range;
		  delete (yyvsp[(3) - (5)].range_delay).delay;
		;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1590 "parse.y"
    { pform_set_port_type((yylsp[(1) - (5)]), (yyvsp[(4) - (5)].perm_strings), (yyvsp[(3) - (5)].range_delay).range, (yyvsp[(2) - (5)].flag), (yyvsp[(1) - (5)].porttype));
		;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1598 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), (yyvsp[(1) - (6)].porttype), 0);
		;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1602 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::POUTPUT, 0);
		;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1610 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::PINPUT, 0);
		  yyerror((yylsp[(2) - (6)]), "error: reg variables cannot be inputs.");
		;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1615 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::PINOUT, 0);
		  yyerror((yylsp[(2) - (6)]), "error: reg variables cannot be inouts.");
		;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1620 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: Invalid variable list"
			  " in port declaration.");
		  if ((yyvsp[(3) - (5)].range_delay).range) delete (yyvsp[(3) - (5)].range_delay).range;
		  if ((yyvsp[(3) - (5)].range_delay).delay) delete (yyvsp[(3) - (5)].range_delay).delay;
		  yyerrok;
		;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1631 "parse.y"
    { ; ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1637 "parse.y"
    { pform_make_events((yyvsp[(2) - (3)].perm_strings), (yylsp[(1) - (3)]).text, (yylsp[(1) - (3)]).first_line);
		;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1644 "parse.y"
    { pform_makegates((yyvsp[(2) - (4)].gatetype), str_strength, 0, (yyvsp[(3) - (4)].gates), (yyvsp[(1) - (4)].named_pexprs));
		;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1648 "parse.y"
    { pform_makegates((yyvsp[(2) - (5)].gatetype), str_strength, (yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].gates), (yyvsp[(1) - (5)].named_pexprs));
		;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1652 "parse.y"
    { pform_makegates((yyvsp[(2) - (5)].gatetype), (yyvsp[(3) - (5)].drive), 0, (yyvsp[(4) - (5)].gates), (yyvsp[(1) - (5)].named_pexprs));
		;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1656 "parse.y"
    { pform_makegates((yyvsp[(2) - (6)].gatetype), (yyvsp[(3) - (6)].drive), (yyvsp[(4) - (6)].exprs), (yyvsp[(5) - (6)].gates), (yyvsp[(1) - (6)].named_pexprs));
		;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1663 "parse.y"
    { pform_makegates(PGBuiltin::PULLUP, pull_strength, 0,
				  (yyvsp[(2) - (3)].gates), 0);
		;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1667 "parse.y"
    { pform_makegates(PGBuiltin::PULLDOWN, pull_strength,
				  0, (yyvsp[(2) - (3)].gates), 0);
		;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1672 "parse.y"
    { pform_makegates(PGBuiltin::PULLUP, (yyvsp[(3) - (6)].drive), 0, (yyvsp[(5) - (6)].gates), 0);
		;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1676 "parse.y"
    { pform_makegates(PGBuiltin::PULLDOWN, (yyvsp[(3) - (6)].drive), 0, (yyvsp[(5) - (6)].gates), 0);
		;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1684 "parse.y"
    { perm_string tmp1 = lex_strings.make((yyvsp[(1) - (4)].text));
		  pform_make_modgates(tmp1, (yyvsp[(2) - (4)].parmvalue), (yyvsp[(3) - (4)].gates));
		  delete (yyvsp[(1) - (4)].text);
		;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1690 "parse.y"
    { yyerror((yylsp[(1) - (4)]), "error: Invalid module instantiation");
		;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1698 "parse.y"
    { pform_make_pgassign_list((yyvsp[(4) - (5)].exprs), (yyvsp[(3) - (5)].exprs), (yyvsp[(2) - (5)].drive), (yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1701 "parse.y"
    { pform_make_pmemassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1704 "parse.y"
    { pform_make_psquareassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1707 "parse.y"
    { pform_make_pmultassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1710 "parse.y"
    { pform_make_pmultaddassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1713 "parse.y"
    { pform_make_pmultaccumassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1718 "parse.y"
    { PProcess*tmp = pform_make_behavior(PProcess::PR_ALWAYS,
						     (yyvsp[(3) - (3)].statement), (yyvsp[(1) - (3)].named_pexprs));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1724 "parse.y"
    { PProcess*tmp = pform_make_behavior(PProcess::PR_INITIAL,
						     (yyvsp[(3) - (3)].statement), (yyvsp[(1) - (3)].named_pexprs));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1736 "parse.y"
    { pform_push_scope((yyvsp[(2) - (3)].text)); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1739 "parse.y"
    { PTask*tmp = new PTask;
		  perm_string tmp2 = lex_strings.make((yyvsp[(2) - (7)].text));
		  tmp->set_file((yylsp[(1) - (7)]).text);
		  tmp->set_lineno((yylsp[(1) - (7)]).first_line);
		  tmp->set_ports((yyvsp[(5) - (7)].wires));
		  tmp->set_statement((yyvsp[(6) - (7)].statement));
		  pform_set_task(tmp2, tmp);
		  pform_pop_scope();
		  delete (yyvsp[(2) - (7)].text);
		;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1756 "parse.y"
    { pform_push_scope((yyvsp[(3) - (4)].text)); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1759 "parse.y"
    { perm_string name = lex_strings.make((yyvsp[(3) - (8)].text));
		  PFunction *tmp = new PFunction(name);
		  tmp->set_file((yylsp[(1) - (8)]).text);
		  tmp->set_lineno((yylsp[(1) - (8)]).first_line);
		  tmp->set_ports((yyvsp[(6) - (8)].wires));
		  tmp->set_statement((yyvsp[(7) - (8)].statement));
		  tmp->set_return((yyvsp[(2) - (8)].function_type));
		  pform_set_function(name, tmp);
		  pform_pop_scope();
		  delete (yyvsp[(3) - (8)].text);
		;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1774 "parse.y"
    { /* empty lists are legal syntax. */ ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1777 "parse.y"
    {
		;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1781 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in specify block");
		  yyerrok;
		;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1790 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: invalid module item. "
			  "Did you forget an initial or always?");
		  yyerrok;
		;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1796 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: syntax error in left side "
			  "of continuous assignment.");
		  yyerrok;
		;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1802 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in "
			  "continuous assignment");
		  yyerrok;
		;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1808 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: I give up on this "
			  "function definition.");
		  yyerrok;
		;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1817 "parse.y"
    { perm_string tmp3 = lex_strings.make((yyvsp[(3) - (9)].text));
		  perm_string tmp5 = lex_strings.make((yyvsp[(5) - (9)].text));
		  pform_set_attrib(tmp3, tmp5, (yyvsp[(7) - (9)].text));
		  delete (yyvsp[(3) - (9)].text);
		  delete (yyvsp[(5) - (9)].text);
		;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1824 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: Malformed $attribute parameter list."); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1848 "parse.y"
    { net_decl_assign_t*tmp = new net_decl_assign_t;
		  tmp->next = tmp;
		  tmp->name = (yyvsp[(1) - (3)].text);
		  tmp->expr = (yyvsp[(3) - (3)].expr);
		  (yyval.net_decl_assign) = tmp;
		;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1858 "parse.y"
    { net_decl_assign_t*tmp = (yyvsp[(1) - (3)].net_decl_assign);
		  (yyvsp[(3) - (3)].net_decl_assign)->next = tmp->next;
		  tmp->next = (yyvsp[(3) - (3)].net_decl_assign);
		  (yyval.net_decl_assign) = tmp;
		;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1864 "parse.y"
    { (yyval.net_decl_assign) = (yyvsp[(1) - (1)].net_decl_assign);
		;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1869 "parse.y"
    { (yyval.nettype) = NetNet::WIRE; ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1870 "parse.y"
    { (yyval.nettype) = NetNet::TRI; ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1871 "parse.y"
    { (yyval.nettype) = NetNet::TRI1; ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1872 "parse.y"
    { (yyval.nettype) = NetNet::SUPPLY0; ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1873 "parse.y"
    { (yyval.nettype) = NetNet::WAND; ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1874 "parse.y"
    { (yyval.nettype) = NetNet::TRIAND; ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1875 "parse.y"
    { (yyval.nettype) = NetNet::TRI0; ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1876 "parse.y"
    { (yyval.nettype) = NetNet::SUPPLY1; ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1877 "parse.y"
    { (yyval.nettype) = NetNet::WOR; ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1878 "parse.y"
    { (yyval.nettype) = NetNet::TRIOR; ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1882 "parse.y"
    { (yyval.nettype) = NetNet::REG; ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1887 "parse.y"
    { PExpr*tmp = (yyvsp[(3) - (3)].expr);
		  if (!pform_expression_is_constant(tmp)) {
			yyerror((yylsp[(3) - (3)]), "error: parameter value "
			            "must be a constant expression.");
			delete tmp;
			tmp = 0;
		  } else {
			pform_set_parameter(lex_strings.make((yyvsp[(1) - (3)].text)),
					    active_signed,
					    active_range, tmp);
		  }
		  delete (yyvsp[(1) - (3)].text);
		;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1904 "parse.y"
    { active_range = (yyvsp[(1) - (1)].exprs); active_signed = false; ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1906 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1909 "parse.y"
    { active_range = (yyvsp[(2) - (2)].exprs); active_signed = true; ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1911 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1929 "parse.y"
    { PExpr*tmp = (yyvsp[(3) - (3)].expr);
		  if (!pform_expression_is_constant(tmp)) {
			yyerror((yylsp[(3) - (3)]), "error: parameter value "
			            "must be constant.");
			delete tmp;
			tmp = 0;
		  } else {
			pform_set_localparam(lex_strings.make((yyvsp[(1) - (3)].text)),
					     active_signed,
					     active_range, tmp);
		  }
		  delete (yyvsp[(1) - (3)].text);
		;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1946 "parse.y"
    { active_range = (yyvsp[(1) - (1)].exprs); active_signed = false; ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1948 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1951 "parse.y"
    { active_range = (yyvsp[(2) - (2)].exprs); active_signed = true; ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1953 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 1981 "parse.y"
    { struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = (yyvsp[(3) - (4)].exprs);
		  tmp->by_name = 0;
		  (yyval.parmvalue) = tmp;
		;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 1987 "parse.y"
    { struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = 0;
		  tmp->by_name = (yyvsp[(3) - (4)].named_pexprs);
		  (yyval.parmvalue) = tmp;
		;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 1993 "parse.y"
    { assert((yyvsp[(2) - (2)].number));
		  PENumber*tmp = new PENumber((yyvsp[(2) - (2)].number));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);

		  struct parmvalue_t*lst = new struct parmvalue_t;
		  lst->by_order = new svector<PExpr*>(1);
		  (*lst->by_order)[0] = tmp;
		  lst->by_name = 0;
		  (yyval.parmvalue) = lst;
		;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2005 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: syntax error in parameter value "
			  "assignment list.");
		  (yyval.parmvalue) = 0;
		;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2010 "parse.y"
    { (yyval.parmvalue) = 0; ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2015 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = (yyvsp[(4) - (5)].expr);
		  free((yyvsp[(2) - (5)].text));
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2022 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (4)].text));
		  tmp->parm = 0;
		  free((yyvsp[(2) - (4)].text));
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2032 "parse.y"
    { svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].named_pexpr);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2037 "parse.y"
    { svector<named_pexpr_t*>*tmp =
			new svector<named_pexpr_t*>(*(yyvsp[(1) - (3)].named_pexprs),(yyvsp[(3) - (3)].named_pexpr));
		  delete (yyvsp[(1) - (3)].named_pexprs);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2060 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2068 "parse.y"
    { Module::port_t*tmp = (yyvsp[(4) - (5)].mport);
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.mport) = tmp;
		;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2079 "parse.y"
    { Module::port_t*tmp = (yyvsp[(2) - (3)].mport);
		  tmp->name = perm_string();
		  (yyval.mport) = tmp;
		;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2088 "parse.y"
    { Module::port_t*tmp = (yyvsp[(5) - (7)].mport);
		  tmp->name = lex_strings.make((yyvsp[(2) - (7)].text));
		  delete (yyvsp[(2) - (7)].text);
		  (yyval.mport) = tmp;
		;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2096 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2097 "parse.y"
    { (yyval.mport) = 0; ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2114 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(1) - (1)].text), (yylsp[(1) - (1)]).text,
						     (yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2122 "parse.y"
    { PEIdent*wtmp = new PEIdent(hname_t((yyvsp[(1) - (6)].text)));
		  wtmp->set_file((yylsp[(1) - (6)]).text);
		  wtmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  if (!pform_expression_is_constant((yyvsp[(3) - (6)].expr))) {
			yyerror((yylsp[(3) - (6)]), "error: msb expression of "
				"port part select must be constant.");
		  }
		  if (!pform_expression_is_constant((yyvsp[(5) - (6)].expr))) {
			yyerror((yylsp[(5) - (6)]), "error: lsb expression of "
				"port part select must be constant.");
		  }
		  wtmp->msb_ = (yyvsp[(3) - (6)].expr);
		  wtmp->lsb_ = (yyvsp[(5) - (6)].expr);
		  Module::port_t*ptmp = new Module::port_t;
		  ptmp->name = perm_string();
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = wtmp;
		  delete (yyvsp[(1) - (6)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2144 "parse.y"
    { PEIdent*tmp = new PEIdent(hname_t((yyvsp[(1) - (4)].text)));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  if (!pform_expression_is_constant((yyvsp[(3) - (4)].expr))) {
			yyerror((yylsp[(3) - (4)]), "error: port bit select "
				"must be constant.");
		  }
		  tmp->msb_ = (yyvsp[(3) - (4)].expr);
		  Module::port_t*ptmp = new Module::port_t;
		  ptmp->name = perm_string();
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = tmp;
		  delete (yyvsp[(1) - (4)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2161 "parse.y"
    { yyerror((yylsp[(1) - (4)]), "error: invalid port bit select");
		  Module::port_t*ptmp = new Module::port_t;
		  PEIdent*wtmp = new PEIdent(hname_t((yyvsp[(1) - (4)].text)));
		  wtmp->set_file((yylsp[(1) - (4)]).text);
		  wtmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  ptmp->name = lex_strings.make((yyvsp[(1) - (4)].text));
		  ptmp->expr = svector<PEIdent*>(1);
		  ptmp->expr[0] = wtmp;
		  delete (yyvsp[(1) - (4)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2177 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2179 "parse.y"
    { Module::port_t*tmp = (yyvsp[(1) - (3)].mport);
		  tmp->expr = svector<PEIdent*>(tmp->expr, (yyvsp[(3) - (3)].mport)->expr);
		  delete (yyvsp[(3) - (3)].mport);
		  (yyval.mport) = tmp;
		;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2192 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = (yyvsp[(4) - (5)].expr);
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2199 "parse.y"
    { yyerror((yylsp[(4) - (5)]), "error: invalid port connection expression.");
		  named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = 0;
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2207 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (4)].text));
		  tmp->parm = 0;
		  delete (yyvsp[(2) - (4)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2217 "parse.y"
    { svector<named_pexpr_t*>*tmp;
		  tmp = new svector<named_pexpr_t*>(*(yyvsp[(1) - (3)].named_pexprs), (yyvsp[(3) - (3)].named_pexpr));
		  delete (yyvsp[(1) - (3)].named_pexprs);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2223 "parse.y"
    { svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].named_pexpr);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2230 "parse.y"
    { (yyval.porttype) = NetNet::PINPUT; ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2231 "parse.y"
    { (yyval.porttype) = NetNet::POUTPUT; ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2232 "parse.y"
    { (yyval.porttype) = NetNet::PINOUT; ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2237 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*> (2);
		  if (!pform_expression_is_constant((yyvsp[(2) - (5)].expr)))
			yyerror((yylsp[(2) - (5)]), "error: msb of range must be constant.");

		  (*tmp)[0] = (yyvsp[(2) - (5)].expr);

		  if (!pform_expression_is_constant((yyvsp[(4) - (5)].expr)))
			yyerror((yylsp[(4) - (5)]), "error: lsb of range must be constant.");

		  (*tmp)[1] = (yyvsp[(4) - (5)].expr);

		  (yyval.exprs) = tmp;
		;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2254 "parse.y"
    { (yyval.exprs) = 0; ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2259 "parse.y"
    { (yyval.function_type).range = (yyvsp[(1) - (1)].exprs); (yyval.function_type).type = PTF_REG; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2260 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_INTEGER; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2261 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REAL; ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2262 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REALTIME; ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2263 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_TIME; ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2264 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REG; ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2274 "parse.y"
    { pform_makewire((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].text), NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  (yyval.text) = (yyvsp[(1) - (1)].text);
		;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2279 "parse.y"
    { pform_makewire((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].text), NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  if (! pform_expression_is_constant((yyvsp[(3) - (3)].expr)))
			yyerror((yylsp[(3) - (3)]), "error: register declaration assignment"
				" value must be a constant expression.");
		  pform_make_reginit((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].expr));
		  (yyval.text) = (yyvsp[(1) - (3)].text);
		;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2288 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(1) - (6)].text), NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  if (! pform_expression_is_constant((yyvsp[(3) - (6)].expr)))
			yyerror((yylsp[(3) - (6)]), "error: msb of register range must be constant.");
		  if (! pform_expression_is_constant((yyvsp[(5) - (6)].expr)))
			yyerror((yylsp[(3) - (6)]), "error: lsb of register range must be constant.");
		  pform_set_reg_idx((yyvsp[(1) - (6)].text), (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].expr));
		  (yyval.text) = (yyvsp[(1) - (6)].text);
		;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2301 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2307 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2317 "parse.y"
    {
		;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2320 "parse.y"
    {
		;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2323 "parse.y"
    {
		;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2326 "parse.y"
    {
		;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2330 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2334 "parse.y"
    { delete (yyvsp[(5) - (8)].expr);
		;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2338 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2342 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2346 "parse.y"
    { delete (yyvsp[(7) - (12)].expr);
		  delete (yyvsp[(9) - (12)].expr);
		;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2351 "parse.y"
    { delete (yyvsp[(7) - (12)].expr);
		  delete (yyvsp[(9) - (12)].expr);
		;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2356 "parse.y"
    { delete (yyvsp[(5) - (10)].expr);
		  delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2360 "parse.y"
    { delete (yyvsp[(5) - (7)].expr);
		;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2365 "parse.y"
    { ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2366 "parse.y"
    { ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2400 "parse.y"
    { yyerror((yylsp[(2) - (5)]), "Syntax error in delay value list.");
		  yyerrok;
		;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2408 "parse.y"
    { pform_make_specify_path((yyvsp[(2) - (6)].perm_strings), (yyvsp[(3) - (6)].letter), false, (yyvsp[(5) - (6)].perm_strings)); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2411 "parse.y"
    { pform_make_specify_path((yyvsp[(2) - (6)].perm_strings), (yyvsp[(3) - (6)].letter), true, (yyvsp[(5) - (6)].perm_strings)); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2413 "parse.y"
    { yyerror((yylsp[(2) - (3)]), "Invalid simple path");
		  yyerrok;
		;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2420 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2426 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (4)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (4)].text);
		;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2432 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2438 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (6)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (6)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (6)].text);
		;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2447 "parse.y"
    { PExpr*tmp = (yyvsp[(3) - (3)].expr);
		  if (!pform_expression_is_constant(tmp)) {
			yyerror((yylsp[(3) - (3)]), "error: specparam value "
			            "must be a constant expression.");
			delete tmp;
			tmp = 0;
		  } else {
			pform_set_specparam(lex_strings.make((yyvsp[(1) - (3)].text)), tmp);
		  }
		  delete (yyvsp[(1) - (3)].text);
		;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2459 "parse.y"
    { delete (yyvsp[(1) - (7)].text);
		  delete (yyvsp[(3) - (7)].expr);
		  delete (yyvsp[(5) - (7)].expr);
		  delete (yyvsp[(7) - (7)].expr);
		;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2465 "parse.y"
    { delete (yyvsp[(1) - (3)].text);
		  delete (yyvsp[(3) - (3)].expr);
		;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2469 "parse.y"
    { delete (yyvsp[(1) - (7)].text);
		  delete (yyvsp[(4) - (7)].expr);
		  delete (yyvsp[(6) - (7)].expr);
		;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2481 "parse.y"
    { (yyval.letter) = '+'; ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2482 "parse.y"
    { (yyval.letter) = '-'; ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2483 "parse.y"
    { (yyval.letter) = 0;   ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2488 "parse.y"
    { delete (yyvsp[(2) - (2)].expr); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2490 "parse.y"
    { delete (yyvsp[(2) - (2)].expr); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2492 "parse.y"
    { delete (yyvsp[(2) - (4)].expr);
		  delete (yyvsp[(4) - (4)].expr);
		;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2496 "parse.y"
    { delete (yyvsp[(2) - (4)].expr);
		  delete (yyvsp[(4) - (4)].expr);
		;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2500 "parse.y"
    { delete (yyvsp[(1) - (3)].expr);
		  delete (yyvsp[(3) - (3)].expr);
		;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2504 "parse.y"
    { delete (yyvsp[(1) - (1)].expr); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2509 "parse.y"
    {  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2511 "parse.y"
    {  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2515 "parse.y"
    {  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2517 "parse.y"
    { delete (yyvsp[(2) - (2)].hier); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2519 "parse.y"
    {  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2521 "parse.y"
    { delete (yyvsp[(3) - (3)].hier); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2523 "parse.y"
    { delete (yyvsp[(3) - (6)].hier);
		  delete (yyvsp[(5) - (6)].expr);
		;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2527 "parse.y"
    { delete (yyvsp[(1) - (1)].text); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2539 "parse.y"
    { PCAssign*tmp = new PCAssign((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2546 "parse.y"
    { PDeassign*tmp = new PDeassign((yyvsp[(2) - (3)].expr));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2557 "parse.y"
    { PForce*tmp = new PForce((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2563 "parse.y"
    { PRelease*tmp = new PRelease((yyvsp[(2) - (3)].expr));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2576 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ, *(yyvsp[(2) - (3)].statement_list));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2583 "parse.y"
    { pform_push_scope((yyvsp[(3) - (3)].text)); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2586 "parse.y"
    { pform_pop_scope();
		  PBlock*tmp = new PBlock(lex_strings.make((yyvsp[(3) - (7)].text)),
					  PBlock::BL_SEQ, *(yyvsp[(6) - (7)].statement_list));
		  tmp->set_file((yylsp[(1) - (7)]).text);
		  tmp->set_lineno((yylsp[(1) - (7)]).first_line);
		  delete (yyvsp[(3) - (7)].text);
		  delete (yyvsp[(6) - (7)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2596 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2602 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2616 "parse.y"
    { pform_push_scope((yyvsp[(3) - (3)].text)); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2619 "parse.y"
    { pform_pop_scope();
		  PBlock*tmp = new PBlock(lex_strings.make((yyvsp[(3) - (7)].text)),
					  PBlock::BL_PAR, *(yyvsp[(6) - (7)].statement_list));
		  tmp->set_file((yylsp[(1) - (7)]).text);
		  tmp->set_lineno((yylsp[(1) - (7)]).first_line);
		  delete (yyvsp[(3) - (7)].text);
		  delete (yyvsp[(6) - (7)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2629 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2635 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(3) - (4)].text);
		  (yyval.statement) = tmp;
		;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2643 "parse.y"
    { PDisable*tmp = new PDisable(*(yyvsp[(2) - (3)].hier));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2650 "parse.y"
    { PTrigger*tmp = new PTrigger(*(yyvsp[(2) - (3)].hier));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2657 "parse.y"
    { PForever*tmp = new PForever((yyvsp[(2) - (2)].statement));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2663 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR, *(yyvsp[(2) - (3)].statement_list));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2670 "parse.y"
    { PRepeat*tmp = new PRepeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2676 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQ, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2682 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQX, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2688 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQZ, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2694 "parse.y"
    { yyerrok; ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2696 "parse.y"
    { yyerrok; ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2698 "parse.y"
    { yyerrok; ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2700 "parse.y"
    { PCondit*tmp = new PCondit((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement), 0);
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2706 "parse.y"
    { PCondit*tmp = new PCondit((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].statement), (yyvsp[(7) - (7)].statement));
		  tmp->set_file((yylsp[(1) - (7)]).text);
		  tmp->set_lineno((yylsp[(1) - (7)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2712 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: Malformed conditional expression.");
		  (yyval.statement) = (yyvsp[(5) - (5)].statement);
		;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2716 "parse.y"
    { yyerror((yylsp[(1) - (7)]), "error: Malformed conditional expression.");
		  (yyval.statement) = (yyvsp[(5) - (7)].statement);
		;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2721 "parse.y"
    { PForStatement*tmp = new PForStatement((yyvsp[(3) - (13)].expr), (yyvsp[(5) - (13)].expr), (yyvsp[(7) - (13)].expr), (yyvsp[(9) - (13)].expr), (yyvsp[(11) - (13)].expr), (yyvsp[(13) - (13)].statement));
		  tmp->set_file((yylsp[(1) - (13)]).text);
		  tmp->set_lineno((yylsp[(1) - (13)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2728 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(9) - (11)]), "error: Error in for loop step assignment.");
		;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2733 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(7) - (13)]), "error: Error in for loop condition expression.");
		;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2737 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(3) - (5)]), "error: Incomprehensible for loop.");
		;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2741 "parse.y"
    { PWhile*tmp = new PWhile((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement));
		  (yyval.statement) = tmp;
		;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2745 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(3) - (5)]), "error: Error in while loop condition.");
		;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2749 "parse.y"
    { PExpr*del = (*(yyvsp[(1) - (2)].exprs))[0];
		  assert((yyvsp[(1) - (2)].exprs)->count() == 1);
		  PDelayStatement*tmp = new PDelayStatement(del, (yyvsp[(2) - (2)].statement));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2757 "parse.y"
    { PEventStatement*tmp = (yyvsp[(1) - (3)].event_statement);
		  if (tmp == 0) {
			yyerror((yylsp[(1) - (3)]), "error: Invalid event control.");
			(yyval.statement) = 0;
		  } else {
			pform_attach_attributes((yyvsp[(3) - (3)].statement), (yyvsp[(2) - (3)].named_pexprs));
			tmp->set_statement((yyvsp[(3) - (3)].statement));
			(yyval.statement) = tmp;
		  }
		  if ((yyvsp[(2) - (3)].named_pexprs)) delete (yyvsp[(2) - (3)].named_pexprs);
		;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2769 "parse.y"
    { PEventStatement*tmp = new PEventStatement;
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  pform_attach_attributes((yyvsp[(4) - (4)].statement), (yyvsp[(3) - (4)].named_pexprs));
		  tmp->set_statement((yyvsp[(4) - (4)].statement));
		  if ((yyvsp[(3) - (4)].named_pexprs)) delete (yyvsp[(3) - (4)].named_pexprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2778 "parse.y"
    { PEventStatement*tmp = new PEventStatement;
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  pform_attach_attributes((yyvsp[(6) - (6)].statement), (yyvsp[(5) - (6)].named_pexprs));
		  tmp->set_statement((yyvsp[(6) - (6)].statement));
		  if ((yyvsp[(5) - (6)].named_pexprs)) delete (yyvsp[(5) - (6)].named_pexprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2787 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2793 "parse.y"
    { PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2799 "parse.y"
    { assert((yyvsp[(3) - (5)].exprs)->count() == 1);
		  PExpr*del = (*(yyvsp[(3) - (5)].exprs))[0];
		  PAssign*tmp = new PAssign((yyvsp[(1) - (5)].expr),del,(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2807 "parse.y"
    { assert((yyvsp[(3) - (5)].exprs)->count() == 1);
		  PExpr*del = (*(yyvsp[(3) - (5)].exprs))[0];
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (5)].expr),del,(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2815 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (5)].expr),(yyvsp[(3) - (5)].event_statement),(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2821 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (9)].expr),(yyvsp[(7) - (9)].event_statement),(yyvsp[(8) - (9)].expr));
		  tmp->set_file((yylsp[(1) - (9)]).text);
		  tmp->set_lineno((yylsp[(1) - (9)]).first_line);
		  yyerror((yylsp[(3) - (9)]), "sorry: repeat event control not supported.");
		  delete (yyvsp[(5) - (9)].expr);
		  (yyval.statement) = tmp;
		;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2829 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "sorry: Event controls not supported here.");
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (5)].expr),(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2836 "parse.y"
    { yyerror((yylsp[(1) - (9)]), "sorry: Event controls not supported here.");
		  delete (yyvsp[(5) - (9)].expr);
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (9)].expr),(yyvsp[(8) - (9)].expr));
		  tmp->set_file((yylsp[(1) - (9)]).text);
		  tmp->set_lineno((yylsp[(1) - (9)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2844 "parse.y"
    { PEventStatement*tmp;
		  PEEvent*etmp = new PEEvent(PEEvent::POSITIVE, (yyvsp[(3) - (5)].expr));
		  tmp = new PEventStatement(etmp);
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  tmp->set_statement((yyvsp[(5) - (5)].statement));
		  (yyval.statement) = tmp;
		;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2853 "parse.y"
    { PCallTask*tmp = new PCallTask(hname_t((yyvsp[(1) - (5)].text)), *(yyvsp[(3) - (5)].exprs));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  delete (yyvsp[(1) - (5)].text);
		  delete (yyvsp[(3) - (5)].exprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2861 "parse.y"
    { svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(hname_t((yyvsp[(1) - (2)].text)), pt);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  delete (yyvsp[(1) - (2)].text);
		  (yyval.statement) = tmp;
		;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2869 "parse.y"
    { PCallTask*tmp = new PCallTask(*(yyvsp[(1) - (5)].hier), *(yyvsp[(3) - (5)].exprs));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  delete (yyvsp[(1) - (5)].hier);
		  delete (yyvsp[(3) - (5)].exprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2877 "parse.y"
    { svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(*(yyvsp[(1) - (2)].hier), pt);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  delete (yyvsp[(1) - (2)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2885 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: malformed statement");
		  yyerrok;
		  (yyval.statement) = new PNoop;
		;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2893 "parse.y"
    { pform_attach_attributes((yyvsp[(3) - (3)].statement), (yyvsp[(2) - (3)].named_pexprs));
		  svector<Statement*>*tmp = new svector<Statement*>(*(yyvsp[(1) - (3)].statement_list), (yyvsp[(3) - (3)].statement));
		  delete (yyvsp[(1) - (3)].statement_list);
		  if ((yyvsp[(2) - (3)].named_pexprs)) delete (yyvsp[(2) - (3)].named_pexprs);
		  (yyval.statement_list) = tmp;
		;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2900 "parse.y"
    { pform_attach_attributes((yyvsp[(2) - (2)].statement), (yyvsp[(1) - (2)].named_pexprs));
		  svector<Statement*>*tmp = new svector<Statement*>(1);
		  (*tmp)[0] = (yyvsp[(2) - (2)].statement);
		  if ((yyvsp[(1) - (2)].named_pexprs)) delete (yyvsp[(1) - (2)].named_pexprs);
		  (yyval.statement_list) = tmp;
		;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2910 "parse.y"
    { (yyval.statement) = 0; ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2916 "parse.y"
    { (yyval.wires) = new svector<PWire*>(0); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2918 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2925 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2932 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::POUTPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2939 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::POUTPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2946 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINOUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 2953 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINOUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 2963 "parse.y"
    { svector<PWire*>*tmp = new svector<PWire*>(*(yyvsp[(1) - (2)].wires), *(yyvsp[(2) - (2)].wires));
		  delete (yyvsp[(1) - (2)].wires);
		  delete (yyvsp[(2) - (2)].wires);
		  (yyval.wires) = tmp;
		;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 2969 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 2974 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 2976 "parse.y"
    { (yyval.wires) = 0; ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 2980 "parse.y"
    { lex_start_table(); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 2982 "parse.y"
    { lex_end_table(); (yyval.strings) = (yyvsp[(3) - (4)].strings); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 2992 "parse.y"
    { char*tmp = new char[strlen((yyvsp[(1) - (4)].text))+3];
		  strcpy(tmp, (yyvsp[(1) - (4)].text));
		  char*tp = tmp+strlen(tmp);
		  *tp++ = ':';
		  *tp++ = (yyvsp[(3) - (4)].letter);
		  *tp++ = 0;
		  delete[](yyvsp[(1) - (4)].text);
		  (yyval.text) = tmp;
		;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3005 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3011 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (2)].strings);
		  tmp->push_back((yyvsp[(2) - (2)].text));
		  delete (yyvsp[(2) - (2)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3020 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3026 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (2)].strings);
		  tmp->push_back((yyvsp[(2) - (2)].text));
		  delete (yyvsp[(2) - (2)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3035 "parse.y"
    { char*tmp = new char[strlen((yyvsp[(1) - (6)].text))+5];
		  strcpy(tmp, (yyvsp[(1) - (6)].text));
		  char*tp = tmp+strlen(tmp);
		  *tp++ = ':';
		  *tp++ = (yyvsp[(3) - (6)].letter);
		  *tp++ = ':';
		  *tp++ = (yyvsp[(5) - (6)].letter);
		  *tp++ = 0;
		  (yyval.text) = tmp;
		;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3049 "parse.y"
    { PExpr*etmp = new PENumber((yyvsp[(4) - (5)].number));
		  PEIdent*itmp = new PEIdent(hname_t((yyvsp[(2) - (5)].text)));
		  PAssign*atmp = new PAssign(itmp, etmp);
		  atmp->set_file((yylsp[(2) - (5)]).text);
		  atmp->set_lineno((yylsp[(2) - (5)]).first_line);
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.statement) = atmp;
		;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3060 "parse.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3061 "parse.y"
    { (yyval.statement) = 0; ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3066 "parse.y"
    { char*tmp = new char[2];
		  tmp[0] = (yyvsp[(1) - (1)].letter);
		  tmp[1] = 0;
		  (yyval.text) = tmp;
		;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3072 "parse.y"
    { char*tmp = new char[strlen((yyvsp[(1) - (2)].text))+2];
		  strcpy(tmp, (yyvsp[(1) - (2)].text));
		  char*tp = tmp+strlen(tmp);
		  *tp++ = (yyvsp[(2) - (2)].letter);
		  *tp++ = 0;
		  delete[](yyvsp[(1) - (2)].text);
		  (yyval.text) = tmp;
		;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3083 "parse.y"
    { (yyval.letter) = '0'; ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3084 "parse.y"
    { (yyval.letter) = '1'; ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3085 "parse.y"
    { (yyval.letter) = 'x'; ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3086 "parse.y"
    { (yyval.letter) = '?'; ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3087 "parse.y"
    { (yyval.letter) = 'b'; ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3088 "parse.y"
    { (yyval.letter) = '*'; ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3089 "parse.y"
    { (yyval.letter) = '%'; ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3090 "parse.y"
    { (yyval.letter) = 'f'; ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3091 "parse.y"
    { (yyval.letter) = 'F'; ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3092 "parse.y"
    { (yyval.letter) = 'l'; ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3093 "parse.y"
    { (yyval.letter) = 'H'; ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3094 "parse.y"
    { (yyval.letter) = 'B'; ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3095 "parse.y"
    { (yyval.letter) = 'r'; ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3096 "parse.y"
    { (yyval.letter) = 'R'; ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3097 "parse.y"
    { (yyval.letter) = 'M'; ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3098 "parse.y"
    { (yyval.letter) = 'n'; ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3099 "parse.y"
    { (yyval.letter) = 'N'; ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3100 "parse.y"
    { (yyval.letter) = 'p'; ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3101 "parse.y"
    { (yyval.letter) = 'P'; ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3102 "parse.y"
    { (yyval.letter) = 'Q'; ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3103 "parse.y"
    { (yyval.letter) = 'q'; ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3104 "parse.y"
    { (yyval.letter) = '_'; ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3105 "parse.y"
    { (yyval.letter) = '+'; ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3109 "parse.y"
    { (yyval.letter) = '0'; ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3110 "parse.y"
    { (yyval.letter) = '1'; ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3111 "parse.y"
    { (yyval.letter) = 'x'; ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3112 "parse.y"
    { (yyval.letter) = '-'; ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3117 "parse.y"
    { (yyval.wires) = pform_make_udp_input_ports((yyvsp[(2) - (3)].perm_strings)); ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3119 "parse.y"
    { PWire*pp = new PWire((yyvsp[(2) - (3)].text), NetNet::IMPLICIT, NetNet::POUTPUT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3125 "parse.y"
    { PWire*pp = new PWire((yyvsp[(2) - (3)].text), NetNet::REG, NetNet::PIMPLICIT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3131 "parse.y"
    { PWire*pp = new PWire((yyvsp[(3) - (4)].text), NetNet::REG, NetNet::POUTPUT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3140 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3142 "parse.y"
    { svector<PWire*>*tmp = new svector<PWire*>(*(yyvsp[(1) - (2)].wires), *(yyvsp[(2) - (2)].wires));
		  delete (yyvsp[(1) - (2)].wires);
		  delete (yyvsp[(2) - (2)].wires);
		  (yyval.wires) = tmp;
		;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3151 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3157 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (3)].strings);
		  tmp->push_back((yyvsp[(3) - (3)].text));
		  delete (yyvsp[(3) - (3)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3164 "parse.y"
    { (yyval.flag) = true; ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3164 "parse.y"
    { (yyval.flag) = false; ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3167 "parse.y"
    { (yyval.expr) = (yyvsp[(2) - (2)].expr); ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3168 "parse.y"
    { (yyval.expr) = 0; ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3173 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(2) - (2)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(2) - (2)].text);
		;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3179 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (4)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(4) - (4)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(4) - (4)].text);
		;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3197 "parse.y"
    { perm_string tmp2 = lex_strings.make((yyvsp[(2) - (10)].text));
		  pform_make_udp(tmp2, (yyvsp[(4) - (10)].strings), (yyvsp[(7) - (10)].wires), (yyvsp[(9) - (10)].strings), (yyvsp[(8) - (10)].statement),
				 (yylsp[(2) - (10)]).text, (yylsp[(2) - (10)]).first_line);
		  delete[](yyvsp[(2) - (10)].text);
		;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3212 "parse.y"
    { perm_string tmp2 = lex_strings.make((yyvsp[(2) - (13)].text));
		  perm_string tmp6 = lex_strings.make((yyvsp[(6) - (13)].text));
		  pform_make_udp(tmp2, (yyvsp[(5) - (13)].flag), tmp6, (yyvsp[(7) - (13)].expr), (yyvsp[(9) - (13)].perm_strings), (yyvsp[(12) - (13)].strings),
				 (yylsp[(2) - (13)]).text, (yylsp[(2) - (13)]).first_line);
		  delete[](yyvsp[(2) - (13)].text);
		  delete[](yyvsp[(6) - (13)].text);
		;}
    break;



/* Line 1455 of yacc.c  */
#line 8395 "parse.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



