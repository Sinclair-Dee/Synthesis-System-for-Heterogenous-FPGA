#include "BlifToVQM.h"
//#include <iostream>
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char* argv[]){

	//	new BlifToVQM(argv[1], argv[2]);

	//BlifToVQM * bv = new BlifToVQM("E:\\B2V\\BLIF_to_VQM\\fromFile.txt", 
	//	"E:\\B2V\\BLIF_to_VQM\\toFile.txt", "E:\\B2V\\BLIF_to_VQM\\map.vqm");

	//BlifToVQM * bv = new BlifToVQM("E:\\MyPro\\inputFile\\test7\\map.blif", 
	//	"E:\\MyPro\\inputFile\\test7\\toFile.txt", "E:\\MyPro\\inputFile\\test7\\map.vqm");

	//cout << argc << endl;

	BlifToVQM * bv;

	try 
	{
		if (argc == 1)
		{
			bv = new BlifToVQM("map.blif", "", "map.vqm", "");
		}else if(argc == 2){
			bv = new BlifToVQM(argv[1], "E:\\MyPro\\inputFile\\test7\\toFile.txt", "map.vqm", "");
		}else{
			//bv = new BlifToVQM(argv[1], "E:\\MyPro\\inputFile\\test7\\toFile.txt", argv[2], argv[3]);
			bv = new BlifToVQM(argv[1], "", argv[2], argv[3]);
		}

		if (bv != NULL)
		{
			delete bv;
			bv = 0;			
		}
	}
	catch (BlifToVQM::Err)
	{
		// cout << "Error: No File found!!!" << endl;
	}
 
	//getchar();
	
	return 0;
}
