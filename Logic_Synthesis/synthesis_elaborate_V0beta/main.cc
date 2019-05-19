
const char COPYRIGHT[] =
	"Copyright (c) 1998-2004 Stephen Williams (steve@icarus.com)";

/*
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
#ident "$Id: main.cc,v 1.86.2.3 2006/06/12 00:16:53 steve Exp $"
#endif

# include "config.h"

const char NOTICE[] =
	"  This program is free software; you can redistribute it and/or modify\n"
	"  it under the terms of the GNU General Public License as published by\n"
	"  the Free Software Foundation; either version 2 of the License, or\n"
	"  (at your option) any later version.\n"
	"\n"
	"  This program is distributed in the hope that it will be useful,\n"
	"  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"  GNU General Public License for more details.\n"
	"\n"
	"  You should have received a copy of the GNU General Public License\n"
	"  along with this program; if not, write to the Free Software\n"
	"  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA\n"
	;

# include  <stdio.h>
# include  <iostream>
# include  <fstream>
# include  <queue>
# include  <list>
# include  <map>
#include <string>
//# include  <unistd.h>
# include  <stdlib.h>
#if defined(HAVE_TIMES)
# include  <sys/times.h>
#endif
//#if defined(HAVE_GETOPT_H)
//# include  <getopt.h>
//#endif
# include  "pform.h"
# include  "parse_api.h"
# include  "netlist.h"
# include  "target.h"
# include  "compiler.h"
# include "global.h"

#include <cstring>

#include"RunInfoRecord.h"

#if defined(__MINGW32__) && !defined(HAVE_GETOPT_H)
extern "C" int getopt(int argc, char*argv[], const char*fmt);
extern "C" int optind;
extern "C" const char*optarg;
#endif

#if defined(__CYGWIN32__) && !defined(HAVE_GETOPT_H)
extern "C" int getopt(int argc, char*argv[], const char*fmt);
extern "C" int optind;
extern "C" const char*optarg;
#endif

/* Count errors detected in flag processing. */
unsigned flag_errors = 0;

const char VERSION[] = "$Name:  $";

const char*basedir = ".";

const char*target = "dll";
//const char*target = "xnf";

generation_t generation_flag = GN_DEFAULT;

map<string,const char*> flags;
char vpi_module_list[100] = {0};

map<perm_string,unsigned> missing_modules;

list<const char*> library_suff;

list<perm_string> roots;

char*ivlpp_string = 0;

char* depfile_name = NULL;
FILE *depend_file = NULL;

////添加用户文件路径全局变量
//extern "C"{
	char user_in_file_path_name[1024] = {'\0'};
	const char* base = 0;	
	char user_out_file_path_name[1024] = {'\0'};
	char syn_message[1024] = {'\0'};
	const char*edif_blif = "blif";
	//const char*edif_blif = "edif";
//}


/*
* These are the warning enable flags.
*/
bool warn_implicit  = true;
bool warn_timescale = true;
bool warn_portbinding = true;
bool warn_unused = true;

bool error_implicit = false;

/*
* Debug message class flags.
*/
bool debug_scopes = false;
bool debug_eval_tree = false;
bool debug_elaborate = false;
bool debug_synth = false;
bool debug_cprop = false;

/*
* Verbose messages enabled.
*/
bool verbose_flag = true;

/*
* Keep a heap of identifier strings that I encounter. This is a more
* efficient way to allocate those strings.
*/
StringHeapLex lex_strings;

/*
* In library searches, Windows file names are never case sensitive.
*/
#if defined(__MINGW32__)
const bool CASE_SENSITIVE = false;
#else
const bool CASE_SENSITIVE = true;
#endif

/*
* Are we doing synthesis?
*/
bool synthesis = true;

extern void cprop(Design*des);
extern void synth(Design*des);
extern void synth2(Design*des);
//extern void syn_rules(Design*des);
extern void nodangle(Design*des);
extern void xnfio(Design*des);
extern void del_redundance(Design*des);
extern void detect_bufif(Design*des);
extern void NetMemory2NetFF(Design*des);
extern void FF_D_connected_Q(Design*des);
extern void insert_outport_buffer(Design*des);

typedef void (*net_func)(Design*);
static struct net_func_map {
	const char*name;
	void (*func)(Design*);
} func_table[] = {
	{ "cprop",   &cprop },
	{ "nodangle",&nodangle },
	{ "synth",   &synth },
	{ "synth2",  &synth2 },	
	//{ "syn-rules",   &syn_rules },
	{ "xnfio",   &xnfio },
	{ "del-redundance",   &del_redundance },	
	{ "detect-bufif",   &detect_bufif },
	{ "NetMemory2NetFF",   &NetMemory2NetFF },
	{ "FF-D-connected-Q",  &FF_D_connected_Q },
	{ "insert-outport-buffer",   &insert_outport_buffer },
	{ 0, 0 }
};

queue<net_func> net_func_queue;

net_func name_to_net_func(const string&name)
{
	for (unsigned idx = 0 ;  func_table[idx].name ;  idx += 1)
		if (name == func_table[idx].name)
			return func_table[idx].func;

	return 0;
}

const char *net_func_to_name(const net_func func)
{
	for (unsigned idx = 0 ;  func_table[idx].name ;  idx += 1)
		if (func == func_table[idx].func)
			return func_table[idx].name;

	return "This cannot happen";
}

static void process_generation_flag(const char*gen)
{
	if (strcmp(gen,"1") == 0)
		generation_flag = GN_VER1995;

	else if (strcmp(gen,"2") == 0)
		generation_flag = GN_VER2001;

	else if (strcmp(gen,"3.0") == 0)
		generation_flag = GN_SYSVER30;

	else
		generation_flag = GN_DEFAULT;
}



static void parm_to_flagmap(const string&flag)
{
	string key;
	const char*value;
	unsigned off = flag.find('=');
	if (off > flag.size()) {
		key = flag;
		value = "";

	} else {
		key = flag.substr(0, off);
		value = strdup(flag.substr(off+1).c_str());
	}

	flags[key] = value;
}

static void read_iconfig_file()
{
	string parm = "DLL=fpga.tgt.dll";
	parm_to_flagmap(parm);

	net_func tmp;
	tmp = name_to_net_func("synth2");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``synth2''." << endl;

	tmp = name_to_net_func("FF-D-connected-Q");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``FF-D-connected-Q''." << endl;

	tmp = name_to_net_func("NetMemory2NetFF");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``NetFF2NetMemory''." << endl;

	tmp = name_to_net_func("synth");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``synth''." << endl;

	//tmp = name_to_net_func("syn-rules");
	//net_func_queue.push(tmp);
	//if (tmp == 0)
	//	cerr << "No such design transform function ``syn-rules''." << endl;

	tmp = name_to_net_func("cprop");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``cprop''." << endl;

	tmp = name_to_net_func("nodangle");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``nodangle''." << endl;

	//tmp = name_to_net_func("del-redundance");
	//net_func_queue.push(tmp);
	//if (tmp == 0)
	//	cerr << "No such design transform function ``del-redundance''." << endl;

	tmp = name_to_net_func("detect-bufif");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``detect-bufif''." << endl;

	tmp = name_to_net_func("insert-outport-buffer");
	net_func_queue.push(tmp);
	if (tmp == 0)
		cerr << "No such design transform function ``detect-input-output-linked-derectly''." << endl;
}

extern Design* elaborate(list <perm_string> root);

#if defined(HAVE_TIMES)
static double cycles_diff(struct tms *a, struct tms *b)
{
	clock_t aa = a->tms_utime
		+      a->tms_stime
		+      a->tms_cutime
		+      a->tms_cstime;

	clock_t bb = b->tms_utime
		+      b->tms_stime
		+      b->tms_cutime
		+      b->tms_cstime;

	return (aa-bb)/(double)sysconf(_SC_CLK_TCK);
}
#else // ! defined(HAVE_TIMES)
// Provide dummies
struct tms { int x; };
inline static void times(struct tms *) { }
inline static double cycles_diff(struct tms *a, struct tms *b) { return 0; }
#endif // ! defined(HAVE_TIMES)

int main(int argc, char *argv[])
{
	bool help_flag = false;
	bool times_flag = false;
#          if defined(HAVE_TIMES)
	times_flag = true;
#          endif



	struct tms cycles[5];
	base = argv[1];

	char system_path_exe_[4096];//get the system path
	GetModuleFileNameA(NULL,system_path_exe_,4096);
	string system_path_exe = system_path_exe_;
	long loc1 = system_path_exe.rfind("\\");
	string system_path(system_path_exe,0,loc1);
	

	strcat(syn_message,base);
	strcat(syn_message,"\\synReport.txt");	

	InitRunInfoRecordStatic(syn_message ,0); 

	library_suff.push_back(".v");

	//vpi_module_list = strdup("system");	
	strcpy(vpi_module_list,"system");
	assert(strcmp(vpi_module_list,"system")==0);

	flags["VPI_MODULE_LIST"] = vpi_module_list;
	min_typ_max_flag = TYP;
	min_typ_max_warn = 10;

	read_iconfig_file();

	if (verbose_flag) {
		if (times_flag)
			times(cycles+0);
		CRunInfoRecord::Write("  1. Write\n" );
		CRunInfoRecord::WriteInfo("  2. WriteInfo\n" );
		CRunInfoRecord::WriteWarning("  3.WriteWarning\n" );
		CRunInfoRecord::WriteErro("  4. WriteErro\n" );
		cout << endl << "PARSING INPUT" << endl;
	}

	if(argc == 3){
		cout << "You have specified the top module: " << argv[2] << "." <<endl;
		roots.push_back(argv[2]);
	}else if(argc == 2){
		cout << "Because of unspecified top module, the synthesis tool will detect the top module auto." << endl;
	}else{
		cout << "The correct command parameter is: work_path [project_name]." << endl;
	}


	strcat(user_in_file_path_name,base);
	strcat(user_in_file_path_name,"\\");
	strcat(user_in_file_path_name,argv[2]);
	strcat(user_in_file_path_name,".pp");

	string state_file = base;
	state_file += "\\syn_st";

	/* Parse the input. Make the pform. */ 
	int rc = pform_parse(user_in_file_path_name);

	if (rc) {
		return rc;
	}

	/* Check IP cores here!!! -- CCC */
	if (verbose_flag)
		cout << "CHECKING IP CORES" << endl;
	map<perm_string,Module*>::iterator mod;
	for (mod = pform_modules.begin()
		; mod != pform_modules.end()
		; mod++) {

			list<PGate*> gates = (*mod).second->get_gates();
			list<PGate*>::const_iterator gate;

			static bool pll_parse_flag = false;
			for (gate = gates.begin(); !pll_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"altpll") == 0) {
						cout << "  comet_pll(s)" << endl;
												
						pll_parse_flag = true;
						string system_path_altpll = system_path + "\\altpll.v";
						rc = pform_parse(system_path_altpll.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

			static bool M4K_parse_flag = false;
			for (gate = gates.begin(); !M4K_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"altsyncram") == 0) {
						cout << "  memory core(s)" << endl;

						M4K_parse_flag = true;
						string system_path_altsyncram = system_path + "\\altsyncram.v";
						rc = pform_parse(system_path_altsyncram.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

			static bool square_parse_flag = false;
			for (gate = gates.begin(); !square_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"altsquare") == 0) {
						cout << "  DSP core(s): square" << endl;

						square_parse_flag = true;
						string system_path_altsquare = system_path + "\\altsquare.v";
						rc = pform_parse(system_path_altsquare.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

			static bool mul_parse_flag = false;
			for (gate = gates.begin(); !mul_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"lpm_mult") == 0) {
						cout << "  DSP core(s): lpm_mult" << endl;
						
						mul_parse_flag = true;
						string system_path_lpm_mult = system_path + "\\lpm_mult.v";
						rc = pform_parse(system_path_lpm_mult.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

			static bool mul_add_parse_flag = false;
			for (gate = gates.begin(); !mul_add_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"altmult_add") == 0) {
						cout << "  DSP core(s): altmult_add" << endl;

						mul_add_parse_flag = true;
						string system_path_altmult_add = system_path + "\\altmult_add.v";
						rc = pform_parse(system_path_altmult_add.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

			static bool mul_accum_parse_flag = false;
			for (gate = gates.begin(); !mul_accum_parse_flag && gate != gates.end(); gate++) {
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) {
					if (strcmp(mod->get_type(),"altmult_accum") == 0) {
						cout << "  DSP core(s): altmult_accum" << endl;

						mul_accum_parse_flag = true;
						string system_path_altmult_accum = system_path + "\\altmult_accum.v";
						rc = pform_parse(system_path_altmult_accum.c_str());

						if (rc) {
							return rc;
						}
						break;
					}
				}
			}

	}


	/* If the user did not give specific module(s) to start with,
	then look for modules that are not instantiated anywhere.  */
	string output_name;
	if (roots.empty()) {
		map<perm_string,bool> mentioned_p;
		map<perm_string,Module*>::iterator mod;
		if (verbose_flag)
			cout << "LOCATING TOP-LEVEL MODULES" << endl << "  ";
		for (mod = pform_modules.begin()
			; mod != pform_modules.end()
			; mod++) {
				list<PGate*> gates = (*mod).second->get_gates();
				list<PGate*>::const_iterator gate;
				for (gate = gates.begin(); gate != gates.end(); gate++) {
					PGModule *mod = dynamic_cast<PGModule*>(*gate);
					if (mod) {
						// Note that this module has been instantiated
						mentioned_p[mod->get_type()] = true;
					}
				}
		}

		for (mod = pform_modules.begin()
			; mod != pform_modules.end()
			; mod++) {
				if (mentioned_p[(*mod).second->mod_name()] == false) {
					//char tmp[100];
					//strcpy(tmp,(*mod).second->mod_name());
					if (strcmp(edif_blif, "edif") == 0) {
						//flags["-o"] = strcat(tmp,".edf");
						flags["-o"] = "syn.edf";
					}
					else if (strcmp(edif_blif, "blif") == 0) {
						//flags["-o"] = strcat(tmp,".blif");
						flags["-o"] = "syn.blif";
					}
					else
						assert(0);
					if (verbose_flag)
						cout << " " << (*mod).second->mod_name();
					roots.push_back((*mod).second->mod_name());
				}
		}
		if (verbose_flag)
			cout << endl;
	}else{
		assert(argc == 3);
		output_name = argv[2];
		if (strcmp(edif_blif, "edif") == 0) {
			output_name += ".edf";
		}
		else if (strcmp(edif_blif, "blif") == 0) {
			output_name += ".blif";
		}
		else
			assert(0);
		
		flags["-o"] = output_name.c_str();
	}

	/* If there is *still* no guess for the root module, then give
	up completely, and complain. */

	if (roots.empty()) {
		cerr << "No top level modules, and no -s option." << endl;
		return 1;
	}


	if (verbose_flag) {
		if (times_flag) {
			times(cycles+1);
			cerr<<" ... done, "
				<<cycles_diff(cycles+1, cycles+0)<<" seconds."<<endl;
		}
		cout << "ELABORATING DESIGN" << endl;
	}

	/* On with the process of elaborating the module. */
	Design*des = elaborate(roots);

	if ((des == 0) || (des->errors > 0)) {
		if (des != 0) {
			cerr << des->errors
				<< " error(s) during elaboration." << endl;
		} else {
			cerr << "Elaboration failed" << endl;
		}

		goto errors_summary;
	}

	des->set_flags(flags);

	/* Done iwth all the pform data. Delete the modules. */
	for (map<perm_string,Module*>::iterator idx = pform_modules.begin()
		; idx != pform_modules.end() ; idx ++) {

			delete (*idx).second;
			(*idx).second = 0;
	}

	if (verbose_flag) {
		if (times_flag) {
			times(cycles+2);
			cerr<<" ... done, "
				<<cycles_diff(cycles+2, cycles+1)<<" seconds."<<endl;
		}
		cout << "RUNNING FUNCTORS" << endl;
	}

	while (!net_func_queue.empty()) {
		net_func func = net_func_queue.front();
		net_func_queue.pop();
		if (verbose_flag)
			cerr<<" -F "<<net_func_to_name(func)<< " ..." <<endl;
		func(des);
	}

	if (des->errors) {
		cerr << des->errors
			<< " error(s) in post-elaboration processing." <<
			endl;
		return des->errors;
	}

	if (verbose_flag) {
		if (times_flag) {
			times(cycles+3);
			cerr<<" ... done, "
				<<cycles_diff(cycles+3, cycles+2)<<" seconds."<<endl;
		}
	}

	if (verbose_flag) {
		cout << "CODE GENERATION -t "<<target<< endl;
	}

	bool emit_rc;
	emit_rc = emit(des, target);
	if (!emit_rc) {
		cerr << "error: Code generation had errors." << endl;
		return 1;
	}
	

	if (verbose_flag) {
		if (times_flag) {
			times(cycles+4);
			cerr<<" ... done, "
				<<cycles_diff(cycles+4, cycles+3)<<" seconds."<<endl;
		} else {
			cout << "DONE." << endl;
		}
	}

	if (verbose_flag) {
		cout << "STATISTICS" << endl;
		cout << "lex_string:"
			<< " add_count=" << lex_strings.add_count()
			<< " hit_count=" << lex_strings.add_hit_count()
			<< endl;
	}

	FILE* syn_st = fopen(state_file.c_str(), "w");//综合状态输出文件
	if (syn_st == 0) {
		cerr << "error: Cannot open the file \"syn_st\" "<< endl;
		return -1;
	}
	fprintf(syn_st, "2");

	fclose(syn_st);
	syn_st = 0;

	return 0;

errors_summary:
	if (missing_modules.size() > 0) {
		cerr << "*** These modules were missing:" << endl;

		map<perm_string,unsigned>::const_iterator idx;
		for (idx = missing_modules.begin()
			; idx != missing_modules.end()
			; idx ++)
			cerr << "        " << (*idx).first
			<< " referenced " << (*idx).second
			<< " times."<< endl;

		cerr << "***" << endl;
	}

	return des? des->errors : 1;
}

/*
* $Log: main.cc,v $
* Revision 1.86.2.3  2006/06/12 00:16:53  steve
*  Add support for -Wunused warnings.
*
* Revision 1.86.2.2  2006/04/23 04:25:45  steve
*  Add cprop debugging.
*
* Revision 1.86.2.1  2006/04/01 01:37:24  steve
*  Add synth debug flag
*
* Revision 1.86  2004/10/04 01:10:53  steve
*  Clean up spurious trailing white space.
*
* Revision 1.85  2004/09/25 01:58:44  steve
*  Add a debug_elaborate flag
*
* Revision 1.84  2004/09/10 23:51:42  steve
*  Fix the evaluation of constant ternary expressions.
*
* Revision 1.83  2004/09/05 17:44:42  steve
*  Add support for module instance arrays.
*
* Revision 1.82  2004/03/10 04:51:24  steve
*  Add support for system function table files.
*
* Revision 1.81  2004/02/18 17:11:56  steve
*  Use perm_strings for named langiage items.
*
* Revision 1.80  2004/02/15 00:19:29  steve
*  Report elaboration errors without crashing.
*
* Revision 1.79  2003/11/26 01:37:14  steve
*  Properly initialize vpi_module_list with system.
*
* Revision 1.78  2003/11/13 05:55:33  steve
*  Move the DLL= flag to target config files.
*
* Revision 1.77  2003/11/13 04:09:49  steve
*  Pass flags through the temporary config file.
*
* Revision 1.76  2003/11/13 03:10:38  steve
*  ivl -F and -t flags are onpassed throught the -C file.
*
* Revision 1.75  2003/11/10 20:59:03  steve
*  Design::get_flag returns const char* instead of string.
*
* Revision 1.74  2003/11/01 04:22:30  steve
*  Accept functors in the config file.
*
* Revision 1.73  2003/10/26 22:43:42  steve
*  Improve -V messages,
*
* Revision 1.72  2003/09/26 02:17:14  steve
*  Delete pform when done with it.
*
* Revision 1.71  2003/09/25 00:25:14  steve
*  Summary list of missing modules.
*
* Revision 1.70  2003/09/23 05:57:36  steve
*  Pass -m flag from driver via iconfig file.
*
* Revision 1.69  2003/09/22 01:12:08  steve
*  Pass more ivl arguments through the iconfig file.
*
* Revision 1.68  2003/06/20 00:53:19  steve
*  Module attributes from the parser
*  through to elaborated form.
*
* Revision 1.67  2003/04/24 05:25:27  steve
*  Dump design even on errors.
*
* Revision 1.66  2003/03/01 06:25:30  steve
*  Add the lex_strings string handler, and put
*  scope names and system task/function names
*  into this table. Also, permallocate event
*  names from the beginning.
*/

