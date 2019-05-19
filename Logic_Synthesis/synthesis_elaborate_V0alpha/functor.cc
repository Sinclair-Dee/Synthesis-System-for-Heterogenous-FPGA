/*
 * Copyright (c) 1999-2002 Stephen Williams (steve@icarus.com)
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
#ident "$Id: functor.cc,v 1.32.2.1 2006/04/23 04:26:14 steve Exp $"
#endif

# include "config.h"

# include  <iostream>

# include  "functor.h"
# include  "netlist.h"

functor_t::~functor_t()
{
}

void functor_t::event(class Design*, class NetEvent*)
{
}

void functor_t::signal(class Design*, class NetNet*)
{
}

void functor_t::process(class Design*, class NetProcTop*)
{
}

void functor_t::lpm_add_sub(class Design*, class NetAddSub*)
{
}

void functor_t::lpm_compare(class Design*, class NetCompare*)
{
}

void functor_t::lpm_const(class Design*, class NetConst*)
{
}

void functor_t::lpm_divide(class Design*, class NetDivide*)
{
}

void functor_t::lpm_modulo(class Design*, class NetModulo*)
{
}

void functor_t::lpm_ff(class Design*, class NetFF*)
{
}

void functor_t::lpm_logic(class Design*, class NetLogic*)
{
}

void functor_t::lpm_mult(class Design*, class NetMult*)
{
}

void functor_t::lpm_mem(class Design*, class NetMem*)
{
}

void functor_t::lpm_memory(class Design*, class NetMemory*)
{
}

void functor_t::lpm_square(class Design*, class NetSquare*)
{
}

void functor_t::lpm_lmult(class Design*, class NetLMult*)
{
}

void functor_t::lpm_mult_add(class Design*, class NetMultAdd*)
{
}

void functor_t::lpm_mult_accum(class Design*, class NetMultAccum*)
{
}

void functor_t::lpm_mux(class Design*, class NetMux*)
{
}

void functor_t::lpm_ram_dq(class Design*, class NetRamDq*)
{
}

void NetScope::run_functor(Design*des, functor_t*fun)
{
      for (NetScope*cur = sub_ ;  cur ;  cur = cur->sib_) {
	    cur->run_functor(des, fun);
      }

      for (NetEvent*cur = events_ ;  cur ;  /* */) {
	    NetEvent*tmp = cur;
	    cur = cur->snext_;
	    fun->event(des, tmp);
      }

	// apply to signals. Each iteration, allow for the possibility
	// that the current signal deletes itself.
      if (signals_) {
	    unsigned count = 0;
	    NetNet*cur = signals_->sig_next_;
	    do {
		  count += 1;
		  cur = cur->sig_next_;
	    } while (cur != signals_->sig_next_);

	    cur = signals_->sig_next_;
	    for (unsigned idx = 0 ;  idx < count ;  idx += 1) {
		  NetNet*tmp = cur->sig_next_;
		  fun->signal(des, cur);
		  cur = tmp;
	    }
      }

	  if(memories_){
		  NetMemory* cur = memories_;
		  do{
			  cur = cur->snext_;
			  fun->lpm_memory(des, cur);
		  } while (cur != memories_);
		  
	  }
	  
}

void Design::functor(functor_t*fun)
{
	// Scan the scopes
      for (list<NetScope*>::const_iterator scope = root_scopes_.begin();
	   scope != root_scopes_.end(); scope++)
	    (*scope)->run_functor(this, fun);

	// apply to processes
      procs_idx_ = procs_;
      while (procs_idx_) {
	    NetProcTop*idx = procs_idx_;
	    procs_idx_ = idx->next_;
	    fun->process(this, idx);
      }

	// apply to nodes
      if (nodes_) {
	    assert(nodes_functor_cur_ == 0);
	    assert(nodes_functor_nxt_ == 0);

	      /* Scan the circular list of nodes, starting with the
		 front of the list.

		 This loop interacts with the Design::del_node method
		 so that the functor is free to delete any nodes it
		 choose. The destructors of the NetNode objects call
		 the del_node method, which checks with the
		 nodes_functor_* members, to keep the iterator
		 operating safely. */
	    nodes_functor_cur_ = nodes_;
	    do {
		  nodes_functor_nxt_ = nodes_functor_cur_->node_next_;
		  nodes_functor_cur_->functor_node(this, fun);

		  if (nodes_functor_nxt_ == 0)
			break;

		  nodes_functor_cur_ = nodes_functor_nxt_;
	    } while (nodes_ && (nodes_functor_cur_ != nodes_));
	    nodes_functor_cur_ = 0;
	    nodes_functor_nxt_ = 0;

      }
}


void NetNode::functor_node(Design*, functor_t*)
{
}

void NetAddSub::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_add_sub(des, this);
}

void NetCompare::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_compare(des, this);
}

void NetConst::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_const(des, this);
}

void NetDivide::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_divide(des, this);
}

void NetFF::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_ff(des, this);
}

void NetLogic::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_logic(des, this);
}

void NetModulo::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_modulo(des, this);
}

void NetMult::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_mult(des, this);
}

void NetMem::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_mem(des, this);
}

void NetMemory::functor_node(Design*des, functor_t*fun)
{
	fun->lpm_memory(des, this);
}

void NetSquare::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_square(des, this);
}

void NetLMult::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_lmult(des, this);
}

void NetMultAdd::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_mult_add(des, this);
}

void NetMultAccum::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_mult_accum(des, this);
}

void NetMux::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_mux(des, this);
}

void NetRamDq::functor_node(Design*des, functor_t*fun)
{
      fun->lpm_ram_dq(des, this);
}

proc_match_t::~proc_match_t()
{
}

int NetProc::match_proc(proc_match_t*that)
{
      return 0;
}

int proc_match_t::assign(NetAssign*)
{
      return 0;
}

int NetAssign::match_proc(proc_match_t*that)
{
      return that->assign(this);
}

int proc_match_t::assign_nb(NetAssignNB*)
{
      return 0;
}

int NetAssignNB::match_proc(proc_match_t*that)
{
      return that->assign_nb(this);
}

int proc_match_t::block(NetBlock*)
{
      return 0;
}

int NetBlock::match_proc(proc_match_t*that)
{
      return that->block(this);
}

int proc_match_t::condit(NetCondit*)
{
      return 0;
}

int NetCondit::match_proc(proc_match_t*that)
{
      return that->condit(this);
}

int NetEvWait::match_proc(proc_match_t*that)
{
      return that->event_wait(this);
}

int proc_match_t::event_wait(NetEvWait*)
{
      return 0;
}

/*
 * $Log: functor.cc,v $
 * Revision 1.32.2.1  2006/04/23 04:26:14  steve
 *  Constant propagate addresses through NetRamDq read ports.
 *
 * Revision 1.32  2004/10/04 01:10:53  steve
 *  Clean up spurious trailing white space.
 *
 * Revision 1.31  2002/08/16 05:18:27  steve
 *  Fix intermix of node functors and node delete.
 *
 * Revision 1.30  2002/08/12 01:34:59  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.29  2002/08/10 22:07:38  steve
 *  Remove useless error messages.
 *
 * Revision 1.28  2002/06/05 03:44:25  steve
 *  Add support for memory words in l-value of
 *  non-blocking assignments, and remove the special
 *  NetAssignMem_ and NetAssignMemNB classes.
 *
 * Revision 1.27  2002/06/04 05:38:44  steve
 *  Add support for memory words in l-value of
 *  blocking assignments, and remove the special
 *  NetAssignMem class.
 *
 * Revision 1.26  2001/10/19 21:53:24  steve
 *  Support multiple root modules (Philip Blundell)
 *
 * Revision 1.25  2001/07/25 03:10:49  steve
 *  Create a config.h.in file to hold all the config
 *  junk, and support gcc 3.0. (Stephan Boettcher)
 *
 * Revision 1.24  2000/11/19 20:48:30  steve
 *  Fix cases where signal iteration might die early.
 *
 * Revision 1.23  2000/11/18 04:53:04  steve
 *  Watch out in functor, it may delete the last signal.
 *
 * Revision 1.22  2000/09/17 21:26:15  steve
 *  Add support for modulus (Eric Aardoom)
 *
 * Revision 1.21  2000/08/01 02:48:41  steve
 *  Support <= in synthesis of DFF and ram devices.
 */

