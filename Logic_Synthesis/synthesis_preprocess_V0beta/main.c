const char COPYRIGHT[] =
	"Copyright (c) 1999 Stephen Williams (steve@icarus.com)";
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
#ident "$Id: main.c,v 1.20.2.1 2006/06/27 01:37:14 steve Exp $"
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

const char VERSION[] = "$Name:  $ $State: Exp $";

# include  <stdio.h>
# include  <stdlib.h>
#ifdef HAVE_MALLOC_H
# include  <malloc.h>
#endif
//# include  <unistd.h>
# include  <string.h>
# include  <ctype.h>
#if defined(HAVE_GETOPT_H)
//# include  <getopt.h>
#endif
# include  "globals.h"

#define __MINGW32__ 1
#ifdef __MINGW32__
#include <windows.h>
#ifdef HAVE_LIBIBERTY_H
#include <libiberty.h>
#endif
#endif

#if defined(__MINGW32__) && !defined(HAVE_GETOPT_H)
extern int getopt(int argc, char*argv[], const char*fmt);
extern int optind;
extern const char*optarg;
#endif

/*
* This function fills in th ivl_root directory with the appropriate
* path, based on operating system, build time configuration, and
* environment.
*/
static char* get_root_dir()
{
	static char ivl_root[4096];//static char *ivl_root[4096] -> static char ivl_root[4096]
	char * s;
	char basepath[1024];
	GetModuleFileNameA(NULL,basepath,1024);

	strncpy(ivl_root, basepath, 4096);
	s = strrchr(ivl_root, '\\');
	if (s) *s = 0;

	return ivl_root;
}

/*
* Keep in source_list an array of pointers to file names. The array
* is terminated by a pointer to null.
*/
static char**source_list = 0;
static unsigned source_cnt = 0;

void add_source_file(const char *base, const char*name)
{
	if (source_list == 0) {
		source_list = calloc(2, sizeof(char*));

		{   //注意这里的source_list[i]空间没有释放,暂时留给系统处理吧
			int length = strlen(base) + strlen("\\") + strlen(name) + 1;
			source_list[0] = (char*)malloc(length * sizeof(char));
			memset(source_list[0],'\0',length);
			strcat(source_list[0],base);
			strcat(source_list[0],"\\");
			strcat(source_list[0],name);
		}
		source_list[1] = 0;
		source_cnt = 1;
	} else {
		source_list = realloc(source_list, sizeof(char*) * (source_cnt+2));
		{
			int length = strlen(base) + strlen("\\") + strlen(name) + 1;
			source_list[source_cnt+0] = (char*)malloc(length * sizeof(char));
			memset(source_list[source_cnt+0],'\0',length);
			strcat(source_list[source_cnt+0],base);
			strcat(source_list[source_cnt+0],"\\");
			strcat(source_list[source_cnt+0],name);
		}
		source_list[source_cnt+1] = 0;
		source_cnt += 1;
	}
}

char**include_dir = 0;
unsigned include_cnt = 0;

int line_direct_flag = 0;

unsigned error_count = 0;
FILE *depend_file = NULL;

/*
* This function reads from a file a list of file names. Each name
* starts with the first non-space character, and ends with the last
* non-space character. Spaces in the middle are OK.
*/
static int flist_read_names(const char*base, const char*path)
{
	char line_buf[2048];

	FILE*fd = fopen(path, "r");
	if (fd == 0) {
		fprintf(stderr, "%s: unable to open for reading.\n", path);
		return 1;
	}

	while (fgets(line_buf, sizeof line_buf, fd) != 0) {
		char* current_word = strtok(line_buf," \t\n");
		char* last_word = current_word;
		char * cp = 0;
		char*tail = 0;
		while(current_word!=NULL)
		{
			last_word = current_word;
			current_word = strtok(NULL," \t\n");
		}
		if(0 == last_word)
			continue;
		tail = last_word + strlen(last_word);
		if(0 != last_word && 'v' == tail[-1]){
			add_source_file(base,last_word);
		}
	}

	return 0;
}


//argv[1] 用户文件路径
//argv[2] 工程名
int main(int argc, char *argv[])
{
	//const char*base = get_root_dir();
	const char*base = 0;
	char * projectName = 0;
	char flist_path[1024] = {'\0'};
	char out_path[1024] = {'\0'};
	char syn_state_file[1024] = {'\0'};
	unsigned flag_errors = 0;
	const char*dep_path = NULL;
	FILE*out = 0;
	FILE* syn_st = 0;

	char tmp[4096] = {'\0'};
	if( argc <3 ) 
	{
		printf("Program Parameters are not enough!\n");
		printf("Please inputs the Project Directory:\n");				
		scanf("%s",&tmp);
		argv[1] = (char*)malloc( strlen(tmp)+1 );
		strcpy(argv[1],tmp);
		printf("Please inputs the Project Name:\n");
		scanf("%s",&tmp);
		argv[2] = (char*)malloc( strlen(tmp)+1 );
		strcpy(argv[2],tmp);
	}

	base = argv[1];
	projectName = argv[2];

	strcat(syn_state_file,base);
	strcat(syn_state_file,"\\syn_st");
	syn_st = fopen(syn_state_file, "w");//综合状态输出文件
	if (syn_st == 0) {
		fprintf(stderr, "error: Cannot open the file \"syn_st\".");
		return -1;
	}
	fprintf(syn_st, "0");

	fclose(syn_st);
	syn_st = 0;

	/* Define preprocessor keywords that I plan to just pass. */
	define_macro("celldefine",          "`celldefine", 1);
	define_macro("default_nettype",     "`default_nettype", 1);
	define_macro("delay_mode_distributed", "`delay_mode_distributed", 1);
	define_macro("delay_mode_unit",     "`delay_mode_unit", 1);
	define_macro("delay_mode_path",     "`delay_mode_path", 1);
	define_macro("disable_portfaults",  "`disable_portfaults", 1);
	define_macro("enable_portfaults",   "`enable_portfaults", 1);
	define_macro("endcelldefine",       "`endcelldefine", 1);
	define_macro("endprotect",          "`endprotect", 1);
	define_macro("nosuppress_faults",   "`nosuppress_faults", 1);
	define_macro("nounconnected_drive", "`nounconnected_drive", 1);
	define_macro("protect",             "`protect", 1);
	define_macro("resetall",            "`resetall", 1);
	define_macro("suppress_faults",     "`suppress_faults", 1);
	define_macro("timescale",           "`timescale", 1);
	define_macro("unconnected_drive",   "`unconnected_drive", 1);
	define_macro("uselib",              "`uselib", 1);

	//ADU:
	include_dir = (char**)malloc(sizeof(char *)*2);
	include_dir[0] = base;
	include_dir[1] = 0;
	include_cnt = 1;

	strcat(flist_path,base);
	strcat(flist_path,"\\");
	strcat(flist_path, projectName);
	strcat(flist_path,".qsf");


	strcat(out_path,base);
	strcat(out_path,"\\");
	strcat(out_path,projectName);
	strcat(out_path,".pp");
	//strcat(out_path, "\\syn.pp");

	if (flist_path) {
		int rc = flist_read_names(base,flist_path);
		if (rc != 0)
			return rc;
	}

	/* Figure out what to use for an output file. Write to stdout
	if no path is specified. */
	if (out_path) {
		out = fopen(out_path, "w");
		if (out == 0) {
			perror(out_path);
			exit(1);
		}
	} else {
		out = stdout;
	}

	if (source_cnt == 0) {
		printf("No input files given.\n");
		return 1;
	}
	/* Pass to the lexical analyzer the list of input file, and
	start the parser. */
	reset_lexor(out, source_list);

	if (yyparse()) return -1;
	if(error_count == 0){

		syn_st = fopen(syn_state_file, "w");//综合状态输出文件
		if (syn_st == 0) {
			fprintf(stderr, "error: Cannot open the file %s.",syn_state_file);
			return -1;
		}
		fprintf(syn_st, "1");

		fclose(syn_st);
		syn_st = 0;
	}
	return error_count;
}

/*
* $Log: main.c,v $
* Revision 1.20.2.1  2006/06/27 01:37:14  steve
*  Fix const/non-const warnings.
*
* Revision 1.20  2004/09/10 00:15:45  steve
*  Remove bad casts.
*
* Revision 1.19  2004/09/05 21:29:08  steve
*  Better type safety.
*
* Revision 1.18  2004/02/15 18:03:30  steve
*  Cleanup of warnings.
*
* Revision 1.17  2003/09/26 02:08:31  steve
*  Detect missing endif markers.
*
* Revision 1.16  2002/08/12 01:35:02  steve
*  conditional ident string using autoconfig.
*
* Revision 1.15  2002/04/04 05:26:13  steve
*  Add dependency generation.
*
* Revision 1.14  2001/11/21 02:59:27  steve
*  Remove diag print.
*
* Revision 1.13  2001/11/21 02:20:35  steve
*  Pass list of file to ivlpp via temporary file.
*
* Revision 1.12  2001/09/15 18:27:04  steve
*  Make configure detect malloc.h
*
* Revision 1.11  2001/07/25 03:10:50  steve
*  Create a config.h.in file to hold all the config
*  junk, and support gcc 3.0. (Stephan Boettcher)
*
* Revision 1.10  2001/06/23 18:41:02  steve
*  Include stdlib.h
*
* Revision 1.9  2001/05/20 18:08:07  steve
*  local declares if the header is missing.
*/

