/*////////////////////////////////////////////////////////////////////////////

ABC: System for Sequential Synthesis and Verification

http://www.eecs.berkeley.edu/~alanmi/abc/

Copyright (c) The Regents of the University of California. All rights reserved.

Permission is hereby granted, without written agreement and without license or
royalty fees, to use, copy, modify, and distribute this software and its
documentation for any purpose, provided that the above copyright notice and
the following two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF
THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS,
AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

////////////////////////////////////////////////////////////////////////////*/

/**CFile****************************************************************

  FileName    [main.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [The main package.]

  Synopsis    [Here everything starts.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: main.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/


#ifdef ABC_PYTHON_EMBED
#include <Python.h>
#endif /* ABC_PYTHON_EMBED */

#include "base/abc/abc.h"
#include "mainInt.h"

ABC_NAMESPACE_IMPL_START

// this line should be included in the library project
//#define ABC_LIB

//#define ABC_USE_BINARY 1

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static int TypeCheck( Abc_Frame_t * pAbc, const char * s);

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

#ifndef ABC_LIB

/**Function*************************************************************

  Synopsis    [The main() procedure.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int Abc_RealMain( int argc, char * argv[] )
{
    Abc_Frame_t * pAbc;
    char sCommandUsr[500] = {0}, sCommandTmp[100], sReadCmd[20], sWriteCmd[20];
    const char * sOutFile, * sInFile;
    char * sCommand;
    int  fStatus = 0;
    int c, fBatch, fInitSource, fInitRead, fFinalWrite;

    // added to detect memory leaks
    // watch for {,,msvcrtd.dll}*__p__crtBreakAlloc()
    // (http://support.microsoft.com/kb/151585)
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // get global frame (singleton pattern)
    // will be initialized on first call
    pAbc = Abc_FrameGetGlobalFrame();
    pAbc->sBinary = argv[0];

#ifdef ABC_PYTHON_EMBED
    {
        PyObject* pModule;
        void init_pyabc(void);

        Py_SetProgramName(argv[0]);
        Py_NoSiteFlag = 1;
        Py_Initialize();

        init_pyabc();

        pModule = PyImport_ImportModule("pyabc");
        if (pModule)
        {
            Py_DECREF(pModule);
        }
        else
        {
            fprintf( pAbc->Err, "error: pyabc.py not found. PYTHONPATH may not be set properly.\n");
        }
    }
#endif /* ABC_PYTHON_EMBED */

    // default options
    fBatch      = 0;
    fInitSource = 1;
    fInitRead   = 0;
    fFinalWrite = 0;
    sInFile = sOutFile = NULL;
    sprintf( sReadCmd,  "read"  );
    sprintf( sWriteCmd, "write" );

    Extra_UtilGetoptReset();

   // source the resource file
    if ( fInitSource )
    {
        Abc_UtilsSource( pAbc );
    }

    if ( !fBatch )
    {
        // start interactive mode

        // print the hello line
        Abc_UtilsPrintHello( pAbc );
        // print history of the recent commands
        Cmd_HistoryPrint( pAbc, 10 );

        // source the resource file
		/////////////////////////////////////////////////
		// the resource file is hardened
		// change the command to source in mainUtils.c
		// -- CCC
        if ( fInitSource )
        {
            Abc_UtilsSource( pAbc );
        }
        //Fixed Flow of Mapping for Passkey
        {
			char * sCommand;
			char src_file[500],  dest_file[500], vqm_file[500];
			char sReadCommand[1024], sWriteCommand[1024], sVqmConvert[1024];
			char sTmp[1024];
			int  i = 0;
			int  nd = 0, nd_tmp = 0;
			int  net = 0;
			int  lev = 0;
			int  best = 0;
			FILE *syn_st;
			char tmp[1024];

			sprintf(tmp,"%s\\syn_st",argv[1]);
			syn_st = fopen(tmp,"r");
			//if (syn_st == 0) {
			//	fprintf(stderr, "error: Cannot open the file \"syn_st\".\n");
			//	return -1;
			//}
			//else 
			//{
			//	char flag = fgetc(syn_st);
			//	if( '2'!= flag ) {
			//		fprintf(stderr,"Error: The status of syn_st is not correct, maybe synthesis doesn't finish correctly!\n");
			//		fclose(syn_st);
			//		return -1;
			//	}
			//}

			// Put project path into filename so program can read source file
			sprintf(src_file,"\"%s\\%s%s\"",argv[1],argv[2],".blif");
			sprintf(dest_file,"\"%s\\%s%s\"",argv[1],argv[2],"_map.blif");
			sprintf(vqm_file,"\"%s\\%s%s\"",argv[1],argv[2],".vqm");
			sprintf(sReadCommand,"read %s",src_file);
			sprintf(sWriteCommand, "write_hie %s %s;", src_file, dest_file);
			sprintf(sVqmConvert,"write_vqm %s %s %s;",dest_file, vqm_file, argv[1]);

           // execute the reading file command
			fStatus = Cmd_CommandExecute( pAbc,sReadCommand);
			fStatus = Cmd_CommandExecute( pAbc,"ps");
			if(Abc_NtkNodeNum(pAbc->pNtkCur)==0)
			{
				printf("The input file only contains Macros, please check your inputs!\n");
				assert(0);
				return 1;
			}
			sCommand = "cleanup;strash;";
			fStatus = Cmd_CommandExecute( pAbc, sCommand );
		   // loop for logic resynthesis optimization
			//printf("\nNow Start Iterative Logic Re-Synthesis for Optimization\n");
			//while(best == 0) {
			//	sCommand = "resyn2; ps;";
			//	fStatus = Cmd_CommandExecute( pAbc, sCommand );
			//	if ( nd == 0 || (pAbc->pNtkCur->nObjs < nd))//( Abc_NtkNodeNum(pAbc->pNtkCur) < nd ) ) //modifed by jzh@2013-10-09,16:38
			//		nd = pAbc->pNtkCur->nObjs;//Abc_NtkNodeNum(pAbc->pNtkCur);
			//	else
			//		best = 1;
		 //    }
			
			/*start of iterative mapping process, the iterative optimization
			is done only if the results is the best in latest five iteraions*/ 
			printf("\nNow start iterative Mapping Solution\n");
			best = 0;
			nd = 0;
			i = 0;
			while (i < best + 5) {
			   sCommand = "choice; if -K 4 -o; mfs; ps;";
			   fStatus = Cmd_CommandExecute( pAbc, sCommand );
			  // assert( lev == 0 || Abc_NtkLevel(pAbc->pNtkCur) == lev );

			   if ( ( best == 0 && lev == 0 && nd == 0 && net == 0 ) 
				   || ( Abc_NtkLevel(pAbc->pNtkCur) < lev )
				   || ( Abc_NtkNodeNum(pAbc->pNtkCur) < nd ) 
				   || ( Abc_NtkNodeNum(pAbc->pNtkCur) == nd && Abc_NtkGetTotalFanins(pAbc->pNtkCur) < net ) ) 
			   {
				   do 
				   {	nd_tmp = Abc_NtkNodeNum(pAbc->pNtkCur);
						fStatus = Cmd_CommandExecute( pAbc, "mfs -d" );
				   }while(Abc_NtkNodeNum(pAbc->pNtkCur)<nd_tmp); // Use don't care optimization to reduce size of network until no gains can be get

				   fStatus = Cmd_CommandExecute( pAbc, sWriteCommand );
				   nd = Abc_NtkNodeNum(pAbc->pNtkCur);
				   net = Abc_NtkGetTotalFanins(pAbc->pNtkCur);
				   lev = Abc_NtkLevel(pAbc->pNtkCur);
				   best = i;
			   }			   
			   i++;  
		   }
		  printf("\nMapping Phase ends...\n");

		  /*          sCommand = "cec;";
		  fStatus = Cmd_CommandExecute( pAbc, sCommand );  */ 
		  fStatus = Cmd_CommandExecute( pAbc, sVqmConvert );
		  fStatus = Cmd_CommandExecute( pAbc, "time" );

		  {
			  FILE *Report = fopen("resource.rpt","a");
			  fprintf(Report,"\nRun-Time : %5g\n",pAbc->TimeTotal);
			  fflush(Report);
			  fclose(Report);
		  }

		  freopen(tmp,"w",syn_st);
		  fprintf(syn_st, "3");
		  fclose(syn_st);
//#define REPORT
#ifdef REPORT
		  {
			  FILE *fp=fopen("data.rpt","a");
			  fprintf(fp,"Benchmark is %s\nLogic Depth is %d\nNode Number is : %d\nRun-time: %f\n\n",argv[2],lev,nd,pAbc->TimeTotal);
			  fflush(fp);
			  fclose(fp);
		  }
#endif
        }
        
        //* Code below is the part to recieve command string
		    // * for interactive , commented by jzh@213-07-21
        /*
        while ( !feof(stdin) )
        {
            // print command line prompt and
            // get the command from the user
            sCommand = Abc_UtilsGetUsersInput( pAbc );

            // execute the user's command
            fStatus = Cmd_CommandExecute( pAbc, sCommand );

            // stop if the user quitted or an error occurred
            if ( fStatus == -1 || fStatus == -2 )
                break;
        }
        */
    }

#ifdef ABC_PYTHON_EMBED
    {
        Py_Finalize();
    }
#endif /* ABC_PYTHON_EMBED */

    // if the memory should be freed, quit packages
//    if ( fStatus < 0 ) 
    {
        Abc_Stop();
    }
    return 0;

usage:
    Abc_UtilsPrintHello( pAbc );
    Abc_UtilsPrintUsage( pAbc, argv[0] );
    return 1;
}

#endif

/**Function********************************************************************

  Synopsis    [Returns 1 if s is a file type recognized, else returns 0.]

  Description [Returns 1 if s is a file type recognized by ABC, else returns 0. 
  Recognized types are "blif", "bench", "pla", and "none".]

  SideEffects []

******************************************************************************/
static int TypeCheck( Abc_Frame_t * pAbc, const char * s )
{
    if (strcmp(s, "blif") == 0)
        return 1;
    else if (strcmp(s, "bench") == 0)
        return 1;
    else if (strcmp(s, "pla") == 0)
        return 1;
    else if (strcmp(s, "none") == 0)
        return 1;
    else {
        fprintf( pAbc->Err, "unknown type %s\n", s );
        return 0;
    }
}




////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

#if defined(ABC_USE_BINARY)
int main_( int argc, char * argv[] )
#else
int main( int argc, char * argv[] )
#endif
{
  return ABC_NAMESPACE_PREFIX Abc_RealMain(argc, argv);
}
