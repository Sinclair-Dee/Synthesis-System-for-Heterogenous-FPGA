/*
* Copyright (c) 2003 Stephen Williams (steve@icarus.com)
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
#ident "$Id: d-lpm.c,v 1.12.2.1 2005/08/25 18:52:32 steve Exp $"
#endif

/*
* This is the driver for a purely generic LPM module writer. This
* uses LPM version 2 1 0 devices, without particularly considering
* the target technology.
*
* The LPM standard is EIA-IS/103-A  October 1996
* The output is EDIF 2 0 0 format.
*/

# include  "device.h"
# include  "fpga_priv.h"
# include  "edif.h"
# include  "generic.h"
# include  <string.h>
# include  <assert.h>
# include "global.h"

static edif_cell_t lpm_cell_buf(void)
{
	static edif_cell_t tmp = 0;

	if (tmp != 0)
		return tmp;

	tmp = edif_xcell_create(xlib, "BUF", 2);
	edif_cell_portconfig(tmp, 0, "Result0", IVL_SIP_OUTPUT);
	edif_cell_portconfig(tmp, 1, "Data0",   IVL_SIP_INPUT);

	/* A buffer is an inverted inverter. */
	edif_cell_port_pstring(tmp, 0, "LPM_Polarity", "INVERT");

	edif_cell_pstring(tmp,  "LPM_TYPE",  "LPM_INV");
	edif_cell_pinteger(tmp, "LPM_Width", 1);
	//edif_cell_pinteger(tmp, "LPM_Size",  1);//adu
	return tmp;
}

static edif_cell_t lpm_cell_inv(void)
{
	static edif_cell_t tmp = 0;

	if (tmp != 0)
		return tmp;

	tmp = edif_xcell_create(xlib, "INV", 2);
	edif_cell_portconfig(tmp, 0, "Result0", IVL_SIP_OUTPUT);
	edif_cell_portconfig(tmp, 1, "Data0",   IVL_SIP_INPUT);

	edif_cell_pstring(tmp,  "LPM_TYPE",  "LPM_INV");
	edif_cell_pinteger(tmp, "LPM_Width", 1);
	//edif_cell_pinteger(tmp, "LPM_Size",  1);//adu
	return tmp;
}

static edif_cell_t lpm_cell_bufif0(void)
{
	static edif_cell_t tmp = 0;

	if (tmp != 0)
		return tmp;

	tmp = edif_xcell_create(xlib, "BUFIF1", 3);
	edif_cell_portconfig(tmp, 0, "TriData",  IVL_SIP_OUTPUT);
	edif_cell_portconfig(tmp, 1, "Data",     IVL_SIP_INPUT);
	edif_cell_portconfig(tmp, 2, "EnableDT", IVL_SIP_INPUT);

	edif_cell_port_pstring(tmp, 2, "LPM_Polarity", "INVERT");

	edif_cell_pstring(tmp,  "LPM_TYPE",  "LPM_BUSTRI");
	edif_cell_pinteger(tmp, "LPM_Width", 1);
	return tmp;
}

static edif_cell_t lpm_cell_bufif1(void)
{
	static edif_cell_t tmp = 0;

	if (tmp != 0)
		return tmp;

	tmp = edif_xcell_create(xlib, "BUFIF1", 3);
	edif_cell_portconfig(tmp, 0, "TriData",  IVL_SIP_OUTPUT);
	edif_cell_portconfig(tmp, 1, "Data",     IVL_SIP_INPUT);
	edif_cell_portconfig(tmp, 2, "EnableDT", IVL_SIP_INPUT);

	edif_cell_pstring(tmp,  "LPM_TYPE",  "LPM_BUSTRI");
	edif_cell_pinteger(tmp, "LPM_Width", 1);
	return tmp;
}

static edif_cell_t lpm_cell_or(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "or%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_OR");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}

static edif_cell_t lpm_cell_and(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "and%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_AND");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}

/*begin-adu*/
static edif_cell_t lpm_cell_nand(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "nand%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_NAND");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}
/*end-adu*/


static edif_cell_t lpm_cell_xor(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "xor%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_XOR");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}

/*begin-adu*/
static edif_cell_t lpm_cell_xnor(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "xnor%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_XNOR");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}
/*end-adu*/

static edif_cell_t lpm_cell_pll(unsigned siz)
{
	edif_cell_t cell;
	char name[32];

	sprintf(name, "comet_pll");

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "inclk0", IVL_SIP_INPUT);
	edif_cell_portconfig(cell, 1, "pllena", IVL_SIP_INPUT);
	edif_cell_portconfig(cell, 2, "areset", IVL_SIP_INPUT);
	edif_cell_portconfig(cell, 3, "pfdena", IVL_SIP_INPUT);
	edif_cell_portconfig(cell, 4, "c0", IVL_SIP_OUTPUT);
	edif_cell_portconfig(cell, 5, "c1", IVL_SIP_OUTPUT);
	edif_cell_portconfig(cell, 6, "e0", IVL_SIP_OUTPUT);
	edif_cell_portconfig(cell, 7, "locked", IVL_SIP_OUTPUT);

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_PLL");

	return cell;
}

static edif_cell_t lpm_cell_nor(unsigned siz)
{
	unsigned idx;
	edif_cell_t cell;
	char name[32];

	sprintf(name, "nor%u", siz);

	cell = edif_xlibrary_findcell(xlib, name);
	if (cell != 0)
		return cell;

	cell = edif_xcell_create(xlib, strdup(name), siz+1);

	edif_cell_portconfig(cell, 0, "Result0", IVL_SIP_OUTPUT);
	edif_cell_port_pstring(cell, 0, "LPM_Polarity", "INVERT");

	for (idx = 0 ;  idx < siz ;  idx += 1) {
		sprintf(name, "Data%ux0", idx);
		edif_cell_portconfig(cell, idx+1, strdup(name), IVL_SIP_INPUT);
	}

	edif_cell_pstring(cell,  "LPM_TYPE",  "LPM_OR");
	edif_cell_pinteger(cell, "LPM_Width", 1);
	edif_cell_pinteger(cell, "LPM_Size",  siz);

	return cell;
}

static void lpm_show_header(ivl_design_t des)
{
	unsigned idx;
	ivl_scope_t root = ivl_design_root(des);
	unsigned sig_cnt = ivl_scope_sigs(root);
	unsigned nports = 0, pidx;

	/* Count the ports I'm going to use. */
	for (idx = 0 ;  idx < sig_cnt ;  idx += 1) {
		ivl_signal_t sig = ivl_scope_sig(root, idx);

		if (ivl_signal_port(sig) == IVL_SIP_NONE)
			continue;

		if (ivl_signal_attr(sig, "PAD") != 0)
			continue;

		nports += ivl_signal_pins(sig);
	}

	/* Create the base edf object. */
	edf = edif_create(ivl_scope_basename(root), nports);


	pidx = 0;
	for (idx = 0 ;  idx < sig_cnt ;  idx += 1) {
		edif_joint_t jnt;
		ivl_signal_t sig = ivl_scope_sig(root, idx);

		if (ivl_signal_port(sig) == IVL_SIP_NONE)
			continue;

		if (ivl_signal_attr(sig, "PAD") != 0)
			continue;

		if (ivl_signal_pins(sig) == 1) {
			edif_portconfig(edf, pidx, ivl_signal_basename(sig),
				ivl_signal_port(sig));

			assert(ivl_signal_pins(sig) == 1);
			jnt = edif_joint_of_nexus(edf, ivl_signal_pin(sig, 0));
			edif_port_to_joint(jnt, edf, pidx);

		} else {
			const char*name = ivl_signal_basename(sig);
			ivl_signal_port_t dir = ivl_signal_port(sig);
			char buf[128];
			unsigned bit;
			for (bit = 0 ;  bit < ivl_signal_pins(sig) ; bit += 1) {
				const char*tmp;
				sprintf(buf, "%s[%u]", name, bit);
				tmp = strdup(buf);
				edif_portconfig(edf, pidx+bit, tmp, dir);

				jnt = edif_joint_of_nexus(edf,ivl_signal_pin(sig,bit));
				edif_port_to_joint(jnt, edf, pidx+bit);
			}
		}

		pidx += ivl_signal_pins(sig);
	}

	assert(pidx == nports);

	xlib = edif_xlibrary_create(edf, "LPM_LIBRARY");
}

static void lpm_show_footer(ivl_design_t des)
{
	if ( strcmp(edif_blif, "blif") == 0 )
		blif_print(xnf, edf);
	else
		edif_print(xnf, edf);
}

static void hookup_logic_gate(ivl_net_logic_t net, edif_cell_t cell)
{
	unsigned pin, idx;

	edif_joint_t jnt;
	edif_cellref_t ref = edif_cellref_create(edf, cell);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
	pin = edif_cell_port_byname(cell, "Result0");
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 1 ;  idx < ivl_logic_pins(net) ;  idx += 1) {
		char name[32];

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, idx));
		sprintf(name, "Data%ux0", idx-1);
		pin = edif_cell_port_byname(cell, name);
		edif_add_to_joint(jnt, ref, pin);
	}
}

static void hookup_logic_gate_pll(ivl_net_logic_t net, edif_cell_t cell)
{
	unsigned pin;

	edif_joint_t jnt;
	edif_cellref_t ref = edif_cellref_create(edf, cell);
	
	//ADU	
	edif_cellref_pstring(ref, "up_module_name", ivl_logic_attr(net,"up_module_name"));
	

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
	pin = edif_cell_port_byname(cell, "inclk0");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 1));
	pin = edif_cell_port_byname(cell, "pllena");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 2));
	pin = edif_cell_port_byname(cell, "areset");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 3));
	pin = edif_cell_port_byname(cell, "pfdena");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 4));
	pin = edif_cell_port_byname(cell, "c0");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 5));
	pin = edif_cell_port_byname(cell, "c1");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 6));
	pin = edif_cell_port_byname(cell, "e0");
	edif_add_to_joint(jnt, ref, pin);

	jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 7));
	pin = edif_cell_port_byname(cell, "locked");
	edif_add_to_joint(jnt, ref, pin);
}

static void lpm_logic(ivl_net_logic_t net)
{
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	switch (ivl_logic_type(net)) {

	case IVL_LO_BUFZ:
	case IVL_LO_BUF:
		assert(ivl_logic_pins(net) == 2);
		cell = lpm_cell_buf();
		ref = edif_cellref_create(edf, cell);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
		edif_add_to_joint(jnt, ref, 0);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 1));
		edif_add_to_joint(jnt, ref, 1);
		break;

	case IVL_LO_BUFIF0:
		assert(ivl_logic_pins(net) == 3);
		cell = lpm_cell_bufif0();
		ref = edif_cellref_create(edf, cell);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
		edif_add_to_joint(jnt, ref, 0);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 1));
		edif_add_to_joint(jnt, ref, 1);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 2));
		edif_add_to_joint(jnt, ref, 2);
		break;

	case IVL_LO_BUFIF1:
		assert(ivl_logic_pins(net) == 3);
		cell = lpm_cell_bufif1();
		ref = edif_cellref_create(edf, cell);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
		edif_add_to_joint(jnt, ref, 0);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 1));
		edif_add_to_joint(jnt, ref, 1);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 2));
		edif_add_to_joint(jnt, ref, 2);
		break;

	case IVL_LO_NOT:
		assert(ivl_logic_pins(net) == 2);
		cell = lpm_cell_inv();
		ref = edif_cellref_create(edf, cell);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 0));
		edif_add_to_joint(jnt, ref, 0);

		jnt = edif_joint_of_nexus(edf, ivl_logic_pin(net, 1));
		edif_add_to_joint(jnt, ref, 1);
		break;

	case IVL_LO_OR:
		cell = lpm_cell_or(ivl_logic_pins(net)-1);
		hookup_logic_gate(net, cell);
		break;

	case IVL_LO_NOR:
		cell = lpm_cell_nor(ivl_logic_pins(net)-1);
		hookup_logic_gate(net, cell);
		break;

	case IVL_LO_AND:
		cell = lpm_cell_and(ivl_logic_pins(net)-1);
		hookup_logic_gate( net, cell);
		break;

	//begin-adu
	case IVL_LO_NAND:
		cell = lpm_cell_nand(ivl_logic_pins(net)-1);
		hookup_logic_gate( net, cell);
		break;
	//end-adu

	case IVL_LO_XOR:
		cell = lpm_cell_xor(ivl_logic_pins(net)-1);
		hookup_logic_gate( net, cell);
		break;

	//begin-adu
	case IVL_LO_XNOR:
		cell = lpm_cell_xnor(ivl_logic_pins(net)-1);
		hookup_logic_gate( net, cell);
		break;
	//end-adu

	case IVL_LO_PLL:
		cell = lpm_cell_pll(ivl_logic_pins(net)-1);
		hookup_logic_gate_pll( net, cell);
		break;

	default:
		fprintf(stderr, "UNSUPPORTED LOGIC TYPE: %u\n",
			ivl_logic_type(net));
		break;
	}
}


static void lpm_show_dff(ivl_lpm_t net)
{
	char name[64];
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	unsigned idx;
	unsigned pin, wid = ivl_lpm_width(net);

	sprintf(name, "fd%s%s%s%s%s%u",
		ivl_lpm_enable(net)? "ce" : "",
		ivl_lpm_async_clr(net)? "Ac" : "",
		ivl_lpm_sync_clr(net)? "Sc" : "",
		ivl_lpm_async_set(net)? "As" : "",
		ivl_lpm_sync_set(net)? "Ss" : "",
		wid);

	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		unsigned nports = 2 * wid + 1;
		pin = 0;
		if (ivl_lpm_enable(net))
			nports += 1;
		if (ivl_lpm_async_clr(net))
			nports += 1;
		if (ivl_lpm_sync_clr(net))
			nports += 1;
		if (ivl_lpm_async_set(net))
			nports += 1;
		if (ivl_lpm_sync_set(net))
			nports += 1;

		cell = edif_xcell_create(xlib, strdup(name), nports);
		edif_cell_pstring(cell,  "LPM_Type", "LPM_FF");
		edif_cell_pinteger(cell, "LPM_Width", wid);

		for (idx = 0 ;  idx < wid ;  idx += 1) {

			sprintf(name, "Q%u", idx);
			edif_cell_portconfig(cell, idx*2+0, strdup(name),
				IVL_SIP_OUTPUT);

			sprintf(name, "Data%u", idx);
			edif_cell_portconfig(cell, idx*2+1, strdup(name),
				IVL_SIP_INPUT);
		}

		pin = wid*2;

		if (ivl_lpm_enable(net)) {
			edif_cell_portconfig(cell, pin, "Enable", IVL_SIP_INPUT);
			pin += 1;
		}

		if (ivl_lpm_async_clr(net)) {
			edif_cell_portconfig(cell, pin, "Aclr", IVL_SIP_INPUT);
			pin += 1;
		}

		if (ivl_lpm_sync_clr(net)) {
			edif_cell_portconfig(cell, pin, "Sclr", IVL_SIP_INPUT);
			pin += 1;
		}

		if (ivl_lpm_async_set(net)) {
			edif_cell_portconfig(cell, pin, "Aset", IVL_SIP_INPUT);
			pin += 1;
		}

		if (ivl_lpm_sync_set(net)) {
			edif_cell_portconfig(cell, pin, "Sset", IVL_SIP_INPUT);
			pin += 1;
		}

		edif_cell_portconfig(cell, pin, "Clock", IVL_SIP_INPUT);
		pin += 1;

		assert(pin == nports);
	}

	ref = edif_cellref_create(edf, cell);

	pin = edif_cell_port_byname(cell, "Clock");

	jnt = edif_joint_of_nexus(edf, ivl_lpm_clk(net));
	edif_add_to_joint(jnt, ref, pin);

	if (ivl_lpm_enable(net)) {
		pin = edif_cell_port_byname(cell, "Enable");

		jnt = edif_joint_of_nexus(edf, ivl_lpm_enable(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	if (ivl_lpm_async_clr(net)) {
		pin = edif_cell_port_byname(cell, "Aclr");

		jnt = edif_joint_of_nexus(edf, ivl_lpm_async_clr(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	if (ivl_lpm_sync_clr(net)) {
		pin = edif_cell_port_byname(cell, "Sclr");

		jnt = edif_joint_of_nexus(edf, ivl_lpm_sync_clr(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	if (ivl_lpm_async_set(net)) {
		pin = edif_cell_port_byname(cell, "Aset");

		jnt = edif_joint_of_nexus(edf, ivl_lpm_async_set(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	if (ivl_lpm_sync_set(net)) {
		ivl_expr_t svalue = ivl_lpm_sset_value(net);

		pin = edif_cell_port_byname(cell, "Sset");

		jnt = edif_joint_of_nexus(edf, ivl_lpm_sync_set(net));
		edif_add_to_joint(jnt, ref, pin);

		edif_cellref_pinteger(ref, "LPM_Svalue", ivl_expr_uvalue(svalue));
	}

	for (idx = 0 ;  idx < wid ;  idx += 1) {

		sprintf(name, "Q%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_q(net, idx));
		edif_add_to_joint(jnt, ref, pin);

		sprintf(name, "Data%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
}
/*The start of lpm_show_latch()------ADU*/
static void lpm_show_latch(ivl_lpm_t net)
{
	char name[64];
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	unsigned idx;
	unsigned pin, wid = ivl_lpm_width(net);

	sprintf(name, "latch%s%s%u",
		ivl_lpm_async_clr(net)? "Ac" : "",
		ivl_lpm_async_set(net)? "As" : "",
		wid);

	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		unsigned nports = 2 * wid + 1;
		pin = 0;

		if (ivl_lpm_async_clr(net))
			nports += 1;
		if (ivl_lpm_async_set(net))
			nports += 1;

		cell = edif_xcell_create(xlib, strdup(name), nports);
		edif_cell_pstring(cell,  "LPM_Type", "LPM_latch");
		edif_cell_pinteger(cell, "LPM_Width", wid);

		for (idx = 0 ;  idx < wid ;  idx += 1) {

			sprintf(name, "Q%u", idx);
			edif_cell_portconfig(cell, idx*2+0, strdup(name),
				IVL_SIP_OUTPUT);

			sprintf(name, "Data%u", idx);
			edif_cell_portconfig(cell, idx*2+1, strdup(name),
				IVL_SIP_INPUT);
		}

		pin = wid*2;

		if (ivl_lpm_async_clr(net)) {
			edif_cell_portconfig(cell, pin, "Aclr", IVL_SIP_INPUT);
			pin += 1;
		}

		if (ivl_lpm_async_set(net)) {
			edif_cell_portconfig(cell, pin, "Aset", IVL_SIP_INPUT);
			pin += 1;
		}

		edif_cell_portconfig(cell, pin, "Gate", IVL_SIP_INPUT);
		pin += 1;

		assert(pin == nports);
	}

	ref = edif_cellref_create(edf, cell);

	pin = edif_cell_port_byname(cell, "Gate");
	jnt = edif_joint_of_nexus(edf, ivl_lpm_gate(net));
	edif_add_to_joint(jnt, ref, pin);

	if (ivl_lpm_async_clr(net)) {
		pin = edif_cell_port_byname(cell, "Aclr");
		jnt = edif_joint_of_nexus(edf, ivl_lpm_async_clr(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	if (ivl_lpm_async_set(net)) {
		pin = edif_cell_port_byname(cell, "Aset");
		jnt = edif_joint_of_nexus(edf, ivl_lpm_async_set(net));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < wid ;  idx += 1) {

		sprintf(name, "Q%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_q(net, idx));
		edif_add_to_joint(jnt, ref, pin);

		sprintf(name, "Data%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
}

/*The end of lpm_show_latch()------ADU*/

/*The start of lpm_show_cmp()------0019->ADU*/
static void lpm_show_cmp(ivl_lpm_t net)
{
	unsigned idx;
	unsigned cell_width;
	char cellname[32];
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;
	unsigned pin;

	const char*name;
	const char*type;
	const char*signed_flag = "UNSIGNED";//在property中添加符号位标识符
	
	/*BEGIN-ADU*/
	if(ivl_lpm_signed(net) == 1)
		signed_flag = "SIGNED";
	/*END-ADU*/
	if (ivl_lpm_type(net) == IVL_LPM_CMP_EQ) {
		name = "CMP_EQ";
		type = "LPM_CMP_EQ";
	}
	else if (ivl_lpm_type(net) == IVL_LPM_CMP_NE) {
		name = "CMP_NE";
		type = "LPM_CMP_NE";
	}
	else if (ivl_lpm_type(net) == IVL_LPM_CMP_GE) {
		//将符号位标识符添加到比较器类别当中去——ADU
		if(strcmp(signed_flag,"SIGNED") == 0){
			name = "CMP_GE_SIGNED";
		}else{
			name = "CMP_GE_UNSIGNED";
		}
		type = "LPM_CMP_GE";
	}
	else if (ivl_lpm_type(net) == IVL_LPM_CMP_GT) {
		//将符号位标识符添加到比较器类别当中去——ADU
		if(strcmp(signed_flag,"SIGNED") == 0){
			name = "CMP_GT_SIGNED";
		}else{
			name = "CMP_GT_UNSIGNED";
		}
		type = "LPM_CMP_GT";
	}
	/*begin-ADU*/
	else if (ivl_lpm_type(net) == IVL_LPM_CMP_LE) {
		name = "CMP_LE";
		type = "LPM_CMP_LE";
	}
	else if (ivl_lpm_type(net) == IVL_LPM_CMP_LT) {
		name = "CMP_LT";
		type = "LPM_CMP_LT";
	}
	/*end-ADU*/
	else
		assert(0);

	cell_width = ivl_lpm_width(net);

	sprintf(cellname, "%s_%u", name, cell_width);
	cell = edif_xlibrary_findcell(xlib, cellname);

	if (cell == 0) {
		unsigned pins = cell_width * 2 + 1;

		cell = edif_xcell_create(xlib, strdup(cellname), pins);

		sprintf(cellname, "Result");
		edif_cell_portconfig(cell, 0, strdup(cellname),
			IVL_SIP_OUTPUT);

		for (idx = 0 ;  idx < cell_width ;  idx += 1) {
			sprintf(cellname, "DataA%u", idx);
			edif_cell_portconfig(cell, idx*2+1, strdup(cellname),
				IVL_SIP_INPUT);

			sprintf(cellname, "DataB%u", idx);
			edif_cell_portconfig(cell, idx*2+2, strdup(cellname),
				IVL_SIP_INPUT);
		}

		edif_cell_pstring(cell,  "LPM_Type",  type);
		edif_cell_pinteger(cell, "LPM_Width",  ivl_lpm_width(net));
		edif_cell_pstring(cell,  "LPM_Signed_Flag",  signed_flag);
	}

	ref = edif_cellref_create(edf, cell);

	sprintf(cellname, "Result");
	pin = edif_cell_port_byname(cell, cellname);

	jnt = edif_joint_of_nexus(edf, ivl_lpm_q(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < cell_width ;  idx += 1) {
		sprintf(cellname, "DataA%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);

		sprintf(cellname, "DataB%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
}
/*The end of lpm_show_cmp()------0019->ADU*/

static void lpm_show_mux(ivl_lpm_t net)
{
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	unsigned idx, rdx;

	char cellname[32];

	unsigned wid_r = ivl_lpm_width(net);
	unsigned wid_s = ivl_lpm_selects(net);
	unsigned wid_z = ivl_lpm_size(net);

	sprintf(cellname, "mux%u_%u_%u", wid_r, wid_s, wid_z);
	cell = edif_xlibrary_findcell(xlib, cellname);

	if (cell == 0) {
		unsigned pins = wid_r + wid_s + wid_r*wid_z;

		cell = edif_xcell_create(xlib, strdup(cellname), pins);

		/* Make the output ports. */
		for (idx = 0 ;  idx < wid_r ;  idx += 1) {
			sprintf(cellname, "Result%u", idx);
			edif_cell_portconfig(cell, idx, strdup(cellname),
				IVL_SIP_OUTPUT);
		}

		/* Make the select ports. */
		for (idx = 0 ;  idx < wid_s ;  idx += 1) {
			sprintf(cellname, "Sel%u", idx);
			edif_cell_portconfig(cell, wid_r+idx, strdup(cellname),
				IVL_SIP_INPUT);
		}

		for (idx = 0 ;  idx < wid_z ; idx += 1) {
			unsigned base = wid_r + wid_s + wid_r * idx;
			unsigned rdx;

			for (rdx = 0 ;  rdx < wid_r ;  rdx += 1) {
				sprintf(cellname, "Data%ux%u", idx, rdx);
				edif_cell_portconfig(cell, base+rdx, strdup(cellname),
					IVL_SIP_INPUT);
			}
		}

		edif_cell_pstring(cell,  "LPM_Type",   "LPM_MUX");
		edif_cell_pinteger(cell, "LPM_Width",  wid_r);
		edif_cell_pinteger(cell, "LPM_WidthS", wid_s);
		edif_cell_pinteger(cell, "LPM_Size",   wid_z);
	}


	ref = edif_cellref_create(edf, cell);

	/* Connect the pins of the instance to the nexa. Access the
	cell pins by name. */
	for (idx = 0 ;  idx < wid_r ;  idx += 1) {
		unsigned pin;

		sprintf(cellname, "Result%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_q(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < wid_s ;  idx += 1) {
		unsigned pin;

		sprintf(cellname, "Sel%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_select(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < wid_z ;  idx += 1) {
		for (rdx = 0 ;  rdx < wid_r ;  rdx += 1) {
			unsigned pin;

			sprintf(cellname, "Data%ux%u", idx, rdx);
			pin = edif_cell_port_byname(cell, cellname);

			jnt = edif_joint_of_nexus(edf, ivl_lpm_data2(net, idx, rdx));
			edif_add_to_joint(jnt, ref, pin);
		}
	}
}

static void lpm_show_add(ivl_lpm_t net)
{
	unsigned idx;
	unsigned cell_width;
	char cellname[32];
	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	const char*type = "ADD";

	if (ivl_lpm_type(net) == IVL_LPM_SUB)
		type = "SUB";

	/* Figure out the width of the cell. Normally, it is the LPM
	width known by IVL. But if the top data input bits are
	unconnected, then we really have a width one less, and we
	can use the cout to fill out the output width. */
	cell_width = ivl_lpm_width(net);
	if ( (ivl_lpm_data(net,cell_width-1) == 0)
		&& (ivl_lpm_datab(net,cell_width-1) == 0) )
		cell_width -= 1;

	/* Find the correct ADD/SUB device in the library, search by
	name. If the device is not there, then create it and put it
	in the library. */
	sprintf(cellname, "%s%u", type, cell_width);
	cell = edif_xlibrary_findcell(xlib, cellname);

	if (cell == 0) {
		unsigned pins = cell_width * 3;

		cell = edif_xcell_create(xlib, strdup(cellname), pins);

		for (idx = 0 ;  idx < cell_width ;  idx += 1) {

			sprintf(cellname, "Result%u", idx);
			edif_cell_portconfig(cell, idx*3+0, strdup(cellname),
				IVL_SIP_OUTPUT);

			sprintf(cellname, "DataA%u", idx);
			edif_cell_portconfig(cell, idx*3+1, strdup(cellname),
				IVL_SIP_INPUT);

			sprintf(cellname, "DataB%u", idx);
			edif_cell_portconfig(cell, idx*3+2, strdup(cellname),
				IVL_SIP_INPUT);
		}

		edif_cell_pstring(cell,  "LPM_Type",      "LPM_ADD_SUB");
		edif_cell_pstring(cell,  "LPM_Direction", type);
		edif_cell_pinteger(cell, "LPM_Width",     ivl_lpm_width(net));
	}

	ref = edif_cellref_create(edf, cell);

	/* Connect the pins of the instance to the nexa. Access the
	cell pins by name. */
	for (idx = 0 ;  idx < cell_width ;  idx += 1) {
		unsigned pin;

		sprintf(cellname, "Result%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_q(net, idx));
		edif_add_to_joint(jnt, ref, pin);

		sprintf(cellname, "DataA%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);

		sprintf(cellname, "DataB%u", idx);
		pin = edif_cell_port_byname(cell, cellname);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
}

static void lpm_show_mult(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "mult%u_%u_%u",
		ivl_lpm_mult_width(net,0), ivl_lpm_mult_width(net,1), ivl_lpm_mult_width(net,3));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			ivl_lpm_mult_width(net,0) + ivl_lpm_mult_width(net,1) + ivl_lpm_mult_width(net,3));

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
			sprintf(name, "Result%u", idx);
			edif_cell_portconfig(cell, idx,
				strdup(name),
				IVL_SIP_OUTPUT);
		}
		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
			sprintf(name, "DataA%u", idx);
			edif_cell_portconfig(cell, idx + ivl_lpm_mult_width(net,3),
				strdup(name),
				IVL_SIP_INPUT);
		}
		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
			sprintf(name, "DataB%u", idx);
			edif_cell_portconfig(cell, idx + ivl_lpm_mult_width(net,3) + ivl_lpm_mult_width(net,0),
				strdup(name),
				IVL_SIP_INPUT);
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT");
		edif_cell_pinteger(cell, "LPM_WidthP", ivl_lpm_mult_width(net,3));
		edif_cell_pinteger(cell, "LPM_WidthA", ivl_lpm_mult_width(net,0));
		edif_cell_pinteger(cell, "LPM_WidthB", ivl_lpm_mult_width(net,1));
	}

	ref = edif_cellref_create(edf, cell);

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
		unsigned pin;
		ivl_nexus_t nex;

		sprintf(name, "Result%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
		unsigned pin;
		ivl_nexus_t nex;

		sprintf(name, "DataA%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
		unsigned pin;
		ivl_nexus_t nex;

		sprintf(name, "DataB%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_mip(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell = NULL;
	edif_cellref_t ref;
	edif_joint_t jnt;
	ivl_nexus_t nex;

	//sprintf(name, "mem_aa%u_ab%u_da%u_db%u_qa%u_qb%u",
	//	ivl_lpm_mip_width(net,0), ivl_lpm_mip_width(net,1), ivl_lpm_mip_width(net,2),
	//	ivl_lpm_mip_width(net,3), ivl_lpm_mip_width(net,4), ivl_lpm_mip_width(net,5));
	
	//sprintf(name, "dual_port_ram");//name只有一个的话后边的这个语句要注释
	////cell = edif_xlibrary_findcell(xlib, name);

	sprintf(name, "dual_port_ram_aa%u_ab%u_da%u_db%u_qa%u_qb%u",
		ivl_lpm_mip_width(net,0), ivl_lpm_mip_width(net,1), ivl_lpm_mip_width(net,2),
		ivl_lpm_mip_width(net,3), ivl_lpm_mip_width(net,4), ivl_lpm_mip_width(net,5));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			3 + ivl_lpm_mip_width(net,0) + ivl_lpm_mip_width(net,1) + ivl_lpm_mip_width(net,2)
			+ ivl_lpm_mip_width(net,3) + ivl_lpm_mip_width(net,4) + ivl_lpm_mip_width(net,5));
		
		sprintf(name, "we1");
		edif_cell_portconfig(cell, 0,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "we2");
		edif_cell_portconfig(cell, 1,
			strdup(name),
			IVL_SIP_INPUT);
		pin_idx = 2;

		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,0) ;  idx += 1) {
			sprintf(name, "addr1_%u", idx);
			edif_cell_portconfig(cell, pin_idx, strdup(name), IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,1) ;  idx += 1) {
			sprintf(name, "addr2_%u", idx);
			edif_cell_portconfig(cell, pin_idx, strdup(name), IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,2) ;  idx += 1) {
			sprintf(name, "data1_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,3) ;  idx += 1) {
			sprintf(name, "data2_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "clk");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);
		pin_idx +=1;

		if (ivl_lpm_mip_width(net,5) == 0) {
			for (idx = 0 ;  idx < ivl_lpm_mip_width(net,4) ;  idx += 1) {
				//sprintf(name, "q_%u", idx);
				sprintf(name, "out1_%u", idx);//ADU:
				edif_cell_portconfig(cell, pin_idx,
					strdup(name),
					IVL_SIP_OUTPUT);
				pin_idx++;
			}
		}
		else if (ivl_lpm_mip_width(net,4) == 0) {
			for (idx = 0 ;  idx < ivl_lpm_mip_width(net,5) ;  idx += 1) {
				//sprintf(name, "q_%u", idx);
				sprintf(name, "out2_%u", idx);//ADU:
				edif_cell_portconfig(cell, pin_idx,
					strdup(name),
					IVL_SIP_OUTPUT);
				pin_idx++;
			}
		}
		else {
			for (idx = 0 ;  idx < ivl_lpm_mip_width(net,4) ;  idx += 1) {
				//sprintf(name, "q_%u", idx);
				sprintf(name, "out1_%u", idx);//adu:
				edif_cell_portconfig(cell, pin_idx,
					strdup(name),
					IVL_SIP_OUTPUT);
				pin_idx++;
			}

			for (idx = 0 ;  idx < ivl_lpm_mip_width(net,5) ;  idx += 1) {
				//sprintf(name, "q_%u", 18+idx);
				sprintf(name, "out2_%u", idx);//ADU:
				edif_cell_portconfig(cell, pin_idx,
					strdup(name),
					IVL_SIP_OUTPUT);
				pin_idx++;
			}
		}

		//sprintf(name, "clocken1");
		//edif_cell_portconfig(cell, 1,
		//	strdup(name),
		//	IVL_SIP_INPUT);



		//sprintf(name, "aclr0");
		//edif_cell_portconfig(cell, 3,
		//	strdup(name),
		//	IVL_SIP_INPUT);

		//sprintf(name, "clock0");
		//edif_cell_portconfig(cell, 4,
		//	strdup(name),
		//	IVL_SIP_INPUT);

		//sprintf(name, "wren_b");
		//edif_cell_portconfig(cell, 5,
		//	strdup(name),
		//	IVL_SIP_INPUT);

		//sprintf(name, "aclr1");
		//edif_cell_portconfig(cell, 6,
		//	strdup(name),
		//	IVL_SIP_INPUT);

		//sprintf(name, "clock1");
		//edif_cell_portconfig(cell, 7,
		//	strdup(name),
		//	IVL_SIP_INPUT);
		edif_cell_pstring(  cell,  "LPM_Type",  "LPM_MIP");
		edif_cell_pinteger(cell, "LPM_Width_Address_A", ivl_lpm_mip_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Address_B", ivl_lpm_mip_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Data_A", ivl_lpm_mip_width(net,2));
		edif_cell_pinteger(cell, "LPM_Width_Data_B", ivl_lpm_mip_width(net,3));
		edif_cell_pinteger(cell, "LPM_Width_OUT_A", ivl_lpm_mip_width(net,4));
		edif_cell_pinteger(cell, "LPM_Width_OUT_B", ivl_lpm_mip_width(net,5));
	}

	ref = edif_cellref_create(edf, cell);
	//ADU	
	edif_cellref_pstring(ref, "init_file", ivl_lpm_attr_string(net,"init_file"));
	edif_cellref_pstring(ref, "mode", ivl_lpm_attr_string(net,"mode"));
	edif_cellref_pstring(ref, "up_module_name", ivl_lpm_attr_string(net,"up_module_name"));
	edif_cellref_pstring(ref, "up_scope_name", ivl_lpm_attr_string(net,"up_scope_name"));
	edif_cellref_pinteger(ref, "port_a_first_address", ivl_lpm_attr_integer(net,"port_a_first_address"));
	edif_cellref_pinteger(ref, "port_a_last_address", ivl_lpm_attr_integer(net,"port_a_last_address"));
	edif_cellref_pinteger(ref, "port_a_first_bit_number", ivl_lpm_attr_integer(net,"port_a_first_bit_number"));
	if(strcmp("single_port", ivl_lpm_attr_string(net,"mode")) != 0){
		edif_cellref_pinteger(ref, "port_b_first_address", ivl_lpm_attr_integer(net,"port_b_first_address"));
		edif_cellref_pinteger(ref, "port_b_last_address", ivl_lpm_attr_integer(net,"port_b_last_address"));
		edif_cellref_pinteger(ref, "port_b_first_bit_number", ivl_lpm_attr_integer(net,"port_b_first_bit_number"));
	}

	//sprintf(name, "clocken1");
	//pin = edif_cell_port_byname(cell, name);
	//jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 1));
	//edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "we1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	//sprintf(name, "aclr0");
	//pin = edif_cell_port_byname(cell, name);
	//jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 3));
	//edif_add_to_joint(jnt, ref, pin);

	//sprintf(name, "clock0");
	//pin = edif_cell_port_byname(cell, name);
	//jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 4));
	//edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "we2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 5));
	edif_add_to_joint(jnt, ref, pin);

	//sprintf(name, "aclr1");
	//pin = edif_cell_port_byname(cell, name);
	//jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 6));
	//edif_add_to_joint(jnt, ref, pin);

	//sprintf(name, "clock1");
	//pin = edif_cell_port_byname(cell, name);
	//jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 7));
	//edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_mip_width(net,0) ;  idx += 1) {
		sprintf(name, "addr1_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_address_a(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mip_width(net,1) ;  idx += 1) {
		sprintf(name, "addr2_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_address_b(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mip_width(net,2) ;  idx += 1) {
		sprintf(name, "data1_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_data_a(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mip_width(net,3) ;  idx += 1) {
		sprintf(name, "data2_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_data_b(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "clk");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	if (ivl_lpm_mip_width(net,5) == 0) {
		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,4) ;  idx += 1) {
			//sprintf(name, "q_%u", idx);
			sprintf(name, "out1_%u", idx);//ADU:
			pin = edif_cell_port_byname(cell, name);
			jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_q_a(net, idx));
			edif_add_to_joint(jnt, ref, pin);
		}
	}
	else if (ivl_lpm_mip_width(net,4) == 0) {
		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,5) ;  idx += 1) {
			//sprintf(name, "q_%u", idx);
			sprintf(name, "out2_%u", idx);//ADU:
			pin = edif_cell_port_byname(cell, name);
			jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_q_b(net, idx));
			edif_add_to_joint(jnt, ref, pin);
		}
	}
	else {
		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,4) ;  idx += 1) {
			//sprintf(name, "q_%u", idx);
			sprintf(name, "out1_%u", idx);//ADU:
			pin = edif_cell_port_byname(cell, name);
			jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_q_a(net, idx));
			edif_add_to_joint(jnt, ref, pin);
		}

		for (idx = 0 ;  idx < ivl_lpm_mip_width(net,5) ;  idx += 1) {
			//sprintf(name, "q_%u", 18+idx);
			sprintf(name, "out2_%u", idx);//ADU:
			pin = edif_cell_port_byname(cell, name);
			jnt = edif_joint_of_nexus(edf, ivl_lpm_mip_q_b(net, idx));
			edif_add_to_joint(jnt, ref, pin);
		}
	}
}

static void lpm_show_square(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "square_%u_%u",
		ivl_lpm_mult_width(net,0), ivl_lpm_mult_width(net,1));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			3 + ivl_lpm_mult_width(net,0) + ivl_lpm_mult_width(net,1));

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
			sprintf(name, "data_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "clken");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr");
		edif_cell_portconfig(cell, 1+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock");
		edif_cell_portconfig(cell, 2+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		pin_idx += 3;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_SQUARE");
		edif_cell_pinteger(cell, "LPM_Width_Data", ivl_lpm_mult_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Result", ivl_lpm_mult_width(net,1));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
		sprintf(name, "data_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "clken");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 1));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_lmult(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;
	const unsigned n0 =18;
	const unsigned n1 =18;
	const unsigned n2 =1;
	const unsigned n3 =36;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "mult_%u_%u_%u_%u",
		ivl_lpm_mult_width(net,0), ivl_lpm_mult_width(net,1), ivl_lpm_mult_width(net,2), ivl_lpm_mult_width(net,3));

	if (ivl_lpm_mult_width(net,0)!=18||ivl_lpm_mult_width(net,1)!=18)
		sprintf(name, "mult_%u_%u_%u_%u",n0, n1, n2, n3);

	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			3 + ivl_lpm_mult_width(net,0) + ivl_lpm_mult_width(net,1) + ivl_lpm_mult_width(net,2) + ivl_lpm_mult_width(net,3));

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
			sprintf(name, "dataa_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
			sprintf(name, "datab_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "clken");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr");
		edif_cell_portconfig(cell, 1+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock");
		edif_cell_portconfig(cell, 2+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		pin_idx += 3;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
			sprintf(name, "sum_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT");
		edif_cell_pinteger(cell, "LPM_Width_Data_A", ivl_lpm_mult_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Data_B", ivl_lpm_mult_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Sum", ivl_lpm_mult_width(net,2));
		edif_cell_pinteger(cell, "LPM_Width_Result", ivl_lpm_mult_width(net,3));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
		sprintf(name, "dataa_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
		sprintf(name, "datab_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "clken");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 1));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
		sprintf(name, "sum_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sum(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_lmultout(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;
	const unsigned n0 =36;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "multout_%u_%u",
		ivl_lpm_multout_width(net,0), ivl_lpm_multout_width(net,1));

	if (ivl_lpm_multout_width(net,0)!=36)
		sprintf(name, "multout_%u_%u",n0, n0);

	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name), ivl_lpm_multout_width(net,0) + ivl_lpm_multout_width(net,1) );

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,0) ;  idx += 1) {
			sprintf(name, "dataa_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,1) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		edif_cell_pstring(cell,  "LPM_Type",  "NetLMultSimpleOUT");
		edif_cell_pinteger(cell, "LPMOUT_Width_Data_A", ivl_lpm_multout_width(net,0));
		edif_cell_pinteger(cell, "LPMOUT_Width_Result", ivl_lpm_multout_width(net,1));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,0) ;  idx += 1) {
		sprintf(name, "dataa_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,1) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_mult_add(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "mult_add_%u_%u_%u_%u_%u",
		ivl_lpm_mult_width(net,0), ivl_lpm_mult_width(net,1), ivl_lpm_mult_width(net,2), ivl_lpm_mult_width(net,3), ivl_lpm_mult_width(net,4));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			14 + ivl_lpm_mult_width(net,0) + ivl_lpm_mult_width(net,1) + ivl_lpm_mult_width(net,2) + ivl_lpm_mult_width(net,3) + ivl_lpm_mult_width(net,4));

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
			sprintf(name, "dataa_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
			sprintf(name, "datab_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "clock0");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock1");
		edif_cell_portconfig(cell, 1+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock2");
		edif_cell_portconfig(cell, 2+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock3");
		edif_cell_portconfig(cell, 3+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr0");
		edif_cell_portconfig(cell, 4+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr1");
		edif_cell_portconfig(cell, 5+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr2");
		edif_cell_portconfig(cell, 6+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr3");
		edif_cell_portconfig(cell, 7+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena0");
		edif_cell_portconfig(cell, 8+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena1");
		edif_cell_portconfig(cell, 9+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena2");
		edif_cell_portconfig(cell, 10+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena3");
		edif_cell_portconfig(cell, 11+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "signa");
		edif_cell_portconfig(cell, 12+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "signb");
		edif_cell_portconfig(cell, 13+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		pin_idx += 14;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
			sprintf(name, "scanouta_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
			sprintf(name, "scanoutb_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,4) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT_ADD");
		edif_cell_pinteger(cell, "LPM_Width_Data_A", ivl_lpm_mult_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Data_B", ivl_lpm_mult_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Scanout_A", ivl_lpm_mult_width(net,2));
		edif_cell_pinteger(cell, "LPM_Width_Scanout_B", ivl_lpm_mult_width(net,3));
		edif_cell_pinteger(cell, "LPM_Width_Result", ivl_lpm_mult_width(net,4));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
		sprintf(name, "dataa_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
		sprintf(name, "datab_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "clock0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 1));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 3));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 4));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 5));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 6));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 7));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 8));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 9));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 10));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 11));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "signa");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 12));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "signb");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 13));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
		sprintf(name, "scanouta_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_scanouta(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
		sprintf(name, "scanoutb_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_scanoutb(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,4) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_mult_addout(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	unsigned numdataa = ivl_lpm_multout_width(net,0);
	unsigned numdatab = ivl_lpm_multout_width(net,1);
	unsigned numdatac = ivl_lpm_multout_width(net,2);
	unsigned numdatad = ivl_lpm_multout_width(net,3);
	unsigned numaddsub = ivl_lpm_multout_width(net,4);
	unsigned numaccum = ivl_lpm_multout_width(net,5);
	unsigned numresult = ivl_lpm_multout_width(net,6);
	
	unsigned flag_num = 0;
	if (numdataa) flag_num += 1;
	if (numdatab) flag_num += 1;
	if (numdatac) flag_num += 1;
	if (numdatad) flag_num += 1;
	
	if (!numaccum){//乘加
		if (flag_num == 1)
			sprintf(name, "mult_1add_out_%u_%u",36,36);
		else if (flag_num == 2)
			sprintf(name, "mult_2add_out_%u_%u_%u",36,36,37);
		else if (flag_num == 3)
			sprintf(name, "mult_3add_out_%u_%u_%u_%u",36,36,36,38);
		else if (flag_num == 4)
			sprintf(name, "mult_4add_out_%u_%u_%u_%u_%u",36,36,36,36,39);
		else
			assert(0);
	}
	else{               //乘累加
		if (flag_num == 1)
			sprintf(name, "mult_accum_%u_%u",36,36);
		else if (flag_num == 2)
			sprintf(name, "mult_2accum_%u_%u_%u",36,36,44);
		else if (flag_num == 3)
			sprintf(name, "mult_3accum_%u_%u_%u_%u",36,36,36,38);
		else if (flag_num == 4)
			sprintf(name, "mult_4accum_%u_%u_%u_%u_%u",36,36,36,36,39);
		else
			assert(0);
	}

	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			14 + ivl_lpm_multout_width(net,0) + ivl_lpm_multout_width(net,1) + ivl_lpm_multout_width(net,2) + ivl_lpm_multout_width(net,3) + ivl_lpm_multout_width(net,4)
			     + ivl_lpm_multout_width(net,5)+ ivl_lpm_multout_width(net,6));


		pin_idx = 0;
		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,0) ;  idx += 1) {
			sprintf(name, "dataa_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,1) ;  idx += 1) {
			sprintf(name, "datab_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,2) ;  idx += 1) {
			sprintf(name, "datac_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,3) ;  idx += 1) {
			sprintf(name, "datad_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,4) ;  idx += 1) {
			sprintf(name, "addnsub_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,5) ;  idx += 1) {
			sprintf(name, "accum_sload%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "clock0");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock1");
		edif_cell_portconfig(cell, 1+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock2");
		edif_cell_portconfig(cell, 2+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock3");
		edif_cell_portconfig(cell, 3+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr0");
		edif_cell_portconfig(cell, 4+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr1");
		edif_cell_portconfig(cell, 5+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr2");
		edif_cell_portconfig(cell, 6+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr3");
		edif_cell_portconfig(cell, 7+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena0");
		edif_cell_portconfig(cell, 8+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena1");
		edif_cell_portconfig(cell, 9+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena2");
		edif_cell_portconfig(cell, 10+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena3");
		edif_cell_portconfig(cell, 11+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "signa");
		edif_cell_portconfig(cell, 12+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "signb");
		edif_cell_portconfig(cell, 13+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		pin_idx += 14;

		for (idx = 0 ;  idx < ivl_lpm_multout_width(net,6) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}



		if (!ivl_lpm_multout_width(net,5)){
		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT_ADD_OUT");
		}
		else{
		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT_MAC_OUT");
		}
		edif_cell_pinteger(cell, "LPM_Width_Data_A", ivl_lpm_multout_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Data_B", ivl_lpm_multout_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Data_C", ivl_lpm_multout_width(net,2));
		edif_cell_pinteger(cell, "LPM_Width_Data_D", ivl_lpm_multout_width(net,3));
		edif_cell_pinteger(cell, "LPM_Width_ADDSUB", ivl_lpm_multout_width(net,4));
		edif_cell_pinteger(cell, "LPM_Width_Result", ivl_lpm_multout_width(net,6));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,0) ;  idx += 1) {
		sprintf(name, "dataa_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,1) ;  idx += 1) {
		sprintf(name, "datab_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,2) ;  idx += 1) {
		sprintf(name, "datac_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_datac(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,3) ;  idx += 1) {
		sprintf(name, "datad_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_datad(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,4) ;  idx += 1) {
		sprintf(name, "addnsub_%u", idx);//这个地方改了一天！
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_addsub(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,5) ;  idx += 1) {
		sprintf(name, "accum_sload%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_accumsload(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "clock0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 1));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 3));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 4));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 5));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 6));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 7));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 8));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 9));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 10));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 11));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "signa");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 12));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "signb");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_sbit(net, 13));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_multout_width(net,6) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_multout_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

static void lpm_show_mult_accum(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "mult_accum_%u_%u_%u_%u_%u",
		ivl_lpm_mult_width(net,0), ivl_lpm_mult_width(net,1), ivl_lpm_mult_width(net,2), ivl_lpm_mult_width(net,3), ivl_lpm_mult_width(net,4));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			14 + ivl_lpm_mult_width(net,0) + ivl_lpm_mult_width(net,1) + ivl_lpm_mult_width(net,2) + ivl_lpm_mult_width(net,3) + ivl_lpm_mult_width(net,4));

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
			sprintf(name, "dataa_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
			sprintf(name, "datab_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		sprintf(name, "accum_sload");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock0");
		edif_cell_portconfig(cell, 1+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock1");
		edif_cell_portconfig(cell, 2+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock2");
		edif_cell_portconfig(cell, 3+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "clock3");
		edif_cell_portconfig(cell, 4+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr0");
		edif_cell_portconfig(cell, 5+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr1");
		edif_cell_portconfig(cell, 6+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr2");
		edif_cell_portconfig(cell, 7+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "aclr3");
		edif_cell_portconfig(cell, 8+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena0");
		edif_cell_portconfig(cell, 9+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena1");
		edif_cell_portconfig(cell, 10+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena2");
		edif_cell_portconfig(cell, 11+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		sprintf(name, "ena3");
		edif_cell_portconfig(cell, 12+pin_idx,
			strdup(name),
			IVL_SIP_INPUT);

		pin_idx += 13;

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
			sprintf(name, "scanouta_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
			sprintf(name, "scanoutb_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_mult_width(net,4) ;  idx += 1) {
			sprintf(name, "result_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}

		sprintf(name, "overflow");
		edif_cell_portconfig(cell, pin_idx,
			strdup(name),
			IVL_SIP_OUTPUT);

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_MULT_ACCUM");
		edif_cell_pinteger(cell, "LPM_Width_Data_A", ivl_lpm_mult_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Data_B", ivl_lpm_mult_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Scanout_A", ivl_lpm_mult_width(net,2));
		edif_cell_pinteger(cell, "LPM_Width_Scanout_B", ivl_lpm_mult_width(net,3));
		edif_cell_pinteger(cell, "LPM_Width_Result", ivl_lpm_mult_width(net,4));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,0) ;  idx += 1) {
		sprintf(name, "dataa_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_dataa(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,1) ;  idx += 1) {
		sprintf(name, "datab_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_datab(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "accum_sload");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 0));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 1));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 2));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 3));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "clock3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 4));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 5));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 6));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 7));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "aclr3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 8));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena0");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 9));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena1");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 10));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena2");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 11));
	edif_add_to_joint(jnt, ref, pin);

	sprintf(name, "ena3");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 12));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,2) ;  idx += 1) {
		sprintf(name, "scanouta_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_scanouta(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,3) ;  idx += 1) {
		sprintf(name, "scanoutb_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_scanoutb(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_mult_width(net,4) ;  idx += 1) {
		sprintf(name, "result_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	sprintf(name, "overflow");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_mult_sbit(net, 13));
	edif_add_to_joint(jnt, ref, pin);

}

static void lpm_show_HalfDSPBlock(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin_idx;
	unsigned pin;

	edif_cell_t cell = NULL;
	edif_cellref_t ref;
	edif_joint_t jnt;

	//sprintf(name, "multiply");//如果name不可变，下边的语句要注释。
	////cell = edif_xlibrary_findcell(xlib, name);

	sprintf(name, "multiply_%u_%u", ivl_lpm_mult_width(net,0),  ivl_lpm_mult_width(net,4));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			ivl_lpm_halfdspblock_width(net,0) + ivl_lpm_halfdspblock_width(net,1) + ivl_lpm_halfdspblock_width(net,2) + ivl_lpm_halfdspblock_width(net,3) + ivl_lpm_halfdspblock_width(net,4)
		  +ivl_lpm_halfdspblock_width(net,5) + ivl_lpm_halfdspblock_width(net,6) + ivl_lpm_halfdspblock_width(net,7));

		pin_idx = 0;

		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,0) ;  idx += 1) {
			sprintf(name, "a_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,1) ;  idx += 1) {
			sprintf(name, "b_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,4) ;  idx += 1) {
			sprintf(name, "out_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_OUTPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,5) ;  idx += 1) {
			sprintf(name, "clock_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,6) ;  idx += 1) {
			sprintf(name, "ena_%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}
		for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,7) ;  idx += 1) {
			sprintf(name, "accum_sload%u", idx);
			edif_cell_portconfig(cell, pin_idx,
				strdup(name),
				IVL_SIP_INPUT);
			pin_idx++;
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_HalfDSPBlock");
		edif_cell_pinteger(cell, "LPM_Width_Data_A0", ivl_lpm_halfdspblock_width(net,0));
		edif_cell_pinteger(cell, "LPM_Width_Data_B0", ivl_lpm_halfdspblock_width(net,1));
		edif_cell_pinteger(cell, "LPM_Width_Result",     ivl_lpm_halfdspblock_width(net,4));
	}

	ref = edif_cellref_create(edf, cell);


	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,0) ;  idx += 1) {
		sprintf(name, "a_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_dataa0(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,1) ;  idx += 1) {
		sprintf(name, "b_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_datab0(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,4) ;  idx += 1) {
		sprintf(name, "out_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_result(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,5) ;  idx += 1) {
		sprintf(name, "clock_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_clock(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,6) ;  idx += 1) {
		sprintf(name, "ena_%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_ena(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_halfdspblock_width(net,7) ;  idx += 1) {
		sprintf(name, "accum_sload%u", idx);
		pin = edif_cell_port_byname(cell, name);
		jnt = edif_joint_of_nexus(edf, ivl_lpm_halfdspblock_accumsload(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}


static void lpm_show_constant(ivl_net_const_t net)
{
	/* We only need one instance each of constant 0 and 1 bits. If
	we need either of them, then create an instance reference and
	save that reference here so that later needs for 0 or 1 can
	find that the reference already lives and can be added to the
	joint. */
	static edif_cellref_t cell0_ref = 0;
	static edif_cellref_t cell1_ref = 0;

	static edif_joint_t cell0_jnt = 0;
	static edif_joint_t cell1_jnt = 0;

	edif_cell_t cell0 = edif_xlibrary_findcell(xlib, "cell0");
	edif_cell_t cell1 = edif_xlibrary_findcell(xlib, "cell1");

	const char*bits;
	unsigned idx;

	if (cell0 == 0) {
		cell0 = edif_xcell_create(xlib, "cell0", 1);
		edif_cell_portconfig(cell0, 0, "Result0", IVL_SIP_OUTPUT);

		edif_cell_pstring(cell0,  "LPM_Type",   "LPM_CONSTANT");
		edif_cell_pinteger(cell0, "LPM_Width",  1);
		edif_cell_pinteger(cell0, "LPM_CValue", 0);
	}

	if (cell1 == 0) {
		cell1 = edif_xcell_create(xlib, "cell1", 1);
		edif_cell_portconfig(cell1, 0, "Result0", IVL_SIP_OUTPUT);

		edif_cell_pstring(cell1,  "LPM_Type",   "LPM_CONSTANT");
		edif_cell_pinteger(cell1, "LPM_Width",  1);
		edif_cell_pinteger(cell1, "LPM_CValue", 1);
	}

	bits = ivl_const_bits(net);
	for (idx = 0 ;  idx < ivl_const_pins(net) ;  idx += 1) {
		if (bits[idx] == '1') {
			if (cell1_ref == 0) {
				cell1_ref = edif_cellref_create(edf, cell1);
				cell1_jnt = edif_joint_create(edf);
				edif_add_to_joint(cell1_jnt, cell1_ref, 0);
			}

		} else {
			if (cell0_ref == 0) {
				cell0_ref = edif_cellref_create(edf, cell0);
				cell0_jnt = edif_joint_create(edf);
				edif_add_to_joint(cell0_jnt, cell0_ref, 0);
			}
		}
	}

	for (idx = 0 ;  idx < ivl_const_pins(net) ;  idx += 1) {
		if (bits[idx] == '1')
			edif_nexus_to_joint(edf, cell1_jnt, ivl_const_pin(net,idx));
		else
			edif_nexus_to_joint(edf, cell0_jnt, ivl_const_pin(net,idx));
	}

}
//adu_begin
static void lpm_show_ramDq(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "ramDq%u_%u_%u",
		ivl_lpm_ramDq_awidth(net), ivl_lpm_ramDq_dwidth(net), ivl_lpm_ramDq_depth(net));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			3 + 2*ivl_lpm_ramDq_awidth(net) + 2*ivl_lpm_ramDq_dwidth(net));

		sprintf(name, "InClock");
		edif_cell_portconfig(cell, 0, strdup(name), IVL_SIP_INPUT);

		sprintf(name, "OutClock");
		edif_cell_portconfig(cell, 1, strdup(name), IVL_SIP_INPUT);

		sprintf(name, "WE");
		edif_cell_portconfig(cell, 2, strdup(name), IVL_SIP_INPUT);

		for (idx = 0 ;  idx < ivl_lpm_ramDq_awidth(net) ;  idx += 1) {
			sprintf(name, "RAddress%u", idx);
			edif_cell_portconfig(cell, 3+idx, strdup(name), IVL_SIP_INPUT);
		}

		for (idx = 0 ;  idx < ivl_lpm_ramDq_awidth(net);  idx += 1) {
			sprintf(name, "WAddress%u", idx);
			edif_cell_portconfig(cell, 3+ivl_lpm_ramDq_awidth(net)+idx, strdup(name), IVL_SIP_INPUT);
		}

		for (idx = 0 ;  idx < ivl_lpm_ramDq_dwidth(net);  idx += 1) {
			sprintf(name, "Data%u", idx);
			edif_cell_portconfig(cell, 3+2*ivl_lpm_ramDq_awidth(net)+idx, strdup(name), IVL_SIP_INPUT);
		}

		for (idx = 0 ;  idx < ivl_lpm_ramDq_dwidth(net);  idx += 1) {
			sprintf(name, "Q%u", idx);
			edif_cell_portconfig(cell, 3+2*ivl_lpm_ramDq_awidth(net)+ivl_lpm_ramDq_dwidth(net)+idx, strdup(name), IVL_SIP_OUTPUT);
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_ramDq");
		edif_cell_pinteger(cell, "LPM_aWidth", ivl_lpm_ramDq_awidth(net));
		edif_cell_pinteger(cell, "LPM_dWidth", ivl_lpm_ramDq_dwidth(net));
		edif_cell_pinteger(cell, "LPM_Depth", ivl_lpm_ramDq_depth(net));
	}

	ref = edif_cellref_create(edf, cell);

	//in_clock
	
	sprintf(name, "InClock");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_inclock(net));
	edif_add_to_joint(jnt, ref, pin);
	//out_clock
	
	sprintf(name, "OutClock");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_outclock(net));
	edif_add_to_joint(jnt, ref, pin);
	//we
	
	sprintf(name, "WE");
	pin = edif_cell_port_byname(cell, name);
	jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_we(net));
	edif_add_to_joint(jnt, ref, pin);

	for (idx = 0 ;  idx < ivl_lpm_ramDq_awidth(net) ;  idx += 1) {
		
		sprintf(name, "RAddress%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_raddress(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_ramDq_awidth(net) ;  idx += 1) {
		
		sprintf(name, "WAddress%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_waddress(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_ramDq_dwidth(net) ;  idx += 1) {
		
		sprintf(name, "Data%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}
	for (idx = 0 ;  idx < ivl_lpm_ramDq_dwidth(net) ;  idx += 1) {
		
		sprintf(name, "Q%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_ramDq_q(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}
//adu_end

//ADU:
static void lpm_show_decoder(ivl_lpm_t net)
{
	char name[64];
	unsigned idx;
	unsigned pin;

	edif_cell_t cell;
	edif_cellref_t ref;
	edif_joint_t jnt;

	sprintf(name, "decoder%u_%u",
		ivl_lpm_decoder_width(net), 1U << ivl_lpm_decoder_width(net));
	cell = edif_xlibrary_findcell(xlib, name);

	if (cell == 0) {
		cell = edif_xcell_create(xlib, strdup(name),
			ivl_lpm_decoder_width(net) + (1U<<ivl_lpm_decoder_width(net)));

		for (idx = 0 ;  idx < ivl_lpm_decoder_width(net) ;  idx += 1) {
			sprintf(name, "Data%u", idx);
			edif_cell_portconfig(cell, idx, strdup(name), IVL_SIP_INPUT);
		}

		for (idx = 0 ;  idx < ivl_lpm_decoder_decodes(net);  idx += 1) {
			sprintf(name, "Eq%u", idx);
			edif_cell_portconfig(cell, ivl_lpm_decoder_width(net)+idx, strdup(name), IVL_SIP_OUTPUT);
		}

		edif_cell_pstring(cell,  "LPM_Type",  "LPM_decode");
		edif_cell_pinteger(cell, "LPM_Width", ivl_lpm_decoder_width(net));
		edif_cell_pinteger(cell, "LPM_Decodes", ivl_lpm_decoder_decodes(net));
	}

	ref = edif_cellref_create(edf, cell);

	for (idx = 0 ;  idx < ivl_lpm_decoder_width(net) ;  idx += 1) {

		sprintf(name, "Data%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_decoder_data(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

	for (idx = 0 ;  idx < ivl_lpm_decoder_decodes(net); idx += 1) {

		sprintf(name, "Eq%u", idx);
		pin = edif_cell_port_byname(cell, name);

		jnt = edif_joint_of_nexus(edf, ivl_lpm_decoder_eq(net, idx));
		edif_add_to_joint(jnt, ref, pin);
	}

}

const struct device_s d_lpm_edif = {
	lpm_show_header,
	lpm_show_footer,
	0,
	0,
	lpm_logic,
	lpm_show_dff, /* show_dff */
	lpm_show_latch,/*show_latch-----ADU*/
	lpm_show_cmp,
	lpm_show_cmp,
	lpm_show_cmp,
	lpm_show_cmp, /* show_cmp_gt */
	lpm_show_cmp,/*show_cmp_le-----ADU*/
	lpm_show_cmp,/*show_cmp_lt-----ADU*/
	lpm_show_mux, /* show_mux */
	lpm_show_add, /* show_add */
	lpm_show_add, /* show_sub */
	0, /* show_shiftl */
	0, /* show_shiftr */
	lpm_show_mult, /* show_mult */
	lpm_show_mip,
	lpm_show_square,
	lpm_show_lmult,
	lpm_show_lmultout,
	lpm_show_mult_add,
	lpm_show_mult_addout,
	lpm_show_mult_accum,
	lpm_show_HalfDSPBlock,
	lpm_show_constant, /* show_constant */
	lpm_show_ramDq,
	lpm_show_decoder
};

/*
* $Log: d-lpm.c,v $
* Revision 1.12.2.1  2005/08/25 18:52:32  steve
*  Join cell0 and cell1 instances in LPM target.
*
* Revision 1.12  2004/10/04 01:10:56  steve
*  Clean up spurious trailing white space.
*
* Revision 1.11  2003/11/12 03:20:14  steve
*  devices need show_cmp_gt
*
* Revision 1.10  2003/10/31 03:45:50  steve
*  Handle adders that use Cout for the top bit.
*
* Revision 1.9  2003/10/27 02:18:27  steve
*  Emit constants for LPM device.
*
* Revision 1.8  2003/09/03 23:34:09  steve
*  Support synchronous set of LPM_FF devices.
*
* Revision 1.7  2003/08/26 04:45:47  steve
*  iverilog-vpi support --cflags a la gtk.
*
* Revision 1.6  2003/08/15 02:23:53  steve
*  Add synthesis support for synchronous reset.
*
* Revision 1.5  2003/08/10 16:42:23  steve
*  Add async clear to LPM_FF devices.
*
* Revision 1.4  2003/08/09 03:23:03  steve
*  Add support for IVL_LPM_MULT device.
*
* Revision 1.3  2003/08/09 02:40:50  steve
*  Generate LPM_FF devices.
*
* Revision 1.2  2003/08/07 05:18:04  steve
*  Add support for OR/NOR/bufif0/bufif1.
*
* Revision 1.1  2003/08/07 04:04:01  steve
*  Add an LPM device type.
*
*/

