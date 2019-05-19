/*
 * Copyright (c) 2000 Stephen Williams (steve@icarus.com)
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
#ident "$Id: t-dll-api.cc,v 1.108.2.5 2006/04/16 19:26:40 steve Exp $"
#endif

# include "config.h"
# include  "t-dll.h"
# include  <stdlib.h>
# include  <string.h>
#ifdef HAVE_MALLOC_H
# include  <malloc.h>
#endif

#include <stdexcept>
using std::invalid_argument;

/* THE FOLLOWING ARE FUNCTIONS THAT ARE CALLED FROM THE TARGET. */

extern "C" const char*ivl_design_flag(ivl_design_t des, const char*key)
{
      return des->self->get_flag(key);
}

extern "C" int ivl_design_process(ivl_design_t des,
				  ivl_process_f func,
				  void*cd)
{
      for (ivl_process_t idx = des->threads_;  idx;  idx = idx->next_) {
	    int rc = (func)(idx, cd);
	    if (rc != 0)
		  return rc;
      }

      return 0;
}

extern "C" ivl_scope_t ivl_design_root(ivl_design_t des)
{
      assert (des->nroots_);
      return des->roots_[0];
}

extern "C" void ivl_design_roots(ivl_design_t des, ivl_scope_t **scopes,
				 unsigned int *nscopes)
{
      assert (nscopes && scopes);
      *scopes = &des->roots_[0];
      *nscopes = des->nroots_;
}

extern "C" int ivl_design_time_precision(ivl_design_t des)
{
      return des->time_precision;
}

extern "C" unsigned ivl_design_consts(ivl_design_t des)
{
      return des->nconsts;
}

extern "C" ivl_net_const_t ivl_design_const(ivl_design_t des, unsigned idx)
{
      assert(idx < des->nconsts);
      return des->consts[idx];
}

extern "C" ivl_expr_type_t ivl_expr_type(ivl_expr_t net)
{
      if (net == 0)
	    return IVL_EX_NONE;
      return net->type_;
}


inline static const char *basename(ivl_scope_t scope, const char *inst)
{
      inst += strlen(ivl_scope_name(scope));
      assert(*inst == '.');
      return inst+1;
}

extern "C" const char* ivl_memory_basename(ivl_memory_t net)
{
      return net->basename_;
}

extern "C" ivl_scope_t ivl_memory_scope(ivl_memory_t net)
{
      assert(net);
      return net->scope_;
}

extern "C" int ivl_memory_root(ivl_memory_t net)
{
      return net->root_;
}

extern "C" unsigned ivl_memory_size(ivl_memory_t net)
{
      return net->size_;
}

extern "C" unsigned ivl_memory_width(ivl_memory_t net)
{
      return net->width_;
}


extern "C" const char*ivl_const_bits(ivl_net_const_t net)
{
      assert(net);
      if (net->width_ <= sizeof(char*))
	    return net->b.bit_;
      else
	    return net->b.bits_;
}

extern "C" ivl_nexus_t ivl_const_pin(ivl_net_const_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->width_);
      if (net->width_ == 1)
	    return net->n.pin_;
      else
	    return net->n.pins_[idx];
}

extern "C" unsigned ivl_const_pins(ivl_net_const_t net)
{
      assert(net);
      return net->width_;
}

extern "C" int ivl_const_signed(ivl_net_const_t net)
{
      assert(net);
      return net->signed_;
}

extern "C" const char* ivl_event_name(ivl_event_t net)
{
      static char*name_buffer = 0;
      static unsigned name_size = 0;

      ivl_scope_t scope = net->scope;
      const char*sn = ivl_scope_name(scope);

      unsigned need = strlen(sn) + 1 + strlen(net->name) + 1;
      if (need > name_size) {
	    name_buffer = (char*)realloc(name_buffer, need);
	    name_size = need;
      }

      strcpy(name_buffer, sn);
      char*tmp = name_buffer + strlen(sn);
      *tmp++ = '.';
      strcpy(tmp, net->name);

      cerr << "ANACHRONISM: Call to anachronistic ivl_event_name." << endl;

      return name_buffer;
}

extern "C" const char* ivl_event_basename(ivl_event_t net)
{
      return net->name;
}


extern "C" unsigned ivl_event_nany(ivl_event_t net)
{
      assert(net);
      return net->nany;
}

extern "C" ivl_nexus_t ivl_event_any(ivl_event_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nany);
      return net->pins[idx];
}

extern "C" unsigned ivl_event_nneg(ivl_event_t net)
{
      assert(net);
      return net->nneg;
}

extern "C" ivl_nexus_t ivl_event_neg(ivl_event_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nneg);
      return net->pins[net->nany + idx];
}

extern "C" unsigned ivl_event_npos(ivl_event_t net)
{
      assert(net);
      return net->npos;
}

extern "C" ivl_nexus_t ivl_event_pos(ivl_event_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->npos);
      return net->pins[net->nany + net->nneg + idx];
}

extern "C" const char* ivl_expr_bits(ivl_expr_t net)
{
      assert(net && (net->type_ == IVL_EX_NUMBER));
      return net->u_.number_.bits_;
}

extern "C" ivl_scope_t ivl_expr_def(ivl_expr_t net)
{
      assert(net);

      switch (net->type_) {

	  case IVL_EX_UFUNC:
	    return net->u_.ufunc_.def;

	  default:
	    assert(0);
      }

      return 0;
}

extern "C" double ivl_expr_dvalue(ivl_expr_t net)
{
      assert(net->type_ == IVL_EX_REALNUM);
      return net->u_.real_.value;
}

extern "C" unsigned ivl_expr_lsi(ivl_expr_t net)
{
      switch (net->type_) {

	  case IVL_EX_SIGNAL:
	    return net->u_.signal_.lsi;

	  default:
	    assert(0);

      }
      return 0;
}

extern "C" const char* ivl_expr_name(ivl_expr_t net)
{
      switch (net->type_) {

	  case IVL_EX_SFUNC:
	    return net->u_.sfunc_.name_;

	  case IVL_EX_SIGNAL:
	    return net->u_.signal_.sig->name_;

	  case IVL_EX_VARIABLE:
	    return net->u_.variable_.var->name;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" char ivl_expr_opcode(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {
	  case IVL_EX_BINARY:
	    return net->u_.binary_.op_;

	  case IVL_EX_UNARY:
	    return net->u_.unary_.op_;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" ivl_expr_t ivl_expr_oper1(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {
	  case IVL_EX_BINARY:
	  case IVL_EX_SELECT:
	    return net->u_.binary_.lef_;

	  case IVL_EX_BITSEL:
	    return net->u_.bitsel_.bit;

	  case IVL_EX_UNARY:
	    return net->u_.unary_.sub_;

	  case IVL_EX_MEMORY:
	    return net->u_.memory_.idx_;

	  case IVL_EX_TERNARY:
	    return net->u_.ternary_.cond;

	  default:
	    assert(0);
      }

      return 0;
}

extern "C" ivl_expr_t ivl_expr_oper2(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {
	  case IVL_EX_BINARY:
	  case IVL_EX_SELECT:
	    return net->u_.binary_.rig_;

	  case IVL_EX_TERNARY:
	    return net->u_.ternary_.true_e;

	  default:
	    assert(0);
      }

      return 0;
}

extern "C" ivl_expr_t ivl_expr_oper3(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {

	  case IVL_EX_TERNARY:
	    return net->u_.ternary_.false_e;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" ivl_parameter_t ivl_expr_parameter(ivl_expr_t net)
{
      switch (net->type_) {
	  case IVL_EX_NUMBER:
	    return net->u_.number_.parameter;
	  case IVL_EX_STRING:
	    return net->u_.string_.parameter;
	  case IVL_EX_REALNUM:
	    return net->u_.real_.parameter;
	  default:
	    return 0;
      }
}

extern "C" ivl_expr_t ivl_expr_parm(ivl_expr_t net, unsigned idx)
{
      assert(net);
      switch (net->type_) {

	  case IVL_EX_CONCAT:
	    assert(idx < net->u_.concat_.parms);
	    return net->u_.concat_.parm[idx];

	  case IVL_EX_SFUNC:
	    assert(idx < net->u_.sfunc_.parms);
	    return net->u_.sfunc_.parm[idx];

	  case IVL_EX_UFUNC:
	    assert(idx < net->u_.ufunc_.parms);
	    return net->u_.ufunc_.parm[idx];

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_expr_parms(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {

	  case IVL_EX_CONCAT:
	    return net->u_.concat_.parms;

	  case IVL_EX_SFUNC:
	    return net->u_.sfunc_.parms;

	  case IVL_EX_UFUNC:
	    return net->u_.ufunc_.parms;

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_expr_repeat(ivl_expr_t net)
{
      assert(net);
      assert(net->type_ == IVL_EX_CONCAT);
      return net->u_.concat_.rept;
}

extern "C" ivl_event_t ivl_expr_event(ivl_expr_t net)
{
      assert(net);
      assert(net->type_ == IVL_EX_EVENT);
      return net->u_.event_.event;
}

extern "C" ivl_scope_t ivl_expr_scope(ivl_expr_t net)
{
      assert(net);
      assert(net->type_ == IVL_EX_SCOPE);
      return net->u_.scope_.scope;
}

extern "C" ivl_signal_t ivl_expr_signal(ivl_expr_t net)
{
      assert(net);
      switch (net->type_) {
	  case IVL_EX_BITSEL:
	    return net->u_.bitsel_.sig;

	  case IVL_EX_SIGNAL:
	    return net->u_.signal_.sig;

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" int ivl_expr_signed(ivl_expr_t net)
{
      assert(net);
      return net->signed_;
}

extern "C" const char* ivl_expr_string(ivl_expr_t net)
{
      assert(net->type_ == IVL_EX_STRING);
      return net->u_.string_.value_;
}

extern "C" unsigned long ivl_expr_uvalue(ivl_expr_t net)
{
      switch (net->type_) {

	  case IVL_EX_ULONG:
	    return net->u_.ulong_.value;

	  case IVL_EX_NUMBER: {
		unsigned long val = 0;
		for (unsigned long idx = 0 ;  idx < net->width_ ;  idx += 1) {
		      if (net->u_.number_.bits_[idx] == '1')
			    val |= 1UL << idx;
		}

		return val;
	  }

	  default:
	    assert(0);
	    return 0UL;
      }

}

extern "C" ivl_variable_type_t ivl_expr_value(ivl_expr_t net)
{
      assert(net);
      return net->value_;
}

extern "C" unsigned ivl_expr_width(ivl_expr_t net)
{
      assert(net);
      return net->width_;
}

extern "C" ivl_memory_t ivl_expr_memory(ivl_expr_t net)
{
      assert(net->type_ == IVL_EX_MEMORY);
      return net->u_.memory_.mem_;
}

extern "C" ivl_variable_t ivl_expr_variable(ivl_expr_t net)
{
      assert(net->type_ == IVL_EX_VARIABLE);
      return net->u_.variable_.var;
}

extern "C" const char* ivl_logic_attr(ivl_net_logic_t net, const char*key)
{
      assert(net);
      unsigned idx;

      for (idx = 0 ;  idx < net->nattr ;  idx += 1) {

	    if (strcmp(net->attr[idx].key, key) == 0)
		  return net->attr[idx].type == IVL_ATT_STR
			? net->attr[idx].val.str
			: 0;
      }

      return 0;
}

//adu
extern "C" const char* ivl_lpm_attr_string(ivl_lpm_t net, const char*key)
{
	assert(net);
	unsigned idx;

	for (idx = 0 ;  idx < net->nattr ;  idx += 1) {

		if (strcmp(net->attr[idx].key, key) == 0){
			assert(net->attr[idx].type == IVL_ATT_STR);
			return net->attr[idx].val.str;
		}
	}

	return 0;
}

extern const unsigned ivl_lpm_attr_integer(ivl_lpm_t net, const char*key)
{
	assert(net);
	unsigned idx;

	for (idx = 0 ;  idx < net->nattr ;  idx += 1) {

		if (strcmp(net->attr[idx].key, key) == 0){
			assert(net->attr[idx].type == IVL_ATT_NUM);
			return net->attr[idx].val.num;
		}			
	}

	return 0;
}

extern "C" unsigned ivl_logic_attr_cnt(ivl_net_logic_t net)
{
      return net->nattr;
}

extern "C" ivl_attribute_t ivl_logic_attr_val(ivl_net_logic_t net,
					      unsigned idx)
{
      assert(idx < net->nattr);
      return net->attr + idx;
}

extern "C" const char* ivl_logic_name(ivl_net_logic_t net)
{
      assert(net);
      cerr << "ANACHRONISM: Call to anachronistic ivl_logic_name." << endl;
      return net->name_;
}

extern "C" const char* ivl_logic_basename(ivl_net_logic_t net)
{
      assert(net);
      return net->name_;
}

extern "C" ivl_scope_t ivl_logic_scope(ivl_net_logic_t net)
{
      assert(net);
      return net->scope_;
}

extern "C" ivl_logic_t ivl_logic_type(ivl_net_logic_t net)
{
      return net->type_;
}

extern "C" unsigned ivl_logic_pins(ivl_net_logic_t net)
{
      return net->npins_;
}

extern "C" ivl_nexus_t ivl_logic_pin(ivl_net_logic_t net, unsigned pin)
{
      assert(pin < net->npins_);
      return net->pins_[pin];
}

extern "C" ivl_udp_t ivl_logic_udp(ivl_net_logic_t net)
{
      assert(net->type_ == IVL_LO_UDP);
      assert(net->udp);
      return net->udp;
}

extern "C" unsigned  ivl_logic_delay(ivl_net_logic_t net, unsigned transition)
{
      assert(transition < 3);
      return net->delay[transition];
}


extern "C" unsigned    ivl_udp_sequ(ivl_udp_t net)
{
      return net->sequ;
}

extern "C" unsigned    ivl_udp_nin(ivl_udp_t net)
{
      return net->nin;
}

extern "C" unsigned    ivl_udp_init(ivl_udp_t net)
{
      return net->init;
}

extern "C" const char* ivl_udp_row(ivl_udp_t net, unsigned idx)
{
      assert(idx < net->nrows);
      assert(net->table);
      assert(net->table[idx]);
      return net->table[idx];
}

extern "C" unsigned    ivl_udp_rows(ivl_udp_t net)
{
      return net->nrows;
}

extern "C" const char* ivl_udp_name(ivl_udp_t net)
{
      assert(net->name);
      return net->name;
}

extern "C" unsigned ivl_lpm_attr_cnt(ivl_lpm_t net)
{
      return net->nattr;
}

extern "C" ivl_attribute_t ivl_lpm_attr_val(ivl_lpm_t net, unsigned idx)
{
      if (idx >= net->nattr)
	    return 0;
      else
	    return net->attr + idx;
}

extern "C" const char* ivl_lpm_basename(ivl_lpm_t net)
{
      return net->name;
}

extern "C" ivl_nexus_t ivl_lpm_async_clr(ivl_lpm_t net)
{
      assert(net);
      switch(net->type) {
	  case IVL_LPM_FF:
	    return net->u_.ff.aclr;
	  case IVL_LPM_LATCH:
		  return net->u_.latch.AclrPtr;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_sync_clr(ivl_lpm_t net)
{
      assert(net);
      switch(net->type) {
	  case IVL_LPM_FF:
	    return net->u_.ff.sclr;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_async_set(ivl_lpm_t net)
{
      assert(net);
      switch(net->type) {
	  case IVL_LPM_FF:
	    return net->u_.ff.aset;
	  case IVL_LPM_LATCH:
		  return net->u_.latch.AsetPtr;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_sync_set(ivl_lpm_t net)
{
      assert(net);
      switch(net->type) {
	  case IVL_LPM_FF:
	    return net->u_.ff.sset;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_clk(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    return net->u_.ff.clk;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_gate( ivl_lpm_t netPtr )
{
  assert( netPtr );

  switch ( netPtr->type )
    {
    case IVL_LPM_LATCH:
      return netPtr->u_.latch.gatePtr;
    default:
      assert( false );
      return 0;
    }

}

extern "C" ivl_expr_t ivl_lpm_aset_value(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    return net->u_.ff.aset_value;
	  default:
	    assert(0);
	    return 0;
      }
}
extern "C" ivl_expr_t ivl_lpm_sset_value(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    return net->u_.ff.sset_value;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_scope_t ivl_lpm_define(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_UFUNC:
	    return net->u_.ufunc.def;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_enable(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_RAM:
	  case IVL_LPM_FF:
	    return net->u_.ff.we;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_data(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_ADD:
	  case IVL_LPM_CMP_EQ:
	  case IVL_LPM_CMP_GE:
	  case IVL_LPM_CMP_GT:
	  case IVL_LPM_CMP_LE:/*ADU*/
	  case IVL_LPM_CMP_LT:/*ADU*/
	  case IVL_LPM_CMP_NE:
	  case IVL_LPM_DIVIDE:
	  case IVL_LPM_MOD:
	  case IVL_LPM_SUB:
	    assert(idx < net->u_.arith.width);
	    return net->u_.arith.a[idx];

	  case IVL_LPM_DEMUX:
	    assert(idx < net->u_.demux.width);
	    return net->u_.demux.d[idx];

	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    assert(idx < net->u_.shift.width);
	    return net->u_.shift.d[idx];

	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    assert(idx < net->u_.ff.width);
	    if (net->u_.ff.width == 1)
		  return net->u_.ff.d.pin;
	    else
		  return net->u_.ff.d.pins[idx];

          case IVL_LPM_LATCH:
/*-----ADU
	    if( idx >= net->u_.latch.width )
	      {
		throw invalid_argument( "idx too high" );
	      }

	    if ( net->u_.latch.width != 1U )
	      {
		throw invalid_argument( "Only 1-wide latches are currently supported." );
	      }
*/
		/*start----ADU*/
		assert(idx < net->u_.latch.width);
	    if (net->u_.latch.width == 1)
		  return net->u_.latch.dataPtr.pin;
	    else
		  return net->u_.latch.dataPtr.pins[idx];

	    //return net->u_.latch.dataPtr;
		/*start----ADU*/

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_datab(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {

	  case IVL_LPM_ADD:
	  case IVL_LPM_CMP_EQ:
	  case IVL_LPM_CMP_GE:
	  case IVL_LPM_CMP_GT:
	  case IVL_LPM_CMP_LE:
	  case IVL_LPM_CMP_LT:
	  case IVL_LPM_CMP_NE:
	  case IVL_LPM_DIVIDE:
	  case IVL_LPM_MOD:
	  case IVL_LPM_SUB:
	    assert(idx < net->u_.arith.width);
	    return net->u_.arith.b[idx];

	  case IVL_LPM_DEMUX:
	    assert(idx < net->u_.demux.width/net->u_.demux.size);
	    return net->u_.demux.bit_in[idx];

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_data2(ivl_lpm_t net, unsigned sdx, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MUX:
	    assert(sdx < net->u_.mux.size);
	    assert(idx < net->u_.mux.width);
	    return net->u_.mux.d[sdx*net->u_.mux.width + idx];

	  case IVL_LPM_UFUNC: {
		sdx += 1; /* skip the output port. */
		assert(sdx < net->u_.ufunc.ports);
		assert(idx < net->u_.ufunc.port_wid[sdx]);
		unsigned base = 0;
		for (unsigned i = 0 ;  i < sdx ;  i += 1)
		      base += net->u_.ufunc.port_wid[i];
		return net->u_.ufunc.pins[base+idx];
	  }

	  case IVL_LPM_RAM:
	    if (net->u_.ff.a.mem == 0) {
		    // This is an exploded RAM, so we use sdx and idx
		    // to address a nexa into the exploded ram.
		  if (sdx >= net->u_.ff.scnt)
			return 0;
		  if (idx >= net->u_.ff.width)
			return 0;
		  unsigned adr = sdx * net->u_.ff.width + idx;
		  return net->u_.ff.d.pins[adr];

	    } else {
		    // Normal RAM port does not have data2 nexa
		  return 0;
	    }

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_lpm_data2_width(ivl_lpm_t net, unsigned sdx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_UFUNC:
	    sdx += 1; /* skip the output port. */
	    assert(sdx < net->u_.ufunc.ports);
	    return net->u_.ufunc.port_wid[sdx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_lpm_t ivl_lpm_decode(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	    return net->u_.ff.a.decode;
	  default:
	    assert(0);
	    return 0;
      }
}

/*
 * This function returns the hierarchical name for the LPM device. The
 * name needs to be built up from the scope name and the lpm base
 * name.
 *
 * Anachronism: This function is provided for
 * compatibility. Eventually, it will be removed.
 */
extern "C" const char* ivl_lpm_name(ivl_lpm_t net)
{
      static char*name_buffer = 0;
      static unsigned name_size = 0;

      ivl_scope_t scope = ivl_lpm_scope(net);
      const char*sn = ivl_scope_name(scope);

      unsigned need = strlen(sn) + 1 + strlen(net->name) + 1;
      if (need > name_size) {
	    name_buffer = (char*)realloc(name_buffer, need);
	    name_size = need;
      }

      strcpy(name_buffer, sn);
      char*tmp = name_buffer + strlen(sn);
      *tmp++ = '.';
      strcpy(tmp, net->name);
      return name_buffer;
}


extern "C" ivl_nexus_t ivl_lpm_q(ivl_lpm_t net, unsigned idx)
{
      assert(net);

      switch (net->type) {
	  case IVL_LPM_ADD:
	  case IVL_LPM_DIVIDE:
	  case IVL_LPM_MOD:
	  case IVL_LPM_SUB:
	    assert(idx < net->u_.arith.width);
	    return net->u_.arith.q[idx];

	  case IVL_LPM_CMP_GE:
	  case IVL_LPM_CMP_GT:
	  case IVL_LPM_CMP_EQ:
	  case IVL_LPM_CMP_NE:
	  case IVL_LPM_CMP_LE:/*ADU*/
	  case IVL_LPM_CMP_LT:/*ADU*/
	    assert(idx == 0);
	    return net->u_.arith.q[0];

	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    assert(idx < net->u_.ff.width);
	    if (net->u_.ff.width == 1)
		  return net->u_.ff.q.pin;
	    else
		  return net->u_.ff.q.pins[idx];

          case IVL_LPM_LATCH:
/*---ADU
	    if ( idx >= net->u_.latch.width )
	      {
		throw invalid_argument( "idx too high" );
	      }

	    if ( net->u_.latch.width != 1U )
	      {
		throw invalid_argument( "Only 1-wide latches are currently supported." );
	      }
*/
		/*start-----ADU*/
		assert(idx < net->u_.latch.width);
	    if (net->u_.latch.width == 1)
		  return net->u_.latch.qPtr.pin;
	    else
		  return net->u_.latch.qPtr.pins[idx];

	    //return net->u_.latch.qPtr;
		/*end-----ADU*/

	  case IVL_LPM_MUX:
	    assert(idx < net->u_.mux.width);
	    if (net->u_.mux.width == 1)
		  return net->u_.mux.q.pin;
	    else
		  return net->u_.mux.q.pins[idx];

	  case IVL_LPM_DEMUX:
	    assert(idx < net->u_.demux.width);
	    return net->u_.demux.q[idx];

	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    assert(idx < net->u_.shift.width);
	    return net->u_.shift.q[idx];

	  case IVL_LPM_UFUNC:
	    assert(idx < net->u_.ufunc.port_wid[0]);
	    return net->u_.ufunc.pins[idx];

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_sbit(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
		if (idx == 0) {
		  assert(net->u_.mip.clocken0);
		  return net->u_.mip.clocken0;
		}
		else if (idx == 1) {
		  assert(net->u_.mip.clocken1);
		  return net->u_.mip.clocken1;
		}
		else if (idx == 2) {
		  assert(net->u_.mip.wren_a);
		  return net->u_.mip.wren_a;
		}
		else if (idx == 3) {
		  assert(net->u_.mip.aclr0);
		  return net->u_.mip.aclr0;
		}
		else if (idx == 4) {
		  assert(net->u_.mip.clock0);
		  return net->u_.mip.clock0;
		}
		else if (idx == 5) {
		  assert(net->u_.mip.wren_b);
		  return net->u_.mip.wren_b;
		}
		else if (idx == 6) {
		  assert(net->u_.mip.aclr1);
		  return net->u_.mip.aclr1;
		}
		else if (idx == 7) {
		  assert(net->u_.mip.clock1);
		  return net->u_.mip.clock1;
		}
		else {
		  assert(0);
		  return 0;
		}
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_address_a(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_address_a);
		return net->u_.mip.address_a[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_address_b(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_address_b);
		return net->u_.mip.address_b[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_data_a(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_data_a);
		return net->u_.mip.data_a[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_data_b(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_data_b);
		return net->u_.mip.data_b[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_q_a(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_q_a);
		return net->u_.mip.q_a[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mip_q_b(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
	    assert(idx < net->u_.mip.width_q_b);
		return net->u_.mip.q_b[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_sbit(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_SQUARE:
	  case IVL_LPM_LMULT:
		if (idx == 0) {
		  assert(net->u_.mult.clken);
		  return net->u_.mult.clken;
		}
		else if (idx == 1) {
		  assert(net->u_.mult.aclr);
		  return net->u_.mult.aclr;
		}
		else if (idx == 2) {
		  assert(net->u_.mult.clock);
		  return net->u_.mult.clock;
		}
		else {
		  assert(0);
		  return 0;
		}
	  case IVL_LPM_MULT_ADD:
		if (idx == 0) {
		  assert(net->u_.mult.clock0);
		  return net->u_.mult.clock0;
		}
		else if (idx == 1) {
		  assert(net->u_.mult.clock1);
		  return net->u_.mult.clock1;
		}
		else if (idx == 2) {
		  assert(net->u_.mult.clock2);
		  return net->u_.mult.clock2;
		}
		else if (idx == 3) {
		  assert(net->u_.mult.clock3);
		  return net->u_.mult.clock3;
		}
		else if (idx == 4) {
		  assert(net->u_.mult.aclr0);
		  return net->u_.mult.aclr0;
		}
		else if (idx == 5) {
		  assert(net->u_.mult.aclr1);
		  return net->u_.mult.aclr1;
		}
		else if (idx == 6) {
		  assert(net->u_.mult.aclr2);
		  return net->u_.mult.aclr2;
		}
		else if (idx == 7) {
		  assert(net->u_.mult.aclr3);
		  return net->u_.mult.aclr3;
		}
		else if (idx == 8) {
		  assert(net->u_.mult.ena0);
		  return net->u_.mult.ena0;
		}
		else if (idx == 9) {
		  assert(net->u_.mult.ena1);
		  return net->u_.mult.ena1;
		}
		else if (idx == 10) {
		  assert(net->u_.mult.ena2);
		  return net->u_.mult.ena2;
		}
		else if (idx == 11) {
		  assert(net->u_.mult.ena3);
		  return net->u_.mult.ena3;
		}
		else {
		  assert(0);
		  return 0;
		}
	  case IVL_LPM_MULT_ACCUM:
		if (idx == 0) {
		  assert(net->u_.mult.accum_sload);
		  return net->u_.mult.accum_sload;
		}
		else if (idx == 1) {
		  assert(net->u_.mult.clock0);
		  return net->u_.mult.clock0;
		}
		else if (idx == 2) {
		  assert(net->u_.mult.clock1);
		  return net->u_.mult.clock1;
		}
		else if (idx == 3) {
		  assert(net->u_.mult.clock2);
		  return net->u_.mult.clock2;
		}
		else if (idx == 4) {
		  assert(net->u_.mult.clock3);
		  return net->u_.mult.clock3;
		}
		else if (idx == 5) {
		  assert(net->u_.mult.aclr0);
		  return net->u_.mult.aclr0;
		}
		else if (idx == 6) {
		  assert(net->u_.mult.aclr1);
		  return net->u_.mult.aclr1;
		}
		else if (idx == 7) {
		  assert(net->u_.mult.aclr2);
		  return net->u_.mult.aclr2;
		}
		else if (idx == 8) {
		  assert(net->u_.mult.aclr3);
		  return net->u_.mult.aclr3;
		}
		else if (idx == 9) {
		  assert(net->u_.mult.ena0);
		  return net->u_.mult.ena0;
		}
		else if (idx == 10) {
		  assert(net->u_.mult.ena1);
		  return net->u_.mult.ena1;
		}
		else if (idx == 11) {
		  assert(net->u_.mult.ena2);
		  return net->u_.mult.ena2;
		}
		else if (idx == 12) {
		  assert(net->u_.mult.ena3);
		  return net->u_.mult.ena3;
		}
		else if (idx == 13) {
		  assert(net->u_.mult.overflow);
		  return net->u_.mult.overflow;
		}
		else {
		  assert(0);
		  return 0;
		}
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_data(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_SQUARE:
	    assert(idx < net->u_.mult.width_data);
		return net->u_.mult.data[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_dataa(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MULT:
	  case IVL_LPM_LMULT:
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
	    assert(idx < net->u_.mult.width_dataa);
		return net->u_.mult.dataa[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_datab(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MULT:
	  case IVL_LPM_LMULT:
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
	    assert(idx < net->u_.mult.width_datab);
		return net->u_.mult.datab[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_sum(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_LMULT:
	    assert(idx < net->u_.mult.width_sum);
		return net->u_.mult.sum[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_scanouta(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
	    assert(idx < net->u_.mult.width_scanouta);
		return net->u_.mult.scanouta[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_scanoutb(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
	    assert(idx < net->u_.mult.width_scanoutb);
		return net->u_.mult.scanoutb[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_nexus_t ivl_lpm_mult_result(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_SQUARE:
	  case IVL_LPM_MULT:
	  case IVL_LPM_LMULT:
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
	    assert(idx < net->u_.mult.width_result);
		return net->u_.mult.result[idx];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_scope_t ivl_lpm_scope(ivl_lpm_t net)
{
      assert(net);
      return net->scope;
}

extern "C" ivl_nexus_t ivl_lpm_select(ivl_lpm_t net, unsigned idx)
{
      switch (net->type) {
	  case IVL_LPM_RAM:
	    assert(idx < net->u_.ff.swid);
	    if (net->u_.ff.swid == 1)
		  return net->u_.ff.s.pin;
	    else
		  return net->u_.ff.s.pins[idx];

	  case IVL_LPM_DECODE:
	  case IVL_LPM_MUX:
	    assert(idx < net->u_.mux.swid);
	    if (net->u_.mux.swid == 1)
		  return net->u_.mux.s.pin;
	    else
		  return net->u_.mux.s.pins[idx];

	  case IVL_LPM_DEMUX:
	    assert(idx < net->u_.demux.awid);
	    return net->u_.demux.a[idx];

	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    assert(idx < net->u_.shift.select);
	    return net->u_.shift.s[idx];

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_lpm_selects(ivl_lpm_t net)
{
      switch (net->type) {
	  case IVL_LPM_RAM:
	    return net->u_.ff.swid;
	  case IVL_LPM_DECODE:
	  case IVL_LPM_MUX:
	    return net->u_.mux.swid;
	  case IVL_LPM_DEMUX:
	    return net->u_.demux.awid;
	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    return net->u_.shift.select;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" int ivl_lpm_signed(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	  case IVL_LPM_MUX:
	  case IVL_LPM_DEMUX:
	    return 0;
	  case IVL_LPM_ADD:
	  case IVL_LPM_CMP_EQ:
	  case IVL_LPM_CMP_GE:
	  case IVL_LPM_CMP_GT:
	  case IVL_LPM_CMP_NE:
	  case IVL_LPM_DIVIDE:
	  case IVL_LPM_MOD:
	  case IVL_LPM_SUB:
	    return net->u_.arith.signed_flag;
	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    return net->u_.shift.signed_flag;
	  case IVL_LPM_DECODE:
	  case IVL_LPM_UFUNC:
	    return 0;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_lpm_size(ivl_lpm_t net)
{
      switch (net->type) {
	  case IVL_LPM_DEMUX:
	    return net->u_.demux.size;
	  case IVL_LPM_MUX:
	    return net->u_.mux.size;
	  case IVL_LPM_RAM:
	    return net->u_.ff.scnt;
	  case IVL_LPM_UFUNC:
	    return net->u_.ufunc.ports - 1;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_lpm_type_t ivl_lpm_type(ivl_lpm_t net)
{
      return net->type;
}

extern "C" unsigned ivl_lpm_width(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_FF:
	  case IVL_LPM_RAM:
	    return net->u_.ff.width;
          case IVL_LPM_LATCH:
	    return net->u_.latch.width;
	  case IVL_LPM_DECODE:
	  case IVL_LPM_MUX:
	    return net->u_.mux.width;
	  case IVL_LPM_DEMUX:
	    return net->u_.demux.width;
	  case IVL_LPM_ADD:
	  case IVL_LPM_CMP_EQ:
	  case IVL_LPM_CMP_GE:
	  case IVL_LPM_CMP_GT:
	  case IVL_LPM_CMP_NE:
	  case IVL_LPM_CMP_LE:/*ADU*/
	  case IVL_LPM_CMP_LT:/*ADU*/
	  case IVL_LPM_DIVIDE:
	  case IVL_LPM_MOD:
	  case IVL_LPM_SUB:
	    return net->u_.arith.width;
	  case IVL_LPM_SHIFTL:
	  case IVL_LPM_SHIFTR:
	    return net->u_.shift.width;
	  case IVL_LPM_UFUNC:
	    return net->u_.ufunc.port_wid[0];
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_lpm_mip_width(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_MIP:
		if (idx == 0) return net->u_.mip.width_address_a;
		else if (idx == 1) return net->u_.mip.width_address_b;
		else if (idx == 2) return net->u_.mip.width_data_a;
		else if (idx == 3) return net->u_.mip.width_data_b;
		else if (idx == 4) return net->u_.mip.width_q_a;
		else if (idx == 5) return net->u_.mip.width_q_b;
		else {
		  assert(0);
		  return 0;
		};
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_lpm_mult_width(ivl_lpm_t net, unsigned idx)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_SQUARE:
		if (idx == 0) return net->u_.mult.width_data;
		else if (idx == 1) return net->u_.mult.width_result;
		else {
		  assert(0);
		  return 0;
		};
	  case IVL_LPM_MULT:
	  case IVL_LPM_LMULT:
		if (idx == 0) return net->u_.mult.width_dataa;
		else if (idx == 1) return net->u_.mult.width_datab;
		else if (idx == 2) return net->u_.mult.width_sum;
		else if (idx == 3) return net->u_.mult.width_result;
		else {
		  assert(0);
		  return 0;
		};
	  case IVL_LPM_MULT_ADD:
	  case IVL_LPM_MULT_ACCUM:
		if (idx == 0) return net->u_.mult.width_dataa;
		else if (idx == 1) return net->u_.mult.width_datab;
		else if (idx == 2) return net->u_.mult.width_scanouta;
		else if (idx == 3) return net->u_.mult.width_scanoutb;
		else if (idx == 4) return net->u_.mult.width_result;
		else {
		  assert(0);
		  return 0;
		};
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_memory_t ivl_lpm_memory(ivl_lpm_t net)
{
      assert(net);
      switch (net->type) {
	  case IVL_LPM_RAM:
	    return net->u_.ff.a.mem;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_expr_t ivl_lval_mux(ivl_lval_t net)
{
      assert(net);
      if (net->type_ == IVL_LVAL_MUX)
	    return net->idx;
      return 0x0;
}

extern "C" ivl_expr_t ivl_lval_idx(ivl_lval_t net)
{
      assert(net);
      if (net->type_ == IVL_LVAL_MEM)
	    return net->idx;
      return 0x0;
}

extern "C" ivl_memory_t ivl_lval_mem(ivl_lval_t net)
{
      assert(net);
      if (net->type_ == IVL_LVAL_MEM)
	    return net->n.mem;
      return 0x0;
}

extern "C" ivl_variable_t ivl_lval_var(ivl_lval_t net)
{
      assert(net);
      if (net->type_ == IVL_LVAL_VAR)
	    return net->n.var;
      return 0x0;
}

extern "C" unsigned ivl_lval_part_off(ivl_lval_t net)
{
      assert(net);
      return net->loff_;
}

extern "C" unsigned ivl_lval_pins(ivl_lval_t net)
{
      assert(net);
      return net->width_;
}

extern "C" ivl_nexus_t ivl_lval_pin(ivl_lval_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->width_);
      assert(net->type_ != IVL_LVAL_MEM);
      return ivl_signal_pin(net->n.sig, idx+net->loff_);
}

extern "C" ivl_signal_t ivl_lval_sig(ivl_lval_t net)
{
      assert(net);
      return net->n.sig;
}

extern "C" const char* ivl_nexus_name(ivl_nexus_t net)
{
      assert(net);
      return net->name_;
}

extern "C" void* ivl_nexus_get_private(ivl_nexus_t net)
{
      assert(net);
      return net->private_data;
}

extern "C" void ivl_nexus_set_private(ivl_nexus_t net, void*data)
{
      assert(net);
      net->private_data = data;
}

extern "C" unsigned ivl_nexus_ptrs(ivl_nexus_t net)
{
      assert(net);
      return net->nptr_;
}

extern "C" ivl_nexus_ptr_t ivl_nexus_ptr(ivl_nexus_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nptr_);
      return net->ptrs_ + idx;
}

extern "C" ivl_drive_t ivl_nexus_ptr_drive0(ivl_nexus_ptr_t net)
{
      assert(net);
      return (ivl_drive_t)(net->drive0);
}

extern "C" ivl_drive_t ivl_nexus_ptr_drive1(ivl_nexus_ptr_t net)
{
      assert(net);
      return (ivl_drive_t)(net->drive1);
}

extern "C" unsigned ivl_nexus_ptr_pin(ivl_nexus_ptr_t net)
{
      assert(net);
      return net->pin_;
}

extern "C" ivl_net_const_t ivl_nexus_ptr_con(ivl_nexus_ptr_t net)
{
      if (net == 0)
	    return 0;
      if (net->type_ != __NEXUS_PTR_CON)
	    return 0;
      return net->l.con;
}

extern "C" ivl_net_logic_t ivl_nexus_ptr_log(ivl_nexus_ptr_t net)
{
      if (net == 0)
	    return 0;
      if (net->type_ != __NEXUS_PTR_LOG)
	    return 0;
      return net->l.log;
}

extern "C" ivl_lpm_t ivl_nexus_ptr_lpm(ivl_nexus_ptr_t net)
{
      if (net == 0)
	    return 0;
      if (net->type_ != __NEXUS_PTR_LPM)
	    return 0;
      return net->l.lpm;
}

extern "C" ivl_signal_t ivl_nexus_ptr_sig(ivl_nexus_ptr_t net)
{
      if (net == 0)
	    return 0;
      if (net->type_ != __NEXUS_PTR_SIG)
	    return 0;
      return net->l.sig;
}

extern "C" const char* ivl_parameter_basename(ivl_parameter_t net)
{
      assert(net);
      return net->basename;
}

extern "C" ivl_expr_t ivl_parameter_expr(ivl_parameter_t net)
{
      assert(net);
      return net->value;
}

extern "C" ivl_scope_t ivl_parameter_scope(ivl_parameter_t net)
{
      assert(net);
      return net->scope;
}

extern "C" ivl_process_type_t ivl_process_type(ivl_process_t net)
{
      return net->type_;
}

extern "C" ivl_scope_t ivl_process_scope(ivl_process_t net)
{
      return net->scope_;
}

extern "C" ivl_statement_t ivl_process_stmt(ivl_process_t net)
{
      return net->stmt_;
}

extern "C" unsigned ivl_process_attr_cnt(ivl_process_t net)
{
      return net->nattr;
}

extern "C" ivl_attribute_t ivl_process_attr_val(ivl_process_t net,
						unsigned idx)
{
      assert(idx < net->nattr);
      return net->attr + idx;
}

extern "C" unsigned ivl_scope_attr_cnt(ivl_scope_t net)
{
      assert(net);
      return net->nattr;
}

extern "C" ivl_attribute_t ivl_scope_attr_val(ivl_scope_t net,
					      unsigned idx)
{
      assert(idx < net->nattr);
      return net->attr + idx;
}

extern "C" const char* ivl_scope_basename(ivl_scope_t net)
{
      assert(net);

      return net->name_;
}

extern "C" int ivl_scope_children(ivl_scope_t net,
				  ivl_scope_f func,
				  void*cd)
{
      for (ivl_scope_t cur = net->child_; cur;  cur = cur->sibling_) {
	    int rc = func(cur, cd);
	    if (rc != 0)
		  return rc;
      }

      return 0;
}

extern "C" ivl_statement_t ivl_scope_def(ivl_scope_t net)
{
      assert(net);
      return net->def;
}

extern "C" unsigned ivl_scope_events(ivl_scope_t net)
{
      assert(net);
      return net->nevent_;
}

extern "C" ivl_event_t ivl_scope_event(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nevent_);
      return net->event_[idx];
}

extern "C" unsigned ivl_scope_vars(ivl_scope_t net)
{
      assert(net);
      return net->nvar_;
}

extern "C" ivl_variable_t ivl_scope_var(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nvar_);
      return net->var_[idx];
}

extern "C" unsigned ivl_scope_logs(ivl_scope_t net)
{
      assert(net);
      return net->nlog_;
}

extern "C" ivl_net_logic_t ivl_scope_log(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nlog_);
      return net->log_[idx];
}

extern "C" unsigned ivl_scope_lpms(ivl_scope_t net)
{
      assert(net);
      return net->nlpm_;
}

extern "C" ivl_lpm_t ivl_scope_lpm(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nlpm_);
      return net->lpm_[idx];
}

extern "C" unsigned ivl_scope_mems(ivl_scope_t net)
{
      assert(net);
      return net->nmem_;
}

extern "C" ivl_memory_t ivl_scope_mem(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nmem_);
      return net->mem_[idx];
}

static unsigned scope_name_len(ivl_scope_t net)
{
      unsigned len = 0;

      for (ivl_scope_t cur = net ;  cur ;  cur = cur->parent)
	    len += strlen(cur->name_) + 1;

      return len;
}

static void push_scope_basename(ivl_scope_t net, char*buf)
{
      if (net->parent == 0) {
	    strcpy(buf, net->name_);
	    return;
      }

      push_scope_basename(net->parent, buf);
      strcat(buf, ".");
      strcat(buf, net->name_);
}

extern "C" const char* ivl_scope_name(ivl_scope_t net)
{
      static char*name_buffer = 0;
      static unsigned name_size = 0;

      if (net->parent == 0)
	    return net->name_;

      unsigned needlen = scope_name_len(net);

      if (name_size < needlen) {
	    name_buffer = (char*)realloc(name_buffer, needlen);
	    name_size = needlen;
      }


      push_scope_basename(net, name_buffer);

      return name_buffer;
}

extern "C" unsigned ivl_scope_params(ivl_scope_t net)
{
      assert(net);
      return net->nparam_;
}

extern "C" ivl_parameter_t ivl_scope_param(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nparam_);
      return net->param_ + idx;
}

extern "C" ivl_scope_t ivl_scope_parent(ivl_scope_t net)
{
      assert(net);
      return net->parent;
}

extern "C" unsigned ivl_scope_ports(ivl_scope_t net)
{
      assert(net);
      assert(net->type_ == IVL_SCT_FUNCTION);
      return net->ports;
}

extern "C" ivl_signal_t ivl_scope_port(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(net->type_ == IVL_SCT_FUNCTION);
      assert(idx < net->ports);
      return net->port[idx];
}

extern "C" unsigned ivl_scope_sigs(ivl_scope_t net)
{
      assert(net);
      return net->nsigs_;
}

extern "C" ivl_signal_t ivl_scope_sig(ivl_scope_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->nsigs_);
      return net->sigs_[idx];
}

extern "C" int ivl_scope_time_units(ivl_scope_t net)
{
      assert(net);
      return net->time_units;
}

extern "C" ivl_scope_type_t ivl_scope_type(ivl_scope_t net)
{
      assert(net);
      return net->type_;
}

extern "C" const char* ivl_scope_tname(ivl_scope_t net)
{
      assert(net);
      return net->tname_;
}

extern "C" const char* ivl_signal_attr(ivl_signal_t net, const char*key)
{
      if (net->nattr == 0)
	    return 0;

      for (unsigned idx = 0 ;  idx < net->nattr ;  idx += 1)

	    if (strcmp(key, net->attr[idx].key) == 0)
		  return net->attr[idx].type == IVL_ATT_STR
			? net->attr[idx].val.str
			: 0;

      return 0;
}

extern "C" unsigned ivl_signal_attr_cnt(ivl_signal_t net)
{
      return net->nattr;
}

extern "C" ivl_attribute_t ivl_signal_attr_val(ivl_signal_t net, unsigned idx)
{
      assert(idx < net->nattr);
      return net->attr + idx;
}

extern "C" const char* ivl_signal_basename(ivl_signal_t net)
{
      return net->name_;
}

extern "C" const char* ivl_signal_name(ivl_signal_t net)
{
      static char*name_buffer = 0;
      static unsigned name_size = 0;

      unsigned needlen = scope_name_len(net->scope_);
      needlen += strlen(net->name_) + 2;

      if (name_size < needlen) {
	    name_buffer = (char*)realloc(name_buffer, needlen);
	    name_size = needlen;
      }

      push_scope_basename(net->scope_, name_buffer);
      strcat(name_buffer, ".");
      strcat(name_buffer, net->name_);

      return name_buffer;
}

extern "C" ivl_nexus_t ivl_signal_pin(ivl_signal_t net, unsigned idx)
{
      assert(net);
      assert(idx < net->width_);
      if (net->width_ == 1) {
	    return net->n.pin_;

      } else {
	    return net->n.pins_[idx];
      }
}

extern "C" unsigned ivl_signal_pins(ivl_signal_t net)
{
      return net->width_;
}

extern "C" int ivl_signal_msb(ivl_signal_t net)
{
      assert(net->lsb_dist == 1 || net->lsb_dist == -1);
      return net->lsb_index + net->lsb_dist * (net->width_ - 1);
}

extern "C" int ivl_signal_lsb(ivl_signal_t net)
{
      return net->lsb_index;
}

extern "C" ivl_signal_port_t ivl_signal_port(ivl_signal_t net)
{
      return net->port_;
}

extern "C" int ivl_signal_local(ivl_signal_t net)
{
      return net->local_;
}

extern "C" int ivl_signal_signed(ivl_signal_t net)
{
      return net->signed_;
}

extern "C" int ivl_signal_integer(ivl_signal_t net)
{
      return net->isint_;
}

extern "C" ivl_signal_type_t ivl_signal_type(ivl_signal_t net)
{
      return net->type_;
}

extern "C" ivl_statement_type_t ivl_statement_type(ivl_statement_t net)
{
      return net->type_;
}

extern "C" ivl_scope_t ivl_stmt_block_scope(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_BLOCK:
	  case IVL_ST_FORK:
	    return net->u_.block_.scope;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_stmt_block_count(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_BLOCK:
	  case IVL_ST_FORK:
	    return net->u_.block_.nstmt_;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_statement_t ivl_stmt_block_stmt(ivl_statement_t net,
					       unsigned i)
{
      switch (net->type_) {
	  case IVL_ST_BLOCK:
	  case IVL_ST_FORK:
	    return net->u_.block_.stmt_ + i;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_scope_t ivl_stmt_call(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_DISABLE:
	    return net->u_.disable_.scope;

	  case IVL_ST_UTASK:
	    return net->u_.utask_.def;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned ivl_stmt_case_count(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_CASE:
	  case IVL_ST_CASER:
	  case IVL_ST_CASEX:
	  case IVL_ST_CASEZ:
	    return net->u_.case_.ncase;
	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_expr_t ivl_stmt_case_expr(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_CASE:
	  case IVL_ST_CASER:
	  case IVL_ST_CASEX:
	  case IVL_ST_CASEZ:
	    assert(idx < net->u_.case_.ncase);
	    return net->u_.case_.case_ex[idx];

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_statement_t ivl_stmt_case_stmt(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_CASE:
	  case IVL_ST_CASER:
	  case IVL_ST_CASEX:
	  case IVL_ST_CASEZ:
	    assert(idx < net->u_.case_.ncase);
	    return net->u_.case_.case_st + idx;

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_expr_t ivl_stmt_cond_expr(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_CONDIT:
	    return net->u_.condit_.cond_;

	  case IVL_ST_CASE:
	  case IVL_ST_CASER:
	  case IVL_ST_CASEX:
	  case IVL_ST_CASEZ:
	    return net->u_.case_.cond;

	  case IVL_ST_REPEAT:
	  case IVL_ST_WHILE:
	    return net->u_.while_.cond_;

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" ivl_statement_t ivl_stmt_cond_false(ivl_statement_t net)
{
      assert(net->type_ == IVL_ST_CONDIT);
      if (net->u_.condit_.stmt_[1].type_ == IVL_ST_NONE)
	    return 0;
      else
	    return net->u_.condit_.stmt_ + 1;
}

extern "C" ivl_statement_t ivl_stmt_cond_true(ivl_statement_t net)
{
      assert(net->type_ == IVL_ST_CONDIT);
      if (net->u_.condit_.stmt_[0].type_ == IVL_ST_NONE)
	    return 0;
      else
	    return net->u_.condit_.stmt_ + 0;
}

extern "C" ivl_expr_t ivl_stmt_delay_expr(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_ASSIGN:
	  case IVL_ST_ASSIGN_NB:
	    return net->u_.assign_.delay;

	  case IVL_ST_DELAYX:
	    return net->u_.delayx_.expr;

	  default:
	    assert(0);
	    return 0;
      }
}

extern "C" unsigned long ivl_stmt_delay_val(ivl_statement_t net)
{
      assert(net->type_ == IVL_ST_DELAY);
      return net->u_.delay_.delay_;
}

extern "C" unsigned ivl_stmt_nevent(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_WAIT:
	    return net->u_.wait_.nevent;

	  case IVL_ST_TRIGGER:
	    return 1;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" ivl_event_t ivl_stmt_events(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_WAIT:
	    assert(idx < net->u_.wait_.nevent);
	    if (net->u_.wait_.nevent == 1)
		  return net->u_.wait_.event;
	    else
		  return net->u_.wait_.events[idx];

	  case IVL_ST_TRIGGER:
	    assert(idx == 0);
	    return net->u_.wait_.event;
	  default:
	    assert(0);
      }
      return 0;
}

extern "C" ivl_lval_t ivl_stmt_lval(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_ASSIGN:
	  case IVL_ST_ASSIGN_NB:
	    assert(idx < net->u_.assign_.lvals_);
	    return net->u_.assign_.lval_ + idx;

	  case IVL_ST_CASSIGN:
	  case IVL_ST_DEASSIGN:
	  case IVL_ST_FORCE:
	  case IVL_ST_RELEASE:
	    assert(idx < net->u_.cassign_.lvals);
	    return net->u_.cassign_.lval + idx;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" unsigned ivl_stmt_lvals(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_ASSIGN:
	  case IVL_ST_ASSIGN_NB:
	    return net->u_.assign_.lvals_;

	  case IVL_ST_CASSIGN:
	  case IVL_ST_DEASSIGN:
	  case IVL_ST_FORCE:
	  case IVL_ST_RELEASE:
	    return net->u_.cassign_.lvals;

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" unsigned ivl_stmt_lwidth(ivl_statement_t net)
{
      assert((net->type_ == IVL_ST_ASSIGN)
	     || (net->type_ == IVL_ST_ASSIGN_NB));

      unsigned sum = 0;
      for (unsigned idx = 0 ;  idx < net->u_.assign_.lvals_ ;  idx += 1) {
	    ivl_lval_t cur = net->u_.assign_.lval_ + idx;
	    switch(cur->type_) {
		case IVL_LVAL_MUX:
		  sum += 1;
		  break;
		case IVL_LVAL_REG:
		  sum += ivl_lval_pins(cur);
		  break;
		case IVL_LVAL_MEM:
		  sum += ivl_memory_width(ivl_lval_mem(cur));
		  break;
		case IVL_LVAL_VAR:
		  sum += 0;
		  break;
		default:
		  assert(0);
	    }
      }

      return sum;
}

extern "C" const char* ivl_stmt_name(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_STASK:
	    return net->u_.stask_.name_;
	  default:
	    assert(0);
      }

      return 0;
}

extern "C" ivl_nexus_t ivl_stmt_nexus(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_CASSIGN:
	  case IVL_ST_FORCE:
	    assert(idx < net->u_.cassign_.npins);
	    return net->u_.cassign_.pins[idx];
	  default:
	    assert(0);
      }

      return 0;
}

extern "C" unsigned ivl_stmt_nexus_count(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_CASSIGN:
	  case IVL_ST_FORCE:
	    return net->u_.cassign_.npins;
	  default:
	    assert(0);
      }

      return 0;
}

extern "C" ivl_expr_t ivl_stmt_parm(ivl_statement_t net, unsigned idx)
{
      switch (net->type_) {
	  case IVL_ST_STASK:
	    assert(idx < net->u_.stask_.nparm_);
	    return net->u_.stask_.parms_[idx];

	  default:
	    assert(0);
      }
      return 0;
}

extern "C" unsigned ivl_stmt_parm_count(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_STASK:
	    return net->u_.stask_.nparm_;
	  default:
	    assert(0);
      }
      return 0;
}

extern "C" ivl_expr_t ivl_stmt_rval(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_ASSIGN:
	  case IVL_ST_ASSIGN_NB:
	    return net->u_.assign_.rval_;
	  default:
	    assert(0);
      }

      return 0;
}

extern "C" ivl_statement_t ivl_stmt_sub_stmt(ivl_statement_t net)
{
      switch (net->type_) {
	  case IVL_ST_DELAY:
	    return net->u_.delay_.stmt_;
	  case IVL_ST_DELAYX:
	    return net->u_.delayx_.stmt_;
	  case IVL_ST_FOREVER:
	    return net->u_.forever_.stmt_;
	  case IVL_ST_WAIT:
	    return net->u_.wait_.stmt_;
	  case IVL_ST_REPEAT:
	  case IVL_ST_WHILE:
	    return net->u_.while_.stmt_;
	  default:
	    assert(0);
      }

      return 0;
}

extern "C" const char* ivl_variable_name(ivl_variable_t net)
{
      assert(net);
      return net->name;
}

extern "C" ivl_variable_type_t ivl_variable_type(ivl_variable_t net)
{
      assert(net);
      return net->type;
}

//ADU_BEGIN
extern "C" unsigned ivl_lpm_ramDq_awidth(ivl_lpm_t net){
	assert(net);
	return net->u_.ramDq.awidth;
}

extern "C" unsigned ivl_lpm_ramDq_dwidth(ivl_lpm_t net){
	assert(net);
	return net->u_.ramDq.dwith;
}

extern "C" unsigned ivl_lpm_ramDq_depth(ivl_lpm_t net){
	assert(net);
	return net->u_.ramDq.depth;
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_inclock(ivl_lpm_t net){
	return net->u_.ramDq.clk_in;
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_outclock(ivl_lpm_t net){
	return net->u_.ramDq.clk_out;
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_we(ivl_lpm_t net){
	return net->u_.ramDq.we;
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_raddress(ivl_lpm_t net,int idx){
	return net->u_.ramDq.raddr[idx];
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_waddress(ivl_lpm_t net,int idx){
	return net->u_.ramDq.waddr[idx];
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_data(ivl_lpm_t net,int idx){
	return net->u_.ramDq.data_in[idx];
}

extern "C" ivl_nexus_t ivl_lpm_ramDq_q(ivl_lpm_t net,int idx){
	return net->u_.ramDq.data_out[idx];
}

//decoder
extern "C" unsigned ivl_lpm_decoder_awidth(ivl_lpm_t net){
	assert(net);
	return net->u_.decoder.awidth;
}

extern "C" ivl_nexus_t ivl_lpm_decoder_data(ivl_lpm_t net,int idx){
	assert(idx < net->u_.decoder.awidth);
	return net->u_.decoder.data[idx];
}

extern "C" ivl_nexus_t ivl_lpm_decoder_result(ivl_lpm_t net,int idx){
	assert(idx < (1U << net->u_.decoder.awidth));
	return net->u_.decoder.result[idx];
}

//ADU_END

/*
 * $Log: t-dll-api.cc,v $
 * Revision 1.108.2.5  2006/04/16 19:26:40  steve
 *  Fix handling of exploded memories with partial or missing resets.
 *
 * Revision 1.108.2.4  2006/03/26 23:09:24  steve
 *  Handle asynchronous demux/bit replacements.
 *
 * Revision 1.108.2.3  2006/03/12 07:34:19  steve
 *  Fix the memsynth1 case.
 *
 * Revision 1.108.2.2  2006/02/25 05:03:29  steve
 *  Add support for negedge FFs by using attributes.
 *
 * Revision 1.108.2.1  2006/02/19 00:11:33  steve
 *  Handle synthesis of FF vectors with l-value decoder.
 *
 * Revision 1.108  2004/10/04 01:10:55  steve
 *  Clean up spurious trailing white space.
 *
 * Revision 1.107  2004/06/30 02:16:27  steve
 *  Implement signed divide and signed right shift in nets.
 *
 * Revision 1.106  2003/12/03 02:46:24  steve
 *  Add support for wait on list of named events.
 *
 * Revision 1.105  2003/11/10 20:59:03  steve
 *  Design::get_flag returns const char* instead of string.
 *
 * Revision 1.104  2003/09/03 23:33:30  steve
 *  Pass FF synchronous set values to code generator.
 *
 * Revision 1.103  2003/08/22 23:14:26  steve
 *  Preserve variable ranges all the way to the vpi.
 *
 * Revision 1.102  2003/08/15 02:23:53  steve
 *  Add synthesis support for synchronous reset.
 *
 * Revision 1.101  2003/06/24 01:38:03  steve
 *  Various warnings fixed.
 *
 * Revision 1.100  2003/06/23 01:25:44  steve
 *  Module attributes make it al the way to ivl_target.
 *
 * Revision 1.99  2003/05/14 05:26:41  steve
 *  Support real expressions in case statements.
 *
 * Revision 1.98  2003/04/22 04:48:30  steve
 *  Support event names as expressions elements.
 *
 * Revision 1.97  2003/04/11 05:18:08  steve
 *  Handle signed magnitude compare all the
 *  way through to the vvp code generator.
 *
 * Revision 1.96  2003/03/10 23:40:53  steve
 *  Keep parameter constants for the ivl_target API.
 *
 * Revision 1.95  2003/03/06 04:32:40  steve
 *  Wrong sense of need compared to have.
 *
 * Revision 1.94  2003/03/06 01:24:37  steve
 *  Obsolete the ivl_event_name function.
 *
 * Revision 1.93  2003/03/06 00:28:42  steve
 *  All NetObj objects have lex_string base names.
 *
 * Revision 1.92  2003/03/03 02:22:41  steve
 *  Scope names stored only as basename.
 *
 * Revision 1.91  2003/02/26 01:29:24  steve
 *  LPM objects store only their base names.
 *
 * Revision 1.90  2003/01/26 21:15:59  steve
 *  Rework expression parsing and elaboration to
 *  accommodate real/realtime values and expressions.
 *
 * Revision 1.89  2002/12/21 00:55:58  steve
 *  The $time system task returns the integer time
 *  scaled to the local units. Change the internal
 *  implementation of vpiSystemTime the $time functions
 *  to properly account for this. Also add $simtime
 *  to get the simulation time.
 *
 * Revision 1.88  2002/10/23 01:47:17  steve
 *  Fix synth2 handling of aset/aclr signals where
 *  flip-flops are split by begin-end blocks.
 *
 * Revision 1.87  2002/09/26 03:18:04  steve
 *  Generate vvp code for asynch set/reset of NetFF.
 *
 * Revision 1.86  2002/08/12 01:35:00  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.85  2002/08/07 02:17:14  steve
 *  Dead code.
 *
 * Revision 1.84  2002/08/05 04:18:45  steve
 *  Store only the base name of memories.
 *
 * Revision 1.83  2002/07/05 21:26:17  steve
 *  Avoid emitting to vvp local net symbols.
 *
 * Revision 1.82  2002/06/21 04:59:35  steve
 *  Carry integerness throughout the compilation.
 *
 * Revision 1.81  2002/05/27 00:08:45  steve
 *  Support carrying the scope of named begin-end
 *  blocks down to the code generator, and have
 *  the vvp code generator use that to support disable.
 *
 * Revision 1.80  2002/05/26 01:39:03  steve
 *  Carry Verilog 2001 attributes with processes,
 *  all the way through to the ivl_target API.
 *
 *  Divide signal reference counts between rval
 *  and lval references.
 *
 * Revision 1.79  2002/05/24 04:36:23  steve
 *  Verilog 2001 attriubtes on nets/wires.
 */

