/**CFile****************************************************************

  FileName    [Cpp_Interface.h]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [The BLIF-To-VQM package.]

  Synopsis    [External declarations of the BLIF-To-VQM package.]

  Author      [jzh]
  
  Affiliation [IECAS]

  Date        [Ver. 1.0. Started - Nov 18, 2013.]

  Revision    []

***********************************************************************/
#ifndef   __CPP_INTERFACE_HEAD_GUIDER__
#define   __CPP_INTERFACE_HEAD_GUIDER__

#include "base/main/mainInt.h"

#ifdef __cplusplus
extern "C" 
{
#endif
	void BLIF2VQM(char * infile, char * outfile, char *project_path);
	int IoCommandWriteVQM( Abc_Frame_t *pAbc, int argc, char **argv );
#ifdef __cplusplus
}
#endif

#endif //__CPP_INTERFACE_HEAD_GUIDER__