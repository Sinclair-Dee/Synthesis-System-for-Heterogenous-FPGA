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
#ident "$Id: net_assign.cc,v 1.18.2.3 2006/05/02 02:00:15 steve Exp $"
#endif

# include "config.h"

# include  "netlist.h"

/*
 * NetAssign
 */

unsigned count_lval_width(const NetAssign_*idx)
{
      unsigned wid = 0;
      while (idx) {
	    wid += idx->lwidth();
	    idx = idx->more;
      }
      return wid;
}

NetAssign_::NetAssign_(NetNet*s)
: sig_(s), mem_(0), var_(0), bmux_(0)
{
      loff_ = 0;
      lwid_ = sig_->pin_count();
      sig_->incr_lref();
      more = 0;
      mem_lref_ = false;
	  turn_sig_to_wire_on_release_ = true;//ADU:
}

NetAssign_::NetAssign_(NetMemory*s)
: sig_(0), mem_(s), var_(0), bmux_(0)
{
      loff_ = 0;
      lwid_ = mem_->width();
      more = 0;
      mem_lref_ = false;
	  turn_sig_to_wire_on_release_ = true;//ADU:
}

NetAssign_::NetAssign_(NetVariable*s)
: sig_(0), mem_(0), var_(s), bmux_(0)
{
      loff_ = 0;
      lwid_ = 0;
      more = 0;
      mem_lref_ = false;
	  turn_sig_to_wire_on_release_ = true;//ADU:
}

NetAssign_::~NetAssign_()
{
      if (sig_) {
	    sig_->decr_lref();
	    if (turn_sig_to_wire_on_release_ && sig_->peek_lref() == 0)
		  sig_->type(NetNet::WIRE);
      }

      if (mem_) {
	    NetNet*exp = mem_->reg_from_explode();
	    if (exp && mem_lref_) {
		  exp->decr_lref();
		  if (turn_sig_to_wire_on_release_ && exp->peek_lref() == 0)
			exp->type(NetNet::WIRE);
	    }
      }
      assert( more == 0 );
      if (bmux_) delete bmux_;
}

void NetAssign_::set_bmux(NetExpr*r)
{
      assert(bmux_ == 0);
      bmux_ = r;
}

NetExpr* NetAssign_::bmux()
{
      return bmux_;
}

const NetExpr* NetAssign_::bmux() const
{
      return bmux_;
}

unsigned NetAssign_::lwidth() const
{
      if (mem_)  return lwid_;
      else if (bmux_) return 1;
      else return lwid_;
}

perm_string NetAssign_::name() const
{
      if (sig_) {
	    return sig_->name();
      } else if (mem_) {
	    return mem_->name();
      } else {
	    return perm_string::literal("");
      }
}

NetNet* NetAssign_::sig() const
{
      if (mem_)
	    return 0;
      else
	    return sig_;
}

NetMemory* NetAssign_::mem() const
{
      return mem_;
}

NetVariable* NetAssign_::var() const
{
      return var_;
}

void NetAssign_::incr_mem_lref()
{
      if (! mem_lref_) {
	    assert(mem_);
	    NetNet*exp = mem_->reg_from_explode();
	    assert(exp);
	    exp->incr_lref();
	    mem_lref_ = true;
      }
}

void NetAssign_::set_part(unsigned lo, unsigned lw)
{
      loff_ = lo;
      lwid_ = lw;
      if (sig_) {
	    assert(sig_->pin_count() >= (lo + lw));
      } else {
	    assert(mem_);
	    assert(lwid_ == mem_->width());
      }
}

unsigned NetAssign_::get_loff() const
{
      return loff_;
}

/*
 */
void NetAssign_::turn_sig_to_wire_on_release()
{
      turn_sig_to_wire_on_release_ = true;
}

NetAssignBase::NetAssignBase(NetAssign_*lv, NetExpr*rv)
: lval_(lv), rval_(rv), delay_(0)
{
}

NetAssignBase::~NetAssignBase()
{
      if (rval_) delete rval_;
      while (lval_) {
	    NetAssign_*tmp = lval_;
	    lval_ = tmp->more;
	    tmp->more = 0;
	    delete tmp;
      }
}

NetExpr* NetAssignBase::rval()
{
      return rval_;
}

const NetExpr* NetAssignBase::rval() const
{
      return rval_;
}

void NetAssignBase::set_rval(NetExpr*r)
{
      if (rval_) delete rval_;
      rval_ = r;
}

NetAssign_* NetAssignBase::l_val(unsigned idx)
{
      NetAssign_*cur = lval_;
      while (idx > 0) {
	    if (cur == 0)
		  return cur;

	    cur = cur->more;
	    idx -= 1;
      }

      assert(idx == 0);
      return cur;
}

const NetAssign_* NetAssignBase::l_val(unsigned idx) const
{
      const NetAssign_*cur = lval_;
      while (idx > 0) {
	    if (cur == 0)
		  return cur;

	    cur = cur->more;
	    idx -= 1;
      }

      assert(idx == 0);
      return cur;
}

unsigned NetAssignBase::l_val_count() const
{
      const NetAssign_*cur = lval_;
      unsigned cnt = 0;
      while (cur) {
	    cnt += 1;
	    cur = cur->more;
      }

      return cnt;
}

unsigned NetAssignBase::lwidth() const
{
      unsigned sum = 0;
      for (NetAssign_*cur = lval_ ;  cur ;  cur = cur->more)
	    sum += cur->lwidth();
      return sum;
}

void NetAssignBase::set_delay(NetExpr*expr)
{
      delay_ = expr;
}

const NetExpr* NetAssignBase::get_delay() const
{
      return delay_;
}

NetAssign::NetAssign(NetAssign_*lv, NetExpr*rv)
: NetAssignBase(lv, rv)
{
}

NetAssign::~NetAssign()
{
}

NetAssignNB::NetAssignNB(NetAssign_*lv, NetExpr*rv)
: NetAssignBase(lv, rv)
{
}

NetAssignNB::~NetAssignNB()
{
}

/*
 * $Log: net_assign.cc,v $
 * Revision 1.18.2.3  2006/05/02 02:00:15  steve
 *  Fix uninitialized mem_lref_ member.
 *
 * Revision 1.18.2.2  2006/04/16 19:26:38  steve
 *  Fix handling of exploded memories with partial or missing resets.
 *
 * Revision 1.18.2.1  2006/03/12 07:34:17  steve
 *  Fix the memsynth1 case.
 *
 * Revision 1.18  2004/08/28 15:08:31  steve
 *  Do not change reg to wire in NetAssign_ unless synthesizing.
 *
 * Revision 1.17  2004/02/18 17:11:56  steve
 *  Use perm_strings for named langiage items.
 *
 * Revision 1.16  2003/01/26 21:15:58  steve
 *  Rework expression parsing and elaboration to
 *  accommodate real/realtime values and expressions.
 *
 * Revision 1.15  2002/08/12 01:34:59  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.14  2002/08/04 18:28:15  steve
 *  Do not use hierarchical names of memories to
 *  generate vvp labels. -tdll target does not
 *  used hierarchical name string to look up the
 *  memory objects in the design.
 *
 * Revision 1.13  2002/07/02 03:02:57  steve
 *  Change the signal to a net when assignments go away.
 *
 * Revision 1.12  2002/06/08 23:42:46  steve
 *  Add NetRamDq synthsesis from memory l-values.
 *
 * Revision 1.11  2002/06/04 05:38:44  steve
 *  Add support for memory words in l-value of
 *  blocking assignments, and remove the special
 *  NetAssignMem class.
 *
 * Revision 1.10  2002/05/26 01:39:02  steve
 *  Carry Verilog 2001 attributes with processes,
 *  all the way through to the ivl_target API.
 *
 *  Divide signal reference counts between rval
 *  and lval references.
 *
 * Revision 1.9  2002/04/21 22:31:02  steve
 *  Redo handling of assignment internal delays.
 *  Leave it possible for them to be calculated
 *  at run time.
 */

