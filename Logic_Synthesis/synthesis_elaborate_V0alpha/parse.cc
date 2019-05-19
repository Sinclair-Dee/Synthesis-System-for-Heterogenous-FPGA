
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
#define YYLAST   5135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  196
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  133
/* YYNRULES -- Number of rules.  */
#define YYNRULES  540
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1273

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
     526,   532,   537,   543,   546,   548,   551,   556,   562,   568,
     572,   575,   580,   584,   586,   588,   590,   592,   594,   596,
     598,   600,   602,   604,   606,   608,   610,   612,   614,   616,
     618,   620,   622,   624,   626,   628,   630,   632,   634,   636,
     638,   642,   644,   648,   650,   654,   656,   660,   664,   671,
     678,   685,   692,   701,   703,   704,   706,   707,   709,   714,
     717,   721,   723,   728,   735,   739,   743,   747,   749,   750,
     751,   762,   764,   766,   769,   773,   777,   778,   779,   784,
     787,   791,   796,   803,   810,   817,   823,   829,   836,   843,
     850,   857,   863,   866,   870,   874,   879,   885,   891,   898,
     902,   906,   913,   920,   925,   930,   936,   943,   950,   957,
     964,   971,   975,   979,   980,   988,   989,   998,  1001,  1005,
    1009,  1012,  1018,  1022,  1026,  1036,  1042,  1045,  1047,  1049,
    1050,  1054,  1058,  1060,  1062,  1064,  1066,  1068,  1070,  1072,
    1074,  1076,  1078,  1080,  1082,  1086,  1088,  1089,  1093,  1094,
    1099,  1101,  1105,  1109,  1111,  1112,  1116,  1117,  1122,  1124,
    1128,  1133,  1138,  1141,  1144,  1145,  1151,  1156,  1158,  1162,
    1164,  1170,  1174,  1182,  1184,  1185,  1187,  1194,  1199,  1204,
    1206,  1210,  1216,  1222,  1227,  1231,  1233,  1235,  1237,  1239,
    1245,  1247,  1248,  1250,  1252,  1254,  1256,  1258,  1259,  1261,
    1265,  1272,  1274,  1278,  1282,  1285,  1288,  1295,  1302,  1313,
    1322,  1333,  1344,  1357,  1370,  1381,  1389,  1391,  1395,  1397,
    1400,  1406,  1410,  1418,  1430,  1438,  1450,  1458,  1470,  1478,
    1490,  1492,  1494,  1496,  1502,  1506,  1512,  1519,  1526,  1530,
    1532,  1537,  1541,  1548,  1552,  1560,  1564,  1572,  1574,  1578,
    1580,  1582,  1583,  1586,  1589,  1594,  1599,  1603,  1605,  1606,
    1608,  1610,  1613,  1616,  1620,  1627,  1629,  1635,  1639,  1645,
    1649,  1653,  1654,  1662,  1665,  1670,  1671,  1679,  1682,  1687,
    1691,  1695,  1698,  1702,  1708,  1715,  1722,  1729,  1736,  1743,
    1750,  1756,  1764,  1770,  1778,  1792,  1804,  1818,  1824,  1830,
    1836,  1839,  1843,  1848,  1855,  1860,  1865,  1871,  1877,  1883,
    1893,  1899,  1909,  1915,  1921,  1924,  1930,  1933,  1936,  1940,
    1943,  1945,  1947,  1949,  1954,  1960,  1965,  1971,  1976,  1982,
    1985,  1987,  1989,  1990,  1991,  1996,  1998,  2000,  2005,  2007,
    2010,  2012,  2015,  2022,  2028,  2030,  2031,  2033,  2036,  2038,
    2040,  2042,  2044,  2046,  2048,  2050,  2052,  2054,  2056,  2058,
    2060,  2062,  2064,  2066,  2068,  2070,  2072,  2074,  2076,  2078,
    2080,  2082,  2084,  2086,  2088,  2090,  2094,  2098,  2102,  2107,
    2109,  2112,  2114,  2118,  2120,  2121,  2124,  2125,  2128,  2133,
    2144
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
     167,    -1,     3,   282,   166,   280,   167,    -1,   166,   227,
     167,    -1,     3,   282,    -1,     3,   166,   280,   167,    -1,
     232,   163,   231,    -1,   231,    -1,    32,    -1,    75,    -1,
      82,    -1,    78,    -1,   133,    -1,   132,    -1,    35,    -1,
      36,    -1,    37,    -1,    79,    -1,    80,    -1,    81,    -1,
      77,    -1,    98,    -1,    85,    -1,    99,    -1,    41,    -1,
      92,    -1,   115,    -1,   100,    -1,   116,    -1,   117,    -1,
     101,    -1,   102,    -1,   142,    -1,     3,    -1,   234,   176,
       3,    -1,     3,    -1,   235,   163,     3,    -1,   276,    -1,
     236,   163,   276,    -1,   238,    -1,   237,   163,   238,    -1,
     237,   163,     3,    -1,   200,    67,   239,   240,   283,     3,
      -1,   200,    66,   239,   240,   283,     3,    -1,   200,    83,
     239,   240,   283,     3,    -1,   200,    83,   261,   240,   283,
       3,    -1,   200,    83,   261,   240,   283,     3,   164,   225,
      -1,   260,    -1,    -1,   104,    -1,    -1,   234,    -1,   234,
     172,   225,   173,    -1,   234,   282,    -1,   174,   227,   175,
      -1,   234,    -1,   234,   172,   225,   173,    -1,   234,   172,
     225,   150,   225,   173,    -1,   174,   227,   175,    -1,   241,
     164,   225,    -1,   244,   163,   243,    -1,   243,    -1,    -1,
      -1,   200,   248,     3,   246,   251,   250,   165,   247,   257,
      51,    -1,    74,    -1,    72,    -1,   283,   214,    -1,   166,
     236,   167,    -1,   166,   237,   167,    -1,    -1,    -1,   168,
     166,   252,   167,    -1,    84,   262,    -1,   252,   163,   262,
      -1,   252,   163,    84,   262,    -1,   200,   260,   240,   249,
     235,   165,    -1,   200,   260,   240,   249,   259,   165,    -1,
     200,   260,   240,   218,   259,   165,    -1,   123,   209,   249,
     235,   165,    -1,   281,   240,   249,   235,   165,    -1,   281,
     260,   240,   283,   235,   165,    -1,    83,   261,   240,   283,
     235,   165,    -1,    67,   261,   240,   283,   235,   165,    -1,
      66,   261,   240,   283,   235,   165,    -1,   281,   240,   249,
       1,   165,    -1,   200,   203,    -1,    44,   211,   165,    -1,
      56,   235,   165,    -1,   200,   233,   232,   165,    -1,   200,
     233,   213,   232,   165,    -1,   200,   233,   218,   232,   165,
      -1,   200,   233,   218,   213,   232,   165,    -1,    91,   232,
     165,    -1,    90,   232,   165,    -1,    91,   166,   221,   167,
     232,   165,    -1,    90,   166,   220,   167,   232,   165,    -1,
       3,   272,   232,   165,    -1,     3,   272,     1,   165,    -1,
      33,   219,   214,   244,   165,    -1,    33,   143,   166,   227,
     167,   165,    -1,    33,   144,   166,   227,   167,   165,    -1,
      33,   145,   166,   227,   167,   165,    -1,    33,   146,   166,
     227,   167,   165,    -1,    33,   147,   166,   227,   167,   165,
      -1,   200,    31,   302,    -1,   200,    65,   302,    -1,    -1,
     113,     3,   165,   254,   309,   306,    55,    -1,    -1,    61,
     284,     3,   165,   255,   230,   302,    50,    -1,   106,    53,
      -1,   106,   289,    53,    -1,   106,     1,    53,    -1,     1,
     165,    -1,    33,     1,   164,   225,   165,    -1,    33,     1,
     165,    -1,    61,     1,    50,    -1,   141,   166,     3,   163,
       5,   163,     5,   167,   165,    -1,   141,   166,     1,   167,
     165,    -1,   256,   253,    -1,   253,    -1,   256,    -1,    -1,
       3,   164,   225,    -1,   259,   163,   258,    -1,   258,    -1,
     130,    -1,   118,    -1,   120,    -1,   110,    -1,   126,    -1,
     121,    -1,   119,    -1,   111,    -1,   131,    -1,   122,    -1,
      95,    -1,     3,   164,   225,    -1,   266,    -1,    -1,   282,
     264,   266,    -1,    -1,   104,   282,   265,   266,    -1,   262,
      -1,   266,   163,   262,    -1,     3,   164,   225,    -1,   271,
      -1,    -1,   282,   269,   271,    -1,    -1,   104,   282,   270,
     271,    -1,   267,    -1,   271,   163,   267,    -1,   168,   166,
     227,   167,    -1,   168,   166,   274,   167,    -1,   168,     8,
      -1,   168,     1,    -1,    -1,   176,     3,   166,   225,   167,
      -1,   176,     3,   166,   167,    -1,   273,    -1,   274,   163,
     273,    -1,   277,    -1,   176,     3,   166,   277,   167,    -1,
     174,   278,   175,    -1,   176,     3,   166,   174,   278,   175,
     167,    -1,   275,    -1,    -1,     3,    -1,     3,   172,   225,
     150,   225,   173,    -1,     3,   172,   225,   173,    -1,     3,
     172,     1,   173,    -1,   277,    -1,   278,   163,   277,    -1,
     176,     3,   166,   225,   167,    -1,   176,     3,   166,     1,
     167,    -1,   176,     3,   166,   167,    -1,   280,   163,   279,
      -1,   279,    -1,    67,    -1,    83,    -1,    66,    -1,   172,
     225,   150,   225,   173,    -1,   282,    -1,    -1,   282,    -1,
      68,    -1,    93,    -1,    94,    -1,   114,    -1,    -1,     3,
      -1,     3,   164,   225,    -1,     3,   172,   225,   150,   225,
     173,    -1,   285,    -1,   286,   163,   285,    -1,   107,   297,
     165,    -1,   293,   165,    -1,   290,   165,    -1,    64,   166,
     225,   167,   293,   165,    -1,    64,   166,   225,   167,   290,
     165,    -1,   134,   166,   299,   163,   299,   163,   215,   300,
     167,   165,    -1,   135,   166,   299,   163,   215,   300,   167,
     165,    -1,   136,   166,   299,   163,   299,   163,   215,   300,
     167,   165,    -1,   138,   166,   299,   163,   299,   163,   215,
     300,   167,   165,    -1,   140,   166,   299,   163,   299,   163,
     215,   163,   215,   300,   167,   165,    -1,   137,   166,   299,
     163,   299,   163,   215,   163,   215,   300,   167,   165,    -1,
     139,   166,   299,   163,   215,   163,   225,   300,   167,   165,
      -1,   139,   166,   299,   163,   215,   167,   165,    -1,   215,
      -1,   288,   163,   215,    -1,   287,    -1,   289,   287,    -1,
     291,   164,   166,   288,   167,    -1,   291,   164,   216,    -1,
     166,    86,   295,   298,    12,     3,   167,    -1,   166,    86,
     295,   298,    12,   166,   228,   292,   225,   167,   167,    -1,
     166,    86,   295,   298,    20,     3,   167,    -1,   166,    86,
     295,   298,    20,   166,   228,   292,   225,   167,   167,    -1,
     166,    76,   295,   298,    12,     3,   167,    -1,   166,    76,
     295,   298,    12,   166,   228,   292,   225,   167,   167,    -1,
     166,    76,   295,   298,    20,     3,   167,    -1,   166,    76,
     295,   298,    20,   166,   228,   292,   225,   167,   167,    -1,
      21,    -1,    22,    -1,   150,    -1,   294,   164,   166,   288,
     167,    -1,   294,   164,   216,    -1,   294,   164,   166,     1,
     167,    -1,   166,   295,   298,    12,   295,   167,    -1,   166,
     295,   298,    20,   295,   167,    -1,   166,     1,   167,    -1,
       3,    -1,     3,   172,   228,   173,    -1,   295,   163,     3,
      -1,   295,   163,     3,   172,   228,   173,    -1,     3,   164,
     225,    -1,     3,   164,   225,   150,   225,   150,   225,    -1,
       6,   164,   225,    -1,     6,   164,   166,   225,   163,   225,
     167,    -1,   296,    -1,   297,   163,   296,    -1,   156,    -1,
     157,    -1,    -1,    86,   225,    -1,    76,   225,    -1,    86,
     228,   148,   225,    -1,    76,   228,   148,   225,    -1,   228,
     148,   225,    -1,   228,    -1,    -1,   301,    -1,   163,    -1,
     163,   234,    -1,   301,   163,    -1,   301,   163,   234,    -1,
     301,   163,   234,   172,   228,   173,    -1,     3,    -1,    33,
     241,   164,   225,   165,    -1,    42,   241,   165,    -1,    58,
     241,   164,   225,   165,    -1,    96,   241,   165,    -1,    34,
     305,    48,    -1,    -1,    34,   150,     3,   303,   205,   305,
      48,    -1,    34,    48,    -1,    34,   150,     3,    48,    -1,
      -1,    60,   150,     3,   304,   205,   305,    69,    -1,    60,
      69,    -1,    60,   150,     3,    69,    -1,    45,   234,   165,
      -1,    30,   234,   165,    -1,    59,   302,    -1,    60,   305,
      69,    -1,    97,   166,   225,   167,   302,    -1,    38,   166,
     225,   167,   207,    49,    -1,    39,   166,   225,   167,   207,
      49,    -1,    40,   166,   225,   167,   207,    49,    -1,    38,
     166,   225,   167,     1,    49,    -1,    39,   166,   225,   167,
       1,    49,    -1,    40,   166,   225,   167,     1,    49,    -1,
      64,   166,   225,   167,   306,    -1,    64,   166,   225,   167,
     306,    47,   306,    -1,    64,   166,     1,   167,   306,    -1,
      64,   166,     1,   167,   306,    47,   306,    -1,    57,   166,
     242,   164,   225,   165,   225,   165,   242,   164,   225,   167,
     302,    -1,    57,   166,   242,   164,   225,   165,   225,   165,
       1,   167,   302,    -1,    57,   166,   242,   164,   225,   165,
       1,   165,   242,   164,   225,   167,   302,    -1,    57,   166,
       1,   167,   302,    -1,   129,   166,   225,   167,   302,    -1,
     129,   166,     1,   167,   302,    -1,   212,   306,    -1,   222,
     200,   306,    -1,   169,   158,   200,   306,    -1,   169,   166,
     158,   167,   200,   306,    -1,   242,   164,   225,   165,    -1,
     242,    10,   225,   165,    -1,   242,   164,   212,   225,   165,
      -1,   242,    10,   212,   225,   165,    -1,   242,   164,   222,
     225,   165,    -1,   242,   164,    97,   166,   225,   167,   222,
     225,   165,    -1,   242,    10,   222,   225,   165,    -1,   242,
      10,    97,   166,   225,   167,   222,   225,   165,    -1,   125,
     166,   225,   167,   306,    -1,     4,   166,   227,   167,   165,
      -1,     4,   165,    -1,   234,   166,   227,   167,   165,    -1,
     234,   165,    -1,     1,   165,    -1,   305,   200,   302,    -1,
     200,   302,    -1,   302,    -1,   165,    -1,   203,    -1,    67,
     283,   235,   165,    -1,    67,   104,   283,   235,   165,    -1,
      83,   283,   235,   165,    -1,    83,   104,   283,   235,   165,
      -1,    66,   283,   235,   165,    -1,    66,   104,   283,   235,
     165,    -1,   308,   307,    -1,   307,    -1,   308,    -1,    -1,
      -1,   112,   311,   312,    54,    -1,   314,    -1,   315,    -1,
     319,   150,   321,   165,    -1,   313,    -1,   314,   313,    -1,
     316,    -1,   315,   316,    -1,   319,   150,   320,   150,   321,
     165,    -1,    65,     3,   164,   199,   165,    -1,   317,    -1,
      -1,   320,    -1,   319,   320,    -1,   177,    -1,   178,    -1,
     179,    -1,   149,    -1,   180,    -1,   158,    -1,   160,    -1,
     181,    -1,   182,    -1,   183,    -1,   184,    -1,   185,    -1,
     186,    -1,   187,    -1,   188,    -1,   189,    -1,   190,    -1,
     191,    -1,   192,    -1,   193,    -1,   194,    -1,   195,    -1,
     156,    -1,   177,    -1,   178,    -1,   179,    -1,   157,    -1,
      67,   235,   165,    -1,    83,     3,   165,    -1,    95,     3,
     165,    -1,    95,    83,     3,   165,    -1,   322,    -1,   323,
     322,    -1,     3,    -1,   324,   163,     3,    -1,    95,    -1,
      -1,   164,   225,    -1,    -1,    67,     3,    -1,   327,   163,
      67,     3,    -1,    87,     3,   166,   324,   167,   165,   323,
     318,   310,    52,    -1,    87,     3,   166,    83,   325,     3,
     326,   163,   327,   167,   165,   310,    52,    -1
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
    1058,  1066,  1074,  1082,  1091,  1093,  1110,  1120,  1135,  1150,
    1161,  1177,  1189,  1197,  1206,  1207,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1238,
    1242,  1254,  1260,  1286,  1292,  1301,  1307,  1313,  1334,  1349,
    1364,  1379,  1394,  1420,  1421,  1424,  1424,  1430,  1437,  1452,
    1463,  1475,  1482,  1491,  1500,  1510,  1519,  1525,  1535,  1538,
    1534,  1547,  1547,  1549,  1554,  1555,  1556,  1562,  1564,  1568,
    1569,  1570,  1574,  1583,  1592,  1600,  1606,  1614,  1618,  1626,
    1631,  1636,  1647,  1652,  1653,  1660,  1664,  1668,  1672,  1679,
    1683,  1688,  1692,  1700,  1706,  1714,  1717,  1720,  1723,  1726,
    1729,  1734,  1740,  1753,  1752,  1773,  1772,  1790,  1793,  1797,
    1806,  1812,  1818,  1824,  1833,  1840,  1846,  1847,  1851,  1852,
    1864,  1874,  1880,  1886,  1887,  1888,  1889,  1890,  1891,  1892,
    1893,  1894,  1895,  1899,  1903,  1920,  1921,  1921,  1926,  1926,
    1934,  1935,  1945,  1962,  1963,  1963,  1968,  1968,  1976,  1977,
    1997,  2003,  2009,  2021,  2027,  2031,  2038,  2048,  2053,  2076,
    2084,  2095,  2104,  2113,  2114,  2130,  2138,  2160,  2177,  2193,
    2195,  2208,  2215,  2223,  2233,  2239,  2247,  2248,  2249,  2253,
    2270,  2271,  2276,  2277,  2278,  2279,  2280,  2281,  2290,  2295,
    2304,  2317,  2323,  2332,  2333,  2336,  2339,  2342,  2345,  2349,
    2353,  2357,  2361,  2366,  2371,  2376,  2382,  2383,  2387,  2388,
    2392,  2393,  2397,  2398,  2399,  2400,  2401,  2402,  2403,  2404,
    2408,  2409,  2410,  2414,  2415,  2416,  2423,  2426,  2429,  2436,
    2442,  2448,  2454,  2463,  2475,  2481,  2485,  2493,  2494,  2498,
    2499,  2500,  2504,  2506,  2508,  2512,  2516,  2520,  2526,  2527,
    2531,  2533,  2535,  2537,  2539,  2543,  2555,  2562,  2573,  2579,
    2592,  2600,  2599,  2612,  2618,  2633,  2632,  2645,  2651,  2659,
    2666,  2673,  2679,  2686,  2692,  2698,  2704,  2710,  2712,  2714,
    2716,  2722,  2728,  2732,  2736,  2743,  2748,  2753,  2757,  2761,
    2765,  2773,  2785,  2794,  2803,  2809,  2815,  2823,  2831,  2837,
    2845,  2852,  2860,  2869,  2877,  2885,  2893,  2901,  2909,  2916,
    2926,  2927,  2932,  2934,  2941,  2948,  2955,  2962,  2969,  2979,
    2985,  2990,  2993,  2997,  2997,  3003,  3004,  3008,  3021,  3027,
    3036,  3042,  3051,  3065,  3077,  3078,  3082,  3088,  3100,  3101,
    3102,  3103,  3104,  3105,  3106,  3107,  3108,  3109,  3110,  3111,
    3112,  3113,  3114,  3115,  3116,  3117,  3118,  3119,  3120,  3121,
    3122,  3126,  3127,  3128,  3129,  3133,  3135,  3141,  3147,  3156,
    3158,  3167,  3173,  3181,  3181,  3184,  3185,  3189,  3195,  3208,
    3223
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
     231,   231,   232,   232,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   235,   235,   236,   236,   237,   237,   237,   238,   238,
     238,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   242,   242,   242,   242,   243,   244,   244,   246,   247,
     245,   248,   248,   249,   250,   250,   250,   251,   251,   252,
     252,   252,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   254,   253,   255,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   256,   256,   257,   257,
     258,   259,   259,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   262,   263,   264,   263,   265,   263,
     266,   266,   267,   268,   269,   268,   270,   268,   271,   271,
     272,   272,   272,   272,   272,   273,   273,   274,   274,   275,
     275,   275,   275,   276,   276,   277,   277,   277,   277,   278,
     278,   279,   279,   279,   280,   280,   281,   281,   281,   282,
     283,   283,   284,   284,   284,   284,   284,   284,   285,   285,
     285,   286,   286,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   288,   288,   289,   289,
     290,   290,   291,   291,   291,   291,   291,   291,   291,   291,
     292,   292,   292,   293,   293,   293,   294,   294,   294,   295,
     295,   295,   295,   296,   296,   296,   296,   297,   297,   298,
     298,   298,   299,   299,   299,   299,   299,   299,   300,   300,
     301,   301,   301,   301,   301,   301,   302,   302,   302,   302,
     302,   303,   302,   302,   302,   304,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   305,   305,
     306,   306,   307,   307,   307,   307,   307,   307,   307,   308,
     308,   309,   309,   311,   310,   312,   312,   313,   314,   314,
     315,   315,   316,   317,   318,   318,   319,   319,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   321,   321,   321,   321,   322,   322,   322,   322,   323,
     323,   324,   324,   325,   325,   326,   326,   327,   327,   328,
     328
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
       5,     4,     5,     2,     1,     2,     4,     5,     5,     3,
       2,     4,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     6,     6,
       6,     6,     8,     1,     0,     1,     0,     1,     4,     2,
       3,     1,     4,     6,     3,     3,     3,     1,     0,     0,
      10,     1,     1,     2,     3,     3,     0,     0,     4,     2,
       3,     4,     6,     6,     6,     5,     5,     6,     6,     6,
       6,     5,     2,     3,     3,     4,     5,     5,     6,     3,
       3,     6,     6,     4,     4,     5,     6,     6,     6,     6,
       6,     3,     3,     0,     7,     0,     8,     2,     3,     3,
       2,     5,     3,     3,     9,     5,     2,     1,     1,     0,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     0,     3,     0,     4,
       1,     3,     3,     1,     0,     3,     0,     4,     1,     3,
       4,     4,     2,     2,     0,     5,     4,     1,     3,     1,
       5,     3,     7,     1,     0,     1,     6,     4,     4,     1,
       3,     5,     5,     4,     3,     1,     1,     1,     1,     5,
       1,     0,     1,     1,     1,     1,     1,     0,     1,     3,
       6,     1,     3,     3,     2,     2,     6,     6,    10,     8,
      10,    10,    12,    12,    10,     7,     1,     3,     1,     2,
       5,     3,     7,    11,     7,    11,     7,    11,     7,    11,
       1,     1,     1,     5,     3,     5,     6,     6,     3,     1,
       4,     3,     6,     3,     7,     3,     7,     1,     3,     1,
       1,     0,     2,     2,     4,     4,     3,     1,     0,     1,
       1,     2,     2,     3,     6,     1,     5,     3,     5,     3,
       3,     0,     7,     2,     4,     0,     7,     2,     4,     3,
       3,     2,     3,     5,     6,     6,     6,     6,     6,     6,
       5,     7,     5,     7,    13,    11,    13,     5,     5,     5,
       2,     3,     4,     6,     4,     4,     5,     5,     5,     9,
       5,     9,     5,     5,     2,     5,     2,     2,     3,     2,
       1,     1,     1,     4,     5,     4,     5,     4,     5,     2,
       1,     1,     0,     0,     4,     1,     1,     4,     1,     2,
       1,     2,     6,     5,     1,     0,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     4,     1,
       2,     1,     3,     1,     0,     2,     0,     2,     4,    10,
      13
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      11,     0,     0,     0,     0,    11,     0,     4,    63,    64,
      14,    10,     0,    13,     0,     0,     1,     5,   222,   221,
       0,     0,     9,     0,     0,     0,   218,   189,   139,   137,
       6,     7,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,   135,    15,    91,   138,    12,
     531,   534,     0,     0,   227,   133,     8,    99,   100,   101,
      97,   103,    98,    95,    92,    93,   129,     0,    94,   102,
      96,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,   533,     0,     0,     0,     0,     0,   226,   132,     0,
       0,   144,   133,   134,   145,   122,   123,   120,   124,   121,
     125,   117,   118,   119,   126,   127,   112,   113,   114,     0,
     111,   104,   110,   115,   116,   108,   109,   105,   106,   107,
       0,     0,   190,   536,   532,     0,     0,     0,    11,     0,
     143,     0,     0,   131,     0,   142,     0,   140,     0,     0,
       0,     0,     0,   529,   495,     0,     0,     0,   325,     0,
       0,     0,     0,     0,   195,   323,   193,   319,   219,     0,
       0,   128,     0,   535,     0,   191,     0,     0,     0,     0,
       0,   494,     0,   530,    65,     0,   229,     0,   228,     0,
     329,     0,     0,   204,   204,   204,   324,   224,    11,   225,
       0,   130,   146,   141,     0,     0,     0,   525,   526,   527,
       0,     0,   483,     0,     0,     0,   230,     0,     0,     0,
     321,     0,   286,   290,   284,   289,   285,   288,   292,   287,
     283,   291,   206,   203,   206,   293,   206,   206,   194,   197,
     196,     0,   314,     0,     0,     0,     0,   338,   336,   337,
       0,     0,     0,     0,    45,     0,     0,   277,     0,     0,
     206,   537,     0,     0,   192,   528,     0,     0,   539,   294,
     231,   328,     0,   327,   330,     0,     0,   205,   341,   341,
     341,   341,   270,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,    57,     0,    47,     0,     0,     0,     0,
       0,   343,   344,   345,   346,   342,     0,   206,   206,   206,
       0,   133,   163,     0,   133,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,     0,     0,     0,     0,    44,   341,     0,     0,
     164,   170,   171,   172,   180,     0,     0,     0,   165,   176,
     167,   173,   174,   175,   166,     0,   178,   181,     0,     0,
       0,   177,   179,   183,   186,   187,     0,   182,   184,   185,
     169,   168,   188,   242,     0,   206,   276,   220,   341,   206,
       0,     0,     0,   501,   520,   503,   504,   498,   499,   500,
     502,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,     0,   488,   485,   486,
     490,     0,   496,     0,     0,   320,   340,     0,     0,     0,
       0,   313,   312,   133,     0,   133,     0,     0,   272,   133,
     133,   133,   133,   133,     0,     0,    76,    80,    75,    79,
      74,    78,    77,    81,     0,     0,     0,    56,     0,     0,
       0,   243,     0,    48,   244,   273,     0,   341,   341,   341,
     133,   160,     0,     0,     0,   250,     0,   249,   269,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,     0,   401,   268,   369,   355,     0,
     354,     0,   263,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,     0,     0,    11,     0,     0,     0,     0,     0,     0,
       0,   133,     0,    11,   211,     0,   261,   262,     0,   348,
     351,     0,     0,     0,     0,   308,     0,   303,   304,     0,
       0,   300,     0,   295,   296,     0,     0,     0,     0,     0,
       0,     0,     0,   133,     0,     0,     0,   341,     0,   341,
     538,     0,   493,   484,   489,     0,   491,     0,     0,   497,
     326,     0,   199,   198,   200,   201,     0,     0,   317,     0,
     254,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    60,    61,     0,    52,   133,   207,     0,
     217,     0,     0,    49,    46,   265,     0,     0,     0,     0,
       0,   335,     0,   133,     0,   159,   162,     0,     0,     0,
       0,     0,   353,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,   388,     0,   401,   401,   399,   400,     0,
       0,     0,   371,     0,   384,   482,    43,    42,    41,     0,
     223,     0,     0,   467,   464,   133,     0,     0,   423,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,   431,
     427,     0,    11,     0,     0,     0,     0,     0,     0,    50,
      11,     0,    82,     0,   471,   470,   450,     0,   466,   133,
       0,     0,     0,    25,     0,     0,     0,    18,    30,     0,
     306,    23,     0,     0,    29,   298,    22,     0,     0,    27,
      20,    28,    21,    24,     0,     0,    26,    19,     0,     0,
       0,   245,     0,     0,     0,     0,     0,   540,     0,     0,
     524,   498,   499,   500,     0,     0,   322,     0,     0,   310,
       0,   311,   271,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,   209,     0,
       0,   255,     0,    11,     0,     0,     0,     0,   156,     0,
     161,     0,     0,     0,     0,     0,   393,     0,   395,   398,
     403,    91,   402,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   391,     0,     0,   366,     0,
       0,     0,   341,   341,   341,   472,   480,   481,     0,   235,
     275,     0,     0,   430,     0,   421,   469,   420,     0,     0,
       0,     0,   417,   429,     0,   211,     0,     0,   425,   432,
       0,     0,   419,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    90,   214,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,   352,   302,     0,   309,   305,     0,   301,   297,     0,
      17,   246,     0,   247,     0,   282,     0,   191,     0,     0,
     241,   236,     0,   521,   522,   523,     0,   487,   202,     0,
     318,   256,   257,   258,   259,   260,    71,    70,    68,    66,
      69,    67,     0,    53,     0,   210,     0,   215,   216,   339,
     341,   341,   341,     0,   154,     0,   240,   239,   238,     0,
     334,   157,   158,   252,   251,     0,     0,     0,   129,     0,
       0,   406,     0,   408,     0,     0,     0,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,   370,   385,
     383,   341,     0,   341,     0,   341,     0,   479,     0,     0,
       0,     0,   424,    34,   468,     0,     0,     0,     0,     0,
       0,   428,    34,     0,     0,     0,     0,     0,     0,    51,
     452,    84,    89,    88,    11,     0,     0,    83,     0,     0,
     212,     0,     0,     0,     0,   455,     0,     0,     0,   454,
       0,   307,   299,    16,   248,     0,     0,   234,   232,   233,
     237,     0,   316,     0,     0,     0,   208,   341,     0,   341,
       0,   341,     0,   153,   155,     0,     0,   333,     0,   357,
     356,     0,     0,   405,   404,     0,   415,   410,     0,   409,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   386,   387,   367,     0,     0,
       0,     0,     0,     0,   264,     0,   463,   416,    31,    33,
      11,     0,     0,    40,     0,     0,     0,     0,     0,     0,
     447,     0,   418,    11,   442,   440,   433,   462,   449,   448,
       0,    86,    87,   465,     0,     0,   457,   460,     0,   456,
     458,     0,   280,   281,   492,   315,     0,    54,     0,     0,
       0,     0,     0,     0,     0,   266,   332,   331,     0,     0,
     408,   411,     0,   412,   408,     0,   408,   408,   365,     0,
     376,     0,   378,     0,   372,     0,   374,     0,   392,     0,
     477,     0,   473,     0,   475,     0,    32,    11,   437,     0,
       0,    37,     0,   434,    39,     0,   438,   435,   439,   436,
       0,    11,     0,     0,   453,   213,     0,     0,   350,     0,
      59,     0,   151,     0,   147,     0,   149,   394,   396,     0,
     359,   413,     0,     0,     0,     0,     0,   380,   381,   382,
       0,     0,     0,     0,   478,   474,   476,   274,   422,    38,
      36,    35,     0,     0,   426,   443,   441,     0,     0,    55,
     152,   148,   150,     0,     0,     0,   408,     0,     0,   408,
       0,     0,     0,     0,     0,     0,     0,     0,   358,     0,
     360,     0,   361,   364,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   459,   414,     0,     0,   377,   379,   373,
     375,     0,     0,     0,   363,   362,     0,   445,     0,     0,
       0,   446,   444
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,    45,   670,    12,    13,   815,  1079,  1080,
    1083,  1084,   346,   347,   305,   306,   532,   457,   458,   808,
     606,     7,   302,   303,   454,   455,   533,   853,   854,   108,
      67,  1085,    47,   924,   925,   322,   323,   384,    48,   186,
     172,   173,   174,   242,   288,   609,   535,   610,   611,     8,
      54,   210,    20,   506,   149,   107,   167,   267,   655,   773,
     268,   269,   885,   886,   243,   247,   551,   552,   718,   876,
     553,   545,   546,   713,   873,   547,   294,   588,   589,   175,
     176,   177,   201,   621,   622,   270,   426,   507,   316,   540,
     541,   338,   809,   339,   340,   341,  1200,   342,   343,   495,
     482,   483,   650,   636,  1048,  1049,   695,   973,   982,   671,
     696,   816,   817,   818,   223,   277,   416,   417,   418,   419,
     420,   191,   192,   421,   422,   745,   163,   164,    52,   102,
     159,   215,     9
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -774
static const yytype_int16 yypact[] =
{
     219,    98,    19,  -121,   230,   249,   564,  -774,  -774,  -774,
     -64,  -774,   148,  -774,    81,   281,  -774,  -774,  -774,  -774,
     305,  1689,  -774,   371,   264,   214,  -774,  -774,   235,  -774,
    -774,   417,  -774,   157,   157,   157,   157,   122,   157,   157,
     157,  1689,   157,   138,  1689,  -774,  4310,  -774,   435,  -774,
    -774,   316,   241,   432,   282,  1689,  -774,  -774,  -774,  -774,
    -774,  -774,  -774,  -774,  -774,  -774,  3988,   320,  -774,  -774,
    -774,  2196,    43,  1689,  1689,  1689,  1689,  1689,  1689,  1689,
    1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,
    1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,  1689,
     489,  -774,   509,   516,   378,   415,   374,   391,  4310,   292,
    1689,  -774,  1689,  1689,  -774,   348,   348,   585,   585,   585,
     585,   553,   553,   553,  4330,  4406,  2009,  4501,  4501,  4024,
    4481,  4501,  2009,   348,   348,   708,   708,  -774,  -774,  -774,
     359,  2256,  -774,   428,  -774,   377,   605,   513,   131,   466,
    -774,  4044,   291,  4310,  1689,  -774,  1689,  -774,  1689,   459,
     636,   641,   275,  -774,   426,   493,   660,   460,   520,   701,
     719,   356,   483,   510,  -774,  -774,  -774,  -774,  -774,  1689,
     610,  4310,  2407,  4310,   685,  -774,    -6,   661,   666,   759,
     798,  -774,   698,  -774,  -774,   671,  -774,   226,  -774,  1533,
    -774,   379,   706,  1082,  1082,   993,    16,  -774,   233,  -774,
    4641,  4310,  -774,  -774,   873,   523,   881,  -774,  -774,  -774,
     732,   741,  -774,   860,  1689,   660,  -774,   753,  2305,   701,
    -774,    35,  -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,
    -774,  -774,   834,  -774,   834,  -774,   834,   834,  -774,  -774,
    -774,   766,   776,    80,    29,   636,   114,   857,   857,   857,
      85,   133,   393,   973,   817,   821,  4868,  -774,  4765,   944,
     739,  -774,   929,   841,  -774,  -774,   647,  4940,  -774,  4310,
    -774,  -774,  1689,  -774,  -774,   701,   856,  -774,   850,   850,
     850,   850,  -774,    86,   111,   504,   862,   867,   889,   894,
     898,   407,  -774,   893,  1689,  -774,   524,   361,  1062,   542,
    1016,  -774,  -774,  -774,  -774,  -774,  1068,   834,   834,   834,
     302,  1894,  -774,   606,  1921,   714,  1021,  -774,   909,   338,
     912,   917,   939,   940,   943,   951,   955,   350,  -774,   901,
     928,   918,   945,   965,   985,   394,  -774,   850,   891,  1348,
    -774,  -774,  -774,  -774,  -774,  1348,   895,    83,  -774,  -774,
    -774,  -774,  -774,  -774,  -774,    90,  -774,  -774,   903,   919,
     110,  -774,  -774,  -774,  -774,  -774,   920,  -774,  -774,  -774,
    -774,  -774,  -774,  -774,   125,   834,  -774,  -774,   850,   834,
    1127,   698,   986,  -774,  -774,  -774,  -774,  -774,  -774,  -774,
    -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,
    -774,  -774,  -774,  -774,  -774,  -774,  1080,  -774,  4940,  4940,
    -774,  4390,  -774,  2456,   390,  -774,  -774,  1149,  1150,  1151,
    1158,  -774,  -774,  1594,   998,  1689,   764,  1689,  -774,  1689,
    1689,  1689,  1689,  1689,  1001,  1002,  -774,  -774,  -774,  -774,
    -774,  -774,  -774,  -774,  1003,  1004,   172,  -774,    38,  4064,
    1062,  -774,  1689,  -774,  -774,  -774,  1005,   850,   850,   850,
    1629,  1007,  1009,   531,   135,  -774,  1010,  -774,  -774,  1689,
    1014,  1019,  -774,   767,   375,   375,   375,   375,   375,   375,
     375,  1017,   996,  1166,  1166,   431,  -774,  -774,  -774,   176,
    -774,   180,  -774,  1038,  1039,  1040,   636,   893,  1043,  1008,
    1023,   682,  1062,    38,   187,  1045,  1048,  1051,    38,  1062,
    1052,    38,  1348,   245,  1055,    38,  1056,  1063,  1064,   196,
     115,  1689,  1274,  1209,   506,   106,  -774,  -774,  1069,   388,
    -774,   774,  1070,  1072,   850,  -774,  1073,  1074,  -774,  1076,
     850,  -774,  1084,  1088,  -774,  1090,   785,  1091,   794,  1093,
      48,  1094,   803,  1805,   135,   129,   804,   401,   969,   850,
    -774,  1181,  -774,  -774,  -774,  4437,  -774,  4846,  4893,  -774,
    -774,  1095,  -774,  -774,  -774,  1075,  1244,   532,  -774,   534,
    -774,  -774,  3449,   539,   557,   558,   568,   584,   354,   518,
     457,   462,  -774,  -774,  -774,  1689,  -774,  1689,   577,  1100,
    -774,   808,  1689,  -774,  4310,  -774,   636,   636,   636,  1262,
     587,  -774,   592,  1629,   135,  -774,  -774,   135,  2674,  1689,
    2143,   338,  -774,  1689,  1689,  1120,  1108,  1110,  1111,  1113,
    1121,  1122,  1123,  -774,   157,   431,   431,  -774,  -774,  1284,
     328,  1689,  -774,  1673,  -774,   932,  -774,  -774,  -774,   814,
    -774,  1124,  1285,  -774,  -774,  1689,    57,  1128,  -774,  1288,
    1348,    96,  1689,  1689,  1689,  1129,   238,    47,  1133,  -774,
    -774,  1290,   101,  1705,  1134,  1689,  1689,  1734,  1689,  -774,
    1209,  1338,  1125,   423,  -774,  -774,  -774,  1274,  -774,  1689,
    1689,  1961,  1988,  -774,  1689,  1689,  1295,  -774,  -774,  1689,
    -774,  -774,  1299,  1299,  -774,  -774,  -774,   660,   660,  -774,
    -774,  -774,  -774,  -774,  1295,   819,  -774,  -774,   823,   135,
     827,  -774,  1300,  1302,  1141,   828,   636,  -774,   404,  4940,
    -774,  1144,  1145,  1152,  1168,  1155,  -774,  1689,  1156,  -774,
    1147,  -774,  -774,  1159,  1160,  1161,  1162,  1163,  1154,  1169,
    1170,  1173,  1177,  1183,   597,  4139,   442,  1689,  -774,  1689,
      38,  -774,  2523,   362,   842,   854,   855,  1164,  -774,  1153,
    -774,   598,   600,   879,   880,  1182,  4215,  1689,  4310,  -774,
    4310,  1187,  4310,  1205,  1689,   375,  1689,   375,   375,   375,
    1689,   375,  1184,   482,   559,  1189,  1166,  1166,  -774,   611,
    1191,   614,    94,   128,   199,  -774,  -774,   932,  1274,  -774,
    -774,  1193,   620,  -774,  1689,  1311,  -774,  -774,  1348,  2709,
    2785,  2864,  -774,  -774,  1195,   628,  1199,  1689,  1303,  -774,
    1197,  2884,  -774,  2904,  2940,  1201,  2960,  1202,  1274,  1206,
    1689,  1689,  1207,    71,  -774,  4310,  -774,  -774,   639,  2347,
    1210,   149,  1689,  1689,  3485,  1211,  1689,  1689,  3505,  4310,
    4235,  -774,  4310,  1299,  -774,  1074,   660,  -774,  1088,   884,
    -774,  -774,   888,  -774,  1215,  -774,   905,  1215,   916,   921,
    -774,  -774,   922,  -774,  -774,  -774,   404,  -774,  4310,  2094,
    -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,  -774,
    -774,  -774,  1689,  -774,  1689,  -774,  2372,  4310,  -774,  -774,
     224,   225,   268,   769,  -774,  1186,  -774,  -774,  -774,  1498,
    -774,  -774,  -774,  -774,  -774,  1218,  1219,  1689,  3968,  1689,
    1689,  4310,  1212,    37,  1217,  1222,  1226,   644,  1228,  -774,
     152,   160,   164,   173,   157,   645,   646,  1689,  -774,  -774,
    -774,   850,   636,   850,   636,   850,   636,  -774,  1337,  1390,
    1239,  3525,  -774,   769,  -774,  1393,  1422,  1454,  1348,  1689,
    3602,  -774,   769,  1274,  1274,  1348,  1274,  1348,  1348,  -774,
    -774,  -774,  4310,  4310,  1209,  2034,  2034,  -774,  1245,  1689,
    -774,  1689,   729,  3678,  3698,  -774,  1689,  3718,  3755,  -774,
    1689,  1074,  1088,  -774,  -774,  1689,  1300,  -774,  -774,  -774,
    -774,  1246,  -774,  2980,   658,  4255,  -774,   850,   636,   850,
     636,   850,   636,  -774,  -774,  1359,  1248,  -774,  3059,  -774,
    -774,  4290,  1689,  4310,  4310,  1689,  -774,  1062,  1249,  1250,
    1689,  1689,  1689,  1689,  1252,  1689,  1251,   157,  1261,   157,
    1265,   157,  1266,   157,  1264,  -774,  -774,  -774,   636,   926,
     636,   927,   636,   931,  -774,  1267,  -774,  -774,  -774,   769,
    1209,   100,  1098,  -774,   236,   250,   119,   790,   182,   906,
    -774,  3775,  -774,  1209,  1372,  1388,  -774,  -774,  -774,  -774,
    1274,  -774,  -774,  -774,  2558,  3135,  -774,  -774,  3155,  -774,
    -774,  2607,  4310,  -774,  -774,  -774,  1689,  -774,  1689,   636,
     935,   636,   953,   636,   957,  -774,  -774,  -774,  1689,  3175,
      37,  1125,  1273,  1062,    37,  1277,    37,  2634,  -774,  1278,
    -774,   195,  -774,   195,  -774,   195,  -774,   195,  -774,   962,
    -774,   970,  -774,   976,  -774,  1281,  -774,   382,  -774,  1274,
    1274,  -774,  1297,  -774,  -774,  1274,  -774,  -774,  -774,  -774,
    1789,   329,  1274,  1274,  -774,  -774,  1283,  1283,  -774,  1286,
    4310,   979,  -774,   982,  -774,   983,  -774,  4310,  -774,  1287,
    -774,   651,  1289,  1689,  1293,  1301,  1689,  -774,  -774,  -774,
    1689,  1689,  1689,  1689,  -774,  -774,  -774,  -774,  -774,  -774,
    -774,  -774,  1304,  3795,  -774,  -774,  -774,  1689,  1689,  -774,
    -774,  -774,  -774,  1305,   157,  1306,    37,  1307,  1309,    37,
    3214,  3234,  3254,  3330,    41,    79,  3871,  3948,  -774,  1291,
    -774,  1308,  -774,  -774,  1312,  1313,  1317,  1318,  1319,  1314,
    1320,  1324,  -774,  -774,  -774,  1327,  1328,  -774,  -774,  -774,
    -774,  1689,  1348,  1689,  -774,  -774,  3409,  -774,  3429,  1348,
    1348,  -774,  -774
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -774,  -774,  -774,  1190,     3,  -774,  1444,  -265,  -774,   494,
    -428,  -301,  -774,  -774,  -158,  -774,   150,  -363,   991,  -603,
     -28,  1495,  -371,  -774,  -312,  -308,  -687,  -774,  -140,   -21,
    -774,    -8,    -9,   586,  -774,  1036,  -215,  -774,    26,  -248,
    -774,  -774,  1310,   697,   330,   116,  -671,   743,  -774,  -774,
    -774,  -774,  -774,  -376,  -774,  -774,  -774,  1247,  -774,  -774,
    -774,  -774,   498,   786,   562,   631,  -148,  -774,  -774,  -774,
    -616,   809,  -774,  -774,  -774,  -614,  -774,   770,  -774,  -774,
    1322,    88,  1238,   745,   907,  -774,  -217,  -193,  -774,   825,
    -365,  1194,   876,  -774,   747,  -774,  -529,   750,  -774,  -451,
     908,  -774,   317,  -380,  -773,  -774,  -345,  -774,  -774,  -518,
    -662,   730,  -774,  -774,  1157,  -774,  -774,  1135,  -774,  -774,
    1132,  -774,  -774,   583,  -404,   656,  1391,  -774,  -774,  -774,
    -774,  -774,  -774
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -524
static const yytype_int16 yytable[] =
{
      46,   383,   764,     6,   536,   682,   836,   309,     6,   472,
     537,   562,   568,   565,   863,   867,   476,   579,   196,   168,
      66,   564,    14,    71,    57,    58,    59,    60,    62,    63,
      64,    65,    27,    68,    70,   857,    72,   308,   168,   315,
    1046,    27,   645,   646,    27,    15,   325,   109,   834,   226,
      27,   539,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   280,   141,   436,
    1250,   295,    27,   -73,   542,   847,   543,   431,   320,   151,
     140,   549,   153,   195,   432,   427,   428,   429,   430,   875,
      21,    10,   878,   471,   152,   637,   638,   639,   640,   641,
     642,   559,   434,  -206,   320,   310,   701,  -347,    27,     1,
     556,   558,    11,    61,     1,    27,    28,    29,   320,    30,
      31,    32,   320,   181,   168,   182,   320,   183,   320,    69,
     548,    27,    28,    29,   827,    30,    31,    32,   554,  1158,
     463,   171,    27,   995,     1,  1056,   968,   216,   211,   217,
      27,    28,    29,  1058,    30,    31,    32,  1060,  1166,   566,
     839,   579,    22,   579,   744,   602,  1062,   679,   228,   602,
     603,   604,   311,   602,   603,   604,   990,   544,   603,   604,
     169,   733,   170,   943,   550,   725,   732,   947,   961,   602,
    1047,   304,   729,   279,   603,   604,   113,   312,   313,   285,
       1,   171,   607,   266,   287,   531,  1197,  1198,   114,    -3,
     304,   531,   823,   296,   297,   298,   299,   300,   314,   195,
      16,  1168,   963,   100,   996,   668,   249,  1162,   997,    27,
      28,    29,     1,    30,    31,    32,   301,    24,   -73,    -2,
    1159,   321,   433,   531,   -73,   304,     1,   200,   659,  1011,
    1012,   423,   304,    33,    34,    35,   304,    50,     1,  1159,
     702,   266,     1,   690,   616,   617,   618,   435,   188,  1082,
     307,   691,  -206,   459,    25,  1163,   304,   759,    41,   763,
     758,   563,   761,   456,  -324,   435,    44,   456,  -324,   324,
     304,   435,   613,   965,    41,   169,     2,   170,    26,  1024,
     225,    23,    44,   473,   680,  1002,   473,   284,  1057,   286,
     735,  1094,  1095,    41,  1097,   826,  1059,   710,  1027,  1029,
    1061,    44,  1159,   715,   307,   744,     2,   669,   605,  1063,
     806,   480,   651,   724,   481,  1199,   653,    51,   807,   728,
     730,   491,     1,   492,  1067,   955,   956,  1189,   189,   879,
       3,  1192,   688,  1194,  1195,    79,    80,    81,   774,   775,
     776,   304,  1031,   200,    10,   534,   736,    53,    27,    28,
      29,   534,    30,    31,    32,     1,  -133,    36,    37,    38,
       3,   768,    39,    40,   326,   681,   304,   304,  1214,  -133,
    1165,    55,    41,   833,   103,     1,    42,    43,   104,   783,
      44,   101,   784,   113,   100,   942,   592,   944,   945,   946,
    1161,   948,   203,   204,    56,   587,   493,   473,   920,   921,
    1208,   593,   594,   595,   596,   597,   494,   105,  1174,   205,
     304,   614,  1130,   447,   160,   922,   327,  1134,  1135,  1136,
     106,   633,  1139,  1241,   113,   113,  1244,   328,   628,   150,
     161,   634,   620,   449,   503,   451,   180,   504,   470,   444,
     445,   652,   162,   654,   304,   635,   635,   635,   635,   635,
     635,   635,   453,   974,   608,   888,   307,   111,   892,  1217,
    1218,   190,   142,   160,   950,   446,   447,  1209,  1210,   505,
     329,   689,   951,  1211,    93,    94,    95,    96,    97,   161,
    1215,  1216,   143,  1179,   882,   448,   449,   450,   451,   144,
     760,   162,   113,   693,   762,   462,   155,   330,   331,   332,
     333,   334,   335,   336,   452,   453,   697,   100,   666,   608,
     147,    41,   229,   145,   608,   676,   447,   608,   534,    44,
     446,   608,   704,   229,   230,   473,   692,   148,   534,   337,
     705,   740,  1157,  1249,  1251,   581,   449,   301,   451,   877,
     448,   952,   450,   304,   289,  1171,   290,   291,   146,   953,
    1035,   893,   894,   895,   765,   453,   113,   647,   648,   452,
    1226,   772,   158,  1229,   649,    73,    74,   166,   856,   766,
     388,    98,    79,    80,    81,   113,   446,    99,   786,   788,
     165,   100,   790,   792,  1201,   781,  1202,   915,  1203,   962,
     964,   966,   184,   197,   791,   793,   448,   198,   450,   667,
     765,   178,   765,  1090,   675,   802,    18,   678,    19,   185,
    1096,   684,  1098,  1099,   187,   452,   206,   467,   468,   469,
     207,   829,   830,   831,    30,    31,  1164,   822,  1033,  1164,
     194,  1164,   841,   195,   843,   844,   846,   765,   437,   438,
     855,   698,   699,   208,   828,  1087,  1089,   209,   700,   859,
     864,   868,   100,   869,   870,   828,   272,   460,   872,   461,
     273,   858,   199,   848,   113,   113,   534,   750,   625,   749,
     560,   751,   113,   835,   168,   216,   753,   464,  1078,    93,
      94,    95,    96,    97,  1069,   567,  1071,  1078,  1073,   569,
     113,   113,   202,   534,   754,   755,   898,  1028,  1030,  1032,
     849,   113,    27,    28,    29,   756,    30,    31,    32,    91,
      92,    93,    94,    95,    96,    97,   916,   113,   917,   767,
     113,   757,   214,   100,   778,   779,    33,    34,    35,   780,
     912,   113,   220,   779,   913,   931,   938,   932,  1068,   474,
    1070,   475,  1072,   941,   957,   765,   923,   957,   958,   765,
    1120,   960,  1122,   113,  1124,   212,   635,   970,   635,   635,
     635,  1162,   635,    27,    28,    29,   608,    30,    31,    32,
     700,   221,   113,   971,   100,   850,   998,  1053,   649,   649,
     222,  1054,  1065,  1066,  1156,   851,   980,    33,    34,    35,
    1149,  1116,  1151,  1224,  1153,  1117,   218,   100,   385,   992,
     993,   219,   389,  1082,  1119,   224,  1121,   356,  1123,  1167,
     357,  1003,  1004,   287,   534,  1007,  1008,   664,   665,   232,
     233,   862,   866,   365,   534,  1101,  1102,   234,   235,   236,
     237,   238,   368,   369,   370,   239,    95,    96,    97,   240,
     241,  1181,   231,  1183,   534,  1185,   271,   474,  1023,   477,
      36,    37,    38,   376,   274,    39,    40,   692,   317,   318,
     319,   765,   508,  1025,   509,    41,   538,   275,   539,    42,
      43,   244,   246,    44,   555,   276,   185,  1162,  1038,    27,
      28,    29,   278,    30,    31,    32,  1041,  1267,  1043,  1044,
     557,   561,   185,   539,  1271,  1272,   281,   474,   923,   591,
     631,   292,   632,    33,    34,    35,   765,   706,   287,   707,
    -133,    36,    37,    38,   293,  1064,    39,    40,   216,  1082,
     720,   534,   245,  -133,   496,  1169,    41,   216,  1091,   722,
      42,    43,   803,   804,    44,   328,   706,   474,   727,   731,
     734,   770,   185,   771,   855,   855,   344,   216,  1104,   819,
    1105,   855,   706,   345,   880,  1108,   474,   348,   881,  1111,
     474,   216,   883,   891,  1112,   387,   390,  1100,   812,   813,
     356,   575,   577,   357,   534,   216,   391,   926,   329,   534,
     534,   534,   534,   534,   534,   814,   365,   216,   216,   927,
     928,  1129,   304,   425,   765,   368,   369,   370,   439,   765,
     765,   765,  1137,   440,   765,   330,   331,   332,   333,   334,
     335,   336,   474,   474,   933,   934,   376,   706,  1141,  1013,
    1143,   474,  1145,  1014,  1147,   441,  -133,    36,    37,    38,
     442,   456,    39,    40,   443,    27,   465,   337,  1016,  -133,
    1017,   466,    41,  1131,   478,   479,    42,    43,   484,   216,
      44,  1018,   499,   485,  1016,   216,  1019,  1020,   245,   216,
     216,  1150,  1152,   498,   216,   765,  1154,  1180,   216,   510,
    1182,    27,   511,   232,   233,   486,   487,  1187,   534,   488,
     500,   234,   235,   236,   237,   238,   216,   489,  1184,   239,
     216,   490,  1186,   240,   241,   216,   534,  1204,   512,   501,
     570,   513,   514,   216,   573,  1205,   515,   516,   517,   216,
     518,  1206,   216,   519,  1220,   216,   216,  1221,  1222,  1213,
     502,   572,   582,   583,   584,   520,   521,   522,   523,  1191,
     828,   585,   524,   590,   598,   599,   600,   601,   644,   492,
     615,   662,   765,   623,   828,   765,   624,   627,   629,  1230,
    1231,  1232,  1233,   630,   643,   534,   534,   510,   663,    27,
     511,   534,   232,   233,   525,   526,  1236,  1237,   534,   534,
     234,   235,   236,   237,   238,   656,   657,   658,   239,     1,
     661,   672,   240,   241,   673,  1239,   512,   674,   677,   513,
     514,   683,   685,   527,   515,   516,   517,   528,   518,   686,
     687,   519,     1,   737,   703,   708,   709,   712,   711,   747,
    1266,   714,  1268,   520,   521,   522,   523,   748,  1160,   716,
     524,   717,   920,   921,   -11,   719,   721,   -11,   723,   726,
     835,   835,   746,   694,   769,   777,   529,   530,   794,   922,
     -11,   795,   531,   796,   797,   510,   798,    27,   511,   -11,
     -11,   -11,   525,   526,   799,   800,   801,   805,   534,   820,
     821,   825,   824,   838,   832,   534,   534,   837,   539,   842,
     -11,   100,   543,   884,   512,   887,   890,   513,   514,  -521,
    -522,   527,   515,   516,   517,   528,   518,  -523,   896,   519,
     897,   906,   899,   586,   901,   902,   903,   904,   905,   619,
     929,   520,   521,   522,   523,   939,   907,   908,   524,   849,
     909,    27,    28,    29,   910,    30,    31,    32,   337,   510,
     911,    27,   511,   940,   529,   530,   969,   949,   959,   972,
     531,   954,   978,   979,   983,    33,    34,    35,   987,   989,
     525,   526,   981,   991,   994,  1045,  1001,  1006,   512,  1015,
    1050,   513,   514,  1039,  1040,  1051,   515,   516,   517,  1052,
     518,  1055,  1074,   519,  1081,  1075,    27,    28,    29,   527,
      30,    31,    32,   528,  1076,   520,   521,   522,   523,  1125,
    1103,  1114,   524,  1133,   850,  1126,  1132,  1138,  1140,  1172,
      33,    34,    35,  1086,   851,    27,    28,    29,  1142,    30,
      31,    32,  1144,  1146,  1155,  1173,  1082,  1148,  1190,   694,
    1193,  1196,   529,   530,   525,   526,  1207,  1159,   531,    33,
      34,    35,   861,  1219,  1223,  1088,  1225,    27,    28,    29,
    1227,    30,    31,    32,  1254,  1082,   392,    49,  1228,  1234,
    1238,  1240,  1242,   527,  1243,  1255,  1093,   528,  1261,  1256,
    1257,    33,    34,    35,  1258,  1259,  1260,  1262,  1263,    36,
      37,    38,  1264,  1265,    39,    40,   852,  1082,   660,  1036,
      17,    27,    28,    29,    41,    30,    31,    32,    42,    43,
     626,  1034,    44,   918,  1113,   386,   529,   530,   250,   889,
     900,   874,   531,   424,   930,    33,    34,    35,   248,   811,
     782,   871,   935,   497,   227,   936,    27,    28,    29,   789,
      30,    31,    32,  -133,    36,    37,    38,   967,   571,    39,
      40,   576,  1021,   574,     0,   193,  -133,     0,     0,    41,
      33,    34,    35,    42,    43,     0,     0,    44,     0,     0,
       0,     0,  -133,    36,    37,    38,     0,     0,    39,    40,
       0,     0,     0,     0,     0,  -133,     0,     0,    41,     0,
       0,     0,    42,    43,     0,     0,    44,    27,    28,    29,
       0,    30,    31,    32,  -133,    36,    37,    38,     0,     0,
      39,    40,     0,     0,     0,     0,     0,  -133,     0,     0,
      41,    33,    34,    35,    42,    43,     0,     0,    44,     0,
       0,     0,    27,    28,    29,     0,    30,    31,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      37,    38,     0,     0,    39,    40,    33,    34,    35,     0,
       0,     0,     0,     0,    41,  1037,     0,     0,    42,    43,
       0,     0,    44,     0,   810,     0,    27,    28,    29,     0,
      30,    31,    32,     0,    36,    37,    38,     0,     0,    39,
      40,     0,    27,    28,    29,     0,    30,    31,    32,    41,
      33,    34,    35,    42,    43,     0,   840,    44,    27,    28,
      29,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    34,    35,   845,     0,    27,    28,    29,
       0,    30,    31,    32,     0,    36,    37,    38,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      41,    33,    34,    35,    42,    43,     0,     0,    44,     0,
     586,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,     0,    39,    40,     0,     0,     0,
    1212,     0,    27,    28,    29,    41,    30,    31,    32,    42,
      43,     0,     0,    44,     0,   619,     0,     0,    27,    28,
      29,     0,    30,    31,    32,     0,    33,    34,    35,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,    39,
      40,     0,    33,    34,    35,     0,     0,     0,     0,    41,
      36,    37,    38,    42,    43,    39,    40,    44,     0,     0,
       0,     0,     0,     0,     0,    41,    36,    37,    38,    42,
      43,    39,    40,    44,     0,     0,     0,   444,   445,     0,
       0,    41,     0,     0,     0,    42,    43,     0,     0,    44,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
      39,    40,     0,   446,   447,     0,     0,    27,    28,    29,
      41,    30,    31,    32,    42,    43,     0,     0,    44,     0,
       0,     0,     0,   448,   449,   450,   451,     0,     0,     0,
       0,    33,    34,    35,    27,    28,    29,     0,    30,    31,
      32,     0,   452,   453,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,     0,    39,    40,     0,    33,    34,
      35,     0,     0,     0,     0,    41,    36,    37,    38,    42,
      43,    39,    40,    44,    27,    28,    29,     0,    30,    31,
      32,    41,     0,     0,     0,    42,    43,     0,     0,    44,
       0,     0,   446,     0,     0,     0,     0,     0,    33,    34,
      35,    27,    28,    29,     0,    30,    31,    32,     0,     0,
       0,     0,   448,     0,   450,     0,     0,     0,     0,     0,
     447,     0,     0,     0,     0,    33,    34,    35,     0,    73,
      74,   452,    75,    76,    77,    78,    79,    80,    81,     0,
     449,     0,   451,     0,     0,     0,     0,    27,    28,    29,
       0,    30,    31,    32,     0,    36,    37,    38,     0,   453,
      39,    40,     0,     0,     0,     0,     0,     0,   860,     0,
      41,    33,    34,    35,    42,    43,     0,     0,    44,     0,
       0,     0,    36,    37,    38,     0,     0,    39,    40,     0,
       0,     0,     0,     0,     0,   865,     0,    41,     0,     0,
       0,    42,    43,     0,     0,    44,     0,    27,    28,    29,
       0,    30,    31,    32,     0,     0,     0,     0,     0,     0,
     850,     0,    36,    37,    38,     0,     0,    39,    40,     0,
     851,    33,    34,    35,     0,     0,     0,    41,     0,   529,
     861,    42,    43,     0,     0,    44,     0,     0,     0,    36,
      37,    38,     0,     0,    39,    40,    27,    28,    29,     0,
      30,    31,    32,     0,    41,     0,   529,   861,    42,    43,
       0,     0,    44,    91,    92,    93,    94,    95,    96,    97,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      41,     0,     0,     0,    42,    43,    73,    74,    44,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
      39,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      41,  1022,     0,     0,    42,    43,    73,    74,    44,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,     0,     0,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,    39,
      40,     0,     0,     0,     0,     0,     0,     0,     0,   787,
       0,     0,     0,    42,    43,    73,    74,    44,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    73,    74,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,     0,
     112,     0,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,    73,    74,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,     0,    87,   156,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    73,    74,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,   157,
       0,     0,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,   282,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,   283,     0,
       0,    82,    83,    84,    85,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,   999,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1000,    87,   612,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    73,    74,     0,    75,    76,    77,    78,
      79,    80,    81,     0,     0,  1026,     0,     0,    82,    83,
      84,    85,    86,     0,     0,     0,    87,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
     213,     0,     0,    82,    83,    84,    85,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    73,    74,     0,
      75,    76,    77,    78,    79,    80,    81,     0,     0,   580,
       0,     0,    82,    83,    84,    85,    86,  1046,     0,     0,
       0,     0,     0,     0,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,   919,     0,     0,    82,
      83,    84,    85,    86,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    73,
      74,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,  1175,     0,     0,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1178,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    73,    74,  1047,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    73,    74,   975,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      73,    74,   976,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
      73,    74,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
      73,    74,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
       0,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,   977,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,   984,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    73,
      74,   985,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,     0,     0,   986,     0,    87,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,     0,     0,   988,     0,    87,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    73,    74,  1115,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    73,    74,  1127,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,  1176,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,  1177,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      73,    74,  1188,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
       0,     0,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,  1245,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,  1246,     0,    87,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    73,
      74,  1247,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    73,
      74,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    73,
      74,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    73,    74,  1248,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,     0,     0,  1269,     0,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,     0,     0,     0,     0,  1270,     0,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
       0,     0,    73,    74,   752,    75,    76,    77,    78,    79,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
    1005,     0,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
    1009,     0,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,    73,    74,
    1077,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,     0,     0,
       0,    87,     0,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,     0,    73,    74,  1092,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1106,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1107,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,    73,    74,  1109,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,     0,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
    1110,     0,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
    1170,     0,     0,     0,    87,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,    73,    74,
    1235,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    73,    74,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,     0,     0,
      87,     0,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,    73,    74,  1252,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    73,    74,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,     0,     0,     0,    87,     0,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,     0,     0,  1253,     0,     0,     0,    87,   110,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,     0,
       0,  1042,     0,     0,     0,     0,     0,    87,   110,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    73,
      74,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,     0,
       0,     0,     0,    87,   154,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,    87,   179,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,    87,   612,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    73,    74,     0,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,     0,     0,     0,    87,   914,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      73,    74,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
      73,    74,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
      73,    74,     0,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,   937,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1010,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,    87,  1118,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    73,    74,     0,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,     0,     0,    84,    85,    86,     0,     0,     0,    87,
    1128,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,     0,     0,    84,    85,
      86,    73,    74,     0,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
     578,     0,     0,     0,     0,     0,   394,     0,   395,     0,
     396,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   393,   738,     0,     0,
       0,     0,     0,   394,     0,   395,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,    89,    90,    91,    92,    93,    94,    95,
      96,    97,   251,     0,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,    95,
      96,    97,     0,     0,     1,     0,     0,     0,     0,     0,
       0,     0,   -11,   -11,   253,     0,   -11,   -11,   -11,     0,
       0,     0,   -11,     0,     0,   254,     0,     0,     0,     0,
       0,     0,  -279,     0,     0,     0,     0,   255,     0,     0,
       0,     0,   256,     0,     0,     0,   -11,   257,   258,   -11,
       0,     0,   -11,     0,     0,     0,   -11,     0,   -11,   -11,
     -11,   -11,   -11,   -11,   259,   -11,   -11,     0,     0,     0,
       0,   260,   261,   -11,   -11,   -11,   -11,     0,     0,   -11,
     -11,   -11,   -11,   -11,     0,     0,     0,   262,     0,     0,
       0,   -11,   -11,     0,   263,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   264,     0,   251,   -11,   252,     0,
       0,   -11,   -11,   -11,   -11,     0,     0,     0,     0,     0,
       0,     0,   265,   -11,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,   -11,   -11,   253,     0,
     -11,   -11,   -11,     0,     0,     0,   -11,     0,     0,   254,
       0,     0,     0,     0,     0,     0,  -278,     0,     0,     0,
       0,   255,     0,     0,     0,     0,   256,     0,     0,     0,
     -11,   257,   258,   -11,     0,     0,   -11,     0,     0,     0,
     -11,     0,   -11,   -11,   -11,   -11,   -11,   -11,   259,   -11,
     -11,     0,     0,     0,     0,   260,   261,   -11,   -11,   -11,
     -11,     0,     0,   -11,   -11,   -11,   -11,   -11,     0,     0,
       0,   262,     0,     0,     0,   -11,   -11,     0,   263,   -11,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   264,     0,
       0,   -11,     0,     0,     0,   -11,   -11,   -11,   -11,   349,
     350,     0,     0,   351,   352,   353,   265,   -11,     0,   354,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   355,     0,     0,   356,     0,     0,   357,
       0,     0,     0,   358,     0,   359,   360,   361,   362,   363,
     364,     0,   365,   366,     0,     0,     0,     0,     0,     0,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   374,
     375,     0,     0,     0,     0,     0,     0,     0,   232,   233,
       0,     0,   376,   377,   378,   379,   234,   235,   236,   237,
     238,     0,     0,     0,   239,   393,   739,     0,   240,   241,
     380,   381,   394,     0,   395,     0,   396,     0,     0,     0,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   393,     0,     0,     0,     0,     0,     0,   394,
     740,   395,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     741,   742,   743,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   393,
       0,     0,     0,     0,     0,     0,   394,     0,   395,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415
};

static const yytype_int16 yycheck[] =
{
      21,   266,   605,     0,   349,   523,   677,   255,     5,   321,
     355,   376,   388,   384,   701,   702,   324,   421,   166,     3,
      41,   384,     3,    44,    33,    34,    35,    36,    37,    38,
      39,    40,     3,    42,    43,   697,    44,   254,     3,   256,
       3,     3,   493,   494,     3,   166,   261,    55,     1,   197,
       3,     3,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,   225,    99,   294,
       1,     1,     3,     3,     1,   688,     3,     1,     3,   110,
      98,     1,   113,     3,     8,   288,   289,   290,   291,   713,
     164,     3,   718,   320,   112,   485,   486,   487,   488,   489,
     490,     1,     1,     3,     3,     1,    10,     3,     3,    23,
     368,   369,    24,     1,    23,     3,     4,     5,     3,     7,
       8,     9,     3,   154,     3,   156,     3,   158,     3,     1,
     357,     3,     4,     5,    48,     7,     8,     9,   365,    49,
     308,   148,     3,    82,    23,     3,   818,   163,   179,   165,
       3,     4,     5,     3,     7,     8,     9,     3,    49,   384,
      69,   575,    24,   577,   578,     3,     3,   522,   199,     3,
       8,     9,    68,     3,     8,     9,   848,   104,     8,     9,
     174,   567,   176,   796,   104,   560,   567,   800,   104,     3,
     163,   172,   565,   224,     8,     9,   163,    93,    94,   174,
      23,   208,   174,   210,   104,   174,    21,    22,   175,     0,
     172,   174,   165,   143,   144,   145,   146,   147,   114,     3,
       0,    49,   104,   176,   163,    48,     3,     1,   167,     3,
       4,     5,    23,     7,     8,     9,   166,   166,   168,     0,
     150,   166,   166,   174,   174,   172,    23,   169,   506,   873,
     876,   282,   172,    27,    28,    29,   172,     3,    23,   150,
     164,   268,    23,   158,   467,   468,   469,   166,     3,    43,
     254,   166,   172,   304,     3,    49,   172,   599,   166,   601,
     598,   166,   600,   168,   163,   166,   174,   168,   167,   166,
     172,   166,   460,   104,   166,   174,    87,   176,     3,   912,
      84,   163,   174,   321,    69,   166,   324,   229,   166,   231,
     568,   983,   984,   166,   986,   670,   166,   544,   104,   104,
     166,   174,   150,   550,   308,   739,    87,   150,   166,   166,
      12,     3,   166,   560,     6,   150,   166,    83,    20,   564,
     565,     1,    23,     3,   957,   806,   807,  1130,    83,   724,
     141,  1134,   166,  1136,  1137,    17,    18,    19,   616,   617,
     618,   172,   104,   285,     3,   349,   569,   163,     3,     4,
       5,   355,     7,     8,     9,    23,   150,   151,   152,   153,
     141,   608,   156,   157,     1,   150,   172,   172,    69,   163,
     150,   166,   166,   165,   163,    23,   170,   171,   167,   624,
     174,    95,   627,   163,   176,   795,   437,   797,   798,   799,
    1082,   801,    66,    67,     7,   433,    76,   435,    66,    67,
      48,   439,   440,   441,   442,   443,    86,     5,  1100,    83,
     172,   462,  1045,    89,    67,    83,    53,  1050,  1051,  1052,
     168,    76,  1055,  1226,   163,   163,  1229,    64,   479,   167,
      83,    86,   470,   109,    70,   111,   175,    73,   166,    62,
      63,   499,    95,   501,   172,   484,   485,   486,   487,   488,
     489,   490,   128,   828,   458,   733,   460,   167,   736,  1176,
    1177,    65,     3,    67,    12,    88,    89,  1159,  1160,   105,
     107,   529,    20,  1165,   156,   157,   158,   159,   160,    83,
    1172,  1173,     3,  1116,   729,   108,   109,   110,   111,     3,
      63,    95,   163,   531,    62,   164,   167,   134,   135,   136,
     137,   138,   139,   140,   127,   128,   533,   176,   512,   513,
     166,   166,   163,   165,   518,   519,    89,   521,   522,   174,
      88,   525,   164,   163,   175,   563,   530,   166,   532,   166,
     172,   157,  1080,  1234,  1235,   175,   109,   166,   111,   717,
     108,    12,   110,   172,   244,  1093,   246,   247,   163,    20,
     925,   177,   178,   179,   605,   128,   163,   156,   157,   127,
    1193,   612,   164,  1196,   163,    10,    11,    84,   175,   607,
     270,   166,    17,    18,    19,   163,    88,   172,   629,   630,
       5,   176,   633,   634,  1143,   623,  1145,   175,  1147,   812,
     813,   814,   163,   163,   633,   634,   108,   167,   110,   513,
     651,   165,   653,   978,   518,   644,    72,   521,    74,     3,
     985,   525,   987,   988,     3,   127,   163,   317,   318,   319,
     167,   672,   673,   674,     7,     8,  1084,   665,   923,  1087,
     167,  1089,   683,     3,   685,   686,   687,   688,   164,   165,
     691,   165,   166,   163,   671,   976,   977,   167,   172,   700,
     701,   702,   176,   704,   705,   682,   163,   163,   709,   165,
     167,   699,   172,   690,   163,   163,   670,   163,   167,   167,
     370,   167,   163,   677,     3,   163,   167,   165,   973,   156,
     157,   158,   159,   160,   962,   385,   964,   982,   966,   389,
     163,   163,     3,   697,   167,   167,   747,   920,   921,   922,
       1,   163,     3,     4,     5,   167,     7,     8,     9,   154,
     155,   156,   157,   158,   159,   160,   767,   163,   769,   172,
     163,   167,    67,   176,   167,   163,    27,    28,    29,   167,
     163,   163,     3,   163,   167,   167,   787,   167,   961,   163,
     963,   165,   965,   794,   163,   796,   773,   163,   167,   800,
    1028,   167,  1030,   163,  1032,   175,   795,   167,   797,   798,
     799,     1,   801,     3,     4,     5,   770,     7,     8,     9,
     172,     3,   163,   824,   176,    76,   167,   163,   163,   163,
     112,   167,   167,   167,  1079,    86,   837,    27,    28,    29,
    1068,   163,  1070,   172,  1072,   167,   165,   176,   266,   850,
     851,   165,   270,    43,  1027,   164,  1029,    68,  1031,    49,
      71,   862,   863,   104,   818,   866,   867,   165,   166,   110,
     111,   701,   702,    84,   828,   995,   996,   118,   119,   120,
     121,   122,    93,    94,    95,   126,   158,   159,   160,   130,
     131,  1119,   166,  1121,   848,  1123,     3,   163,   899,   165,
     151,   152,   153,   114,     3,   156,   157,   861,   257,   258,
     259,   912,     1,   914,     3,   166,     1,   165,     3,   170,
     171,   204,   205,   174,     1,   164,     3,     1,   929,     3,
       4,     5,    52,     7,     8,     9,   937,  1262,   939,   940,
       1,     1,     3,     3,  1269,  1270,   173,   163,   925,   165,
     163,   165,   165,    27,    28,    29,   957,   163,   104,   165,
     150,   151,   152,   153,   168,   954,   156,   157,   163,    43,
     165,   925,    95,   163,    53,    49,   166,   163,   979,   165,
     170,   171,   645,   646,   174,    64,   163,   163,   165,   165,
       1,   163,     3,   165,   995,   996,     3,   163,   999,   165,
    1001,  1002,   163,   166,   165,  1006,   163,   166,   165,  1010,
     163,   163,   165,   165,  1015,    51,    67,   994,    66,    67,
      68,   418,   419,    71,   978,   163,   165,   165,   107,   983,
     984,   985,   986,   987,   988,    83,    84,   163,   163,   165,
     165,  1042,   172,   167,  1045,    93,    94,    95,   166,  1050,
    1051,  1052,  1053,   166,  1055,   134,   135,   136,   137,   138,
     139,   140,   163,   163,   165,   165,   114,   163,  1057,   165,
    1059,   163,  1061,   165,  1063,   166,   150,   151,   152,   153,
     166,   168,   156,   157,   166,     3,    50,   166,   163,   163,
     165,     3,   166,  1047,    53,   166,   170,   171,   166,   163,
     174,   165,   164,   166,   163,   163,   165,   165,    95,   163,
     163,   165,   165,   165,   163,  1116,   165,  1118,   163,     1,
     165,     3,     4,   110,   111,   166,   166,  1128,  1082,   166,
     165,   118,   119,   120,   121,   122,   163,   166,   165,   126,
     163,   166,   165,   130,   131,   163,  1100,   165,    30,   164,
       3,    33,    34,   163,    54,   165,    38,    39,    40,   163,
      42,   165,   163,    45,   165,   163,   163,   165,   165,  1170,
     165,   165,     3,     3,     3,    57,    58,    59,    60,  1133,
    1157,     3,    64,   165,   163,   163,   163,   163,   172,     3,
     165,   163,  1193,   166,  1171,  1196,   167,   167,   164,  1200,
    1201,  1202,  1203,   164,   167,  1159,  1160,     1,   165,     3,
       4,  1165,   110,   111,    96,    97,  1217,  1218,  1172,  1173,
     118,   119,   120,   121,   122,   167,   167,   167,   126,    23,
     167,   166,   130,   131,   166,  1224,    30,   166,   166,    33,
      34,   166,   166,   125,    38,    39,    40,   129,    42,   166,
     166,    45,    23,    52,   165,   165,   164,   163,   165,   164,
    1261,   165,  1263,    57,    58,    59,    60,     3,   150,   165,
      64,   163,    66,    67,    68,   165,   165,    71,   165,   165,
    1234,  1235,   167,   165,   164,     3,   168,   169,   148,    83,
      84,   163,   174,   163,   163,     1,   163,     3,     4,    93,
      94,    95,    96,    97,   163,   163,   163,     3,  1262,   165,
       5,     3,   164,     3,   165,  1269,  1270,   164,     3,   165,
     114,   176,     3,     3,    30,     3,   165,    33,    34,   165,
     165,   125,    38,    39,    40,   129,    42,   165,   150,    45,
     165,   167,   166,   176,   165,   165,   165,   165,   165,   176,
     166,    57,    58,    59,    60,   148,   167,   167,    64,     1,
     167,     3,     4,     5,   167,     7,     8,     9,   166,     1,
     167,     3,     4,   148,   168,   169,   163,   173,   167,    48,
     174,   172,   167,   164,   167,    27,    28,    29,   167,   167,
      96,    97,    69,   167,   167,   163,   166,   166,    30,   164,
     163,    33,    34,   165,   165,   163,    38,    39,    40,   163,
      42,   163,    55,    45,     1,     5,     3,     4,     5,   125,
       7,     8,     9,   129,   165,    57,    58,    59,    60,    50,
     165,   165,    64,   163,    76,   167,   167,   165,   167,    47,
      27,    28,    29,     1,    86,     3,     4,     5,   167,     7,
       8,     9,   167,   167,   167,    47,    43,   173,   165,   165,
     163,   163,   168,   169,    96,    97,   165,   150,   174,    27,
      28,    29,   169,   167,   167,     1,   167,     3,     4,     5,
     167,     7,     8,     9,   173,    43,   276,    23,   167,   165,
     165,   165,   165,   125,   165,   167,   982,   129,   164,   167,
     167,    27,    28,    29,   167,   167,   167,   167,   164,   151,
     152,   153,   165,   165,   156,   157,   158,    43,   507,     1,
       5,     3,     4,     5,   166,     7,     8,     9,   170,   171,
     474,   925,   174,   770,  1016,   268,   168,   169,   208,   733,
     750,   712,   174,   285,   779,    27,    28,    29,   206,   653,
     623,   706,   785,   339,     1,   785,     3,     4,     5,   631,
       7,     8,     9,   150,   151,   152,   153,   817,   391,   156,
     157,   419,   896,   418,    -1,   164,   163,    -1,    -1,   166,
      27,    28,    29,   170,   171,    -1,    -1,   174,    -1,    -1,
      -1,    -1,   150,   151,   152,   153,    -1,    -1,   156,   157,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,   166,    -1,
      -1,    -1,   170,   171,    -1,    -1,   174,     3,     4,     5,
      -1,     7,     8,     9,   150,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
     166,    27,    28,    29,   170,   171,    -1,    -1,   174,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,   166,   167,    -1,    -1,   170,   171,
      -1,    -1,   174,    -1,     1,    -1,     3,     4,     5,    -1,
       7,     8,     9,    -1,   151,   152,   153,    -1,    -1,   156,
     157,    -1,     3,     4,     5,    -1,     7,     8,     9,   166,
      27,    28,    29,   170,   171,    -1,     1,   174,     3,     4,
       5,    -1,     7,     8,     9,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,     1,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    27,    28,    29,   170,   171,    -1,    -1,   174,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,   166,     7,     8,     9,   170,
     171,    -1,    -1,   174,    -1,   176,    -1,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,   166,
     151,   152,   153,   170,   171,   156,   157,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,   151,   152,   153,   170,
     171,   156,   157,   174,    -1,    -1,    -1,    62,    63,    -1,
      -1,   166,    -1,    -1,    -1,   170,   171,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    88,    89,    -1,    -1,     3,     4,     5,
     166,     7,     8,     9,   170,   171,    -1,    -1,   174,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,    -1,    -1,    -1,
      -1,    27,    28,    29,     3,     4,     5,    -1,     7,     8,
       9,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,   156,   157,    -1,    27,    28,
      29,    -1,    -1,    -1,    -1,   166,   151,   152,   153,   170,
     171,   156,   157,   174,     3,     4,     5,    -1,     7,     8,
       9,   166,    -1,    -1,    -1,   170,   171,    -1,    -1,   174,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,     3,     4,     5,    -1,     7,     8,     9,    -1,    -1,
      -1,    -1,   108,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    10,
      11,   127,    13,    14,    15,    16,    17,    18,    19,    -1,
     109,    -1,   111,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,   151,   152,   153,    -1,   128,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
     166,    27,    28,    29,   170,   171,    -1,    -1,   174,    -1,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,   166,    -1,    -1,
      -1,   170,   171,    -1,    -1,   174,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,   151,   152,   153,    -1,    -1,   156,   157,    -1,
      86,    27,    28,    29,    -1,    -1,    -1,   166,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,     3,     4,     5,    -1,
       7,     8,     9,    -1,   166,    -1,   168,   169,   170,   171,
      -1,    -1,   174,   154,   155,   156,   157,   158,   159,   160,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,   170,   171,    10,    11,   174,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,   167,    -1,    -1,   170,   171,    10,    11,   174,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,   170,   171,    10,    11,   174,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
     174,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,   173,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,   173,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,   173,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    -1,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
     173,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,   173,
      -1,    -1,    25,    26,    27,    28,    29,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,   173,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,   173,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    10,    11,   163,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    10,    11,   167,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      10,    11,   167,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    10,
      11,   167,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    10,    11,   167,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    10,    11,   167,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      10,    11,   167,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    10,
      11,   167,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    10,    11,   167,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    10,    11,   165,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    10,    11,
     165,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,    10,    11,   165,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    10,    11,   165,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    10,    11,
     165,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    10,    11,   165,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
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
      25,    26,    27,    28,    29,    -1,    -1,    -1,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   149,   150,    -1,    -1,
      -1,    -1,    -1,   156,    -1,   158,    -1,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   152,   153,   154,   155,   156,   157,   158,
     159,   160,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    36,    37,    -1,
      -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      -1,    -1,    71,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,    -1,     1,   126,     3,    -1,
      -1,   130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    -1,    -1,    71,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,    -1,    -1,
      -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,   131,   132,   133,    31,
      32,    -1,    -1,    35,    36,    37,   141,   142,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    -1,    -1,    71,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      -1,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,   126,   149,   150,    -1,   130,   131,
     132,   133,   156,    -1,   158,    -1,   160,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   149,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   149,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   158,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195
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
     167,   227,   280,   232,   232,   167,   225,   166,   225,   296,
     225,   228,   225,   228,   148,   163,   163,   163,   163,   163,
     163,   163,   228,   298,   298,     3,    12,    20,   215,   288,
       1,   288,    66,    67,    83,   203,   307,   308,   309,   165,
     165,     5,   227,   165,   164,     3,   302,    48,   200,   225,
     225,   225,   165,   165,     1,   234,   242,   164,     3,    69,
       1,   225,   165,   225,   225,     1,   225,   215,   200,     1,
      76,    86,   158,   223,   224,   225,   175,   306,   227,   225,
      97,   169,   212,   222,   225,    97,   212,   222,   225,   225,
     225,   285,   225,   270,   267,   271,   265,   262,   266,   286,
     165,   165,   232,   165,     3,   258,   259,     3,   235,   259,
     165,   165,   235,   177,   178,   179,   150,   165,   225,   166,
     273,   165,   165,   165,   165,   165,   167,   167,   167,   167,
     167,   167,   163,   167,   150,   175,   225,   225,   243,   173,
      66,    67,    83,   200,   229,   230,   165,   165,   165,   166,
     279,   167,   167,   165,   165,   290,   293,   150,   225,   148,
     148,   225,   299,   215,   299,   299,   299,   215,   299,   173,
      12,    20,    12,    20,   172,   295,   295,   163,   167,   167,
     167,   104,   283,   104,   283,   104,   283,   307,   306,   163,
     167,   225,    48,   303,   302,   167,   167,   167,   167,   164,
     225,    69,   304,   167,   167,   167,   167,   167,   167,   167,
     306,   167,   225,   225,   167,    82,   163,   167,   167,   150,
     173,   166,   166,   225,   225,   165,   166,   225,   225,   165,
     150,   271,   266,   165,   165,   164,   163,   165,   165,   165,
     165,   321,   167,   225,   215,   225,   173,   104,   283,   104,
     283,   104,   283,   203,   229,   302,     1,   167,   225,   165,
     165,   225,   163,   225,   225,   163,     3,   163,   300,   301,
     163,   163,   163,   163,   167,   163,     3,   166,     3,   166,
       3,   166,     3,   166,   228,   167,   167,   215,   283,   235,
     283,   235,   283,   235,    55,     5,   165,   165,   203,   204,
     205,     1,    43,   206,   207,   227,     1,   207,     1,   207,
     302,   225,   165,   205,   306,   306,   302,   306,   302,   302,
     200,   224,   224,   165,   225,   225,   165,   165,   225,   165,
     165,   225,   225,   258,   165,   167,   163,   167,   150,   283,
     235,   283,   235,   283,   235,    50,   167,   167,   150,   225,
     215,   234,   167,   163,   215,   215,   215,   225,   165,   215,
     167,   228,   167,   228,   167,   228,   167,   228,   173,   235,
     165,   235,   165,   235,   165,   167,   203,   305,    49,   150,
     150,   306,     1,    49,   206,   150,    49,    49,    49,    49,
     165,   305,    47,    47,   306,   173,   167,   167,   173,   215,
     225,   235,   165,   235,   165,   235,   165,   225,   167,   300,
     165,   234,   300,   163,   300,   300,   163,    21,    22,   150,
     292,   292,   292,   292,   165,   165,   165,   165,    48,   306,
     306,   306,     1,   225,    69,   306,   306,   222,   222,   167,
     165,   165,   165,   167,   172,   167,   215,   167,   167,   215,
     225,   225,   225,   225,   165,   165,   225,   225,   165,   228,
     165,   300,   165,   165,   300,   167,   167,   167,   167,   242,
       1,   242,   165,   165,   173,   167,   167,   167,   167,   167,
     167,   164,   167,   164,   165,   165,   225,   302,   225,   167,
     167,   302,   302
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
#line 1136 "parse.y"
    { lgate*tmp = new lgate;
		  svector<PExpr*>*rng = (yyvsp[(2) - (5)].exprs);
		  tmp->name = (yyvsp[(1) - (5)].text);
		  tmp->parms_by_name = (yyvsp[(4) - (5)].named_pexprs);
		  tmp->range[0] = (*rng)[0];
		  tmp->range[1] = (*rng)[1];
		  tmp->file  = (yylsp[(1) - (5)]).text;
		  tmp->lineno = (yylsp[(1) - (5)]).first_line;
		  delete (yyvsp[(1) - (5)].text);
		  delete rng;
		  (yyval.gate) = tmp;
		;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1151 "parse.y"
    { lgate*tmp = new lgate;
		  tmp->name = "";
		  tmp->parms = (yyvsp[(2) - (3)].exprs);
		  tmp->file  = (yylsp[(1) - (3)]).text;
		  tmp->lineno = (yylsp[(1) - (3)]).first_line;
		  (yyval.gate) = tmp;
		;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1162 "parse.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 1178 "parse.y"
    { lgate*tmp = new lgate;
		  tmp->name = (yyvsp[(1) - (4)].text);
		  tmp->parms_by_name = (yyvsp[(3) - (4)].named_pexprs);
		  tmp->file  = (yylsp[(1) - (4)]).text;
		  tmp->lineno = (yylsp[(1) - (4)]).first_line;
		  delete (yyvsp[(1) - (4)].text);
		  (yyval.gate) = tmp;
		;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1190 "parse.y"
    { svector<lgate>*tmp1 = (yyvsp[(1) - (3)].gates);
		  lgate*tmp2 = (yyvsp[(3) - (3)].gate);
		  svector<lgate>*out = new svector<lgate> (*tmp1, *tmp2);
		  delete tmp1;
		  delete tmp2;
		  (yyval.gates) = out;
		;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1198 "parse.y"
    { svector<lgate>*tmp = new svector<lgate>(1);
		  (*tmp)[0] = *(yyvsp[(1) - (1)].gate);
		  delete (yyvsp[(1) - (1)].gate);
		  (yyval.gates) = tmp;
		;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1206 "parse.y"
    { (yyval.gatetype) = PGBuiltin::AND; ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1207 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NAND; ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1208 "parse.y"
    { (yyval.gatetype) = PGBuiltin::OR; ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1209 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOR; ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1210 "parse.y"
    { (yyval.gatetype) = PGBuiltin::XOR; ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1211 "parse.y"
    { (yyval.gatetype) = PGBuiltin::XNOR; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1212 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUF; ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1213 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUFIF0; ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1214 "parse.y"
    { (yyval.gatetype) = PGBuiltin::BUFIF1; ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1215 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOT; ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1216 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOTIF0; ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1217 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NOTIF1; ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1218 "parse.y"
    { (yyval.gatetype) = PGBuiltin::NMOS; ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1219 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RNMOS; ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1220 "parse.y"
    { (yyval.gatetype) = PGBuiltin::PMOS; ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1221 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RPMOS; ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1222 "parse.y"
    { (yyval.gatetype) = PGBuiltin::CMOS; ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1223 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RCMOS; ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1224 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRAN; ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1225 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRAN; ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1226 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRANIF0; ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1227 "parse.y"
    { (yyval.gatetype) = PGBuiltin::TRANIF1; ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1228 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRANIF0; ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1229 "parse.y"
    { (yyval.gatetype) = PGBuiltin::RTRANIF1; ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1230 "parse.y"
    { (yyval.gatetype) = PGBuiltin::PLL; ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1239 "parse.y"
    { (yyval.hier) = new hname_t((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1243 "parse.y"
    { hname_t * tmp = (yyvsp[(1) - (3)].hier);
		  tmp->append((yyvsp[(3) - (3)].text));
		  delete (yyvsp[(3) - (3)].text);
		  (yyval.hier) = tmp;
		;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1255 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1261 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1287 "parse.y"
    { svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].mport);
		  (yyval.mports) = tmp;
		;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1293 "parse.y"
    { svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*(yyvsp[(1) - (3)].mports), (yyvsp[(3) - (3)].mport));
		  delete (yyvsp[(1) - (3)].mports);
		  (yyval.mports) = tmp;
		;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1302 "parse.y"
    { svector<Module::port_t*>*tmp
			  = new svector<Module::port_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].mport);
		  (yyval.mports) = tmp;
		;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1308 "parse.y"
    { svector<Module::port_t*>*tmp
			= new svector<Module::port_t*>(*(yyvsp[(1) - (3)].mports), (yyvsp[(3) - (3)].mport));
		  delete (yyvsp[(1) - (3)].mports);
		  (yyval.mports) = tmp;
		;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1314 "parse.y"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 1336 "parse.y"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 1351 "parse.y"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 1366 "parse.y"
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
#line 1381 "parse.y"
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

  case 202:

/* Line 1455 of yacc.c  */
#line 1396 "parse.y"
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

  case 203:

/* Line 1455 of yacc.c  */
#line 1420 "parse.y"
    { (yyval.nettype) = (yyvsp[(1) - (1)].nettype); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1421 "parse.y"
    { (yyval.nettype) = NetNet::IMPLICIT; ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1424 "parse.y"
    { (yyval.flag) = true; ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1424 "parse.y"
    {(yyval.flag) = false; ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1431 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (1)].hier));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1438 "parse.y"
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

  case 209:

/* Line 1455 of yacc.c  */
#line 1453 "parse.y"
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

  case 210:

/* Line 1455 of yacc.c  */
#line 1464 "parse.y"
    { PEConcat*tmp = new PEConcat(*(yyvsp[(2) - (3)].exprs));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1476 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (1)].hier));
		  tmp->set_file((yylsp[(1) - (1)]).text);
		  tmp->set_lineno((yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1483 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (4)].hier));
		  tmp->msb_ = (yyvsp[(3) - (4)].expr);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(1) - (4)].hier);

		  (yyval.expr) = tmp;
		;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1492 "parse.y"
    { PEIdent*tmp = new PEIdent(*(yyvsp[(1) - (6)].hier));
		  tmp->msb_ = (yyvsp[(3) - (6)].expr);
		  tmp->lsb_ = (yyvsp[(5) - (6)].expr);
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  delete (yyvsp[(1) - (6)].hier);
		  (yyval.expr) = tmp;
		;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1501 "parse.y"
    { PEConcat*tmp = new PEConcat(*(yyvsp[(2) - (3)].exprs));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].exprs);
		  (yyval.expr) = tmp;
		;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1511 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(2);
		  (*tmp)[0] = (yyvsp[(1) - (3)].expr);
		  (*tmp)[1] = (yyvsp[(3) - (3)].expr);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1520 "parse.y"
    { svector<PExpr*>*tmp = new svector<PExpr*>(*(yyvsp[(1) - (3)].exprs), *(yyvsp[(3) - (3)].exprs));
		  delete (yyvsp[(1) - (3)].exprs);
		  delete (yyvsp[(3) - (3)].exprs);
		  (yyval.exprs) = tmp;
		;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1526 "parse.y"
    { (yyval.exprs) = (yyvsp[(1) - (1)].exprs); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1535 "parse.y"
    { pform_startmodule((yyvsp[(3) - (3)].text), (yylsp[(2) - (3)]).text, (yylsp[(2) - (3)]).first_line, (yyvsp[(1) - (3)].named_pexprs)); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1538 "parse.y"
    { pform_module_set_ports((yyvsp[(6) - (7)].mports)); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1541 "parse.y"
    { pform_endmodule((yyvsp[(3) - (10)].text));
		  delete (yyvsp[(3) - (10)].text);
		;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1550 "parse.y"
    { (yyval.range_delay).range = (yyvsp[(1) - (2)].exprs); (yyval.range_delay).delay = (yyvsp[(2) - (2)].exprs); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1554 "parse.y"
    { (yyval.mports) = (yyvsp[(2) - (3)].mports); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1555 "parse.y"
    { (yyval.mports) = (yyvsp[(2) - (3)].mports); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1556 "parse.y"
    { (yyval.mports) = 0; ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1575 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), (yyvsp[(4) - (6)].range_delay).range, (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype),
				 NetNet::NOT_A_PORT, (yyvsp[(1) - (6)].named_pexprs));
		  if ((yyvsp[(4) - (6)].range_delay).delay != 0) {
			yyerror((yylsp[(4) - (6)]), "sorry: net delays not supported.");
			delete (yyvsp[(4) - (6)].range_delay).delay;
		  }
		  if ((yyvsp[(1) - (6)].named_pexprs)) delete (yyvsp[(1) - (6)].named_pexprs);
		;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1584 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), (yyvsp[(4) - (6)].range_delay).range, (yyvsp[(3) - (6)].flag), (yyvsp[(4) - (6)].range_delay).delay,
				 str_strength, (yyvsp[(5) - (6)].net_decl_assign), (yyvsp[(2) - (6)].nettype));
		  if ((yyvsp[(1) - (6)].named_pexprs)) {
			yyerror((yylsp[(3) - (6)]), "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete (yyvsp[(1) - (6)].named_pexprs);
		  }
		;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1593 "parse.y"
    { pform_makewire((yylsp[(2) - (6)]), 0, (yyvsp[(3) - (6)].flag), 0, (yyvsp[(4) - (6)].drive), (yyvsp[(5) - (6)].net_decl_assign), (yyvsp[(2) - (6)].nettype));
		  if ((yyvsp[(1) - (6)].named_pexprs)) {
			yyerror((yylsp[(4) - (6)]), "sorry: Attributes not supported "
				"on net declaration assignments.");
			delete (yyvsp[(1) - (6)].named_pexprs);
		  }
		;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1601 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "sorry: trireg nets not supported.");
		  delete (yyvsp[(3) - (5)].range_delay).range;
		  delete (yyvsp[(3) - (5)].range_delay).delay;
		;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1607 "parse.y"
    { pform_set_port_type((yylsp[(1) - (5)]), (yyvsp[(4) - (5)].perm_strings), (yyvsp[(3) - (5)].range_delay).range, (yyvsp[(2) - (5)].flag), (yyvsp[(1) - (5)].porttype));
		;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1615 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), (yyvsp[(1) - (6)].porttype), 0);
		;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1619 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::POUTPUT, 0);
		;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1627 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::PINPUT, 0);
		  yyerror((yylsp[(2) - (6)]), "error: reg variables cannot be inputs.");
		;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1632 "parse.y"
    { pform_makewire((yylsp[(1) - (6)]), (yyvsp[(4) - (6)].exprs), (yyvsp[(3) - (6)].flag), (yyvsp[(5) - (6)].perm_strings), (yyvsp[(2) - (6)].nettype), NetNet::PINOUT, 0);
		  yyerror((yylsp[(2) - (6)]), "error: reg variables cannot be inouts.");
		;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1637 "parse.y"
    { yyerror((yylsp[(3) - (5)]), "error: Invalid variable list"
			  " in port declaration.");
		  if ((yyvsp[(3) - (5)].range_delay).range) delete (yyvsp[(3) - (5)].range_delay).range;
		  if ((yyvsp[(3) - (5)].range_delay).delay) delete (yyvsp[(3) - (5)].range_delay).delay;
		  yyerrok;
		;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1648 "parse.y"
    { ; ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1654 "parse.y"
    { pform_make_events((yyvsp[(2) - (3)].perm_strings), (yylsp[(1) - (3)]).text, (yylsp[(1) - (3)]).first_line);
		;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1661 "parse.y"
    { pform_makegates((yyvsp[(2) - (4)].gatetype), str_strength, 0, (yyvsp[(3) - (4)].gates), (yyvsp[(1) - (4)].named_pexprs));
		;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1665 "parse.y"
    { pform_makegates((yyvsp[(2) - (5)].gatetype), str_strength, (yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].gates), (yyvsp[(1) - (5)].named_pexprs));
		;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1669 "parse.y"
    { pform_makegates((yyvsp[(2) - (5)].gatetype), (yyvsp[(3) - (5)].drive), 0, (yyvsp[(4) - (5)].gates), (yyvsp[(1) - (5)].named_pexprs));
		;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1673 "parse.y"
    { pform_makegates((yyvsp[(2) - (6)].gatetype), (yyvsp[(3) - (6)].drive), (yyvsp[(4) - (6)].exprs), (yyvsp[(5) - (6)].gates), (yyvsp[(1) - (6)].named_pexprs));
		;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1680 "parse.y"
    { pform_makegates(PGBuiltin::PULLUP, pull_strength, 0,
				  (yyvsp[(2) - (3)].gates), 0);
		;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1684 "parse.y"
    { pform_makegates(PGBuiltin::PULLDOWN, pull_strength,
				  0, (yyvsp[(2) - (3)].gates), 0);
		;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1689 "parse.y"
    { pform_makegates(PGBuiltin::PULLUP, (yyvsp[(3) - (6)].drive), 0, (yyvsp[(5) - (6)].gates), 0);
		;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1693 "parse.y"
    { pform_makegates(PGBuiltin::PULLDOWN, (yyvsp[(3) - (6)].drive), 0, (yyvsp[(5) - (6)].gates), 0);
		;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1701 "parse.y"
    { perm_string tmp1 = lex_strings.make((yyvsp[(1) - (4)].text));
		  pform_make_modgates(tmp1, (yyvsp[(2) - (4)].parmvalue), (yyvsp[(3) - (4)].gates));
		  delete (yyvsp[(1) - (4)].text);
		;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1707 "parse.y"
    { yyerror((yylsp[(1) - (4)]), "error: Invalid module instantiation");
		;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1715 "parse.y"
    { pform_make_pgassign_list((yyvsp[(4) - (5)].exprs), (yyvsp[(3) - (5)].exprs), (yyvsp[(2) - (5)].drive), (yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1718 "parse.y"
    { pform_make_pmemassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1721 "parse.y"
    { pform_make_psquareassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1724 "parse.y"
    { pform_make_pmultassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1727 "parse.y"
    { pform_make_pmultaddassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1730 "parse.y"
    { pform_make_pmultaccumassign((yyvsp[(4) - (6)].exprs), (yylsp[(1) - (6)]).text, (yylsp[(1) - (6)]).first_line); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1735 "parse.y"
    { PProcess*tmp = pform_make_behavior(PProcess::PR_ALWAYS,
						     (yyvsp[(3) - (3)].statement), (yyvsp[(1) - (3)].named_pexprs));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1741 "parse.y"
    { PProcess*tmp = pform_make_behavior(PProcess::PR_INITIAL,
						     (yyvsp[(3) - (3)].statement), (yyvsp[(1) - (3)].named_pexprs));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1753 "parse.y"
    { pform_push_scope((yyvsp[(2) - (3)].text)); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1756 "parse.y"
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

  case 265:

/* Line 1455 of yacc.c  */
#line 1773 "parse.y"
    { pform_push_scope((yyvsp[(3) - (4)].text)); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1776 "parse.y"
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

  case 267:

/* Line 1455 of yacc.c  */
#line 1791 "parse.y"
    { /* empty lists are legal syntax. */ ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1794 "parse.y"
    {
		;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1798 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in specify block");
		  yyerrok;
		;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1807 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: invalid module item. "
			  "Did you forget an initial or always?");
		  yyerrok;
		;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1813 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: syntax error in left side "
			  "of continuous assignment.");
		  yyerrok;
		;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1819 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: syntax error in "
			  "continuous assignment");
		  yyerrok;
		;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1825 "parse.y"
    { yyerror((yylsp[(1) - (3)]), "error: I give up on this "
			  "function definition.");
		  yyerrok;
		;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1834 "parse.y"
    { perm_string tmp3 = lex_strings.make((yyvsp[(3) - (9)].text));
		  perm_string tmp5 = lex_strings.make((yyvsp[(5) - (9)].text));
		  pform_set_attrib(tmp3, tmp5, (yyvsp[(7) - (9)].text));
		  delete (yyvsp[(3) - (9)].text);
		  delete (yyvsp[(5) - (9)].text);
		;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1841 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: Malformed $attribute parameter list."); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1865 "parse.y"
    { net_decl_assign_t*tmp = new net_decl_assign_t;
		  tmp->next = tmp;
		  tmp->name = (yyvsp[(1) - (3)].text);
		  tmp->expr = (yyvsp[(3) - (3)].expr);
		  (yyval.net_decl_assign) = tmp;
		;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1875 "parse.y"
    { net_decl_assign_t*tmp = (yyvsp[(1) - (3)].net_decl_assign);
		  (yyvsp[(3) - (3)].net_decl_assign)->next = tmp->next;
		  tmp->next = (yyvsp[(3) - (3)].net_decl_assign);
		  (yyval.net_decl_assign) = tmp;
		;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1881 "parse.y"
    { (yyval.net_decl_assign) = (yyvsp[(1) - (1)].net_decl_assign);
		;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1886 "parse.y"
    { (yyval.nettype) = NetNet::WIRE; ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1887 "parse.y"
    { (yyval.nettype) = NetNet::TRI; ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1888 "parse.y"
    { (yyval.nettype) = NetNet::TRI1; ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1889 "parse.y"
    { (yyval.nettype) = NetNet::SUPPLY0; ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1890 "parse.y"
    { (yyval.nettype) = NetNet::WAND; ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1891 "parse.y"
    { (yyval.nettype) = NetNet::TRIAND; ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1892 "parse.y"
    { (yyval.nettype) = NetNet::TRI0; ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1893 "parse.y"
    { (yyval.nettype) = NetNet::SUPPLY1; ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1894 "parse.y"
    { (yyval.nettype) = NetNet::WOR; ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1895 "parse.y"
    { (yyval.nettype) = NetNet::TRIOR; ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1899 "parse.y"
    { (yyval.nettype) = NetNet::REG; ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1904 "parse.y"
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

  case 296:

/* Line 1455 of yacc.c  */
#line 1921 "parse.y"
    { active_range = (yyvsp[(1) - (1)].exprs); active_signed = false; ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1923 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1926 "parse.y"
    { active_range = (yyvsp[(2) - (2)].exprs); active_signed = true; ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1928 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1946 "parse.y"
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

  case 304:

/* Line 1455 of yacc.c  */
#line 1963 "parse.y"
    { active_range = (yyvsp[(1) - (1)].exprs); active_signed = false; ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1965 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1968 "parse.y"
    { active_range = (yyvsp[(2) - (2)].exprs); active_signed = true; ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1970 "parse.y"
    { active_range = 0;
		  active_signed = false;
		;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 1998 "parse.y"
    { struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = (yyvsp[(3) - (4)].exprs);
		  tmp->by_name = 0;
		  (yyval.parmvalue) = tmp;
		;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2004 "parse.y"
    { struct parmvalue_t*tmp = new struct parmvalue_t;
		  tmp->by_order = 0;
		  tmp->by_name = (yyvsp[(3) - (4)].named_pexprs);
		  (yyval.parmvalue) = tmp;
		;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2010 "parse.y"
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

  case 313:

/* Line 1455 of yacc.c  */
#line 2022 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: syntax error in parameter value "
			  "assignment list.");
		  (yyval.parmvalue) = 0;
		;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2027 "parse.y"
    { (yyval.parmvalue) = 0; ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2032 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = (yyvsp[(4) - (5)].expr);
		  free((yyvsp[(2) - (5)].text));
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2039 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (4)].text));
		  tmp->parm = 0;
		  free((yyvsp[(2) - (4)].text));
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2049 "parse.y"
    { svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].named_pexpr);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2054 "parse.y"
    { svector<named_pexpr_t*>*tmp =
			new svector<named_pexpr_t*>(*(yyvsp[(1) - (3)].named_pexprs),(yyvsp[(3) - (3)].named_pexpr));
		  delete (yyvsp[(1) - (3)].named_pexprs);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2077 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2085 "parse.y"
    { Module::port_t*tmp = (yyvsp[(4) - (5)].mport);
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.mport) = tmp;
		;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2096 "parse.y"
    { Module::port_t*tmp = (yyvsp[(2) - (3)].mport);
		  tmp->name = perm_string();
		  (yyval.mport) = tmp;
		;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2105 "parse.y"
    { Module::port_t*tmp = (yyvsp[(5) - (7)].mport);
		  tmp->name = lex_strings.make((yyvsp[(2) - (7)].text));
		  delete (yyvsp[(2) - (7)].text);
		  (yyval.mport) = tmp;
		;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2113 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2114 "parse.y"
    { (yyval.mport) = 0; ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2131 "parse.y"
    { Module::port_t*ptmp;
		  ptmp = pform_module_port_reference((yyvsp[(1) - (1)].text), (yylsp[(1) - (1)]).text,
						     (yylsp[(1) - (1)]).first_line);
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.mport) = ptmp;
		;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2139 "parse.y"
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

  case 327:

/* Line 1455 of yacc.c  */
#line 2161 "parse.y"
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

  case 328:

/* Line 1455 of yacc.c  */
#line 2178 "parse.y"
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

  case 329:

/* Line 1455 of yacc.c  */
#line 2194 "parse.y"
    { (yyval.mport) = (yyvsp[(1) - (1)].mport); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2196 "parse.y"
    { Module::port_t*tmp = (yyvsp[(1) - (3)].mport);
		  tmp->expr = svector<PEIdent*>(tmp->expr, (yyvsp[(3) - (3)].mport)->expr);
		  delete (yyvsp[(3) - (3)].mport);
		  (yyval.mport) = tmp;
		;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2209 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = (yyvsp[(4) - (5)].expr);
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2216 "parse.y"
    { yyerror((yylsp[(4) - (5)]), "error: invalid port connection expression.");
		  named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (5)].text));
		  tmp->parm = 0;
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2224 "parse.y"
    { named_pexpr_t*tmp = new named_pexpr_t;
		  tmp->name = lex_strings.make((yyvsp[(2) - (4)].text));
		  tmp->parm = 0;
		  delete (yyvsp[(2) - (4)].text);
		  (yyval.named_pexpr) = tmp;
		;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2234 "parse.y"
    { svector<named_pexpr_t*>*tmp;
		  tmp = new svector<named_pexpr_t*>(*(yyvsp[(1) - (3)].named_pexprs), (yyvsp[(3) - (3)].named_pexpr));
		  delete (yyvsp[(1) - (3)].named_pexprs);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2240 "parse.y"
    { svector<named_pexpr_t*>*tmp = new svector<named_pexpr_t*>(1);
		  (*tmp)[0] = (yyvsp[(1) - (1)].named_pexpr);
		  (yyval.named_pexprs) = tmp;
		;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2247 "parse.y"
    { (yyval.porttype) = NetNet::PINPUT; ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2248 "parse.y"
    { (yyval.porttype) = NetNet::POUTPUT; ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2249 "parse.y"
    { (yyval.porttype) = NetNet::PINOUT; ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2254 "parse.y"
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

  case 341:

/* Line 1455 of yacc.c  */
#line 2271 "parse.y"
    { (yyval.exprs) = 0; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2276 "parse.y"
    { (yyval.function_type).range = (yyvsp[(1) - (1)].exprs); (yyval.function_type).type = PTF_REG; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2277 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_INTEGER; ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2278 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REAL; ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2279 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REALTIME; ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2280 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_TIME; ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2281 "parse.y"
    { (yyval.function_type).range = 0;  (yyval.function_type).type = PTF_REG; ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2291 "parse.y"
    { pform_makewire((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].text), NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  (yyval.text) = (yyvsp[(1) - (1)].text);
		;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2296 "parse.y"
    { pform_makewire((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].text), NetNet::REG,
				 NetNet::NOT_A_PORT, 0);
		  if (! pform_expression_is_constant((yyvsp[(3) - (3)].expr)))
			yyerror((yylsp[(3) - (3)]), "error: register declaration assignment"
				" value must be a constant expression.");
		  pform_make_reginit((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].text), (yyvsp[(3) - (3)].expr));
		  (yyval.text) = (yyvsp[(1) - (3)].text);
		;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2305 "parse.y"
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

  case 351:

/* Line 1455 of yacc.c  */
#line 2318 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2324 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2334 "parse.y"
    {
		;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2337 "parse.y"
    {
		;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2340 "parse.y"
    {
		;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2343 "parse.y"
    {
		;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2347 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2351 "parse.y"
    { delete (yyvsp[(5) - (8)].expr);
		;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2355 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2359 "parse.y"
    { delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2363 "parse.y"
    { delete (yyvsp[(7) - (12)].expr);
		  delete (yyvsp[(9) - (12)].expr);
		;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2368 "parse.y"
    { delete (yyvsp[(7) - (12)].expr);
		  delete (yyvsp[(9) - (12)].expr);
		;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2373 "parse.y"
    { delete (yyvsp[(5) - (10)].expr);
		  delete (yyvsp[(7) - (10)].expr);
		;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2377 "parse.y"
    { delete (yyvsp[(5) - (7)].expr);
		;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2382 "parse.y"
    { ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2383 "parse.y"
    { ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2417 "parse.y"
    { yyerror((yylsp[(2) - (5)]), "Syntax error in delay value list.");
		  yyerrok;
		;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2425 "parse.y"
    { pform_make_specify_path((yyvsp[(2) - (6)].perm_strings), (yyvsp[(3) - (6)].letter), false, (yyvsp[(5) - (6)].perm_strings)); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2428 "parse.y"
    { pform_make_specify_path((yyvsp[(2) - (6)].perm_strings), (yyvsp[(3) - (6)].letter), true, (yyvsp[(5) - (6)].perm_strings)); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2430 "parse.y"
    { yyerror((yylsp[(2) - (3)]), "Invalid simple path");
		  yyerrok;
		;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2437 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (1)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (1)].text);
		;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2443 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(1) - (4)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(1) - (4)].text);
		;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2449 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (3)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (3)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (3)].text);
		;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2455 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (6)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(3) - (6)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(3) - (6)].text);
		;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2464 "parse.y"
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

  case 394:

/* Line 1455 of yacc.c  */
#line 2476 "parse.y"
    { delete (yyvsp[(1) - (7)].text);
		  delete (yyvsp[(3) - (7)].expr);
		  delete (yyvsp[(5) - (7)].expr);
		  delete (yyvsp[(7) - (7)].expr);
		;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2482 "parse.y"
    { delete (yyvsp[(1) - (3)].text);
		  delete (yyvsp[(3) - (3)].expr);
		;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2486 "parse.y"
    { delete (yyvsp[(1) - (7)].text);
		  delete (yyvsp[(4) - (7)].expr);
		  delete (yyvsp[(6) - (7)].expr);
		;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2498 "parse.y"
    { (yyval.letter) = '+'; ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2499 "parse.y"
    { (yyval.letter) = '-'; ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2500 "parse.y"
    { (yyval.letter) = 0;   ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2505 "parse.y"
    { delete (yyvsp[(2) - (2)].expr); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2507 "parse.y"
    { delete (yyvsp[(2) - (2)].expr); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2509 "parse.y"
    { delete (yyvsp[(2) - (4)].expr);
		  delete (yyvsp[(4) - (4)].expr);
		;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2513 "parse.y"
    { delete (yyvsp[(2) - (4)].expr);
		  delete (yyvsp[(4) - (4)].expr);
		;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2517 "parse.y"
    { delete (yyvsp[(1) - (3)].expr);
		  delete (yyvsp[(3) - (3)].expr);
		;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2521 "parse.y"
    { delete (yyvsp[(1) - (1)].expr); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2526 "parse.y"
    {  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2528 "parse.y"
    {  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2532 "parse.y"
    {  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2534 "parse.y"
    { delete (yyvsp[(2) - (2)].hier); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2536 "parse.y"
    {  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2538 "parse.y"
    { delete (yyvsp[(3) - (3)].hier); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2540 "parse.y"
    { delete (yyvsp[(3) - (6)].hier);
		  delete (yyvsp[(5) - (6)].expr);
		;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2544 "parse.y"
    { delete (yyvsp[(1) - (1)].text); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2556 "parse.y"
    { PCAssign*tmp = new PCAssign((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2563 "parse.y"
    { PDeassign*tmp = new PDeassign((yyvsp[(2) - (3)].expr));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2574 "parse.y"
    { PForce*tmp = new PForce((yyvsp[(2) - (5)].expr), (yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2580 "parse.y"
    { PRelease*tmp = new PRelease((yyvsp[(2) - (3)].expr));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2593 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ, *(yyvsp[(2) - (3)].statement_list));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2600 "parse.y"
    { pform_push_scope((yyvsp[(3) - (3)].text)); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2603 "parse.y"
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

  case 423:

/* Line 1455 of yacc.c  */
#line 2613 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2619 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_SEQ);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2633 "parse.y"
    { pform_push_scope((yyvsp[(3) - (3)].text)); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2636 "parse.y"
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

  case 427:

/* Line 1455 of yacc.c  */
#line 2646 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2652 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR);
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  delete (yyvsp[(3) - (4)].text);
		  (yyval.statement) = tmp;
		;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2660 "parse.y"
    { PDisable*tmp = new PDisable(*(yyvsp[(2) - (3)].hier));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2667 "parse.y"
    { PTrigger*tmp = new PTrigger(*(yyvsp[(2) - (3)].hier));
		  tmp->set_file((yylsp[(2) - (3)]).text);
		  tmp->set_lineno((yylsp[(2) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2674 "parse.y"
    { PForever*tmp = new PForever((yyvsp[(2) - (2)].statement));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2680 "parse.y"
    { PBlock*tmp = new PBlock(PBlock::BL_PAR, *(yyvsp[(2) - (3)].statement_list));
		  tmp->set_file((yylsp[(1) - (3)]).text);
		  tmp->set_lineno((yylsp[(1) - (3)]).first_line);
		  delete (yyvsp[(2) - (3)].statement_list);
		  (yyval.statement) = tmp;
		;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2687 "parse.y"
    { PRepeat*tmp = new PRepeat((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2693 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQ, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2699 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQX, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2705 "parse.y"
    { PCase*tmp = new PCase(NetCase::EQZ, (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].citems));
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2711 "parse.y"
    { yyerrok; ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2713 "parse.y"
    { yyerrok; ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2715 "parse.y"
    { yyerrok; ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2717 "parse.y"
    { PCondit*tmp = new PCondit((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement), 0);
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2723 "parse.y"
    { PCondit*tmp = new PCondit((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].statement), (yyvsp[(7) - (7)].statement));
		  tmp->set_file((yylsp[(1) - (7)]).text);
		  tmp->set_lineno((yylsp[(1) - (7)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2729 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "error: Malformed conditional expression.");
		  (yyval.statement) = (yyvsp[(5) - (5)].statement);
		;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2733 "parse.y"
    { yyerror((yylsp[(1) - (7)]), "error: Malformed conditional expression.");
		  (yyval.statement) = (yyvsp[(5) - (7)].statement);
		;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2738 "parse.y"
    { PForStatement*tmp = new PForStatement((yyvsp[(3) - (13)].expr), (yyvsp[(5) - (13)].expr), (yyvsp[(7) - (13)].expr), (yyvsp[(9) - (13)].expr), (yyvsp[(11) - (13)].expr), (yyvsp[(13) - (13)].statement));
		  tmp->set_file((yylsp[(1) - (13)]).text);
		  tmp->set_lineno((yylsp[(1) - (13)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2745 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(9) - (11)]), "error: Error in for loop step assignment.");
		;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2750 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(7) - (13)]), "error: Error in for loop condition expression.");
		;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2754 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(3) - (5)]), "error: Incomprehensible for loop.");
		;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2758 "parse.y"
    { PWhile*tmp = new PWhile((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].statement));
		  (yyval.statement) = tmp;
		;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2762 "parse.y"
    { (yyval.statement) = 0;
		  yyerror((yylsp[(3) - (5)]), "error: Error in while loop condition.");
		;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2766 "parse.y"
    { PExpr*del = (*(yyvsp[(1) - (2)].exprs))[0];
		  assert((yyvsp[(1) - (2)].exprs)->count() == 1);
		  PDelayStatement*tmp = new PDelayStatement(del, (yyvsp[(2) - (2)].statement));
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2774 "parse.y"
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

  case 452:

/* Line 1455 of yacc.c  */
#line 2786 "parse.y"
    { PEventStatement*tmp = new PEventStatement;
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  pform_attach_attributes((yyvsp[(4) - (4)].statement), (yyvsp[(3) - (4)].named_pexprs));
		  tmp->set_statement((yyvsp[(4) - (4)].statement));
		  if ((yyvsp[(3) - (4)].named_pexprs)) delete (yyvsp[(3) - (4)].named_pexprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2795 "parse.y"
    { PEventStatement*tmp = new PEventStatement;
		  tmp->set_file((yylsp[(1) - (6)]).text);
		  tmp->set_lineno((yylsp[(1) - (6)]).first_line);
		  pform_attach_attributes((yyvsp[(6) - (6)].statement), (yyvsp[(5) - (6)].named_pexprs));
		  tmp->set_statement((yyvsp[(6) - (6)].statement));
		  if ((yyvsp[(5) - (6)].named_pexprs)) delete (yyvsp[(5) - (6)].named_pexprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2804 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2810 "parse.y"
    { PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));
		  tmp->set_file((yylsp[(1) - (4)]).text);
		  tmp->set_lineno((yylsp[(1) - (4)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2816 "parse.y"
    { assert((yyvsp[(3) - (5)].exprs)->count() == 1);
		  PExpr*del = (*(yyvsp[(3) - (5)].exprs))[0];
		  PAssign*tmp = new PAssign((yyvsp[(1) - (5)].expr),del,(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2824 "parse.y"
    { assert((yyvsp[(3) - (5)].exprs)->count() == 1);
		  PExpr*del = (*(yyvsp[(3) - (5)].exprs))[0];
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (5)].expr),del,(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2832 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (5)].expr),(yyvsp[(3) - (5)].event_statement),(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2838 "parse.y"
    { PAssign*tmp = new PAssign((yyvsp[(1) - (9)].expr),(yyvsp[(7) - (9)].event_statement),(yyvsp[(8) - (9)].expr));
		  tmp->set_file((yylsp[(1) - (9)]).text);
		  tmp->set_lineno((yylsp[(1) - (9)]).first_line);
		  yyerror((yylsp[(3) - (9)]), "sorry: repeat event control not supported.");
		  delete (yyvsp[(5) - (9)].expr);
		  (yyval.statement) = tmp;
		;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2846 "parse.y"
    { yyerror((yylsp[(1) - (5)]), "sorry: Event controls not supported here.");
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (5)].expr),(yyvsp[(4) - (5)].expr));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2853 "parse.y"
    { yyerror((yylsp[(1) - (9)]), "sorry: Event controls not supported here.");
		  delete (yyvsp[(5) - (9)].expr);
		  PAssignNB*tmp = new PAssignNB((yyvsp[(1) - (9)].expr),(yyvsp[(8) - (9)].expr));
		  tmp->set_file((yylsp[(1) - (9)]).text);
		  tmp->set_lineno((yylsp[(1) - (9)]).first_line);
		  (yyval.statement) = tmp;
		;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2861 "parse.y"
    { PEventStatement*tmp;
		  PEEvent*etmp = new PEEvent(PEEvent::POSITIVE, (yyvsp[(3) - (5)].expr));
		  tmp = new PEventStatement(etmp);
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  tmp->set_statement((yyvsp[(5) - (5)].statement));
		  (yyval.statement) = tmp;
		;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2870 "parse.y"
    { PCallTask*tmp = new PCallTask(hname_t((yyvsp[(1) - (5)].text)), *(yyvsp[(3) - (5)].exprs));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  delete (yyvsp[(1) - (5)].text);
		  delete (yyvsp[(3) - (5)].exprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2878 "parse.y"
    { svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(hname_t((yyvsp[(1) - (2)].text)), pt);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  delete (yyvsp[(1) - (2)].text);
		  (yyval.statement) = tmp;
		;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2886 "parse.y"
    { PCallTask*tmp = new PCallTask(*(yyvsp[(1) - (5)].hier), *(yyvsp[(3) - (5)].exprs));
		  tmp->set_file((yylsp[(1) - (5)]).text);
		  tmp->set_lineno((yylsp[(1) - (5)]).first_line);
		  delete (yyvsp[(1) - (5)].hier);
		  delete (yyvsp[(3) - (5)].exprs);
		  (yyval.statement) = tmp;
		;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2894 "parse.y"
    { svector<PExpr*>pt (0);
		  PCallTask*tmp = new PCallTask(*(yyvsp[(1) - (2)].hier), pt);
		  tmp->set_file((yylsp[(1) - (2)]).text);
		  tmp->set_lineno((yylsp[(1) - (2)]).first_line);
		  delete (yyvsp[(1) - (2)].hier);
		  (yyval.statement) = tmp;
		;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2902 "parse.y"
    { yyerror((yylsp[(1) - (2)]), "error: malformed statement");
		  yyerrok;
		  (yyval.statement) = new PNoop;
		;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2910 "parse.y"
    { pform_attach_attributes((yyvsp[(3) - (3)].statement), (yyvsp[(2) - (3)].named_pexprs));
		  svector<Statement*>*tmp = new svector<Statement*>(*(yyvsp[(1) - (3)].statement_list), (yyvsp[(3) - (3)].statement));
		  delete (yyvsp[(1) - (3)].statement_list);
		  if ((yyvsp[(2) - (3)].named_pexprs)) delete (yyvsp[(2) - (3)].named_pexprs);
		  (yyval.statement_list) = tmp;
		;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2917 "parse.y"
    { pform_attach_attributes((yyvsp[(2) - (2)].statement), (yyvsp[(1) - (2)].named_pexprs));
		  svector<Statement*>*tmp = new svector<Statement*>(1);
		  (*tmp)[0] = (yyvsp[(2) - (2)].statement);
		  if ((yyvsp[(1) - (2)].named_pexprs)) delete (yyvsp[(1) - (2)].named_pexprs);
		  (yyval.statement_list) = tmp;
		;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2927 "parse.y"
    { (yyval.statement) = 0; ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2933 "parse.y"
    { (yyval.wires) = new svector<PWire*>(0); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2935 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2942 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2949 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::POUTPUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2956 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::POUTPUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 2963 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINOUT, false,
						(yyvsp[(2) - (4)].exprs), (yyvsp[(3) - (4)].perm_strings),
						(yylsp[(1) - (4)]).text, (yylsp[(1) - (4)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 2970 "parse.y"
    { svector<PWire*>*tmp
			= pform_make_task_ports(NetNet::PINOUT, true,
						(yyvsp[(3) - (5)].exprs), (yyvsp[(4) - (5)].perm_strings),
						(yylsp[(1) - (5)]).text, (yylsp[(1) - (5)]).first_line);
		  (yyval.wires) = tmp;
		;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 2980 "parse.y"
    { svector<PWire*>*tmp = new svector<PWire*>(*(yyvsp[(1) - (2)].wires), *(yyvsp[(2) - (2)].wires));
		  delete (yyvsp[(1) - (2)].wires);
		  delete (yyvsp[(2) - (2)].wires);
		  (yyval.wires) = tmp;
		;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 2986 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 2991 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 2993 "parse.y"
    { (yyval.wires) = 0; ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 2997 "parse.y"
    { lex_start_table(); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 2999 "parse.y"
    { lex_end_table(); (yyval.strings) = (yyvsp[(3) - (4)].strings); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3009 "parse.y"
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

  case 488:

/* Line 1455 of yacc.c  */
#line 3022 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3028 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (2)].strings);
		  tmp->push_back((yyvsp[(2) - (2)].text));
		  delete (yyvsp[(2) - (2)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3037 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3043 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (2)].strings);
		  tmp->push_back((yyvsp[(2) - (2)].text));
		  delete (yyvsp[(2) - (2)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3052 "parse.y"
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

  case 493:

/* Line 1455 of yacc.c  */
#line 3066 "parse.y"
    { PExpr*etmp = new PENumber((yyvsp[(4) - (5)].number));
		  PEIdent*itmp = new PEIdent(hname_t((yyvsp[(2) - (5)].text)));
		  PAssign*atmp = new PAssign(itmp, etmp);
		  atmp->set_file((yylsp[(2) - (5)]).text);
		  atmp->set_lineno((yylsp[(2) - (5)]).first_line);
		  delete (yyvsp[(2) - (5)].text);
		  (yyval.statement) = atmp;
		;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3077 "parse.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3078 "parse.y"
    { (yyval.statement) = 0; ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3083 "parse.y"
    { char*tmp = new char[2];
		  tmp[0] = (yyvsp[(1) - (1)].letter);
		  tmp[1] = 0;
		  (yyval.text) = tmp;
		;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3089 "parse.y"
    { char*tmp = new char[strlen((yyvsp[(1) - (2)].text))+2];
		  strcpy(tmp, (yyvsp[(1) - (2)].text));
		  char*tp = tmp+strlen(tmp);
		  *tp++ = (yyvsp[(2) - (2)].letter);
		  *tp++ = 0;
		  delete[](yyvsp[(1) - (2)].text);
		  (yyval.text) = tmp;
		;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3100 "parse.y"
    { (yyval.letter) = '0'; ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3101 "parse.y"
    { (yyval.letter) = '1'; ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3102 "parse.y"
    { (yyval.letter) = 'x'; ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3103 "parse.y"
    { (yyval.letter) = '?'; ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3104 "parse.y"
    { (yyval.letter) = 'b'; ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3105 "parse.y"
    { (yyval.letter) = '*'; ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3106 "parse.y"
    { (yyval.letter) = '%'; ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3107 "parse.y"
    { (yyval.letter) = 'f'; ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3108 "parse.y"
    { (yyval.letter) = 'F'; ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3109 "parse.y"
    { (yyval.letter) = 'l'; ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3110 "parse.y"
    { (yyval.letter) = 'H'; ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3111 "parse.y"
    { (yyval.letter) = 'B'; ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3112 "parse.y"
    { (yyval.letter) = 'r'; ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3113 "parse.y"
    { (yyval.letter) = 'R'; ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3114 "parse.y"
    { (yyval.letter) = 'M'; ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3115 "parse.y"
    { (yyval.letter) = 'n'; ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3116 "parse.y"
    { (yyval.letter) = 'N'; ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3117 "parse.y"
    { (yyval.letter) = 'p'; ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3118 "parse.y"
    { (yyval.letter) = 'P'; ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3119 "parse.y"
    { (yyval.letter) = 'Q'; ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3120 "parse.y"
    { (yyval.letter) = 'q'; ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3121 "parse.y"
    { (yyval.letter) = '_'; ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3122 "parse.y"
    { (yyval.letter) = '+'; ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3126 "parse.y"
    { (yyval.letter) = '0'; ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3127 "parse.y"
    { (yyval.letter) = '1'; ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3128 "parse.y"
    { (yyval.letter) = 'x'; ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3129 "parse.y"
    { (yyval.letter) = '-'; ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3134 "parse.y"
    { (yyval.wires) = pform_make_udp_input_ports((yyvsp[(2) - (3)].perm_strings)); ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3136 "parse.y"
    { PWire*pp = new PWire((yyvsp[(2) - (3)].text), NetNet::IMPLICIT, NetNet::POUTPUT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3142 "parse.y"
    { PWire*pp = new PWire((yyvsp[(2) - (3)].text), NetNet::REG, NetNet::PIMPLICIT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3148 "parse.y"
    { PWire*pp = new PWire((yyvsp[(3) - (4)].text), NetNet::REG, NetNet::POUTPUT);
		  svector<PWire*>*tmp = new svector<PWire*>(1);
		  (*tmp)[0] = pp;
		  (yyval.wires) = tmp;
		;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3157 "parse.y"
    { (yyval.wires) = (yyvsp[(1) - (1)].wires); ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3159 "parse.y"
    { svector<PWire*>*tmp = new svector<PWire*>(*(yyvsp[(1) - (2)].wires), *(yyvsp[(2) - (2)].wires));
		  delete (yyvsp[(1) - (2)].wires);
		  delete (yyvsp[(2) - (2)].wires);
		  (yyval.wires) = tmp;
		;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3168 "parse.y"
    { list<string>*tmp = new list<string>;
		  tmp->push_back((yyvsp[(1) - (1)].text));
		  delete (yyvsp[(1) - (1)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3174 "parse.y"
    { list<string>*tmp = (yyvsp[(1) - (3)].strings);
		  tmp->push_back((yyvsp[(3) - (3)].text));
		  delete (yyvsp[(3) - (3)].text);
		  (yyval.strings) = tmp;
		;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3181 "parse.y"
    { (yyval.flag) = true; ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3181 "parse.y"
    { (yyval.flag) = false; ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3184 "parse.y"
    { (yyval.expr) = (yyvsp[(2) - (2)].expr); ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3185 "parse.y"
    { (yyval.expr) = 0; ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3190 "parse.y"
    { list<perm_string>*tmp = new list<perm_string>;
		  tmp->push_back(lex_strings.make((yyvsp[(2) - (2)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(2) - (2)].text);
		;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3196 "parse.y"
    { list<perm_string>*tmp = (yyvsp[(1) - (4)].perm_strings);
		  tmp->push_back(lex_strings.make((yyvsp[(4) - (4)].text)));
		  (yyval.perm_strings) = tmp;
		  delete[](yyvsp[(4) - (4)].text);
		;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3214 "parse.y"
    { perm_string tmp2 = lex_strings.make((yyvsp[(2) - (10)].text));
		  pform_make_udp(tmp2, (yyvsp[(4) - (10)].strings), (yyvsp[(7) - (10)].wires), (yyvsp[(9) - (10)].strings), (yyvsp[(8) - (10)].statement),
				 (yylsp[(2) - (10)]).text, (yylsp[(2) - (10)]).first_line);
		  delete[](yyvsp[(2) - (10)].text);
		;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3229 "parse.y"
    { perm_string tmp2 = lex_strings.make((yyvsp[(2) - (13)].text));
		  perm_string tmp6 = lex_strings.make((yyvsp[(6) - (13)].text));
		  pform_make_udp(tmp2, (yyvsp[(5) - (13)].flag), tmp6, (yyvsp[(7) - (13)].expr), (yyvsp[(9) - (13)].perm_strings), (yyvsp[(12) - (13)].strings),
				 (yylsp[(2) - (13)]).text, (yylsp[(2) - (13)]).first_line);
		  delete[](yyvsp[(2) - (13)].text);
		  delete[](yyvsp[(6) - (13)].text);
		;}
    break;



/* Line 1455 of yacc.c  */
#line 8369 "parse.cc"
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



