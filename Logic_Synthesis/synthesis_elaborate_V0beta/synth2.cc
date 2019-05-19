/*
* Copyright (c) 2002-2006 Stephen Williams (steve@icarus.com)
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
#ident "$Id: synth2.cc,v 1.39.2.48 2007/05/30 17:48:54 steve Exp $"
#endif

# include "config.h"

# include  "functor.h"
# include  "netlist.h"
# include  "compiler.h"
#include <cassert>
#include "NetLatch.h"
#include <climits>

#include <new> // standard operator new
using std::bad_alloc;

static int debug_synth2=0;

#ifdef __FUNCTION__

#define DEBUG_SYNTH2_ENTRY(class) if (debug_synth2) { cerr << "Enter " << class << "::" \
	<< __FUNCTION__ << endl; dump(cerr, 4); }
#define DEBUG_SYNTH2_EXIT(class,val) if (debug_synth2) { cerr << "Exit " << class << "::" \
	<< __FUNCTION__ << ", result " << val << endl; }

#else
#define DEBUG_SYNTH2_ENTRY(class)
#define DEBUG_SYNTH2_EXIT(class,val)
#endif

//#if 0
//bool NetProc::synth_async_noaccum(Design*des, NetScope*scope, bool sync_flag,
//struct sync_accounting_cell*nex_ff,
//	NetNet*nex_map, NetNet*nex_out)
//{
//	/* Make an unconnected stub for the accum_in net. */
//	const perm_string tmp = perm_string::literal("tmp");
//	NetNet*stub = new NetNet(scope, tmp, NetNet::WIRE, nex_out->pin_count());
//
//	bool flag = synth_async(des, scope, sync_flag, nex_ff,
//		nex_map, nex_out, stub);
//
//	delete stub;
//	return flag;
//}
//
//#endif

bool NetProc::synth_async_accum(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet* accum_in)
{
	
	bool flag = synth_async(des, scope, sync_flag, nex_ff,
		nex_map, nex_out, accum_in);

	return flag;
}

bool NetProc::synth_async(Design*des, NetScope*scope, bool sync_flag,
	struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	NetNet*accum_in)
{
	return false;
}

//ADU:
bool NetProc::synth_sync(Design*des, NetScope*scope,
	struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events, 
	NetNet* control_net, NetNet* zero_one_aload_net,
	NetNet* accum_in){
		return false;
}

bool NetProc::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events,NetNet* accum_in)
{
	return synth_async_accum(des, scope, true, nex_ff, nex_map, nex_out, accum_in);
}

//#if 0
//static unsigned find_nexus_in_set(const NetNet*nset, const Nexus*nex)
//{
//	unsigned idx = 0;
//	for (idx = 0 ;  idx < nset->pin_count() ;  idx += 1)
//		if (nset->pin(idx).nexus() == nex)
//			return idx;
//
//	return idx;
//}
//#endif

struct nexus_map_t {
	const Nexus*nex;
	int idx;
};
static int ncp_compare(const void*p1, const void*p2)
{
	const Nexus*a1 = ((const struct nexus_map_t*)p1) -> nex;
	const Nexus*a2 = ((const struct nexus_map_t*)p2) -> nex;
	if (a1 < a2)
		return -1;
	if (a1 > a2)
		return 1;
	return 0;
}

static struct nexus_map_t*make_nexus_index(const NetNet*nset)
{
	struct nexus_map_t*table = new struct nexus_map_t[nset->pin_count()];
	for (unsigned idx = 0 ;  idx < nset->pin_count() ;  idx += 1) {
		table[idx].nex = nset->pin(idx).nexus();
		table[idx].idx = idx;
	}
	qsort(table, nset->pin_count(), sizeof(struct nexus_map_t), ncp_compare);
	return table;
}

static int map_nexus_in_index(struct nexus_map_t*table, size_t ntable,
	const Nexus*nex)
{
	struct nexus_map_t key;
	key.nex = nex;
	struct nexus_map_t*res = (struct nexus_map_t*)
		bsearch(&key, table, ntable,
		sizeof(struct nexus_map_t), ncp_compare);

	if (res == 0)
		return -1;

	assert(res->nex == nex);
	return res->idx;
}



/*
* Async synthesis of assignments is done by synthesizing the rvalue
* expression, then connecting the l-value directly to the output of
* the r-value.
*
* The nex_map is the O-set for the statement, and lists the positions
* of the outputs as the caller wants results linked up. The nex_out,
* however, is the set of nexa that are to actually get linked to the
* r-value.
*/

bool NetAssignBase::synth_async(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	NetNet*accum_in)
{
	NetNet*rsig = rval_->synthesize(des);
	if (rsig == 0) {
		cerr << get_line() << ": error: Cannot synthesize r-value "
			<< "expression of assignment." << endl;
		des->errors += 1;
		return false;
	}

	assert(rsig);

	unsigned roff = 0;

	for (NetAssign_*cur = lval_ ;  cur ;  cur = cur->more) {

		NetMemory*lmem = cur->mem();
		if (lmem && !sync_flag) {
			cerr << get_line() << ": error: Cannot synthesize memory "
				<< "assignment in asynchronous logic." << endl;
			des->errors += 1;
			return false;
		}

		/* Is this an assignment to a memory? If so, then
		explode the memory to an array of reg bits. The
		context that is calling this will attach a decoder
		between the ff and the r-val. In fact, the memory at
		this point has already been scanned and exploded, so
		the explode_to_reg method below will return a
		pre-existing vector.

		Note that this is only workable if we are in the
		asynchronous path of a synchronous thread. The
		sync_flag must be true in this case. */
		if (lmem) {
			assert(sync_flag);
			synth_async_mem_sync_(des, scope, cur, rsig, roff,
				nex_map, nex_out);
			continue;
		}

		if (cur->bmux() && !sync_flag) {
			cerr << get_line() << ": error: Assign to bit select "
				<< "Not possible in asynchronous logic." << endl;
			des->errors += 1;
			return false;
		}

		NetNet*lsig = cur->sig();
		if (!lsig) {
			cerr << get_line() << ": error: NetAssignBase::synth_async "
				"on unsupported lval ";
			dump_lval(cerr);
			cerr << endl;
			des->errors += 1;
			return false;
		}


		/* Handle the special case that this is a decoded
		enable. generate a demux for the device, with the
		WriteData connected to the r-value and the Data
		vector connected to the feedback. */
		if (cur->bmux() != 0) {
			assert(sync_flag);

			NetNet*adr = cur->bmux()->synthesize(des);

			/* Create a NetEemux wide enough to connect to all
			the bits of the lvalue signal (generally more
			then the bits of lwidth). */
			NetDemux*dq = new NetDemux(scope, scope->local_symbol(),
				lsig->pin_count(),
				adr->pin_count(),
				lsig->pin_count());
			des->add_node(dq);
			dq->set_line(*this);

			/* The bmux expression connects to the address of
			the Demux device. */
			for (unsigned idx = 0; idx < adr->pin_count() ;  idx += 1)
				connect(dq->pin_Address(idx), adr->pin(idx));

			assert(cur->lwidth() == 1);

			/* Cycle the associated FF Data and Q through the
			demux to make synchronous "latches" that the
			Demux modifies. */
			assert(nex_ff[0].ff->width() >= lsig->pin_count());
			for (unsigned idx = 0; idx < lsig->pin_count(); idx += 1) {
				unsigned off = cur->get_loff()+idx;
				connect(nex_ff[0].ff->pin_Q(off), dq->pin_Data(idx));
			}

			struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

			for (unsigned idx = 0; idx < lsig->pin_count(); idx += 1) {
				unsigned off = cur->get_loff()+idx;
				int tmp = map_nexus_in_index(nex_map_idx,
					nex_map->pin_count(),
					lsig->pin(off).nexus());
				assert(tmp >= 0);
				unsigned ptr = tmp;
				connect(nex_out->pin(ptr), dq->pin_Q(idx));
			}

			delete[]nex_map_idx;

			/* The r-value (1 bit) connects to the WriteData
			input of the demux. */
			connect(dq->pin_WriteData(0), rsig->pin(roff));

			roff += cur->lwidth();
			cur->turn_sig_to_wire_on_release();
			continue;
		}

		/* By this point ant bmux() has been dealt with. Panic
		if that is not so. */
		assert(! cur->bmux());

		/* Bind the outputs that we do make to the nex_out. Use the
		nex_map to map the l-value bit position to the nex_out bit
		position. */

		/*start----ADU*/
		//NexusSet*inputs = this->nex_input();
		//NexusSet outputs;
		//this->nex_output(outputs);
		//
		//if( ! inputs->contains( outputs ) ){
		//	struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		//	for (unsigned idx = 0 ;  idx < cur->lwidth() ;  idx += 1) {
		//	  unsigned off = cur->get_loff()+idx;
		//	  int tmp = map_nexus_in_index(nex_map_idx,
		//					   nex_map->pin_count(),
		//					   lsig->pin(off).nexus());
		//	  assert(tmp >= 0);
		//	  unsigned ptr = tmp;
		//	  connect(nex_out->pin(ptr), rsig->pin(roff+idx));
		//	}			
		//}
		//delete inputs;
		/*end----ADU*/


		struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < cur->lwidth() ;  idx += 1) {
			unsigned off = cur->get_loff()+idx;
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				lsig->pin(off).nexus());
			assert(tmp >= 0);
			unsigned ptr = tmp;
			connect(nex_out->pin(ptr), rsig->pin(roff+idx));
		}


		roff += cur->lwidth();

		/* This lval_ represents a reg that is a WIRE in the
		synthesized results. This function signals the destructor
		to change the REG that this l-value refers to into a
		WIRE. It is done then, at the last minute, so that pending
		synthesis can continue to work with it as a WIRE. */
		cur->turn_sig_to_wire_on_release();
	}

	return true;
}

/*
* Handle the special case of assignment to memory. Explode the memory
* to an array of reg bits. The context that is calling this will
* attach a decoder between the ff and the r-val. In fact, the memory
* at this point has already been scanned and exploded, so the
* explode_to_reg method below will return a pre-existing vector.
*/
bool NetAssignBase::synth_async_mem_sync_(Design*des, NetScope*scope,
	NetAssign_*cur,
	NetNet*rsig, unsigned&roff,
	NetNet*nex_map, NetNet*nex_out)
{
	NetMemory*lmem = cur->mem();
	assert(lmem);

	if (debug_synth) {
		cerr << get_line() << ": debug: Start synthesis of assign "
			"to memory " << lmem->name() << "." << endl;
	}

	NetNet*msig = lmem->explode_to_reg();
	cur->incr_mem_lref();

	/* Handle the special case that the mux expression is
	constant. In this case, just hook up the pertinent bits. */
	if (NetEConst*ae = dynamic_cast<NetEConst*>(cur->bmux())) {
		long adr_s = ae->value().as_long();
		if (adr_s >= (long)lmem->count()) {
			cerr << get_line() << ": error: "
				<< "Address " << adr_s
				<< " is outside range of memory."
				<< " Skipping assignment." << endl;
			des->errors += 1;
			return false;
		}

		struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		unsigned adr = lmem->index_to_address(adr_s) * lmem->width();
		for (unsigned idx = 0 ;  idx < cur->lwidth() ;  idx += 1) {
			unsigned off = adr+idx;
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				msig->pin(off).nexus());
			assert(tmp >= 0);
			unsigned ptr = tmp;
			connect(nex_out->pin(ptr), rsig->pin(roff+idx));
		}

		delete[]nex_map_idx;

		cur->turn_sig_to_wire_on_release();
		return true;
	}

	assert(cur->bmux() != 0);

	NetNet*adr = cur->bmux()->synthesize(des);

	NetDemux*dq = new NetDemux(scope, scope->local_symbol(),
		msig->pin_count(),
		adr->pin_count(),
		msig->pin_count() / cur->lwidth());
	des->add_node(dq);
	dq->set_line(*this);

	for (unsigned idx = 0; idx < adr->pin_count() ;  idx += 1)
		connect(dq->pin_Address(idx), adr->pin(idx));

	struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

	for (unsigned idx = 0; idx < msig->pin_count(); idx += 1) {
		unsigned off = idx;
		int tmp = map_nexus_in_index(nex_map_idx,
			nex_map->pin_count(),
			msig->pin(off).nexus());
		assert(tmp >= 0);
		unsigned ptr = tmp;
		connect(nex_out->pin(ptr), dq->pin_Q(idx));
	}

	delete[]nex_map_idx;

	for (unsigned idx = 0 ;  idx < msig->pin_count(); idx += 1)
		connect(dq->pin_Data(idx), nex_map->pin(roff+idx));

	for (unsigned idx = 0 ;  idx < cur->lwidth() ;  idx += 1)
		connect(dq->pin_WriteData(idx), rsig->pin(roff+idx));

	roff += cur->lwidth();
	cur->turn_sig_to_wire_on_release();

	if (debug_synth) {
		cerr << get_line() << ": debug: Finish synthesis of assign "
			"to memory " << lmem->name() << "." << endl;
	}

	return true;
}

/*
* Sequential blocks are translated to asynchronous logic by
* translating each statement of the block, in order, into gates. The
* nex_out for the block is the union of the nex_out for all the
* substatements.
*/
bool NetBlock::synth_async(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet*accum_in)
{
	if (last_ == 0) {
		return true;
	}

	if (debug_synth) {
		cerr << get_line() << ": debug: "
			<< (sync_flag?"sync":"async")
			<< " synthesis of statement block. "
			<< "pin_count=" << nex_out->pin_count() << endl;
	}

	const perm_string tmp1 = perm_string::literal("tmp1");
	const perm_string tmp2 = perm_string::literal("tmp2");
	const perm_string tmp3 = perm_string::literal("tmp3");

	NetNet*accum_out = new NetNet(scope, tmp3, NetNet::WIRE,
		nex_out->pin_count());
	accum_out->local_flag(true);

	/* Output that ultimately have not been driven should collect
	their value from the accumulated input. */
	assert(accum_out->pin_count() == accum_in->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		if (accum_in->pin(idx).nexus()->is_driven())
			connect(accum_out->pin(idx), accum_in->pin(idx));
	}

	bool flag = true;
	NetProc*cur = last_;
	do {
		NetNet*new_accum;

		cur = cur->next_;

		/* Create a temporary nex_map for the substatement. */
		NexusSet tmp_set;
		cur->nex_output(tmp_set);
		NetNet*tmp_map = new NetNet(scope, tmp1, NetNet::WIRE,
			tmp_set.count());
		for (unsigned idx = 0 ;  idx < tmp_map->pin_count() ;  idx += 1)
			connect(tmp_set[idx], tmp_map->pin(idx));

		/* Create also a temporary net_out to collect the
		output. */
		NetNet*tmp_out = new NetNet(scope, tmp2, NetNet::WIRE,
			tmp_set.count());
		tmp_out->set_line(*this);

		struct sync_accounting_cell*tmp_ff = 0;
		if(sync_flag)
			tmp_ff = new struct sync_accounting_cell[tmp_out->pin_count()];//ADU:

		/* Make a temporary set of currently accumulated outputs
		that we can pass to the synth_async of the
		sub-statement. Some sub-statements will use this to
		handle default cases specially. We will delete this
		temporary map as soon as the synth_async is done. */
		new_accum = new NetNet(scope, tmp3, NetNet::WIRE, tmp_set.count());
		struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_set.count() ;  idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_set[idx]);
			assert(tmp >= 0);
			unsigned ptr = tmp;

			if(sync_flag){
				NetFF*ff2 = nex_ff[ptr].ff ;//ADU:
				tmp_ff[idx].ff = ff2;       //ADU:
				tmp_ff[idx].pin = 0;        //ADU:
				tmp_ff[idx].proc = cur;     //ADU:
			}			

			if (accum_out->pin(ptr).nexus()->is_driven())
				connect(new_accum->pin(idx), accum_out->pin(ptr));
		}

		delete [] nex_map_idx;

		//ADU: ��ʱ��alwaysģ����,
		//     �����nex_ff�а�����FF�ĸ�����tmp���ŵĸ������ܲ�һ��,����
		/*
		always@(posedge clk)
			if(en1)
				begin
				y1 <= a;
				y2 <= b;
				end
			else
				begin
				y1 <= c;

				if(en2)
					y2 <= d;
			end
		*/
		bool ok_flag = cur->synth_async(des, scope, sync_flag, tmp_ff,
			tmp_map, tmp_out, new_accum);
		flag = flag && ok_flag;

		if(sync_flag)
			delete[]tmp_ff;//ADU:
		delete new_accum;

		/* NOTE: tmp_set is not valid after this point, since
		the cur->synth_async method may change nexa that it
		refers to. */

		if (ok_flag == false)
			continue;

		/* Now start building a new set of accumulated outputs
		that we will pass to the next statement of the block,
		or that will be the output of the block. */
		NetNet* new_accum_2 = new NetNet(scope, tmp3, NetNet::WIRE,
			nex_out->pin_count());
		new_accum_2->set_line(*this);
		new_accum_2->local_flag(true);

		/* Use the nex_map to link up the output from the
		substatement to the output of the block as a
		whole. */

		nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_out->pin_count() ; idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_map->pin(idx).nexus());
			if (tmp < 0) {
				cerr << cur->get_line() << ": internal error: "
					<< "Nexus isn't in nex_map?! idx=" << idx
					<< " map width = " << nex_map->pin_count()
					<< " tmp_map count = " << tmp_map->pin_count()
					<< endl;
			}
			assert(tmp >= 0);
			unsigned ptr = tmp;
			connect(new_accum_2->pin(ptr), tmp_out->pin(idx));
		}

		delete[]nex_map_idx;

		delete tmp_map;
		delete tmp_out;

		assert(new_accum_2->pin_count() == accum_out->pin_count());

		/* Anything that is not redriven by the current
		statement inherits the value that was driven from any
		previous statements. Thus, the current statement can
		*override* the outputs of any previous statements. */
		for (unsigned idx = 0;  idx < new_accum_2->pin_count();  idx += 1) {

			if (new_accum_2->pin(idx).nexus()->is_driven())
				continue;

			connect(new_accum_2->pin(idx), accum_out->pin(idx));
		}
		delete accum_out;
		accum_out = new_accum_2;

	} while (cur != last_);


	/* Now bind the accumulated output values to the nex_out
	passed in. Note that each previous step has already did the
	pin mapping, so just connect. */
	assert(nex_out->pin_count() == accum_out->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		connect(nex_out->pin(idx), accum_out->pin(idx));
	}

	delete accum_out;

	if (debug_synth) {
		cerr << get_line() << ": debug: "
			<< (sync_flag?"sync":"async")
			<< " synthesis of statement block complete. " << endl;

	}
	return flag;
}

bool NetCase::synth_async(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet*accum)
{	
	typedef unsigned long long ULL64bit;
	
	if(type_ == EQX || type_ == EQZ){
		std::cerr << this->get_line() << ": error: "
			<< "Current version do not support the casex and casez." << std::endl;

		des->errors += 1;
		return false;
	}
	

	NetNet*esig = expr_->synthesize(des);

	bool full_case_flag = false;
	if (attribute(perm_string::literal("ivl_full_case")).len() > 0)
		full_case_flag = true;

	/* Scan the select vector looking for constant bits. The
	constant bits will be elided from the select input connect,
	but we still need to keep track of them. */
	unsigned sel_pins = 0;
	ULL64bit sel_mask = 0;
	ULL64bit sel_ref = 0;
	for (unsigned idx = 0 ;  idx < esig->pin_count() ;  idx += 1) {

		if (esig->pin(idx).nexus()->drivers_constant()) {
			verinum::V bit = esig->pin(idx).nexus()->driven_value();
			if (bit == verinum::V1)
				sel_ref |= 1 << idx;

		} else {
			sel_pins += 1;
			sel_mask |= 1 << idx;
		}
	}

	/* At this point, sel_ref represents the constant part of the
	select input. That is, (select&sel_mask) == sel_ref for all
	guard values that are reachable. We can use this to skip
	unreachable guards. */


	/* Build a map of guard values to mux select values. This
	helps account for constant select bits that are being
	elided. The guard2sel mapping will only be valid for
	reachable guards. */

	map<ULL64bit,ULL64bit>guard2sel;
	unsigned cur = 0;
	for (ULL64bit idx = 0 ;  idx < (1U<<esig->pin_count()) ;  idx += 1) {
		if ((idx & ~sel_mask) == sel_ref) {
			guard2sel[idx] = cur;
			cur += 1;
		}
	}
	assert(cur == (1Ull << sel_pins));

	unsigned nondefault_items = 0;
	for (unsigned item = 0 ;  item < nitems_ ;  item += 1) {
		if (items_[item].guard != 0)
			nondefault_items += 1;
	}

	/* Handle the special case that this can be done it a smaller
	1-hot MUX. If there are fewer active cases then there are
	select pins, then a 1-hot encoding should be better. */

	bool one_hot_flag = true;
	for(unsigned item = 0 ;  item < nitems_ ;  item += 1){
		unsigned one_count = 0;
		if(items_[item].guard != 0){
			NetEConst*ge = dynamic_cast<NetEConst*>(items_[item].guard);/*��NetEConsParam�̳���NetEConst����ADU*/
			if (ge == 0) {
				cerr << items_[item].guard->get_line() << ": error: "
					<< "Guard expression is not constant for synthesis."
					<< endl;
				des->errors += 1;
				continue;
			}
			verinum gval = ge->value();
			for(unsigned i = 0; i < gval.len(); i++){
				if(verinum::V1 == gval[i])
					one_count++;
			}
		}
		if(one_count > 1){
			one_hot_flag = false;
			break;
		}
			
	}
	
	if (one_hot_flag && nondefault_items < sel_pins) {
		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "Implement case statement as 1-hot MUX." << endl;
		return synth_async_1hot_(des, scope, sync_flag, nex_ff,
			nex_map, nex_out, accum,
			esig, nondefault_items);
	}

	NetMux*mux = new NetMux(scope, scope->local_symbol(),
		nex_out->pin_count(),
		1U << sel_pins, sel_pins);
	mux->set_line(*this);

	/* Connect the non-constant select bits to the select input of
	the mux device. */
	cur = 0;
	for (unsigned idx = 0 ;  idx < esig->pin_count() ;  idx += 1) {
		/* skip bits that are known to be constant. */
		if ((sel_mask & (1U << idx)) == 0)
			continue;

		connect(mux->pin_Sel(cur), esig->pin(idx));
		cur += 1;
	}
	assert(cur == sel_pins);

	NetProc**statement_map = new NetProc*[1 << sel_pins];
	for (unsigned item = 0 ;  item < (1U<<sel_pins) ;  item += 1)
		statement_map[item] = 0;

	/* Assign the input statements to MUX inputs. This involves
	calculating the guard value, passing that through the
	guard2sel map, then saving the statement in the
	statement_map. If I find a default case, then save that for
	use later. */
	NetProc*default_statement = 0;
	for (unsigned item = 0 ;  item < nitems_ ;  item += 1) {
		/* Skip the default case this pass. */
		if (items_[item].guard == 0) {
			default_statement = items_[item].statement;
			continue;
		}

		NetEConst*ge = dynamic_cast<NetEConst*>(items_[item].guard);/*��NetEConsParam�̳���NetEConst����ADU*/
		if (ge == 0) {
			cerr << items_[item].guard->get_line() << ": error: "
				<< "Guard expression is not constant for synthesis."
				<< endl;
			des->errors += 1;
			continue;
		}

		assert(ge);
		verinum gval = ge->value();

		list<verinum>gstack;
		gstack.push_front(gval);

		/* A guard may have X/Z values, if this is a casex
		statement. In this case, replace a number with an x/z
		values with two numbers, one with a 0 substituted,
		another with a 1 substituted. Only process as a guard
		numbers that are well defined. The gstack allows us
		to build a list of numbers that match the pattern. */
		while (! gstack.empty()) {
			verinum tmp = gstack.front();
			gstack.pop_front();

			if (tmp.is_defined()
				|| type() == NetCase::EQ) {

					/* Skip guards that are unreachable. */
					if ((sel_ref&~sel_mask) != (tmp.as_ulong()&~sel_mask))
						continue;

					unsigned sel_idx = guard2sel[tmp.as_ulong()];
					assert(items_[item].statement);
					statement_map[sel_idx] = items_[item].statement;/*ѡ���sel�����ݶ˵Ķ�Ӧ��ϵ����ADU*/

			} else {
				/* Process casex and casez patterns. */
				verinum tmp0 = tmp;
				verinum tmp1 = tmp;
				unsigned idx = 0;
				verinum::V tv = verinum::Vz;
				while (idx < tmp.len()) {
					tv = tmp.get(idx);
					if (tv == verinum::Vx)
						break;
					if (tv == verinum::Vz)
						break;
					idx += 1;
				}
				// Can't handle an X in a casez statement.
				assert(tv==verinum::Vx? type()==NetCase::EQX : true);
				assert(idx < tmp.len());

				tmp0.set(idx, verinum::V0);
				tmp1.set(idx, verinum::V1);
				gstack.push_front(tmp1);
				gstack.push_front(tmp0);
			}
		}
	}

	/* Set up a default default_sig that uses the accumulated
	input pins. This binding is suppressed by an actual default
	statement if one exists. */
	NetNet*default_sig = 0;
	if (default_statement == 0) {
		default_sig = accum;
		for (unsigned idx = 0 ;  idx < accum->pin_count() ;  idx += 1) {
			if (! accum->pin(idx).is_linkedToSignificantSignal()) {
				default_sig = 0;
				break;
			}
		}
	}

	bool return_flag = true;

	/* Now that statements match with mux inputs, synthesize the
	sub-statements. If I get to an input that has no statement,
	then use the default statement there. */
	for (unsigned item = 0 ;  item < (1U<<sel_pins) ;  item += 1) {

		/* Detect the case that this is a default input, and I
		have a precalculated default_sig. */
		if ((statement_map[item] == 0) && (default_sig != 0)) {
			for (unsigned idx = 0 ;  idx < mux->width() ;  idx += 1)
				if(default_sig->pin(idx).is_linkedToSignificantSignal())
					connect(mux->pin_Data(idx, item), default_sig->pin(idx));
			continue;
		}

		NetNet*sig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, nex_map->pin_count());
		sig->local_flag(true);

		/* If this statement is missing, arrange for the default
		statement to be processed here. Also, make the sig be
		the default sig so that the next time we run into a
		reference to the default, we just hook up to the
		default again. */
		if ((statement_map[item] == 0) && (default_statement != 0)) {
			statement_map[item] = default_statement;			
			default_sig = sig;
			default_statement = 0;
		}

		if (statement_map[item] == 0 && !sync_flag && !full_case_flag) {
			/* Missing case and no default; this could still be
			* synthesizable with synchronous logic, but not here. */
			cout << get_line()
				<< ": warning: Case item " << item << " is missing"
				<< " in combinational process.\n" << "It will generate latch(s)" << endl;
		}

		/* Synthesize this specified case. The synth_async will
		connect all the output bits it knows how to the sig net. */

		/*��������ʾ���������case�����������always�����ʱ�ͻ�ִ����������statement_map[2] == 0�����
		case({s1,s0})
		2'b00: y = a;
		2'b01: y = b;
		endcase

		*/
		if(statement_map[item] != 0){
			statement_map[item]->synth_async(des, scope, sync_flag, nex_ff, nex_map, sig, accum);

			for (unsigned idx = 0 ;  idx < mux->width() ;  idx += 1) {
				if (sig->pin(idx).is_linkedToSignificantSignal() && (sig->pin(idx).nexus() != nex_map->pin(idx).nexus()))//sig->pin(idx).is_linked()
					connect(mux->pin_Data(idx,item), sig->pin(idx));
				else if (accum->pin(idx).is_linkedToSignificantSignal() && (accum->pin(idx).nexus() != nex_map->pin(idx).nexus())){//accum->pin(idx).is_linked()
					connect(mux->pin_Data(idx,item), accum->pin(idx));
					//assert(0);
				}				
				else if (sync_flag){
					//���ӷ��ں�����ͳһ����ADU
					//connect(mux->pin_Data(idx,item), nex_map->pin(idx));
				}
				else {
					/* No likely input for this bit. So
					leave it. The connectivity test
					below will determine if this is an
					error or not. */
				}
			}
		}else{
			for (unsigned idx = 0 ;  idx < mux->width() ;  idx += 1){
				if (accum->pin(idx).is_linkedToSignificantSignal() && (accum->pin(idx).nexus() != nex_map->pin(idx).nexus())){
					connect(mux->pin_Data(idx,item), accum->pin(idx));
				}
			}
		}
	}

	/*start----��������ǻ�������Դ�������޸�---ADU*/
	for (unsigned wdx = 0 ;  wdx < mux->width() ;  wdx += 1) {
		if(sync_flag){
			//��ʱ��������
			for (unsigned item = 0 ; item < (1U<<sel_pins) ;  item += 1) {
				if (!mux->pin_Data(wdx,item).is_linkedToSignificantSignal()){
					connect(mux->pin_Data(wdx, item), nex_map->pin(wdx));
				}
			}
			connect(mux->pin_Result(wdx), nex_out->pin(wdx));
		}else{
			//����ϴ���
			unsigned linked_count = 0;

			for (unsigned item = 0 ; item < (1U<<sel_pins) ;  item += 1) {
				if (mux->pin_Data(wdx,item).is_linkedToSignificantSignal()) {
					linked_count += 1;
				}
			}

			//Ӧ�ü��linked_count == 0�������������
			if(linked_count == 0){
				//�д�����
				//�����������ʱ����MUX�϶����������˿�ֱ������
				//��˶�Ӧ�ĸ�nexus_out�����ӣ�������һ������
				//assert(0);
				NetConst*zero = new NetConst(scope, scope->local_symbol(),
					verinum::V0);
				zero->set_line(*this);
				des->add_node(zero);

				NetNet*zsig = new NetNet(scope, scope->local_symbol(),
					NetNet::WIRE, 1);
				zsig->local_flag(true);
				zsig->set_line(*this);
				connect(zsig->pin(0), zero->pin(0));
				for (unsigned item = 0 ; item < (1U<<sel_pins) ;  item += 1) {					
					connect(mux->pin_Data(wdx,item), zsig->pin(0));
				}
			}else if (linked_count <= (1U<<sel_pins)/2){
				NetLatch *const latchPtr = new NetLatch(scope, scope->local_symbol(), 1U);
				des->add_node( latchPtr );
				latchPtr->set_line( *this );

				NetNet*mux_out_sig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
				mux_out_sig->local_flag(true);
				mux_out_sig->set_line(*this);

				connect( mux->pin_Result(wdx), mux_out_sig->pin(0) );
				connect( mux_out_sig->pin(0), latchPtr->pin_Data(0) );

				connect( latchPtr->pin_Q(0), nex_out->pin(wdx) );

				NetLogic*ORgate = new NetLogic(scope, scope->local_symbol(),
					linked_count+1, NetLogic::OR);
				des->add_node(ORgate);

				unsigned temp_linked_count = 0;
				for (unsigned item = 0 ; item < (1U<<sel_pins) ;  item += 1) {

					if ( mux->pin_Data(wdx,item).is_linkedToSignificantSignal() ) {						

						NetLogic*ANDgate = new NetLogic(scope, scope->local_symbol(),
							sel_pins+1, NetLogic::AND);
						des->add_node(ANDgate);
						verinum*res = new verinum(item, sel_pins);
						for(unsigned idx = 0; idx < sel_pins; idx++ ){
							if(res->get(idx) == verinum::V0){
								NetLogic*NOTgate = new NetLogic(scope, scope->local_symbol(),
									2, NetLogic::NOT);
								des->add_node(NOTgate);

								NetNet*NOToutsig = new NetNet(scope, scope->local_symbol(),
									NetNet::WIRE, 1);
								NOToutsig->local_flag(true);

								connect( esig->pin(idx), NOTgate->pin(1) );
								connect( NOTgate->pin(0), NOToutsig->pin(0) );
								connect( NOToutsig->pin(0), ANDgate->pin(idx+1) );
							}else{
								connect( esig->pin(idx), ANDgate->pin(idx+1) );
							}
						}

						NetNet* AndOutSig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
						AndOutSig->local_flag(true);
						connect( ANDgate->pin(0), AndOutSig->pin(0) );
						connect( AndOutSig->pin(0), ORgate->pin(temp_linked_count + 1) );

						temp_linked_count ++;
					}else{
						//��δ���ӵ�mux->pin_Data(wdx,item)�ӵء���ADU
						NetConst*zero = new NetConst(scope, scope->local_symbol(),
							verinum::V0);
						zero->set_line(*this);
						des->add_node(zero);

						NetNet*zsig = new NetNet(scope, scope->local_symbol(),
							NetNet::WIRE, 1);
						zsig->local_flag(true);
						zsig->set_line(*this);

						connect(zsig->pin(0), zero->pin(0));
						connect(mux->pin_Data(wdx,item), zsig->pin(0));
					}
				}
				assert(temp_linked_count == linked_count);

				NetNet*temp_sig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
				temp_sig->local_flag(true);
				temp_sig->set_line(*this);

				connect( ORgate->pin(0), temp_sig->pin(0) );
				connect( temp_sig->pin(0), latchPtr->pin_Gate() );
			}else if(linked_count < (1U<<sel_pins)){
				//
				NetLatch *const latchPtr = new NetLatch(scope, scope->local_symbol(), 1U);
				des->add_node( latchPtr );
				latchPtr->set_line( *this );

				NetNet*mux_out_sig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
				mux_out_sig->local_flag(true);
				mux_out_sig->set_line(*this);

				connect( mux->pin_Result(wdx), mux_out_sig->pin(0) );
				connect( mux_out_sig->pin(0), latchPtr->pin_Data(0) );

				connect( latchPtr->pin_Q(0), nex_out->pin(wdx) );

				NetLogic*ORgate = new NetLogic(scope, scope->local_symbol(), (1U<<sel_pins) - linked_count+1, NetLogic::OR);
				des->add_node(ORgate);

				unsigned temp_not_linked_count = 0;
				for (unsigned item = 0 ; item < (1U<<sel_pins) ;  item += 1) {

					if ( mux->pin_Data(wdx,item).is_linked() ) {
						//�����κδ���
					}else{						

						NetLogic*ANDgate = new NetLogic(scope, scope->local_symbol(),
							sel_pins+1, NetLogic::AND);
						des->add_node(ANDgate);
						verinum*res = new verinum(item, sel_pins);
						for(unsigned idx = 0; idx < sel_pins; idx++ ){
							if(res->get(idx) == verinum::V0){
								NetLogic*NOTgate = new NetLogic(scope, scope->local_symbol(),
									2, NetLogic::NOT);
								des->add_node(NOTgate);

								NetNet*NOToutsig = new NetNet(scope, scope->local_symbol(),
									NetNet::WIRE, 1);
								NOToutsig->local_flag(true);

								connect( esig->pin(idx), NOTgate->pin(1) );
								connect( NOTgate->pin(0), NOToutsig->pin(0) );
								connect( NOToutsig->pin(0), ANDgate->pin(idx+1) );
							}else{
								connect( esig->pin(idx), ANDgate->pin(idx+1) );
							}
						}

						NetNet* AndOutSig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
						AndOutSig->local_flag(true);
						connect( ANDgate->pin(0), AndOutSig->pin(0) );
						connect( AndOutSig->pin(0), ORgate->pin(temp_not_linked_count + 1) );

						temp_not_linked_count ++;

						//��δ���ӵ�mux->pin_Data(wdx,item)�ӵء���ADU
						NetConst*zero = new NetConst(scope, scope->local_symbol(),
							verinum::V0);
						zero->set_line(*this);
						des->add_node(zero);

						NetNet*zsig = new NetNet(scope, scope->local_symbol(),
							NetNet::WIRE, 1);
						zsig->local_flag(true);
						zsig->set_line(*this);

						connect(zsig->pin(0), zero->pin(0));
						connect(mux->pin_Data(wdx,item), zsig->pin(0));

					}
				}
				assert( temp_not_linked_count == ((1U<<sel_pins) - linked_count) );

				//����Ĵ��봦��OR��NOT�Լ�����
				NetNet*OR_out_sig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
				OR_out_sig->local_flag(true);
				OR_out_sig->set_line(*this);

				NetLogic*NOTgate = new NetLogic(scope, scope->local_symbol(),2, NetLogic::NOT);
				des->add_node(NOTgate);

				NetNet*NOT_out_sig = new NetNet(scope, scope->local_symbol(),NetNet::WIRE, 1);
				NOT_out_sig->local_flag(true);
				NOT_out_sig->set_line(*this);

				connect( ORgate->pin(0), OR_out_sig->pin(0) );
				connect( OR_out_sig->pin(0), NOTgate->pin(1) );

				connect( NOTgate->pin(0), NOT_out_sig->pin(0) );
				connect( NOT_out_sig->pin(0), latchPtr->pin_Gate() );
			}else{
				connect(nex_out->pin(wdx), mux->pin_Result(wdx));
			}
		}
		
	}
	/*end-------ADU*/

	delete[]statement_map;
	des->add_node(mux);

	return return_flag;
}

bool NetCase::synth_async_1hot_(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet*accum,
	NetNet*esig, unsigned hot_items)
{
	unsigned sel_pins = hot_items;

	NetMux*mux = new NetMux(scope, scope->local_symbol(),
		nex_out->pin_count(),
		1U << sel_pins, sel_pins);
	mux->set_line(*this);

	/* Hook up the output of the mux to the mapped output pins. */
	for (unsigned idx = 0 ;  idx < mux->width() ;  idx += 1)
		connect(nex_out->pin(idx), mux->pin_Result(idx));

	NetNet*tmps = new NetNet(scope, scope->local_symbol(),
		NetNet::WIRE, sel_pins);
	for (unsigned idx = 0 ;  idx < sel_pins ;  idx += 1)
		connect(tmps->pin(idx), mux->pin_Sel(idx));

	NetProc*default_statement = 0;
	unsigned use_item = 0;
	for (unsigned item = 0 ;  item < nitems_ ;  item += 1) {
		if (items_[item].guard == 0) {
			default_statement = items_[item].statement;
			continue;
		}

		NetNet*tmp1 = items_[item].guard->synthesize(des);
		assert(tmp1);

		NetLogic*reduc = new NetLogic(scope, scope->local_symbol(),
			1 + esig->pin_count(),
			NetLogic::AND);
		des->add_node(reduc);

		tmps = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, esig->pin_count());
		for (unsigned idx = 0 ;  idx < tmps->pin_count() ;  idx += 1)
			connect(tmps->pin(idx), reduc->pin(idx+1));

		assert(tmp1->pin_count() == esig->pin_count());
		for (unsigned idx = 0 ;  idx < tmp1->pin_count() ; idx += 1) {
			NetCaseCmp*cmp = new NetCaseCmp(scope,scope->local_symbol());
			des->add_node(cmp);
			connect(cmp->pin(0), reduc->pin(1+idx));
			connect(cmp->pin(1), esig->pin(idx));
			connect(cmp->pin(2), tmp1->pin(idx));
		}

		connect(mux->pin_Sel(use_item), reduc->pin(0));

		NetNet*item_sig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, nex_map->pin_count());
		assert(items_[item].statement);
		items_[item].statement->synth_async(des, scope, sync_flag, nex_ff,
			nex_map, item_sig, accum);
		for (unsigned idx = 0 ;  idx < item_sig->pin_count() ;  idx += 1)
			connect(mux->pin_Data(idx, 1<<use_item), item_sig->pin(idx));

		use_item += 1;
	}

	assert(use_item == hot_items);

	/* Set up a default default_sig that uses the accumulated
	input pins. This binding is suppressed by an actual default
	statement if one exists. */
	NetNet*default_sig = 0;
	if (default_statement) {
		default_sig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, nex_map->pin_count());
		default_statement->synth_async(des, scope, sync_flag, nex_ff,
			nex_map, default_sig, accum);

	}

	if (default_sig == 0 && default_statement == 0) {
		default_sig = accum;
		for (unsigned idx = 0 ;  idx < accum->pin_count() ;  idx += 1) {
			if (! accum->pin(idx).is_linked()) {
				default_sig = 0;
				break;
			}
		}
	}

	/* No explicit sig, so if this is a synchronous process,
	connect the input to the output. */
	if (default_sig == 0 && sync_flag) {
		default_sig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, nex_map->pin_count());
		for (unsigned idx = 0; idx < default_sig->pin_count() ;  idx += 1)
			connect(default_sig->pin(idx), nex_map->pin(idx));
	}


	for (unsigned item = 0 ;  item < (1UL<<sel_pins) ;  item += 1) {
		unsigned count_bits = 0;
		for (unsigned idx = 0 ;  idx < sel_pins ;  idx += 1) {
			if (item & (1<<idx))
				count_bits += 1;
		}

		if (count_bits == 1)
			continue;

		for (unsigned idx = 0 ;  idx < mux->width() ;  idx += 1)
			connect(mux->pin_Data(idx,item), default_sig->pin(idx));
	}

	des->add_node(mux);
	return true;
}


/*
* Handle synthesis for an asynchronous condition statement. If we get
* here, we know that the CE of a DFF has already been filled, so the
* condition expression goes to the select of an asynchronous mux, unless a latch is inferred in which case it goes to the latch's gate input.
*/
bool NetCondit::synth_async(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	NetNet*accum)
{
	/* Detect the special case that this is a nul-effect (for
	synthesis) statement. This happens, for example, for code
	like this: if (foo) $display(...); */
	if (nex_out->pin_count() == 0) {
		if (debug_synth) {
			cerr << get_line() << ": debug: Skip synthesis of "
				<< "Condit statement with null effect." << endl;
		}
		return true;
	}

	NetNet*ssig = expr_->synthesize(des);
	if (ssig == 0) {
		if (debug_synth) {
			cerr << get_line() << ": debug: Unable to synthesize "
				<< "condition expression." << endl;
		}
		return false;
	}

	assert(ssig);

	/* Use the accumulated input net as a default input for
	covering a missing clause, except that if I find portions
	are unconnected, then give up on that idea. */

	/*ADU:
	Codes following require that all the pins of the signal default_sig
	should link to the significant signal.
	
	Does this work??
	*/
	NetNet*default_sig = accum;
	for (unsigned idx = 0 ;  idx < default_sig->pin_count() ;  idx += 1) {
		if (! default_sig->pin(idx).is_linkedToSignificantSignal()) {
			default_sig = 0;
			break;
		}
	}

	// At least one of the clauses must have contents. */
	assert(if_ != 0 || else_ != 0);

	NetNet*asig = new NetNet(scope, scope->local_symbol(),
		NetNet::WIRE, nex_map->pin_count());
	asig->local_flag(true);

	if (if_ == 0) {
		/* If the if clause is missing, then take the clause to
		be an assignment from the defaults input. If there is
		no defaults input and a latch is not inferred, then take the input to be from the
		output. */
		if (default_sig) {
			for (unsigned idx = 0 ;  idx < asig->pin_count() ;  idx += 1)
				connect(asig->pin(idx), default_sig->pin(idx));
		}
	} else {
		bool flag = if_->synth_async(des, scope, sync_flag, nex_ff,
			nex_map, asig, accum);
		if (!flag) {
			delete asig;
			cerr << get_line() << ": error: Asynchronous if statement"
				<< " true clause failed to synthesize." << endl;
			des->errors += 1;
			return false;
		}
	}

	NetNet*bsig = new NetNet(scope, scope->local_symbol(),
		NetNet::WIRE, nex_map->pin_count());
	bsig->local_flag(true);

	if (else_ == 0) {
		if (default_sig) {
			for (unsigned idx = 0 ;  idx < asig->pin_count() ;  idx += 1)
				connect(bsig->pin(idx), default_sig->pin(idx));
		}
	} else {
		bool flag = else_->synth_async(des, scope, sync_flag, nex_ff,
			nex_map, bsig, accum);
		if (!flag) {
			delete asig;
			delete bsig;
			cerr << get_line() << ": error: Asynchronous if statement"
				<< " else clause failed to synthesize." << endl;
			des->errors += 1;
			return false;
		}
	}


	unsigned mux_width = 0;

	/* Figure out how many mux bits we are going to need. */
	for (unsigned idx = 0 ;  idx < nex_out->pin_count();  idx += 1) {
		int flag = 0;
		if (asig->pin(idx).is_linkedToSignificantSignal() && (asig->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (asig->pin(idx).is_linked())����ADU
			flag |= 0100;/*�˽���������ADU*/
		if (bsig->pin(idx).is_linkedToSignificantSignal() && (bsig->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (bsig->pin(idx).is_linked())����ADU
			flag |= 0010;
		if (accum->pin(idx).is_linkedToSignificantSignal() && (accum->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (accum->pin(idx).is_linked())����ADU
			flag |= 0001;
		switch (flag) {
		case 0111:
		case 0110:
		case 0101:
			mux_width += 1;
			//if(flag == 0101)
			//	assert(0);
			break;
		case 0100:
			if (sync_flag)
				mux_width += 1;
			break;
		case 0011:
			mux_width += 1;
			//assert(0);
			break;
		case 0010:
			if (sync_flag)
				mux_width += 1;
			break;
		case 0001:
			mux_width += 1;
			assert(0);
			break;
		case 0000:
			break;
		}

	}

	NetMux*mux = 0;
	if(mux_width > 0){
		/* Create a mux and hook it up. */
		mux = new NetMux(scope, scope->local_symbol(),
			mux_width, 2, 1);
		des->add_node(mux);
		mux->set_line(*this);
		NetNet*mux_sig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, mux_width);
		mux_sig->local_flag(true);
		mux_sig->set_line(*this);

		for (unsigned idx = 0 ;  idx < mux_width ;  idx += 1)
			connect(mux->pin_Result(idx), mux_sig->pin(idx));

		if (debug_synth) {
			cerr << get_line() << ": debug: Condit synth to MUX "
				<< " width=" << mux_width
				<< " sel_width=1" << endl;
		}

		connect(mux->pin_Sel(0), ssig->pin(0));
	}

	/* Connected the clauses to the data inputs of the
	condition. If there are bits unassigned by the case, then
	bind them from the accum bits instead. If the bit is not
	represented in the accum list, but this is a synchronous
	output, then get the bit from the nex_map, which is the
	output held in the DFF. */
	mux_width = 0;
	for (unsigned idx = 0 ;  idx < nex_out->pin_count() ;  idx += 1) {
		int flag = 0;
		if (asig->pin(idx).is_linkedToSignificantSignal() && (asig->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (asig->pin(idx).is_linked())����ADU
			flag |= 0100;/*�˽���������ADU*/
		if (bsig->pin(idx).is_linkedToSignificantSignal() && (bsig->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (bsig->pin(idx).is_linked())����ADU
			flag |= 0010;
		if (accum->pin(idx).is_linkedToSignificantSignal() && (accum->pin(idx).nexus() != nex_map->pin(idx).nexus()))//if (accum->pin(idx).is_linked())����ADU
			flag |= 0001;

		switch (flag) {
		case 0111:
		case 0110:
			connect(mux->pin_Data(mux_width, 1), asig->pin(idx));
			connect(mux->pin_Data(mux_width, 0), bsig->pin(idx));
			connect(nex_out->pin(idx), mux->pin_Result(mux_width));
			mux_width += 1;
			//if(flag == 0111)
			//	assert(0);
			break;
		case 0101:
			connect(mux->pin_Data(mux_width, 1), asig->pin(idx));
			connect(mux->pin_Data(mux_width, 0), accum->pin(idx));
			connect(nex_out->pin(idx), mux->pin_Result(mux_width));
			mux_width += 1;
			//assert(0);
			break;
		case 0100:
			if (sync_flag) {
				connect(mux->pin_Data(mux_width, 1), asig->pin(idx));
				connect(mux->pin_Data(mux_width, 0),nex_map->pin(idx));
				connect(nex_out->pin(idx), mux->pin_Result(mux_width));
				mux_width += 1;
			} else {
#if 0
				cerr << get_line()
					<< ": error: Condition false clause "
					<< "does not assign expected outputs." << endl;
				des->errors += 1;
				return_flag = false;
#else
				/*start---ADU*/
				NetLatch *const latchPtr = new NetLatch(scope, scope->local_symbol(), 1U);
				latchPtr->set_line( *this );
				connect( latchPtr->pin_Data( 0 ), asig->pin(idx) );
				connect( nex_out->pin( idx ), latchPtr->pin_Q( 0 ) );
				connect( latchPtr->pin_Gate(), ssig->pin(0) );
				des->add_node( latchPtr );
				/*end---ADU*/
#endif
			}
			break;
		case 0011:
			connect(mux->pin_Data(mux_width, 1), accum->pin(idx));
			connect(mux->pin_Data(mux_width, 0), bsig->pin(idx));
			connect(nex_out->pin(idx), mux->pin_Result(mux_width));
			mux_width += 1;
			//assert(0);
			break;
		case 0010:
			if (sync_flag) {
				connect(mux->pin_Data(mux_width, 1),nex_map->pin(idx));
				connect(mux->pin_Data(mux_width, 0), bsig->pin(idx));
				connect(nex_out->pin(idx), mux->pin_Result(mux_width));
				mux_width += 1;
			} else {
#if 0
				cerr << get_line()
					<< ": error: Condition true clause "
					<< "does not assign expected outputs." << endl;
				des->errors += 1;
				return_flag = false;
#else
				/*start---ADU*/
				NetLatch *const latchPtr = new NetLatch(scope, scope->local_symbol(), 1U);
				latchPtr->set_line( *this );
				des->add_node( latchPtr );

				//���һ����ֵ�����else���ж�if��û�У������������������Ŀ����ź���Ҫ���һ�����š���ADU
				/*---ADU
				always@(*)
				if(la)
				q1 = a;
				else
				q2 = b;
				���ߣ�
				always@(*)
				if(la)
				q = q;
				else
				q = a;
				*/
				NetLogic* gate = new NetLogic(scope, scope->local_symbol(),
					2, NetLogic::NOT);
				//gate->set_line( *this );
				des->add_node(gate);

				//����gate֮������Ų���ֱ���������м�������һ��net����������pin�е�nexus�е�private_dataΪ0����ADU
				NetNet* tmpsig = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
				tmpsig->local_flag(true);

				connect(gate->pin(1), ssig->pin(0));
				connect(gate->pin(0), tmpsig->pin(0));

				connect(latchPtr->pin_Gate(), tmpsig->pin(0));

				connect( latchPtr->pin_Data( 0 ), bsig->pin(idx) );
				connect( nex_out->pin( idx ), latchPtr->pin_Q( 0 ) );

				/*end---ADU*/
#endif
			}
			break;
		case 0001:
			connect(mux->pin_Data(mux_width, 1), accum->pin(idx));
			connect(mux->pin_Data(mux_width, 0), accum->pin(idx));
			connect(nex_out->pin(idx), mux->pin_Result(mux_width));
			mux_width += 1;
			assert(0);
			break;
		case 0000:
			if (sync_flag) {
				/*ADU
				This will result in the port Q linked to port D in the same register
				In the following step, we will replace the link with "1" or "0",which 
				is determined by the set or reset port.*/
				connect(nex_out->pin(idx), nex_map->pin(idx));
			} else {
				//cerr << get_line() << ": internal error: "
				//     << "Unexplained mode?" << endl;
				//cerr << get_line() << ": XXXX: "
				//     << "nex_out->pin_count() = "
				//     << nex_out->pin_count() << endl;
				//assert(0);
			}
			break;
		default:
			assert(0);
			break;
		}
	}

	return true;
}

bool NetEvWait::synth_async(Design*des, NetScope*scope, bool sync_flag,
	sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet*accum_in)
{
	bool flag = statement_->synth_async(des, scope, sync_flag, nex_ff,
		nex_map, nex_out, accum_in);
	return flag;
}

bool NetProcTop::synth_async(Design*des)
{
	NexusSet nex_set;
	statement_->nex_output(nex_set);

	const perm_string tmp1 = perm_string::literal("tmp");
	NetNet*nex_out = new NetNet(scope(), tmp1, NetNet::WIRE,
		nex_set.count());
	for (unsigned idx = 0 ;  idx < nex_out->pin_count() ;  idx += 1)
		connect(nex_set[idx], nex_out->pin(idx));

	NetNet* accum_in = new NetNet(scope(), scope()->local_symbol(),
		NetNet::WIRE, nex_set.count());

	bool flag = statement_->synth_async_accum(des, scope(), false, 0,
		nex_out, nex_out, accum_in);

	delete nex_out;
	return flag;
}

static bool merge_ff_slices(NetFF*ff1, unsigned idx1,
	NetFF*ff2, unsigned idx2)
{

	/* If the Aset inputs are connected, and not to each other
	(possible since pre-existing Asets are carried forwards)
	then there is a conflict. */
	if (ff1->pin_Aset().is_linked()
		&& ff2->pin_Aset().is_linked()
		&& ! ff1->pin_Aset().is_linked(ff2->pin_Aset())) {
			cerr << ff2->get_line() << ": error: "
				<< "DFF Aset conflicts with "
				<< ff1->get_line() << "." << endl;
			return false;
	}

	if (ff1->pin_Aclr().is_linked()
		&& ff2->pin_Aclr().is_linked()
		&& ! ff1->pin_Aclr().is_linked(ff2->pin_Aclr())) {
			cerr << ff2->get_line() << ": error: "
				<< "DFF Aclr conflicts with "
				<< ff1->get_line() << "." << endl;
			return false;
	}

#if XXXX
	if (ff2->pin_Data(idx2).is_linked())
		connect(ff1->pin_Data(idx1), ff2->pin_Data(idx2));
	if (ff2->pin_Aset().is_linked())
		connect(ff1->pin_Aset(), ff2->pin_Aset());
	if (ff2->pin_Aclr().is_linked())
		connect(ff1->pin_Aclr(), ff2->pin_Aclr());
	if (ff2->pin_Sclr().is_linked())
		connect(ff1->pin_Sclr(),  ff2->pin_Sclr());
	if (ff2->pin_Sset().is_linked())
		connect(ff1->pin_Sset(),  ff2->pin_Sset());
	if (ff2->pin_Clock().is_linked())
		connect(ff1->pin_Clock(), ff2->pin_Clock());
#endif
	if (ff2->pin_Enable().is_linked())
		connect(ff1->pin_Enable(),ff2->pin_Enable());

	return true;
}

bool NetAssignBase::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events,NetNet* accum_in)
{
	unsigned count_lval = 0;
	NetAssign_*demux = 0;

	for (NetAssign_*cur = lval_ ;  cur ;  cur = cur->more) {
		if (cur->bmux())
			demux = cur;
		if (cur->mem())
			demux = cur;

		count_lval += 1;
	}

	if (demux != 0 && count_lval != 1) {
		cerr << get_line() << ": error: Cannot synthesize assignmnents"
			<< "that mix memory and vector assignments." << endl;
		return false;
	}

	/* There is no memory address, so resort to async
	assignments. */
	if (demux == 0) {
		if (debug_synth) {
			cerr << get_line() << ": debug: Looks like simple assign "
				<< "to a synchronous vector." << endl;
		}

		/* Synthesize the input to the DFF. */
		return synth_async_accum(des, scope, true, nex_ff,
			nex_map, nex_out, accum_in);
	}

	assert(demux->bmux() != 0);

	/* Obviously, we need the r-value synthesized to connect it up. */
	NetNet*rsig = rval_->synthesize(des);
	assert(rsig->pin_count() == lval_->lwidth());

	if (debug_synth) {
		cerr << get_line() << ": debug: Looks like an assign "
			<< "to an addressed memory word." << endl;
	}

	//ADU: find the FF refered to the memory
	NetMemory* mem = demux->mem();
	NetNet*msig = mem->explode_to_reg();

	NetFF* ff = 0;
	struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

	if (demux->mem() && dynamic_cast<NetEConst*>(demux->bmux())){
		NetEConst*ae = dynamic_cast<NetEConst*>(demux->bmux());
		long adr = ae->value().as_long();
		adr = mem->index_to_address(adr) * mem->width();

		for (unsigned idx = 0 ;  idx < nex_map->pin_count() ;  idx += 1) {
			unsigned off = adr+idx;
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				msig->pin(off).nexus());
			assert(tmp >= 0);
			unsigned ptr = tmp;
			nex_ff[ptr].ff->set_mem(mem);
			ff = nex_ff[ptr].ff;
		}
	}else{
		assert(msig->pin_count() == nex_map->pin_count());
		for (unsigned idx = 0 ;  idx < nex_map->pin_count() ;  idx += 1) {

			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				msig->pin(idx).nexus());
			assert(tmp >= 0);
			unsigned ptr = tmp;
			nex_ff[ptr].ff->set_mem(mem);
			ff = nex_ff[ptr].ff;
		}
	}

	delete[]nex_map_idx;

	//clk
	assert(ff->pin_Clock().is_linked());
	NetNet * clk = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
	connect(clk->pin(0), ff->pin_Clock());

	//waddr
	NetNet * waddr = demux->bmux()->synthesize(des);

	//we
	NetNet * we = 0;
	if(ff->pin_Enable().is_linked()){
		we = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
		connect(we->pin(0), ff->pin_Enable());
	}

	

	NetMemoryRef* mem_ref = new NetMemoryRef(mem, waddr, rsig, clk, we);

	mem->set_mem_ref(mem_ref);

	if (debug_synth) {
		cerr << get_line() << ": debug: Synchronous assign done." << endl;
	}

	return true;
}

/*
* This method is called when a block is encountered near the surface
* of a synchronous always statement. For example, this code will be
* invoked for input like this:
*
*     always @(posedge clk...) begin
*           <statement1>
*           <statement2>
*           ...
*     end
*
* This needs to be split into a DFF bank for each statement, because
* the statements may each infer different reset and enable signals.
*/
bool NetBlock::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events_in, NetNet* accum_in)
{
	if (last_ == 0) {
		return true;
	}


	const perm_string tmp1 = perm_string::literal("tmp1");
	const perm_string tmp2 = perm_string::literal("tmp2");
	const perm_string tmp3 = perm_string::literal("tmp3");

	NetNet*accum_out = new NetNet(scope, tmp3, NetNet::WIRE,
		nex_out->pin_count());
	accum_out->local_flag(true);

	/* Output that ultimately have not been driven should collect
	their value from the accumulated input. */
	assert(accum_out->pin_count() == accum_in->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		if (accum_in->pin(idx).nexus()->is_driven())
			connect(accum_out->pin(idx), accum_in->pin(idx));
	}

	bool flag = true;
	NetProc*cur = last_;
	do {
		NetNet*new_accum;

		cur = cur->next_;

		/* Create a temporary nex_map for the substatement. */
		NexusSet tmp_set;
		cur->nex_output(tmp_set);
		NetNet*tmp_map = new NetNet(scope, tmp1, NetNet::WIRE,
			tmp_set.count());
		for (unsigned idx = 0 ;  idx < tmp_map->pin_count() ;  idx += 1)
			connect(tmp_set[idx], tmp_map->pin(idx));

		/* Create also a temporary net_out to collect the
		output. */
		NetNet*tmp_out = new NetNet(scope, tmp2, NetNet::WIRE,
			tmp_set.count());
		tmp_out->set_line(*this);

		struct sync_accounting_cell*tmp_ff = 
			new struct sync_accounting_cell[tmp_out->pin_count()];//ADU:

		/* Make a temporary set of currently accumulated outputs
		that we can pass to the synth_async of the
		sub-statement. Some sub-statements will use this to
		handle default cases specially. We will delete this
		temporary map as soon as the synth_async is done. */
		new_accum = new NetNet(scope, tmp3, NetNet::WIRE, tmp_set.count());
		struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_set.count() ;  idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_set[idx]);
			assert(tmp >= 0);
			unsigned ptr = tmp;

			NetFF*ff2 = nex_ff[ptr].ff ;//ADU:
			tmp_ff[idx].ff = ff2;       //ADU:
			tmp_ff[idx].pin = 0;        //ADU:
			tmp_ff[idx].proc = cur;     //ADU:
			
			if (accum_out->pin(ptr).nexus()->is_driven())
				connect(new_accum->pin(idx), accum_out->pin(ptr));
		}

		delete [] nex_map_idx;

		//ADU: ��ʱ��alwaysģ����,
		//     �����nex_ff�а�����FF�ĸ�����tmp���ŵĸ������ܲ�һ��,����
		/*
		always@(posedge clk)
			if(en1)
				begin
				y1 <= a;
				y2 <= b;
				end
			else
				begin
				y1 <= c;

				if(en2)
					y2 <= d;
			end
		*/
		bool ok_flag = cur->synth_sync(des, scope,
			tmp_ff, tmp_map, tmp_out,
			events_in, new_accum);

		flag = flag && ok_flag;

		delete[]tmp_ff;//ADU:
		delete new_accum;

		/* NOTE: tmp_set is not valid after this point, since
		the cur->synth_async method may change nexa that it
		refers to. */

		if (ok_flag == false)
			continue;

		/* Now start building a new set of accumulated outputs
		that we will pass to the next statement of the block,
		or that will be the output of the block. */
		NetNet* new_accum_2 = new NetNet(scope, tmp3, NetNet::WIRE,
			nex_out->pin_count());
		new_accum_2->set_line(*this);
		new_accum_2->local_flag(true);

		/* Use the nex_map to link up the output from the
		substatement to the output of the block as a
		whole. */

		nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_out->pin_count() ; idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_map->pin(idx).nexus());
			if (tmp < 0) {
				cerr << cur->get_line() << ": internal error: "
					<< "Nexus isn't in nex_map?! idx=" << idx
					<< " map width = " << nex_map->pin_count()
					<< " tmp_map count = " << tmp_map->pin_count()
					<< endl;
			}
			assert(tmp >= 0);
			unsigned ptr = tmp;
			connect(new_accum_2->pin(ptr), tmp_out->pin(idx));
		}

		delete[]nex_map_idx;

		delete tmp_map;
		delete tmp_out;

		assert(new_accum_2->pin_count() == accum_out->pin_count());

		/* Anything that is not redriven by the current
		statement inherits the value that was driven from any
		previous statements. Thus, the current statement can
		*override* the outputs of any previous statements. */
		for (unsigned idx = 0;  idx < new_accum_2->pin_count();  idx += 1) {

			if (new_accum_2->pin(idx).nexus()->is_driven())
				continue;

			connect(new_accum_2->pin(idx), accum_out->pin(idx));
		}
		delete accum_out;
		accum_out = new_accum_2;

	} while (cur != last_);


	/* Now bind the accumulated output values to the nex_out
	passed in. Note that each previous step has already did the
	pin mapping, so just connect. */
	assert(nex_out->pin_count() == accum_out->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		connect(nex_out->pin(idx), accum_out->pin(idx));
	}

	delete accum_out;

	return flag;
}


bool NetBlock::synth_sync(Design*des, NetScope*scope,
	struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events_in,
	NetNet* control_net,
	NetNet* zero_one_aload_net,
	NetNet* accum_in)
{
	if (last_ == 0) {
		return true;
	}


	const perm_string tmp1 = perm_string::literal("tmp1");
	const perm_string tmp2 = perm_string::literal("tmp2");
	const perm_string tmp3 = perm_string::literal("tmp3");

	NetNet*accum_out = new NetNet(scope, tmp3, NetNet::WIRE,
		nex_out->pin_count());
	accum_out->local_flag(true);

	/* Output that ultimately have not been driven should collect
	their value from the accumulated input. */
	assert(accum_out->pin_count() == accum_in->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		if (accum_in->pin(idx).nexus()->is_driven())
			connect(accum_out->pin(idx), accum_in->pin(idx));
	}

	bool flag = true;
	NetProc*cur = last_;
	do {
		NetNet*new_accum;

		cur = cur->next_;

		/* Create a temporary nex_map for the substatement. */
		NexusSet tmp_set;
		cur->nex_output(tmp_set);
		NetNet*tmp_map = new NetNet(scope, tmp1, NetNet::WIRE,
			tmp_set.count());
		for (unsigned idx = 0 ;  idx < tmp_map->pin_count() ;  idx += 1)
			connect(tmp_set[idx], tmp_map->pin(idx));

		/* Create also a temporary net_out to collect the
		output. */
		NetNet*tmp_out = new NetNet(scope, tmp2, NetNet::WIRE,
			tmp_set.count());
		tmp_out->set_line(*this);

		struct sync_accounting_cell*tmp_ff = 
			new struct sync_accounting_cell[tmp_out->pin_count()];//ADU:

		/* Make a temporary set of currently accumulated outputs
		that we can pass to the synth_async of the
		sub-statement. Some sub-statements will use this to
		handle default cases specially. We will delete this
		temporary map as soon as the synth_async is done. */
		new_accum = new NetNet(scope, tmp3, NetNet::WIRE, tmp_set.count());
		struct nexus_map_t*nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_set.count() ;  idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_set[idx]);
			assert(tmp >= 0);
			unsigned ptr = tmp;

			NetFF*ff2 = nex_ff[ptr].ff ;//ADU:
			tmp_ff[idx].ff = ff2;       //ADU:
			tmp_ff[idx].pin = 0;        //ADU:
			tmp_ff[idx].proc = cur;     //ADU:
			
			if (accum_out->pin(ptr).nexus()->is_driven())
				connect(new_accum->pin(idx), accum_out->pin(ptr));
		}

		delete [] nex_map_idx;

		//ADU: ��ʱ��alwaysģ����,
		//     �����nex_ff�а�����FF�ĸ�����tmp���ŵĸ������ܲ�һ��,����
		/*
		always@(posedge clk)
			if(en1)
				begin
				y1 <= a;
				y2 <= b;
				end
			else
				begin
				y1 <= c;

				if(en2)
					y2 <= d;
			end
		*/
		bool ok_flag = cur->synth_sync(des, scope,
			tmp_ff, tmp_map, tmp_out,
			events_in, control_net,
			zero_one_aload_net, new_accum);

		flag = flag && ok_flag;

		delete[]tmp_ff;//ADU:
		delete new_accum;

		/* NOTE: tmp_set is not valid after this point, since
		the cur->synth_async method may change nexa that it
		refers to. */

		if (ok_flag == false)
			continue;

		/* Now start building a new set of accumulated outputs
		that we will pass to the next statement of the block,
		or that will be the output of the block. */
		NetNet* new_accum_2 = new NetNet(scope, tmp3, NetNet::WIRE,
			nex_out->pin_count());
		new_accum_2->set_line(*this);
		new_accum_2->local_flag(true);

		/* Use the nex_map to link up the output from the
		substatement to the output of the block as a
		whole. */

		nex_map_idx = make_nexus_index(nex_map);

		for (unsigned idx = 0 ;  idx < tmp_out->pin_count() ; idx += 1) {
			int tmp = map_nexus_in_index(nex_map_idx,
				nex_map->pin_count(),
				tmp_map->pin(idx).nexus());
			if (tmp < 0) {
				cerr << cur->get_line() << ": internal error: "
					<< "Nexus isn't in nex_map?! idx=" << idx
					<< " map width = " << nex_map->pin_count()
					<< " tmp_map count = " << tmp_map->pin_count()
					<< endl;
			}
			assert(tmp >= 0);
			unsigned ptr = tmp;
			connect(new_accum_2->pin(ptr), tmp_out->pin(idx));
		}

		delete[]nex_map_idx;

		delete tmp_map;
		delete tmp_out;

		assert(new_accum_2->pin_count() == accum_out->pin_count());

		/* Anything that is not redriven by the current
		statement inherits the value that was driven from any
		previous statements. Thus, the current statement can
		*override* the outputs of any previous statements. */
		for (unsigned idx = 0;  idx < new_accum_2->pin_count();  idx += 1) {

			if (new_accum_2->pin(idx).nexus()->is_driven())
				continue;

			connect(new_accum_2->pin(idx), accum_out->pin(idx));
		}
		delete accum_out;
		accum_out = new_accum_2;

	} while (cur != last_);


	/* Now bind the accumulated output values to the nex_out
	passed in. Note that each previous step has already did the
	pin mapping, so just connect. */
	assert(nex_out->pin_count() == accum_out->pin_count());
	for (unsigned idx = 0 ;  idx < accum_out->pin_count() ;  idx += 1) {
		connect(nex_out->pin(idx), accum_out->pin(idx));
	}

	delete accum_out;

	return flag;
}

static bool test_ff_set_clr(struct sync_accounting_cell*nex_ff, unsigned bits)
{
	for (unsigned idx = 0 ;  idx < bits ;  idx += 1) {
		NetFF*ff = nex_ff[idx].ff;
		if (ff->pin_Sset().is_linked())
			return true;
		if (ff->pin_Sclr().is_linked())
			return true;
	}

	return false;
}

int NetCondit::connect_set_clr_range_(struct sync_accounting_cell*nex_ff,
	unsigned bits, NetNet*rst,
	const verinum&val)
{
	/* Oops, this is not a constant, presumably because the case
	is not fully connected. In this case, we need to fall back
	on more general synthesis. */
	if (! val.is_defined()) {
		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "Give up on set/clr synthesis, since "
			<< "r-value = " << val << endl;
		return -1;
	}

	assert(val.is_defined());

	unsigned base = 0;
	unsigned wid = nex_ff[0].ff->width();
	while (base < bits) {
		NetFF*ff = nex_ff[base].ff;
		assert(base+wid <= bits);

		verinum tmp(0UL, wid);
		for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
			assert(nex_ff[base+idx].ff == ff);
			tmp.set(idx, val.get(base+idx));
		}

		if (tmp.is_zero()) {
			connect(ff->pin_Sclr(), rst->pin(0));

		} else {
			connect(ff->pin_Sset(), rst->pin(0));
			ff->sset_value(tmp);
		}
		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "Create a synchronous set for "
			<< ff->width() << " bit ff." << endl;

		base += wid;
	}

	return 0;
}

int NetCondit::connect_enable(Design*des, NetScope*scope,
	NetFF* ff, NetNet*ce)
{
	if (ff->pin_Enable().is_linked()) {
		NetLogic*ce_and = new NetLogic(scope,
			scope->local_symbol(), 3,
			NetLogic::AND);
		des->add_node(ce_and);
		connect(ff->pin_Enable(), ce_and->pin(1));
		connect(ce->pin(0), ce_and->pin(2));

		ff->pin_Enable().unlink();
		connect(ff->pin_Enable(), ce_and->pin(0));

		NetNet*tmp = new NetNet(scope, scope->local_symbol(),
			NetNet::IMPLICIT, 1);
		tmp->local_flag(true);
		connect(ff->pin_Enable(), tmp->pin(0));

	} else {

		connect(ff->pin_Enable(), ce->pin(0));
	}
	return 0;
}

/*
* This method handles the case where I find a conditional near the
* surface of a synchronous thread. This conditional can be a CE or an
* asynchronous set/reset, depending on whether the pin of the
* expression is connected to an event, or not.
*/
bool NetCondit::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events_in,NetNet* accum_in)
{
	if (debug_synth) {  
		cerr << get_line() << ": debug: "
			<< "Start sync synthesis of conditional" << endl;
	}


	if (debug_synth) {
		cerr << get_line() << ": debug: "
			<< "Condit expression input not sensitive, "
			<< "so must be synchronous. " << endl;
	}

	assert(!events_in.count());//ADU:

	/* Detect the case that this is a *synchronous* set/reset. It
	is not asyncronous because we know the condition is not
	included in the sensitivity list, but if the if_ case is
	constant (has no inputs) then we can model this as a
	synchronous set/reset.

	This is only synchronous set/reset if there is a true and a
	false clause, and no inputs. The "no inputs" requirement is
	met if the assignments are of all constant values.

	Also, we will not allow both Sset and Sclr to be used on a
	single LPM_FF (due to unclear priority issues) so don't try
	if either are already connected.

	XXXX This should be disabled if there is a memory involved
	in any sub-statements? */

	//ͬ��������λ�źŵ�����
	NexusSet*a_set = if_? if_->nex_input() : 0;

	if (a_set && (a_set->count() == 0)
		&& if_ && else_
		&& !test_ff_set_clr(nex_ff, nex_map->pin_count())) {

			NetNet*rst = expr_->synthesize(des);
			assert(rst->pin_count() == 1);

			/* Synthesize the true clause to figure out what
			kind of set/reset we have. */
			NetNet*asig = new NetNet(scope, scope->local_symbol(),
				NetNet::WIRE, nex_map->pin_count());
			asig->local_flag(true);
			bool flag = if_->synth_async_accum(des, scope, true, nex_ff,
				nex_map, asig, accum_in);

			if (!flag) {
				/* This path leads nowhere */
				delete asig;
			} else do {
				assert(asig->pin_count() == nex_map->pin_count());

				unsigned nbits = nex_map->pin_count();

				/* Collect the set/reset value into a verinum. If
				this turns out to be entirely 0 values, then
				use the Sclr input. Otherwise, use the Aset
				input and save the set value. */
				verinum tmp (verinum::V0, nbits);
				for (unsigned bit = 0; bit< nbits; bit += 1) {
					if(!asig->pin(bit).nexus()->drivers_constant()){
						cout<<"errot"<<endl;
					}
					assert(asig->pin(bit).nexus()->drivers_constant());
					tmp.set(bit, asig->pin(bit).nexus()->driven_value());
				}

				if (connect_set_clr_range_(nex_ff, nbits, rst, tmp) < 0) {
					delete asig;
					break;
				}


				delete a_set;

				assert(else_ != 0);
				flag = else_->synth_sync(des, scope,
					nex_ff, nex_map, nex_out,
					svector<NetEvProbe*>(0), accum_in)
					&& flag;

				if (debug_synth)
					cerr << get_line() << ": debug: "
					<< "End synthesis of conditional" << endl;
				return flag;
			} while (0);
	}

	delete a_set;
	
	/* If this is an if/then/else, then it is likely a
	combinational if, and I should synthesize it that way. */
	//һ��������䣨if_else_�����ۺϣ����ۺϳ�ѡͨ��
	if (if_ && else_) {
		if (debug_synth) {
			cerr << get_line() << ": debug: "
				<< "Condit expression looks like a synchronous mux."
				<< endl;
		}

		bool flag =synth_async_accum(des, scope, true, nex_ff,
			nex_map, nex_out, accum_in);

		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "End synthesis of conditional" << endl;
		return flag;
	}

	/* At this point, all that's left are synchronous enables and
	synchronous disables. These are cases where only one of the
	if_ and else_ clauses is given. */
	assert( if_ && !else_);//ADU:
	/*
	always@(posedge clk)
		begin
		y1 <= a;
		if(en)
			y1 <= b;
	end
	*/
	if(accum_in->at_least_one_pin_linked_to_signi_signal()){
		bool flag =synth_async_accum(des, scope, true, nex_ff,
			nex_map, nex_out, accum_in);

		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "End synthesis of conditional" << endl;
		return flag;
	}
		
	if (debug_synth) {
		cerr << get_line() << ": debug: "
			<< "Condit expression looks like a synchronous "
			<< (if_? "enable" : "disable") << ". " << endl;
	}

	NetProc*clause = if_? if_ : else_;
	assert(clause);

	/* If this is a conditional disable, then turn it to an enable
	by putting a NOT in front of the disable expression. */
	NetExpr*condit = expr_;
	if (else_ != 0) {
		assert(if_ == 0);
		condit = new NetEUReduce('!', condit);
		condit->set_line(*expr_);
	}

	/* Synthesize the enable expression. */
	NetNet*ce = condit->synthesize(des);
	assert(ce->pin_count() == 1);

	/* What's left, is a synchronous CE statement like this:

	if (expr_) <true statement>;

	The expr_ expression has already been synthesized to the ce
	net, so we connect it here to the FF. What's left is to
	synthesize the substatement as a combinational
	statement. */

	unsigned nbits = nex_map->pin_count();
	for (unsigned bit = 0 ;  bit < nbits ;  bit += 1){
		NetFF*ff = nex_ff[bit].ff;
		connect_enable(des, scope, ff, ce);
	}


	if (debug_synth) {
		cerr << get_line() << ": debug: "
			<< "Condit expression make input that is sync enabled."
			<< endl;
	}

	bool flag = clause->synth_sync(des, scope,
		nex_ff, nex_map, nex_out,
		events_in, accum_in);


	if (debug_synth)
		cerr << get_line() << ": debug: "
		<< "End synthesis of conditional" << endl;
	return flag;
}

bool NetCondit::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events_in,
	NetNet* control_net, 
	NetNet* zero_one_aload_net,
	NetNet* accum_in)
{
	if (debug_synth) {  
		cerr << get_line() << ": debug: "
			<< "Start sync synthesis of conditional" << endl;
	}

	/* First try to turn the condition expression into an
	asynchronous set/reset. If the condition expression has
	inputs that are included in the sensitivity list, then it
	is likely intended as an asynchronous input. */

	NexusSet*expr_input = expr_->nex_input();
	assert(expr_input);

	assert(events_in.count());//ADU:
	for (unsigned idx = 0 ;  idx < events_in.count() ;  idx += 1) {


		NetEvProbe*ev = events_in[idx];
		NexusSet pin_set;
		pin_set.add(ev->pin(0).nexus());

		if (! expr_input->contains(pin_set))
			continue;

		/* If we are taking this to be an asynchronous
		set/clear, then *all* the condition expression inputs
		must be asynchronous. Check that here. */
		if (! pin_set.contains(*expr_input)) {

			NexusSet tmp_set;
			tmp_set.add(ev->pin(0).nexus());
			for (unsigned tmp = idx+1; tmp<events_in.count(); tmp += 1) {
				NetEvProbe*ev_tmp = events_in[tmp];
				tmp_set.add(ev_tmp->pin(0).nexus());
			}

			if (! tmp_set.contains(*expr_input)) {
				cerr << get_line() << ": error: Condition expression "
					<< "mixes synchronous and asynchronous "
					<< "inputs." << endl;
				des->errors += 1;
			}
		}

		/* Ah, this edge is in the sensitivity list for the
		expression, so we have an asynchronous
		input. Synthesize the set/reset input expression. */

		NetNet*rst = expr_->synthesize(des);
		assert(rst->pin_count() == 1);

		/* WARNING: This case relies on the ff vector being a
		single NetFF. */
		// for (unsigned bit = 1 ;  bit < nex_out->pin_count() ; bit += 1) {
		//assert(nex_ff[0].ff == nex_ff[bit].ff);
		// }
		//NetFF*ff = nex_ff[0].ff;

		/* XXXX I really should find a way to check that the
		edge used on the reset input is correct. This would
		involve interpreting the exression that is fed by the
		reset expression. */
		//assert(ev->edge() == NetEvProbe::POSEDGE);

		/* Synthesize the true clause to figure out what
		kind of set/reset we have. */
		NetNet*asig = new NetNet(scope, scope->local_symbol(),
			NetNet::WIRE, nex_map->pin_count());
		asig->local_flag(true);

		assert(if_ != 0);
		bool flag = if_->synth_async_accum(des, scope, true, nex_ff,
			nex_map, asig, accum_in);

		assert(asig->pin_count() == nex_map->pin_count());

		delete expr_input;		

		/* Create a new NetEvProbe list that does not include
		the current probe that we've absorbed into this
		input. */
		assert(events_in.count() >= 1);
		svector<NetEvProbe*> events_tmp (events_in.count() - 1);
		unsigned count_events = 0;
		for (unsigned tmp = 0 ;  tmp < events_in.count() ;  tmp += 1) {
			if (tmp == idx) continue;
			events_tmp[count_events++] = events_in[tmp];
		}

		NetLogic* contr_OR = new NetLogic(scope, scope->local_symbol(),	3, NetLogic::OR);
		des->add_node(contr_OR);

		connect(rst->pin(0), contr_OR->pin(1));
		connect(contr_OR->pin(0), control_net->pin(0));

		NetMux*mux = new NetMux(scope, scope->local_symbol(), nex_map->pin_count(), 2, 1);
		des->add_node(mux);
		connect(rst->pin(0), mux->pin_Sel(0));
		//assert(zero_one_aload_net->pin_count() == nex_map->pin_count());
		if(zero_one_aload_net->pin_count() != nex_map->pin_count()){
			cerr << get_line()<<": error: syntax is not support in current version." << endl;
			return false;
			//assert(zero_one_aload_net->pin_count() == nex_map->pin_count());
		}
		for(unsigned i = 0; i < nex_map->pin_count(); i++){
			connect(mux->pin_Result(i), zero_one_aload_net->pin(i));

			if(asig->pin(i).is_linkedToSignificantSignal())
				connect(asig->pin(i), mux->pin_Data(i,1));
			else
				connect(mux->pin_Data(i,1), nex_map->pin(i));
		}


		if(events_tmp.count()){
			assert(else_);
			NetNet* tmp_control_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
			NetNet* tmp_zero_one_aload_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, nex_map->pin_count());
			connect(tmp_control_net->pin(0), contr_OR->pin(2));
			for(unsigned i = 0; i < nex_map->pin_count(); i++){
				connect(mux->pin_Data(i,0), tmp_zero_one_aload_net->pin(i));
			}

			flag = flag && else_->synth_sync(des, scope,
				nex_ff, nex_map,
				nex_out, events_tmp, tmp_control_net, 
				tmp_zero_one_aload_net,accum_in);
		}else{
			NetConst*zero = new NetConst(scope, scope->local_symbol(), verinum::V0);
			zero->set_line(*this);
			des->add_node(zero);

			NetNet*zsig = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
			zsig->set_line(*this);
			connect(zsig->pin(0), zero->pin(0));
			connect(zsig->pin(0), contr_OR->pin(2));//connect to zero
			for(unsigned i = 0; i < nex_map->pin_count(); i++){
				connect(mux->pin_Data(i,0), zsig->pin(0));
			}

			if (else_) {
				flag = flag && else_->synth_sync(des, scope,
					nex_ff, nex_map,
					nex_out, events_tmp, accum_in);
			
			}else{
				//ADU: Connect D with Q in the same FF.
				//If it is above case, then the value do not have the matter with clk,
				//so in the Func: "FF_D_connected_Q", we will replace the FFs with latchs,
				//which have the asynchronous control signal.
				cout << "Warning: it may connect D with Q in the same FF" << endl;
				for(unsigned i = 0; i < nex_map->pin_count(); i++){
					connect(nex_map->pin(i), nex_out->pin(i));
				}
			}
		}		

		if (debug_synth)
			cerr << get_line() << ": debug: "
			<< "End synthesis of conditional" << endl;
		return flag;
	}

	return false;

}

bool NetEvWait::synth_sync(Design*des, NetScope*scope,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out,
	const svector<NetEvProbe*>&events_in, NetNet* accum_in)
{
	if (events_in.count() > 0) {
		cerr << get_line() << ": error: Events are unaccounted"
			<< " for in process synthesis. (evw)" << endl;
		des->errors += 1;
	}

	assert(events_in.count() == 0);

	if (debug_synth) {
		cerr << get_line() << ": debug: Start synthesis of event wait statement."
			<< endl;
	}

	/* This can't be other then one unless there are named events,
	which I cannot synthesize. */
	assert(nevents_ == 1);
	NetEvent*ev = events_[0];

	assert(ev->nprobe() >= 1);
	svector<NetEvProbe*>events (ev->nprobe() - 1);

	/* Get the input set from the substatement. This will be used
	to figure out which of the probes in the clock. */
	NexusSet*statement_input = statement_ -> nex_input();

	/* Search for a clock input. The clock input is the edge event
	that is not also an input to the substatement. */
	NetEvProbe*pclk = 0;

	unsigned event_idx = 0;
	for (unsigned idx = 0 ;  idx < ev->nprobe() ;  idx += 1) {
		NetEvProbe*tmp = ev->probe(idx);
		assert(tmp->pin_count() == 1);

		NexusSet tmp_nex;
		tmp_nex .add( tmp->pin(0).nexus() );

		if (! statement_input ->contains(tmp_nex)) {
			if (pclk != 0) {
				cerr << get_line() << ": error: Too many "
					<< "clocks for synchronous logic." << endl;
				cerr << get_line() << ":      : Perhaps an"
					<< " asynchronous set/reset is misused?" << endl;
				des->errors += 1;
			}
			pclk = tmp;

		} else {
			events[event_idx++] = tmp;
		}
	}

	if (pclk == 0) {
		cerr << get_line() << ": error: None of the edges"
			<< " are valid clock inputs." << endl;
		cerr << get_line() << ":      : Perhaps the clock"
			<< " is read by a statement or expression?" << endl;
		des->errors += 1;
		return false;
	}


	// unsigned base = 0;
	//  while (base < nex_map->pin_count()) {
	// unsigned wid = nex_ff[base].ff->width();
	// assert((base + wid) <= nex_map->pin_count());

	// NetFF*ff = nex_ff[base].ff;
	// connect(ff->pin_Clock(), pclk->pin(0));
	// if (pclk->edge() == NetEvProbe::NEGEDGE)
	//ff->attribute(perm_string::literal("ivl:clock_polarity"),
	//verinum("INVERT"));

	// base += wid;
	//  }
	NetNet* clk2 = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
	connect( clk2->pin(0), pclk->pin(0));
	if (pclk->edge() == NetEvProbe::NEGEDGE){
		NetLogic* gate = new NetLogic(scope, scope->local_symbol(),	2, NetLogic::NOT);
		des->add_node(gate);
		connect( gate->pin(1), pclk->pin(0) );
		
		clk2->pin(0).unlink();
		connect( clk2->pin(0), gate->pin(0));
		
	}
	for (unsigned idx = 0 ;  idx < nex_map->pin_count() ;  idx += 1){
		NetFF*ff = nex_ff[idx].ff;
		connect(ff->pin_Clock(), clk2->pin(0));
		//if (pclk->edge() == NetEvProbe::NEGEDGE){
		//	ff->attribute(perm_string::literal("ivl:clock_polarity"), verinum("INVERT"));
		//}
	}


	

	/* Synthesize the input to the DFF. */
	bool flag = false;
	if(events.count()){
		//ADU: Generate the asynchronous control logics
		NetNet* control_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
		NetNet*zero_one_aload_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, nex_map->pin_count());
		//Aset AND logic
		for(unsigned idx = 0; idx < nex_map->pin_count(); idx ++){
			assert(!nex_ff[idx].ff->pin_Aset().is_linked());
			assert(!nex_ff[idx].ff->pin_Aclr().is_linked());
			NetLogic*Aset_AND = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			des->add_node(Aset_AND);

			NetNet* Aset_AND_out_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
			connect(Aset_AND_out_net->pin(0), Aset_AND->pin(0));
			connect(Aset_AND_out_net->pin(0), nex_ff[idx].ff->pin_Aset());

			connect(control_net->pin(0), Aset_AND->pin(1));//control signal
			connect(zero_one_aload_net->pin(idx), Aset_AND->pin(2));
		}
		//Aclr AND logic

		for(unsigned idx = 0; idx < nex_map->pin_count(); idx ++){
			NetLogic*Aclr_AND = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			des->add_node(Aclr_AND);

			NetNet* Aclr_AND_out_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
			connect(Aclr_AND_out_net->pin(0), Aclr_AND->pin(0));
			connect(Aclr_AND_out_net->pin(0), nex_ff[idx].ff->pin_Aclr());

			NetLogic*Aclr_NOT = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
			des->add_node(Aclr_NOT);

			NetNet* Aclr_NOT_out_net = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1U);
			connect(Aclr_NOT_out_net->pin(0), Aclr_NOT->pin(0));
			connect(Aclr_NOT_out_net->pin(0), Aclr_AND->pin(2));

			connect(control_net->pin(0), Aclr_AND->pin(1));//control signal
			connect(zero_one_aload_net->pin(idx), Aclr_NOT->pin(1));
		}

		flag = statement_->synth_sync(des, scope, nex_ff,
			nex_map, nex_out, events, control_net, zero_one_aload_net, accum_in);
	}else{
		flag = statement_->synth_sync(des, scope, nex_ff,
			nex_map, nex_out, events, accum_in);
	}

	if (debug_synth) {
		cerr << get_line() << ": debug: Finished synthesis of event wait statement."
			<< endl;
	}

	return flag;
}

bool NetWhile::synth_async(Design*des, NetScope*scope, bool sync_flag,
struct sync_accounting_cell*nex_ff,
	NetNet*nex_map, NetNet*nex_out, NetNet*accum_in)
{
	cerr << get_line()
		<< ": error: Cannot synthesize for or while loops."
		<< endl;
	des->errors += 1;
	return false;
}

bool NetProcTop::synth_sync(Design*des)
{
	if (debug_synth) {
		cerr << get_line() << ": debug: Start synthesis of process." << endl;
	}

	NexusSet nex_set;
	statement_->nex_output(nex_set);

	if (debug_synth) {
		cerr << get_line() << ": debug: Process seems to have "
			<< nex_set.count() << " output bits." << endl;
	}

	//NetFF*ff = new NetFF(scope(), scope()->local_symbol(),
	//nex_set.count());
	//des->add_node(ff);
	//ff->attribute(perm_string::literal("LPM_FFType"), verinum("DFF"));
	//unsigned process_pin_count = ff->width();

	NetFF*ff=0;

	struct sync_accounting_cell*nex_ff
		= new struct sync_accounting_cell[nex_set.count()];
	for (unsigned idx = 0 ;  idx < nex_set.count() ;  idx += 1) {
		ff = new NetFF(scope(), scope()->local_symbol(), 1U);
		des->add_node(ff);
		ff->attribute(perm_string::literal("LPM_FFType"), verinum("DFF"));

		nex_ff[idx].ff = ff;
		nex_ff[idx].pin = 0;
		nex_ff[idx].proc = statement_;
	}

	/* The D inputs to the DFF device will receive the output from
	the statements of the process. */
	NetNet*nex_d = new NetNet(scope(), scope()->local_symbol(),
		NetNet::WIRE, nex_set.count());
	nex_d->local_flag(true);
	for (unsigned idx = 0 ;  idx < nex_set.count() ;  idx += 1) {

		ff = nex_ff[idx].ff ;
		connect(nex_d->pin(idx), ff->pin_Data(0));
	}

	/* The Q outputs of the DFF will connect to the actual outputs
	of the process. Thus, the DFF will be between the outputs
	of the process and the outputs of the substatement. */
	const perm_string tmpq = perm_string::literal("tmpq");
	NetNet*nex_q = new NetNet(scope(), tmpq, NetNet::WIRE,
		nex_set.count());
	for (unsigned idx = 0 ;  idx < nex_set.count() ;  idx += 1) {

		ff = nex_ff[idx].ff;
		connect(nex_set[idx], nex_q->pin(idx));
		connect(nex_q->pin(idx), ff->pin_Q(0));
	}

	/* Synthesize the input to the DFF. */
	NetNet* accum_in = new NetNet(scope(), scope()->local_symbol(),
		NetNet::WIRE, nex_set.count());

	bool flag = statement_->synth_sync(des, scope(),
		nex_ff,
		nex_q, nex_d,
		svector<NetEvProbe*>(), accum_in);

	//check the ffs is mem or not, if so ,delete it.
	for (unsigned idx = 0; idx < nex_set.count(); idx++){
		if(nex_ff[idx].ff->get_mem())
			delete nex_ff[idx].ff;
	}

	delete nex_q;
	delete[]nex_ff;

	
	

	if (debug_synth) {
		cerr << get_line() << ": debug: Finished synthesis of process." << endl;
	}

	return flag;
}

class synth2_f  : public functor_t {

public:
	void process(class Design*, class NetProcTop*);

private:
};


/*
* Look at a process. If it is asynchronous, then synthesize it as an
* asynchronous process and delete the process itself for its gates.
*/
void synth2_f::process(class Design*des, class NetProcTop*top)
{
	if (top->attribute(perm_string::literal("ivl_synthesis_off")).as_ulong() != 0)
		return;

	/* If the scope that contains this process as a cell attribute
	attached to it, then skip synthesis. */
	if (top->scope()->attribute(perm_string::literal("ivl_synthesis_cell")).len() > 0)
		return;

	if (top->is_synchronous()) do {
		bool flag = top->synth_sync(des);
		if (! flag) {
			cerr << top->get_line() << ": error: "
				<< "Unable to synthesize synchronous process." << endl;
			des->errors += 1;
			return;
		}
		des->delete_process(top);
		return;
	} while (0);

	if (! top->is_asynchronous()) {
		bool synth_error_flag = false;
		if (top->attribute(perm_string::literal("ivl_combinational")).as_ulong() != 0) {
			cerr << top->get_line() << ": error: "
				<< "Process is marked combinational,"
				<< " but isn't really." << endl;
			des->errors += 1;
			synth_error_flag = true;
		}

		if (top->attribute(perm_string::literal("ivl_synthesis_on")).as_ulong() != 0) {
			cerr << top->get_line() << ": error: "
				<< "Process is marked for synthesis,"
				<< " but I can't do it." << endl;
			des->errors += 1;
			synth_error_flag = true;
		}

		if (! synth_error_flag) {
			cerr << top->get_line() << ": warning: "
				<< "Process not synthesized." << endl;
		}
		return;
	}

	if (! top->synth_async(des)) {
		cerr << top->get_line() << ": error: "
			<< "Asynchronous process cannot be synthesized." << endl;
		des->errors += 1;
		return;
	}

	des->delete_process(top);
}

void synth2(Design*des)
{
	debug_synth2 = atoi(des->get_flag("ivl-synth2-debug"));
	synth2_f synth_obj;
	des->functor(&synth_obj);
}

/*
* $Log: synth2.cc,v $
* Revision 1.39.2.48  2007/05/30 17:48:54  steve
*  Support Latch synthesis (Alan Feldstein)
*
* Revision 1.39.2.47  2006/11/11 23:10:20  steve
*  Fix async blocks to take accumulated input.
*
* Revision 1.39.2.46  2006/11/02 02:13:15  steve
*  Error message for condit expression not synthesized.
*
* Revision 1.39.2.45  2006/10/30 02:03:30  steve
*  Detect and synthesize enables using condit false.
*
* Revision 1.39.2.44  2006/08/23 04:08:19  steve
*  Fix missing sig on certain mux outputs.
*  Ignore condit statements (for synthesis) with no outputs.
*
* Revision 1.39.2.43  2006/08/22 04:22:45  steve
*  Add synthesis for casez statements.
*
* Revision 1.39.2.42  2006/08/15 03:41:25  steve
*  Improve performance of unlink of heavily connected nexa.
*
* Revision 1.39.2.41  2006/08/08 02:17:49  steve
*  Improved nexus management performance.
*
* Revision 1.39.2.40  2006/07/23 19:42:34  steve
*  Handle statement output override better in blocks.
*
* Revision 1.39.2.39  2006/07/10 00:21:54  steve
*  Add support for full_case attribute.
*
* Revision 1.39.2.38  2006/07/02 00:50:15  steve
*  Properly synthesize casex statements.
*
* Revision 1.39.2.37  2006/06/26 00:05:46  steve
*  Handle case where case output appears to be internal.
*
* Revision 1.39.2.36  2006/06/23 03:49:47  steve
*  synthesis of NetCondit handles partial resets.
*
* Revision 1.39.2.35  2006/06/07 03:17:39  steve
*  Fix partial use of NetMux in sync condit statements.
*
* Revision 1.39.2.34  2006/06/02 23:42:48  steve
*  Compilation warnings.
*
* Revision 1.39.2.33  2006/06/01 03:01:48  steve
*  Handle condit clauses with unassigned outputs.
*
* Revision 1.39.2.32  2006/05/20 16:06:48  steve
*  Replace assertions with error messages.
*
* Revision 1.39.2.31  2006/05/18 01:47:12  steve
*  Fix synthesis of l-value bit select in block.
*
* Revision 1.39.2.30  2006/05/05 01:56:36  steve
*  Handle memory assignments out of range during synthesis
*
* Revision 1.39.2.29  2006/04/16 19:26:39  steve
*  Fix handling of exploded memories with partial or missing resets.
*
* Revision 1.39.2.28  2006/04/10 03:43:40  steve
*  Exploded memories accessed by constant indices.
*
* Revision 1.39.2.27  2006/04/01 01:37:58  steve
*  Punt on set/reset if some sources are unconnected.
*
* Revision 1.39.2.26  2006/03/26 23:09:24  steve
*  Handle asynchronous demux/bit replacements.
*
* Revision 1.39.2.25  2006/03/18 18:43:22  steve
*  Better error messages when synthesis fails.
*
* Revision 1.39.2.24  2006/03/16 05:39:43  steve
*  Fix a spelling error in an error message.
*
* Revision 1.39.2.23  2006/03/12 07:34:18  steve
*  Fix the memsynth1 case.
*
* Revision 1.39.2.22  2006/02/25 05:03:29  steve
*  Add support for negedge FFs by using attributes.
*
* Revision 1.39.2.21  2006/02/19 00:11:33  steve
*  Handle synthesis of FF vectors with l-value decoder.
*
* Revision 1.39.2.20  2006/01/27 01:58:53  steve
*  Document how the default statement handling works.
*
* Revision 1.39.2.19  2006/01/22 00:13:59  steve
*  Fix pin_Sel overrun.
*
* Revision 1.39.2.18  2006/01/21 21:42:32  steve
*  When mux has wide select but sparse choices, use 1hot translation.
*
* Revision 1.39.2.17  2006/01/18 06:16:11  steve
*  Restrict DFF to only one of Sset and Sclr.
*
* Revision 1.39.2.16  2006/01/18 01:23:25  steve
*  Rework l-value handling to allow for more l-value type flexibility.
*
* Revision 1.39.2.15  2006/01/01 02:25:07  steve
*  Case statement handles partial outputs.
*
* Revision 1.39.2.14  2006/01/01 01:30:39  steve
*  Allow for implicit case default in synchronous processes.
*
* Revision 1.39.2.13  2005/12/31 04:28:15  steve
*  Fix crashes caused bu synthesis of sqrt32.v.
*
* Revision 1.39.2.12  2005/12/19 01:13:47  steve
*  Handle DFF definitions spread out within a block.
*
* Revision 1.39.2.11  2005/12/15 02:38:51  steve
*  Fix missing outputs from synchronous conditionals to get out from in.
*
* Revision 1.39.2.10  2005/12/14 01:53:39  steve
*  Handle both asynchronous set and reset.
*
* Revision 1.39.2.9  2005/12/14 00:54:30  steve
*  Account for sync vs async muxes.
*
* Revision 1.39.2.8  2005/12/10 04:26:32  steve
*  Handle concatenations in l-values.
*
* Revision 1.39.2.7  2005/12/10 03:30:51  steve
*  Fix crash on block with assignments that assign lval to self.
*
* Revision 1.39.2.6  2005/12/07 02:14:37  steve
*  Error messages for missing else clauses.
*
* Revision 1.39.2.5  2005/11/16 00:38:26  steve
*  Handle partial sets of conditional clauses.
*
* Revision 1.39.2.4  2005/11/13 22:28:48  steve
*  Allow for block output to be set throughout the statements.
*
* Revision 1.39.2.3  2005/09/11 02:56:38  steve
*  Attach line numbers to NetMux devices.
*
* Revision 1.39.2.2  2005/08/22 01:00:42  steve
*  Add support for implicit defaults in case and conditions.
*
* Revision 1.39.2.1  2005/08/21 22:49:54  steve
*  Handle statements in blocks overriding previous statement outputs.
*/

