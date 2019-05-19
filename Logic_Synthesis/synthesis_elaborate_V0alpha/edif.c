/*
* Copyright (c) 200Stephen Williams (steve@icarus.com)
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
#ident "$Id: edif.c,v 1.8.2.1 2005/08/27 22:29:30 steve Exp $"
#endif

# include  "edif.h"
# include  <stdlib.h>
# include  <string.h>
#ifdef HAVE_MALLOC_H
# include  <malloc.h>
#endif
# include  <assert.h>
# include <math.h>
#include "global.h"

typedef enum property_e {
	PRP_NONE = 0,
	PRP_STRING,
	PRP_INTEGER
} property_t;

struct cellref_property_ {
	const char*name;
	property_t ptype;
	union {
		const char*str;
		long num;
	} value_;
	struct cellref_property_*next;
};

struct edif_s {
	const char*name;
	/* List the ports of the design. */
	unsigned nports;
	struct __cell_port*ports;
	/* All the external libraries attached to me. */
	edif_xlibrary_t xlibs;
	/* list the cellref instances. */
	edif_cellref_t celref;
	/* The root instance has cellref properties as well. */
	struct cellref_property_*property;
	/* Keep a list of all the nexa */
	struct edif_joint_s*nexa;
};

struct edif_xlibrary_s {
	/* Name of this library. */
	const char*name;
	/* The cells that are contained in this library. */
	struct edif_cell_s*cells;
	/* point to the optional celltable. */
	const struct edif_xlib_celltable*celltable;
	/* used to list libraries in an edif_t. */
	struct edif_xlibrary_s*next;
};


struct __cell_port {
	const char*name;
	const char*ename;
	struct cellref_property_*property;
	ivl_signal_port_t dir;
};

struct edif_cell_s {
	const char*name;
	edif_xlibrary_t xlib;

	unsigned nports;
	struct __cell_port*ports;

	struct cellref_property_*property;
	struct edif_cell_s*next;
};

struct edif_cellref_s {
	struct edif_cell_s* cell;
	unsigned u;
	struct cellref_property_*property;
	struct edif_cellref_s* next;
};

struct joint_cell_ {
	struct edif_cellref_s*cell;
	unsigned port;
	struct joint_cell_*next;
};

struct edif_joint_s {
	const char*name;
	struct joint_cell_*links;
	struct edif_joint_s*next;
};


static int is_edif_name(const char*text)
{
	static const char*edif_name_chars = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
	return (strspn(text, edif_name_chars) == strlen(text));
}

edif_t edif_create(const char*design_name, unsigned nports)
{edif_t edf=0;

edf = (struct edif_s *)malloc(sizeof(struct edif_s)); 


if(0 == edf ){
	printf("error\n");
}
edf->name  = design_name;
edf->nports= nports;
edf->ports = nports? calloc(nports, sizeof(struct __cell_port)) : 0;
edf->celref= 0;
edf->xlibs = 0;
edf->property = 0;
edf->nexa  = 0;
return edf;
}

void edif_portconfig(edif_t edf, unsigned idx,
	const char*name, ivl_signal_port_t dir)
{
	assert(idx < edf->nports);

	edf->ports[idx].name = name;
	if (is_edif_name(name)) {
		edf->ports[idx].ename = 0;

	} else {
		char buf[16];
		sprintf(buf, "PORT%u", idx);
		edf->ports[idx].ename = strdup(buf);
	}

	edf->ports[idx].dir  = dir;
}

void edif_port_to_joint(edif_joint_t jnt, edif_t edf, unsigned port)
{
	struct joint_cell_* jc = malloc(sizeof(struct joint_cell_));

	jc->cell = 0;
	jc->port = port;
	jc->next = jnt->links;
	jnt->links = jc;
}

void edif_pstring(edif_t edf, const char*name, const char*value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name = name;
	prp->ptype = PRP_STRING;
	prp->value_.str = value;
	prp->next = edf->property;
	edf->property = prp;
}

edif_xlibrary_t edif_xlibrary_create(edif_t edf, const char*name)
{
	edif_xlibrary_t xlib = malloc(sizeof(struct edif_xlibrary_s));

	xlib->name  = name;
	xlib->cells = 0;
	xlib->celltable = 0;
	xlib->next = edf->xlibs;
	edf->xlibs = xlib;

	return xlib;
}

void edif_xlibrary_set_celltable(edif_xlibrary_t xlib,
	const struct edif_xlib_celltable*tab)
{
	assert(xlib->celltable == 0);
	xlib->celltable = tab;
}

edif_cell_t edif_xlibrary_findcell(edif_xlibrary_t xlib,
	const char*cell_name)
{
	const struct edif_xlib_celltable*tcur;
	edif_cell_t cur;

	for (cur = xlib->cells ;  cur ;  cur = cur->next) {
		if (strcmp(cell_name, cur->name) == 0)
			return cur;
	}

	if (xlib->celltable == 0)
		return 0;

	for (tcur = xlib->celltable ;  tcur->cell_name ;  tcur += 1)
		if (strcmp(cell_name, tcur->cell_name) == 0) {
			return (tcur->cell_func)(xlib);
		}

		return 0;
}

edif_cell_t edif_xlibrary_scope_cell(edif_xlibrary_t xlib,
	ivl_scope_t scope)
{
	unsigned port_count, idx;
	edif_cell_t cur;

	/* Check to see if the cell is already somehow defined. */
	cur = edif_xlibrary_findcell(xlib, ivl_scope_tname(scope));
	if (cur) return cur;

	/* Count the ports of the scope. */
	port_count = 0;
	for (idx = 0 ;  idx < ivl_scope_sigs(scope) ;  idx += 1) {
		ivl_signal_t sig = ivl_scope_sig(scope, idx);

		if (ivl_signal_port(sig) == IVL_SIP_NONE)
			continue;

		port_count += 1;
	}

	cur = edif_xcell_create(xlib, ivl_scope_tname(scope), port_count);

	port_count = 0;
	for (idx = 0 ;  idx < ivl_scope_sigs(scope) ;  idx += 1) {
		ivl_signal_t sig = ivl_scope_sig(scope, idx);

		if (ivl_signal_port(sig) == IVL_SIP_NONE)
			continue;

		edif_cell_portconfig(cur, port_count,
			ivl_signal_basename(sig),
			ivl_signal_port(sig));
		port_count += 1;
	}

	return cur;
}

edif_cell_t edif_xcell_create(edif_xlibrary_t xlib, const char*name,
	unsigned nports)
{
	unsigned idx;
	edif_cell_t cell = malloc(sizeof(struct edif_cell_s));

	cell->name = name;
	cell->xlib = xlib;
	cell->nports = nports;
	cell->ports  = calloc(nports, sizeof(struct __cell_port));
	cell->property = 0;

	for (idx = 0 ;  idx < nports ;  idx += 1) {
		cell->ports[idx].name = "?";
		cell->ports[idx].dir = IVL_SIP_NONE;
		cell->ports[idx].property = 0;
	}

	cell->next = xlib->cells;
	xlib->cells = cell;

	return cell;
}

void edif_cell_portconfig(edif_cell_t cell, unsigned idx,
	const char*name, ivl_signal_port_t dir)
{
	assert(idx < cell->nports);

	cell->ports[idx].name = name;
	cell->ports[idx].dir  = dir;
}

void edif_cell_port_pstring(edif_cell_t cell, unsigned idx,
	const char*name, const char*value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name  = name;
	prp->ptype = PRP_STRING;
	prp->value_.str = value;
	prp->next  = cell->ports[idx].property;
	cell->ports[idx].property = prp;
}

unsigned edif_cell_port_byname(edif_cell_t cell, const char*name)
{
	unsigned idx = 0;
	for (idx = 0 ;  idx < cell->nports ;  idx += 1)
		if (strcmp(name, cell->ports[idx].name) == 0)
			break;

	return idx;
}

void edif_cell_pstring(edif_cell_t cell, const char*name,
	const char*value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name  = name;
	prp->ptype = PRP_STRING;
	prp->value_.str = value;
	prp->next  = cell->property;
	cell->property = prp;
}

void edif_cell_pinteger(edif_cell_t cell, const char*name,
	long value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name  = name;
	prp->ptype = PRP_INTEGER;
	prp->value_.num = value;
	prp->next  = cell->property;
	cell->property = prp;
}

edif_cellref_t edif_cellref_create(edif_t edf, edif_cell_t cell)
{
	static unsigned u_number = 0;
	edif_cellref_t ref = malloc(sizeof(struct edif_cellref_s));

	u_number += 1;

	assert(cell);
	assert(edf);

	ref->u = u_number;
	ref->cell = cell;
	ref->property = 0;
	ref->next = edf->celref;
	edf->celref = ref;

	return ref;
}

void edif_cellref_pstring(edif_cellref_t ref, const char*name,
	const char*value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name = name;
	prp->ptype = PRP_STRING;
	prp->value_.str = value;
	prp->next = ref->property;
	ref->property = prp;
}

void edif_cellref_pinteger(edif_cellref_t ref, const char*name, long value)
{
	struct cellref_property_*prp = malloc(sizeof(struct cellref_property_));
	prp->name = name;
	prp->ptype = PRP_INTEGER;
	prp->value_.num = value;
	prp->next = ref->property;
	ref->property = prp;
}

edif_joint_t edif_joint_create(edif_t edf)
{
	edif_joint_t jnt = malloc(sizeof(struct edif_joint_s));

	jnt->name = 0;
	jnt->links = 0;
	jnt->next  = edf->nexa;
	edf->nexa  = jnt;
	return jnt;
}

edif_joint_t edif_joint_of_nexus(edif_t edf, ivl_nexus_t nex)
{
	void*tmp = ivl_nexus_get_private(nex);
	edif_joint_t jnt;

	if (tmp == 0) {
		jnt = edif_joint_create(edf);
		ivl_nexus_set_private(nex, jnt);
		return jnt;
	}

	jnt = (edif_joint_t) tmp;
	return jnt;
}

void edif_nexus_to_joint(edif_t edf, edif_joint_t jnt, ivl_nexus_t nex)
{
	void*tmp = ivl_nexus_get_private(nex);

	if (tmp != 0) {
		/* There is a joint already on the nexus. Move all the
		joint cells to the joint I'm joining to. */
		edif_joint_t njnt = (edif_joint_t)tmp;
		while (njnt->links) {
			struct joint_cell_*cell = njnt->links;
			njnt->links = cell->next;
			cell->next = jnt->links;
			jnt->links = cell;
		}

		/* Now njnt is dead, and should be removed from edif. */
		/* Or we can ignore it as harmless. */
	}

	ivl_nexus_set_private(nex, jnt);
}

void edif_joint_rename(edif_joint_t jnt, const char*name)
{
	assert(jnt->name == 0);
	jnt->name = name;
}

void edif_add_to_joint(edif_joint_t jnt, edif_cellref_t cell, unsigned port)
{
	struct joint_cell_* jc = malloc(sizeof(struct joint_cell_));

	jc->cell = cell;
	jc->port = port;
	jc->next = jnt->links;
	jnt->links = jc;
}

static void fprint_property(FILE*fd, const struct cellref_property_*prp)
{
	fprintf(fd, "(property %s ", prp->name);
	switch (prp->ptype) {
	case PRP_NONE:
		break;
	case PRP_STRING:
		fprintf(fd, "(string \"%s\")", prp->value_.str);
		break;
	case PRP_INTEGER:
		fprintf(fd, "(integer %ld)", prp->value_.num);
		break;
	}
	fprintf(fd, ")");
}

/*
* This function takes all the data structures that have been
* assembled by the code generator, and writes them into an EDIF
* formatted file.
*/
void edif_print(FILE*fd, edif_t edf)
{
	edif_xlibrary_t xlib;
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;
	struct cellref_property_*prp;
	unsigned idx;

	fprintf(fd, "(edif %s\n", edf->name);
	fprintf(fd, "    (edifVersion 2 0 0)\n");
	fprintf(fd, "    (edifLevel 0)\n");
	fprintf(fd, "    (keywordMap (keywordLevel 0))\n");
	fprintf(fd, "    (status\n");
	fprintf(fd, "     (written\n");
	fprintf(fd, "        (timeStamp 0 0 0 0 0 0)\n");
	fprintf(fd, "        (author \"unknown\")\n");
	fprintf(fd, "        (program \"Icarus Verilog/fpga.tgt\")))\n");
	fflush(fd);

	for (xlib = edf->xlibs ;  xlib ;  xlib = xlib->next) {

		fprintf(fd, "    (external %s "
			"(edifLevel 0) "
			"(technology (numberDefinition))\n",
			xlib->name);

		for (cell = xlib->cells ;  cell ;  cell = cell->next) {
			fprintf(fd, "      (cell %s (cellType GENERIC)\n",
				cell->name);
			fprintf(fd, "            (view net\n"
				"              (viewType NETLIST)\n"
				"              (interface");

			for (idx = 0 ;  idx < cell->nports ;  idx += 1) {
				struct __cell_port*pp = cell->ports + idx;
				fprintf(fd, "\n                (port %s", pp->name);
				switch (pp->dir) {
				case IVL_SIP_INPUT:
					fprintf(fd, " (direction INPUT)");
					break;
				case IVL_SIP_OUTPUT:
					fprintf(fd, " (direction OUTPUT)");
					break;
				case IVL_SIP_INOUT:
					fprintf(fd, " (direction INOUT)");
					break;
				default:
					break;
				}

				for (prp = pp->property ;  prp ;  prp=prp->next) {
					fprintf(fd, " ");
					fprint_property(fd, prp);
				}

				fprintf(fd, ")");
			}

			for (prp = cell->property ;  prp ;  prp = prp->next) {
				fprintf(fd, "\n                ");
				fprint_property(fd, prp);
			}
			fprintf(fd, ")))\n");
		}

		fprintf(fd, "    )\n"); /* terminate (external ...) sexp */
	}
	fflush(fd);

	/* Write out the library header */
	fprintf(fd, "    (library DESIGN\n");
	fprintf(fd, "      (edifLevel 0)\n");
	fprintf(fd, "      (technology (numberDefinition))\n");

	/* The root module is a cell in the library. */
	fprintf(fd, "      (cell %s\n", edf->name);
	fprintf(fd, "        (cellType GENERIC)\n");
	fprintf(fd, "        (view net\n");
	fprintf(fd, "          (viewType NETLIST)\n");
	fprintf(fd, "          (interface\n");

	for (idx = 0 ;  idx < edf->nports ;  idx += 1) {
		fprintf(fd, "            (port ");
		if (edf->ports[idx].ename == 0)
			fprintf(fd, "%s ", edf->ports[idx].name);
		else
			fprintf(fd, "(rename %s \"%s\") ",
			edf->ports[idx].ename,
			edf->ports[idx].name);

		switch (edf->ports[idx].dir) {
		case IVL_SIP_INPUT:
			fprintf(fd, "(direction INPUT)");
			break;
		case IVL_SIP_OUTPUT:
			fprintf(fd, "(direction OUTPUT)");
			break;
		case IVL_SIP_INOUT:
			fprintf(fd, "(direction INOUT)");
			break;
		default:
			break;
		}
		fprintf(fd, ")\n");
	}

	fprintf(fd, "          )\n"); /* end the (interface ) sexp */
	fflush(fd);

	fprintf(fd, "          (contents\n");

	/* Display all the instances. */
	for (ref = edf->celref ;  ref ;  ref = ref->next) {

		assert(ref->cell);

		fprintf(fd, "(instance U%u (viewRef net "
			"(cellRef %s (libraryRef %s)))",
			ref->u, ref->cell->name, ref->cell->xlib->name);

		for (prp = ref->property ;  prp ;  prp = prp->next) {
			fprintf(fd, " ");
			fprint_property(fd, prp);
		}

		fprintf(fd, ")\n");
	}

	fprintf(fd, "\n");

	fflush(fd);

	/* Display all the joints. */
	idx = 0;
	for (jnt = edf->nexa ;  jnt ;  jnt = jnt->next, idx += 1) {
		struct joint_cell_*jc;

		fprintf(fd, "(net ");
		if (jnt->name != 0)
			fprintf(fd, "(rename N%u \"%s\")", idx, jnt->name);
		else
			fprintf(fd, "N%u", idx);
		fprintf(fd, " (joined");

		for (jc = jnt->links ;  jc ;  jc = jc->next) {
			if (jc->cell) {
				fprintf(fd, " (portRef %s (instanceRef U%u))",
					jc->cell->cell->ports[jc->port].name,
					jc->cell->u);
			} else {
				/* Reference to a port of the main cell. */
				if (edf->ports[jc->port].ename)
					fprintf(fd, " (portRef %s)",
					edf->ports[jc->port].ename);
				else
					fprintf(fd, " (portRef %s)",
					edf->ports[jc->port].name);
			}
		}
		fprintf(fd, "))\n");
	}

	fprintf(fd, "          )\n"); /* end the (contents...) sexp */

	fprintf(fd, "        )\n"); /* end the (view ) sexp */
	fprintf(fd, "      )\n"); /* end the (cell ) sexp */
	fprintf(fd, "    )\n"); /* end the (library DESIGN) sexp */

	/* Make an instance of the defined object */
	fprintf(fd, "    (design %s\n", edf->name);
	fprintf(fd, "      (cellRef %s (libraryRef DESIGN))\n", edf->name);

	for (prp = edf->property ;  prp ;  prp = prp->next) {
		fprintf(fd, "       ");
		fprint_property(fd, prp);
		fprintf(fd, "\n");
	}

	fprintf(fd, "    )\n");



	fprintf(fd, ")\n");
	fflush(fd);
}

/*
* This function takes all the data structures that have been
* assembled by the code generator, and writes them into an BLIF
* formatted file.
*/
void blif_print(FILE*fd, edif_t edf)
{
	edif_xlibrary_t xlib;
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;
	struct cellref_property_*prp;
	unsigned idx;
	char**net_name;
	struct joint_cell_*jc;
	char pll_info_path[1024] = {'\0'};
	char m4k_info_path[1024] = {'\0'};
	FILE* pll_info = 0;
	FILE* m4k_info = 0;
	strcat(pll_info_path, base);
	strcat(pll_info_path,"\\comet_pll.info");
	strcat(m4k_info_path, base);
	strcat(m4k_info_path,"\\comet_m4k.info");

	pll_info = fopen(pll_info_path, "w");
	if (!pll_info)
	{
		printf("error: cannot open the file %s", pll_info_path);
		exit(1);
	}

	m4k_info = fopen(m4k_info_path, "w");
	if (!pll_info)
	{
		printf("error: cannot open the file %s", pll_info_path);
		exit(1);
	}

	printf("Print blif file ...\n");
	/* cells */
	for (xlib = edf->xlibs; xlib; xlib = xlib->next) {
		for (cell = xlib->cells; cell; cell = cell->next) {
			int done = 0;
			for (ref = edf->celref; ref; ref = ref->next) {
				if (ref->cell->name == cell->name) {
					done = 1;
					break;
				}
			}
			if (done) {
				fprintf(fd, ".model %s\n",
					cell->name);

				fprintf(fd, ".inputs");
				for (idx = 0; idx < cell->nports; idx += 1) {
					struct __cell_port*pp = cell->ports + idx;
					if (pp->dir == IVL_SIP_INPUT || pp->dir == IVL_SIP_INOUT)
						fprintf(fd, " %s", pp->name);
				}
				fprintf(fd, "\n");

				fprintf(fd, ".outputs");
				for (idx = 0; idx < cell->nports; idx += 1) {
					struct __cell_port*pp = cell->ports + idx;
					if (pp->dir == IVL_SIP_OUTPUT)
						fprintf(fd, " %s", pp->name);
				}
				fprintf(fd, "\n");

				fflush(fd);

				blif_show_cell(fd, cell);

				fprintf(fd, ".end\n");
				fprintf(fd, "\n");				
			}
		}
	}
	fflush(fd);

	/* the root module */
	fprintf(fd, ".model %s\n", edf->name);

	fprintf(fd, ".inputs");
	for (idx = 0; idx < edf->nports; idx += 1) {
		if (edf->ports[idx].dir == IVL_SIP_INPUT || edf->ports[idx].dir == IVL_SIP_INOUT)
			fprintf(fd, " %s", edf->ports[idx].name);
	}
	fprintf(fd, "\n");

	fprintf(fd, ".outputs");
	for (idx = 0; idx < edf->nports; idx += 1) {
		if (edf->ports[idx].dir == IVL_SIP_OUTPUT)
			fprintf(fd, " %s", edf->ports[idx].name);
	}
	fprintf(fd, "\n");

	/* all the joints for input and output */
	idx = 0;
	for (jnt = edf->nexa; jnt; jnt = jnt->next)
		idx += 1;
	net_name = (char**) malloc (idx*sizeof(char*));
	idx = 0;
	for (jnt = edf->nexa; jnt; jnt = jnt->next, idx += 1) {
		net_name[idx] = NULL;
		for (jc = jnt->links; jc; jc = jc->next)
			if ( !(jc->cell) )
				net_name[idx] = edf->ports[jc->port].name;
	}



	/* all the instances in the root module */
	for (ref = edf->celref; ref; ref = ref->next) {
		assert(ref->cell);
		
		if(strcmp(ref->cell->name, "comet_pll") == 0){
			
			static int pll_exist_flag = 0;
			struct cellref_property_*prp = 0;

			const char* up_module_name = 0;
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "up_module_name") {
					up_module_name = prp->value_.str;
					break;
				}
			}
			assert(up_module_name);
			pll_exist_flag++;
			
			
			if (pll_exist_flag == 1)
			{
				fprintf(pll_info, "#comet_pll\n");
			}
			fprintf(pll_info, "%s\\%s.v\n",base, up_module_name);
			printf("%s\\%s.v\n",base, up_module_name);
			
		}else if(strncmp(ref->cell->name, "mem_",4) == 0){

			static int m4k_exist_flag = 0;
			struct cellref_property_*prp = 0;

			const char* mode = 0;
			const char* up_scope_name = 0;
			const char* up_module_name = 0;
			const char* init_file = 0;
			unsigned port_a_first_address = 0;
			unsigned port_a_last_address = 0;
			unsigned port_a_first_bit_number = 0;
			unsigned port_b_first_address = 0;
			unsigned port_b_last_address = 0;
			unsigned port_b_first_bit_number = 0;
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "init_file") {
					init_file = prp->value_.str;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "mode") {
					mode = prp->value_.str;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "up_scope_name") {
					up_scope_name = prp->value_.str;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "up_module_name") {
					up_module_name = prp->value_.str;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "port_a_first_address") {
					port_a_first_address = prp->value_.num;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "port_a_last_address") {
					port_a_last_address = prp->value_.num;
					break;
				}
			}
			for (prp = ref->property; prp; prp = prp->next) {
				if (prp->name == "port_a_first_bit_number") {
					port_a_first_bit_number = prp->value_.num;
					break;
				}
			}
			if(strcmp("single_port",mode) != 0){
				for (prp = ref->property; prp; prp = prp->next) {
					if (prp->name == "port_b_first_address") {
						port_b_first_address = prp->value_.num;
						break;
					}
				}
				for (prp = ref->property; prp; prp = prp->next) {
					if (prp->name == "port_b_last_address") {
						port_b_last_address = prp->value_.num;
						break;
					}
				}
				for (prp = ref->property; prp; prp = prp->next) {
					if (prp->name == "port_b_first_bit_number") {
						port_b_first_bit_number = prp->value_.num;
						break;
					}
				}
			}

			m4k_exist_flag++;


			if (m4k_exist_flag == 1)
			{
				fprintf(m4k_info, "#comet_m4k\n");
			}
			fprintf(m4k_info, "%s | ",up_scope_name);
			fprintf(m4k_info, "%s\\%s.v | ",base, up_module_name);

			if(strcmp(init_file, "NULL") == 0){
				fprintf(m4k_info, "\"\"",init_file);
			}else{
				fprintf(m4k_info, "\"%s\" | ",init_file);
			}

			fprintf(m4k_info, "%d | ",port_a_first_address);
			fprintf(m4k_info, "%d | ",port_a_last_address);
			fprintf(m4k_info, "%d",port_a_first_bit_number);
			if(strcmp("single_port",mode) != 0){
				fprintf(m4k_info, " | %d | ",port_b_first_address);
				fprintf(m4k_info, "%d | ",port_b_last_address);
				fprintf(m4k_info, "%d",port_b_first_bit_number);
			}			
			
			fprintf(m4k_info, "\n");

		}
		fprintf(fd, ".subckt %s", ref->cell->name);
		idx = 0;
		for (jnt = edf->nexa; jnt; jnt = jnt->next, idx += 1) {
			struct joint_cell_*jc;
			for (jc = jnt->links; jc; jc = jc->next)
				if (jc->cell && jc->cell->u == ref->u)
					if (net_name[idx])
						fprintf(fd, " %s=%s", jc->cell->cell->ports[jc->port].name, net_name[idx]);
					else
						fprintf(fd, " %s=N%u", jc->cell->cell->ports[jc->port].name, idx);
		}
		fprintf(fd, "\n");
	}
	free(net_name);

	fprintf(fd, ".end\n");
	fflush(fd);
	fclose(pll_info);
	fclose(m4k_info);
}

void blif_show_cell(FILE*fd, edif_cell_t cell)
{
	signed idx,jdx,kdx;
	int*ones;
	unsigned one_count;
	unsigned res;
	unsigned pos;
	int*sel;

	const char*name = cell->name;

	struct cellref_property_*prp;
	const char*type;
	for (prp = cell->property; prp; prp = prp->next) {
		if (prp->name == "LPM_TYPE" || prp->name == "LPM_Type") {
			type = prp->value_.str;
			break;
		}
	}

	assert(name);
	assert(type);

	/*BEGIN--ADU*/
	if(type == "LPM_CMP_GT"){

		char* signed_flag;
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Width"){
				width = prp->value_.num;
				break;
			}
		}
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Signed_Flag"){
				signed_flag = prp->value_.str;
				break;
			}
		}
		if (strcmp(signed_flag,"SIGNED") == 0){
			assert(width >= 2);
			idx = width - 1;
			/*比较器的blif格式输出从高位到低位输出，与其他运算单元如加法器输出方向相反。
			其中LPM_CMP_GT和LPM_CMP_GE两种形式的比较器需要考虑符号位。
			例：reg signed [3:0] a, b。
			a = 3'b101, b = 3'b011，则a<b.*/

			/*最高位或者符号位单独输出*/
			/*高位向低位的进位分别由两个信号决定Cin和Din，
			**以LPM_CMP_GT_UNSIGN举例
			**Cin  Din  意义
			** 1    0   无需再比较低位，因为高位已经决定了a>b
			** 0    1   无需再比较低位，因为高位已经决定了a<b
			** 0    0   高位相等，需继续比较低位
			** 1    1   不会输出这种情况
			
			**上述逻辑适用于LPM_CMP_GE情况*/
			fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			fprintf(fd, "01 1\n");
			//fprintf(fd, "10 0\n");
			//fprintf(fd, "11 0\n");

			fprintf(fd, ".names DataA%u DataB%u Din%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			//fprintf(fd, "01 0\n");
			fprintf(fd, "10 1\n");
			//fprintf(fd, "11 0\n");
			/**/
			for (idx -= 1; idx >= 1; idx-- ){
				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Cin%u\n", idx, idx, idx+1, idx+1, idx);
				fprintf(fd, "--1- 1\n");
				//fprintf(fd, "--01 0\n");
				//fprintf(fd, "0000 0\n");
				//fprintf(fd, "0100 0\n");
				fprintf(fd, "1000 1\n");
				//fprintf(fd, "1100 0\n");

				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Din%u\n", idx, idx, idx+1, idx+1, idx);
				//fprintf(fd, "--1- 0\n");
				fprintf(fd, "--01 1\n");
				//fprintf(fd, "0000 0\n");
				fprintf(fd, "0100 1\n");
				//fprintf(fd, "1000 0\n");
				//fprintf(fd, "1100 0\n");
			}

			fprintf(fd, ".names DataA0 DataB0 Cin1 Din1 Result\n");
			fprintf(fd, "--1- 1\n");
			//fprintf(fd, "--01 0\n");
			//fprintf(fd, "0000 0\n");
			//fprintf(fd, "0100 0\n");
			fprintf(fd, "1000 1\n");
			//fprintf(fd, "1100 0\n");
		}else{
			assert(width);
			idx = width - 1;
			fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			//fprintf(fd, "01 0\n");
			fprintf(fd, "10 1\n");
			//fprintf(fd, "11 0\n");

			fprintf(fd, ".names DataA%u DataB%u Din%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			fprintf(fd, "01 1\n");
			//fprintf(fd, "10 0\n");
			//fprintf(fd, "11 0\n");

			for (idx -= 1; idx >= 1; idx-- ){
				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Cin%u\n", idx, idx, idx+1, idx+1, idx);
				fprintf(fd, "--1- 1\n");
				//fprintf(fd, "--01 0\n");
				//fprintf(fd, "0000 0\n");
				//fprintf(fd, "0100 0\n");
				fprintf(fd, "1000 1\n");
				//fprintf(fd, "1100 0\n");

				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Din%u\n", idx, idx, idx+1, idx+1, idx);
				//fprintf(fd, "--1- 0\n");
				fprintf(fd, "--01 1\n");
				//fprintf(fd, "0000 0\n");
				fprintf(fd, "0100 1\n");
				//fprintf(fd, "1000 0\n");
				//fprintf(fd, "1100 0\n");
			}

			fprintf(fd, ".names DataA0 DataB0 Cin1 Din1 Result\n");
			fprintf(fd, "--1- 1\n");
			//fprintf(fd, "--01 0\n");
			//fprintf(fd, "0000 0\n");
			//fprintf(fd, "0100 0\n");
			fprintf(fd, "1000 1\n");
			//fprintf(fd, "1100 0\n");
		}			
	}

	else if(type == "LPM_CMP_GE"){

		char* signed_flag;
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Width"){
				width = prp->value_.num;
				break;
			}
		}
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Signed_Flag"){
				signed_flag = prp->value_.str;
				break;
			}
		}
		if (strcmp(signed_flag,"SIGNED") == 0){
			assert(width >= 2);
			idx = width - 1;
			fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			fprintf(fd, "01 1\n");
			//fprintf(fd, "10 0\n");
			//fprintf(fd, "11 0\n");

			fprintf(fd, ".names DataA%u DataB%u Din%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			//fprintf(fd, "01 0\n");
			fprintf(fd, "10 1\n");
			//fprintf(fd, "11 0\n");

			for (idx -= 1; idx >= 1; idx-- ){
				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Cin%u\n", idx, idx, idx+1, idx+1, idx);
				fprintf(fd, "--1- 1\n");
				//fprintf(fd, "--01 0\n");
				//fprintf(fd, "0000 0\n");
				//fprintf(fd, "0100 0\n");
				fprintf(fd, "1000 1\n");
				//fprintf(fd, "1100 0\n");

				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Din%u\n", idx, idx, idx+1, idx+1, idx);
				//fprintf(fd, "--1- 0\n");
				fprintf(fd, "--01 1\n");
				//fprintf(fd, "0000 0\n");
				fprintf(fd, "0100 1\n");
				//fprintf(fd, "1000 0\n");
				//fprintf(fd, "1100 0\n");
			}

			fprintf(fd, ".names DataA0 DataB0 Cin1 Din1 Result\n");
			fprintf(fd, "--1- 1\n");
			//fprintf(fd, "--01 0\n");
			fprintf(fd, "0000 1\n");
			//fprintf(fd, "0100 0\n");
			fprintf(fd, "1000 1\n");
			fprintf(fd, "1100 1\n");
		}else{
			assert(width);
			idx = width - 1;
			fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			//fprintf(fd, "01 0\n");
			fprintf(fd, "10 1\n");
			//fprintf(fd, "11 0\n");

			fprintf(fd, ".names DataA%u DataB%u Din%u\n", idx, idx, idx);
			//fprintf(fd, "00 0\n");
			fprintf(fd, "01 1\n");
			//fprintf(fd, "10 0\n");
			//fprintf(fd, "11 0\n");

			for (idx -= 1; idx >= 1; idx-- ){
				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Cin%u\n", idx, idx, idx+1, idx+1, idx);
				fprintf(fd, "--1- 1\n");
				//fprintf(fd, "--01 0\n");
				//fprintf(fd, "0000 0\n");
				//fprintf(fd, "0100 0\n");
				fprintf(fd, "1000 1\n");
				//fprintf(fd, "1100 0\n");

				fprintf(fd, ".names DataA%u DataB%u Cin%u Din%u Din%u\n", idx, idx, idx+1, idx+1, idx);
				//fprintf(fd, "--1- 0\n");
				fprintf(fd, "--01 1\n");
				//fprintf(fd, "0000 0\n");
				fprintf(fd, "0100 1\n");
				//fprintf(fd, "1000 0\n");
				//fprintf(fd, "1100 0\n");
			}

			fprintf(fd, ".names DataA0 DataB0 Cin1 Din1 Result\n");
			fprintf(fd, "--1- 1\n");
			//fprintf(fd, "--01 0\n");
			fprintf(fd, "0000 1\n");
			//fprintf(fd, "0100 0\n");
			fprintf(fd, "1000 1\n");
			fprintf(fd, "1100 1\n");
		}
	}

	else if(type == "LPM_CMP_EQ"){

		char* signed_flag;
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Width"){
				width = prp->value_.num;
				break;
			}
		}

		assert(width);
		idx = width - 1;
		//Cin为1说明不相等，Cin为0说明前面的高位相等，有待剩余低位的比较
		fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
		//fprintf(fd, "00 0\n");
		fprintf(fd, "01 1\n");
		fprintf(fd, "10 1\n");
		//fprintf(fd, "11 0\n");

		for (idx -= 1; idx >= 1; idx-- ){
			fprintf(fd, ".names DataA%u DataB%u Cin%u Cin%u\n", idx, idx, idx+1, idx);
			fprintf(fd, "--1 1\n");
			//fprintf(fd, "000 0\n");
			fprintf(fd, "010 1\n");
			fprintf(fd, "100 1\n");
			//fprintf(fd, "110 0\n");
		}

		fprintf(fd, ".names DataA0 DataB0 Cin1 Result\n");
		//fprintf(fd, "--1 0\n");
		fprintf(fd, "000 1\n");
		//fprintf(fd, "010 0\n");
		//fprintf(fd, "100 0\n");
		fprintf(fd, "110 1\n");
	}
	else if(type == "LPM_CMP_NE"){

		char* signed_flag;
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_Width"){
				width = prp->value_.num;
				break;
			}
		}

		assert(width);
		idx = width - 1;
		//Cin为1说明不相等，Cin为0说明前面的高位相等，有待剩余低位的比较
		fprintf(fd, ".names DataA%u DataB%u Cin%u\n", idx, idx, idx);
		//fprintf(fd, "00 0\n");
		fprintf(fd, "01 1\n");
		fprintf(fd, "10 1\n");
		//fprintf(fd, "11 0\n");

		for (idx -= 1; idx >= 1; idx-- ){
			fprintf(fd, ".names DataA%u DataB%u Cin%u Cin%u\n", idx, idx, idx+1, idx);
			fprintf(fd, "--1 1\n");
			//fprintf(fd, "000 0\n");
			fprintf(fd, "010 1\n");
			fprintf(fd, "100 1\n");
			//fprintf(fd, "110 0\n");
		}

		fprintf(fd, ".names DataA0 DataB0 Cin1 Result\n");
		fprintf(fd, "--1 1\n");
		//fprintf(fd, "000 0\n");
		fprintf(fd, "010 1\n");
		fprintf(fd, "100 1\n");
		//fprintf(fd, "110 0\n");
	}
	/*END--ADU*/
	else if (name == "BUF" && type == "LPM_INV") {
		fprintf(fd, ".names Data Result\n");
		fprintf(fd, "1 1\n");
	}

	else if (name == "BUFIF0" || name == "BUFIF1"){
		fprintf(fd, "# EnableDT Data TriData\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name == "INV" && type == "LPM_INV") {
		fprintf(fd, ".names Data Result\n");
		fprintf(fd, "0 1\n");
	}

	else if (name[0] == 'o' && name[1] == 'r' && type == "LPM_OR") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");
		for (idx = 0; idx < size; idx++)
			fprintf(fd, "0");
		fprintf(fd, " 0\n");
	}

	else if (name[0] == 'n' && name[1] == 'o' && name[2] == 'r' && type == "LPM_OR") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, "0");
		}
		fprintf(fd, " 1\n");
	}

	else if (name[0] == 'a' && name[1] == 'n' && name[2] == 'd' && type == "LPM_AND") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, "1");
		}
		fprintf(fd, " 1\n");
	}

	//begin-adu
	else if (name[0] == 'n' && name[1] == 'a' && name[2] == 'n' && name[3] =='d' && type == "LPM_NAND") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, "1");
		}
		fprintf(fd, " 0\n");
	}
	//end-adu

	else if (name[0] == 'x' && name[1] == 'o' && name[2] == 'r' && type == "LPM_XOR") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");

		ones = (int*) malloc (size*sizeof(int));
		for (idx = 0; idx < pow(2,(double)size); idx++) {
			for (jdx = 0; jdx < size; jdx++)
				ones[jdx] = 0;
			one_count = 0;
			res = idx;
			pos = size - 1;
			//记录‘1’的个数，以及‘1’的位置
			while (res > 0) {
				if ( res >= pow(2,(double)pos) ) {
					ones[pos] = 1;
					one_count++;
					res -= pow(2,(double)pos);
				}
				pos--;
			}
			if (one_count % 2 == 1) {
				for (jdx = size-1; jdx >= 0; jdx--) {
					if (ones[jdx] == 1)
						fprintf(fd, "1");
					else
						fprintf(fd, "0");
				}
				fprintf(fd, " 1\n");
			}
		}
		free(ones);
	}

	//begin-adu
	else if (name[0] == 'x' && name[1] == 'n' && name[2] == 'o' && name[3] == 'r' && type == "LPM_XNOR") {
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Size")
				size = prp->value_.num;
		assert(size);

		fprintf(fd, ".names");
		for (idx = 0; idx < size; idx++) {
			fprintf(fd, " Data%ux0", idx);
		}
		fprintf(fd, " Result0\n");

		ones = (int*) malloc (size*sizeof(int));
		for (idx = 0; idx < pow(2,(double)size); idx++) {
			int zero_count = 0;
			for (jdx = 0; jdx < size; jdx++)
				ones[jdx] = 0;
			one_count = 0;
			res = idx;
			pos = size - 1;
			//记录‘1’的个数，以及‘1’的位置
			while (res > 0) {
				if ( res >= pow(2,(double)pos) ) {
					ones[pos] = 1;
					one_count++;
					res -= pow(2,(double)pos);
				}
				pos--;
			}
			zero_count = size - one_count;
			if (zero_count % 2 == 0) {
				for (jdx = size-1; jdx >= 0; jdx--) {
					if (ones[jdx] == 1)
						fprintf(fd, "1");
					else
						fprintf(fd, "0");
				}
				fprintf(fd, " 1\n");
			}
		}
		free(ones);
	}
	//end-adu

	//else if (name == "pll" && type == "LPM_PLL") {
	//else if (name[0] == 'p' && name[1] == 'l' && name[2] == 'l' && type == "LPM_PLL") {
	else if (strcmp(name,"comet_pll") == 0 && type == "LPM_PLL") {
		fprintf(fd, "# inclk0 pllena areset pfdena\n");
		fprintf(fd, "# c0 c1 e0 locked\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 'f' && name[1] == 'd' && type == "LPM_FF") {
		//if (name[2] != 'c' && name[2] != 's') {
		//	unsigned width;
		//	for (prp = cell->property; prp; prp = prp->next)
		//		if (prp->name == "LPM_Width")
		//			width = prp->value_.num;
		//	assert(width);

		//	for (idx = 0; idx < width; idx++)
		//		fprintf(fd, ".latch Data%u Q%u re Clock\n", idx, idx);//这里可能不只是re可能为fe——ADU
		//}
		//else {
			idx = 2;
			fprintf(fd, "# Data");
			if (name[idx] == 'c' && name[idx+1] == 'e' ) {
				fprintf(fd, " Enable");
				idx = idx + 2;
			}
			if (name[idx] == 'A' && name[idx+1] == 'c' ) {
				fprintf(fd, " Aclr");
				idx = idx + 2;
			}
			if (name[idx] == 'S' && name[idx+1] == 'c' ) {
				fprintf(fd, " Sclr");
				idx = idx + 2;
			}
			if (name[idx] == 'A' && name[idx+1] == 's' ) {
				fprintf(fd, " Aset");
				idx = idx + 2;
			}
			if (name[idx] == 'S' && name[idx+1] == 's' ) {
				fprintf(fd, " Sset");
				idx = idx + 2;
			}
			fprintf(fd, " Clock\n");
			fprintf(fd, "# Q\n");
			fprintf(fd, ".blackbox\n");
		//}
	}

	else if (name[0] == 'l' && name[1] == 'a' && type == "LPM_latch") {
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Width")
				width = prp->value_.num;
		assert(width);


		idx = 5;
		fprintf(fd, "# Data");
		if (name[idx] == 'A' && name[idx+1] == 'c' ) {
			fprintf(fd, " Aclr");
			idx = idx + 2;
		}
		if (name[idx] == 'A' && name[idx+1] == 's' ) {
			fprintf(fd, " Aset");
			idx = idx + 2;
		}

		fprintf(fd, " ah Gate\n");
		fprintf(fd, "# Q\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 'm' && name[1] == 'u' && name[2] == 'x' && type == "LPM_MUX") {
		unsigned width;
		unsigned widths;
		unsigned size;
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width")
				width = prp->value_.num;
			else if (prp->name == "LPM_WidthS")
				widths = prp->value_.num;
			else if (prp->name == "LPM_Size")
				size = prp->value_.num;
		}
		assert(width);
		//assert(widths);
		assert(size);

		for (idx = 0; idx < width; idx++) {
			fprintf(fd, ".names");
			for (jdx = 0; jdx < size; jdx++)
				fprintf(fd, " Data%ux%u", jdx, idx);
			for (jdx = 0; jdx < widths; jdx++)
				fprintf(fd, " Sel%u", jdx);
			fprintf(fd, " Result%u\n", idx);

			sel = (int*) malloc (widths*sizeof(int));
			for (jdx = 0; jdx < widths; jdx++)
				sel[jdx] = 0;
			for (jdx = 0; jdx < size; jdx++) {
				for (kdx = 0; kdx < size; kdx++) {
					if (kdx == jdx)
						fprintf(fd, "1");
					else
						fprintf(fd, "-");
				}
				for (kdx = 0; kdx < widths; kdx++) {
					fprintf(fd, "%i", sel[kdx]);
				}
				fprintf(fd, " 1\n");

				pos = 0;
				while (sel[pos] == 1)
					pos++;
				if (pos < widths)
					sel[pos] = 1;
				for (kdx = 0; kdx < pos; kdx++)
					sel[kdx] = 0;
			}
			free(sel);
		}
	}

	else if (name[0] == 'A' && name[1] == 'D' && name[2] == 'D' && type == "LPM_ADD_SUB") {
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Width")
				width = prp->value_.num;
		assert(width);

		fprintf(fd, ".names C0\n");
		fprintf(fd, " 0\n");
		for (idx = 0; idx < width; idx++) {
			fprintf(fd, ".names DataA%u DataB%u C%u Result%u\n", idx, idx, idx, idx);
			fprintf(fd, "001 1\n");
			fprintf(fd, "010 1\n");
			fprintf(fd, "100 1\n");
			fprintf(fd, "111 1\n");
			fprintf(fd, ".names DataA%u DataB%u C%u C%u\n", idx, idx, idx, idx+1);
			fprintf(fd, "011 1\n");
			fprintf(fd, "101 1\n");
			fprintf(fd, "110 1\n");
			fprintf(fd, "111 1\n");
		}
		fprintf(fd, ".names C%u Cout\n", width);
		fprintf(fd, "1 1\n");
	}

	else if (name[0] == 'S' && name[1] == 'U' && name[2] == 'B' && type == "LPM_ADD_SUB") {
		unsigned width;
		for (prp = cell->property; prp; prp = prp->next)
			if (prp->name == "LPM_Width")
				width = prp->value_.num;
		assert(width);

		fprintf(fd, ".names C0\n");
		fprintf(fd, " 0\n");
		for (idx = 0; idx < width; idx++) {
			fprintf(fd, ".names DataA%u DataB%u C%u Result%u\n", idx, idx, idx, idx);
			fprintf(fd, "001 1\n");
			fprintf(fd, "010 1\n");
			fprintf(fd, "100 1\n");
			fprintf(fd, "111 1\n");
			fprintf(fd, ".names DataA%u DataB%u C%u C%u\n", idx, idx, idx, idx+1);
			fprintf(fd, "001 1\n");
			fprintf(fd, "010 1\n");
			fprintf(fd, "011 1\n");
			fprintf(fd, "111 1\n");
		}
		fprintf(fd, ".names C%u Cout\n", width);
		fprintf(fd, "1 1\n");
	}

	else if (name[0] == 'm' && name[1] == 'u' && name[2] == 'l' && name[3] == 't' && name[4] != '_' && type == "LPM_MULT") {
		unsigned width_p = 0;
		unsigned width_a = 0;
		unsigned width_b = 0;
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_WidthP")
				width_p = prp->value_.num;
			if (prp->name == "LPM_WidthA")
				width_a = prp->value_.num;
			if (prp->name == "LPM_WidthB")
				width_b = prp->value_.num;
		}
		assert(width_p);
		assert(width_a);
		assert(width_b);

		for (idx = 0; idx < width_a; idx++) {
			fprintf(fd, ".names P0_%u\n", idx);
			fprintf(fd, " 0\n");
		}
		for (idx = 0; idx < width_b; idx++) {
			for (jdx = 0; jdx < width_a; jdx++) {
				fprintf(fd, ".names DataA%u DataB%u M%u_%u\n", jdx, idx, idx, jdx);
				fprintf(fd, "11 1\n");
			}

			fprintf(fd, ".names P%u_0 M%u_0 Result%u\n", idx, idx, idx);
			fprintf(fd, "01 1\n");
			fprintf(fd, "10 1\n");
			fprintf(fd, ".names P%u_0 M%u_0 C%u_1\n", idx, idx, idx);
			fprintf(fd, "11 1\n");
			for (jdx = 1; jdx < width_a; jdx++) {
				fprintf(fd, ".names P%u_%u M%u_%u C%u_%u P%u_%u\n", idx, jdx, idx, jdx, idx, jdx, idx+1, jdx-1);
				fprintf(fd, "001 1\n");
				fprintf(fd, "010 1\n");
				fprintf(fd, "100 1\n");
				fprintf(fd, "111 1\n");
				fprintf(fd, ".names P%u_%u M%u_%u C%u_%u C%u_%u\n", idx, jdx, idx, jdx, idx, jdx, idx, jdx+1);
				fprintf(fd, "001 1\n");
				fprintf(fd, "010 1\n");
				fprintf(fd, "011 1\n");
				fprintf(fd, "111 1\n");
			}
			fprintf(fd, ".names C%u_%u P%u_%u\n", idx, width_a, idx+1, width_a-1);
			fprintf(fd, "1 1\n");
		}
		for (idx = width_b; idx < width_p; idx++) {
			if (idx - width_b < width_a) {
				fprintf(fd, ".names P%u_%u Result%u\n", width_b, idx-width_b, idx);
				fprintf(fd, "1 1\n");
			}
			else {
				fprintf(fd, ".names Result%u\n", idx);
				fprintf(fd, " 0\n");
			}
		}
	}

	else if (name[0] == 'm' && name[1] == 'e' && name[2] == 'm' && type == "LPM_MIP") {
		fprintf(fd, "#");
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width_Address_A")
				fprintf(fd, " Width_Address_A = %i", prp->value_.num);
			else if (prp->name == "LPM_Width_Address_B")
				fprintf(fd, " Width_Address_B = %i", prp->value_.num);
			else if (prp->name == "LPM_Width_Data_A")
				fprintf(fd, " Width_Data_A = %i", prp->value_.num);
			else if (prp->name == "LPM_Width_Data_B")
				fprintf(fd, " Width_Data_B = %i", prp->value_.num);
			else if (prp->name == "LPM_Width_Q_A")
				fprintf(fd, " Width_Q_A = %i", prp->value_.num);
			else if (prp->name == "LPM_Width_Q_B")
				fprintf(fd, " Width_Q_B = %i", prp->value_.num);
		}
		fprintf (fd, "\n");

		fprintf(fd, "# clocken0 clocken1 wren_a aclr0 clock0 wren_b aclr1 clock1 address_a address_b data_a data_b\n");
		fprintf(fd, "# q\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 's' && name[1] == 'q' && name[2] == 'u' && name[3] == 'a' && name[4] == 'r' && name[5] == 'e' && type == "LPM_SQUARE") {
		fprintf(fd, "#");
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width_Data")
				fprintf(fd, " Width_Data = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Result")
				fprintf(fd, " Width_Result = %l", prp->value_.num);
		}
		fprintf (fd, "\n");

		fprintf(fd, "# data clken aclr clock\n");
		fprintf(fd, "# result\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 'm' && name[1] == 'u' && name[2] == 'l' && name[3] == 't' && name[4] == '_' && type == "LPM_MULT") {
		fprintf(fd, "#");
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width_Data_A")
				fprintf(fd, " Width_Data_A = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Data_B")
				fprintf(fd, " Width_Data_B = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Sum")
				fprintf(fd, " Width_Sum = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Result")
				fprintf(fd, " Width_Result = %l", prp->value_.num);
		}
		fprintf (fd, "\n");

		fprintf(fd, "# dataa datab clken aclr clock sum\n");
		fprintf(fd, "# result\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 'm' && name[1] == 'u' && name[2] == 'l' && name[3] == 't' && name[4] == '_' && type == "LPM_MULT_ADD") {
		fprintf(fd, "#");
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width_Data_A")
				fprintf(fd, " Width_Data_A = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Data_B")
				fprintf(fd, " Width_Data_B = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Scanout_A")
				fprintf(fd, " Width_Scanout_A = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Scanout_B")
				fprintf(fd, " Width_Scanout_B = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Result")
				fprintf(fd, " Width_Result = %l", prp->value_.num);
		}
		fprintf (fd, "\n");

		fprintf(fd, "# dataa datab clock0 clock1 clock2 clock3 aclr0 aclr1 aclr2 aclr3 ena0 ena1 ena2 ena3\n");
		fprintf(fd, "# scanouta scanoutb result\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name[0] == 'm' && name[1] == 'u' && name[2] == 'l' && name[3] == 't' && name[4] == '_' && type == "LPM_MULT_ACCUM") {
		fprintf(fd, "#");
		for (prp = cell->property; prp; prp = prp->next) {
			if (prp->name == "LPM_Width_Data_A")
				fprintf(fd, " Width_Data_A = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Data_B")
				fprintf(fd, " Width_Data_B = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Scanout_A")
				fprintf(fd, " Width_Scanout_A = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Scanout_B")
				fprintf(fd, " Width_Scanout_B = %l", prp->value_.num);
			else if (prp->name == "LPM_Width_Result")
				fprintf(fd, " Width_Result = %l", prp->value_.num);
		}
		fprintf (fd, "\n");

		fprintf(fd, "# dataa datab accum_sload clock0 clock1 clock2 clock3 aclr0 aclr1 aclr2 aclr3 ena0 ena1 ena2 ena3\n");
		fprintf(fd, "# scanouta scanoutb result overflow\n");
		fprintf(fd, ".blackbox\n");
	}

	else if (name == "cell0" && type == "LPM_CONSTANT") {
		fprintf(fd, ".names Result0\n");
		fprintf(fd, " 0\n");
	}

	else if (name == "cell1" && type == "LPM_CONSTANT") {
		fprintf(fd, ".names Result0\n");
		fprintf(fd, " 1\n");
	}
	else if (!strncmp(name,"decoder",7) && type == "LPM_decoder") {
		unsigned awidth;
		unsigned idx = 0;
		unsigned j = 0;
		for (prp = cell->property; prp; prp = prp->next){
			if (prp->name == "LPM_aWidth"){
				awidth = prp->value_.num;
				break;
			}
		}

		for(idx = 0; idx < ( 1U << awidth ); idx++ ){
			fprintf(fd, ".names");
			for(j = 0; j < awidth; j++){
				fprintf(fd, " Data%u",j);
			}
			fprintf(fd, " Result%u\n",idx);

			for(j = 0; j < awidth; j++){
				fprintf(fd, "%u",((idx >> j) & 1) ? 1 : 0);
			}
			fprintf(fd, " 1\n");
		}
	}
	else
		assert(0);
}


/*
* $Log: edif.c,v $
* Revision 1.8.2.1  2005/08/27 22:29:30  steve
*  Back-port edif_nexus_to_joint from tgt-edif.
*
* Revision 1.8  2003/09/03 23:34:09  steve
*  Support synchronous set of LPM_FF devices.
*
* Revision 1.7  2003/08/07 05:18:04  steve
*  Add support for OR/NOR/bufif0/bufif1.
*
* Revision 1.6  2003/08/07 04:04:01  steve
*  Add an LPM device type.
*
* Revision 1.5  2003/06/24 03:55:00  steve
*  Add ivl_synthesis_cell support for virtex2.
*
* Revision 1.4  2003/04/04 04:59:03  steve
*  Add xlibrary celltable.
*
* Revision 1.3  2003/03/30 03:43:44  steve
*  Handle wide ports of macros.
*
* Revision 1.2  2003/03/24 02:29:04  steve
*  Give proper basenames to PAD signals.
*
* Revision 1.1  2003/03/24 00:47:54  steve
*  Add new virtex2 architecture family, and
*  also the new edif.h EDIF management functions.
*
*/

