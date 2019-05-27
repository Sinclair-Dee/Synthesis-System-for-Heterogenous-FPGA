#include <assert.h>
#include "Cpp_Interface.h"
#include "vqm/BlifToVQM.h"


void BLIF2VQM(char * infile, char * outfile, char *project_path)
{
	BlifToVQM * bv;
	std::string str1(infile), str2(outfile), str3(project_path);
	try
	{
		bv = new BlifToVQM(str1, "", str2, str3);
		assert(bv != NULL);
		delete bv;
		bv = NULL;
	}
	catch (BlifToVQM::Err)
	{
		assert(0);
	}

}

int IoCommandWriteVQM( Abc_Frame_t *pAbc, int argc, char **argv )
{
	//char *blif, *vqm;
	if(argc != 4)
		goto usage;
	//blif = argv[1];
	//vqm  = argv[2];
	printf("Waiting for VQM-transfer...");
	BLIF2VQM(argv[1], argv[2], argv[3]);
	return 0;

usage:
	fprintf( pAbc->Err, "usage: write_vqm <org.blif> <file.vqm> <verilog-file path>\n" );
	return 1;
}