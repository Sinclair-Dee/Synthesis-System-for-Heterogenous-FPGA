
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 259 "parse.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE VLlval;

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

extern YYLTYPE VLlloc;

