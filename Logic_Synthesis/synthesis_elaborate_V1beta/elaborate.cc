/*
* Copyright (c) 1998-2004 Stephen Williams (steve@icarus.com)
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
#ident "$Id: elaborate.cc,v 1.308.2.6 2006/11/27 01:47:14 steve Exp $"
#endif

# include "config.h"
# include "flag.h"

/*
* Elaboration takes as input a complete parse tree and the name of a
* root module, and generates as output the elaborated design. This
* elaborated design is presented as a Module, which does not
* reference any other modules. It is entirely self contained.
*/

# include  <typeinfo>
# include  <sstream>
# include  <list>
# include  "pform.h"
# include  "PEvent.h"
# include  "netlist.h"
# include  "netmisc.h"
# include  "util.h"
# include  "parse_api.h"
# include  "compiler.h"
# define DIVICE 7
//DSP映射颗粒度控制
int Control_Coarse_DSP;
# define Coarse_DSP 1
# define Atom_DSP 2


static Link::strength_t drive_type(PGate::strength_t drv)
{
	switch (drv) {
	case PGate::HIGHZ:
		return Link::HIGHZ;
	case PGate::WEAK:
		return Link::WEAK;
	case PGate::PULL:
		return Link::PULL;
	case PGate::STRONG:
		return Link::STRONG;
	case PGate::SUPPLY:
		return Link::SUPPLY;
	default:
		assert(0);
	}
	return Link::STRONG;
}


void PGate::elaborate(Design*des, NetScope*scope) const
{
	cerr << "internal error: what kind of gate? " <<
		typeid(*this).name() << endl;
}

void PGate::elaborate_single(Design*des, NetScope*scope) const
{
	cerr << "internal error: what kind of gate? " <<
		typeid(*this).name() << endl;
}

void PGate::elaborate_dual(Design*des, NetScope*scope) const
{
	cerr << "internal error: what kind of gate? " <<
		typeid(*this).name() << endl;
}

void PGate::elaborate_bidir_dual(Design*des, NetScope*scope) const
{
	cerr << "internal error: what kind of gate? " <<
		typeid(*this).name() << endl;
}

void process_attributes(Design*des, NetScope*scope,
	Attrib*dest, const map<perm_string,PExpr*>&table)
{
	struct attrib_list_t*attrib_list = 0;
	unsigned attrib_list_n = 0;
	attrib_list = evaluate_attributes(table, attrib_list_n,
		des, scope);

	for (unsigned adx = 0 ;  adx < attrib_list_n ;  adx += 1)
		dest->attribute(attrib_list[adx].key,
		attrib_list[adx].val);

	delete[]attrib_list;
}

/*
* Elaborate the continuous assign. (This is *not* the procedural
* assign.) Elaborate the lvalue and rvalue, and do the assignment.
*/
void PGAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	unsigned long rise_time, fall_time, decay_time;
	eval_delays(des, scope, rise_time, fall_time, decay_time);

	Link::strength_t drive0 = drive_type(strength0());
	Link::strength_t drive1 = drive_type(strength1());

	assert(pin(0));
	assert(pin(1));

	/* Elaborate the l-value. */
	NetNet*lval = pin(0)->elaborate_lnet(des, scope);
	if (lval == 0) {
		des->errors += 1;
		return;
	}


	/* Handle the special case that the rval is simply an
	identifier. Get the rval as a NetNet, then use NetBUFZ
	objects to connect it to the l-value. This is necessary to
	direct drivers. This is how I attach strengths to the
	assignment operation. */
	if (const PEIdent*id = dynamic_cast<const PEIdent*>(pin(1))) {
		NetNet*rid = id->elaborate_net(des, scope, lval->pin_count(),
			0, 0, 0, Link::STRONG,
			Link::STRONG);
		if (rid == 0) {
			des->errors += 1;
			return;
		}

		assert(rid);


		/* If the right hand net is the same type as the left
		side net (i.e., WIRE/WIRE) then it is enough to just
		connect them together. Otherwise, put a bufz between
		them to carry strengths from the rval.

		While we are at it, handle the case where the r-value
		is not as wide as the l-value by padding with a
		constant-0. */

		unsigned cnt = lval->pin_count();
		if (rid->pin_count() < cnt)
			cnt = rid->pin_count();

		bool need_driver_flag = false;

		/* If the device is linked to itself, a driver is
		needed. Should I print a warning here? */
		for (unsigned idx = 0 ;  idx < cnt ;  idx += 1) {
			if (lval->pin(idx) .is_linked (rid->pin(idx))) {
				need_driver_flag = true;
				break;
			}
		}

		/* If the nets are different type (i.e., reg vs. tri) then
		a driver is needed. */
		if (rid->type() != lval->type())
			need_driver_flag = true;

		/* If there is a delay, then I need a driver to carry
		it. */
		if (rise_time || fall_time || decay_time)
			need_driver_flag = true;

		/* If there is a strength to be carried, then I need a
		driver to carry that strength. */
		for (unsigned idx = 0 ;  idx < cnt ;  idx += 1) {
			if (rid->pin(idx).drive0() != drive0) {
				need_driver_flag = true;
				break;
			}
			if (rid->pin(idx).drive1() != drive1) {
				need_driver_flag = true;
				break;
			}
		}

		if (! need_driver_flag) {
			/* Don't need a driver, presumably because the
			r-value already has the needed drivers. Just
			hook things up. If the r-value is too narrow
			for the l-value, then sign extend it or zero
			extend it, whichever makes sense. */
			unsigned idx;
			for (idx = 0 ;  idx < cnt; idx += 1)
				connect(lval->pin(idx), rid->pin(idx));

			if (cnt < lval->pin_count()) {
				if (lval->get_signed() && rid->get_signed()) {
					for (idx = cnt
						;  idx < lval->pin_count()
						;  idx += 1)
						connect(lval->pin(idx), rid->pin(cnt-1));

				} else {
					verinum tmpv (0UL, lval->pin_count()-cnt);
					NetConst*tmp = new NetConst(scope,
						scope->local_symbol(),
						tmpv);
					des->add_node(tmp);
					for (idx = cnt
						;  idx < lval->pin_count()
						; idx += 1)
						connect(lval->pin(idx), tmp->pin(idx-cnt));
				}
			}

		} else {
			/* Do need a driver. Use BUFZ objects to carry the
			strength and delays. */
			unsigned idx;
			for (idx = 0 ; idx < cnt ;  idx += 1) {
				NetBUFZ*dev = new NetBUFZ(scope,scope->local_symbol());
				connect(lval->pin(idx), dev->pin(0));
				connect(rid->pin(idx), dev->pin(1));
				dev->rise_time(rise_time);
				dev->fall_time(fall_time);
				dev->decay_time(decay_time);
				dev->pin(0).drive0(drive0);
				dev->pin(0).drive1(drive1);
				des->add_node(dev);
			}

			if (cnt < lval->pin_count()) {
				if (lval->get_signed() && rid->get_signed()) {
					for (idx = cnt
						;  idx < lval->pin_count()
						;  idx += 1)
						connect(lval->pin(idx), lval->pin(cnt-1));

				} else {
					NetConst*dev = new NetConst(scope,
						scope->local_symbol(),
						verinum::V0);

					des->add_node(dev);
					dev->pin(0).drive0(drive0);
					dev->pin(0).drive1(drive1);
					for (idx = cnt
						;  idx < lval->pin_count()
						; idx += 1)
						connect(lval->pin(idx), dev->pin(0));
				}
			}
		}

		return;
	}

	/* Elaborate the r-value. Account for the initial decays,
	which are going to be attached to the last gate before the
	generated NetNet. */
	NetNet*rval = pin(1)->elaborate_net(des, scope,
		lval->pin_count(),
		rise_time, fall_time, decay_time,
		drive0, drive1);
	if (rval == 0) {
		cerr << get_line() << ": error: Unable to elaborate r-value: "
			<< *pin(1) << endl;
		des->errors += 1;
		return;
	}

	assert(lval && rval);


	/* If the r-value insists on being smaller then the l-value
	(perhaps it is explicitly sized) the pad it out to be the
	right width so that something is connected to all the bits
	of the l-value. */
	if (lval->pin_count() > rval->pin_count())
		rval = pad_to_width(des, rval, lval->pin_count());

	for (unsigned idx = 0 ;  idx < lval->pin_count() ;  idx += 1)
		connect(lval->pin(idx), rval->pin(idx));

	if (lval->local_flag())
		delete lval;

}


void PMemAssign::elaborate_single(Design*des, NetScope*scope) const
{
	assert(scope);

	bool fast_m4k_mapping = false;
	// if to favourite fast M4K, not supported yet!
	if (fast_m4k_mapping){
		cerr << "Fast_M4K_mapping not supported yet!" << endl;
		assert(0);
	}

	NetNet*eval[14];
	
	//clocken0 | clocken1 | wren_a | aclr0 | clock0 | wren_b | aclr1 | clock1 | address_a | address_b | data_a | data_b | q_a | q_b | rden_a | rden_b
	//    0    |     1    |    2   |   3   |    4   |    5   |   6   |    7   |      8    |      9    |    10  |    11  |  12 |  13 |   14   |   15
	for (unsigned idx = 0; idx < 14; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}

		assert(eval[idx]);
	}

	// =========================================
	//	add memory partition here !!! --CCC 
	// =========================================

	unsigned width_address_a = eval[8]->pin_count();
	//unsigned width_address_b = eval[9]->pin_count();
	unsigned width_data_a = eval[10]->pin_count();
	//unsigned width_data_b = eval[11]->pin_count();

	// from device
	unsigned address[9] = {12,  11,  10,  9,  9,  8,  8,  7,  7};
	unsigned width[9] = {   1,   2,   4,  9,  8, 18, 16, 36, 32};

	// memory partition results
	unsigned address_a_m4k;//主体部分m4k的地址宽度
	unsigned data_a_m4k;//主体部分m4k的数据宽度
	unsigned data_a_last_m4k;//最后一个m4k的数据宽度
	unsigned depth_a_m4k = 0;//深度上需要m4k的个数
	unsigned log_depth_a_m4k = 0;
	unsigned width_a_m4k = 0;//宽度上需要m4k的个数
	unsigned cost_m4k = 0;

	// ===================================================
	// ===================================================
	//	find the M4K configuration:
	//	address_a_m4k -
	//			address bits of M4K's port A
	//	data_a_m4k -
	//			data width of M4K's port A
	//	data_a_last_m4k -
	//			data width of last M4K's port A
	//	depth_a_m4k -
	//			number of M4K's along depth of port A
	//	log_depth_a_m4k -
	//			log of depth_a_m4k
	//	width_a_m4k -
	//			number of M4K's along width of port A
	// ===================================================
	// ===================================================

	for (unsigned idx = 0; idx < 9; idx++) {
		// from device
		unsigned address_tmp = address[idx];
		unsigned width_tmp = width[idx];
		unsigned width_last_tmp = width[idx];

		unsigned num_depth = 1;
		unsigned log_num_depth = 0;
		if ( width_address_a >= address_tmp ) {
			num_depth = pow(2,double(width_address_a-address_tmp));
			log_num_depth = width_address_a - address_tmp;
		}
		else
			address_tmp = width_address_a;
		unsigned num_width = 1;
		if ( width_data_a >= width_tmp ) {
			num_width = width_data_a / width_tmp;
			width_last_tmp = width_data_a % width_tmp;
			if (width_last_tmp == 0)
				width_last_tmp = width_tmp;
			else
				num_width++;
		}
		else {
			width_tmp = width_data_a;
			width_last_tmp = width_data_a;
		}
		if ( ( num_width - 1 ) * width_tmp + width_last_tmp != width_data_a ) {
			cerr << "[ Num_Width(" << num_width << ") - 1 ] * Width_Tmp(" << width_tmp << ") + Width_Last_Tmp(" << width_last_tmp << ")" << endl;
			cerr << "!= Width_Data_A(" << width_data_a << ")!" << endl;
			assert(0);
		}

		// here we optimize for:
		// first, timing, which is num_depth
		// secondly, resource, which is num_depth * num_width
		unsigned cost = num_depth * 10000 + num_width;

		if ( ( cost_m4k == 0 ) || ( cost < cost_m4k ) ) {
			address_a_m4k = address_tmp;
			data_a_m4k = width_tmp;
			data_a_last_m4k = width_last_tmp;
			depth_a_m4k = num_depth;
			log_depth_a_m4k = log_num_depth;
			width_a_m4k = num_width;
			cost_m4k = cost;
		}
	}

	// ====================
	//	generate M4K's
	// ====================

	// need WR_EN AND here!!!
	// use eval[8]->pin[log_depth_a_m4k]
	// if log_depth_a_m4k > 0
	NetNet* sig_not = 0;
	NetNet* sig_and = 0;
	NetNet* sig_and_old = 0;
	NetNet* sig_wr_en_a = eval[2];
	if (log_depth_a_m4k > 0) {
		// first, generate NOT's for each eval[8]->pin[log_depth_a_m4k]
		// use pin(width_address_a-1:width_address_a-log_depth_a_m4k)
		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_a_m4k);
		sig_not->local_flag(true);
		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
			// pin 0 - output
			connect(gate->pin(0), sig_not->pin(idx));
			// pin 1 - input
			unsigned wren_offset = width_address_a - log_depth_a_m4k;
			connect(gate->pin(1), eval[8]->pin(idx+wren_offset));
			des->add_node(gate);
		}

		// secondly, generate AND's using eval[8]->pin[log_depth_a_m4k] and sig_not
		for (unsigned idx = 1; idx < log_depth_a_m4k; idx++) {
			sig_and_old = sig_and;
			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,(double)(idx+1)));
			sig_and->local_flag(true);
			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_and->pin(jdx));
				if (idx == 1) {
					if (jdx % 2 == 0)
						connect(gate->pin(1), sig_not->pin(0));
					else {
						unsigned wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(1), eval[8]->pin(0+wren_offset));
					}
					if (jdx < 2)
						connect(gate->pin(2), sig_not->pin(1));
					else {
						unsigned wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(2), eval[8]->pin(1+wren_offset));
					}
				}
				else {
					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
					if (jdx < pow(2,(double)idx))
						connect(gate->pin(2), sig_not->pin(idx));
					else {
						unsigned wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(2), eval[8]->pin(idx+wren_offset));
					}
				}
				des->add_node(gate);
			}
		}

		// thirdly, generate AND's using sig_and and wr_en
		sig_wr_en_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_a_m4k);
		sig_wr_en_a->local_flag(true);
		if (log_depth_a_m4k == 1) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(0));
			connect(gate->pin(1), sig_not->pin(0));
			connect(gate->pin(2), eval[2]->pin(0));
			des->add_node(gate);
			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(1));
			unsigned wren_offset = width_address_a - log_depth_a_m4k;
			connect(gate->pin(1), eval[8]->pin(0+wren_offset));
			connect(gate->pin(2), eval[2]->pin(0));
			des->add_node(gate);
		}
		else {
			for (unsigned idx = 0; idx < depth_a_m4k; idx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_wr_en_a->pin(idx));
				connect(gate->pin(1), sig_and->pin(idx));
				connect(gate->pin(2), eval[2]->pin(0));
				des->add_node(gate);
			}
		}
	}

	// CCC
	//NetNet*sig_mem = eval[13];
	//连接NetMem与其控制信号、数据输入信号，以及数据输出信号
	NetNet*sig_mem_a = eval[12];
	if (log_depth_a_m4k > 0) {
		sig_mem_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_a*depth_a_m4k);
		sig_mem_a->local_flag(true);
	}
	for (unsigned depth_a_idx = 0; depth_a_idx < depth_a_m4k; depth_a_idx++) {
		for (unsigned width_a_idx = 0; width_a_idx < width_a_m4k; width_a_idx++) {

			unsigned data_a = data_a_m4k;
			if (width_a_idx == width_a_m4k - 1)
				data_a = data_a_last_m4k;

			NetMem*mem = new NetMem(scope, scope->local_symbol(),
				address_a_m4k,
				0,
				data_a,
				0,
				data_a,
				0);
			des->add_node(mem);

			//ADU:begin
			NetExpr* expr_initial_file = scope->parameters["init_file"].expr;
			if(!expr_initial_file){
				verinum attri_initial_file("NULL");
				mem->attribute("init_file",attri_initial_file);
			}else{
				NetEConst*econst_initial_file = dynamic_cast<NetEConst*>(expr_initial_file);
				if(!econst_initial_file){
					cerr << "Error:" << __LINE__;
					des->errors++;
					return;
				}

				verinum attri_initial_file(econst_initial_file->value());
				mem->attribute("init_file",attri_initial_file);
			}
			

			verinum attri_mode("single_port");
			mem->attribute("mode",attri_mode);

			NetScope* up = scope->parent();
			perm_string up_module_name = up->module_name();			
			verinum attri_up_module_name(up_module_name.str());
			mem->attribute("up_module_name",attri_up_module_name);

			std::string up_scope_name = up->name();
			verinum attri_up_scope_name(up_scope_name);
			mem->attribute("up_scope_name",attri_up_scope_name);
			//ADU:end

			connect(mem->pin_clocken0(),eval[0]->pin(0));
			connect(mem->pin_clocken1(),eval[1]->pin(0));
			// use WR_EN AND gate output here!!!
			connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_a_idx));

			//ADU:
			unsigned port_a_first_address = depth_a_idx*pow(2,double(address_a_m4k));
			unsigned port_a_last_address = port_a_first_address + pow(2,double(address_a_m4k)) - 1;
			
			verinum attri_port_a_first_address(port_a_first_address);
			mem->attribute("port_a_first_address",attri_port_a_first_address);
			verinum attri_port_a_last_address(port_a_last_address);
			mem->attribute("port_a_last_address",attri_port_a_last_address);

			connect(mem->pin_aclr0(),eval[3]->pin(0));
			connect(mem->pin_clock0(),eval[4]->pin(0));
			connect(mem->pin_wren_b(),eval[5]->pin(0));
			connect(mem->pin_aclr1(),eval[6]->pin(0));
			connect(mem->pin_clock1(),eval[7]->pin(0));
			//CCC
			// width_address_a is divided into 2 parts
			// address_a_m4k, used for M4K's address
			// log_depth_a_m4k, used for WR_EN AND
			unsigned address_a_offset = 0;
			for (unsigned idx = 0; idx < address_a_m4k; idx++)
				connect(mem->pin_address_a(idx),eval[8]->pin(idx+address_a_offset));

			// width_address_b is 0
			//unsigned address_b_offset = 0;
			//for (unsigned idx = 0; idx < address_b_m4k; idx++)
			// connect(mem->pin_address_b(idx),eval[9]->pin(idx+address_b_offset));

			unsigned data_a_offset = width_a_idx * data_a_m4k;
			//ADU:
			unsigned port_a_first_bit_number = data_a_offset;
			verinum attri_port_a_first_bit_number(port_a_first_bit_number);
			mem->attribute("port_a_first_bit_number", attri_port_a_first_bit_number);

			for (unsigned idx = 0; idx < data_a; idx++) {
				connect(mem->pin_data_a(idx),eval[10]->pin(idx+data_a_offset));
				if (log_depth_a_m4k == 0)
					connect(mem->pin_q_a(idx),eval[12]->pin(idx+data_a_offset));
				else
					connect(mem->pin_q_a(idx),sig_mem_a->pin(idx+depth_a_idx*width_data_a+data_a_offset));
			}

			//unsigned data_b_offset = width_b_idx * data_b_m4k;
			//for (unsigned idx = 0; idx < data_b; idx++) {
			// connect(mem->pin_data_b(idx),eval[11]->pin(idx+data_b_offset));
			// if (log_depth_b_m4k == 0)
			//  connect(mem->pin_q_b(idx),eval[13]->pin(idx+data_b_offset));
			// else
			//  connect(mem->pin_q_b(idx),sig_mem_b->pin(idx+depth_b_idx*width_data_b+data_b_offset));
			//}

		}

	}

	// need output MUX here!!!
	// use eval[8]->pin[log_depth_a_m4k]
	// pin(width_address_a-1:width_address_a-log_depth_a_m4k)
	// if log_depth_a_m4k > 0
	if (log_depth_a_m4k > 0) {
		// NetMux(xx, xx, data_width, mux_input_num, select_width);
		NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
		// Connect the data inputs
		for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
			for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
				connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
		// Connect the output
		for (unsigned idx = 0; idx < width_data_a; idx++)
			connect(mux->pin_Result(idx), eval[12]->pin(idx));
		// Connect the select input
		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++)
			connect(mux->pin_Sel(idx), eval[8]->pin(idx+width_address_a-log_depth_a_m4k));
		des->add_node(mux);
	}
}

void PMemAssign::elaborate_dual(Design*des, NetScope*scope) const
{
	assert(scope);

	bool fast_m4k_mapping = false;
	// if to favourite fast M4K, not supported yet!
	if (fast_m4k_mapping){
		cerr << "Fast_M4K_mapping not supported yet!" << endl;
		assert(0);
	}

	NetNet*eval[16];

	//clocken0 | clocken1 | wren_a | aclr0 | clock0 | wren_b | aclr1 | clock1 | address_a | address_b | data_a | data_b | q_a | q_b | rden_a | rden_b
	//    0    |     1    |    2   |   3   |    4   |    5   |   6   |    7   |      8    |      9    |    10  |    11  |  12 |  13 |   14   |   15

	for (unsigned idx = 0; idx < 16; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}

		assert(eval[idx]);
	}

	// =========================================
	//	add memory partition here !!! --CCC 
	// =========================================

	unsigned width_address_a = eval[8]->pin_count();
	unsigned width_address_b = eval[9]->pin_count();
	unsigned width_data_a = eval[10]->pin_count();
	unsigned width_data_b = eval[11]->pin_count();

	if ( pow(2,(double)width_address_a) * width_data_a != pow(2,(double)width_address_b) * width_data_b ) {
		cerr << "( 2 ^ Width_Address_A(" << width_address_a << ") ) * Width_Data_A(" << width_data_a << ")" << endl;
		cerr << "!= ( 2 ^ Width_Address_B(" << width_address_b << ") ) * Width_Data_B(" << width_data_b << ")" << endl;
		assert(0);
	}
	if (width_data_a > width_data_b) {
		if ( ( width_data_a != width_data_b * 2 ) && ( width_data_a != width_data_b * 4 )
			&& ( width_data_a != width_data_b * 8 ) && ( width_data_a != width_data_b * 16) 
			&& ( width_data_a != width_data_b * 32 ) ) {
				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
				//assert(0);
				des->errors ++;//ADU
				return;
		}
	}
	else if (width_data_a < width_data_b) {
		if ( ( width_data_b != width_data_a * 2 ) && ( width_data_b != width_data_a * 4 )
			&& ( width_data_b != width_data_a * 8 ) && ( width_data_b != width_data_a * 16)
			&& ( width_data_b != width_data_a * 32 ) ) {
				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
				//assert(0);
				des->errors ++;//ADU
				return;
		}
	}

	// double ratio - ratio of width_data_a to width_data_b
	// signed log_ratio - log of ratio of address bits of port A to B
	double ratio = (float)width_data_a / (float)width_data_b;
	signed log_ratio = width_address_a - width_address_b;
	if (ratio * pow(2,(double)log_ratio) != 1) {
		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
		assert(0);
	}
	if ( ( ratio < 1 ) && ( (int) ( 1/ratio ) ) != pow(2,(double)log_ratio) ) {
		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
		assert(0);
	}

	// from device
	unsigned address[9] = {12,  11,  10,  9,  9,  8,  8,  7,  7};
	unsigned width[9] = {   1,   2,   4,  9,  8, 18, 16, 36, 32};

	// memory partition results
	unsigned address_b_m4k;
	unsigned data_b_m4k;
	unsigned data_b_last_m4k;
	unsigned depth_b_m4k = 0;
	unsigned log_depth_b_m4k = 0;
	unsigned width_b_m4k = 0;
	unsigned cost_m4k = 0;

	// ===================================================
	// ===================================================
	//	find the M4K configuration:
	//	address_b_m4k -
	//			address bits of M4K's port B
	//	data_b_m4k -
	//			data width of M4K's port B
	//	data_b_last_m4k -
	//			data width of last M4K's port B
	//	depth_b_m4k -
	//			number of M4K's along depth of port B
	//	log_depth_b_m4k -
	//			log of depth_b_m4k
	//	width_b_m4k -
	//			number of M4K's along width of port B
	// ===================================================
	// ===================================================

	for (unsigned idx = 0; idx < 9; idx++) {
		// from device
		unsigned address_tmp = address[idx];
		unsigned width_tmp = width[idx];
		unsigned width_last_tmp = width[idx];
		//当b端口的位宽大于a端口时，即ratio<1时，要保证最后一组的位宽( width_data_b % width_tmp )要能被int( 1/ratio )整除
		//例：逻辑存储为：a端口512x16，b端口256x32时，ratio = 0.5，则b端口可以配置成256x18+256x14，相应的a端口可以配置成512x9+512x7；
		//例：逻辑存储为：a端口1kx8，b端口256x32时，ratio = 0.25，那么b端口就不能配置成256x18+256x14，
		//		最后一组的位宽( width_data_b % width_tmp = 14)不能被int( 1/ratio ) = 4整除。所以b端口只能配置成256x16+256x16，a宽口配置成512x8+512x8
		if ( ( ( ratio >= 1 ) && ( log_ratio <= 0 ) && ( width_tmp * ratio <= 36 ) ) 
			|| ( ( ratio < 1 ) && ( ( width_data_b % width_tmp ) % (int) ( 1/ratio ) ) == 0 && ( width_tmp * ratio >= 1 ) ) ) {
			unsigned num_depth = 1;
			unsigned log_num_depth = 0;
			if ( width_address_b >= address_tmp ) {
				num_depth = pow(2,double(width_address_b-address_tmp));
				log_num_depth = width_address_b - address_tmp;
			}
			else
				address_tmp = width_address_b;
			unsigned num_width = 1;
			if ( width_data_b >= width_tmp ) {
				num_width = width_data_b / width_tmp;
				width_last_tmp = width_data_b % width_tmp;
				if (width_last_tmp == 0)
					width_last_tmp = width_tmp;
				else
					num_width++;
			}
			else {
				width_tmp = width_data_b;
				width_last_tmp = width_data_b;
			}
			if ( ( num_width - 1 ) * width_tmp + width_last_tmp != width_data_b ) {
				cerr << "[ Num_Width(" << num_width << ") - 1 ] * Width_Tmp(" << width_tmp << ") + Width_Last_Tmp(" << width_last_tmp << ")" << endl;
				cerr << "!= Width_Data_B(" << width_data_b << ")!" << endl;
				assert(0);
			}

			// here we optimize for:
			// first, timing, which is num_depth
			// secondly, resource, which is num_depth * num_width
			unsigned cost = num_depth * 10000 + num_width;

			if ( ( cost_m4k == 0 ) || ( cost < cost_m4k ) ) {
				if ( ( ( ratio <= 1 ) && ( log_ratio >= 0 ) ) || ( num_depth * pow(2,(double)(address[idx]-7)) >= ratio ) ) {
					address_b_m4k = address_tmp;
					data_b_m4k = width_tmp;
					data_b_last_m4k = width_last_tmp;
					depth_b_m4k = num_depth;
					log_depth_b_m4k = log_num_depth;
					width_b_m4k = num_width;
					cost_m4k = cost;
				}
			}
		}
	}

	// ============================================================
	// ============================================================
	//	find the M4K configuration (continued):
	//	address_a_m4k -
	//			address bits of M4K's port A
	//	data_a_m4k -
	//			data width of M4K's port A
	//	data_a_last_m4k -
	//			data width of last M4K's port A
	//	depth_a_m4k -
	//			number of M4K's along depth of port A
	//	log_depth_a_m4k -
	//			log of depth_a_m4k
	//	width_a_m4k -
	//			number of M4K's along width of port A
	//	ratio_m4k -
	//			ratio of data_a_m4k to data_b_m4k
	//	log_ratio_m4k -
	//			log of ratio of address_a_m4k to address_b_m4k
	// ============================================================
	// ============================================================

	unsigned address_a_m4k = address_b_m4k;
	unsigned data_a_m4k = data_b_m4k;
	unsigned data_a_last_m4k = data_b_last_m4k;
	double ratio_m4k = 1;
	signed log_ratio_m4k = 0;
	unsigned depth_a_m4k = depth_b_m4k;
	unsigned log_depth_a_m4k = log_depth_b_m4k;
	unsigned width_a_m4k = width_b_m4k;
	// if compared to port B, port A is fat
	if ( ( ratio > 1 ) && ( log_ratio < 0 ) ) {
		// to construct port A
		// first, a coarser grain construction on the number of M4K's along the depth and width
		// each iteration, depth_a_m4k is / 2, while width_a_m4k is * 2
		//while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) && ( depth_a_m4k % 2 == 0 ) ) {/*该循环计算端口A的情况，宽度和深度上都需要几个m4k*/
		//	depth_a_m4k = depth_a_m4k / 2;                                                       /*第二个条件逻辑保证时序优先，即如果深度上只需要一个m4k则停止宽展宽度*/
		//	log_depth_a_m4k--;
		//	width_a_m4k = width_a_m4k * 2;
		//}
		// secondly, a finer grain construction on the M4K's depth and width
		// each iteration, M4K's depth is / 2, while M4K's width is * 2
		while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio )/* && ( address_a_m4k > 4 )*/ ) {
			address_a_m4k--;//更新A端口地址线位数
			data_a_m4k = data_a_m4k * 2;//更新A端口数据线位数
			data_a_last_m4k = data_a_last_m4k * 2;
			ratio_m4k = ratio_m4k * 2;
			log_ratio_m4k--;
		}
	}
	// else if compared to port B, port A is tall
	else if ( ( ratio < 1 ) && ( log_ratio > 0 ) ) {
		// to construct port A
		// first, a finer grain construction on the M4K's depth and width
		// each iteration, M4K's depth is * 2, while M4K's width is / 2
		while ( ( ratio_m4k > ratio ) && ( data_a_m4k % 2 == 0 ) ) {
			address_a_m4k++;
			data_a_m4k = data_a_m4k / 2;
			data_a_last_m4k = data_a_last_m4k / 2;
			ratio_m4k = ratio_m4k / 2;
			log_ratio_m4k++;
		}
		// secondly, a coarser grain construction on the number of M4K's along the depth and width
		// each iteration, depth_a_m4k is * 2, while width_a_m4k is / 2
		/*while ( ( ratio_m4k * width_a_m4k / width_b_m4k > ratio ) && ( width_a_m4k % 2 == 0 ) ) {
		depth_a_m4k = depth_a_m4k * 2;
		log_depth_a_m4k++;
		width_a_m4k = width_a_m4k / 2;
		}*/
	}
	if ( ( width_a_m4k - 1 ) * data_a_m4k + data_a_last_m4k != width_data_a ) {
		cerr << "[ Width_A_M4K(" << width_a_m4k << ") - 1 ] * Data_A_M4K(" << data_a_m4k << ") + Data_A_Last_M4K(" << data_a_last_m4k << ")" << endl;
		cerr << "!= Width_Data_A(" << width_data_a << ")!" << endl;
		assert(0);
	}

	// ====================
	//	generate M4K's
	// ====================

	if (depth_b_m4k * width_b_m4k != depth_a_m4k * width_a_m4k) {
		cerr << "Depth_B_M4K(" << depth_b_m4k << ") * Width_B_M4K(" << width_b_m4k << ")" << endl;
		cerr << "!= Depth_A_M4K(" << depth_a_m4k << ") * Width_A_M4K, error!" << endl;
		assert(0);
	}

	if (address_a_m4k + log_depth_a_m4k != width_address_a) {
		cerr << "Address_A_M4K(" << address_a_m4k << ") + Log_Depth_A_M4K(" << log_depth_a_m4k << ")" << endl;
		cerr << "!= Width_Address_A(" << width_address_a << "), error!" << endl;
		assert(0);
	}
	if (address_b_m4k + log_depth_b_m4k != width_address_b) {
		cerr << "Address_B_M4K(" << address_b_m4k << ") + Log_Depth_B_M4K(" << log_depth_b_m4k << ")" << endl;
		cerr << "!= Width_Address_B(" << width_address_b << "), error!" << endl;
		assert(0);
	}

	// need WR_EN AND here!!!
	// use eval[8]->pin[log_depth_a_m4k]
	// if log_depth_a_m4k > 0
	NetNet* sig_not = 0;
	NetNet* sig_and = 0;
	NetNet* sig_and_old = 0;
	NetNet* sig_wr_en_a = eval[2];
	if (log_depth_a_m4k > 0) {
		// first, generate NOT's for each eval[8]->pin[log_depth_a_m4k]
		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_a-1:width_address_a-log_depth_a_m4k)
		// else if ratio < 1 && log_ratio > 0, use pin(width_address_a-1:width_address_a-log_depth_b_m4k) and pin(log_depth_a_m4k-log_depth_b_m4k-1:0)
		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_a_m4k);
		sig_not->local_flag(true);
		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
			// pin 0 - output
			connect(gate->pin(0), sig_not->pin(idx));
			// pin 1 - input
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				wren_offset = width_address_a - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0)
				if (idx < log_depth_a_m4k - log_depth_b_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_a - log_depth_a_m4k;
			connect(gate->pin(1), eval[8]->pin(idx+wren_offset));
			des->add_node(gate);
		}

		// secondly, generate AND's using eval[8]->pin[log_depth_a_m4k] and sig_not
		for (unsigned idx = 1; idx < log_depth_a_m4k; idx++) {
			sig_and_old = sig_and;
			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,(double)(idx+1)));
			sig_and->local_flag(true);
			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_and->pin(jdx));
				if (idx == 1) {
					if (jdx % 2 == 0)
						connect(gate->pin(1), sig_not->pin(0));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0)
							if (0 < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(1), eval[8]->pin(0+wren_offset));
					}
					if (jdx < 2)
						connect(gate->pin(2), sig_not->pin(1));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0)
							if (1 < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(2), eval[8]->pin(1+wren_offset));
					}
				}
				else {
					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
					if (jdx < pow(2,(double)idx))
						connect(gate->pin(2), sig_not->pin(idx));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0)
							if (idx < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						connect(gate->pin(2), eval[8]->pin(idx+wren_offset));
					}
				}
				des->add_node(gate);
			}
		}

		// thirdly, generate AND's using sig_and and wr_en
		sig_wr_en_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_a_m4k);
		sig_wr_en_a->local_flag(true);
		if (log_depth_a_m4k == 1) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(0));
			connect(gate->pin(1), sig_not->pin(0));
			connect(gate->pin(2), eval[2]->pin(0));
			des->add_node(gate);
			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(1));
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				wren_offset = width_address_a - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0)
				if (0 < log_depth_a_m4k - log_depth_b_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_a - log_depth_a_m4k;
			connect(gate->pin(1), eval[8]->pin(0+wren_offset));
			connect(gate->pin(2), eval[2]->pin(0));
			des->add_node(gate);
		}
		else {
			for (unsigned idx = 0; idx < depth_a_m4k; idx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_wr_en_a->pin(idx));
				connect(gate->pin(1), sig_and->pin(idx));
				connect(gate->pin(2), eval[2]->pin(0));
				des->add_node(gate);
			}
		}
	}

	// CCC
	//NetNet*sig_mem = eval[13];
	NetNet*sig_mem_b = eval[13];
	if (log_depth_b_m4k > 0) {
		sig_mem_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_b*depth_b_m4k);
		sig_mem_b->local_flag(true);
	}
	for (unsigned depth_b_idx = 0; depth_b_idx < depth_b_m4k; depth_b_idx++) {
		for (unsigned width_b_idx = 0; width_b_idx < width_b_m4k; width_b_idx++) {

			unsigned data_a = data_a_m4k;
			unsigned data_b = data_b_m4k;
			if (width_b_idx == width_b_m4k - 1) {
				data_a = data_a_last_m4k;
				data_b = data_b_last_m4k;
			}

			NetMem*mem = new NetMem(scope, scope->local_symbol(),
				address_a_m4k,
				address_b_m4k,
				data_a,
				0,
				0,
				data_b);
			des->add_node(mem);

			//ADU:begin
			NetExpr* expr_initial_file = scope->parameters["init_file"].expr;
			if(!expr_initial_file){
				verinum attri_initial_file("NULL");
				mem->attribute("init_file",attri_initial_file);
			}else{
				NetEConst*econst_initial_file = dynamic_cast<NetEConst*>(expr_initial_file);
				if(!econst_initial_file){
					cerr << "Error:" << __LINE__;
					des->errors++;
					return;
				}

				verinum attri_initial_file(econst_initial_file->value());
				mem->attribute("init_file",attri_initial_file);
			}


			verinum attri_mode("simple_dual_port");
			mem->attribute("mode",attri_mode);

			NetScope* up = scope->parent();
			perm_string up_module_name = up->module_name();			
			verinum attri_up_module_name(up_module_name.str());
			mem->attribute("up_module_name",attri_up_module_name);

			std::string up_scope_name = up->name();
			verinum attri_up_scope_name(up_scope_name);
			mem->attribute("up_scope_name",attri_up_scope_name);
			//ADU:end

			//ADU:
			unsigned port_b_first_address = depth_b_idx*pow(2,double(address_b_m4k));
			unsigned port_b_last_address = port_b_first_address + pow(2,double(address_b_m4k)) - 1;

			verinum attri_port_b_first_address(port_b_first_address);
			mem->attribute("port_b_first_address",attri_port_b_first_address);
			verinum attri_port_b_last_address(port_b_last_address);
			mem->attribute("port_b_last_address",attri_port_b_last_address);

			//unsigned port_a_first_address = (int(depth_b_idx/ratio)+int(width_b_idx/width_a_m4k))*pow(2,double(address_a_m4k));
			unsigned port_a_first_address = depth_b_idx*pow(2,double(address_a_m4k));
			unsigned port_a_last_address = port_a_first_address + pow(2,double(address_a_m4k)) - 1;

			verinum attri_port_a_first_address(port_a_first_address);
			mem->attribute("port_a_first_address",attri_port_a_first_address);
			verinum attri_port_a_last_address(port_a_last_address);
			mem->attribute("port_a_last_address",attri_port_a_last_address);

			connect(mem->pin_clocken0(),eval[0]->pin(0));
			connect(mem->pin_clocken1(),eval[1]->pin(0));
			// use WR_EN AND gate output here!!!
			if (ratio >= 1 && log_ratio <= 0)
				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx/(width_a_m4k/width_b_m4k)));
			else if (ratio < 1 && log_ratio > 0)
				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx*(depth_a_m4k/depth_b_m4k)+width_b_idx/width_a_m4k));
			connect(mem->pin_aclr0(),eval[3]->pin(0));
			connect(mem->pin_clock0(),eval[4]->pin(0));
			connect(mem->pin_wren_b(),eval[15]->pin(0));//改接到rden_b
			connect(mem->pin_aclr1(),eval[6]->pin(0));
			connect(mem->pin_clock1(),eval[7]->pin(0));
			//CCC
			// width_address_a is divided into 2 parts
			// address_a_m4k, used for M4K's address
			// log_depth_a_m4k, used for WR_EN AND
			//处理端口A即简单双端口的写地址
			unsigned address_a_offset;
			if (ratio >= 1 && log_ratio <= 0)
				address_a_offset = 0;
			else if (ratio < 1 && log_ratio > 0)
				address_a_offset = log_depth_a_m4k - log_depth_b_m4k;
			for (unsigned idx = 0; idx < address_a_m4k; idx++)
				connect(mem->pin_address_a(idx),eval[8]->pin(idx+address_a_offset));

			// width_address_b is divided into 2 parts
			// address_b_m4k, used for M4K's address
			// log_depth_b_m4k, used for output MUX
			//处理端口B即简单双端口的读地址
			unsigned address_b_offset;
			if (ratio >= 1 && log_ratio <= 0)
				address_b_offset = log_depth_b_m4k - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0)
				address_b_offset = 0;
			for (unsigned idx = 0; idx < address_b_m4k; idx++)
				connect(mem->pin_address_b(idx),eval[9]->pin(idx+address_b_offset));

			//处理端口A即简单双端口的写输入数据
			if (ratio >= 1 && log_ratio <= 0) {
				unsigned data_a_offset = ( depth_b_idx % ( width_a_m4k / width_b_m4k ) ) * width_data_b + width_b_idx * data_b_m4k;
				for (unsigned ratio_idx = 0; ratio_idx < ratio_m4k; ratio_idx++) {
					for (unsigned idx = 0; idx < data_b; idx++) {
						connect(mem->pin_data_a(idx+ratio_idx*data_b),eval[10]->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
						//ADU:
						if(ratio_idx == 0 && idx == 0){
							unsigned port_a_first_bit_number = idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k;
							verinum attri_port_a_first_bit_number(port_a_first_bit_number);
							mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);

							mem->attribute("port_b_first_bit_number",attri_port_a_first_bit_number);
						}
					}
				}
			}
			else if (ratio < 1 && log_ratio > 0) {
				unsigned data_a_offset = width_b_idx % width_a_m4k * data_a_m4k;
				for (unsigned idx = 0; idx < data_a; idx++) {
					connect(mem->pin_data_a(idx),eval[10]->pin(idx+data_a_offset));
					//ADU:
					//if(idx == 0){
					//	unsigned port_a_first_bit_number = idx+data_a_offset;
					//	verinum attri_port_a_first_bit_number(port_a_first_bit_number);
					//	mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
					//}					
				}
			}

			//处理端口B即简单双端口的读输出数据
			if (ratio >= 1 && log_ratio <= 0) {
				unsigned data_b_offset = width_b_idx * data_b_m4k;
				for (unsigned idx = 0; idx < data_b; idx++) {
					//ADU:
					//if(idx == 0){
					//	unsigned port_b_first_bit_number = idx+data_b_offset;
					//	verinum attri_port_b_first_bit_number(port_b_first_bit_number);
					//	mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
					//}
					//						  connect(mem->pin_data_b(idx),eval[11]->pin(idx+data_b_offset));
					if (log_depth_b_m4k == 0){
						connect(mem->pin_q_b(idx),eval[13]->pin(idx+data_b_offset));
						
					}
					else{
						connect(mem->pin_q_b(idx),sig_mem_b->pin(idx+depth_b_idx*width_data_b+data_b_offset));
					}
				}
			}
			else if (ratio < 1 && log_ratio > 0) {
				unsigned data_b_offset = width_b_idx * data_a_m4k;
				for (unsigned ratio_idx = 0; ratio_idx < 1 / ratio_m4k; ratio_idx++) {
					for (unsigned idx = 0; idx < data_a; idx++) {
						//ADU:
						if(ratio_idx == 0 && idx == 0){
							unsigned port_b_first_bit_number = idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k;
							verinum attri_port_b_first_bit_number(port_b_first_bit_number);
							mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);

							mem->attribute("port_a_first_bit_number",attri_port_b_first_bit_number);
						}
						//							  connect(mem->pin_data_b(idx+ratio_idx*data_a),eval[11]->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
						if (log_depth_b_m4k == 0){
							connect(mem->pin_q_b(idx+ratio_idx*data_a),eval[13]->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
							
						}
						else{
							connect(mem->pin_q_b(idx+ratio_idx*data_a),sig_mem_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k+depth_b_idx*width_data_b));
						}
					}
				}
			}

		}

	}

	if ( ratio >= 1 && log_ratio <= 0 ) {


		// need output MUX here!!!
		// use eval[9]->pin[log_depth_b_m4k]
		// pin(log_depth_b_m4k-log_depth_a_m4k-1:0) and pin(width_address_b-1:width_address_b-log_depth_a_m4k)
		// if log_depth_b_m4k > 0
		if (log_depth_b_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_b; idx++)
				connect(mux->pin_Result(idx), eval[13]->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
				if (idx < log_depth_b_m4k - log_depth_a_m4k)
					connect(mux->pin_Sel(idx), eval[9]->pin(idx));
				else
					connect(mux->pin_Sel(idx), eval[9]->pin(idx+width_address_b-log_depth_b_m4k));
			}
			des->add_node(mux);
		}

	}

	else if (ratio < 1 && log_ratio > 0) {
		
		// need output MUX here!!!
		// use eval[9]->pin[log_depth_b_m4k]
		// pin(width_address_b-1:width_address_b-log_depth_b_m4k)
		// if log_depth_b_m4k > 0
		if (log_depth_b_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_b; idx++)
				connect(mux->pin_Result(idx), eval[13]->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++)
				connect(mux->pin_Sel(idx), eval[9]->pin(idx+width_address_b-log_depth_b_m4k));
			des->add_node(mux);
		}

	}
}

//M4K 综合映射备份
/****************************************************************/
//备份(以下注释的部分为M4K综合映射实现部分)
//void PMemAssign::elaborate_bidir_dual(Design*des, NetScope*scope) const
//{
//	assert(scope);
//	bool swap_operands = false;	//ADU: width_data_b <= width_data_a
//								//wren_a <-> wren_b
//								//address_a <-> address_b
//								//data_a <-> data_b
//								//q_a <-> q_b
//	bool fast_m4k_mapping = false;
//	// if to favourite fast M4K, not supported yet!
//	if (fast_m4k_mapping){
//		cerr << "Fast_M4K_mapping not supported yet!" << endl;
//		assert(0);
//	}
//
//	NetNet*eval[14];
//	//clocken0 | clocken1 | wren_a | aclr0 | clock0 | wren_b | aclr1 | clock1 | address_a | address_b | data_a | data_b | q_a | q_b | rden_a | rden_b
//	//    0    |     1    |    2   |   3   |    4   |    5   |   6   |    7   |      8    |      9    |    10  |    11  |  12 |  13 |   14   |   15
//	for (unsigned idx = 0; idx < 14; idx++) {
//		assert(pin(idx));
//
//		/* Elaborate the r-value. Account for the initial decays,
//		which are going to be attached to the last gate before the
//		generated NetNet. */
//		eval[idx] = pin(idx)->elaborate_net(des, scope,
//			1, 0, 0, 0, Link::STRONG, Link::STRONG);
//		if (eval[idx] == 0) {
//			cerr << get_line() << ": error: Unable to elaborate value: "
//				<< *pin(idx) << endl;
//			des->errors += 1;
//			return;
//		}
//
//		assert(eval[idx]);
//	}
//	NetNet*net_clocken0		= eval[0];
//	NetNet*net_clocken1		= eval[1]; 
//	NetNet*net_wren_a		= eval[2];
//	NetNet*net_aclr0		= eval[3]; 
//	NetNet*net_clock0		= eval[4];
//	NetNet*net_wren_b		= eval[5]; 
//	NetNet*net_aclr1		= eval[6];
//	NetNet*net_clock1		= eval[7];
//	NetNet*net_address_a	= eval[8];
//	NetNet*net_address_b	= eval[9]; 
//	NetNet*net_data_a		= eval[10];
//	NetNet*net_data_b		= eval[11]; 
//	NetNet*net_q_a			= eval[12];
//	NetNet*net_q_b			= eval[13]; 
//	// =========================================
//	//	add memory partition here !!! --CCC 
//	// =========================================
//
//	unsigned width_address_a = net_address_a->pin_count();
//	unsigned width_address_b = net_address_b->pin_count();
//	unsigned width_data_a = net_data_a->pin_count();
//	unsigned width_data_b = net_data_b->pin_count();
//	if(width_data_b > width_data_a){
//		swap_operands = true;
//
//		net_wren_a = eval[5];
//		net_wren_b = eval[2];
//		net_data_a = eval[11];
//		net_data_b = eval[10];
//		net_q_a = eval[13];
//		net_q_b = eval[12];
//		net_address_a = eval[9];
//		net_address_b = eval[8];
//
//		width_address_a = net_address_a->pin_count();
//		width_address_b = net_address_b->pin_count();
//		width_data_a = net_data_a->pin_count();
//		width_data_b = net_data_b->pin_count();
//	}
//		
//
//	if ( pow(2,(double)width_address_a) * width_data_a != pow(2,(double)width_address_b) * width_data_b ) {
//		cerr << "( 2 ^ Width_Address_A(" << width_address_a << ") ) * Width_Data_A(" << width_data_a << ")" << endl;
//		cerr << "!= ( 2 ^ Width_Address_B(" << width_address_b << ") ) * Width_Data_B(" << width_data_b << ")" << endl;
//		//assert(0);
//		des->errors ++;
//		cerr << __LINE__ << endl;
//		return;
//	}
//	if (width_data_a > width_data_b) {
//		if ( ( width_data_a != width_data_b * 2 ) && ( width_data_a != width_data_b * 4 )
//			&& ( width_data_a != width_data_b * 8 ) && ( width_data_a != width_data_b * 16) ) {
//				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
//				//assert(0);
//				des->errors ++;
//				cerr << __LINE__ << endl;
//				return;
//		}
//	}
//	else if (width_data_a < width_data_b) {
//		cerr << "width_data_b cannot > width_data_a\n";
//		//assert(0);
//		des->errors ++;
//		cerr << __LINE__ << endl;
//		return;
//		
//	}
//
//	// double ratio - ratio of width_data_a to width_data_b
//	// signed log_ratio - log of ratio of address bits of port A to B
//	double ratio = (float)width_data_a / (float)width_data_b;
//	signed log_ratio = width_address_a - width_address_b;
//
//
//	if (ratio * pow(2,(double)log_ratio) != 1) {
//		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
//		//assert(0);
//		des->errors ++;
//		cerr << __LINE__ << endl;
//		return;
//	}
//	if ( ratio < 1 ) {
//		cerr << "Ratio should not < 1\n";
//		//assert(0);
//		des->errors ++;
//		cerr << __LINE__ << endl;
//		return;
//	}
//
//	// from device
//	unsigned address[7] = {12,  11,  10,  9,  9,  8,  8};
//	unsigned width[7] = {   1,   2,   4,  9,  8, 18, 16};
//
//	// memory partition results
//	unsigned address_b_m4k;
//	unsigned data_b_m4k;
//	unsigned data_b_last_m4k;
//	unsigned depth_b_m4k = 0;
//	unsigned log_depth_b_m4k = 0;
//	unsigned width_b_m4k = 0;
//	unsigned cost_m4k = 0;
//
//	// ===================================================
//	// ===================================================
//	//	find the M4K configuration:
//	//	address_b_m4k -
//	//			address bits of M4K's port B
//	//	data_b_m4k -
//	//			data width of M4K's port B
//	//	data_b_last_m4k -
//	//			data width of last M4K's port B
//	//	depth_b_m4k -
//	//			number of M4K's along depth of port B
//	//	log_depth_b_m4k -
//	//			log of depth_b_m4k
//	//	width_b_m4k -
//	//			number of M4K's along width of port B
//	// ===================================================
//	// ===================================================
//
//	for (unsigned idx = 0; idx < 7; idx++) {
//		// from device
//		unsigned address_tmp = address[idx];
//		unsigned width_tmp = width[idx];
//		unsigned width_last_tmp = width[idx];
//
//		if ( ( ( ratio >= 1 ) && ( log_ratio <= 0 ) && ( width_tmp * ratio <= 36 ) ) 
//			|| (( ratio < 1 ) && ( ( width_data_b % width_tmp ) % (int) ( 1/ratio ) ) == 0 && ( width_tmp * ratio >= 1 ) ) ) {
//			unsigned num_depth = 1;
//			unsigned log_num_depth = 0;
//			if ( width_address_b >= address_tmp ) {
//				num_depth = pow(2,double(width_address_b-address_tmp));
//				log_num_depth = width_address_b - address_tmp;
//			}
//			else
//				address_tmp = width_address_b;
//			unsigned num_width = 1;
//			if ( width_data_b >= width_tmp ) {
//				num_width = width_data_b / width_tmp;
//				width_last_tmp = width_data_b % width_tmp;
//				if (width_last_tmp == 0)
//					width_last_tmp = width_tmp;
//				else
//					num_width++;
//			}
//			else {
//				width_tmp = width_data_b;
//				width_last_tmp = width_data_b;
//			}
//			if ( ( num_width - 1 ) * width_tmp + width_last_tmp != width_data_b ) {
//				cerr << "[ Num_Width(" << num_width << ") - 1 ] * Width_Tmp(" << width_tmp << ") + Width_Last_Tmp(" << width_last_tmp << ")" << endl;
//				cerr << "!= Width_Data_B(" << width_data_b << ")!" << endl;
//				assert(0);
//			}
//
//			// here we optimize for:
//			// first, timing, which is num_depth
//			// secondly, resource, which is num_depth * num_width
//			unsigned cost = num_depth * 10000 + num_width;
//
//			if ( ( cost_m4k == 0 ) || ( cost < cost_m4k ) ) {
//				if ( ( ( ratio <= 1 ) && ( log_ratio >= 0 ) ) || ( num_depth * pow(2,(double)(address[idx]-8)) >= ratio ) ) {
//					address_b_m4k = address_tmp;
//					data_b_m4k = width_tmp;
//					data_b_last_m4k = width_last_tmp;
//					depth_b_m4k = num_depth;
//					log_depth_b_m4k = log_num_depth;
//					width_b_m4k = num_width;
//					cost_m4k = cost;
//				}
//			}
//		}
//	}
//
//	// ============================================================
//	// ============================================================
//	//	find the M4K configuration (continued):
//	//	address_a_m4k -
//	//			address bits of M4K's port A
//	//	data_a_m4k -
//	//			data width of M4K's port A
//	//	data_a_last_m4k -
//	//			data width of last M4K's port A
//	//	depth_a_m4k -
//	//			number of M4K's along depth of port A
//	//	log_depth_a_m4k -
//	//			log of depth_a_m4k
//	//	width_a_m4k -
//	//			number of M4K's along width of port A
//	//	ratio_m4k -
//	//			ratio of data_a_m4k to data_b_m4k
//	//	log_ratio_m4k -
//	//			log of ratio of address_a_m4k to address_b_m4k
//	// ============================================================
//	// ============================================================
//
//	unsigned address_a_m4k = address_b_m4k;
//	unsigned data_a_m4k = data_b_m4k;
//	unsigned data_a_last_m4k = data_b_last_m4k;
//	double ratio_m4k = 1;
//	signed log_ratio_m4k = 0;
//	unsigned depth_a_m4k = depth_b_m4k;
//	unsigned log_depth_a_m4k = log_depth_b_m4k;
//	unsigned width_a_m4k = width_b_m4k;
//	// if compared to port B, port A is fat
//	if ( ( ratio > 1 ) && ( log_ratio < 0 ) ) {
//		// to construct port A
//		// first, a coarser grain construction on the number of M4K's along the depth and width
//		// each iteration, depth_a_m4k is / 2, while width_a_m4k is * 2
//		/*while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) && ( depth_a_m4k % 2 == 0 ) ) {
//		depth_a_m4k = depth_a_m4k / 2;
//		log_depth_a_m4k--;
//		width_a_m4k = width_a_m4k * 2;
//		}*/
//		// secondly, a finer grain construction on the M4K's depth and width
//		// each iteration, M4K's depth is / 2, while M4K's width is * 2
//		while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) /*&& ( address_a_m4k > 4 ) */) {
//			address_a_m4k--;
//			data_a_m4k = data_a_m4k * 2;
//			data_a_last_m4k = data_a_last_m4k * 2;
//			ratio_m4k = ratio_m4k * 2;
//			log_ratio_m4k--;
//		}
//	}
//	// else if compared to port B, port A is tall
//	else if ( ( ratio < 1 ) && ( log_ratio > 0 ) ) {
//		assert(0);
//		// to construct port A
//		// first, a finer grain construction on the M4K's depth and width
//		// each iteration, M4K's depth is * 2, while M4K's width is / 2
//		while ( ( ratio_m4k > ratio ) && ( data_a_m4k % 2 == 0 ) ) {
//			address_a_m4k++;
//			data_a_m4k = data_a_m4k / 2;
//			data_a_last_m4k = data_a_last_m4k / 2;
//			ratio_m4k = ratio_m4k / 2;
//			log_ratio_m4k++;
//		}
//		// secondly, a coarser grain construction on the number of M4K's along the depth and width
//		// each iteration, depth_a_m4k is * 2, while width_a_m4k is / 2
//		/*while ( ( ratio_m4k * width_a_m4k / width_b_m4k > ratio ) && ( width_a_m4k % 2 == 0 ) ) {
//		depth_a_m4k = depth_a_m4k * 2;
//		log_depth_a_m4k++;
//		width_a_m4k = width_a_m4k / 2;
//		}*/
//	}
//	if ( ( width_a_m4k - 1 ) * data_a_m4k + data_a_last_m4k != width_data_a ) {
//		cerr << "[ Width_A_M4K(" << width_a_m4k << ") - 1 ] * Data_A_M4K(" << data_a_m4k << ") + Data_A_Last_M4K(" << data_a_last_m4k << ")" << endl;
//		cerr << "!= Width_Data_A(" << width_data_a << ")!" << endl;
//		//assert(0);
//		cerr << __LINE__<< endl;
//		des->errors ++;
//		return;
//	}
//
//	// ====================
//	//	generate M4K's
//	// ====================
//
//	if (depth_b_m4k * width_b_m4k != depth_a_m4k * width_a_m4k) {
//		cerr << "Depth_B_M4K(" << depth_b_m4k << ") * Width_B_M4K(" << width_b_m4k << ")" << endl;
//		cerr << "!= Depth_A_M4K(" << depth_a_m4k << ") * Width_A_M4K, error!" << endl;
//		//assert(0);
//		cerr << __LINE__<< endl;
//		des->errors ++;
//		return;
//	}
//
//	if (address_a_m4k + log_depth_a_m4k != width_address_a) {
//		cerr << "Address_A_M4K(" << address_a_m4k << ") + Log_Depth_A_M4K(" << log_depth_a_m4k << ")" << endl;
//		cerr << "!= Width_Address_A(" << width_address_a << "), error!" << endl;
//		//assert(0);
//		cerr << __LINE__<< endl;
//		des->errors ++;
//		return;
//	}
//	if (address_b_m4k + log_depth_b_m4k != width_address_b) {
//		cerr << "Address_B_M4K(" << address_b_m4k << ") + Log_Depth_B_M4K(" << log_depth_b_m4k << ")" << endl;
//		cerr << "!= Width_Address_B(" << width_address_b << "), error!" << endl;
//		//assert(0);
//		cerr << __LINE__<< endl;
//		des->errors ++;
//		return;
//	}
//
//	// need WR_EN AND here!!!
//	// use net_address_a->pin[log_depth_a_m4k]
//	// if log_depth_a_m4k > 0
//	NetNet* sig_not = 0;
//	NetNet* sig_and = 0;
//	NetNet* sig_and_old = 0;
//	NetNet* sig_wr_en_a = net_wren_a;
//	if (log_depth_a_m4k > 0) {
//		// first, generate NOT's for each net_address_a->pin[log_depth_a_m4k]
//		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_a-1:width_address_a-log_depth_a_m4k)
//		// else if ratio < 1 && log_ratio > 0, use pin(width_address_a-1:width_address_a-log_depth_b_m4k) and pin(log_depth_a_m4k-log_depth_b_m4k-1:0)
//		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_a_m4k);
//		sig_not->local_flag(true);
//		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
//			// pin 0 - output
//			connect(gate->pin(0), sig_not->pin(idx));
//			// pin 1 - input
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				wren_offset = width_address_a - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				if (idx < log_depth_a_m4k - log_depth_b_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_a - log_depth_a_m4k;
//			}			
//			connect(gate->pin(1), net_address_a->pin(idx+wren_offset));
//			des->add_node(gate);
//		}
//
//		// secondly, generate AND's using net_address_a->pin[log_depth_a_m4k] and sig_not
//		for (unsigned idx = 1; idx < log_depth_a_m4k; idx++) {
//			sig_and_old = sig_and;
//			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
//			sig_and->local_flag(true);
//			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_and->pin(jdx));
//				if (idx == 1) {
//					if (jdx % 2 == 0)
//						connect(gate->pin(1), sig_not->pin(0));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							if (0 < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						}							
//						connect(gate->pin(1), net_address_a->pin(0+wren_offset));
//					}
//					if (jdx < 2)
//						connect(gate->pin(2), sig_not->pin(1));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							if (1 < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						}							
//						connect(gate->pin(2), net_address_a->pin(1+wren_offset));
//					}
//				}
//				else {
//					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
//					if (jdx < pow(2,(double)idx))
//						connect(gate->pin(2), sig_not->pin(idx));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							if (idx < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						}							
//						connect(gate->pin(2), net_address_a->pin(idx+wren_offset));
//					}
//				}
//				des->add_node(gate);
//			}
//		}
//
//		// thirdly, generate AND's using sig_and and wr_en
//		sig_wr_en_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_a_m4k);
//		sig_wr_en_a->local_flag(true);
//		if (log_depth_a_m4k == 1) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_a->pin(0));
//			connect(gate->pin(1), sig_not->pin(0));
//			connect(gate->pin(2), net_wren_a->pin(0));
//			des->add_node(gate);
//			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_a->pin(1));
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				wren_offset = width_address_a - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				if (0 < log_depth_a_m4k - log_depth_b_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_a - log_depth_a_m4k;
//			}				
//			connect(gate->pin(1), net_address_a->pin(0+wren_offset));
//			connect(gate->pin(2), net_wren_a->pin(0));
//			des->add_node(gate);
//		}
//		else {
//			for (unsigned idx = 0; idx < depth_a_m4k; idx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_wr_en_a->pin(idx));
//				connect(gate->pin(1), sig_and->pin(idx));
//				connect(gate->pin(2), net_wren_a->pin(0));
//				des->add_node(gate);
//			}
//		}
//	}
//
//	// need WR_EN AND here!!!
//	// use net_address_b->pin[log_depth_b_m4k]
//	// if log_depth_b_m4k > 0
//	//NetNet* sig_not = 0;
//	//NetNet* sig_and = 0;
//	//NetNet* sig_and_old = 0;
//	//NetNet* sig_wr_en = net_wren_a;
//	NetNet* sig_wr_en_b = net_wren_b;
//	if (log_depth_b_m4k > 0) {
//		// first, generate NOT's for each net_address_b->pin[log_depth_b_m4k]
//		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_b-1:width_address_b-log_depth_a_m4k) and pin(log_depth_b_m4k-log_depth_a_m4k-1:0)
//		// else if ratio < 1 && log_ratio > 0, use pin(width_address_b-1:width_address_b-log_depth_b_m4k)
//		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_b_m4k);
//		sig_not->local_flag(true);
//		for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
//			// pin 0 - output
//			connect(gate->pin(0), sig_not->pin(idx));
//			// pin 1 - input
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				if (idx < log_depth_b_m4k - log_depth_a_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_b - log_depth_b_m4k;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				wren_offset = width_address_b - log_depth_b_m4k;
//			}
//			
//			connect(gate->pin(1), net_address_b->pin(idx+wren_offset));
//			des->add_node(gate);
//		}
//
//		// secondly, generate AND's using net_address_b->pin[log_depth_b_m4k] and sig_not
//		for (unsigned idx = 1; idx < log_depth_b_m4k; idx++) {
//			sig_and_old = sig_and;
//			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
//			sig_and->local_flag(true);
//			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_and->pin(jdx));
//				if (idx == 1) {
//					if (jdx % 2 == 0)
//						connect(gate->pin(1), sig_not->pin(0));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (0 < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							wren_offset = width_address_b - log_depth_b_m4k;
//						}
//							
//						connect(gate->pin(1), net_address_b->pin(0+wren_offset));
//					}
//					if (jdx < 2)
//						connect(gate->pin(2), sig_not->pin(1));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (1 < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							wren_offset = width_address_b - log_depth_b_m4k;
//						}
//							
//						connect(gate->pin(2), net_address_b->pin(1+wren_offset));
//					}
//				}
//				else {
//					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
//					if (jdx < pow(2,(double)idx))
//						connect(gate->pin(2), sig_not->pin(idx));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (idx < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0){
//							assert(0);
//							wren_offset = width_address_b - log_depth_b_m4k;
//						}
//							
//						connect(gate->pin(2), net_address_b->pin(idx+wren_offset));
//					}
//				}
//				des->add_node(gate);
//			}
//		}
//
//		// thirdly, generate AND's using sig_and and wr_en
//		sig_wr_en_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_b_m4k);
//		sig_wr_en_b->local_flag(true);
//		if (log_depth_b_m4k == 1) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_b->pin(0));
//			connect(gate->pin(1), sig_not->pin(0));
//			connect(gate->pin(2), net_wren_b->pin(0));
//			des->add_node(gate);
//			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_b->pin(1));
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				if (0 < log_depth_b_m4k - log_depth_a_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_b - log_depth_b_m4k;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				wren_offset = width_address_b - log_depth_b_m4k;
//			}
//				
//			connect(gate->pin(1), net_address_b->pin(0+wren_offset));
//			connect(gate->pin(2), net_wren_b->pin(0));
//			des->add_node(gate);
//		}
//		else {
//			for (unsigned idx = 0; idx < depth_b_m4k; idx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_wr_en_b->pin(idx));
//				connect(gate->pin(1), sig_and->pin(idx));
//				connect(gate->pin(2), net_wren_b->pin(0));
//				des->add_node(gate);
//			}
//		}
//	}
//
//	// CCC
//	//NetNet*sig_mem = net_q_b;
//	NetNet*sig_mem_a = net_q_a;
//	NetNet*sig_mem_b = net_q_b;
//	if (log_depth_a_m4k > 0) {
//		sig_mem_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_a*depth_a_m4k);
//		sig_mem_a->local_flag(true);
//	}
//	if (log_depth_b_m4k > 0) {
//		sig_mem_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_b*depth_b_m4k);
//		sig_mem_b->local_flag(true);
//	}
//	for (unsigned depth_b_idx = 0; depth_b_idx < depth_b_m4k; depth_b_idx++) {
//		for (unsigned width_b_idx = 0; width_b_idx < width_b_m4k; width_b_idx++) {
//
//			unsigned data_a = data_a_m4k;
//			unsigned data_b = data_b_m4k;
//			if (width_b_idx == width_b_m4k - 1) {
//				data_a = data_a_last_m4k;
//				data_b = data_b_last_m4k;
//			}
//
//			NetMem*mem = new NetMem(scope, scope->local_symbol(),
//				address_a_m4k,
//				address_b_m4k,
//				data_a,
//				data_b,
//				data_a,
//				data_b);
//			des->add_node(mem);
//
//			//ADU:begin
//			NetExpr* expr_initial_file = scope->parameters["init_file"].expr;
//			if(!expr_initial_file){
//				verinum attri_initial_file("NULL");
//				mem->attribute("init_file",attri_initial_file);
//			}else{
//				NetEConst*econst_initial_file = dynamic_cast<NetEConst*>(expr_initial_file);
//				if(!econst_initial_file){
//					cerr << "Error:" << __LINE__;
//					des->errors++;
//					return;
//				}
//
//				verinum attri_initial_file(econst_initial_file->value());
//				mem->attribute("init_file",attri_initial_file);
//			}
//
//			verinum attri_mode("ture_dual_port");
//			mem->attribute("mode",attri_mode);
//
//			NetScope* up = scope->parent();
//			perm_string up_module_name = up->module_name();			
//			verinum attri_up_module_name(up_module_name.str());
//			mem->attribute("up_module_name",attri_up_module_name);
//
//			std::string up_scope_name = up->name();
//			verinum attri_up_scope_name(up_scope_name);
//			mem->attribute("up_scope_name",attri_up_scope_name);
//			//ADU:end
//
//			//ADU:
//			unsigned port_b_first_address = depth_b_idx*pow(2,double(address_b_m4k));
//			unsigned port_b_last_address = port_b_first_address + pow(2,double(address_b_m4k)) - 1;
//
//			verinum attri_port_b_first_address(port_b_first_address);
//			mem->attribute("port_b_first_address",attri_port_b_first_address);
//			verinum attri_port_b_last_address(port_b_last_address);
//			mem->attribute("port_b_last_address",attri_port_b_last_address);
//
//			//unsigned port_a_first_address = (int(depth_b_idx/ratio)+int(width_b_idx/width_a_m4k))*pow(2,double(address_a_m4k));
//			unsigned port_a_first_address = depth_b_idx*pow(2,double(address_a_m4k));
//			unsigned port_a_last_address = port_a_first_address + pow(2,double(address_a_m4k)) - 1;
//
//			verinum attri_port_a_first_address(port_a_first_address);
//			mem->attribute("port_a_first_address",attri_port_a_first_address);
//			verinum attri_port_a_last_address(port_a_last_address);
//			mem->attribute("port_a_last_address",attri_port_a_last_address);
//
//
//			connect(mem->pin_clocken0(),eval[0]->pin(0));
//			connect(mem->pin_clocken1(),eval[1]->pin(0));
//			// use WR_EN AND gate output here!!!
//			if (ratio >= 1 && log_ratio <= 0)
//				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx/(width_a_m4k/width_b_m4k)));
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx*(depth_a_m4k/depth_b_m4k)+width_b_idx/width_a_m4k));
//			}
//				
//			connect(mem->pin_aclr0(),eval[3]->pin(0));
//			connect(mem->pin_clock0(),eval[4]->pin(0));
//			// use WR_EN AND gate output here!!!
//			connect(mem->pin_wren_b(),sig_wr_en_b->pin(depth_b_idx));
//			connect(mem->pin_aclr1(),eval[6]->pin(0));
//			connect(mem->pin_clock1(),eval[7]->pin(0));
//			//CCC
//			// width_address_a is divided into 2 parts
//			// address_a_m4k, used for M4K's address
//			// log_depth_a_m4k, used for WR_EN AND
//			unsigned address_a_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				address_a_offset = 0;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				address_a_offset = log_depth_a_m4k - log_depth_b_m4k;
//			}
//				
//			for (unsigned idx = 0; idx < address_a_m4k; idx++)
//				connect(mem->pin_address_a(idx),net_address_a->pin(idx+address_a_offset));
//
//			// width_address_b is divided into 2 parts
//			// address_b_m4k, used for M4K's address
//			// log_depth_b_m4k, used for output MUX
//			unsigned address_b_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				address_b_offset = log_depth_b_m4k - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0){
//				assert(0);
//				address_b_offset = 0;
//			}
//				
//			for (unsigned idx = 0; idx < address_b_m4k; idx++)
//				connect(mem->pin_address_b(idx),net_address_b->pin(idx+address_b_offset));
//
//			if (ratio >= 1 && log_ratio <= 0) {
//				unsigned data_a_offset = ( depth_b_idx % ( width_a_m4k / width_b_m4k ) ) * width_data_b + width_b_idx * data_b_m4k;
//				for (unsigned ratio_idx = 0; ratio_idx < ratio_m4k; ratio_idx++) {
//					for (unsigned idx = 0; idx < data_b; idx++) {
//						//ADU:
//						if(ratio_idx == 0 && idx == 0){
//							unsigned port_a_first_bit_number = idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k;
//							verinum attri_port_a_first_bit_number(port_a_first_bit_number);
//							mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
//
//							mem->attribute("port_b_first_bit_number",attri_port_a_first_bit_number);
//						}
//
//						connect(mem->pin_data_a(idx+ratio_idx*data_b),net_data_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
//						if (log_depth_a_m4k == 0)
//							connect(mem->pin_q_a(idx+ratio_idx*data_b),net_q_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
//						else
//							connect(mem->pin_q_a(idx+ratio_idx*data_b),sig_mem_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k+depth_b_idx/(width_a_m4k/width_b_m4k)*width_data_a));
//					}
//				}
//			}
//			else if (ratio < 1 && log_ratio > 0) {
//				assert(0);
//				unsigned data_a_offset = width_b_idx % width_a_m4k * data_a_m4k;
//				for (unsigned idx = 0; idx < data_a; idx++) {
//					//ADU:
//					//if(idx == 0){
//					//	unsigned port_a_first_bit_number = idx+data_a_offset;
//					//	verinum attri_port_a_first_bit_number(port_a_first_bit_number);
//					//	mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
//					//}
//
//					connect(mem->pin_data_a(idx),net_data_a->pin(idx+data_a_offset));
//					if (log_depth_a_m4k == 0)
//						connect(mem->pin_q_a(idx),net_q_a->pin(idx+data_a_offset));
//					else
//						connect(mem->pin_q_a(idx),sig_mem_a->pin(idx+data_a_offset+depth_b_idx*depth_a_m4k/depth_b_m4k*width_data_a));
//				}
//			}
//
//			if (ratio >= 1 && log_ratio <= 0) {
//				unsigned data_b_offset = width_b_idx * data_b_m4k;
//				for (unsigned idx = 0; idx < data_b; idx++) {
//					//ADU:
//					//if(idx == 0){
//					//	unsigned port_b_first_bit_number = idx+data_b_offset;
//					//	verinum attri_port_b_first_bit_number(port_b_first_bit_number);
//					//	mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
//					//}
//
//					connect(mem->pin_data_b(idx),net_data_b->pin(idx+data_b_offset));
//					if (log_depth_b_m4k == 0)
//						connect(mem->pin_q_b(idx),net_q_b->pin(idx+data_b_offset));
//					else
//						connect(mem->pin_q_b(idx),sig_mem_b->pin(idx+depth_b_idx*width_data_b+data_b_offset));
//				}
//			}
//			else if (ratio < 1 && log_ratio > 0) {
//				assert(0);
//				unsigned data_b_offset = width_b_idx * data_a_m4k;
//				for (unsigned ratio_idx = 0; ratio_idx < 1 / ratio_m4k; ratio_idx++) {
//					for (unsigned idx = 0; idx < data_a; idx++) {
//						//ADU:
//						if(ratio_idx == 0 && idx == 0){
//							unsigned port_b_first_bit_number = idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k;
//							verinum attri_port_b_first_bit_number(port_b_first_bit_number);
//							mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
//
//							mem->attribute("port_a_first_bit_number",attri_port_b_first_bit_number);
//						}
//
//						connect(mem->pin_data_b(idx+ratio_idx*data_a),net_data_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
//						if (log_depth_b_m4k == 0)
//							connect(mem->pin_q_b(idx+ratio_idx*data_a),net_q_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
//						else
//							connect(mem->pin_q_b(idx+ratio_idx*data_a),sig_mem_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k+depth_b_idx*width_data_b));
//					}
//				}
//			}
//
//		}
//
//	}
//
//	if ( ratio >= 1 && log_ratio <= 0 ) {
//
//		// need output MUX here!!!
//		// use net_address_a->pin[log_depth_a_m4k]
//		// pin(width_address_a-1:width_address_a-log_depth_a_m4k)
//		// if log_depth_a_m4k > 0
//		if (log_depth_a_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_a; idx++)
//				connect(mux->pin_Result(idx), net_q_a->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++)
//				connect(mux->pin_Sel(idx), net_address_a->pin(idx+width_address_a-log_depth_a_m4k));
//			des->add_node(mux);
//		}
//
//		// need output MUX here!!!
//		// use net_address_b->pin[log_depth_b_m4k]
//		// pin(log_depth_b_m4k-log_depth_a_m4k-1:0) and pin(width_address_b-1:width_address_b-log_depth_a_m4k)
//		// if log_depth_b_m4k > 0
//		if (log_depth_b_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_b; idx++)
//				connect(mux->pin_Result(idx), net_q_b->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
//				if (idx < log_depth_b_m4k - log_depth_a_m4k)
//					connect(mux->pin_Sel(idx), net_address_b->pin(idx));
//				else
//					connect(mux->pin_Sel(idx), net_address_b->pin(idx+width_address_b-log_depth_b_m4k));
//			}
//			des->add_node(mux);
//		}
//
//	}
//
//	else if (ratio < 1 && log_ratio > 0) {
//		assert(0);
//
//		// need output MUX here!!!
//		// use net_address_a->pin[log_depth_a_m4k]
//		// pin(log_depth_a_m4k-log_depth_b_m4k-1:0) and pin(width_address_a-1:width_address_a-log_depth_b_m4k)
//		// if log_depth_a_m4k > 0
//		if (log_depth_a_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_a; idx++)
//				connect(mux->pin_Result(idx), net_q_a->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
//				if (idx < log_depth_a_m4k - log_depth_b_m4k)
//					connect(mux->pin_Sel(idx), net_address_a->pin(idx));
//				else
//					connect(mux->pin_Sel(idx), net_address_a->pin(idx+width_address_a-log_depth_a_m4k));
//			}
//			des->add_node(mux);
//		}
//
//		// need output MUX here!!!
//		// use net_address_b->pin[log_depth_b_m4k]
//		// pin(width_address_b-1:width_address_b-log_depth_b_m4k)
//		// if log_depth_b_m4k > 0
//		if (log_depth_b_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_b; idx++)
//				connect(mux->pin_Result(idx), net_q_b->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++)
//				connect(mux->pin_Sel(idx), net_address_b->pin(idx+width_address_b-log_depth_b_m4k));
//			des->add_node(mux);
//		}
//
//	}
//}
/****************************************************************/






void PMemAssign::elaborate_bidir_dual(Design*des, NetScope*scope) const
{//M9k 综合映射
	assert(scope);
	bool swap_operands = false;	//ADU: width_data_b <= width_data_a
								//wren_a <-> wren_b
								//address_a <-> address_b
								//data_a <-> data_b
								//q_a <-> q_b
	bool fast_m4k_mapping = false;
	// if to favourite fast M4K, not supported yet!
	if (fast_m4k_mapping){
		cerr << "Fast_M4K_mapping not supported yet!" << endl;
		assert(0);
	}

	NetNet*eval[14];
	//clocken0 | clocken1 | wren_a | aclr0 | clock0 | wren_b | aclr1 | clock1 | address_a | address_b | data_a | data_b | q_a | q_b | rden_a | rden_b
	//    0    |     1    |    2   |   3   |    4   |    5   |   6   |    7   |      8    |      9    |    10  |    11  |  12 |  13 |   14   |   15
	for (unsigned idx = 0; idx < 14; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}

		assert(eval[idx]);
	}
	NetNet*net_clocken0		= eval[0];
	NetNet*net_clocken1		= eval[1]; 
	NetNet*net_wren_a		= eval[2];
	NetNet*net_aclr0		= eval[3]; 
	NetNet*net_clock0		= eval[4];
	NetNet*net_wren_b		= eval[5]; 
	NetNet*net_aclr1		= eval[6];
	NetNet*net_clock1		= eval[7];
	NetNet*net_address_a	= eval[8];
	NetNet*net_address_b	= eval[9]; 
	NetNet*net_data_a		= eval[10];
	NetNet*net_data_b		= eval[11]; 
	NetNet*net_q_a			= eval[12];
	NetNet*net_q_b			= eval[13]; 
	// =========================================
	//	add memory partition here !!! --CCC 
	// =========================================

	unsigned width_address_a = net_address_a->pin_count();
	unsigned width_address_b = net_address_b->pin_count();
	unsigned width_data_a = net_data_a->pin_count();
	unsigned width_data_b = net_data_b->pin_count();
	if(width_data_b > width_data_a){
		swap_operands = true;

		net_wren_a = eval[5];
		net_wren_b = eval[2];
		net_data_a = eval[11];
		net_data_b = eval[10];
		net_q_a = eval[13];
		net_q_b = eval[12];
		net_address_a = eval[9];
		net_address_b = eval[8];

		width_address_a = net_address_a->pin_count();
		width_address_b = net_address_b->pin_count();
		width_data_a = net_data_a->pin_count();
		width_data_b = net_data_b->pin_count();
	}
		

	if ( pow(2,(double)width_address_a) * width_data_a != pow(2,(double)width_address_b) * width_data_b ) {
		cerr << "( 2 ^ Width_Address_A(" << width_address_a << ") ) * Width_Data_A(" << width_data_a << ")" << endl;
		cerr << "!= ( 2 ^ Width_Address_B(" << width_address_b << ") ) * Width_Data_B(" << width_data_b << ")" << endl;
		//assert(0);
		des->errors ++;
		cerr << __LINE__ << endl;
		return;
	}
	if (width_data_a > width_data_b) {
		if ( ( width_data_a != width_data_b * 2 ) && ( width_data_a != width_data_b * 4 )
			&& ( width_data_a != width_data_b * 8 ) && ( width_data_a != width_data_b * 16) ) {
				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
				//assert(0);
				des->errors ++;
				cerr << __LINE__ << endl;
				return;
		}
	}
	else if (width_data_a < width_data_b) {
		cerr << "width_data_b cannot > width_data_a\n";
		//assert(0);
		des->errors ++;
		cerr << __LINE__ << endl;
		return;
		
	}

	// double ratio - ratio of width_data_a to width_data_b
	// signed log_ratio - log of ratio of address bits of port A to B
	double ratio = (float)width_data_a / (float)width_data_b;
	signed log_ratio = width_address_a - width_address_b;


	if (ratio * pow(2,(double)log_ratio) != 1) {
		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
		//assert(0);
		des->errors ++;
		cerr << __LINE__ << endl;
		return;
	}
	if ( ratio < 1 ) {
		cerr << "Ratio should not < 1\n";
		//assert(0);
		des->errors ++;
		cerr << __LINE__ << endl;
		return;
	}

	//配置表来源于altera的handbook。
	// M4K的全部备用配置
	//unsigned address[7] = {12,  11,  10,  9,  9,  8,  8};
	//unsigned width[7] = {   1,   2,   4,  9,  8, 18, 16};

	//全部的配置
	//unsigned address[DIVICE] = {13,  12,  11,  10,  10,   9,  9,    8,    8  };
	//unsigned width[DIVICE] =    {  1,     2,   4,     8,    9, 16, 18,  32,  36};

	////控制只用某一种配置
	//unsigned address[DIVICE] = {13  };
	//unsigned width[DIVICE] =    { 1 };

	//所有有意义的配置
	unsigned address[DIVICE] = {13,  12,  11,    10,   9,     8,    8  };
	unsigned width[DIVICE] =    {  1,    2,    4,     8,   16,   32,   36 };


	// memory partition results
	unsigned address_b_m4k;
	unsigned data_b_m4k;
	unsigned data_b_last_m4k;
	unsigned depth_b_m4k = 0;
	unsigned log_depth_b_m4k = 0;
	unsigned width_b_m4k = 0;
	unsigned cost_m4k = 0;

	// ===================================================
	// ===================================================
	//	find the M4K configuration:
	//	address_b_m4k -
	//			address bits of M4K's port B
	//	data_b_m4k -
	//			data width of M4K's port B
	//	data_b_last_m4k -
	//			data width of last M4K's port B
	//	depth_b_m4k -
	//			number of M4K's along depth of port B
	//	log_depth_b_m4k -
	//			log of depth_b_m4k
	//	width_b_m4k -
	//			number of M4K's along width of port B
	// ===================================================
	// ===================================================

	for (unsigned idx = 0; idx < DIVICE; idx++) {
		// from device
		unsigned address_tmp = address[idx];
		unsigned width_tmp = width[idx];
		unsigned width_last_tmp = width[idx];

		if ( ( ( ratio >= 1 ) && ( log_ratio <= 0 ) && ( width_tmp * ratio <= 36 ) ) 
			|| (( ratio < 1 ) && ( ( width_data_b % width_tmp ) % (int) ( 1/ratio ) ) == 0 && ( width_tmp * ratio >= 1 ) ) ) {
			unsigned num_depth = 1;
			unsigned log_num_depth = 0;
			if ( width_address_b >= address_tmp ) {
				num_depth = pow(2,double(width_address_b-address_tmp));
				log_num_depth = width_address_b - address_tmp;
			}
			else
				address_tmp = width_address_b;
			unsigned num_width = 1;
			if ( width_data_b >= width_tmp ) {
				num_width = width_data_b / width_tmp;
				width_last_tmp = width_data_b % width_tmp;
				if (width_last_tmp == 0)
					width_last_tmp = width_tmp;
				else
					num_width++;
			}
			else {
				width_tmp = width_data_b;
				width_last_tmp = width_data_b;
			}
			if ( ( num_width - 1 ) * width_tmp + width_last_tmp != width_data_b ) {
				cerr << "[ Num_Width(" << num_width << ") - 1 ] * Width_Tmp(" << width_tmp << ") + Width_Last_Tmp(" << width_last_tmp << ")" << endl;
				cerr << "!= Width_Data_B(" << width_data_b << ")!" << endl;
				assert(0);
			}

			// here we optimize for:
			// first, timing, which is num_depth
			// secondly, resource, which is num_depth * num_width
			unsigned cost = num_depth * 10000 + num_width;

			if ( ( cost_m4k == 0 ) || ( cost < cost_m4k ) ) {
				if ( ( ( ratio <= 1 ) && ( log_ratio >= 0 ) ) || ( num_depth * pow(2,(double)(address[idx]-8)) >= ratio ) ) {
					address_b_m4k = address_tmp;
					data_b_m4k = width_tmp;
					data_b_last_m4k = width_last_tmp;
					depth_b_m4k = num_depth;
					log_depth_b_m4k = log_num_depth;
					width_b_m4k = num_width;
					cost_m4k = cost;
				}
			}
		}
	}

	// ============================================================
	// ============================================================
	//	find the M4K configuration (continued):
	//	address_a_m4k -
	//			address bits of M4K's port A
	//	data_a_m4k -
	//			data width of M4K's port A
	//	data_a_last_m4k -
	//			data width of last M4K's port A
	//	depth_a_m4k -
	//			number of M4K's along depth of port A
	//	log_depth_a_m4k -
	//			log of depth_a_m4k
	//	width_a_m4k -
	//			number of M4K's along width of port A
	//	ratio_m4k -
	//			ratio of data_a_m4k to data_b_m4k
	//	log_ratio_m4k -
	//			log of ratio of address_a_m4k to address_b_m4k
	// ============================================================
	// ============================================================

	unsigned address_a_m4k = address_b_m4k;
	unsigned data_a_m4k = data_b_m4k;
	unsigned data_a_last_m4k = data_b_last_m4k;
	double ratio_m4k = 1;
	signed log_ratio_m4k = 0;
	unsigned depth_a_m4k = depth_b_m4k;
	unsigned log_depth_a_m4k = log_depth_b_m4k;
	unsigned width_a_m4k = width_b_m4k;
	// if compared to port B, port A is fat
	if ( ( ratio > 1 ) && ( log_ratio < 0 ) ) {
		// to construct port A
		// first, a coarser grain construction on the number of M4K's along the depth and width
		// each iteration, depth_a_m4k is / 2, while width_a_m4k is * 2
		/*while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) && ( depth_a_m4k % 2 == 0 ) ) {
		depth_a_m4k = depth_a_m4k / 2;
		log_depth_a_m4k--;
		width_a_m4k = width_a_m4k * 2;
		}*/
		// secondly, a finer grain construction on the M4K's depth and width
		// each iteration, M4K's depth is / 2, while M4K's width is * 2
		while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) /*&& ( address_a_m4k > 4 ) */) {
			address_a_m4k--;
			data_a_m4k = data_a_m4k * 2;
			data_a_last_m4k = data_a_last_m4k * 2;
			ratio_m4k = ratio_m4k * 2;
			log_ratio_m4k--;
		}
	}
	// else if compared to port B, port A is tall
	else if ( ( ratio < 1 ) && ( log_ratio > 0 ) ) {
		assert(0);
		// to construct port A
		// first, a finer grain construction on the M4K's depth and width
		// each iteration, M4K's depth is * 2, while M4K's width is / 2
		while ( ( ratio_m4k > ratio ) && ( data_a_m4k % 2 == 0 ) ) {
			address_a_m4k++;
			data_a_m4k = data_a_m4k / 2;
			data_a_last_m4k = data_a_last_m4k / 2;
			ratio_m4k = ratio_m4k / 2;
			log_ratio_m4k++;
		}
		// secondly, a coarser grain construction on the number of M4K's along the depth and width
		// each iteration, depth_a_m4k is * 2, while width_a_m4k is / 2
		/*while ( ( ratio_m4k * width_a_m4k / width_b_m4k > ratio ) && ( width_a_m4k % 2 == 0 ) ) {
		depth_a_m4k = depth_a_m4k * 2;
		log_depth_a_m4k++;
		width_a_m4k = width_a_m4k / 2;
		}*/
	}
	if ( ( width_a_m4k - 1 ) * data_a_m4k + data_a_last_m4k != width_data_a ) {
		cerr << "[ Width_A_M4K(" << width_a_m4k << ") - 1 ] * Data_A_M4K(" << data_a_m4k << ") + Data_A_Last_M4K(" << data_a_last_m4k << ")" << endl;
		cerr << "!= Width_Data_A(" << width_data_a << ")!" << endl;
		//assert(0);
		cerr << __LINE__<< endl;
		des->errors ++;
		return;
	}

	// ====================
	//	generate M4K's
	// ====================

	if (depth_b_m4k * width_b_m4k != depth_a_m4k * width_a_m4k) {
		cerr << "Depth_B_M4K(" << depth_b_m4k << ") * Width_B_M4K(" << width_b_m4k << ")" << endl;
		cerr << "!= Depth_A_M4K(" << depth_a_m4k << ") * Width_A_M4K, error!" << endl;
		//assert(0);
		cerr << __LINE__<< endl;
		des->errors ++;
		return;
	}

	if (address_a_m4k + log_depth_a_m4k != width_address_a) {
		cerr << "Address_A_M4K(" << address_a_m4k << ") + Log_Depth_A_M4K(" << log_depth_a_m4k << ")" << endl;
		cerr << "!= Width_Address_A(" << width_address_a << "), error!" << endl;
		//assert(0);
		cerr << __LINE__<< endl;
		des->errors ++;
		return;
	}
	if (address_b_m4k + log_depth_b_m4k != width_address_b) {
		cerr << "Address_B_M4K(" << address_b_m4k << ") + Log_Depth_B_M4K(" << log_depth_b_m4k << ")" << endl;
		cerr << "!= Width_Address_B(" << width_address_b << "), error!" << endl;
		//assert(0);
		cerr << __LINE__<< endl;
		des->errors ++;
		return;
	}

	// need WR_EN AND here!!!
	// use net_address_a->pin[log_depth_a_m4k]
	// if log_depth_a_m4k > 0
	NetNet* sig_not = 0;
	NetNet* sig_and = 0;
	NetNet* sig_and_old = 0;
	NetNet* sig_wr_en_a = net_wren_a;
	if (log_depth_a_m4k > 0) {
		// first, generate NOT's for each net_address_a->pin[log_depth_a_m4k]
		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_a-1:width_address_a-log_depth_a_m4k)
		// else if ratio < 1 && log_ratio > 0, use pin(width_address_a-1:width_address_a-log_depth_b_m4k) and pin(log_depth_a_m4k-log_depth_b_m4k-1:0)
		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_a_m4k);
		sig_not->local_flag(true);
		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
			// pin 0 - output
			connect(gate->pin(0), sig_not->pin(idx));
			// pin 1 - input
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				wren_offset = width_address_a - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				if (idx < log_depth_a_m4k - log_depth_b_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_a - log_depth_a_m4k;
			}			
			connect(gate->pin(1), net_address_a->pin(idx+wren_offset));
			des->add_node(gate);
		}

		// secondly, generate AND's using net_address_a->pin[log_depth_a_m4k] and sig_not
		for (unsigned idx = 1; idx < log_depth_a_m4k; idx++) {
			sig_and_old = sig_and;
			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
			sig_and->local_flag(true);
			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_and->pin(jdx));
				if (idx == 1) {
					if (jdx % 2 == 0)
						connect(gate->pin(1), sig_not->pin(0));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							if (0 < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						}							
						connect(gate->pin(1), net_address_a->pin(0+wren_offset));
					}
					if (jdx < 2)
						connect(gate->pin(2), sig_not->pin(1));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							if (1 < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						}							
						connect(gate->pin(2), net_address_a->pin(1+wren_offset));
					}
				}
				else {
					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
					if (jdx < pow(2,(double)idx))
						connect(gate->pin(2), sig_not->pin(idx));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							wren_offset = width_address_a - log_depth_a_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							if (idx < log_depth_a_m4k - log_depth_b_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_a - log_depth_a_m4k;
						}							
						connect(gate->pin(2), net_address_a->pin(idx+wren_offset));
					}
				}
				des->add_node(gate);
			}
		}

		// thirdly, generate AND's using sig_and and wr_en
		sig_wr_en_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_a_m4k);
		sig_wr_en_a->local_flag(true);
		if (log_depth_a_m4k == 1) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(0));
			connect(gate->pin(1), sig_not->pin(0));
			connect(gate->pin(2), net_wren_a->pin(0));
			des->add_node(gate);
			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_a->pin(1));
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				wren_offset = width_address_a - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				if (0 < log_depth_a_m4k - log_depth_b_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_a - log_depth_a_m4k;
			}				
			connect(gate->pin(1), net_address_a->pin(0+wren_offset));
			connect(gate->pin(2), net_wren_a->pin(0));
			des->add_node(gate);
		}
		else {
			for (unsigned idx = 0; idx < depth_a_m4k; idx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_wr_en_a->pin(idx));
				connect(gate->pin(1), sig_and->pin(idx));
				connect(gate->pin(2), net_wren_a->pin(0));
				des->add_node(gate);
			}
		}
	}

	// need WR_EN AND here!!!
	// use net_address_b->pin[log_depth_b_m4k]
	// if log_depth_b_m4k > 0
	//NetNet* sig_not = 0;
	//NetNet* sig_and = 0;
	//NetNet* sig_and_old = 0;
	//NetNet* sig_wr_en = net_wren_a;
	NetNet* sig_wr_en_b = net_wren_b;
	if (log_depth_b_m4k > 0) {
		// first, generate NOT's for each net_address_b->pin[log_depth_b_m4k]
		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_b-1:width_address_b-log_depth_a_m4k) and pin(log_depth_b_m4k-log_depth_a_m4k-1:0)
		// else if ratio < 1 && log_ratio > 0, use pin(width_address_b-1:width_address_b-log_depth_b_m4k)
		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_b_m4k);
		sig_not->local_flag(true);
		for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
			// pin 0 - output
			connect(gate->pin(0), sig_not->pin(idx));
			// pin 1 - input
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				if (idx < log_depth_b_m4k - log_depth_a_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_b - log_depth_b_m4k;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				wren_offset = width_address_b - log_depth_b_m4k;
			}
			
			connect(gate->pin(1), net_address_b->pin(idx+wren_offset));
			des->add_node(gate);
		}

		// secondly, generate AND's using net_address_b->pin[log_depth_b_m4k] and sig_not
		for (unsigned idx = 1; idx < log_depth_b_m4k; idx++) {
			sig_and_old = sig_and;
			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
			sig_and->local_flag(true);
			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_and->pin(jdx));
				if (idx == 1) {
					if (jdx % 2 == 0)
						connect(gate->pin(1), sig_not->pin(0));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							if (0 < log_depth_b_m4k - log_depth_a_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_b - log_depth_b_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							wren_offset = width_address_b - log_depth_b_m4k;
						}
							
						connect(gate->pin(1), net_address_b->pin(0+wren_offset));
					}
					if (jdx < 2)
						connect(gate->pin(2), sig_not->pin(1));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							if (1 < log_depth_b_m4k - log_depth_a_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_b - log_depth_b_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							wren_offset = width_address_b - log_depth_b_m4k;
						}
							
						connect(gate->pin(2), net_address_b->pin(1+wren_offset));
					}
				}
				else {
					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
					if (jdx < pow(2,(double)idx))
						connect(gate->pin(2), sig_not->pin(idx));
					else {
						unsigned wren_offset;
						if (ratio >= 1 && log_ratio <= 0)
							if (idx < log_depth_b_m4k - log_depth_a_m4k)
								wren_offset = 0;
							else
								wren_offset = width_address_b - log_depth_b_m4k;
						else if (ratio < 1 && log_ratio > 0){
							assert(0);
							wren_offset = width_address_b - log_depth_b_m4k;
						}
							
						connect(gate->pin(2), net_address_b->pin(idx+wren_offset));
					}
				}
				des->add_node(gate);
			}
		}

		// thirdly, generate AND's using sig_and and wr_en
		sig_wr_en_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_b_m4k);
		sig_wr_en_b->local_flag(true);
		if (log_depth_b_m4k == 1) {
			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_b->pin(0));
			connect(gate->pin(1), sig_not->pin(0));
			connect(gate->pin(2), net_wren_b->pin(0));
			des->add_node(gate);
			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
			connect(gate->pin(0), sig_wr_en_b->pin(1));
			unsigned wren_offset;
			if (ratio >= 1 && log_ratio <= 0)
				if (0 < log_depth_b_m4k - log_depth_a_m4k)
					wren_offset = 0;
				else
					wren_offset = width_address_b - log_depth_b_m4k;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				wren_offset = width_address_b - log_depth_b_m4k;
			}
				
			connect(gate->pin(1), net_address_b->pin(0+wren_offset));
			connect(gate->pin(2), net_wren_b->pin(0));
			des->add_node(gate);
		}
		else {
			for (unsigned idx = 0; idx < depth_b_m4k; idx++) {
				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
				connect(gate->pin(0), sig_wr_en_b->pin(idx));
				connect(gate->pin(1), sig_and->pin(idx));
				connect(gate->pin(2), net_wren_b->pin(0));
				des->add_node(gate);
			}
		}
	}

	// CCC
	//NetNet*sig_mem = net_q_b;
	NetNet*sig_mem_a = net_q_a;
	NetNet*sig_mem_b = net_q_b;
	if (log_depth_a_m4k > 0) {
		sig_mem_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_a*depth_a_m4k);
		sig_mem_a->local_flag(true);
	}
	if (log_depth_b_m4k > 0) {
		sig_mem_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_b*depth_b_m4k);
		sig_mem_b->local_flag(true);
	}
	for (unsigned depth_b_idx = 0; depth_b_idx < depth_b_m4k; depth_b_idx++) {
		for (unsigned width_b_idx = 0; width_b_idx < width_b_m4k; width_b_idx++) {

			unsigned data_a = data_a_m4k;
			unsigned data_b = data_b_m4k;
			if (width_b_idx == width_b_m4k - 1) {
				data_a = data_a_last_m4k;
				data_b = data_b_last_m4k;
			}

			NetMem*mem = new NetMem(scope, scope->local_symbol(),
				address_a_m4k,
				address_b_m4k,
				data_a,
				data_b,
				data_a,
				data_b);
			des->add_node(mem);

			//ADU:begin
			NetExpr* expr_initial_file = scope->parameters["init_file"].expr;
			if(!expr_initial_file){
				verinum attri_initial_file("NULL");
				mem->attribute("init_file",attri_initial_file);
			}else{
				NetEConst*econst_initial_file = dynamic_cast<NetEConst*>(expr_initial_file);
				if(!econst_initial_file){
					cerr << "Error:" << __LINE__;
					des->errors++;
					return;
				}

				verinum attri_initial_file(econst_initial_file->value());
				mem->attribute("init_file",attri_initial_file);
			}

			verinum attri_mode("ture_dual_port");
			mem->attribute("mode",attri_mode);

			NetScope* up = scope->parent();
			perm_string up_module_name = up->module_name();			
			verinum attri_up_module_name(up_module_name.str());
			mem->attribute("up_module_name",attri_up_module_name);

			std::string up_scope_name = up->name();
			verinum attri_up_scope_name(up_scope_name);
			mem->attribute("up_scope_name",attri_up_scope_name);
			//ADU:end

			//ADU:
			unsigned port_b_first_address = depth_b_idx*pow(2,double(address_b_m4k));
			unsigned port_b_last_address = port_b_first_address + pow(2,double(address_b_m4k)) - 1;

			verinum attri_port_b_first_address(port_b_first_address);
			mem->attribute("port_b_first_address",attri_port_b_first_address);
			verinum attri_port_b_last_address(port_b_last_address);
			mem->attribute("port_b_last_address",attri_port_b_last_address);

			//unsigned port_a_first_address = (int(depth_b_idx/ratio)+int(width_b_idx/width_a_m4k))*pow(2,double(address_a_m4k));
			unsigned port_a_first_address = depth_b_idx*pow(2,double(address_a_m4k));
			unsigned port_a_last_address = port_a_first_address + pow(2,double(address_a_m4k)) - 1;

			verinum attri_port_a_first_address(port_a_first_address);
			mem->attribute("port_a_first_address",attri_port_a_first_address);
			verinum attri_port_a_last_address(port_a_last_address);
			mem->attribute("port_a_last_address",attri_port_a_last_address);


			connect(mem->pin_clocken0(),eval[0]->pin(0));
			connect(mem->pin_clocken1(),eval[1]->pin(0));
			// use WR_EN AND gate output here!!!
			if (ratio >= 1 && log_ratio <= 0)
				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx/(width_a_m4k/width_b_m4k)));
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx*(depth_a_m4k/depth_b_m4k)+width_b_idx/width_a_m4k));
			}
				
			connect(mem->pin_aclr0(),eval[3]->pin(0));
			connect(mem->pin_clock0(),eval[4]->pin(0));
			// use WR_EN AND gate output here!!!
			connect(mem->pin_wren_b(),sig_wr_en_b->pin(depth_b_idx));
			connect(mem->pin_aclr1(),eval[6]->pin(0));
			connect(mem->pin_clock1(),eval[7]->pin(0));
			//CCC
			// width_address_a is divided into 2 parts
			// address_a_m4k, used for M4K's address
			// log_depth_a_m4k, used for WR_EN AND
			unsigned address_a_offset;
			if (ratio >= 1 && log_ratio <= 0)
				address_a_offset = 0;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				address_a_offset = log_depth_a_m4k - log_depth_b_m4k;
			}
				
			for (unsigned idx = 0; idx < address_a_m4k; idx++)
				connect(mem->pin_address_a(idx),net_address_a->pin(idx+address_a_offset));

			// width_address_b is divided into 2 parts
			// address_b_m4k, used for M4K's address
			// log_depth_b_m4k, used for output MUX
			unsigned address_b_offset;
			if (ratio >= 1 && log_ratio <= 0)
				address_b_offset = log_depth_b_m4k - log_depth_a_m4k;
			else if (ratio < 1 && log_ratio > 0){
				assert(0);
				address_b_offset = 0;
			}
				
			for (unsigned idx = 0; idx < address_b_m4k; idx++)
				connect(mem->pin_address_b(idx),net_address_b->pin(idx+address_b_offset));

			if (ratio >= 1 && log_ratio <= 0) {
				unsigned data_a_offset = ( depth_b_idx % ( width_a_m4k / width_b_m4k ) ) * width_data_b + width_b_idx * data_b_m4k;
				for (unsigned ratio_idx = 0; ratio_idx < ratio_m4k; ratio_idx++) {
					for (unsigned idx = 0; idx < data_b; idx++) {
						//ADU:
						if(ratio_idx == 0 && idx == 0){
							unsigned port_a_first_bit_number = idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k;
							verinum attri_port_a_first_bit_number(port_a_first_bit_number);
							mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);

							mem->attribute("port_b_first_bit_number",attri_port_a_first_bit_number);
						}

						connect(mem->pin_data_a(idx+ratio_idx*data_b),net_data_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
						if (log_depth_a_m4k == 0)
							connect(mem->pin_q_a(idx+ratio_idx*data_b),net_q_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
						else
							connect(mem->pin_q_a(idx+ratio_idx*data_b),sig_mem_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k+depth_b_idx/(width_a_m4k/width_b_m4k)*width_data_a));
					}
				}
			}
			else if (ratio < 1 && log_ratio > 0) {
				assert(0);
				unsigned data_a_offset = width_b_idx % width_a_m4k * data_a_m4k;
				for (unsigned idx = 0; idx < data_a; idx++) {
					//ADU:
					//if(idx == 0){
					//	unsigned port_a_first_bit_number = idx+data_a_offset;
					//	verinum attri_port_a_first_bit_number(port_a_first_bit_number);
					//	mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
					//}

					connect(mem->pin_data_a(idx),net_data_a->pin(idx+data_a_offset));
					if (log_depth_a_m4k == 0)
						connect(mem->pin_q_a(idx),net_q_a->pin(idx+data_a_offset));
					else
						connect(mem->pin_q_a(idx),sig_mem_a->pin(idx+data_a_offset+depth_b_idx*depth_a_m4k/depth_b_m4k*width_data_a));
				}
			}

			if (ratio >= 1 && log_ratio <= 0) {
				unsigned data_b_offset = width_b_idx * data_b_m4k;
				for (unsigned idx = 0; idx < data_b; idx++) {
					//ADU:
					//if(idx == 0){
					//	unsigned port_b_first_bit_number = idx+data_b_offset;
					//	verinum attri_port_b_first_bit_number(port_b_first_bit_number);
					//	mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
					//}

					connect(mem->pin_data_b(idx),net_data_b->pin(idx+data_b_offset));
					if (log_depth_b_m4k == 0)
						connect(mem->pin_q_b(idx),net_q_b->pin(idx+data_b_offset));
					else
						connect(mem->pin_q_b(idx),sig_mem_b->pin(idx+depth_b_idx*width_data_b+data_b_offset));
				}
			}
			else if (ratio < 1 && log_ratio > 0) {
				assert(0);
				unsigned data_b_offset = width_b_idx * data_a_m4k;
				for (unsigned ratio_idx = 0; ratio_idx < 1 / ratio_m4k; ratio_idx++) {
					for (unsigned idx = 0; idx < data_a; idx++) {
						//ADU:
						if(ratio_idx == 0 && idx == 0){
							unsigned port_b_first_bit_number = idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k;
							verinum attri_port_b_first_bit_number(port_b_first_bit_number);
							mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);

							mem->attribute("port_a_first_bit_number",attri_port_b_first_bit_number);
						}

						connect(mem->pin_data_b(idx+ratio_idx*data_a),net_data_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
						if (log_depth_b_m4k == 0)
							connect(mem->pin_q_b(idx+ratio_idx*data_a),net_q_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
						else
							connect(mem->pin_q_b(idx+ratio_idx*data_a),sig_mem_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k+depth_b_idx*width_data_b));
					}
				}
			}

		}

	}

	if ( ratio >= 1 && log_ratio <= 0 ) {

		// need output MUX here!!!
		// use net_address_a->pin[log_depth_a_m4k]
		// pin(width_address_a-1:width_address_a-log_depth_a_m4k)
		// if log_depth_a_m4k > 0
		if (log_depth_a_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_a; idx++)
				connect(mux->pin_Result(idx), net_q_a->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++)
				connect(mux->pin_Sel(idx), net_address_a->pin(idx+width_address_a-log_depth_a_m4k));
			des->add_node(mux);
		}

		// need output MUX here!!!
		// use net_address_b->pin[log_depth_b_m4k]
		// pin(log_depth_b_m4k-log_depth_a_m4k-1:0) and pin(width_address_b-1:width_address_b-log_depth_a_m4k)
		// if log_depth_b_m4k > 0
		if (log_depth_b_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_b; idx++)
				connect(mux->pin_Result(idx), net_q_b->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
				if (idx < log_depth_b_m4k - log_depth_a_m4k)
					connect(mux->pin_Sel(idx), net_address_b->pin(idx));
				else
					connect(mux->pin_Sel(idx), net_address_b->pin(idx+width_address_b-log_depth_b_m4k));
			}
			des->add_node(mux);
		}

	}

	else if (ratio < 1 && log_ratio > 0) {
		assert(0);

		// need output MUX here!!!
		// use net_address_a->pin[log_depth_a_m4k]
		// pin(log_depth_a_m4k-log_depth_b_m4k-1:0) and pin(width_address_a-1:width_address_a-log_depth_b_m4k)
		// if log_depth_a_m4k > 0
		if (log_depth_a_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_a; idx++)
				connect(mux->pin_Result(idx), net_q_a->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
				if (idx < log_depth_a_m4k - log_depth_b_m4k)
					connect(mux->pin_Sel(idx), net_address_a->pin(idx));
				else
					connect(mux->pin_Sel(idx), net_address_a->pin(idx+width_address_a-log_depth_a_m4k));
			}
			des->add_node(mux);
		}

		// need output MUX here!!!
		// use net_address_b->pin[log_depth_b_m4k]
		// pin(width_address_b-1:width_address_b-log_depth_b_m4k)
		// if log_depth_b_m4k > 0
		if (log_depth_b_m4k > 0) {
			// NetMux(xx, xx, data_width, mux_input_num, select_width);
			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
			// Connect the data inputs
			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
			// Connect the output
			for (unsigned idx = 0; idx < width_data_b; idx++)
				connect(mux->pin_Result(idx), net_q_b->pin(idx));
			// Connect the select input
			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++)
				connect(mux->pin_Sel(idx), net_address_b->pin(idx+width_address_b-log_depth_b_m4k));
			des->add_node(mux);
		}

	}
}

//void PMemAssign::elaborate_bidir_dual(Design*des, NetScope*scope) const
//{
//	assert(scope);
//	bool swap_operands = false;//ADU: width_data_b <= width_data_a
//	bool fast_m4k_mapping = false;
//	// if to favourite fast M4K, not supported yet!
//	if (fast_m4k_mapping){
//		cerr << "Fast_M4K_mapping not supported yet!" << endl;
//		assert(0);
//	}
//
//	NetNet*eval[14];
//	//clocken0 | clocken1 | wren_a | aclr0 | clock0 | wren_b | aclr1 | clock1 | address_a | address_b | data_a | data_b | q_a | q_b
//	//    0    |     1    |    2   |   3   |    4   |    5   |   6   |    7   |      8    |      9    |    10  |    11  |  12 |  13
//	for (unsigned idx = 0; idx < 14; idx++) {
//		assert(pin(idx));
//
//		/* Elaborate the r-value. Account for the initial decays,
//		which are going to be attached to the last gate before the
//		generated NetNet. */
//		eval[idx] = pin(idx)->elaborate_net(des, scope,
//			1, 0, 0, 0, Link::STRONG, Link::STRONG);
//		if (eval[idx] == 0) {
//			cerr << get_line() << ": error: Unable to elaborate value: "
//				<< *pin(idx) << endl;
//			des->errors += 1;
//			return;
//		}
//
//		assert(eval[idx]);
//	}
//
//	// =========================================
//	//	add memory partition here !!! --CCC 
//	// =========================================
//
//	unsigned width_address_a = eval[8]->pin_count();
//	unsigned width_address_b = eval[9]->pin_count();
//	unsigned width_data_a = eval[10]->pin_count();
//	unsigned width_data_b = eval[11]->pin_count();
//	if(width_data_b > width_address_a)
//		swap_operands = true;
//
//	if ( pow(2,(double)width_address_a) * width_data_a != pow(2,(double)width_address_b) * width_data_b ) {
//		cerr << "( 2 ^ Width_Address_A(" << width_address_a << ") ) * Width_Data_A(" << width_data_a << ")" << endl;
//		cerr << "!= ( 2 ^ Width_Address_B(" << width_address_b << ") ) * Width_Data_B(" << width_data_b << ")" << endl;
//		assert(0);
//	}
//	if (width_data_a > width_data_b) {
//		if ( ( width_data_a != width_data_b * 2 ) && ( width_data_a != width_data_b * 4 )
//			&& ( width_data_a != width_data_b * 8 ) && ( width_data_a != width_data_b * 16) ) {
//				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
//				assert(0);
//		}
//	}
//	else if (width_data_a < width_data_b) {
//		if ( ( width_data_b != width_data_a * 2 ) && ( width_data_b != width_data_a * 4 )
//			&& ( width_data_b != width_data_a * 8 ) && ( width_data_b != width_data_a * 16) ) {
//				cerr << "Width_Data_A(" << width_data_a << "), Width_Data_B(" << width_data_b << "), not supported!" << endl;
//				assert(0);
//		}
//	}
//
//	// double ratio - ratio of width_data_a to width_data_b
//	// signed log_ratio - log of ratio of address bits of port A to B
//	double ratio = (float)width_data_a / (float)width_data_b;
//	signed log_ratio = width_address_a - width_address_b;
//	if (ratio * pow(2,(double)log_ratio) != 1) {
//		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
//		assert(0);
//	}
//	if ( ( ratio < 1 ) && ( (int) ( 1/ratio ) ) != pow(2,(double)log_ratio) ) {
//		cerr << "Ratio = " << ratio << ", Log_Ratio = " << log_ratio << ", error!" << endl;
//		assert(0);
//	}
//
//	// from device
//	unsigned address[7] = {12,  11,  10,  9,  9,  8,  8};
//	unsigned width[7] = {   1,   2,   4,  9,  8, 18, 16};
//
//	// memory partition results
//	unsigned address_b_m4k;
//	unsigned data_b_m4k;
//	unsigned data_b_last_m4k;
//	unsigned depth_b_m4k = 0;
//	unsigned log_depth_b_m4k = 0;
//	unsigned width_b_m4k = 0;
//	unsigned cost_m4k = 0;
//
//	// ===================================================
//	// ===================================================
//	//	find the M4K configuration:
//	//	address_b_m4k -
//	//			address bits of M4K's port B
//	//	data_b_m4k -
//	//			data width of M4K's port B
//	//	data_b_last_m4k -
//	//			data width of last M4K's port B
//	//	depth_b_m4k -
//	//			number of M4K's along depth of port B
//	//	log_depth_b_m4k -
//	//			log of depth_b_m4k
//	//	width_b_m4k -
//	//			number of M4K's along width of port B
//	// ===================================================
//	// ===================================================
//
//	for (unsigned idx = 0; idx < 7; idx++) {
//		// from device
//		unsigned address_tmp = address[idx];
//		unsigned width_tmp = width[idx];
//		unsigned width_last_tmp = width[idx];
//
//		if ( ( ( ratio >= 1 ) && ( log_ratio <= 0 ) ) || ( ( ( width_data_b % width_tmp ) % (int) ( 1/ratio ) ) == 0 ) ) {
//			unsigned num_depth = 1;
//			unsigned log_num_depth = 0;
//			if ( width_address_b >= address_tmp ) {
//				num_depth = pow(2,double(width_address_b-address_tmp));
//				log_num_depth = width_address_b - address_tmp;
//			}
//			else
//				address_tmp = width_address_b;
//			unsigned num_width = 1;
//			if ( width_data_b >= width_tmp ) {
//				num_width = width_data_b / width_tmp;
//				width_last_tmp = width_data_b % width_tmp;
//				if (width_last_tmp == 0)
//					width_last_tmp = width_tmp;
//				else
//					num_width++;
//			}
//			else {
//				width_tmp = width_data_b;
//				width_last_tmp = width_data_b;
//			}
//			if ( ( num_width - 1 ) * width_tmp + width_last_tmp != width_data_b ) {
//				cerr << "[ Num_Width(" << num_width << ") - 1 ] * Width_Tmp(" << width_tmp << ") + Width_Last_Tmp(" << width_last_tmp << ")" << endl;
//				cerr << "!= Width_Data_B(" << width_data_b << ")!" << endl;
//				assert(0);
//			}
//
//			// here we optimize for:
//			// first, timing, which is num_depth
//			// secondly, resource, which is num_depth * num_width
//			unsigned cost = num_depth * 10000 + num_width;
//
//			if ( ( cost_m4k == 0 ) || ( cost < cost_m4k ) ) {
//				if ( ( ( ratio <= 1 ) && ( log_ratio >= 0 ) ) || ( num_depth * pow(2,(double)(address[idx]-8)) >= ratio ) ) {
//					address_b_m4k = address_tmp;
//					data_b_m4k = width_tmp;
//					data_b_last_m4k = width_last_tmp;
//					depth_b_m4k = num_depth;
//					log_depth_b_m4k = log_num_depth;
//					width_b_m4k = num_width;
//					cost_m4k = cost;
//				}
//			}
//		}
//	}
//
//	// ============================================================
//	// ============================================================
//	//	find the M4K configuration (continued):
//	//	address_a_m4k -
//	//			address bits of M4K's port A
//	//	data_a_m4k -
//	//			data width of M4K's port A
//	//	data_a_last_m4k -
//	//			data width of last M4K's port A
//	//	depth_a_m4k -
//	//			number of M4K's along depth of port A
//	//	log_depth_a_m4k -
//	//			log of depth_a_m4k
//	//	width_a_m4k -
//	//			number of M4K's along width of port A
//	//	ratio_m4k -
//	//			ratio of data_a_m4k to data_b_m4k
//	//	log_ratio_m4k -
//	//			log of ratio of address_a_m4k to address_b_m4k
//	// ============================================================
//	// ============================================================
//
//	unsigned address_a_m4k = address_b_m4k;
//	unsigned data_a_m4k = data_b_m4k;
//	unsigned data_a_last_m4k = data_b_last_m4k;
//	double ratio_m4k = 1;
//	signed log_ratio_m4k = 0;
//	unsigned depth_a_m4k = depth_b_m4k;
//	unsigned log_depth_a_m4k = log_depth_b_m4k;
//	unsigned width_a_m4k = width_b_m4k;
//	// if compared to port B, port A is fat
//	if ( ( ratio > 1 ) && ( log_ratio < 0 ) ) {
//		// to construct port A
//		// first, a coarser grain construction on the number of M4K's along the depth and width
//		// each iteration, depth_a_m4k is / 2, while width_a_m4k is * 2
//		while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) && ( depth_a_m4k % 2 == 0 ) ) {
//			depth_a_m4k = depth_a_m4k / 2;
//			log_depth_a_m4k--;
//			width_a_m4k = width_a_m4k * 2;
//		}
//		// secondly, a finer grain construction on the M4K's depth and width
//		// each iteration, M4K's depth is / 2, while M4K's width is * 2
//		while ( ( ratio_m4k * width_a_m4k / width_b_m4k < ratio ) && ( address_a_m4k > 4 ) ) {
//			address_a_m4k--;
//			data_a_m4k = data_a_m4k * 2;
//			data_a_last_m4k = data_a_last_m4k * 2;
//			ratio_m4k = ratio_m4k * 2;
//			log_ratio_m4k--;
//		}
//	}
//	// else if compared to port B, port A is tall
//	else if ( ( ratio < 1 ) && ( log_ratio > 0 ) ) {
//		// to construct port A
//		// first, a finer grain construction on the M4K's depth and width
//		// each iteration, M4K's depth is * 2, while M4K's width is / 2
//		while ( ( ratio_m4k > ratio ) && ( data_a_m4k % 2 == 0 ) ) {
//			address_a_m4k++;
//			data_a_m4k = data_a_m4k / 2;
//			data_a_last_m4k = data_a_last_m4k / 2;
//			ratio_m4k = ratio_m4k / 2;
//			log_ratio_m4k++;
//		}
//		// secondly, a coarser grain construction on the number of M4K's along the depth and width
//		// each iteration, depth_a_m4k is * 2, while width_a_m4k is / 2
//		while ( ( ratio_m4k * width_a_m4k / width_b_m4k > ratio ) && ( width_a_m4k % 2 == 0 ) ) {
//			depth_a_m4k = depth_a_m4k * 2;
//			log_depth_a_m4k++;
//			width_a_m4k = width_a_m4k / 2;
//		}
//	}
//	if ( ( width_a_m4k - 1 ) * data_a_m4k + data_a_last_m4k != width_data_a ) {
//		cerr << "[ Width_A_M4K(" << width_a_m4k << ") - 1 ] * Data_A_M4K(" << data_a_m4k << ") + Data_A_Last_M4K(" << data_a_last_m4k << ")" << endl;
//		cerr << "!= Width_Data_A(" << width_data_a << ")!" << endl;
//		assert(0);
//	}
//
//	// ====================
//	//	generate M4K's
//	// ====================
//
//	if (depth_b_m4k * width_b_m4k != depth_a_m4k * width_a_m4k) {
//		cerr << "Depth_B_M4K(" << depth_b_m4k << ") * Width_B_M4K(" << width_b_m4k << ")" << endl;
//		cerr << "!= Depth_A_M4K(" << depth_a_m4k << ") * Width_A_M4K, error!" << endl;
//		assert(0);
//	}
//
//	if (address_a_m4k + log_depth_a_m4k != width_address_a) {
//		cerr << "Address_A_M4K(" << address_a_m4k << ") + Log_Depth_A_M4K(" << log_depth_a_m4k << ")" << endl;
//		cerr << "!= Width_Address_A(" << width_address_a << "), error!" << endl;
//		assert(0);
//	}
//	if (address_b_m4k + log_depth_b_m4k != width_address_b) {
//		cerr << "Address_B_M4K(" << address_b_m4k << ") + Log_Depth_B_M4K(" << log_depth_b_m4k << ")" << endl;
//		cerr << "!= Width_Address_B(" << width_address_b << "), error!" << endl;
//		assert(0);
//	}
//
//	// need WR_EN AND here!!!
//	// use eval[8]->pin[log_depth_a_m4k]
//	// if log_depth_a_m4k > 0
//	NetNet* sig_not = 0;
//	NetNet* sig_and = 0;
//	NetNet* sig_and_old = 0;
//	NetNet* sig_wr_en_a = eval[2];
//	if (log_depth_a_m4k > 0) {
//		// first, generate NOT's for each eval[8]->pin[log_depth_a_m4k]
//		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_a-1:width_address_a-log_depth_a_m4k)
//		// else if ratio < 1 && log_ratio > 0, use pin(width_address_a-1:width_address_a-log_depth_b_m4k) and pin(log_depth_a_m4k-log_depth_b_m4k-1:0)
//		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_a_m4k);
//		sig_not->local_flag(true);
//		for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
//			// pin 0 - output
//			connect(gate->pin(0), sig_not->pin(idx));
//			// pin 1 - input
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				wren_offset = width_address_a - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0)
//				if (idx < log_depth_a_m4k - log_depth_b_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_a - log_depth_a_m4k;
//			connect(gate->pin(1), eval[8]->pin(idx+wren_offset));
//			des->add_node(gate);
//		}
//
//		// secondly, generate AND's using eval[8]->pin[log_depth_a_m4k] and sig_not
//		for (unsigned idx = 1; idx < log_depth_a_m4k; idx++) {
//			sig_and_old = sig_and;
//			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
//			sig_and->local_flag(true);
//			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_and->pin(jdx));
//				if (idx == 1) {
//					if (jdx % 2 == 0)
//						connect(gate->pin(1), sig_not->pin(0));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							if (0 < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						connect(gate->pin(1), eval[8]->pin(0+wren_offset));
//					}
//					if (jdx < 2)
//						connect(gate->pin(2), sig_not->pin(1));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							if (1 < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						connect(gate->pin(2), eval[8]->pin(1+wren_offset));
//					}
//				}
//				else {
//					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
//					if (jdx < pow(2,(double)idx))
//						connect(gate->pin(2), sig_not->pin(idx));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							wren_offset = width_address_a - log_depth_a_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							if (idx < log_depth_a_m4k - log_depth_b_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_a - log_depth_a_m4k;
//						connect(gate->pin(2), eval[8]->pin(idx+wren_offset));
//					}
//				}
//				des->add_node(gate);
//			}
//		}
//
//		// thirdly, generate AND's using sig_and and wr_en
//		sig_wr_en_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_a_m4k);
//		sig_wr_en_a->local_flag(true);
//		if (log_depth_a_m4k == 1) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_a->pin(0));
//			connect(gate->pin(1), sig_not->pin(0));
//			connect(gate->pin(2), eval[2]->pin(0));
//			des->add_node(gate);
//			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_a->pin(1));
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				wren_offset = width_address_a - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0)
//				if (0 < log_depth_a_m4k - log_depth_b_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_a - log_depth_a_m4k;
//			connect(gate->pin(1), eval[8]->pin(0+wren_offset));
//			connect(gate->pin(2), eval[2]->pin(0));
//			des->add_node(gate);
//		}
//		else {
//			for (unsigned idx = 0; idx < depth_a_m4k; idx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_wr_en_a->pin(idx));
//				connect(gate->pin(1), sig_and->pin(idx));
//				connect(gate->pin(2), eval[2]->pin(0));
//				des->add_node(gate);
//			}
//		}
//	}
//
//	// need WR_EN AND here!!!
//	// use eval[9]->pin[log_depth_b_m4k]
//	// if log_depth_b_m4k > 0
//	//NetNet* sig_not = 0;
//	//NetNet* sig_and = 0;
//	//NetNet* sig_and_old = 0;
//	//NetNet* sig_wr_en = eval[2];
//	NetNet* sig_wr_en_b = eval[5];
//	if (log_depth_b_m4k > 0) {
//		// first, generate NOT's for each eval[9]->pin[log_depth_b_m4k]
//		// if ratio >= 1 && log_ratio <= 0, use pin(width_address_b-1:width_address_b-log_depth_a_m4k) and pin(log_depth_b_m4k-log_depth_a_m4k-1:0)
//		// else if ratio < 1 && log_ratio > 0, use pin(width_address_b-1:width_address_b-log_depth_b_m4k)
//		sig_not = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, log_depth_b_m4k);
//		sig_not->local_flag(true);
//		for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 2, NetLogic::NOT);
//			// pin 0 - output
//			connect(gate->pin(0), sig_not->pin(idx));
//			// pin 1 - input
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				if (idx < log_depth_b_m4k - log_depth_a_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_b - log_depth_b_m4k;
//			else if (ratio < 1 && log_ratio > 0)
//				wren_offset = width_address_b - log_depth_b_m4k;
//			connect(gate->pin(1), eval[9]->pin(idx+wren_offset));
//			des->add_node(gate);
//		}
//
//		// secondly, generate AND's using eval[9]->pin[log_depth_b_m4k] and sig_not
//		for (unsigned idx = 1; idx < log_depth_b_m4k; idx++) {
//			sig_and_old = sig_and;
//			sig_and = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, pow(2,double(idx+1)));
//			sig_and->local_flag(true);
//			for (unsigned jdx = 0; jdx < pow(2,double(idx+1)); jdx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_and->pin(jdx));
//				if (idx == 1) {
//					if (jdx % 2 == 0)
//						connect(gate->pin(1), sig_not->pin(0));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (0 < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							wren_offset = width_address_b - log_depth_b_m4k;
//						connect(gate->pin(1), eval[9]->pin(0+wren_offset));
//					}
//					if (jdx < 2)
//						connect(gate->pin(2), sig_not->pin(1));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (1 < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							wren_offset = width_address_b - log_depth_b_m4k;
//						connect(gate->pin(2), eval[9]->pin(1+wren_offset));
//					}
//				}
//				else {
//					connect(gate->pin(1), sig_and_old->pin(jdx % (int)pow(2,(double)idx)));
//					if (jdx < pow(2,(double)idx))
//						connect(gate->pin(2), sig_not->pin(idx));
//					else {
//						unsigned wren_offset;
//						if (ratio >= 1 && log_ratio <= 0)
//							if (idx < log_depth_b_m4k - log_depth_a_m4k)
//								wren_offset = 0;
//							else
//								wren_offset = width_address_b - log_depth_b_m4k;
//						else if (ratio < 1 && log_ratio > 0)
//							wren_offset = width_address_b - log_depth_b_m4k;
//						connect(gate->pin(2), eval[9]->pin(idx+wren_offset));
//					}
//				}
//				des->add_node(gate);
//			}
//		}
//
//		// thirdly, generate AND's using sig_and and wr_en
//		sig_wr_en_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, depth_b_m4k);
//		sig_wr_en_b->local_flag(true);
//		if (log_depth_b_m4k == 1) {
//			NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_b->pin(0));
//			connect(gate->pin(1), sig_not->pin(0));
//			connect(gate->pin(2), eval[5]->pin(0));
//			des->add_node(gate);
//			gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//			connect(gate->pin(0), sig_wr_en_b->pin(1));
//			unsigned wren_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				if (0 < log_depth_b_m4k - log_depth_a_m4k)
//					wren_offset = 0;
//				else
//					wren_offset = width_address_b - log_depth_b_m4k;
//			else if (ratio < 1 && log_ratio > 0)
//				wren_offset = width_address_b - log_depth_b_m4k;
//			connect(gate->pin(1), eval[9]->pin(0+wren_offset));
//			connect(gate->pin(2), eval[5]->pin(0));
//			des->add_node(gate);
//		}
//		else {
//			for (unsigned idx = 0; idx < depth_b_m4k; idx++) {
//				NetLogic*gate = new NetLogic(scope, scope->local_symbol(), 3, NetLogic::AND);
//				connect(gate->pin(0), sig_wr_en_b->pin(idx));
//				connect(gate->pin(1), sig_and->pin(idx));
//				connect(gate->pin(2), eval[5]->pin(0));
//				des->add_node(gate);
//			}
//		}
//	}
//
//	// CCC
//	//NetNet*sig_mem = eval[13];
//	NetNet*sig_mem_a = eval[12];
//	NetNet*sig_mem_b = eval[13];
//	if (log_depth_a_m4k > 0) {
//		sig_mem_a = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_a*depth_a_m4k);
//		sig_mem_a->local_flag(true);
//	}
//	if (log_depth_b_m4k > 0) {
//		sig_mem_b = new NetNet(scope, scope->local_symbol(), NetNet::WIRE, width_data_b*depth_b_m4k);
//		sig_mem_b->local_flag(true);
//	}
//	for (unsigned depth_b_idx = 0; depth_b_idx < depth_b_m4k; depth_b_idx++) {
//		for (unsigned width_b_idx = 0; width_b_idx < width_b_m4k; width_b_idx++) {
//
//			unsigned data_a = data_a_m4k;
//			unsigned data_b = data_b_m4k;
//			if (width_b_idx == width_b_m4k - 1) {
//				data_a = data_a_last_m4k;
//				data_b = data_b_last_m4k;
//			}
//
//			NetMem*mem = new NetMem(scope, scope->local_symbol(),
//				address_a_m4k,
//				address_b_m4k,
//				data_a,
//				data_b,
//				data_a,
//				data_b);
//			des->add_node(mem);
//
//			//ADU:begin
//			verinum attri_mode("ture_dual_port");
//			mem->attribute("mode",attri_mode);
//
//			NetScope* up = scope->parent();
//			perm_string up_module_name = up->module_name();			
//			verinum attri_up_module_name(up_module_name.str());
//			mem->attribute("up_module_name",attri_up_module_name);
//
//			std::string up_scope_name = up->name();
//			verinum attri_up_scope_name(up_scope_name);
//			mem->attribute("up_scope_name",attri_up_scope_name);
//			//ADU:end
//
//			//ADU:
//			unsigned port_b_first_address = depth_b_idx*pow(2,double(address_b_m4k));
//			unsigned port_b_last_address = port_b_first_address + pow(2,double(address_b_m4k)) - 1;
//
//			verinum attri_port_b_first_address(port_b_first_address);
//			mem->attribute("port_b_first_address",attri_port_b_first_address);
//			verinum attri_port_b_last_address(port_b_last_address);
//			mem->attribute("port_b_last_address",attri_port_b_last_address);
//
//			unsigned port_a_first_address = (int(depth_b_idx/ratio)+int(width_b_idx/width_a_m4k))*pow(2,double(address_a_m4k));
//			unsigned port_a_last_address = port_a_first_address + pow(2,double(address_a_m4k)) - 1;
//
//			verinum attri_port_a_first_address(port_a_first_address);
//			mem->attribute("port_a_first_address",attri_port_a_first_address);
//			verinum attri_port_a_last_address(port_a_last_address);
//			mem->attribute("port_a_last_address",attri_port_a_last_address);
//
//
//			connect(mem->pin_clocken0(),eval[0]->pin(0));
//			connect(mem->pin_clocken1(),eval[1]->pin(0));
//			// use WR_EN AND gate output here!!!
//			if (ratio >= 1 && log_ratio <= 0)
//				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx/(width_a_m4k/width_b_m4k)));
//			else if (ratio < 1 && log_ratio > 0)
//				connect(mem->pin_wren_a(),sig_wr_en_a->pin(depth_b_idx*(depth_a_m4k/depth_b_m4k)+width_b_idx/width_a_m4k));
//			connect(mem->pin_aclr0(),eval[3]->pin(0));
//			connect(mem->pin_clock0(),eval[4]->pin(0));
//			// use WR_EN AND gate output here!!!
//			connect(mem->pin_wren_b(),sig_wr_en_b->pin(depth_b_idx));
//			connect(mem->pin_aclr1(),eval[6]->pin(0));
//			connect(mem->pin_clock1(),eval[7]->pin(0));
//			//CCC
//			// width_address_a is divided into 2 parts
//			// address_a_m4k, used for M4K's address
//			// log_depth_a_m4k, used for WR_EN AND
//			unsigned address_a_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				address_a_offset = 0;
//			else if (ratio < 1 && log_ratio > 0)
//				address_a_offset = log_depth_a_m4k - log_depth_b_m4k;
//			for (unsigned idx = 0; idx < address_a_m4k; idx++)
//				connect(mem->pin_address_a(idx),eval[8]->pin(idx+address_a_offset));
//
//			// width_address_b is divided into 2 parts
//			// address_b_m4k, used for M4K's address
//			// log_depth_b_m4k, used for output MUX
//			unsigned address_b_offset;
//			if (ratio >= 1 && log_ratio <= 0)
//				address_b_offset = log_depth_b_m4k - log_depth_a_m4k;
//			else if (ratio < 1 && log_ratio > 0)
//				address_b_offset = 0;
//			for (unsigned idx = 0; idx < address_b_m4k; idx++)
//				connect(mem->pin_address_b(idx),eval[9]->pin(idx+address_b_offset));
//
//			if (ratio >= 1 && log_ratio <= 0) {
//				unsigned data_a_offset = ( depth_b_idx % ( width_a_m4k / width_b_m4k ) ) * width_data_b + width_b_idx * data_b_m4k;
//				for (unsigned ratio_idx = 0; ratio_idx < ratio_m4k; ratio_idx++) {
//					for (unsigned idx = 0; idx < data_b; idx++) {
//						//ADU:
//						if(ratio_idx == 0 && idx == 0){
//							unsigned port_a_first_bit_number = idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k;
//							verinum attri_port_a_first_bit_number(port_a_first_bit_number);
//							mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
//						}
//
//						connect(mem->pin_data_a(idx+ratio_idx*data_b),eval[10]->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
//						if (log_depth_a_m4k == 0)
//							connect(mem->pin_q_a(idx+ratio_idx*data_b),eval[12]->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k));
//						else
//							connect(mem->pin_q_a(idx+ratio_idx*data_b),sig_mem_a->pin(idx+data_a_offset+ratio_idx*width_data_b*width_a_m4k/width_b_m4k+depth_b_idx/(width_a_m4k/width_b_m4k)*width_data_a));
//					}
//				}
//			}
//			else if (ratio < 1 && log_ratio > 0) {
//				unsigned data_a_offset = width_b_idx % width_a_m4k * data_a_m4k;
//				for (unsigned idx = 0; idx < data_a; idx++) {
//					//ADU:
//					if(idx == 0){
//						unsigned port_a_first_bit_number = idx+data_a_offset;
//						verinum attri_port_a_first_bit_number(port_a_first_bit_number);
//						mem->attribute("port_a_first_bit_number",attri_port_a_first_bit_number);
//					}
//
//					connect(mem->pin_data_a(idx),eval[10]->pin(idx+data_a_offset));
//					if (log_depth_a_m4k == 0)
//						connect(mem->pin_q_a(idx),eval[12]->pin(idx+data_a_offset));
//					else
//						connect(mem->pin_q_a(idx),sig_mem_a->pin(idx+data_a_offset+depth_b_idx*depth_a_m4k/depth_b_m4k*width_data_a));
//				}
//			}

//
//			if (ratio >= 1 && log_ratio <= 0) {
//				unsigned data_b_offset = width_b_idx * data_b_m4k;
//				for (unsigned idx = 0; idx < data_b; idx++) {
//					//ADU:
//					if(idx == 0){
//						unsigned port_b_first_bit_number = idx+data_b_offset;
//						verinum attri_port_b_first_bit_number(port_b_first_bit_number);
//						mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
//					}
//
//					connect(mem->pin_data_b(idx),eval[11]->pin(idx+data_b_offset));
//					if (log_depth_b_m4k == 0)
//						connect(mem->pin_q_b(idx),eval[13]->pin(idx+data_b_offset));
//					else
//						connect(mem->pin_q_b(idx),sig_mem_b->pin(idx+depth_b_idx*width_data_b+data_b_offset));
//				}
//			}
//			else if (ratio < 1 && log_ratio > 0) {
//				unsigned data_b_offset = width_b_idx * data_a_m4k;
//				for (unsigned ratio_idx = 0; ratio_idx < 1 / ratio_m4k; ratio_idx++) {
//					for (unsigned idx = 0; idx < data_a; idx++) {
//						//ADU:
//						if(ratio_idx == 0 && idx == 0){
//							unsigned port_b_first_bit_number = idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k;
//							verinum attri_port_b_first_bit_number(port_b_first_bit_number);
//							mem->attribute("port_b_first_bit_number",attri_port_b_first_bit_number);
//						}
//
//						connect(mem->pin_data_b(idx+ratio_idx*data_a),eval[11]->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
//						if (log_depth_b_m4k == 0)
//							connect(mem->pin_q_b(idx+ratio_idx*data_a),eval[13]->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k));
//						else
//							connect(mem->pin_q_b(idx+ratio_idx*data_a),sig_mem_b->pin(idx+data_b_offset+ratio_idx*width_data_a*depth_a_m4k/depth_b_m4k+depth_b_idx*width_data_b));
//					}
//				}
//			}
//
//		}
//
//	}
//
//	if ( ratio >= 1 && log_ratio <= 0 ) {
//
//		// need output MUX here!!!
//		// use eval[8]->pin[log_depth_a_m4k]
//		// pin(width_address_a-1:width_address_a-log_depth_a_m4k)
//		// if log_depth_a_m4k > 0
//		if (log_depth_a_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_a; idx++)
//				connect(mux->pin_Result(idx), eval[12]->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++)
//				connect(mux->pin_Sel(idx), eval[8]->pin(idx+width_address_a-log_depth_a_m4k));
//			des->add_node(mux);
//		}
//
//		// need output MUX here!!!
//		// use eval[9]->pin[log_depth_b_m4k]
//		// pin(log_depth_b_m4k-log_depth_a_m4k-1:0) and pin(width_address_b-1:width_address_b-log_depth_a_m4k)
//		// if log_depth_b_m4k > 0
//		if (log_depth_b_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_b; idx++)
//				connect(mux->pin_Result(idx), eval[13]->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++) {
//				if (idx < log_depth_b_m4k - log_depth_a_m4k)
//					connect(mux->pin_Sel(idx), eval[9]->pin(idx));
//				else
//					connect(mux->pin_Sel(idx), eval[9]->pin(idx+width_address_b-log_depth_b_m4k));
//			}
//			des->add_node(mux);
//		}
//
//	}
//
//	else if (ratio < 1 && log_ratio > 0) {
//
//		// need output MUX here!!!
//		// use eval[8]->pin[log_depth_a_m4k]
//		// pin(log_depth_a_m4k-log_depth_b_m4k-1:0) and pin(width_address_a-1:width_address_a-log_depth_b_m4k)
//		// if log_depth_a_m4k > 0
//		if (log_depth_a_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_a, depth_a_m4k, log_depth_a_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_a_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_a; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_a->pin(depth_idx*width_data_a+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_a; idx++)
//				connect(mux->pin_Result(idx), eval[12]->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_a_m4k; idx++) {
//				if (idx < log_depth_a_m4k - log_depth_b_m4k)
//					connect(mux->pin_Sel(idx), eval[8]->pin(idx));
//				else
//					connect(mux->pin_Sel(idx), eval[8]->pin(idx+width_address_a-log_depth_a_m4k));
//			}
//			des->add_node(mux);
//		}
//
//		// need output MUX here!!!
//		// use eval[9]->pin[log_depth_b_m4k]
//		// pin(width_address_b-1:width_address_b-log_depth_b_m4k)
//		// if log_depth_b_m4k > 0
//		if (log_depth_b_m4k > 0) {
//			// NetMux(xx, xx, data_width, mux_input_num, select_width);
//			NetMux*mux = new NetMux(scope, scope->local_symbol(), width_data_b, depth_b_m4k, log_depth_b_m4k);
//			// Connect the data inputs
//			for (unsigned depth_idx = 0; depth_idx < depth_b_m4k; depth_idx++)
//				for (unsigned width_idx = 0; width_idx < width_data_b; width_idx++)
//					connect(mux->pin_Data(width_idx,depth_idx), sig_mem_b->pin(depth_idx*width_data_b+width_idx));
//			// Connect the output
//			for (unsigned idx = 0; idx < width_data_b; idx++)
//				connect(mux->pin_Result(idx), eval[13]->pin(idx));
//			// Connect the select input
//			for (unsigned idx = 0; idx < log_depth_b_m4k; idx++)
//				connect(mux->pin_Sel(idx), eval[9]->pin(idx+width_address_b-log_depth_b_m4k));
//			des->add_node(mux);
//		}
//
//	}
//}


void PSquareAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*eval[5];

	for (unsigned idx = 0; idx < 5; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}

		assert(eval[idx]);
	}

	NetSquare*square = new NetSquare(scope, scope->local_symbol(),
		eval[0]->pin_count(),
		eval[4]->pin_count());
	des->add_node(square);

	for (unsigned idx = 0 ;  idx < eval[0]->pin_count() ; idx += 1)
		connect(square->pin_data(idx), eval[0]->pin(idx));
	connect(square->pin_clken(), eval[1]->pin(0));
	connect(square->pin_aclr(), eval[2]->pin(0));
	connect(square->pin_clock(), eval[3]->pin(0));
	for (unsigned idx = 0 ;  idx < eval[4]->pin_count() ; idx += 1)
		connect(square->pin_result(idx), eval[4]->pin(idx));
}

void PMultAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	bool fast_mult_mapping = false;
	// if to favourite Mult, not supported yet!
	if (fast_mult_mapping){
		cerr << "Fast_Mult_mapping not supported yet!" << endl;
		assert(0);
	}

	NetNet*eval[7];
	//dataa| datab | clken | aclr | clock | sum | result | 
	//    0   |     1   |    2     |   3   |    4     |    5  |   6      |   

	for (unsigned idx = 0; idx < 7; idx++) {
		assert(pin(idx));
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}
		assert(eval[idx]);
	}
Flag_mult->flag_mult_signed_dataa0  =
				static_cast<unsigned>(eval[0]->get_signed()) ;
Flag_mult->flag_mult_signed_datab0 = 
				static_cast<unsigned>(eval[1]->get_signed()) ;
Flag_mult->flag_mult_signed_result   = 
				static_cast<unsigned>(eval[6]->get_signed()) ;
/**********************************************************
--DSP端口分组
--36位以内
eval[7]：
//dataa| datab | clken | aclr | clock | sum | result | 
//    0   |     1   |    2     |   3   |    4     |    5  |   6      |  
**********************************************************/
	unsigned MaxInputData = eval[0]->pin_count() > eval[1]->pin_count()?eval[0]->pin_count():eval[0]->pin_count();
	assert(MaxInputData);

	if(MaxInputData <=18)
	{
		/********************************/
		NetLMult*mult = new NetLMult(scope, scope->local_symbol(),
			eval[0]->pin_count(),	
			eval[1]->pin_count(),
			eval[5]->pin_count(),
			eval[6]->pin_count());
		des->add_node(mult);
		NetLMultSimpleOUT*multout = new NetLMultSimpleOUT(scope, scope->local_symbol(),	eval[6]->pin_count(),eval[6]->pin_count());
		des->add_node(multout);
		NetNet *sig_rs = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[6]->pin_count());
//test begin
//
//NetLMultSimpleOUT*multout1 = new NetLMultSimpleOUT(scope, scope->local_symbol(),	0,15);
//des->add_node(multout1);
//NetNet *sig_rs1 = 
//		new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 15);
//for (unsigned idx = 0 ;  idx < 15 ; idx += 1) {
//	connect(sig_rs1->pin(idx), multout1->pin_result(idx));
//}	
//NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
//	 8,  8,  8,  8, 
//	 2, 0, 18);
//des->add_node(multaddout);
//NetNet *sig_rs1 = 
//		new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 32);
//for (unsigned idx = 0 ;  idx < 8 ; idx += 1)
//	connect(multaddout->pin_dataa(idx), sig_rs1->pin(idx));
//for (unsigned idx = 0 ;  idx < 8 ; idx += 1)
//	connect(multaddout->pin_datab(idx), sig_rs1->pin(idx+8));
//for (unsigned idx = 0 ;  idx < 8 ; idx += 1)
//	connect(multaddout->pin_datac(idx), sig_rs1->pin(idx+16));
//for (unsigned idx = 0 ;  idx < 8 ; idx += 1)
//	connect(multaddout->pin_datad(idx), sig_rs1->pin(idx+24));
//NetNet *sig_rs2 = 
//	new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 16);
//for (unsigned idx = 0 ;  idx < 2 ; idx += 1)
//	connect(multaddout->pin_addnsub(idx), sig_rs2->pin(idx));
////for (unsigned idx = 0 ;  idx < 1 ; idx += 1)
////	connect(multaddout->pin_accumsload(idx), sig_rs2->pin(idx+2));
//
//connect(multaddout->pin_clock0(), sig_rs2->pin(2));
//connect(multaddout->pin_clock1(),sig_rs2->pin(3) );
//connect(multaddout->pin_clock2(), sig_rs2->pin(4));
//connect(multaddout->pin_clock3(), sig_rs2->pin(5));
//connect(multaddout->pin_aclr0(), sig_rs2->pin(6));
//connect(multaddout->pin_aclr1(),sig_rs2->pin(7) );
//connect(multaddout->pin_aclr2(), sig_rs2->pin(8));
//connect(multaddout->pin_aclr3(), sig_rs2->pin(9));
//connect(multaddout->pin_ena0(), sig_rs2->pin(10));
//connect(multaddout->pin_ena1(), sig_rs2->pin(11));
//connect(multaddout->pin_ena2(), sig_rs2->pin(12));
//connect(multaddout->pin_ena3(), sig_rs2->pin(13));
//connect(multaddout->pin_signa(), sig_rs2->pin(14));
//connect(multaddout->pin_signb(), sig_rs2->pin(15));
//NetNet *sig_rs3 = 
//	new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 18);
//for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
//	connect(multaddout->pin_result(idx), sig_rs3->pin(idx));

//test end

		for (unsigned idx = 0 ;  idx < eval[0]->pin_count() ; idx += 1)
			connect(mult->pin_dataa(idx), eval[0]->pin(idx));	
		for (unsigned idx = 0 ;  idx < eval[1]->pin_count() ; idx += 1)
			connect(mult->pin_datab(idx), eval[1]->pin(idx));
		connect(mult->pin_clken(), eval[2]->pin(0));
		connect(mult->pin_aclr(), eval[3]->pin(0));//test
		connect(mult->pin_clock(), eval[4]->pin(0));
		for (unsigned idx = 0 ;  idx < eval[5]->pin_count() ; idx += 1)
			connect(mult->pin_sum(idx), eval[5]->pin(idx));
		for (unsigned idx = 0 ;  idx < eval[6]->pin_count() ; idx += 1) {
			connect(mult->pin_result(idx), sig_rs->pin(idx));
			connect(sig_rs->pin(idx), multout->pin_dataa(idx));
		}		
		for (unsigned idx = 0 ;  idx < eval[6]->pin_count() ; idx += 1)
			connect(multout->pin_result(idx),eval[6]->pin(idx));
	}
	else
		if(MaxInputData>18&&MaxInputData<=36)
	{
		//bool flag = false;
		//Link::strength_t drive0 = drive_type(strength0());
		//Link::strength_t drive1 = drive_type(strength1());

		NetLMult*mult0 = new NetLMult(scope, scope->local_symbol(),18,18,eval[5]->pin_count(),36);
		NetLMult*mult1 = new NetLMult(scope, scope->local_symbol(),18,18,eval[5]->pin_count(),36);
		NetLMult*mult2 = new NetLMult(scope, scope->local_symbol(),18,18,eval[5]->pin_count(),36);
		NetLMult*mult3 = new NetLMult(scope, scope->local_symbol(),18,18,eval[5]->pin_count(),36);
		des->add_node(mult0);
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
			connect(mult0->pin_dataa(idx), eval[0]->pin(idx));	
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
			connect(mult0->pin_datab(idx), eval[1]->pin(idx));
		connect(mult0->pin_clken(), eval[2]->pin(0));
		connect(mult0->pin_aclr(), eval[3]->pin(0));
		connect(mult0->pin_clock(), eval[4]->pin(0));
		for (unsigned idx = 0 ;  idx < 	eval[5]->pin_count() ; idx += 1)
			connect(mult0->pin_sum(idx), eval[5]->pin(idx));
		for (unsigned idx = 0 ;  idx < 36 ; idx += 1)
			connect(mult0->pin_result(idx), eval[6]->pin(idx));

		des->add_node(mult1);
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
			connect(mult1->pin_dataa(idx), eval[0]->pin(idx));	
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
			connect(mult1->pin_datab(idx), eval[1]->pin(idx+18));
		connect(mult1->pin_clken(), eval[2]->pin(0));
		connect(mult1->pin_aclr(), eval[3]->pin(0));
		connect(mult1->pin_clock(), eval[4]->pin(0));
		for (unsigned idx = 0 ;  idx < 	eval[5]->pin_count() ; idx += 1)
			connect(mult1->pin_sum(idx), eval[5]->pin(idx));
		for (unsigned idx = 0 ;  idx < 36 ; idx += 1)
			connect(mult1->pin_result(idx), eval[6]->pin(idx+36));

		//NetConst*zero[18];
		//for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
		//	zero[idx]= new NetConst(scope, scope->local_symbol(),
		//		verinum::V0);
		NetNet *sig_rs1[18];
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1){
			sig_rs1[idx] = 
				new NetNet(scope, scope->local_symbol(), NetNet::/*SUPPLY0*/WIRE, 1);
			sig_rs1[idx]->port_type(NetNet::PINOUT);
			sig_rs1[idx]->local_flag(true);
		}		
		des->add_node(mult2);
		for (unsigned idx = 0 ;  idx < 18 ; idx += 1){
						connect(mult2->pin_dataa(idx), sig_rs1[idx]->pin(0)/*eval[0]->pin(idx+18)*/);	
						//connect(sig_rs1->pin(idx),zero[idx]->pin(0));	
		}

		for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
			connect(mult2->pin_datab(idx), eval[1]->pin(idx));
		connect(mult2->pin_clken(), eval[2]->pin(0));
		connect(mult2->pin_aclr(), eval[3]->pin(0));
		connect(mult2->pin_clock(), eval[4]->pin(0));
		for (unsigned idx = 0 ;  idx < 	eval[5]->pin_count() ; idx += 1)
			connect(mult2->pin_sum(idx), eval[5]->pin(idx));

		NetNet *sig_rs0 = 
			new NetNet(scope, scope->local_symbol(), NetNet::/*SUPPLY0*/WIRE, 36);
		sig_rs0->port_type(NetNet::POUTPUT);
		//NetConst*zero[36];
		//for (unsigned idx = 0 ;  idx < 36 ; idx += 1){
		//	zero[idx]= new NetConst(scope, scope->local_symbol(),
		//		verinum::V0);
			//zero[idx]->set_line(*this);
			//des->add_node(zero[idx]);
			//zero[idx]->pin(0).drive0(drive0);
			//zero[idx]->pin(0).drive1(drive1);
		//}
		for (unsigned idx = 0 ;  idx < 36 ; idx += 1){
			//mult2->pin_result(idx).nexus()->driven_ = Nexus::V0;
	/*		mult2->value(idx) = verinum::V0;*/
			connect(mult2->pin_result(idx), sig_rs0->pin(idx));
			//connect(sig_rs0->pin(idx), sig_rs1->pin(idx));
			//connect(sig_rs0->pin(idx), zero[idx]->pin(0));
			//zero[idx]->pin(0).nexus()->driven_value() = verinum::V0;
		}


		//des->add_node(mult3);
		//for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
		//	connect(mult3->pin_dataa(idx), eval[0]->pin(idx+18));	
		//for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
		//	connect(mult3->pin_datab(idx), eval[1]->pin(idx+18));
		//connect(mult3->pin_clken(), eval[2]->pin(0));
		//connect(mult3->pin_aclr(), eval[3]->pin(0));
		//connect(mult3->pin_clock(), eval[4]->pin(0));
		//for (unsigned idx = 0 ;  idx < 	eval[5]->pin_count() ; idx += 1)
		//	connect(mult3->pin_sum(idx), eval[5]->pin(idx));
		//for (unsigned idx = 0 ;  idx < 18 ; idx += 1)
		//	connect(mult3->pin_result(idx), eval[6]->pin(idx+54));


		bool flag = true;
		if (!flag)
		{
			assert(0);
		}
	}
		else
			assert(0);
			//36+ TODO
}



void PMultAddAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*eval[19];
	//dataa| datab | clock0 | aclr | clock | sum | result | 
	//    0   |     1   |        2        |   3   |    4     |    5  |   6      |   

	for (unsigned idx = 0; idx < 19; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}

		assert(eval[idx]);
	}

/*************************************
mult_add拆分
dataa| datab | clock0 | aclr | clock | sum | result | 
   0   |     1   |        2        |   3   |    4     |    5  |   6      |  
dataa,datab,clock0,clock1,clock2,clock3,aclr0,aclr1,aclr2,aclr3,ena0,ena1,ena2,ena3,signa,signb,scanouta,scanoutb,result);
*************************************/
assert(eval[0]->pin_count()/eval[16]->pin_count() 
	== eval[1]->pin_count()/eval[17]->pin_count());
unsigned NumOfMult =eval[0]->pin_count()/eval[16]->pin_count();
unsigned MaxInputData = eval[16]->pin_count() > eval[17]->pin_count()?eval[16]->pin_count():eval[17]->pin_count();
assert(MaxInputData);

if (MaxInputData <=18){
	if (NumOfMult == 1){
		NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count(),
			eval[1]->pin_count(),
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count());
		des->add_node(mult_add0);
		NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
			eval[18]->pin_count(),  0,  0,  0, 
			1, 0, eval[18]->pin_count());
		des->add_node(multaddout);

		for (unsigned idx = 0 ;  idx < eval[0]->pin_count() ; idx += 1)
			connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
		for (unsigned idx = 0 ;  idx < eval[1]->pin_count() ; idx += 1)
			connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));

		connect(mult_add0->pin_clock0(), eval[2]->pin(0));
		connect(mult_add0->pin_clock1(), eval[3]->pin(0));
		connect(mult_add0->pin_clock2(), eval[4]->pin(0));
		connect(mult_add0->pin_clock3(), eval[5]->pin(0));
		connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add0->pin_ena0(), eval[10]->pin(0));
		connect(mult_add0->pin_ena1(), eval[11]->pin(0));
		connect(mult_add0->pin_ena2(), eval[12]->pin(0));
		connect(mult_add0->pin_ena3(), eval[13]->pin(0));
		connect(mult_add0->pin_signa(), eval[14]->pin(0));
		connect(mult_add0->pin_signb(), eval[15]->pin(0));

		connect(multaddout->pin_clock0(), eval[2]->pin(0));
		connect(multaddout->pin_clock1(), eval[3]->pin(0));
		connect(multaddout->pin_clock2(), eval[4]->pin(0));
		connect(multaddout->pin_clock3(), eval[5]->pin(0));
		connect(multaddout->pin_aclr0(), eval[6]->pin(0));
		connect(multaddout->pin_aclr1(), eval[7]->pin(0));
		connect(multaddout->pin_aclr2(), eval[8]->pin(0));
		connect(multaddout->pin_aclr3(), eval[9]->pin(0));
		connect(multaddout->pin_ena0(), eval[10]->pin(0));
		connect(multaddout->pin_ena1(), eval[11]->pin(0));
		connect(multaddout->pin_ena2(), eval[12]->pin(0));
		connect(multaddout->pin_ena3(), eval[13]->pin(0));
		connect(multaddout->pin_signa(), eval[14]->pin(0));
		connect(multaddout->pin_signb(), eval[15]->pin(0));

		NetNet *sig_rs1 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1);
		sig_rs1->port_type(NetNet::PINPUT);
		connect(multaddout->pin_addnsub(0), sig_rs1->pin(0));

		for (unsigned idx = 0 ;  idx < eval[16]->pin_count() ; idx += 1)
			connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
		for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1)
			connect(mult_add0->pin_scanoutb(idx), eval[17]->pin(idx));

		NetNet *sig_rs = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count());
		sig_rs->port_type(NetNet::PINPUT);
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count() ; idx += 1){
			connect(mult_add0->pin_result(idx), sig_rs->pin(idx));
			connect(sig_rs->pin(idx), multaddout->pin_dataa(idx));	
		}
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count() ; idx += 1){
			connect(multaddout->pin_result(idx),eval[18]->pin(idx));	
		}
	}		
	else if (NumOfMult == 2){
		NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/2,
			eval[1]->pin_count()/2,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-1);
		des->add_node(mult_add0);

		NetMultAdd*mult_add1 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/2,
			eval[1]->pin_count()/2,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-1);
		des->add_node(mult_add1);

		NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
			eval[18]->pin_count()-1, eval[18]->pin_count()-1,  0,  0, 
			1, 0, eval[18]->pin_count());
		des->add_node(multaddout);

		for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/2; idx += 1){
			connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
			connect(mult_add1->pin_dataa(idx), eval[0]->pin(idx+eval[0]->pin_count()/2));
		}
		for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/2 ; idx += 1){
			connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));
			connect(mult_add1->pin_datab(idx), eval[1]->pin(idx+eval[1]->pin_count()/2));
		}
		connect(mult_add0->pin_clock0(), eval[2]->pin(0));
		connect(mult_add0->pin_clock1(), eval[3]->pin(0));
		connect(mult_add0->pin_clock2(), eval[4]->pin(0));
		connect(mult_add0->pin_clock3(), eval[5]->pin(0));
		connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add0->pin_ena0(), eval[10]->pin(0));
		connect(mult_add0->pin_ena1(), eval[11]->pin(0));
		connect(mult_add0->pin_ena2(), eval[12]->pin(0));
		connect(mult_add0->pin_ena3(), eval[13]->pin(0));
		connect(mult_add0->pin_signa(), eval[14]->pin(0));
		connect(mult_add0->pin_signb(), eval[15]->pin(0));

		connect(mult_add1->pin_clock0(), eval[2]->pin(0));
		connect(mult_add1->pin_clock1(), eval[3]->pin(0));
		connect(mult_add1->pin_clock2(), eval[4]->pin(0));
		connect(mult_add1->pin_clock3(), eval[5]->pin(0));
		connect(mult_add1->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add1->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add1->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add1->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add1->pin_ena0(), eval[10]->pin(0));
		connect(mult_add1->pin_ena1(), eval[11]->pin(0));
		connect(mult_add1->pin_ena2(), eval[12]->pin(0));
		connect(mult_add1->pin_ena3(), eval[13]->pin(0));
		connect(mult_add1->pin_signa(), eval[14]->pin(0));
		connect(mult_add1->pin_signb(), eval[15]->pin(0));

		NetNet *sig_rs4 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs5 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		for (unsigned idx = 0 ;  idx < eval[16]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
			connect(mult_add1->pin_scanouta(idx), sig_rs4->pin(idx));//eval[16]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanoutb(idx), eval[17]->pin(idx));
			connect(mult_add1->pin_scanoutb(idx), sig_rs5->pin(idx));//eval[17]->pin(idx)
		}

		connect(multaddout->pin_clock0(), eval[2]->pin(0));
		connect(multaddout->pin_clock1(), eval[3]->pin(0));
		connect(multaddout->pin_clock2(), eval[4]->pin(0));
		connect(multaddout->pin_clock3(), eval[5]->pin(0));
		connect(multaddout->pin_aclr0(), eval[6]->pin(0));
		connect(multaddout->pin_aclr1(), eval[7]->pin(0));
		connect(multaddout->pin_aclr2(), eval[8]->pin(0));
		connect(multaddout->pin_aclr3(), eval[9]->pin(0));
		connect(multaddout->pin_ena0(), eval[10]->pin(0));
		connect(multaddout->pin_ena1(), eval[11]->pin(0));
		connect(multaddout->pin_ena2(), eval[12]->pin(0));
		connect(multaddout->pin_ena3(), eval[13]->pin(0));
		connect(multaddout->pin_signa(), eval[14]->pin(0));
		connect(multaddout->pin_signb(), eval[15]->pin(0));
		NetNet *sig_rs2 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 2);
		connect(multaddout->pin_addnsub(0), sig_rs2->pin(0));
		//connect(multaddout->pin_addnsub(1), sig_rs2->pin(1));

		NetNet *sig_rs0 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-1);
		NetNet *sig_rs1 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-1);
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count()-1 ; idx += 1){
			connect(mult_add0->pin_result(idx),sig_rs0->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add1->pin_result(idx),sig_rs1->pin(idx) );//eval[18]->pin(idx)
			connect(sig_rs0->pin(idx),multaddout->pin_dataa(idx));//eval[18]->pin(idx)
			connect(sig_rs1->pin(idx),multaddout->pin_datab(idx) );//eval[18]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count() ; idx += 1){
			connect(multaddout->pin_result(idx),eval[18]->pin(idx));	
		}
	}
	else if (NumOfMult == 3){
		NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/3,
			eval[1]->pin_count()/3,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		des->add_node(mult_add0);
		NetMultAdd*mult_add1 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/3,
			eval[1]->pin_count()/3,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		des->add_node(mult_add1);
		NetMultAdd*mult_add2 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/3,
			eval[1]->pin_count()/3,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		des->add_node(mult_add2);
		NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
			eval[18]->pin_count()-2, eval[18]->pin_count()-2,  eval[18]->pin_count()-2,  0, 
			2, 0, eval[18]->pin_count());
		des->add_node(multaddout);

		for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/3; idx += 1){
			connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
			connect(mult_add1->pin_dataa(idx), eval[0]->pin(idx+eval[0]->pin_count()/3));
			connect(mult_add2->pin_dataa(idx), eval[0]->pin(idx+2*eval[0]->pin_count()/3));
		}
		for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/3 ; idx += 1){
			connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));
			connect(mult_add1->pin_datab(idx), eval[1]->pin(idx+eval[1]->pin_count()/3));
			connect(mult_add2->pin_datab(idx), eval[1]->pin(idx+2*eval[1]->pin_count()/3));
		}
		connect(mult_add0->pin_clock0(), eval[2]->pin(0));
		connect(mult_add0->pin_clock1(), eval[3]->pin(0));
		connect(mult_add0->pin_clock2(), eval[4]->pin(0));
		connect(mult_add0->pin_clock3(), eval[5]->pin(0));
		connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add0->pin_ena0(), eval[10]->pin(0));
		connect(mult_add0->pin_ena1(), eval[11]->pin(0));
		connect(mult_add0->pin_ena2(), eval[12]->pin(0));
		connect(mult_add0->pin_ena3(), eval[13]->pin(0));
		connect(mult_add0->pin_signa(), eval[14]->pin(0));
		connect(mult_add0->pin_signb(), eval[15]->pin(0));

		connect(mult_add1->pin_clock0(), eval[2]->pin(0));
		connect(mult_add1->pin_clock1(), eval[3]->pin(0));
		connect(mult_add1->pin_clock2(), eval[4]->pin(0));
		connect(mult_add1->pin_clock3(), eval[5]->pin(0));
		connect(mult_add1->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add1->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add1->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add1->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add1->pin_ena0(), eval[10]->pin(0));
		connect(mult_add1->pin_ena1(), eval[11]->pin(0));
		connect(mult_add1->pin_ena2(), eval[12]->pin(0));
		connect(mult_add1->pin_ena3(), eval[13]->pin(0));
		connect(mult_add1->pin_signa(), eval[14]->pin(0));
		connect(mult_add1->pin_signb(), eval[15]->pin(0));

		connect(mult_add2->pin_clock0(), eval[2]->pin(0));
		connect(mult_add2->pin_clock1(), eval[3]->pin(0));
		connect(mult_add2->pin_clock2(), eval[4]->pin(0));
		connect(mult_add2->pin_clock3(), eval[5]->pin(0));
		connect(mult_add2->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add2->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add2->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add2->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add2->pin_ena0(), eval[10]->pin(0));
		connect(mult_add2->pin_ena1(), eval[11]->pin(0));
		connect(mult_add2->pin_ena2(), eval[12]->pin(0));
		connect(mult_add2->pin_ena3(), eval[13]->pin(0));
		connect(mult_add2->pin_signa(), eval[14]->pin(0));
		connect(mult_add2->pin_signb(), eval[15]->pin(0));

		NetNet *sig_rs4 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs5 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs6 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		NetNet *sig_rs7 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		for (unsigned idx = 0 ;  idx < eval[16]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
			connect(mult_add1->pin_scanouta(idx), sig_rs4->pin(idx));//eval[16]->pin(idx)
			connect(mult_add2->pin_scanouta(idx), sig_rs5->pin(idx));//eval[16]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanoutb(idx), eval[17]->pin(idx));
			connect(mult_add1->pin_scanoutb(idx), sig_rs6->pin(idx));//eval[17]->pin(idx)
			connect(mult_add2->pin_scanoutb(idx), sig_rs7->pin(idx));//eval[17]->pin(idx)
		}

		connect(multaddout->pin_clock0(), eval[2]->pin(0));
		connect(multaddout->pin_clock1(), eval[3]->pin(0));
		connect(multaddout->pin_clock2(), eval[4]->pin(0));
		connect(multaddout->pin_clock3(), eval[5]->pin(0));
		connect(multaddout->pin_aclr0(), eval[6]->pin(0));
		connect(multaddout->pin_aclr1(), eval[7]->pin(0));
		connect(multaddout->pin_aclr2(), eval[8]->pin(0));
		connect(multaddout->pin_aclr3(), eval[9]->pin(0));
		connect(multaddout->pin_ena0(), eval[10]->pin(0));
		connect(multaddout->pin_ena1(), eval[11]->pin(0));
		connect(multaddout->pin_ena2(), eval[12]->pin(0));
		connect(multaddout->pin_ena3(), eval[13]->pin(0));
		connect(multaddout->pin_signa(), eval[14]->pin(0));
		connect(multaddout->pin_signb(), eval[15]->pin(0));
		NetNet *sig_rs2 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 2);
		connect(multaddout->pin_addnsub(0), sig_rs2->pin(0));
		connect(multaddout->pin_addnsub(1), sig_rs2->pin(1));

		NetNet *sig_rs0 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		NetNet *sig_rs1 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		NetNet *sig_rs3 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count()-2 ; idx += 1){
			connect(mult_add0->pin_result(idx),sig_rs0->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add1->pin_result(idx),sig_rs1->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add2->pin_result(idx),sig_rs3->pin(idx) );//eval[18]->pin(idx)
			connect(sig_rs0->pin(idx),multaddout->pin_dataa(idx));//eval[18]->pin(idx)
			connect(sig_rs1->pin(idx),multaddout->pin_datab(idx) );//eval[18]->pin(idx)
			connect(sig_rs3->pin(idx),multaddout->pin_datac(idx) );//eval[18]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count() ; idx += 1){
			connect(multaddout->pin_result(idx),eval[18]->pin(idx));	
		}


	}
	else if (NumOfMult == 4){
		NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/4,
			eval[1]->pin_count()/4,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		NetMultAdd*mult_add1 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/4,
			eval[1]->pin_count()/4,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		NetMultAdd*mult_add2 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/4,
			eval[1]->pin_count()/4,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		NetMultAdd*mult_add3 = new NetMultAdd(scope, scope->local_symbol(),
			eval[0]->pin_count()/4,
			eval[1]->pin_count()/4,
			eval[16]->pin_count(),
			eval[17]->pin_count(),
			eval[18]->pin_count()-2);
		des->add_node(mult_add0);
		des->add_node(mult_add1);
		des->add_node(mult_add2);
		des->add_node(mult_add3);

		NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
			eval[18]->pin_count()-2, eval[18]->pin_count()-2,  eval[18]->pin_count()-2,  eval[18]->pin_count()-2, 
			2, 0, eval[18]->pin_count());
		des->add_node(multaddout);

		for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/4; idx += 1){
			connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
			connect(mult_add1->pin_dataa(idx), eval[0]->pin(idx+eval[0]->pin_count()/4));
			connect(mult_add2->pin_dataa(idx), eval[0]->pin(idx+2*eval[0]->pin_count()/4));
			connect(mult_add3->pin_dataa(idx), eval[0]->pin(idx+3*eval[0]->pin_count()/4));
		}
		for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/4 ; idx += 1){
			connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));
			connect(mult_add1->pin_datab(idx), eval[1]->pin(idx+eval[1]->pin_count()/4));
			connect(mult_add2->pin_datab(idx), eval[1]->pin(idx+2*eval[1]->pin_count()/4));
			connect(mult_add3->pin_datab(idx), eval[1]->pin(idx+3*eval[1]->pin_count()/4));
		}
		connect(mult_add0->pin_clock0(), eval[2]->pin(0));
		connect(mult_add0->pin_clock1(), eval[3]->pin(0));
		connect(mult_add0->pin_clock2(), eval[4]->pin(0));
		connect(mult_add0->pin_clock3(), eval[5]->pin(0));
		connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add0->pin_ena0(), eval[10]->pin(0));
		connect(mult_add0->pin_ena1(), eval[11]->pin(0));
		connect(mult_add0->pin_ena2(), eval[12]->pin(0));
		connect(mult_add0->pin_ena3(), eval[13]->pin(0));
		connect(mult_add0->pin_signa(), eval[14]->pin(0));
		connect(mult_add0->pin_signb(), eval[15]->pin(0));

		connect(mult_add1->pin_clock0(), eval[2]->pin(0));
		connect(mult_add1->pin_clock1(), eval[3]->pin(0));
		connect(mult_add1->pin_clock2(), eval[4]->pin(0));
		connect(mult_add1->pin_clock3(), eval[5]->pin(0));
		connect(mult_add1->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add1->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add1->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add1->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add1->pin_ena0(), eval[10]->pin(0));
		connect(mult_add1->pin_ena1(), eval[11]->pin(0));
		connect(mult_add1->pin_ena2(), eval[12]->pin(0));
		connect(mult_add1->pin_ena3(), eval[13]->pin(0));
		connect(mult_add1->pin_signa(), eval[14]->pin(0));
		connect(mult_add1->pin_signb(), eval[15]->pin(0));

		connect(mult_add2->pin_clock0(), eval[2]->pin(0));
		connect(mult_add2->pin_clock1(), eval[3]->pin(0));
		connect(mult_add2->pin_clock2(), eval[4]->pin(0));
		connect(mult_add2->pin_clock3(), eval[5]->pin(0));
		connect(mult_add2->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add2->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add2->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add2->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add2->pin_ena0(), eval[10]->pin(0));
		connect(mult_add2->pin_ena1(), eval[11]->pin(0));
		connect(mult_add2->pin_ena2(), eval[12]->pin(0));
		connect(mult_add2->pin_ena3(), eval[13]->pin(0));
		connect(mult_add2->pin_signa(), eval[14]->pin(0));
		connect(mult_add2->pin_signb(), eval[15]->pin(0));

		connect(mult_add3->pin_clock0(), eval[2]->pin(0));
		connect(mult_add3->pin_clock1(), eval[3]->pin(0));
		connect(mult_add3->pin_clock2(), eval[4]->pin(0));
		connect(mult_add3->pin_clock3(), eval[5]->pin(0));
		connect(mult_add3->pin_aclr0(), eval[6]->pin(0));
		connect(mult_add3->pin_aclr1(), eval[7]->pin(0));
		connect(mult_add3->pin_aclr2(), eval[8]->pin(0));
		connect(mult_add3->pin_aclr3(), eval[9]->pin(0));
		connect(mult_add3->pin_ena0(), eval[10]->pin(0));
		connect(mult_add3->pin_ena1(), eval[11]->pin(0));
		connect(mult_add3->pin_ena2(), eval[12]->pin(0));
		connect(mult_add3->pin_ena3(), eval[13]->pin(0));
		connect(mult_add3->pin_signa(), eval[14]->pin(0));
		connect(mult_add3->pin_signb(), eval[15]->pin(0));

		NetNet *sig_rs4 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs5 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs6 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
		NetNet *sig_rs7 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		NetNet *sig_rs8 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		NetNet *sig_rs9 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
		for (unsigned idx = 0 ;  idx < eval[16]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
			connect(mult_add1->pin_scanouta(idx), sig_rs4->pin(idx));//eval[16]->pin(idx)
			connect(mult_add2->pin_scanouta(idx), sig_rs5->pin(idx));//eval[16]->pin(idx)
			connect(mult_add3->pin_scanouta(idx), sig_rs6->pin(idx));//eval[16]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1){
			connect(mult_add0->pin_scanoutb(idx), eval[17]->pin(idx));
			connect(mult_add1->pin_scanoutb(idx), sig_rs7->pin(idx));//eval[17]->pin(idx)
			connect(mult_add2->pin_scanoutb(idx), sig_rs8->pin(idx));//eval[17]->pin(idx)
			connect(mult_add3->pin_scanoutb(idx), sig_rs9->pin(idx));//eval[17]->pin(idx)
		}
		connect(multaddout->pin_clock0(), eval[2]->pin(0));
		connect(multaddout->pin_clock1(), eval[3]->pin(0));
		connect(multaddout->pin_clock2(), eval[4]->pin(0));
		connect(multaddout->pin_clock3(), eval[5]->pin(0));
		connect(multaddout->pin_aclr0(), eval[6]->pin(0));
		connect(multaddout->pin_aclr1(), eval[7]->pin(0));
		connect(multaddout->pin_aclr2(), eval[8]->pin(0));
		connect(multaddout->pin_aclr3(), eval[9]->pin(0));
		connect(multaddout->pin_ena0(), eval[10]->pin(0));
		connect(multaddout->pin_ena1(), eval[11]->pin(0));
		connect(multaddout->pin_ena2(), eval[12]->pin(0));
		connect(multaddout->pin_ena3(), eval[13]->pin(0));
		connect(multaddout->pin_signa(), eval[14]->pin(0));
		connect(multaddout->pin_signb(), eval[15]->pin(0));

		NetNet *sig_rs2 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 2);
		connect(multaddout->pin_addnsub(0), sig_rs2->pin(0));
		connect(multaddout->pin_addnsub(1), sig_rs2->pin(1));

		NetNet *sig_rs0 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		NetNet *sig_rs1 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		NetNet *sig_rs3 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		NetNet *sig_rs10 = 
			new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[18]->pin_count()-2);
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count()-2 ; idx += 1){
			connect(mult_add0->pin_result(idx),sig_rs0->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add1->pin_result(idx),sig_rs1->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add2->pin_result(idx),sig_rs3->pin(idx) );//eval[18]->pin(idx)
			connect(mult_add3->pin_result(idx),sig_rs10->pin(idx) );//eval[18]->pin(idx)
			connect(sig_rs0->pin(idx),multaddout->pin_dataa(idx));//eval[18]->pin(idx)
			connect(sig_rs1->pin(idx),multaddout->pin_datab(idx) );//eval[18]->pin(idx)
			connect(sig_rs3->pin(idx),multaddout->pin_datac(idx) );//eval[18]->pin(idx)
			connect(sig_rs10->pin(idx),multaddout->pin_datad(idx) );//eval[18]->pin(idx)
		}
		for (unsigned idx = 0 ;  idx < eval[18]->pin_count() ; idx += 1){
			connect(multaddout->pin_result(idx),eval[18]->pin(idx));	
		}

	}
	else{
		assert(0);
	}



} 
else if (MaxInputData >18&&MaxInputData <=36){
	//to do 36
	assert(0);
}
else if (MaxInputData >36){
	assert(0);
}
else{
	assert(0);
}

}

void PMultAccumAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);
	Control_Coarse_DSP =Coarse_DSP;
	NetNet*eval[Mult_Acc_Parse_Number];

	for (unsigned idx = 0; idx < Mult_Acc_Parse_Number; idx++) {
		assert(pin(idx));

		/* Elaborate the r-value. Account for the initial decays,
		which are going to be attached to the last gate before the
		generated NetNet. */
		eval[idx] = pin(idx)->elaborate_net(des, scope,
			1, 0, 0, 0, Link::STRONG, Link::STRONG);
		if (eval[idx] == 0) {
			cerr << get_line() << ": error: Unable to elaborate value: "
				<< *pin(idx) << endl;
			des->errors += 1;
			return;
		}
		assert(eval[idx]);
	}

if(Control_Coarse_DSP == Coarse_DSP){

	assert(eval[0]->pin_count()/eval[2]->pin_count() 
		== eval[1]->pin_count()/eval[3]->pin_count());
	unsigned NumOfMult =eval[0]->pin_count()/eval[2]->pin_count();
	unsigned MaxInputData = eval[2]->pin_count() > eval[3]->pin_count()?eval[2]->pin_count():eval[3]->pin_count();
	assert(MaxInputData);
	if(MaxInputData <=18){
		if (NumOfMult == 1){
			//todo
			assert(0);
		}
		else if (NumOfMult == 2){
			/*二端口******************************************/
			unsigned Flog_Port_Hang = 36- 2*MaxInputData;
			HalfDSPBlock*mult_acc = new HalfDSPBlock(scope, scope->local_symbol(),
				//data
				36, 36, 0,0, 
				0,0,0,0,
				//scan
				0, 0,
				//chain
				0, 0,
				//clock/ena/aclr   aclr 没有用上。
				1,1, 0, 
				//control
				0,1,0,0,0,0,0,0,0,0,0,
				//overflow
				0,0, 
				//out
				72/*eval[4]->pin_count()*/);
			des->add_node(mult_acc);
			for (unsigned idx = 0 ;  idx < eval[0]->pin_count(); idx += 1){
				connect(mult_acc->pin_dataa0(idx), eval[0]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[1]->pin_count() ; idx += 1){
				connect(mult_acc->pin_datab0(idx), eval[1]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[4]->pin_count(); idx += 1){
				connect(mult_acc->pin_result(idx), eval[4]->pin(idx));
			}
			//clock accum_sload ena 
			for (unsigned idx = 0 ;  idx < eval[5]->pin_count(); idx += 1){
				connect(mult_acc->pin_clock(idx), eval[5]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[6]->pin_count(); idx += 1){
				connect(mult_acc->pin_accumsload(idx), eval[6]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[7]->pin_count(); idx += 1){
				connect(mult_acc->pin_ena(idx), eval[7]->pin(idx));
			}


			if (Flog_Port_Hang){
				unsigned Flog_outPort_Hang = 72 - eval[4]->pin_count();

				NetNet *sig_rs0 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号
				NetNet *sig_rs1 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号

				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_dataa0(eval[0]->pin_count() + idx ), sig_rs0->pin(idx));
				}
				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_datab0(eval[1]->pin_count() + idx ), sig_rs1->pin(idx));
				}
				NetNet *sig_rs4 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_outPort_Hang);//悬空的信号
				sig_rs4->port_type(NetNet::POUTPUT);
				for (unsigned idx = 0;  idx < Flog_outPort_Hang ; idx += 1){
					connect(mult_acc->pin_result(eval[4]->pin_count() + idx ), sig_rs4->pin(idx));
				}

			}
			/*四端口******************************************
			unsigned Flog_Port_Hang = 18- MaxInputData;
			HalfDSPBlock*mult_acc = new HalfDSPBlock(scope, scope->local_symbol(),
				//data
				18, 18, 18,18, 
				0,0,0,0,
				//scan
				0, 0,
				//chain
				0, 0,
				//clock/ena/aclr
				0,0, 0, 
				//control
				0,0,0,0,0,0,0,0,0,0,0,
				//overflow
				0,0, 
				//out
				72);
			des->add_node(mult_acc);
			for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/2; idx += 1){
				connect(mult_acc->pin_dataa0(idx), eval[0]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/2 ; idx += 1){
				connect(mult_acc->pin_datab0(idx), eval[1]->pin(idx));
			}

			for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/2; idx += 1){
				connect(mult_acc->pin_dataa1(idx), eval[0]->pin(idx + eval[0]->pin_count()/2 ));
			}
			for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/2 ; idx += 1){
				connect(mult_acc->pin_datab1(idx), eval[1]->pin(idx + eval[1]->pin_count()/2));
			}

			for (unsigned idx = 0 ;  idx < eval[4]->pin_count(); idx += 1){
				connect(mult_acc->pin_result(idx), eval[4]->pin(idx));
			}

			if (Flog_Port_Hang){
				unsigned Flog_outPort_Hang = 72 - eval[4]->pin_count();

				NetNet *sig_rs0 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号
				sig_rs0->port_type(NetNet::PINPUT);
				NetNet *sig_rs1 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号
				sig_rs1->port_type(NetNet::PINPUT);
				NetNet *sig_rs2 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号
				sig_rs2->port_type(NetNet::PINPUT);
				NetNet *sig_rs3 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_Port_Hang);//悬空的信号
				sig_rs3->port_type(NetNet::PINPUT);

				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_dataa0(eval[0]->pin_count()/2 + idx ), sig_rs0->pin(idx));
				}
				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_datab0(eval[1]->pin_count()/2 + idx ), sig_rs1->pin(idx));
				}
				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_dataa1(eval[0]->pin_count()/2 + idx ), sig_rs2->pin(idx));
				}
				for (unsigned idx = 0;  idx < Flog_Port_Hang ; idx += 1){
					connect(mult_acc->pin_datab1(eval[1]->pin_count()/2 + idx ), sig_rs3->pin(idx));
				}

				NetNet *sig_rs4 = 
					new NetNet(scope, scope->local_symbol(), NetNet::WIRE, Flog_outPort_Hang);//悬空的信号
				sig_rs0->port_type(NetNet::POUTPUT);
				for (unsigned idx = 0;  idx < Flog_outPort_Hang ; idx += 1){
					connect(mult_acc->pin_result(eval[4]->pin_count() + idx ), sig_rs4->pin(idx));
				}
				
			}
		****************************************************************/
		} 

		else if (NumOfMult == 3){
			//todo
			assert(0);
		}
		else if (NumOfMult == 4){
			//todo
			assert(0);
		}
		else
			assert(0);
	}
	else if (MaxInputData >18&&MaxInputData <=36){
		//to do 36
		assert(0);
		if (NumOfMult == 1){
			//todo
			assert(0);
		}
		else if (NumOfMult == 2){
			//todo
			assert(0);
		} 
		else if (NumOfMult == 3){
			//todo
			assert(0);
		}
		else if (NumOfMult == 4)
		{
			//todo
			assert(0);
		}
		else
			assert(0);
	}
	else if (MaxInputData >36){
		//todo
		assert(0);
		if (NumOfMult == 1){
			//todo
			assert(0);
		}
		else if (NumOfMult == 2){
			//todo
			assert(0);
		} 
		else if (NumOfMult == 3){
			//todo
			assert(0);
		}
		else if (NumOfMult == 4){
			//todo
			assert(0);
		}
		else
			assert(0);
	}
	else{
		//todo
		assert(0);
		if (NumOfMult == 1){
			//todo
			assert(0);
		}
		else if (NumOfMult == 2){
			//todo
			assert(0);
		} 
		else if (NumOfMult == 3){
			//todo
			assert(0);
		}
		else if (NumOfMult == 4){
			//todo
			assert(0);
		}
		else
			assert(0);
	}
}
if (Control_Coarse_DSP == Atom_DSP){
/*************************************
mult_mac拆分
dataa,datab,accum_sload,clock0,clock1,clock2,clock3,aclr0,aclr1,aclr2
   0        1            2               3            4          5          6       7      8        9
aclr3  ena0,ena1,ena2,ena3,signa,signb,scanouta,scanoutb,result 
  10      11     12   13    14     15      16        17           18         19
*************************************/
	assert(eval[0]->pin_count()/eval[16]->pin_count() 
		== eval[1]->pin_count()/eval[17]->pin_count());
	unsigned NumOfMult =eval[0]->pin_count()/eval[16]->pin_count();
	unsigned MaxInputData = eval[17]->pin_count() > eval[17]->pin_count()?eval[16]->pin_count():eval[17]->pin_count();
	assert(MaxInputData);

	if(MaxInputData <=18){
		if (NumOfMult == 1){
			//NetMultAccum*mult_accum = new NetMultAccum(scope, scope->local_symbol(),
			//	eval[0]->pin_count(),
			//	eval[1]->pin_count(),
			//	eval[16]->pin_count(),
			//	eval[17]->pin_count(),
			//	eval[19]->pin_count());
			//des->add_node(mult_accum);

			//for (unsigned idx = 0 ;  idx < eval[0]->pin_count() ; idx += 1)
			//	connect(mult_accum->pin_dataa(idx), eval[0]->pin(idx));
			//for (unsigned idx = 0 ;  idx < eval[1]->pin_count() ; idx += 1)
			//	connect(mult_accum->pin_datab(idx), eval[1]->pin(idx));
			//connect(mult_accum->pin_accum_sload(), eval[2]->pin(0));
			//connect(mult_accum->pin_clock0(), eval[3]->pin(0));
			//connect(mult_accum->pin_clock1(), eval[4]->pin(0));
			//connect(mult_accum->pin_clock2(), eval[5]->pin(0));
			//connect(mult_accum->pin_clock3(), eval[6]->pin(0));
			//connect(mult_accum->pin_aclr0(), eval[7]->pin(0));
			//connect(mult_accum->pin_aclr1(), eval[8]->pin(0));
			//connect(mult_accum->pin_aclr2(), eval[9]->pin(0));
			//connect(mult_accum->pin_aclr3(), eval[10]->pin(0));
			//connect(mult_accum->pin_ena0(), eval[11]->pin(0));
			//connect(mult_accum->pin_ena1(), eval[12]->pin(0));
			//connect(mult_accum->pin_ena2(), eval[13]->pin(0));
			//connect(mult_accum->pin_ena3(), eval[14]->pin(0));
			//for (unsigned idx = 0 ;  idx < eval[15]->pin_count() ; idx += 1)
			//	connect(mult_accum->pin_scanouta(idx), eval[15]->pin(idx));
			//for (unsigned idx = 0 ;  idx < eval[16]->pin_count() ; idx += 1)
			//	connect(mult_accum->pin_scanoutb(idx), eval[16]->pin(idx));
			//for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1)
			//	connect(mult_accum->pin_result(idx), eval[17]->pin(idx));
			//connect(mult_accum->pin_overflow(), eval[18]->pin(0));
			NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
				eval[0]->pin_count(),
				eval[1]->pin_count(),
				eval[16]->pin_count(),
				eval[17]->pin_count(),
				eval[16]->pin_count()+eval[17]->pin_count());
			des->add_node(mult_add0);
			NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
				eval[16]->pin_count()+eval[17]->pin_count(), 0,  0,  0, 
				1, 1, eval[19]->pin_count());
			des->add_node(multaddout);

			for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/2; idx += 1){
				connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
			}
			for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/2 ; idx += 1){
				connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));
			}
			connect(mult_add0->pin_clock0(), eval[2]->pin(0));
			connect(mult_add0->pin_clock1(), eval[3]->pin(0));
			connect(mult_add0->pin_clock2(), eval[4]->pin(0));
			connect(mult_add0->pin_clock3(), eval[5]->pin(0));
			connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
			connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
			connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
			connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
			connect(mult_add0->pin_ena0(), eval[10]->pin(0));
			connect(mult_add0->pin_ena1(), eval[11]->pin(0));
			connect(mult_add0->pin_ena2(), eval[12]->pin(0));
			connect(mult_add0->pin_ena3(), eval[13]->pin(0));
			connect(mult_add0->pin_signa(), eval[14]->pin(0));
			connect(mult_add0->pin_signb(), eval[15]->pin(0));

			NetNet *sig_rs4 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
			for (unsigned idx = 0 ;  idx < eval[16]->pin_count(); idx += 1){
				connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
			}
			connect(multaddout->pin_clock0(), eval[2]->pin(0));
			connect(multaddout->pin_clock1(), eval[3]->pin(0));
			connect(multaddout->pin_clock2(), eval[4]->pin(0));
			connect(multaddout->pin_clock3(), eval[5]->pin(0));
			connect(multaddout->pin_aclr0(), eval[6]->pin(0));
			connect(multaddout->pin_aclr1(), eval[7]->pin(0));
			connect(multaddout->pin_aclr2(), eval[8]->pin(0));
			connect(multaddout->pin_aclr3(), eval[9]->pin(0));
			connect(multaddout->pin_ena0(), eval[10]->pin(0));
			connect(multaddout->pin_ena1(), eval[11]->pin(0));
			connect(multaddout->pin_ena2(), eval[12]->pin(0));
			connect(multaddout->pin_ena3(), eval[13]->pin(0));
			connect(multaddout->pin_signa(), eval[14]->pin(0));
			connect(multaddout->pin_signb(), eval[15]->pin(0));
			connect(multaddout->pin_accumsload(0), eval[18]->pin(0));
			NetNet *sig_rs2 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1);
			connect(multaddout->pin_addnsub(0), sig_rs2->pin(0));

			NetNet *sig_rs0 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count()+eval[17]->pin_count());
			for (unsigned idx = 0 ;  idx < eval[16]->pin_count()+eval[17]->pin_count() ; idx += 1){
				connect(mult_add0->pin_result(idx),sig_rs0->pin(idx) );//eval[18]->pin(idx)
				connect(sig_rs0->pin(idx),multaddout->pin_dataa(idx));//eval[18]->pin(idx)
			}

			for (unsigned idx = 0 ;  idx < eval[19]->pin_count() ; idx += 1){
				connect(multaddout->pin_result(idx),eval[19]->pin(idx));	
			}
		}
		else if (NumOfMult == 2){
			NetMultAdd*mult_add0 = new NetMultAdd(scope, scope->local_symbol(),
				eval[0]->pin_count()/2,
				eval[1]->pin_count()/2,
				eval[16]->pin_count(),
				eval[17]->pin_count(),
				eval[16]->pin_count()+eval[17]->pin_count());
			des->add_node(mult_add0);

			NetMultAdd*mult_add1 = new NetMultAdd(scope, scope->local_symbol(),
				eval[0]->pin_count()/2,
				eval[1]->pin_count()/2,
				eval[16]->pin_count(),
				eval[17]->pin_count(),
				eval[16]->pin_count()+eval[17]->pin_count());
			des->add_node(mult_add1);

			NetMultAddOut* multaddout = new NetMultAddOut(scope, scope->local_symbol(),
				eval[16]->pin_count()+eval[17]->pin_count(), 
				eval[16]->pin_count()+eval[17]->pin_count(),  0,  0, 
				1, 1, eval[19]->pin_count());
			des->add_node(multaddout);

			for (unsigned idx = 0 ;  idx < eval[0]->pin_count()/2; idx += 1){
				connect(mult_add0->pin_dataa(idx), eval[0]->pin(idx));
				connect(mult_add1->pin_dataa(idx), eval[0]->pin(idx+eval[0]->pin_count()/2));
			}
			for (unsigned idx = 0 ;  idx < eval[1]->pin_count()/2 ; idx += 1){
				connect(mult_add0->pin_datab(idx), eval[1]->pin(idx));
				connect(mult_add1->pin_datab(idx), eval[1]->pin(idx+eval[1]->pin_count()/2));
			}
			connect(mult_add0->pin_clock0(), eval[2]->pin(0));
			connect(mult_add0->pin_clock1(), eval[3]->pin(0));
			connect(mult_add0->pin_clock2(), eval[4]->pin(0));
			connect(mult_add0->pin_clock3(), eval[5]->pin(0));
			connect(mult_add0->pin_aclr0(), eval[6]->pin(0));
			connect(mult_add0->pin_aclr1(), eval[7]->pin(0));
			connect(mult_add0->pin_aclr2(), eval[8]->pin(0));
			connect(mult_add0->pin_aclr3(), eval[9]->pin(0));
			connect(mult_add0->pin_ena0(), eval[10]->pin(0));
			connect(mult_add0->pin_ena1(), eval[11]->pin(0));
			connect(mult_add0->pin_ena2(), eval[12]->pin(0));
			connect(mult_add0->pin_ena3(), eval[13]->pin(0));
			connect(mult_add0->pin_signa(), eval[14]->pin(0));
			connect(mult_add0->pin_signb(), eval[15]->pin(0));

			connect(mult_add1->pin_clock0(), eval[2]->pin(0));
			connect(mult_add1->pin_clock1(), eval[3]->pin(0));
			connect(mult_add1->pin_clock2(), eval[4]->pin(0));
			connect(mult_add1->pin_clock3(), eval[5]->pin(0));
			connect(mult_add1->pin_aclr0(), eval[6]->pin(0));
			connect(mult_add1->pin_aclr1(), eval[7]->pin(0));
			connect(mult_add1->pin_aclr2(), eval[8]->pin(0));
			connect(mult_add1->pin_aclr3(), eval[9]->pin(0));
			connect(mult_add1->pin_ena0(), eval[10]->pin(0));
			connect(mult_add1->pin_ena1(), eval[11]->pin(0));
			connect(mult_add1->pin_ena2(), eval[12]->pin(0));
			connect(mult_add1->pin_ena3(), eval[13]->pin(0));
			connect(mult_add1->pin_signa(), eval[14]->pin(0));
			connect(mult_add1->pin_signb(), eval[15]->pin(0));

			NetNet *sig_rs4 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count());
			NetNet *sig_rs5 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[17]->pin_count());
			for (unsigned idx = 0 ;  idx < eval[16]->pin_count(); idx += 1){
				connect(mult_add0->pin_scanouta(idx), eval[16]->pin(idx));
				connect(mult_add1->pin_scanouta(idx), sig_rs4->pin(idx));//eval[16]->pin(idx)
			}
			for (unsigned idx = 0 ;  idx < eval[17]->pin_count() ; idx += 1){
				connect(mult_add0->pin_scanoutb(idx), eval[17]->pin(idx));
				connect(mult_add1->pin_scanoutb(idx), sig_rs5->pin(idx));//eval[17]->pin(idx)
			}

			connect(multaddout->pin_clock0(), eval[2]->pin(0));
			connect(multaddout->pin_clock1(), eval[3]->pin(0));
			connect(multaddout->pin_clock2(), eval[4]->pin(0));
			connect(multaddout->pin_clock3(), eval[5]->pin(0));
			connect(multaddout->pin_aclr0(), eval[6]->pin(0));
			connect(multaddout->pin_aclr1(), eval[7]->pin(0));
			connect(multaddout->pin_aclr2(), eval[8]->pin(0));
			connect(multaddout->pin_aclr3(), eval[9]->pin(0));
			connect(multaddout->pin_ena0(), eval[10]->pin(0));
			connect(multaddout->pin_ena1(), eval[11]->pin(0));
			connect(multaddout->pin_ena2(), eval[12]->pin(0));
			connect(multaddout->pin_ena3(), eval[13]->pin(0));
			connect(multaddout->pin_signa(), eval[14]->pin(0));
			connect(multaddout->pin_signb(), eval[15]->pin(0));
			connect(multaddout->pin_accumsload(0), eval[18]->pin(0));
			NetNet *sig_rs2 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1);
			connect(multaddout->pin_addnsub(0), sig_rs2->pin(0));

			NetNet *sig_rs0 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count()+eval[17]->pin_count());
			NetNet *sig_rs1 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, eval[16]->pin_count()+eval[17]->pin_count());
			for (unsigned idx = 0 ;  idx < eval[16]->pin_count()+eval[17]->pin_count() ; idx += 1){
				connect(mult_add0->pin_result(idx),sig_rs0->pin(idx) );//eval[18]->pin(idx)
				connect(mult_add1->pin_result(idx),sig_rs1->pin(idx) );//eval[18]->pin(idx)
				connect(sig_rs0->pin(idx),multaddout->pin_dataa(idx));//eval[18]->pin(idx)
				connect(sig_rs1->pin(idx),multaddout->pin_datab(idx) );//eval[18]->pin(idx)
			}
			for (unsigned idx = 0 ;  idx < eval[19]->pin_count() ; idx += 1){
				connect(multaddout->pin_result(idx),eval[19]->pin(idx));	
			}
		}
		else if (NumOfMult == 3){
			NetNet *sig_rs1 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1);
		}
		else if (NumOfMult == 4){
			NetNet *sig_rs1 = 
				new NetNet(scope, scope->local_symbol(), NetNet::WIRE, 1);
		}
		else{
			assert(0);
		}
	}
	else if (MaxInputData >18&&MaxInputData <=36){
		//to do 36
		assert(0);
	}
	else if (MaxInputData >36){
		assert(0);
	}
	else{
		assert(0);
	}
}

}

/*
* Elaborate a Builtin gate. These normally get translated into
* NetLogic nodes that reflect the particular logic function.
*/
void PGBuiltin::elaborate(Design*des, NetScope*scope) const
{
	unsigned count = 1;
	long low = 0, high = 0;
	string name = string(get_name());

	if (name == "")
		name = scope->local_symbol();

	/* If the Verilog source has a range specification for the
	gates, then I am expected to make more then one
	gate. Figure out how many are desired. */
	if (msb_) {
		NetExpr*msb_exp = elab_and_eval(des, scope, msb_);
		NetExpr*lsb_exp = elab_and_eval(des, scope, lsb_);

		NetEConst*msb_con = dynamic_cast<NetEConst*>(msb_exp);
		NetEConst*lsb_con = dynamic_cast<NetEConst*>(lsb_exp);

		if (msb_con == 0) {
			cerr << get_line() << ": error: Unable to evaluate "
				"expression " << *msb_ << endl;
			des->errors += 1;
			return;
		}

		if (lsb_con == 0) {
			cerr << get_line() << ": error: Unable to evaluate "
				"expression " << *lsb_ << endl;
			des->errors += 1;
			return;
		}

		verinum msb = msb_con->value();
		verinum lsb = lsb_con->value();

		delete msb_exp;
		delete lsb_exp;

		if (msb.as_long() > lsb.as_long())
			count = msb.as_long() - lsb.as_long() + 1;
		else
			count = lsb.as_long() - msb.as_long() + 1;

		low = lsb.as_long();
		high = msb.as_long();
	}


	/* Allocate all the netlist nodes for the gates. */
	NetLogic**cur = new NetLogic*[count];
	assert(cur);

	/* Calculate the gate delays from the delay expressions
	given in the source. For logic gates, the decay time
	is meaningless because it can never go to high
	impedance. However, the bufif devices can generate
	'bz output, so we will pretend that anything can.

	If only one delay value expression is given (i.e., #5
	nand(foo,...)) then rise, fall and decay times are
	all the same value. If two values are given, rise and
	fall times are use, and the decay time is the minimum
	of the rise and fall times. Finally, if all three
	values are given, they are taken as specified. */

	unsigned long rise_time, fall_time, decay_time;
	eval_delays(des, scope, rise_time, fall_time, decay_time);

	struct attrib_list_t*attrib_list = 0;
	unsigned attrib_list_n = 0;
	attrib_list = evaluate_attributes(attributes, attrib_list_n,
		des, scope);

	/* Now make as many gates as the bit count dictates. Give each
	a unique name, and set the delay times. */

	for (unsigned idx = 0 ;  idx < count ;  idx += 1) {
		ostringstream tmp;
		unsigned index;
		if (low < high)
			index = low + idx;
		else
			index = low - idx;

		tmp << name << "<" << index << ">";
		perm_string inm = lex_strings.make(tmp.str());

		switch (type()) {
		case AND:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::AND);
			break;
		case BUF:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::BUF);
			break;
		case BUFIF0:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::BUFIF0);
			break;
		case BUFIF1:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::BUFIF1);
			break;
		case NAND:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NAND);
			break;
		case NMOS:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NMOS);
			break;
		case NOR:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NOR);
			break;
		case NOT:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NOT);
			break;
		case NOTIF0:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NOTIF0);
			break;
		case NOTIF1:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::NOTIF1);
			break;
		case OR:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::OR);
			break;
		case RNMOS:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::RNMOS);
			break;
		case RPMOS:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::RPMOS);
			break;
		case PMOS:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::PMOS);
			break;
		case PULLDOWN:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::PULLDOWN);
			break;
		case PULLUP:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::PULLUP);
			break;
		case XNOR:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::XNOR);
			break;
		case XOR:
			cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::XOR);
			break;
		case PLL:
			{
				cur[idx] = new NetLogic(scope, inm, pin_count(),
				NetLogic::PLL);
				NetScope* up = scope->parent();
				perm_string up_module_name = up->module_name();
				verinum atrri_val(up_module_name.str());
				cur[idx]->attribute("up_module_name",atrri_val);
			}	
			break;
			
		default:
			cerr << get_line() << ": internal error: unhandled "
				"gate type." << endl;
			des->errors += 1;
			return;
		}

		for (unsigned adx = 0 ;  adx < attrib_list_n ;  adx += 1)
			cur[idx]->attribute(attrib_list[adx].key,
			attrib_list[adx].val);

		cur[idx]->rise_time(rise_time);
		cur[idx]->fall_time(fall_time);
		cur[idx]->decay_time(decay_time);

		cur[idx]->pin(0).drive0(drive_type(strength0()));
		cur[idx]->pin(0).drive1(drive_type(strength1()));

		des->add_node(cur[idx]);
	}


	delete[]attrib_list;

	/* The gates have all been allocated, this loop runs through
	the parameters and attaches the ports of the objects. */

	for (unsigned idx = 0 ;  idx < pin_count() ;  idx += 1) {
		const PExpr*ex = pin(idx);
		if (ex == 0) {
			cerr << get_line() << ": error: Logic gate port "
				"expressions are not optional." << endl;
			des->errors += 1;
			return;
		}
		NetNet*sig = ((type()==PLL)?(idx >= 4):(idx == 0))
			? ex->elaborate_lnet(des, scope, true)
			: ex->elaborate_net(des, scope, 0, 0, 0, 0);
		if (sig == 0)
			continue;

		assert(sig);

		if (sig->pin_count() == 1)
			for (unsigned gdx = 0 ;  gdx < count ;  gdx += 1)
				connect(cur[gdx]->pin(idx), sig->pin(0));

		else if (sig->pin_count() == count)
			for (unsigned gdx = 0 ;  gdx < count ;  gdx += 1)
				connect(cur[gdx]->pin(idx), sig->pin(gdx));

		else {
			cerr << get_line() << ": error: Gate count of " <<
				count << " does not match net width of " <<
				sig->pin_count() << " at pin " << idx << "."
				<< endl;
			des->errors += 1;
		}

		if (NetSubnet*tmp = dynamic_cast<NetSubnet*>(sig))
			delete tmp;
	}
}

/*
* Instantiate a module by recursively elaborating it. Set the path of
* the recursive elaboration so that signal names get properly
* set. Connect the ports of the instantiated module to the signals of
* the parameters. This is done with BUFZ gates so that they look just
* like continuous assignment connections.
*/
void PGModule::elaborate_mod_(Design*des, Module*rmod, NetScope*scope) const
{

	assert(scope);

	// This is the array of pin expressions, shuffled to match the
	// order of the declaration. If the source instantiation uses
	// bind by order, this is the same as the source
	// list. Otherwise, the source list is rearranged by name
	// binding into this list.
	svector<PExpr*>pins (rmod->port_count());

	// Detect binding by name. If I am binding by name, then make
	// up a pins array that reflects the positions of the named
	// ports. If this is simply positional binding in the first
	// place, then get the binding from the base class.
	if (pins_) {
		unsigned nexp = rmod->port_count();

		// Scan the bindings, matching them with port names.
		for (unsigned idx = 0 ;  idx < npins_ ;  idx += 1) {

			// Given a binding, look at the module port names
			// for the position that matches the binding name.
			unsigned pidx = rmod->find_port(pins_[idx].name);

			// If the port name doesn't exist, the find_port
			// method will return the port count. Detect that
			// as an error.
			if (pidx == nexp) {
				cerr << get_line() << ": error: port ``" <<
					pins_[idx].name << "'' is not a port of "
					<< get_name() << "." << endl;
				des->errors += 1;
			if (strcmp(pins_[idx].name,"coefsel0") ==0)
			{
				des->errors -= 1;
			}
			if (strcmp(pins_[idx].name,"coefsel1") ==0)
			{
				des->errors -= 1;
			}
			if (strcmp(pins_[idx].name,"coefsel2") ==0)
			{
				des->errors -= 1;
			}
			if (strcmp(pins_[idx].name,"coefsel3") ==0)
			{
				des->errors -= 1;
			}
			if (strcmp(pins_[idx].name,"datac") ==0)
			{
				des->errors -= 1;
			}
			
				continue;
			}

			// If I already bound something to this port, then
			// the (*exp) array will already have a pointer
			// value where I want to place this expression.
			if (pins[pidx]) {
				cerr << get_line() << ": error: port ``" <<
					pins_[idx].name << "'' already bound." <<
					endl;
				des->errors += 1;
				continue;
			}

			// OK, do the binding by placing the expression in
			// the right place.
			pins[pidx] = pins_[idx].parm;
		}


	} else if (pin_count() == 0) {

		/* Handle the special case that no ports are
		connected. It is possible that this is an empty
		connect-by-name list, so we'll allow it and assume
		that is the case. */

		for (unsigned idx = 0 ;  idx < rmod->port_count() ;  idx += 1)
			pins[idx] = 0;

	} else {

		/* Otherwise, this is a positional list of fort
		connections. In this case, the port count must be
		right. Check that is is, the get the pin list. */

		if (pin_count() != rmod->port_count()) {
			cerr << get_line() << ": error: Wrong number "
				"of ports. Expecting " << rmod->port_count() <<
				", got " << pin_count() << "."
				<< endl;
			des->errors += 1;
			return;
		}

		// No named bindings, just use the positional list I
		// already have.
		assert(pin_count() == rmod->port_count());
		pins = get_pins();
	}


	// Elaborate these instances of the module. The recursive
	// elaboration causes the module to generate a netlist with
	// the ports represented by NetNet objects. I will find them
	// later.

	NetScope::scope_vec_t&instance = scope->instance_arrays[get_name()];
	for (unsigned inst = 0 ;  inst < instance.count() ;  inst += 1) {
		rmod->elaborate(des, instance[inst]);
	}



	// Now connect the ports of the newly elaborated designs to
	// the expressions that are the instantiation parameters. Scan
	// the pins, elaborate the expressions attached to them, and
	// bind them to the port of the elaborated module.

	// This can get rather complicated because the port can be
	// unconnected (meaning an empty parameter is passed) connected
	// to a concatenation, or connected to an internally
	// unconnected port.

	for (unsigned idx = 0 ;  idx < pins.count() ;  idx += 1) {

		// Skip unconnected module ports. This happens when a
		// null parameter is passed in.

		if (pins[idx] == 0) {

			// While we're here, look to see if this
			// unconnected (from the outside) port is an
			// input. If so, consider printing a port binding
			// warning.
			if (warn_portbinding) {
				svector<PEIdent*> mport = rmod->get_port(idx);
				if (mport.count() == 0)
					continue;

				NetNet*tmp = des->find_signal(instance[0],
					mport[0]->path());
				assert(tmp);

				if (tmp->port_type() == NetNet::PINPUT) {
					cerr << get_line() << ": warning: "
						<< "Instantiating module "
						<< rmod->mod_name()
						<< " with dangling input port "
						<< rmod->ports[idx]->name
						<< "." << endl;
				}
			}

			continue;
		}

		// Inside the module, the port is zero or more signals
		// that were already elaborated. List all those signals
		// and the NetNet equivalents, for all the instances.
		svector<PEIdent*> mport = rmod->get_port(idx);
		svector<NetNet*>prts (mport.count() * instance.count());

		// Count the internal pins of the port.
		unsigned prts_pin_count = 0;

		for (unsigned inst = 0 ;  inst < instance.count() ;  inst += 1) {
			NetScope*inst_scope = instance[inst];

			// Scan the module sub-ports for this instance...
			for (unsigned ldx = 0 ;  ldx < mport.count() ;  ldx += 1) {
				unsigned lbase = inst * mport.count();
				PEIdent*pport = mport[ldx];
				assert(pport);
				prts[lbase + ldx]
				= pport->elaborate_port(des, inst_scope);
				if (prts[lbase + ldx] == 0)
					continue;

				assert(prts[lbase + ldx]);
				prts_pin_count += prts[lbase + ldx]->pin_count();
			}
		}

		// If I find that the port in unconnected inside the
		// module, then there is nothing to connect. Skip the
		// argument.
		if (prts_pin_count == 0) {
			continue;
		}

		// We know by design that each instance has the same
		// width port. Therefore, the prts_pin_count must be an
		// even multiple of the instance count.
		assert(prts_pin_count % instance.count() == 0);

		// Elaborate the expression that connects to the
		// module[s] port. sig is the thing outside the module
		// that connects to the port.

		NetNet*sig;
		if ((prts.count() >= 1)
			&& (prts[0]->port_type() != NetNet::PINPUT)) {

				sig = pins[idx]->elaborate_lnet(des, scope, true);
				if (sig == 0) {
					cerr << pins[idx]->get_line() << ": error: "
						<< "Output port expression must support "
						<< "continuous assignment." << endl;
					cerr << pins[idx]->get_line() << ":      : "
						<< "Port of " << rmod->mod_name()
						<< " is " << rmod->ports[idx]->name << endl;
					des->errors += 1;
					continue;
				}

		} else {
			/* Input to module. elaborate the expression to
			the desired width. If this in an instance
			array, then let the net determine it's own
			width. We use that, then, to decide how to hook
			it up. */
			unsigned desired_pin_count = prts_pin_count;
			if (instance.count() != 1)
				desired_pin_count = 0;

			sig = pins[idx]->elaborate_net(des, scope,
				desired_pin_count,
				0, 0, 0);
			if (sig == 0) {
				cerr << pins[idx]->get_line()
					<< ": internal error: Port expression "
					<< "too complicated for elaboration." << endl;
				continue;
			}
		}

		assert(sig);

#ifndef NDEBUG
		if ((prts.count() >= 1)
			&& (prts[0]->port_type() != NetNet::PINPUT)) {
				assert(sig->type() != NetNet::REG);
		}
#endif

		/* If we are working with an instance array, then the
		signal width must match the port width exactly. */
		if ((instance.count() != 1)
			&& (sig->pin_count() != prts_pin_count)
			&& (sig->pin_count() != prts_pin_count/instance.count())) {
				cerr << pins[idx]->get_line() << ": error: "
					<< "Port expression width " << sig->pin_count()
					<< " does not match expected width " << prts_pin_count
					<< " or " << (prts_pin_count/instance.count())
					<< "." << endl;
				des->errors += 1;
				continue;
		}

		// Check that the parts have matching pin counts. If
		// not, they are different widths. Note that idx is 0
		// based, but users count parameter positions from 1.
		if ((instance.count() == 1)
			&& (prts_pin_count != sig->pin_count())) {
				cerr << get_line() << ": warning: Port " << (idx+1)
					<< " (" << rmod->ports[idx]->name << ") of "
					<< type_ << " expects " << prts_pin_count <<
					" bits, got " << sig->pin_count() << "." << endl;

				if (prts_pin_count > sig->pin_count()) {
					cerr << get_line() << ":        : Leaving "
						<< (prts_pin_count-sig->pin_count())
						<< " high bits of the port unconnected."
						<< endl;
				} else {
					cerr << get_line() << ":        : Leaving "
						<< (sig->pin_count()-prts_pin_count)
						<< " high bits of the expression dangling."
						<< endl;
				}
		}

		// Connect the sig expression that is the context of the
		// module instance to the ports of the elaborated module.

		// The prts_pin_count variable is the total width of the
		// port and is the maximum number of connections to
		// make. sig is the elaborated expression that connects
		// to that port. If sig has too few pins, then reduce
		// the number of connections to make.

		// Connect this many of the port pins. If the expression
		// is too small, the reduce the number of connects.
		unsigned ccount = prts_pin_count;
		if (instance.count() == 1 && sig->pin_count() < ccount)
			ccount = sig->pin_count();

		// The spin_modulus is the width of the signal (not the
		// port) if this is an instance array. This causes
		// signals wide enough for a single instance to be
		// connected to all the instances.
		unsigned spin_modulus = prts_pin_count;
		if (instance.count() != 1)
			spin_modulus = sig->pin_count();

		// Now scan the concatenation that makes up the port,
		// connecting pins until we run out of port pins or sig
		// pins.

		unsigned spin = 0;
		for (unsigned ldx = prts.count() ;  ldx > 0 ;  ldx -= 1) {
			unsigned cnt = prts[ldx-1]->pin_count();
			if (cnt > ccount)
				cnt = ccount;
			for (unsigned p = 0 ;  p < cnt ;  p += 1) {
				connect(sig->pin(spin%spin_modulus),
					prts[ldx-1]->pin(p));
				ccount -= 1;
				spin += 1;
			}
			if (ccount == 0)
				break;
		}


		if (NetSubnet*tmp = dynamic_cast<NetSubnet*>(sig))
			delete tmp;
	}

}

/*
* From a UDP definition in the source, make a NetUDP
* object. Elaborate the pin expressions as netlists, then connect
* those networks to the pins.
*/

void PGModule::elaborate_udp_(Design*des, PUdp*udp, NetScope*scope) const
{

	perm_string my_name = get_name();
	if (my_name == 0)
		my_name = scope->local_symbol();

	/* When the parser notices delay expressions in front of a
	module or primitive, it interprets them as parameter
	overrides. Correct that misconception here. */
	unsigned long rise_time = 0, fall_time = 0, decay_time = 0;
	if (overrides_) {
		PDelays tmp_del;
		tmp_del.set_delays(overrides_, false);
		tmp_del.eval_delays(des, scope, rise_time, fall_time, decay_time);
	}


	assert(udp);
	NetUDP*net = new NetUDP(scope, my_name, udp->ports.count(), udp);
	net->rise_time(rise_time);
	net->fall_time(fall_time);
	net->decay_time(decay_time);

	struct attrib_list_t*attrib_list = 0;
	unsigned attrib_list_n = 0;
	attrib_list = evaluate_attributes(attributes, attrib_list_n,
		des, scope);

	for (unsigned adx = 0 ;  adx < attrib_list_n ;  adx += 1)
		net->attribute(attrib_list[adx].key, attrib_list[adx].val);

	delete[]attrib_list;


	// This is the array of pin expressions, shuffled to match the
	// order of the declaration. If the source instantiation uses
	// bind by order, this is the same as the source
	// list. Otherwise, the source list is rearranged by name
	// binding into this list.
	svector<PExpr*>pins;

	// Detect binding by name. If I am binding by name, then make
	// up a pins array that reflects the positions of the named
	// ports. If this is simply positional binding in the first
	// place, then get the binding from the base class.
	if (pins_) {
		unsigned nexp = udp->ports.count();
		pins = svector<PExpr*>(nexp);

		// Scan the bindings, matching them with port names.
		for (unsigned idx = 0 ;  idx < npins_ ;  idx += 1) {

			// Given a binding, look at the module port names
			// for the position that matches the binding name.
			unsigned pidx = udp->find_port(pins_[idx].name);

			// If the port name doesn't exist, the find_port
			// method will return the port count. Detect that
			// as an error.
			if (pidx == nexp) {
				cerr << get_line() << ": error: port ``" <<
					pins_[idx].name << "'' is not a port of "
					<< get_name() << "." << endl;
				des->errors += 1;
				continue;
			}

			// If I already bound something to this port, then
			// the (*exp) array will already have a pointer
			// value where I want to place this expression.
			if (pins[pidx]) {
				cerr << get_line() << ": error: port ``" <<
					pins_[idx].name << "'' already bound." <<
					endl;
				des->errors += 1;
				continue;
			}

			// OK, do the binding by placing the expression in
			// the right place.
			pins[pidx] = pins_[idx].parm;
		}

	} else {

		/* Otherwise, this is a positional list of port
		connections. In this case, the port count must be
		right. Check that is is, the get the pin list. */

		if (pin_count() != udp->ports.count()) {
			cerr << get_line() << ": error: Wrong number "
				"of ports. Expecting " << udp->ports.count() <<
				", got " << pin_count() << "."
				<< endl;
			des->errors += 1;
			return;
		}

		// No named bindings, just use the positional list I
		// already have.
		assert(pin_count() == udp->ports.count());
		pins = get_pins();
	}


	/* Handle the output port of the primitive special. It is an
	output port (the only output port) so must be passed an
	l-value net. */
	if (pins[0] == 0) {
		cerr << get_line() << ": warning: output port unconnected."
			<< endl;

	} else {
		NetNet*sig = pins[0]->elaborate_lnet(des, scope, true);
		if (sig == 0) {
			cerr << get_line() << ": error: "
				<< "Output port expression is not valid." << endl;
			cerr << get_line() << ":      : Output "
				<< "port of " << udp->name_
				<< " is " << udp->ports[0] << "." << endl;
			des->errors += 1;
		} else {
			connect(sig->pin(0), net->pin(0));
		}
	}

	/* Run through the pins, making netlists for the pin
	expressions and connecting them to the pin in question. All
	of this is independent of the nature of the UDP. */
	for (unsigned idx = 1 ;  idx < net->pin_count() ;  idx += 1) {
		if (pins[idx] == 0)
			continue;

		NetNet*sig = pins[idx]->elaborate_net(des, scope, 1, 0, 0, 0);
		if (sig == 0) {
			cerr << "internal error: Expression too complicated "
				"for elaboration:" << pins[idx] << endl;
			continue;
		}

		connect(sig->pin(0), net->pin(idx));

		// Delete excess holding signal.
		if (NetSubnet*tmp = dynamic_cast<NetSubnet*>(sig))
			delete tmp;
	}

	// All done. Add the object to the design.
	des->add_node(net);
}


bool PGModule::elaborate_sig(Design*des, NetScope*scope) const
{
	// Look for the module type
	map<perm_string,Module*>::const_iterator mod = pform_modules.find(type_);
	if (mod != pform_modules.end())
		return elaborate_sig_mod_(des, scope, (*mod).second);

	return true;
}


void PGModule::elaborate(Design*des, NetScope*scope) const
{
	// Look for the module type
	map<perm_string,Module*>::const_iterator mod = pform_modules.find(type_);
	if (mod != pform_modules.end()) {
		elaborate_mod_(des, (*mod).second, scope);
		return;
	}

	// Try a primitive type
	map<perm_string,PUdp*>::const_iterator udp = pform_primitives.find(type_);
	if (udp != pform_primitives.end()) {
		assert((*udp).second);
		elaborate_udp_(des, (*udp).second, scope);
		return;
	}

	cerr << get_line() << ": internal error: Unknown module type: " <<
		type_ << endl;
}

void PGModule::elaborate_scope(Design*des, NetScope*sc) const
{
	// Look for the module type
	map<perm_string,Module*>::const_iterator mod = pform_modules.find(type_);
	if (mod != pform_modules.end()) {
		elaborate_scope_mod_(des, (*mod).second, sc);
		return;
	}

	// Try a primitive type
	map<perm_string,PUdp*>::const_iterator udp = pform_primitives.find(type_);
	if (udp != pform_primitives.end())
		return;

	// Not a module or primitive that I know about yet, so try to
	// load a library module file (which parses some new Verilog
	// code) and try again.
	//  if (load_module(type_)) {

	//   // Try again to find the module type
	// mod = pform_modules.find(type_);
	// if (mod != pform_modules.end()) {
	//elaborate_scope_mod_(des, (*mod).second, sc);
	//return;
	// }

	//   // Try again to find a primitive type
	// udp = pform_primitives.find(type_);
	// if (udp != pform_primitives.end())
	//return;
	//  }


	// Not a module or primitive that I know about or can find by
	// any means, so give up.
	cerr << get_line() << ": error: Unknown module type: " << type_ << endl;
	missing_modules[type_] += 1;
	des->errors += 1;
}


NetProc* Statement::elaborate(Design*des, NetScope*) const
{
	cerr << get_line() << ": internal error: elaborate: "
		"What kind of statement? " << typeid(*this).name() << endl;
	NetProc*cur = new NetProc;
	des->errors += 1;
	return cur;
}


NetAssign_* PAssign_::elaborate_lval(Design*des, NetScope*scope) const
{
	assert(lval_);
	return lval_->elaborate_lval(des, scope);
}

/*
* This function elaborates delay expressions. This is a little
* different from normal elaboration because the result may need to be
* scaled.
*/
static NetExpr*elaborate_delay_expr(PExpr*expr, Design*des, NetScope*scope)
{
	NetExpr*dex = elab_and_eval(des, scope, expr);

	/* If the delay expression is a real constant or vector
	constant, then evaluate it, scale it to the local time
	units, and return an adjusted NetEConst. */

	if (NetECReal*tmp = dynamic_cast<NetECReal*>(dex)) {
		verireal fn = tmp->value();

		int shift = scope->time_unit() - des->get_precision();
		long delay = fn.as_long(shift);
		if (delay < 0)
			delay = 0;

		delete tmp;
		return new NetEConst(verinum(delay));
	}


	if (NetEConst*tmp = dynamic_cast<NetEConst*>(dex)) {
		verinum fn = tmp->value();

		unsigned long delay =
			des->scale_to_precision(fn.as_ulong(), scope);

		delete tmp;
		return new NetEConst(verinum(delay));
	}


	/* The expression is not constant, so generate an expanded
	expression that includes the necessary scale shifts, and
	return that expression. */
	int shift = scope->time_unit() - des->get_precision();
	if (shift > 0) {
		unsigned long scale = 1;
		while (shift > 0) {
			scale *= 10;
			shift -= 1;
		}

		NetExpr*scal_val = new NetEConst(verinum(scale));
		dex = new NetEBMult('*', dex, scal_val);
	}

	if (shift < 0) {
		unsigned long scale = 1;
		while (shift < 0) {
			scale *= 10;
			shift += 1;
		}

		NetExpr*scal_val = new NetEConst(verinum(scale));
		dex = new NetEBDiv('/', dex, scal_val);
	}

	return dex;
}

NetProc* PAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	/* elaborate the lval. This detects any part selects and mux
	expressions that might exist. */
	NetAssign_*lv = elaborate_lval(des, scope);
	if (lv == 0) return 0;

	/* If there is an internal delay expression, elaborate it. */
	NetExpr*delay = 0;
	if (delay_ != 0)
		delay = elaborate_delay_expr(delay_, des, scope);


	/* Elaborate the r-value expression, then try to evaluate it. */

	assert(rval());
	NetExpr*rv = elab_and_eval(des, scope, rval());
	if (rv == 0) return 0;
	assert(rv);


	/* Rewrite delayed assignments as assignments that are
	delayed. For example, a = #<d> b; becomes:

	begin
	tmp = b;
	#<d> a = tmp;
	end

	If the delay is an event delay, then the transform is
	similar, with the event delay replacing the time delay. It
	is an event delay if the event_ member has a value.

	This rewriting of the expression allows me to not bother to
	actually and literally represent the delayed assign in the
	netlist. The compound statement is exactly equivalent. */

	if (delay || event_) {
		unsigned wid = lv->lwidth();

		rv->set_width(wid);
		rv = pad_to_width(rv, wid);

		if (wid > rv->expr_width()) {
			cerr << get_line() << ": error: Unable to match "
				"expression width of " << rv->expr_width() <<
				" to l-value width of " << wid << "." << endl;
			//XXXX delete rv;
			return 0;
		}

		NetNet*tmp = new NetNet(scope, scope->local_symbol(),
			NetNet::REG, wid);
		tmp->set_line(*this);

		NetESignal*sig = new NetESignal(tmp);
		sig->set_line(*this);

		/* Generate an assignment of the l-value to the temporary... */
		string n = scope->local_hsymbol();
		NetAssign_*lvt = new NetAssign_(tmp);

		NetAssign*a1 = new NetAssign(lvt, rv);
		a1->set_line(*this);

		/* Generate an assignment of the temporary to the r-value... */
		NetAssign*a2 = new NetAssign(lv, sig);
		a2->set_line(*this);

		/* Generate the delay statement with the final
		assignment attached to it. If this is an event delay,
		elaborate the PEventStatement. Otherwise, create the
		right NetPDelay object. */
		NetProc*st;
		if (event_) {
			st = event_->elaborate_st(des, scope, a2);
			if (st == 0) {
				cerr << event_->get_line() << ": error: "
					"unable to elaborate event expression."
					<< endl;
				des->errors += 1;
				return 0;
			}
			assert(st);

		} else {
			NetPDelay*de = new NetPDelay(delay, a2);
			de->set_line(*this);
			st = de;
		}

		/* And build up the complex statement. */
		NetBlock*bl = new NetBlock(NetBlock::SEQU, 0);
		bl->set_line(*this);
		bl->append(a1);
		bl->append(st);

		return bl;
	}

	/* Based on the specific type of the l-value, do cleanup
	processing on the r-value. */
	if (lv->var()) {

	} else if (rv->expr_type() == NetExpr::ET_REAL) {

		// The r-value is a real. Casting will happen in the
		// code generator, so leave it.

	} else {
		unsigned wid = count_lval_width(lv);
		rv->set_width(wid);
		rv = pad_to_width(rv, wid);
		assert(rv->expr_width() >= wid);
	}

	NetAssign*cur = new NetAssign(lv, rv);
	cur->set_line(*this);

	return cur;
}

/*
* Elaborate non-blocking assignments. The statement is of the general
* form:
*
*    <lval> <= #<delay> <rval> ;
*/
NetProc* PAssignNB::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	/* Elaborate the l-value. */
	NetAssign_*lv = elaborate_lval(des, scope);
	if (lv == 0) return 0;

	assert(rval());

	/* Elaborate and precalculate the r-value. */
	NetExpr*rv = elab_and_eval(des, scope, rval());
	if (rv == 0)
		return 0;

	/* Handle the (common) case that the r-value is a vector. This
	includes just about everything but reals. In this case, we
	need to pad the r-value to match the width of the l-value.

	If in this case the l-val is a variable (i.e. real) then
	the width to pad to will be 0, so this code is harmless. */
	if (rv->expr_type() == NetExpr::ET_VECTOR) {
		unsigned wid = count_lval_width(lv);
		rv->set_width(wid);
		rv = pad_to_width(rv, wid);
	}

	NetExpr*delay = 0;
	if (delay_ != 0)
		delay = elaborate_delay_expr(delay_, des, scope);

	/* All done with this node. Mark its line number and check it in. */
	NetAssignNB*cur = new NetAssignNB(lv, rv);
	cur->set_delay(delay);
	cur->set_line(*this);
	return cur;
}


/*
* This is the elaboration method for a begin-end block. Try to
* elaborate the entire block, even if it fails somewhere. This way I
* get all the error messages out of it. Then, if I detected a failure
* then pass the failure up.
*/
NetProc* PBlock::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetBlock::Type type = (bl_type_==PBlock::BL_PAR)
		? NetBlock::PARA
		: NetBlock::SEQU;

	NetScope*nscope = 0;
	if (name_.str() != 0) {
		nscope = scope->child(name_);
		if (nscope == 0) {
			cerr << get_line() << ": internal error: "
				"unable to find block scope " << scope->name()
				<< "<" << name_ << ">" << endl;
			des->errors += 1;
			return 0;
		}

		assert(nscope);

	}

	NetBlock*cur = new NetBlock(type, nscope);
	cur->set_line(*this);
	bool fail_flag = false;

	if (nscope == 0)
		nscope = scope;

	// Handle the special case that the block contains only one
	// statement. There is no need to keep the block node. Also,
	// don't elide named blocks, because they might be referenced
	// elsewhere.
	if ((list_.count() == 1) && (name_.str() == 0)) {
		assert(list_[0]);
		NetProc*tmp = list_[0]->elaborate(des, nscope);
		return tmp;
	}

	for (unsigned idx = 0 ;  idx < list_.count() ;  idx += 1) {
		assert(list_[idx]);
		NetProc*tmp = list_[idx]->elaborate(des, nscope);
		if (tmp == 0) {
			fail_flag = true;
			continue;
		}

		// If the result turns out to be a noop, then skip it.
		if (NetBlock*tbl = dynamic_cast<NetBlock*>(tmp))
			if (tbl->proc_first() == 0) {
				delete tbl;
				continue;
			}

			cur->append(tmp);
	}

	if (fail_flag) {
		delete cur;
		cur = 0;
	}

	return cur;
}

/*
* Elaborate a case statement.
*/
NetProc* PCase::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetExpr*expr = elab_and_eval(des, scope, expr_);
	if (expr == 0) {
		cerr << get_line() << ": error: Unable to elaborate this case"
			" expression." << endl;
		return 0;
	}

	/* Count the items in the case statement. Note that there may
	be some cases that have multiple guards. Count each as a
	separate item. */
	unsigned icount = 0;
	for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1) {
		PCase::Item*cur = (*items_)[idx];

		if (cur->expr.count() == 0)
			icount += 1;
		else
			icount += cur->expr.count();
	}

	NetCase*res = new NetCase(type_, expr, icount);
	res->set_line(*this);
	process_attributes(des, scope, res, attributes);

	/* Iterate over all the case items (guard/statement pairs)
	elaborating them. If the guard has no expression, then this
	is a "default" cause. Otherwise, the guard has one or more
	expressions, and each guard is a case. */
	unsigned inum = 0;
	for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1) {

		assert(inum < icount);
		PCase::Item*cur = (*items_)[idx];

		if (cur->expr.count() == 0) {
			/* If there are no expressions, then this is the
			default case. */
			NetProc*st = 0;
			if (cur->stat) {
				st = cur->stat->elaborate(des, scope);
			} else {
				st = new NetBlock(NetBlock::SEQU, scope);
				st->set_line(*this);
			}

			res->set_case(inum, 0, st);
			inum += 1;

		} else for (unsigned e = 0; e < cur->expr.count(); e += 1) {

			/* If there are one or more expressions, then
			iterate over the guard expressions, elaborating
			a separate case for each. If the statement is
			nul, then put an appropriate stub in place. */
			NetExpr*gu = 0;
			NetProc*st = 0;
			assert(cur->expr[e]);
			gu = elab_and_eval(des, scope, cur->expr[e]);

			if (cur->stat) {
				st = cur->stat->elaborate(des, scope);
			} else {
				st = new NetBlock(NetBlock::SEQU, scope);
				st->set_line(*this);
			}
			res->set_case(inum, gu, st);
			inum += 1;
		}
	}

	return res;
}

NetProc* PCondit::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	// Elaborate and try to evaluate the conditional expression.
	NetExpr*expr = elab_and_eval(des, scope, expr_);
	if (expr == 0) {
		cerr << get_line() << ": error: Unable to elaborate"
			" condition expression." << endl;
		des->errors += 1;
		return 0;
	}

	// If the condition of the conditional statement is constant,
	// then look at the value and elaborate either the if statement
	// or the else statement. I don't need both. If there is no
	// else_ statement, the use an empty block as a noop.
	if (NetEConst*ce = dynamic_cast<NetEConst*>(expr)) {
		verinum val = ce->value();
		delete expr;

		verinum::V val_reduced = verinum::V0;
		for (unsigned idx = 0 ;  idx < val.len() ;  idx += 1)
			val_reduced = val_reduced | val[idx];

		if (val_reduced == verinum::V1)
			if (if_) {
				return if_->elaborate(des, scope);
			} else {
				NetBlock*tmp = new NetBlock(NetBlock::SEQU, 0);
				tmp->set_line(*this);
				return tmp;
			}
		else if (else_)
			return else_->elaborate(des, scope);
		else {
			NetBlock*tmp = new NetBlock(NetBlock::SEQU, 0);
			tmp->set_line(*this);
			return tmp;
		}
	}

	// If the condition expression is more then 1 bits, then
	// generate a comparison operator to get the result down to
	// one bit. Turn <e> into <e> != 0;

	if (expr->expr_width() < 1) {
		cerr << get_line() << ": internal error: "
			"incomprehensible expression width (0)." << endl;
		return 0;
	}

	if (expr->expr_width() > 1) {
		assert(expr->expr_width() > 1);
		verinum zero (verinum::V0, expr->expr_width());
		NetEConst*ezero = new NetEConst(zero);
		ezero->set_width(expr->expr_width());
		NetEBComp*cmp = new NetEBComp('n', expr, ezero);
		expr = cmp;
	}

	// Well, I actually need to generate code to handle the
	// conditional, so elaborate.
	NetProc*i = if_? if_->elaborate(des, scope) : 0;
	NetProc*e = else_? else_->elaborate(des, scope) : 0;

	// Detect the special cases that the if or else statements are
	// empty blocks. If this is the case, remove the blocks as
	// null statements.
	if (NetBlock*tmp = dynamic_cast<NetBlock*>(i)) {
		if (tmp->proc_first() == 0) {
			delete i;
			i = 0;
		}
	}

	if (NetBlock*tmp = dynamic_cast<NetBlock*>(e)) {
		if (tmp->proc_first() == 0) {
			delete e;
			e = 0;
		}
	}

	NetCondit*res = new NetCondit(expr, i, e);
	res->set_line(*this);
	return res;
}

NetProc* PCallTask::elaborate(Design*des, NetScope*scope) const
{
	if (path_.peek_name(0)[0] == '$')
		return elaborate_sys(des, scope);
	else
		return elaborate_usr(des, scope);
}

/*
* A call to a system task involves elaborating all the parameters,
* then passing the list to the NetSTask object.
*XXXX
* There is a single special case in the call to a system
* task. Normally, an expression cannot take an unindexed
* memory. However, it is possible to take a system task parameter a
* memory if the expression is trivial.
*/
NetProc* PCallTask::elaborate_sys(Design*des, NetScope*scope) const
{
	assert(scope);

	unsigned parm_count = nparms();

	/* Catch the special case that the system task has no
	parameters. The "()" string will be parsed as a single
	empty parameter, when we really mean no parameters at all. */
	if ((nparms() == 1) && (parm(0) == 0))
		parm_count = 0;

	svector<NetExpr*>eparms (parm_count);

	for (unsigned idx = 0 ;  idx < parm_count ;  idx += 1) {
		PExpr*ex = parm(idx);
		eparms[idx] = ex? ex->elaborate_expr(des, scope, true) : 0;

		/* Attempt to pre-evaluate the parameters. It may be
		possible to at least partially reduce the
		expression. */
		if (eparms[idx] && !dynamic_cast<NetEConst*>(eparms[idx])) {
			NetExpr*tmp = eparms[idx]->eval_tree();
			if (tmp != 0) {
				delete eparms[idx];
				eparms[idx] = tmp;
			}
		}
	}

	NetSTask*cur = new NetSTask(path_.peek_name(0), eparms);
	return cur;
}

/*
* A call to a user defined task is different from a call to a system
* task because a user task in a netlist has no parameters: the
* assignments are done by the calling thread. For example:
*
*  task foo;
*    input a;
*    output b;
*    [...]
*  endtask;
*
*  [...] foo(x, y);
*
* is really:
*
*  task foo;
*    reg a;
*    reg b;
*    [...]
*  endtask;
*
*  [...]
*  begin
*    a = x;
*    foo;
*    y = b;
*  end
*/
NetProc* PCallTask::elaborate_usr(Design*des, NetScope*scope) const
{
	assert(scope);

	NetScope*task = des->find_task(scope, path_);
	if (task == 0) {
		cerr << get_line() << ": error: Enable of unknown task "
			<< "``" << path_ << "''." << endl;
		des->errors += 1;
		return 0;
	}

	assert(task);
	assert(task->type() == NetScope::TASK);
	NetTaskDef*def = task->task_def();
	if (def == 0) {
		cerr << get_line() << ": internal error: task " << path_
			<< " doesn't have a definition in " << scope->name()
			<< "." << endl;
		des->errors += 1;
		return 0;
	}
	assert(def);

	if (nparms() != def->port_count()) {
		cerr << get_line() << ": error: Port count mismatch in call to ``"
			<< path_ << "''." << endl;
		des->errors += 1;
		return 0;
	}

	NetUTask*cur;

	/* Handle tasks with no parameters specially. There is no need
	to make a sequential block to hold the generated code. */
	if (nparms() == 0) {
		cur = new NetUTask(task);
		return cur;
	}

	NetBlock*block = new NetBlock(NetBlock::SEQU, 0);
	block->set_line(*this);


	/* Detect the case where the definition of the task is known
	empty. In this case, we need not bother with calls to the
	task, all the assignments, etc. Just return a no-op. */

	if (const NetBlock*tp = dynamic_cast<const NetBlock*>(def->proc())) {
		if (tp->proc_first() == 0)
			return block;
	}

	/* Generate assignment statement statements for the input and
	INOUT ports of the task. These are managed by writing
	assignments with the task port the l-value and the passed
	expression the r-value. We know by definition that the port
	is a reg type, so this elaboration is pretty obvious. */

	for (unsigned idx = 0 ;  idx < nparms() ;  idx += 1) {

		NetNet*port = def->port(idx);
		assert(port->port_type() != NetNet::NOT_A_PORT);
		if (port->port_type() == NetNet::POUTPUT)
			continue;

		NetExpr*rv = elab_and_eval(des, scope, parms_[idx]);
		NetAssign_*lv = new NetAssign_(port);
		NetAssign*pr = new NetAssign(lv, rv);
		block->append(pr);
	}

	/* Generate the task call proper... */
	cur = new NetUTask(task);
	block->append(cur);


	/* Generate assignment statements for the output and INOUT
	ports of the task. The l-value in this case is the
	expression passed as a parameter, and the r-value is the
	port to be copied out.

	We know by definition that the r-value of this copy-out is
	the port, which is a reg. The l-value, however, may be any
	expression that can be a target to a procedural
	assignment, including a memory word. */

	for (unsigned idx = 0 ;  idx < nparms() ;  idx += 1) {

		NetNet*port = def->port(idx);

		/* Skip input ports. */
		assert(port->port_type() != NetNet::NOT_A_PORT);
		if (port->port_type() == NetNet::PINPUT)
			continue;


		/* Elaborate an l-value version of the port expression
		for output and inout ports. If the expression does
		not exist then quietly skip it, but if the expression
		is not a valid l-value print an error message. Note
		that the elaborate_lval method already printed a
		detailed message. */
		NetAssign_*lv;
		if (parms_[idx]) {
			lv = parms_[idx]->elaborate_lval(des, scope);
			if (lv == 0) {
				cerr << parms_[idx]->get_line() << ": error: "
					<< "I give up on task port " << (idx+1)
					<< " expression: " << *parms_[idx] << endl;
			}
		} else {
			lv = 0;
		}

		if (lv == 0)
			continue;

		NetESignal*sig = new NetESignal(port);
		sig->set_line(*this);

		/* Generate the assignment statement. */
		NetAssign*ass = new NetAssign(lv, sig);

		block->append(ass);
	}

	return block;
}

NetCAssign* PCAssign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*lval = lval_->elaborate_anet(des, scope);
	if (lval == 0)
		return 0;

	NetNet*rval = expr_->elaborate_net(des, scope, lval->pin_count(),
		0, 0, 0);
	if (rval == 0)
		return 0;

	if (rval->pin_count() < lval->pin_count())
		rval = pad_to_width(des, rval, lval->pin_count());

	NetCAssign* dev = new NetCAssign(scope, scope->local_symbol(), lval);
	dev->set_line(*this);
	des->add_node(dev);

	for (unsigned idx = 0 ;  idx < dev->pin_count() ;  idx += 1)
		connect(dev->pin(idx), rval->pin(idx));

	return dev;
}

NetDeassign* PDeassign::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*lval = lval_->elaborate_anet(des, scope);
	if (lval == 0)
		return 0;

	NetDeassign*dev = new NetDeassign(lval);
	dev->set_line( *this );
	return dev;
}

/*
* Elaborate the delay statement (of the form #<expr> <statement>) as a
* NetPDelay object. If the expression is constant, evaluate it now
* and make a constant delay. If not, then pass an elaborated
* expression to the constructor of NetPDelay so that the code
* generator knows to evaluate the expression at run time.
*/
NetProc* PDelayStatement::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	/* This call evaluates the delay expression to a NetEConst, if
	possible. This includes transforming NetECReal values to
	integers, and applying the proper scaling. */
	NetExpr*dex = elaborate_delay_expr(delay_, des, scope);

	if (NetEConst*tmp = dynamic_cast<NetEConst*>(dex)) {
		if (statement_)
			return new NetPDelay(tmp->value().as_ulong(),
			statement_->elaborate(des, scope));
		else
			return new NetPDelay(tmp->value().as_ulong(), 0);

		delete dex;

	} else {
		if (statement_)
			return new NetPDelay(dex, statement_->elaborate(des, scope));
		else
			return new NetPDelay(dex, 0);
	}

}

/*
* The disable statement is not yet supported.
*/
NetProc* PDisable::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetScope*target = des->find_scope(scope, scope_);
	if (target == 0) {
		cerr << get_line() << ": error: Cannot find scope "
			<< scope_ << " in " << scope->name() << endl;
		des->errors += 1;
		return 0;
	}

	switch (target->type()) {
	case NetScope::FUNC:
		cerr << get_line() << ": error: Cannot disable functions." << endl;
		des->errors += 1;
		return 0;

	case NetScope::MODULE:
		cerr << get_line() << ": error: Cannot disable modules." << endl;
		des->errors += 1;
		return 0;

	default:
		break;
	}

	NetDisable*obj = new NetDisable(target);
	obj->set_line(*this);
	return obj;
}

/*
* An event statement is an event delay of some sort, attached to a
* statement. Some Verilog examples are:
*
*      @(posedge CLK) $display("clock rise");
*      @event_1 $display("event triggered.");
*      @(data or negedge clk) $display("data or clock fall.");
*
* The elaborated netlist uses the NetEvent, NetEvWait and NetEvProbe
* classes. The NetEvWait class represents the part of the netlist
* that is executed by behavioral code. The process starts waiting on
* the NetEvent when it executes the NetEvWait step. Net NetEvProbe
* and NetEvTrig are structural and behavioral equivalents that
* trigger the event, and awakens any processes blocking in the
* associated wait.
*
* The basic data structure is:
*
*       NetEvWait ---/--->  NetEvent  <----\---- NetEvProbe
*        ...         |                     |         ...
*       NetEvWait ---+                     +---- NetEvProbe
*                                          |         ...
*                                          +---- NetEvTrig
*
* That is, many NetEvWait statements may wait on a single NetEvent
* object, and Many NetEvProbe objects may trigger the NetEvent
* object. The many NetEvWait objects pointing to the NetEvent object
* reflects the possibility of different places in the code blocking
* on the same named event, like so:
*
*         event foo;
*           [...]
*         always begin @foo <statement1>; @foo <statement2> end
*
* This tends to not happen with signal edges. The multiple probes
* pointing to the same event reflect the possibility of many
* expressions in the same blocking statement, like so:
*
*         wire reset, clk;
*           [...]
*         always @(reset or posedge clk) <stmt>;
*
* Conjunctions like this cause a NetEvent object be created to
* represent the overall conjunction, and NetEvProbe objects for each
* event expression.
*
* If the NetEvent object represents a named event from the source,
* then there are NetEvTrig objects that represent the trigger
* statements instead of the NetEvProbe objects representing signals.
* For example:
*
*         event foo;
*         always @foo <stmt>;
*         initial begin
*                [...]
*            -> foo;
*                [...]
*            -> foo;
*                [...]
*         end
*
* Each trigger statement in the source generates a separate NetEvTrig
* object in the netlist. Those trigger objects are elaborated
* elsewhere.
*
* Additional complications arise when named events show up in
* conjunctions. An example of such a case is:
*
*         event foo;
*         wire bar;
*         always @(foo or posedge bar) <stmt>;
*
* Since there is by definition a NetEvent object for the foo object,
* this is handled by allowing the NetEvWait object to point to
* multiple NetEvent objects. All the NetEvProbe based objects are
* collected and pointed as the synthetic NetEvent object, and all the
* named events are added into the list of NetEvent object that the
* NetEvWait object can refer to.
*/

NetProc* PEventStatement::elaborate_st(Design*des, NetScope*scope,
	NetProc*enet) const
{
	assert(scope);

	/* Create a single NetEvent and NetEvWait. Then, create a
	NetEvProbe for each conjunctive event in the event
	list. The NetEvProbe objects all refer back to the NetEvent
	object. */

	NetEvent*ev = new NetEvent(scope->local_symbol());
	ev->set_line(*this);
	unsigned expr_count = 0;

	NetEvWait*wa = new NetEvWait(enet);
	wa->set_line(*this);

	/* If there are no expressions, this is a signal that it is an
	@* statement. Generate an expression to use. */

	if (expr_.count() == 0) {
		assert(enet);
		/* For synthesis we want just the inputs, but for the rest we
		* want inputs and outputs that may cause a value to change. */
		extern bool synthesis; /* Synthesis flag from main.cc */
		bool rem_out = false;
		if (synthesis) rem_out = true;
		NexusSet*nset = enet->nex_input(rem_out);
		if (nset == 0) {
			cerr << get_line() << ": internal error: No NexusSet"
				<< " from statement." << endl;
			enet->dump(cerr, 6);
			des->errors += 1;
			return enet;
		}

		if (nset->count() == 0) {
			cerr << get_line() << ": warning: No inputs to statement."
				<< " Ignoring @*." << endl;
			return enet;
		}

		NetEvProbe*pr = new NetEvProbe(scope, scope->local_symbol(),
			ev, NetEvProbe::ANYEDGE,
			nset->count());
		for (unsigned idx = 0 ;  idx < nset->count() ;  idx += 1)
			connect(nset[0][idx], pr->pin(idx));

		delete nset;
		des->add_node(pr);

		expr_count = 1;

	} else for (unsigned idx = 0 ;  idx < expr_.count() ;  idx += 1) {

		assert(expr_[idx]->expr());

		/* If the expression is an identifier that matches a
		named event, then handle this case all at once at
		skip the rest of the expression handling. */

		if (PEIdent*id = dynamic_cast<PEIdent*>(expr_[idx]->expr())) {
			NetNet*       sig = 0;
			NetMemory*    mem = 0;
			NetVariable*  var = 0;
			const NetExpr*par = 0;
			NetEvent*     eve = 0;

			NetScope*found_in = symbol_search(des, scope, id->path(),
				sig, mem, var, par, eve);

			if (found_in && eve) {
				wa->add_event(eve);
				continue;
			}
		}


		/* So now we have a normal event expression. Elaborate
		the sub-expression as a net and decide how to handle
		the edge. */

		bool save_flag = error_implicit;
		error_implicit = true;
		NetNet*expr = expr_[idx]->expr()->elaborate_net(des, scope,
			0, 0, 0, 0);
		error_implicit = save_flag;
		if (expr == 0) {
			expr_[idx]->dump(cerr);
			cerr << endl;
			des->errors += 1;
			continue;
		}
		assert(expr);

		unsigned pins = (expr_[idx]->type() == PEEvent::ANYEDGE)
			? expr->pin_count() : 1;

		NetEvProbe*pr;
		switch (expr_[idx]->type()) {
		case PEEvent::POSEDGE:
			pr = new NetEvProbe(scope, scope->local_symbol(), ev,
				NetEvProbe::POSEDGE, pins);
			break;

		case PEEvent::NEGEDGE:
			pr = new NetEvProbe(scope, scope->local_symbol(), ev,
				NetEvProbe::NEGEDGE, pins);
			break;

		case PEEvent::ANYEDGE:
			pr = new NetEvProbe(scope, scope->local_symbol(), ev,
				NetEvProbe::ANYEDGE, pins);
			break;

		default:
			assert(0);
		}

		for (unsigned p = 0 ;  p < pr->pin_count() ; p += 1)
			connect(pr->pin(p), expr->pin(p));

		des->add_node(pr);
		expr_count += 1;
	}

	/* If there was at least one conjunction that was an
	expression (and not a named event) then add this
	event. Otherwise, we didn't use it so delete it. */
	if (expr_count > 0) {
		scope->add_event(ev);
		wa->add_event(ev);
		/* NOTE: This event that I am adding to the wait may be
		a duplicate of another event somewhere else. However,
		I don't know that until all the modules are hooked
		up, so it is best to leave find_similar_event to
		after elaboration. */
	} else {
		delete ev;
	}

	return wa;
}

/*
* This is the special case of the event statement, the wait
* statement. This is elaborated into a slightly more complicated
* statement that uses non-wait statements:
*
*     wait (<expr>)  <statement>
*
* becomes
*
*     begin
*         while (1 !== <expr>)
*           @(<expr inputs>) <noop>;
*         <statement>;
*     end
*/
NetProc* PEventStatement::elaborate_wait(Design*des, NetScope*scope,
	NetProc*enet) const
{
	assert(scope);
	assert(expr_.count() == 1);

	const PExpr *pe = expr_[0]->expr();

	/* Elaborate wait expression. Don't eval yet, we will do that
	shortly, after we apply a reduction or. */
	NetExpr*expr = pe->elaborate_expr(des, scope);
	if (expr == 0) {
		cerr << get_line() << ": error: Unable to elaborate"
			" wait condition expression." << endl;
		des->errors += 1;
		return 0;
	}

	// If the condition expression is more then 1 bits, then
	// generate a reduction operator to get the result down to
	// one bit. In other words, Turn <e> into |<e>;

	if (expr->expr_width() < 1) {
		cerr << get_line() << ": internal error: "
			"incomprehensible wait expression width (0)." << endl;
		return 0;
	}

	if (expr->expr_width() > 1) {
		assert(expr->expr_width() > 1);
		NetEUReduce*cmp = new NetEUReduce('|', expr);
		expr = cmp;
	}

	/* precalculate as much as possible of the wait expression. */
	if (NetExpr*tmp = expr->eval_tree()) {
		delete expr;
		expr = tmp;
	}

	/* Detect the unusual case that the wait expression is
	constant. Constant true is OK (it becomes transparent) but
	constant false is almost certainly not what is intended. */
	assert(expr->expr_width() == 1);
	if (NetEConst*ce = dynamic_cast<NetEConst*>(expr)) {
		verinum val = ce->value();
		assert(val.len() == 1);

		/* Constant true -- wait(1) <s1> reduces to <s1>. */
		if (val[0] == verinum::V1) {
			delete expr;
			assert(enet);
			return enet;
		}

		/* Otherwise, false. wait(0) blocks permanently. */

		cerr << get_line() << ": warning: wait expression is "
			<< "constant false." << endl;
		cerr << get_line() << ":        : The statement will "
			<< "block permanently." << endl;

		/* Create an event wait and an otherwise unreferenced
		event variable to force a perpetual wait. */
		NetEvent*wait_event = new NetEvent(scope->local_symbol());
		scope->add_event(wait_event);

		NetEvWait*wait = new NetEvWait(0);
		wait->add_event(wait_event);
		wait->set_line(*this);

		delete expr;
		delete enet;
		return wait;
	}

	/* Invert the sense of the test with an exclusive NOR. In
	other words, if this adjusted expression returns TRUE, then
	wait. */
	assert(expr->expr_width() == 1);
	expr = new NetEBComp('N', expr, new NetEConst(verinum(verinum::V1)));
	NetExpr*tmp = expr->eval_tree();
	if (tmp) {
		delete expr;
		expr = tmp;
	}

	NetEvent*wait_event = new NetEvent(scope->local_symbol());
	scope->add_event(wait_event);

	NetEvWait*wait = new NetEvWait(0 /* noop */);
	wait->add_event(wait_event);
	wait->set_line(*this);

	NexusSet*wait_set = expr->nex_input();
	if (wait_set == 0) {
		cerr << get_line() << ": internal error: No NexusSet"
			<< " from wait expression." << endl;
		des->errors += 1;
		return 0;
	}

	if (wait_set->count() == 0) {
		cerr << get_line() << ": internal error: Empty NexusSet"
			<< " from wait expression." << endl;
		des->errors += 1;
		return 0;
	}

	NetEvProbe*wait_pr = new NetEvProbe(scope, scope->local_symbol(),
		wait_event, NetEvProbe::ANYEDGE,
		wait_set->count());
	for (unsigned idx = 0; idx < wait_set->count() ;  idx += 1)
		connect(wait_set[0][idx], wait_pr->pin(idx));

	delete wait_set;
	des->add_node(wait_pr);

	NetWhile*loop = new NetWhile(expr, wait);
	loop->set_line(*this);

	/* If there is no real substatement (i.e., "wait (foo) ;") then
	we are done. */
	if (enet == 0)
		return loop;

	/* Create a sequential block to combine the wait loop and the
	delayed statement. */
	NetBlock*block = new NetBlock(NetBlock::SEQU, 0);
	block->append(loop);
	block->append(enet);
	block->set_line(*this);

	return block;
}


NetProc* PEventStatement::elaborate(Design*des, NetScope*scope) const
{
	NetProc*enet = 0;
	if (statement_) {
		enet = statement_->elaborate(des, scope);
		if (enet == 0)
			return 0;

	} else {
		enet = new NetBlock(NetBlock::SEQU, 0);
		enet->set_line(*this);
	}

	if ((expr_.count() == 1) && (expr_[0]->type() == PEEvent::POSITIVE))
		return elaborate_wait(des, scope, enet);

	return elaborate_st(des, scope, enet);
}

/*
* Forever statements are represented directly in the netlist. It is
* theoretically possible to use a while structure with a constant
* expression to represent the loop, but why complicate the code
* generators so?
*/
NetProc* PForever::elaborate(Design*des, NetScope*scope) const
{
	NetProc*stat = statement_->elaborate(des, scope);
	if (stat == 0) return 0;

	NetForever*proc = new NetForever(stat);
	return proc;
}

NetProc* PForce::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*lval = lval_->elaborate_net(des, scope, 0, 0, 0, 0);
	if (lval == 0)
		return 0;

	NetNet*rval = expr_->elaborate_net(des, scope, lval->pin_count(),
		0, 0, 0);
	if (rval == 0)
		return 0;

	if (rval->pin_count() < lval->pin_count())
		rval = pad_to_width(des, rval, lval->pin_count());

	NetForce* dev = new NetForce(scope, scope->local_symbol(), lval);
	des->add_node(dev);

	for (unsigned idx = 0 ;  idx < dev->pin_count() ;  idx += 1)
		connect(dev->pin(idx), rval->pin(idx));

	return dev;
}

/*
* elaborate the for loop as the equivalent while loop. This eases the
* task for the target code generator. The structure is:
*
*     begin : top
*       name1_ = expr1_;
*       while (cond_) begin : body
*          statement_;
*          name2_ = expr2_;
*       end
*     end
*/
NetProc* PForStatement::elaborate(Design*des, NetScope*scope) const
{
	NetExpr*etmp;
	assert(scope);

	const PEIdent*id1 = dynamic_cast<const PEIdent*>(name1_);
	assert(id1);
	const PEIdent*id2 = dynamic_cast<const PEIdent*>(name2_);
	assert(id2);

	NetBlock*top = new NetBlock(NetBlock::SEQU, 0);
	top->set_line(*this);

	/* make the expression, and later the initial assignment to
	the condition variable. The statement in the for loop is
	very specifically an assignment. */
	NetNet*sig = des->find_signal(scope, id1->path());
	if (sig == 0) {
		cerr << id1->get_line() << ": register ``" << id1->path()
			<< "'' unknown in this context." << endl;
		des->errors += 1;
		return 0;
	}
	assert(sig);
	NetAssign_*lv = new NetAssign_(sig);

	/* Make the r-value of the initial assignment, and size it
	properly. Then use it to build the assignment statement. */
	etmp = expr1_->elaborate_expr(des, scope);
	etmp->set_width(lv->lwidth());

	NetAssign*init = new NetAssign(lv, etmp);
	init->set_line(*this);

	top->append(init);

	NetBlock*body = new NetBlock(NetBlock::SEQU, 0);
	body->set_line(*this);

	/* Elaborate the statement that is contained in the for
	loop. If there is an error, this will return 0 and I should
	skip the append. No need to worry, the error has been
	reported so it's OK that the netlist is bogus. */
	NetProc*tmp = statement_->elaborate(des, scope);
	if (tmp)
		body->append(tmp);


	/* Elaborate the increment assignment statement at the end of
	the for loop. This is also a very specific assignment
	statement. Put this into the "body" block. */
	sig = des->find_signal(scope, id2->path());
	if (sig == 0) {
		cerr << get_line() << ": error: Unable to find variable "
			<< id2->path() << " in for-loop increment expression." << endl;
		des->errors += 1;
		return body;
	}

	assert(sig);
	lv = new NetAssign_(sig);

	/* Make the rvalue of the increment expression, and size it
	for the lvalue. */
	etmp = expr2_->elaborate_expr(des, scope);
	etmp->set_width(lv->lwidth());
	NetAssign*step = new NetAssign(lv, etmp);
	step->set_line(*this);

	body->append(step);


	/* Elaborate the condition expression. Try to evaluate it too,
	in case it is a constant. This is an interesting case
	worthy of a warning. */
	NetExpr*ce = elab_and_eval(des, scope, cond_);
	if (ce == 0) {
		delete top;
		return 0;
	}

	if (dynamic_cast<NetEConst*>(ce)) {
		cerr << get_line() << ": warning: condition expression "
			"of for-loop is constant." << endl;
	}


	/* All done, build up the loop. */

	NetWhile*loop = new NetWhile(ce, body);
	loop->set_line(*this);
	top->append(loop);
	return top;
}

/*
* (See the PTask::elaborate methods for basic common stuff.)
*
* The return value of a function is represented as a reg variable
* within the scope of the function that has the name of the
* function. So for example with the function:
*
*    function [7:0] incr;
*      input [7:0] in1;
*      incr = in1 + 1;
*    endfunction
*
* The scope of the function is <parent>.incr and there is a reg
* variable <parent>.incr.incr. The elaborate_1 method is called with
* the scope of the function, so the return reg is easily located.
*
* The function parameters are all inputs, except for the synthetic
* output parameter that is the return value. The return value goes
* into port 0, and the parameters are all the remaining ports.
*/

void PFunction::elaborate(Design*des, NetScope*scope) const
{
	NetFuncDef*def = scope->func_def();
	if (def == 0) {
		cerr << get_line() << ": internal error: "
			<< "No function definition for function "
			<< scope->name() << endl;
		return;
	}

	assert(def);

	NetProc*st = statement_->elaborate(des, scope);
	if (st == 0) {
		cerr << statement_->get_line() << ": error: Unable to elaborate "
			"statement in function " << def->name() << "." << endl;
		des->errors += 1;
		return;
	}

	def->set_proc(st);
}

NetProc* PRelease::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*lval = lval_->elaborate_net(des, scope, 0, 0, 0, 0);
	if (lval == 0)
		return 0;

	NetRelease*dev = new NetRelease(lval);
	dev->set_line( *this );
	return dev;
}

NetProc* PRepeat::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetExpr*expr = elab_and_eval(des, scope, expr_);
	if (expr == 0) {
		cerr << get_line() << ": Unable to elaborate"
			" repeat expression." << endl;
		des->errors += 1;
		return 0;
	}

	NetProc*stat = statement_->elaborate(des, scope);
	if (stat == 0) return 0;

	// If the expression is a constant, handle certain special
	// iteration counts.
	if (NetEConst*ce = dynamic_cast<NetEConst*>(expr)) {
		verinum val = ce->value();
		switch (val.as_ulong()) {
		case 0:
			delete expr;
			delete stat;
			return new NetBlock(NetBlock::SEQU, 0);
		case 1:
			delete expr;
			return stat;
		default:
			break;
		}
	}

	NetRepeat*proc = new NetRepeat(expr, stat);
	return proc;
}

/*
* A task definition is elaborated by elaborating the statement that
* it contains, and connecting its ports to NetNet objects. The
* netlist doesn't really need the array of parameters once elaboration
* is complete, but this is the best place to store them.
*
* The first elaboration pass finds the reg objects that match the
* port names, and creates the NetTaskDef object. The port names are
* in the form task.port.
*
*      task foo;
*        output blah;
*        begin <body> end
*      endtask
*
* So in the foo example, the PWire objects that represent the ports
* of the task will include a foo.blah for the blah port. This port is
* bound to a NetNet object by looking up the name. All of this is
* handled by the PTask::elaborate_sig method and the results stashed
* in the created NetTaskDef attached to the scope.
*
* Elaboration pass 2 for the task definition causes the statement of
* the task to be elaborated and attached to the NetTaskDef object
* created in pass 1.
*
* NOTE: I am not sure why I bothered to prepend the task name to the
* port name when making the port list. It is not really useful, but
* that is what I did in pform_make_task_ports, so there it is.
*/

void PTask::elaborate(Design*des, NetScope*task) const
{
	NetTaskDef*def = task->task_def();
	assert(def);

	NetProc*st;
	if (statement_ == 0) {
		st = new NetBlock(NetBlock::SEQU, 0);
		st->set_line(*this);

	} else {

		st = statement_->elaborate(des, task);
		if (st == 0) {
			cerr << statement_->get_line() << ": Unable to elaborate "
				"statement in task " << task->name()
				<< " at " << get_line() << "." << endl;
			return;
		}
	}

	def->set_proc(st);
}

NetProc* PTrigger::elaborate(Design*des, NetScope*scope) const
{
	assert(scope);

	NetNet*       sig = 0;
	NetMemory*    mem = 0;
	NetVariable*  var = 0;
	const NetExpr*par = 0;
	NetEvent*     eve = 0;

	NetScope*found_in = symbol_search(des, scope, event_,
		sig, mem, var, par, eve);

	if (found_in == 0) {
		cerr << get_line() << ": error: event <" << event_ << ">"
			<< " not found." << endl;
		des->errors += 1;
		return 0;
	}

	if (eve == 0) {
		cerr << get_line() << ": error:  <" << event_ << ">"
			<< " is not a named event." << endl;
		des->errors += 1;
		return 0;
	}

	NetEvTrig*trig = new NetEvTrig(eve);
	trig->set_line(*this);
	return trig;
}

/*
* The while loop is fairly directly represented in the netlist.
*/
NetProc* PWhile::elaborate(Design*des, NetScope*scope) const
{
	NetWhile*loop = new NetWhile(elab_and_eval(des, scope, cond_),
		statement_->elaborate(des, scope));
	return loop;
}

/*
* When a module is instantiated, it creates the scope then uses this
* method to elaborate the contents of the module.
*/
bool Module::elaborate(Design*des, NetScope*scope) const
{
	bool result_flag = true;


	// Elaborate functions.
	typedef map<perm_string,PFunction*>::const_iterator mfunc_it_t;
	for (mfunc_it_t cur = funcs_.begin()
		; cur != funcs_.end() ;  cur ++) {

			NetScope*fscope = scope->child((*cur).first);
			assert(fscope);
			(*cur).second->elaborate(des, fscope);
	}

	// Elaborate the task definitions. This is done before the
	// behaviors so that task calls may reference these, and after
	// the signals so that the tasks can reference them.
	typedef map<perm_string,PTask*>::const_iterator mtask_it_t;
	for (mtask_it_t cur = tasks_.begin()
		; cur != tasks_.end() ;  cur ++) {

			NetScope*tscope = scope->child((*cur).first);
			assert(tscope);
			(*cur).second->elaborate(des, tscope);
	}

	// Get all the gates of the module and elaborate them by
	// connecting them to the signals. The gate may be simple or
	// complex.
	const list<PGate*>&gl = get_gates();

	for (list<PGate*>::const_iterator gt = gl.begin()
		; gt != gl.end()
		; gt ++ ) {

			if (mod_name() == "altsyncram") {
				// set operation mode here! -- CCC
				NetExpr *mode;
				for (map<perm_string,NetScope::param_expr_t>::const_iterator cur = scope->parameters.begin();
					cur != scope->parameters.end();
					cur++) {
						if (cur->first == "operation_mode") {
							mode = cur->second.expr;
							break;
						}
				}
				if (mode->expr_width() == 88 || mode->expr_width() == 24)
					(*gt)->elaborate_single(des, scope);
				else if (mode->expr_width() == 72)
					(*gt)->elaborate_dual(des, scope);
				else if (mode->expr_width() == 120)
					(*gt)->elaborate_bidir_dual(des, scope);
				else {
					cerr << get_line() << ": altsyncram has no operation mode." << endl;
					des->errors += 1;
					return false;
				}
			}
			else
				(*gt)->elaborate(des, scope);
	}

	// Elaborate the behaviors, making processes out of them. This
	// involves scanning the PProcess* list, creating a NetProcTop
	// for each process.
	const list<PProcess*>&sl = get_behaviors();

	for (list<PProcess*>::const_iterator st = sl.begin()
		; st != sl.end()
		; st ++ ) {

			NetProc*cur = (*st)->statement()->elaborate(des, scope);
			if (cur == 0) {
				result_flag = false;
				continue;
			}

			NetProcTop*top=NULL;
			switch ((*st)->type()) {
			case PProcess::PR_INITIAL:
				top = new NetProcTop(scope, NetProcTop::KINITIAL, cur);
				break;
			case PProcess::PR_ALWAYS:
				top = new NetProcTop(scope, NetProcTop::KALWAYS, cur);
				break;
			}
			assert(top);

			// Evaluate the attributes for this process, if there
			// are any. These attributes are to be attached to the
			// NetProcTop object.
			struct attrib_list_t*attrib_list = 0;
			unsigned attrib_list_n = 0;
			attrib_list = evaluate_attributes((*st)->attributes,
				attrib_list_n,
				des, scope);

			for (unsigned adx = 0 ;  adx < attrib_list_n ;  adx += 1)
				top->attribute(attrib_list[adx].key,
				attrib_list[adx].val);

			delete[]attrib_list;

			top->set_line(*(*st));
			des->add_process(top);

			/* Detect the special case that this is a combinational
			always block. We want to attach an _ivl_schedule_push
			attribute to this process so that it starts up and
			gets into its wait statement before non-combinational
			code is executed. */
			do {
				if (top->type() != NetProcTop::KALWAYS)
					break;

				NetEvWait*st = dynamic_cast<NetEvWait*>(top->statement());
				if (st == 0)
					break;

				if (st->nevents() != 1)
					break;

				NetEvent*ev = st->event(0);

				if (ev->nprobe() == 0)
					break;

				bool anyedge_test = true;
				for (unsigned idx = 0 ;  anyedge_test && (idx<ev->nprobe())
					; idx += 1) {
						const NetEvProbe*pr = ev->probe(idx);
						if (pr->edge() != NetEvProbe::ANYEDGE)
							anyedge_test = false;
				}

				if (! anyedge_test)
					break;

				top->attribute(perm_string::literal("_ivl_schedule_push"),
					verinum(1));
			} while (0);

	}

	return result_flag;
}

struct root_elem {
	Module *mod;
	NetScope *scope;
};

Design* elaborate(list<perm_string>roots)
{
	svector<root_elem*> root_elems(roots.size());
	bool rc = true;
	unsigned i = 0;
	// This is the output design. I fill it in as I scan the root
	// module and elaborate what I find.
	Design*des = new Design;

	for (list<perm_string>::const_iterator root = roots.begin()
		; root != roots.end()
		; root++) {
	//迭代器遍历所有的根module的module名。
			// Look for the root module in the list.
			map<perm_string,Module*>::const_iterator mod = pform_modules.find(*root);
			if (mod == pform_modules.end()) {
				cerr << "error: Unable to find the root module \""
					<< (*root) << "\" in the Verilog source." << endl;
				cerr << "     : Perhaps ``-s " << (*root)
					<< "'' is incorrect?" << endl;
				des->errors++;
				continue;
			}

			Module *rmod = (*mod).second;

			// Make the root scope, then scan the pform looking for scopes
			// and parameters.
			NetScope*scope = des->make_root_scope(*root);
			scope->time_unit(rmod->time_unit);
			scope->time_precision(rmod->time_precision);
			scope->default_nettype(rmod->default_nettype);
			des->set_precision(rmod->time_precision);
			if (! rmod->elaborate_scope(des, scope)) {
				delete des;
				return 0;
			}

			struct root_elem *r = new struct root_elem;
			r->mod = rmod;
			r->scope = scope;
			root_elems[i++] = r;
	}

	// Errors already? Probably missing root modules. Just give up
	// now and return nothing.
	if (des->errors > 0)
		return des;

	// This method recurses through the scopes, looking for
	// defparam assignments to apply to the parameters in the
	// various scopes. This needs to be done after all the scopes
	// and basic parameters are taken care of because the defparam
	// can assign to a parameter declared *after* it.
	des->run_defparams();


	// At this point, all parameter overrides are done. Scan the
	// scopes and evaluate the parameters all the way down to
	// constants.
	des->evaluate_parameters();

	// With the parameters evaluated down to constants, we have
	// what we need to elaborate signals and memories. This pass
	// creates all the NetNet and NetMemory objects for declared
	// objects.
	for (i = 0; i < root_elems.count(); i++) {
		Module *rmod = root_elems[i]->mod;
		NetScope *scope = root_elems[i]->scope;

		if (! rmod->elaborate_sig(des, scope)) {
			delete des;
			return 0;
		}
	}

	// Now that the structure and parameters are taken care of,
	// run through the pform again and generate the full netlist.
	for (i = 0; i < root_elems.count(); i++) {
		Module *rmod = root_elems[i]->mod;
		NetScope *scope = root_elems[i]->scope;

		rc &= rmod->elaborate(des, scope);
	}


	if (rc == false) {
		delete des;
		des = 0;
	}

	return des;
}


/*
* $Log: elaborate.cc,v $
* Revision 1.308.2.6  2006/11/27 01:47:14  steve
*  Fix crash handling constant true conditional.
*
* Revision 1.308.2.5  2006/11/27 01:32:24  steve
*  Fix evaluate of constant condition expressions.
*
* Revision 1.308.2.4  2006/07/10 00:21:50  steve
*  Add support for full_case attribute.
*
* Revision 1.308.2.3  2006/06/12 00:16:51  steve
*  Add support for -Wunused warnings.
*
* Revision 1.308.2.2  2005/12/10 03:30:50  steve
*  Fix crash on block with assignments that assign lval to self.
*
* Revision 1.308.2.1  2005/11/13 22:28:14  steve
*  Do not panic if case statement is nul.
*
* Revision 1.308  2004/10/04 01:10:52  steve
*  Clean up spurious trailing white space.
*
* Revision 1.307  2004/09/05 21:07:26  steve
*  Support degenerat wait statements.
*
* Revision 1.306  2004/09/05 17:44:41  steve
*  Add support for module instance arrays.
*
* Revision 1.305  2004/06/30 15:32:02  steve
*  Propagate source line number in synthetic delay statements.
*
* Revision 1.304  2004/06/20 15:59:06  steve
*  Only pad the width of vector r-values.
*
* Revision 1.303  2004/06/13 04:56:54  steve
*  Add support for the default_nettype directive.
*
* Revision 1.302  2004/05/31 23:34:37  steve
*  Rewire/generalize parsing an elaboration of
*  function return values to allow for better
*  speed and more type support.
*
* Revision 1.301  2004/05/25 03:42:58  steve
*  Handle wait with constant-false expression.
*
* Revision 1.300  2004/03/08 00:47:44  steve
*  primitive ports can bind bi name.
*
* Revision 1.299  2004/03/08 00:10:29  steve
*  Verilog2001 new style port declartions for primitives.
*
* Revision 1.298  2004/03/07 20:04:10  steve
*  MOre thorough use of elab_and_eval function.
*
* Revision 1.297  2004/02/20 18:53:34  steve
*  Addtrbute keys are perm_strings.
*
* Revision 1.296  2004/02/18 17:11:55  steve
*  Use perm_strings for named langiage items.
*
* Revision 1.295  2004/01/21 04:35:03  steve
*  Get rid of useless warning.
*
* Revision 1.294  2004/01/13 03:42:49  steve
*  Handle wide expressions in wait condition.
*
* Revision 1.293  2003/10/26 04:49:51  steve
*  Attach line number information to for loop parts.
*
* Revision 1.292  2003/09/25 00:25:14  steve
*  Summary list of missing modules.
*
* Revision 1.291  2003/09/20 06:08:53  steve
*  Evaluate nb-assign r-values using elab_and_eval.
*
* Revision 1.290  2003/09/20 06:00:37  steve
*  Evaluate gate array index constants using elab_and_eval.
*
* Revision 1.289  2003/09/20 01:05:35  steve
*  Obsolete find_symbol and find_event from the Design class.
*
* Revision 1.288  2003/09/13 01:01:51  steve
*  Spelling fixes.
*
* Revision 1.287  2003/09/04 20:28:05  steve
*  Support time0 resolution of combinational threads.
*
* Revision 1.286  2003/08/28 04:11:17  steve
*  Spelling patch.
*
* Revision 1.285  2003/08/05 03:01:58  steve
*  Primitive outputs have same limitations as continuous assignment.
*
* Revision 1.284  2003/07/02 04:19:16  steve
*  Elide empty begin-end in conditionals.
*
* Revision 1.283  2003/06/21 01:21:43  steve
*  Harmless fixup of warnings.
*
* Revision 1.282  2003/06/13 19:10:20  steve
*  Handle assign of real to vector.
*
* Revision 1.281  2003/05/19 02:50:58  steve
*  Implement the wait statement behaviorally instead of as nets.
*
* Revision 1.280  2003/05/04 20:04:08  steve
*  Fix truncation of signed constant in constant addition.
*
* Revision 1.279  2003/04/24 05:25:55  steve
*  Include port name in port assignment error message.
*/

