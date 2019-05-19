/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -o -i 7 -C -k '1-3,$' -L ANSI-C -H keyword_hash -N check_identifier -t lexor_keyword.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "lexor_keyword.gperf"

/* Command-line: gperf -o -i 1 -C -k 1-3,$ -L C -H keyword_hash -N check_identifier -tT lexor_keyword.gperf */

#include "config.h"
#include "parse_misc.h"
#include "parse.h"
#include <string.h>
#include "lexor_keyword.h"

#line 11 "lexor_keyword.gperf"
struct lexor_keyword { const char*name; int tokenType; };

#define TOTAL_KEYWORDS 109
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 31
#define MAX_HASH_VALUE 243
/* maximum key range = 213, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
keyword_hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244,  87,  72,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244,  42,  17,  77,
        7,   7,  52,  32,  22,  12,  12, 122,  62,  17,
        7,  27,  42,   7,  12,  22,   7,  47,  62,  97,
       42,   7,  12, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
      244, 244, 244, 244, 244, 244
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct lexor_keyword *
check_identifier (register const char *str, register unsigned int len)
{
  static const struct lexor_keyword wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 30 "lexor_keyword.gperf"
      {"end",		K_end},
      {""}, {""}, {""},
#line 31 "lexor_keyword.gperf"
      {"endcase",	K_endcase},
#line 36 "lexor_keyword.gperf"
      {"endtable",	K_endtable},
#line 33 "lexor_keyword.gperf"
      {"endmodule",	K_endmodule},
#line 35 "lexor_keyword.gperf"
      {"endspecify",	K_endspecify},
#line 32 "lexor_keyword.gperf"
      {"endfunction",	K_endfunction},
#line 34 "lexor_keyword.gperf"
      {"endprimitive",	K_endprimitive},
      {""}, {""},
#line 82 "lexor_keyword.gperf"
      {"rtran",		K_rtran},
#line 103 "lexor_keyword.gperf"
      {"triand",		K_triand},
#line 50 "lexor_keyword.gperf"
      {"integer",	K_integer},
#line 100 "lexor_keyword.gperf"
      {"tri",		K_tri},
#line 96 "lexor_keyword.gperf"
      {"time",		K_time},
#line 104 "lexor_keyword.gperf"
      {"trior",		K_trior},
      {""}, {""},
#line 61 "lexor_keyword.gperf"
      {"not",		K_not},
      {""},
#line 64 "lexor_keyword.gperf"
      {"or",		K_or},
#line 55 "lexor_keyword.gperf"
      {"medium",		K_medium},
#line 27 "lexor_keyword.gperf"
      {"disable",	K_disable},
      {""},
#line 28 "lexor_keyword.gperf"
      {"edge",		K_edge},
#line 48 "lexor_keyword.gperf"
      {"inout",		K_inout},
      {""},
#line 58 "lexor_keyword.gperf"
      {"negedge",	K_negedge},
#line 60 "lexor_keyword.gperf"
      {"nor",		K_nor},
#line 51 "lexor_keyword.gperf"
      {"join",		K_join},
#line 80 "lexor_keyword.gperf"
      {"rnmos",		K_rnmos},
#line 56 "lexor_keyword.gperf"
      {"module",		K_module},
      {""},
#line 14 "lexor_keyword.gperf"
      {"and",		K_and},
#line 57 "lexor_keyword.gperf"
      {"nand",		K_nand},
#line 16 "lexor_keyword.gperf"
      {"begin",		K_begin},
#line 105 "lexor_keyword.gperf"
      {"trireg",		K_trireg},
      {""},
#line 24 "lexor_keyword.gperf"
      {"deassign",	K_deassign},
#line 97 "lexor_keyword.gperf"
      {"tran",		K_tran},
#line 49 "lexor_keyword.gperf"
      {"input",		K_input},
#line 79 "lexor_keyword.gperf"
      {"repeat",		K_repeat},
      {""},
#line 76 "lexor_keyword.gperf"
      {"realtime",	K_realtime},
#line 59 "lexor_keyword.gperf"
      {"nmos",		K_nmos},
#line 94 "lexor_keyword.gperf"
      {"table",		K_table},
#line 86 "lexor_keyword.gperf"
      {"signed",		K_signed},
#line 25 "lexor_keyword.gperf"
      {"default",	K_default},
      {""},
#line 69 "lexor_keyword.gperf"
      {"primitive",	K_primitive},
      {""}, {""},
#line 88 "lexor_keyword.gperf"
      {"specify",	K_specify},
#line 77 "lexor_keyword.gperf"
      {"reg",		K_reg},
      {""},
#line 38 "lexor_keyword.gperf"
      {"event",		K_event},
#line 118 "lexor_keyword.gperf"
      {"square",		K_square},
      {""},
#line 26 "lexor_keyword.gperf"
      {"defparam",	K_defparam},
#line 114 "lexor_keyword.gperf"
      {"xnor",		K_xnor},
      {""},
#line 65 "lexor_keyword.gperf"
      {"output",		K_output},
#line 78 "lexor_keyword.gperf"
      {"release",	K_release},
#line 115 "lexor_keyword.gperf"
      {"xor",		K_xor},
#line 89 "lexor_keyword.gperf"
      {"specparam",	K_specparam},
#line 81 "lexor_keyword.gperf"
      {"rpmos",		K_rpmos},
#line 15 "lexor_keyword.gperf"
      {"assign",		K_assign},
#line 47 "lexor_keyword.gperf"
      {"initial",	K_initial},
      {""},
#line 29 "lexor_keyword.gperf"
      {"else",		K_else},
#line 40 "lexor_keyword.gperf"
      {"force",		K_force},
      {""},
#line 68 "lexor_keyword.gperf"
      {"posedge",	K_posedge},
#line 39 "lexor_keyword.gperf"
      {"for",		K_for},
#line 102 "lexor_keyword.gperf"
      {"tri1",		K_tri1},
      {""}, {""},
#line 41 "lexor_keyword.gperf"
      {"forever",	K_forever},
#line 84 "lexor_keyword.gperf"
      {"rtranif1",	K_rtranif1},
#line 67 "lexor_keyword.gperf"
      {"pmos",		K_pmos},
      {""}, {""}, {""}, {""},
#line 66 "lexor_keyword.gperf"
      {"parameter",	K_parameter},
#line 46 "lexor_keyword.gperf"
      {"if",		K_if},
#line 63 "lexor_keyword.gperf"
      {"notif1",		K_notif1},
#line 91 "lexor_keyword.gperf"
      {"strong1",	K_strong1},
#line 43 "lexor_keyword.gperf"
      {"function",	K_function},
#line 101 "lexor_keyword.gperf"
      {"tri0",		K_tri0},
      {""}, {""}, {""},
#line 83 "lexor_keyword.gperf"
      {"rtranif0",	K_rtranif0},
#line 75 "lexor_keyword.gperf"
      {"real",		K_real},
#line 52 "lexor_keyword.gperf"
      {"large",		K_large},
      {""}, {""}, {""},
#line 112 "lexor_keyword.gperf"
      {"wire",		K_wire},
#line 74 "lexor_keyword.gperf"
      {"rcmos",		K_rcmos},
#line 62 "lexor_keyword.gperf"
      {"notif0",		K_notif0},
#line 90 "lexor_keyword.gperf"
      {"strong0",	K_strong0},
      {""},
#line 119 "lexor_keyword.gperf"
      {"mult",		K_mult},
      {""}, {""},
#line 99 "lexor_keyword.gperf"
      {"tranif1",	K_tranif1},
#line 120 "lexor_keyword.gperf"
      {"mult_add",	K_mult_add},
      {""},
#line 111 "lexor_keyword.gperf"
      {"while",		K_while},
#line 45 "lexor_keyword.gperf"
      {"highz1",		K_highz1},
      {""}, {""},
#line 23 "lexor_keyword.gperf"
      {"cmos",		K_cmos},
#line 87 "lexor_keyword.gperf"
      {"small",		K_small},
      {""},
#line 37 "lexor_keyword.gperf"
      {"endtask",	K_endtask},
#line 113 "lexor_keyword.gperf"
      {"wor",		K_wor},
#line 20 "lexor_keyword.gperf"
      {"case",		K_case},
#line 121 "lexor_keyword.gperf"
      {"mult_accum",	K_mult_accum},
#line 54 "lexor_keyword.gperf"
      {"macromodule",	K_macromodule},
#line 98 "lexor_keyword.gperf"
      {"tranif0",	K_tranif0},
#line 85 "lexor_keyword.gperf"
      {"scalared",	K_scalared},
#line 108 "lexor_keyword.gperf"
      {"wand",		K_wand},
#line 22 "lexor_keyword.gperf"
      {"casez",		K_casez},
#line 44 "lexor_keyword.gperf"
      {"highz0",		K_highz0},
      {""},
#line 106 "lexor_keyword.gperf"
      {"vectored",	K_vectored},
#line 107 "lexor_keyword.gperf"
      {"wait",		K_wait},
      {""}, {""}, {""},
#line 72 "lexor_keyword.gperf"
      {"pulldown",	K_pulldown},
      {""}, {""}, {""}, {""},
#line 17 "lexor_keyword.gperf"
      {"buf",		K_buf},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 21 "lexor_keyword.gperf"
      {"casex",		K_casex},
      {""},
#line 93 "lexor_keyword.gperf"
      {"supply1",	K_supply1},
      {""},
#line 116 "lexor_keyword.gperf"
      {"comet_pll",	K_pll},
#line 53 "lexor_keyword.gperf"
      {"localparam",	K_localparam},
#line 19 "lexor_keyword.gperf"
      {"bufif1",		K_bufif1},
      {""}, {""},
#line 95 "lexor_keyword.gperf"
      {"task",		K_task},
      {""},
#line 73 "lexor_keyword.gperf"
      {"pullup",		K_pullup},
      {""}, {""}, {""}, {""}, {""},
#line 92 "lexor_keyword.gperf"
      {"supply0",	K_supply0},
      {""}, {""}, {""},
#line 18 "lexor_keyword.gperf"
      {"bufif0",		K_bufif0},
#line 117 "lexor_keyword.gperf"
      {"psk_mem",		K_mem},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 42 "lexor_keyword.gperf"
      {"fork",		K_fork},
      {""}, {""}, {""}, {""}, {""},
#line 110 "lexor_keyword.gperf"
      {"weak1",		K_weak1},
      {""}, {""}, {""}, {""},
#line 71 "lexor_keyword.gperf"
      {"pull1",		K_pull1},
#line 13 "lexor_keyword.gperf"
      {"always",		K_always},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 109 "lexor_keyword.gperf"
      {"weak0",		K_weak0},
      {""}, {""}, {""}, {""},
#line 70 "lexor_keyword.gperf"
      {"pull0",		K_pull0}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = keyword_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 122 "lexor_keyword.gperf"


int lexor_keyword_code(const char*str, unsigned nstr)
{
      const struct lexor_keyword*rc = check_identifier(str, nstr);
      if (rc == 0)
	  return IDENTIFIER;
      else
	  return rc->tokenType;
}
