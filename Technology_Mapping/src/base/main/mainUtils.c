/**CFile****************************************************************

  FileName    [mainUtils.c]

  SystemName  [ABC: Logic synthesis and verification system.]
 
  PackageName [The main package.]

  Synopsis    [Miscellaneous utilities.]
 
  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: mainUtils.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/abc/abc.h"
#include "mainInt.h"

#ifdef ABC_USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static char * DateReadFromDateString( char * datestr );

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
char * Abc_UtilsGetVersion( Abc_Frame_t * pAbc )
{
    static char Version[1000];
    sprintf(Version, "%s (compiled %s %s)", ABC_VERSION, __DATE__, __TIME__);
    return Version;
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
char * Abc_UtilsGetUsersInput( Abc_Frame_t * pAbc )
{
    static char Prompt[5000];
    sprintf( Prompt, "abc %02d> ", pAbc->nSteps );
#ifdef ABC_USE_READLINE
    {
    static char * line = NULL;
    if (line != NULL) ABC_FREE(line);
    line = readline(Prompt);  
    if (line == NULL){ printf("***EOF***\n"); exit(0); }
    add_history(line);
    return line;
    }
#else
    {
    char * pRetValue;
    fprintf( pAbc->Out, "%s", Prompt );
    pRetValue = fgets( Prompt, 5000, stdin );
    return Prompt;
    }
#endif
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_UtilsPrintHello( Abc_Frame_t * pAbc )
{
    fprintf( pAbc->Out, "%s\n", pAbc->sVersion );
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_UtilsPrintUsage( Abc_Frame_t * pAbc, char * ProgName )
{
    fprintf( pAbc->Err, "\n" );
    fprintf( pAbc->Err,
             "usage: %s [-c cmd] [-f script] [-h] [-o file] [-s] [-t type] [-T type] [-x] [-b] [file]\n", 
             ProgName);
    fprintf( pAbc->Err, "    -c cmd\texecute commands `cmd'\n");
    fprintf( pAbc->Err, "    -F script\texecute commands from a script file and echo commands\n");
    fprintf( pAbc->Err, "    -f script\texecute commands from a script file\n");
    fprintf( pAbc->Err, "    -h\t\tprint the command usage\n");
    fprintf( pAbc->Err, "    -o file\tspecify output filename to store the result\n");
    fprintf( pAbc->Err, "    -s\t\tdo not read any initialization file\n");
    fprintf( pAbc->Err, "    -t type\tspecify input type (blif_mv (default), blif_mvs, blif, or none)\n");
    fprintf( pAbc->Err, "    -T type\tspecify output type (blif_mv (default), blif_mvs, blif, or none)\n");
    fprintf( pAbc->Err, "    -x\t\tequivalent to '-t none -T none'\n");
    fprintf( pAbc->Err, "    -b\t\trunning in bridge mode\n");
    fprintf( pAbc->Err, "\n" );
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Abc_UtilsSource( Abc_Frame_t * pAbc )
{  
#ifdef WIN32
    //if ( Cmd_CommandExecute(pAbc, "source abc.rc") )
    //{
    //    if ( Cmd_CommandExecute(pAbc, "source ..\\abc.rc") == 0 )
    //        printf( "Loaded \"abc.rc\" from the parent directory.\n" );
    //    else if ( Cmd_CommandExecute(pAbc, "source ..\\..\\abc.rc") == 0 )
    //        printf( "Loaded \"abc.rc\" from the grandparent directory.\n" );
    //}
	char*cmd;

	///////////////////////
	// global parameters //
	///////////////////////
	cmd = "set check      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd ); // checks intermediate networks                         
	cmd = "set checkread  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd ); // checks new networks after reading from file          
	cmd = "set backup     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd ); // saves backup networks retrived by "undo" and "recall"
	cmd = "set savesteps 1"; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd ); // sets the maximum number of backup networks to save   
	cmd = "set progressbar"; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd ); // display the progress bar                             

	//////////////////////////////////////
	// program names for internal calls //
	//////////////////////////////////////
	cmd = "set dotwin dot.exe                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set dotunix dot                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set gsviewwin gsview32.exe           "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set gsviewunix gv                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set siswin sis.exe                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set sisunix sis                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set mvsiswin mvsis.exe               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set mvsisunix mvsis                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set capowin MetaPl-Capo10.1-Win32.exe"; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set capounix MetaPl-Capo10.1         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set gnuplotwin wgnuplot.exe          "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "set gnuplotunix gnuplot              "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	//////////////////////
	// standard aliases //
	//////////////////////
	cmd = "alias b balance                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias cl cleanup                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias clp collapse                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias esd ext_seq_dcs                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias f fraig                          "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias fs fraig_sweep                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias fsto fraig_store                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias fres fraig_restore               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias ft fraig_trust                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias lp lutpack                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pd print_dsd                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pex print_exdc -d                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pf print_factor                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pfan print_fanio                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pl print_level                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pio print_io                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pk print_kmap                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias ps print_stats                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias psu print_supp                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias psy print_symm                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias pun print_unate                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias q quit                           "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias r read                           "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias r3 retime -M 3                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias r3f retime -M 3 -f               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias r3b retime -M 3 -b               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias ren renode                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rh read_hie                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rl read_blif                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rb read_bench                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias ret retime                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rp read_pla                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rt read_truth                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rv read_verilog                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rvl read_verlib                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rsup read_super mcnc5_old.super  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rlib read_library                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rlibc read_library cadence.genlib"; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rw rewrite                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rwz rewrite -z                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rf refactor                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rfz refactor -z                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias re restructure                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rez restructure -z               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rs resub                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rsz resub -z                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias sa set autoexec ps               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias scl scleanup                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias sif if -s                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias so source -x                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias st strash                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias sw sweep                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias ssw ssweep                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias tr0 trace_start                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias tr1 trace_check                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trt \"r c.blif; st; tr0; b; tr1\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias u undo                           "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias w write                          "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wa write_aiger                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wb write_bench                   "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wc write_cnf                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wh write_hie                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wl write_blif                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wp write_pla                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias wv write_verilog                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	//////////////////////
	// standard scripts //
	//////////////////////
	cmd = "alias share       \"b; ren -s; fx; b\"                                                                                                                                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias sharedsd    \"b; ren -b; dsd -g; sw; fx; b\"                                                                                                                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias resyn       \"b; rw; rwz; b; rwz; b\"                                                                                                                               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias resyn2      \"b; rw; rf; b; rw; rwz; b; rfz; rwz; b\"                                                                                                               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias resyn2a     \"b; rw; b; rw; rwz; b; rwz; b\"                                                                                                                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias resyn3      \"b; rs; rs -K 6; b; rsz; rsz -K 6; b; rsz -K 5; b\"                                                                                                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias compress    \"b -l; rw -l; rwz -l; b -l; rwz -l; b -l\"                                                                                                             "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias compress2   \"b -l; rw -l; rf -l; b -l; rw -l; rwz -l; b -l; rfz -l; rwz -l; b -l\"                                                                                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias choice      \"fraig_store; resyn; fraig_store; resyn2; fraig_store; fraig_restore\"                                                                                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias choice2     \"fraig_store; balance; fraig_store; resyn; fraig_store; resyn2; fraig_store; resyn2; fraig_store; fraig_restore\"                                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rwsat       \"st; rw -l; b -l; rw -l; rf -l\"                                                                                                                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias rwsat2      \"st; rw -l; b -l; rw -l; rf -l; fraig; rw -l; b -l; rw -l; rf -l\"                                                                                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias shake       \"st; ps; sat -C 5000; rw -l; ps; sat -C 5000; b -l; rf -l; ps; sat -C 5000; rfz -l; ps; sat -C 5000; rwz -l; ps; sat -C 5000; rfz -l; ps; sat -C 5000\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	///////////////////////////////////////////////
	// resubstitution scripts for the IWLS paper //
	///////////////////////////////////////////////
	cmd = "alias src_rw      \"st; rw -l; rwz -l; rwz -l\"                                                                                                                                                                  "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias src_rs      \"st; rs -K 6 -N 2 -l; rs -K 9 -N 2 -l; rs -K 12 -N 2 -l\"                                                                                                                                     "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias src_rws     \"st; rw -l; rs -K 6 -N 2 -l; rwz -l; rs -K 9 -N 2 -l; rwz -l; rs -K 12 -N 2 -l\"                                                                                                              "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias resyn2rs    \"b; rs -K 6; rw; rs -K 6 -N 2; rf; rs -K 8; b; rs -K 8 -N 2; rw; rs -K 10; rwz; rs -K 10 -N 2; b; rs -K 12; rfz; rs -K 12 -N 2; rwz; b\"                                                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias compress2rs \"b -l; rs -K 6 -l; rw -l; rs -K 6 -N 2 -l; rf -l; rs -K 8 -l; b -l; rs -K 8 -N 2 -l; rw -l; rs -K 10 -l; rwz -l; rs -K 10 -N 2 -l; b -l; rs -K 12 -l; rfz -l; rs -K 12 -N 2 -l; rwz -l; b -l\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	////////////////////////////////////////////////
	// experimental implementation of don't-cares //
	////////////////////////////////////////////////
	cmd = "alias resyn2rsdc    \"b; rs -K 6 -F 2; rw; rs -K 6 -N 2 -F 2; rf; rs -K 8 -F 2; b; rs -K 8 -N 2 -F 2; rw; rs -K 10 -F 2; rwz; rs -K 10 -N 2 -F 2; b; rs -K 12 -F 2; rfz; rs -K 12 -N 2 -F 2; rwz; b\"                                                      "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias compress2rsdc \"b -l; rs -K 6 -F 2 -l; rw -l; rs -K 6 -N 2 -F 2 -l; rf -l; rs -K 8 -F 2 -l; b -l; rs -K 8 -N 2 -F 2 -l; rw -l; rs -K 10 -F 2 -l; rwz -l; rs -K 10 -N 2 -F 2 -l; b -l; rs -K 12 -F 2 -l; rfz -l; rs -K 12 -N 2 -F 2 -l; rwz -l; b -l\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	////////////////////////////////
	// minimizing for FF literals //
	////////////////////////////////
	cmd = "alias fflitmin    \"compress2rs; ren; sop; ps -f\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	/////////////////
	// temporaries //
	/////////////////
	cmd = "alias reach    \"st; ps; compress2; ps; qrel; ps; compress2; ps; qreach -v; ps\"                                                                                                            "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	cmd = "alias qs1      \"qvar -I 96 -u; ps; qbf -P 96\"                                                                                                                                             "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs2      \"qvar -I 96 -u; qvar -I 97 -u; ps; qbf -P 96\"                                                                                                                              "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs3      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; ps; qbf -P 96\"                                                                                                               "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs4      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; ps; qbf -P 96\"                                                                                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs5      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; ps; qbf -P 96\"                                                                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs6      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; qvar -I 101 -u; ps; qbf -P 96\"                                                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs7      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; qvar -I 101 -u; qvar -I 102 -u; ps; qbf -P 96\"                                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs8      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; qvar -I 101 -u; qvar -I 102 -u; qvar -I 103 -u; ps; qbf -P 96\"                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qs9      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; qvar -I 101 -u; qvar -I 102 -u; qvar -I 103 -u; qvar -I 104 -u; ps; qbf -P 96\"                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias qsA      \"qvar -I 96 -u; qvar -I 97 -u; qvar -I 98 -u; qvar -I 99 -u; qvar -I 100 -u; qvar -I 101 -u; qvar -I 102 -u; qvar -I 103 -u; qvar -I 104 -u; qvar -I 105 -u; ps; qbf -P 96\""; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	cmd = "alias chnew    \"st; haig_start; resyn2; haig_use\"                                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias chnewrs  \"st; haig_start; resyn2rs; haig_use\"                                                                                                                                       "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	cmd = "alias stdsd    \"r test/6in.blif; st; ps; u; bdd; dsd -g; st; ps\"                                                                                                                          "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec     \"rec_start; r c.blif; st; rec_add; rec_use\"                                                                                                                                "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec4    \"rec_start -K 4; r i10.blif; st; rec_add; rec_use\"                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec5    \"rec_start -K 5; r i10.blif; st; rec_add; rec_use\"                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec6    \"rec_start -K 6; r i10.blif; st; rec_add; rec_use\"                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec7    \"rec_start -K 7; r i10.blif; st; rec_add; rec_use\"                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec8    \"rec_start -K 8; r i10.blif; st; rec_add; rec_use\"                                                                                                                         "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec10   \"rec_start -K 10; r i10.blif; st; rec_add; rec_use\"                                                                                                                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias trec12   \"rec_start -K 12; r i10.blif; st; rec_add; rec_use\"                                                                                                                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	cmd = "alias tst4     \"r i10_if4.blif; st; ps; r x/rec4_.blif; st; rec_start; r i10_if4.blif; st -r; ps; cec\"                                                                                    "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias tst4n    \"r i10_if4.blif; st; ps; r 5npn/all_functions.aig; st; rec_start; r i10_if4.blif; st -r; ps; cec\"                                                                          "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
	cmd = "alias tst6     \"r i10_if6.blif; st; ps; r x/rec6_16_.blif; st; rec_start; r i10_if6.blif; st -r; ps; cec\"                                                                                 "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );

	cmd = "alias t        \"r c.blif; st; drf\"                                                                                                                                                        "; if ( Cmd_CommandExecute(pAbc, cmd) ) printf( cmd );
#else

#if 0
    {
        char * sPath1, * sPath2;

         // If .rc is present in both the home and current directories, then read
         // it from the home directory.  Otherwise, read it from wherever it's located.
        sPath1 = Extra_UtilFileSearch(".rc", "~/", "r");
        sPath2 = Extra_UtilFileSearch(".rc", ".",  "r");
  
        if ( sPath1 && sPath2 ) {
            /* ~/.rc == .rc : Source the file only once */
            (void) Cmd_CommandExecute(pAbc, "source -s ~/.rc");
        }
        else {
            if (sPath1) {
                (void) Cmd_CommandExecute(pAbc, "source -s ~/.rc");
            }
            if (sPath2) {
                (void) Cmd_CommandExecute(pAbc, "source -s .rc");
            }
        }
        if ( sPath1 ) ABC_FREE(sPath1);
        if ( sPath2 ) ABC_FREE(sPath2);
    
        /* execute the abc script which can be open with the "open_path" */
        Cmd_CommandExecute( pAbc, "source -s abc.rc" );
    }
#endif

#ifdef ABC_PYTHON_EMBED
    if ( getenv("ABC_PYTHON_ABC_RC") )
    {
        /* read script file from $ABC_PYTHON_ABC_RC */

        char * sPath = getenv("ABC_PYTHON_ABC_RC");
        
        if (sPath){
            char * sCmd = ABC_ALLOC(char, strlen(sPath) + 50);
            (void) sprintf(sCmd, "source -s %s", sPath);
            (void) Cmd_CommandExecute(pAbc, sCmd);
            ABC_FREE(sCmd);
        }
    }
    else
#endif /* #ifdef ABC_PYTHON_EMBED */

    {
        char * sPath1, * sPath2;
        char * home;

         // If .rc is present in both the home and current directories, then read
         // it from the home directory.  Otherwise, read it from wherever it's located.
        home = getenv("HOME");
        if (home){
            char * sPath3 = ABC_ALLOC(char, strlen(home) + 2);
            (void) sprintf(sPath3, "%s/", home);
            sPath1 = Extra_UtilFileSearch(".abc.rc", sPath3, "r");
            ABC_FREE(sPath3);
        }else
            sPath1 = NULL;

        sPath2 = Extra_UtilFileSearch(".abc.rc", ".",  "r");

        if ( sPath1 && sPath2 ) {
            /* ~/.rc == .rc : Source the file only once */
            char *tmp_cmd = ABC_ALLOC(char, strlen(sPath1)+12);
            (void) sprintf(tmp_cmd, "source -s %s", sPath1);
            // (void) Cmd_CommandExecute(pAbc, "source -s ~/.abc.rc");
            (void) Cmd_CommandExecute(pAbc, tmp_cmd);
            ABC_FREE(tmp_cmd);
        }
        else {
            if (sPath1) {
                char *tmp_cmd = ABC_ALLOC(char, strlen(sPath1)+12);
                (void) sprintf(tmp_cmd, "source -s %s", sPath1);
                // (void) Cmd_CommandExecute(pAbc, "source -s ~/.abc.rc");
                (void) Cmd_CommandExecute(pAbc, tmp_cmd);
                ABC_FREE(tmp_cmd);
            }
            if (sPath2) {
                char *tmp_cmd = ABC_ALLOC(char, strlen(sPath2)+12);
                (void) sprintf(tmp_cmd, "source -s %s", sPath2);
                // (void) Cmd_CommandExecute(pAbc, "source -s .abc.rc");
                (void) Cmd_CommandExecute(pAbc, tmp_cmd);
                ABC_FREE(tmp_cmd);
            }
        }
        if ( sPath1 ) ABC_FREE(sPath1);
        if ( sPath2 ) ABC_FREE(sPath2);

        /* execute the abc script which can be open with the "open_path" */
        Cmd_CommandExecute( pAbc, "source -s abc.rc" );
    }
    
#endif //WIN32
}

/**Function********************************************************************

  Synopsis    [Returns the date in a brief format assuming its coming from
  the program `date'.]

  Description [optional]

  SideEffects []

******************************************************************************/
char * DateReadFromDateString( char * datestr )
{
  static char result[25];
  char        day[10];
  char        month[10];
  char        zone[10];
  char       *at;
  int         date;
  int         hour;
  int         minute;
  int         second;
  int         year;

  if (sscanf(datestr, "%s %s %2d %2d:%2d:%2d %s %4d",
             day, month, &date, &hour, &minute, &second, zone, &year) == 8) {
    if (hour >= 12) {
      if (hour >= 13) hour -= 12;
      at = "PM";
    }
    else {
      if (hour == 0) hour = 12;
      at = "AM";
    }
    (void) sprintf(result, "%d-%3s-%02d at %d:%02d %s", 
                   date, month, year % 100, hour, minute, at);
    return result;
  }
  else {
    return datestr;
  }
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

