/*
 * Copyright (c) 2002 Stephen Williams (steve@icarus.com)
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
#ident "$Id: async.cc,v 1.7.2.2 2007/05/30 17:48:53 steve Exp $"
#endif

# include "config.h"

# include  "functor.h"
# include  "netlist.h"
# include  <assert.h>

bool NetAssign::is_asynchronous()
{
      return true;
}

bool NetCondit::is_asynchronous()
{
      return false;
}

/*
 * NetEvWait statements come from statements of the form @(...) in the
 * Verilog source. These event waits are considered asynchronous if
 * all of the events in the list are ANYEDGE, and all the inputs to
 * the statement are included in the sensitivity list. If any of the
 * events are posedge or negedge, the statement is synchronous
 * (i.e. an edge-triggered flip-flop) and if any of the inputs are
 * unaccounted for in the sensitivity list then the statement is a
 * latch.
 */
bool NetEvWait::is_asynchronous()
{
	/* The "sense" set contains the set of Nexa that are in the
	   sensitivity list. We also require that the events are all
	   level sensitive, but the nex_async_ method takes care of
	   that test. */
      NexusSet*sense = new NexusSet;
      for (unsigned idx = 0 ;  idx < nevents_ ;  idx += 1) {
	    NexusSet*tmp = event(idx)->nex_async_();
	    if (tmp == 0) {
		  delete sense;
		  return false;
	    }

	    sense->add(*tmp);
	    delete tmp;
      }

      NexusSet*inputs = statement_->nex_input();

	/* All the inputs of the statement must be accounted for in
	   the sensitivity list. Otherwise, there are inputs that can
	   change without the event being triggered, and that implies
	   a latch, not a combinational circuit. */
      if (! sense->contains(*inputs)) {
		  cout << get_line()
		       << ": warning: One or more signals are missing in the sensitivity list of\n"
			   "always block. To enable synthesis of FPGA/CPLD hardware, Iverilog will \nassume that all necessary signals are present in the sensitivity list.\n"<< endl;
		 //delete sense;
	    //delete inputs;
	    //return false;
      }

      delete sense;
      delete inputs;

	/* If it passes all the other tests, then this statement is
	   asynchronous. */
      return true;
}

bool NetProc::is_asynchronous()
{
      return false;
}

bool NetProcTop::is_asynchronous() const
{
      if (type_ == NetProcTop::KINITIAL)
	    return false;

      return statement_->is_asynchronous();
}

/*
 * $Log: async.cc,v $
 * Revision 1.7.2.2  2007/05/30 17:48:53  steve
 *  Support Latch synthesis (Alan Feldstein)
 *
 * Revision 1.7.2.1  2006/05/21 21:58:46  steve
 *  NetESignal input is only selected bits.
 *
 * Revision 1.7  2004/01/18 23:26:54  steve
 *  The is_combinational function really need not recurse.
 *
 * Revision 1.6  2003/12/20 00:33:39  steve
 *  More thorough check that NetEvWait is asynchronous.
 *
 * Revision 1.5  2003/09/04 20:28:05  steve
 *  Support time0 resolution of combinational threads.
 *
 * Revision 1.4  2002/08/18 22:07:16  steve
 *  Detect temporaries in sequential block synthesis.
 *
 * Revision 1.3  2002/08/12 01:34:58  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.2  2002/07/04 00:24:16  steve
 *  initial statements are not asynchronous.
 *
 * Revision 1.1  2002/06/30 02:21:31  steve
 *  Add structure for asynchronous logic synthesis.
 *
 */

