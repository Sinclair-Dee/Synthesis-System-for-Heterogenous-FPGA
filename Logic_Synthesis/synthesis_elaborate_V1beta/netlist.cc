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
#ident "$Id: netlist.cc,v 1.226.2.7 2006/04/23 04:26:14 steve Exp $"
#endif

# include "config.h"

# include <iostream>

# include  <cassert>
# include  <typeinfo>
# include  "compiler.h"
# include  "netlist.h"
# include  "netmisc.h"


ostream& operator<< (ostream&o, NetNet::Type t)
{
	switch (t) {
	case NetNet::NONE:
		o << "net_none";
		break;
	case NetNet::IMPLICIT:
		o << "wire /*implicit*/";
		break;
	case NetNet::IMPLICIT_REG:
		o << "reg /*implicit*/";
		break;
	case NetNet::INTEGER:
		o << "integer";
		break;
	case NetNet::REG:
		o << "reg";
		break;
	case NetNet::SUPPLY0:
		o << "supply0";
		break;
	case NetNet::SUPPLY1:
		o << "supply1";
		break;
	case NetNet::TRI:
		o << "tri";
		break;
	case NetNet::TRI0:
		o << "tri0";
		break;
	case NetNet::TRI1:
		o << "tri1";
		break;
	case NetNet::TRIAND:
		o << "triand";
		break;
	case NetNet::TRIOR:
		o << "trior";
		break;
	case NetNet::WAND:
		o << "wand";
		break;
	case NetNet::WOR:
		o << "wor";
		break;
	case NetNet::WIRE:
		o << "wire";
		break;
	}
	return o;
}


unsigned count_inputs(const Link&pin)
{
	unsigned count = 0;

	const Nexus*nex = pin.nexus();
	for (const Link*clnk = nex->first_nlink()
		; clnk ; clnk = clnk->next_nlink()) {
			const NetObj*cur;
			unsigned cpin;
			clnk->cur_link(cur, cpin);
			if (cur->pin(cpin).get_dir() == Link::INPUT)
				count += 1;
	}

	return count;
}

unsigned count_outputs(const Link&pin)
{
	unsigned count = 0;

	const Nexus*nex = pin.nexus();
	for (const Link*clnk = nex->first_nlink()
		; clnk ; clnk = clnk->next_nlink()) {
			const NetObj*cur;
			unsigned cpin;
			clnk->cur_link(cur, cpin);
			if (cur->pin(cpin).get_dir() == Link::OUTPUT)
				count += 1;
	}

	return count;
}

unsigned count_signals(const Link&pin)
{
	unsigned count = 0;

	const Nexus*nex = pin.nexus();
	for (const Link*clnk = nex->first_nlink()
		; clnk ; clnk = clnk->next_nlink()) {
			const NetObj*cur;
			unsigned cpin;
			clnk->cur_link(cur, cpin);
			if (dynamic_cast<const NetNet*>(cur))
				count += 1;
	}

	return count;
}

const NetNet* find_link_signal(const NetObj*net, unsigned pin, unsigned&bidx)
{
	const Nexus*nex = net->pin(pin).nexus();

	for (const Link*clnk = nex->first_nlink()
		; clnk ; clnk = clnk->next_nlink()) {

			const NetObj*cur;
			unsigned cpin;
			clnk->cur_link(cur, cpin);

			const NetNet*sig = dynamic_cast<const NetNet*>(cur);
			if (sig) {
				bidx = cpin;
				return sig;
			}
	}

	return 0;
}

Link* find_next_output(Link*lnk)
{
	Link*cur = lnk->next_nlink();
	while (cur != lnk) {
		if (cur->get_dir() == Link::OUTPUT)
			return cur;

		cur = cur->next_nlink();
		if (cur == 0)
			cur = lnk->nexus()->first_nlink();
	}

	return 0;
}

NetObj::NetObj(NetScope*s, perm_string n, unsigned np)
	: scope_(s), name_(n), npins_(np), delay1_(0), delay2_(0), delay3_(0)
{
	pins_ = new Link[npins_];
	for (unsigned idx = 0 ;  idx < npins_ ;  idx += 1) {
		pins_[idx].node_ = this;
		pins_[idx].pin_  = idx;
	}
}

NetObj::~NetObj()
{
	delete[]pins_;
}

NetScope* NetObj::scope()
{
	return scope_;
}

const NetScope* NetObj::scope() const
{
	return scope_;
}

//ADU:
//const NetScope* NetObj::top_scope()const{
//	const NetScope* cur = this->scope();
//	while(cur->parent() != 0)
//		cur = cur->parent();
//	return cur;
//}

Link& NetObj::pin(unsigned idx)
{
	if (idx >= npins_) {
		cerr << get_line() << ": internal error: pin("<<idx<<")"
			<< " out of bounds("<<npins_<<")" << endl;
		cerr << get_line() << ":               : typeid="
			<< typeid(*this).name() << endl;
	}

	assert(idx < npins_);
	return pins_[idx];
}

const Link& NetObj::pin(unsigned idx) const
{
	assert(idx < npins_);
	return pins_[idx];
}

NetNode::NetNode(NetScope*s, perm_string n, unsigned npins)
	: NetObj(s, n, npins), node_next_(0), node_prev_(0), design_(0)
{
}

NetNode::~NetNode()
{
	if (design_)
		design_->del_node(this);
}

NetNet::NetNet(NetScope*s, perm_string n, Type t, unsigned npins)
	: NetObj(s, n, npins), sig_next_(0), sig_prev_(0),
	type_(t), port_type_(NOT_A_PORT), signed_(false), msb_(npins-1), lsb_(0),
	local_flag_(true), eref_count_(0), lref_count_(0), mref_(0)/*local_flag_改为true——ADU*/
{
	assert(s);

	release_list_ = 0;

	verinum::V init_value = verinum::Vz;
	Link::DIR dir = Link::PASSIVE;

	switch (t) {
	case REG:
	case INTEGER:
	case IMPLICIT_REG:
		init_value = verinum::Vx;
		dir = Link::OUTPUT;
		break;
	case SUPPLY0:
		init_value = verinum::V0;
		dir = Link::OUTPUT;
		break;
	case SUPPLY1:
		init_value = verinum::V1;
		dir = Link::OUTPUT;
		break;
	default:
		break;
	}

	for (unsigned idx = 0 ;  idx < npins ;  idx += 1) {
		pin(idx).set_name(perm_string::literal("P"), idx);
		pin(idx).set_dir(dir);
		pin(idx).set_init(init_value);
	}

	scope()->add_signal(this);
}

NetNet::NetNet(NetScope*s, perm_string n, Type t, long ms, long ls)
	: NetObj(s, n, ((ms>ls)?ms-ls:ls-ms) + 1),
	sig_next_(0), sig_prev_(0), type_(t),
	port_type_(NOT_A_PORT), signed_(false), msb_(ms), lsb_(ls),
	local_flag_(true), eref_count_(0), lref_count_(0), mref_(0)/*local_flag_改为true——ADU*/
{
	assert(s);

	release_list_ = 0;

	verinum::V init_value = verinum::Vz;
	Link::DIR dir = Link::PASSIVE;

	switch (t) {
	case REG:
	case IMPLICIT_REG:
		init_value = verinum::Vx;
		dir = Link::OUTPUT;
		break;
	case SUPPLY0:
		init_value = verinum::V0;
		dir = Link::OUTPUT;
		break;
	case SUPPLY1:
		init_value = verinum::V1;
		dir = Link::OUTPUT;
		break;
	default:
		break;
	}

	for (unsigned idx = 0 ;  idx < pin_count() ;  idx += 1) {
		pin(idx).set_name(perm_string::literal("P"), idx);
		pin(idx).set_dir(dir);
		pin(idx).set_init(init_value);
	}

	s->add_signal(this);
}

NetNet::~NetNet()
{
	if (eref_count_ > 0) {
		cerr << get_line() << ": internal error: attempt to delete "
			<< "signal ``" << name() << "'' which has "
			<< "expression references." << endl;
		dump_net(cerr, 4);
	}
	assert(eref_count_ == 0);
	if (lref_count_ > 0) {
		cerr << get_line() << ": internal error: attempt to delete "
			<< "signal ``" << name() << "'' which has "
			<< "assign references." << endl;
		dump_net(cerr, 4);
	}
	assert(lref_count_ == 0);
	if (mref_ != 0) {
		cerr << get_line() << ": internal error: attempt to delete "
			<< "signal ``" << name() << "'' which has "
			<< "memory explode references." << endl;
		dump_net(cerr, 4);
	}
	assert(mref_ == 0);

	if (scope())
		scope()->rem_signal(this);

	/* Detach me from all the NetRelease objects that refer to me. */
	while (release_list_) {
		NetRelease*tmp = release_list_;
		release_list_ = tmp->release_next_;
		assert(tmp->lval_ == this);
		tmp->lval_ = 0;
		tmp->release_next_ = 0;
	}
}

bool NetNet::at_least_one_pin_linked_to_signi_signal()const{
	for(unsigned idx = 0; idx < pin_count(); idx++){
		if(pin(idx).is_linkedToSignificantSignal())
			return true;
	}

	return false;
}

NetNet::Type NetNet::type() const
{
	return type_;
}

void NetNet::type(NetNet::Type t)
{
	if (type_ == t)
		return;

	Link::DIR dir = Link::PASSIVE;
	switch (t) {
	case REG:
	case IMPLICIT_REG:
		dir = Link::OUTPUT;
		break;
	case SUPPLY0:
		dir = Link::OUTPUT;
		break;
	case SUPPLY1:
		dir = Link::OUTPUT;
		break;
	default:
		break;
	}

	type_ = t;
	for (unsigned idx = 0 ;  idx < pin_count() ;  idx += 1) {
		pin(idx).set_dir(dir);
	}

}


NetNet::PortType NetNet::port_type() const
{
	return port_type_;
}

void NetNet::port_type(NetNet::PortType t)
{
	port_type_ = t;
}

bool NetNet::get_signed() const
{
	return signed_;
}

void NetNet::set_signed(bool flag)
{
	signed_ = flag;
}

bool NetNet::get_isint() const
{
	return isint_;
}

void NetNet::set_isint(bool flag)
{
	isint_ = flag;
}

long NetNet::lsb() const
{
	return lsb_;
}

long NetNet::msb() const
{
	return msb_;
}

bool NetNet::sb_is_valid(long sb) const
{
	if (msb_ >= lsb_)
		return (sb <= msb_) && (sb >= lsb_);
	else
		return (sb <= lsb_) && (sb >= msb_);
}

unsigned NetNet::sb_to_idx(long sb) const
{
	if (msb_ >= lsb_)
		return sb - lsb_;
	else
		return lsb_ - sb;
}

void NetNet::incr_eref()
{
	eref_count_ += 1;
}

void NetNet::decr_eref()
{
	assert(eref_count_ > 0);
	eref_count_ -= 1;
}

unsigned NetNet::peek_eref() const
{
	return eref_count_;
}

void NetNet::incr_lref()
{
	lref_count_ += 1;
}

void NetNet::decr_lref()
{
	assert(lref_count_ > 0);
	lref_count_ -= 1;
}

unsigned NetNet::peek_lref() const
{
	return lref_count_;
}

unsigned NetNet::get_refs() const
{
	return lref_count_ + eref_count_;
}

void NetNet::mref(NetMemory*mem)
{
	assert(mref_ == 0);
	assert(mem != 0);
	mref_ = mem;
}

NetMemory*NetNet::mref()
{
	return mref_;
}

const NetMemory* NetNet::mref() const
{
	return mref_;
}


NetSubnet::NetSubnet(NetNet*sig, unsigned off, unsigned wid)
	: NetNet(sig->scope(), sig->scope()->local_symbol(), sig->type(), wid)
{
	for (unsigned idx = 0 ;  idx < wid ;  idx += 1)
		connect(sig->pin(idx+off), pin(idx));

	port_type(sig->port_type());
	local_flag(true);
	set_line(*sig);
}

NetProc::NetProc()
	: next_(0)
{
}

NetProc::~NetProc()
{
}

NetProcTop::NetProcTop(NetScope*s, Type t, NetProc*st)
	: type_(t), statement_(st), scope_(s)
{
}

NetProcTop::~NetProcTop()
{
	delete statement_;
}

NetProc* NetProcTop::statement()
{
	return statement_;
}

const NetProc* NetProcTop::statement() const
{
	return statement_;
}

NetScope* NetProcTop::scope()
{
	return scope_;
}

const NetScope* NetProcTop::scope() const
{
	return scope_;
}

/*
* The NetFF class represents an LPM_FF device. The pinout is assigned
* like so:
*    0  -- Clock
*    1  -- Enable
*    2  -- Aload
*    3  -- Aset
*    4  -- Aclr
*    5  -- Sload
*    6  -- Sset
*    7  -- Sclr
*
*    8  -- Data[0]
*    9  -- Q[0]
*     ...
*/

NetFF::NetFF(NetScope*s, perm_string n, unsigned wid)
	: NetNode(s, n, 8 + 2*wid)
{
	demux_ = 0;
	mem_ = 0;

	pin_Clock().set_dir(Link::INPUT);
	pin_Clock().set_name(perm_string::literal("Clock"), 0);
	pin_Enable().set_dir(Link::INPUT);
	pin_Enable().set_name(perm_string::literal("Enable"), 0);
	pin_Aload().set_dir(Link::INPUT);
	pin_Aload().set_name(perm_string::literal("Aload"), 0);
	pin_Aset().set_dir(Link::INPUT);
	pin_Aset().set_name(perm_string::literal("Aset"), 0);
	pin_Aclr().set_dir(Link::INPUT);
	pin_Aclr().set_name(perm_string::literal("Aclr"), 0);
	pin_Sload().set_dir(Link::INPUT);
	pin_Sload().set_name(perm_string::literal("Sload"), 0);
	pin_Sset().set_dir(Link::INPUT);
	pin_Sset().set_name(perm_string::literal("Sset"), 0);
	pin_Sclr().set_dir(Link::INPUT);
	pin_Sclr().set_name(perm_string::literal("Sclr"), 0);
	for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
		pin_Data(idx).set_dir(Link::INPUT);
		pin_Data(idx).set_name(perm_string::literal("Data"), idx);
		pin_Q(idx).set_dir(Link::OUTPUT);
		pin_Q(idx).set_name(perm_string::literal("Q"), idx);
	}
}

NetFF::~NetFF()
{
}

unsigned NetFF::width() const
{
	return (pin_count() - 8) / 2;
}

Link& NetFF::pin_Clock()
{
	return pin(0);
}

const Link& NetFF::pin_Clock() const
{
	return pin(0);
}

Link& NetFF::pin_Enable()
{
	return pin(1);
}

const Link& NetFF::pin_Enable() const
{
	return pin(1);
}

Link& NetFF::pin_Aload()
{
	return pin(2);
}

Link& NetFF::pin_Aset()
{
	return pin(3);
}

const Link& NetFF::pin_Aset() const
{
	return pin(3);
}

Link& NetFF::pin_Aclr()
{
	return pin(4);
}

const Link& NetFF::pin_Aclr() const
{
	return pin(4);
}

Link& NetFF::pin_Sload()
{
	return pin(5);
}

Link& NetFF::pin_Sset()
{
	return pin(6);
}

const Link& NetFF::pin_Sset() const
{
	return pin(6);
}

Link& NetFF::pin_Sclr()
{
	return pin(7);
}

const Link& NetFF::pin_Sclr() const
{
	return pin(7);
}

Link& NetFF::pin_Data(unsigned w)
{
	unsigned pn = 8 + 2*w;
	assert(pn < pin_count());
	return pin(pn);
}

const Link& NetFF::pin_Data(unsigned w) const
{
	unsigned pn = 8 + 2*w;
	assert(pn < pin_count());
	return pin(pn);
}

Link& NetFF::pin_Q(unsigned w)
{
	unsigned pn = 9 + w*2;
	assert(pn < pin_count());
	return pin(pn);
}

const Link& NetFF::pin_Q(unsigned w) const
{
	unsigned pn = 9 + w*2;
	assert(pn < pin_count());
	return pin(pn);
}

void NetFF::aset_value(const verinum&val)
{
	aset_value_ = val;
}

const verinum& NetFF::aset_value() const
{
	return aset_value_;
}

void NetFF::sset_value(const verinum&val)
{
	sset_value_ = val;
}

const verinum& NetFF::sset_value() const
{
	return sset_value_;
}
//ADU设置存储器
bool NetFF::set_mem(NetMemory *mem)
{
	assert(!mem_);
	mem_ = mem;
	return true;
}

NetMemory* NetFF::get_mem()
{
	return mem_;
}


unsigned NetDecode::width() const
{
	return width_;
}

unsigned NetDecode::awidth() const
{
	return pin_count();
}



//ADU: NetDecoder
NetDecoder::NetDecoder(NetScope*s, perm_string name, unsigned wid, unsigned decodes)
	:NetNode(s, name, 3+wid + decodes),width_(wid),decodes_(decodes){
		
		pin(0).set_dir(Link::INPUT); pin(0).set_name(perm_string::literal("Aclr"),  0);
		pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("Clock"), 0);
		pin(2).set_dir(Link::INPUT); pin(2).set_name(perm_string::literal("Enable"), 0);



		for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
			pin_Data(idx).set_dir(Link::INPUT);
			pin_Data(idx).set_name(perm_string::literal("Data"), idx);
		}

		for (unsigned idx = 0 ;  idx < ( 1U << width_) ;  idx += 1) {
			pin_Eq(idx).set_dir(Link::OUTPUT);
			pin_Eq(idx).set_name(perm_string::literal("Result"), idx);
		}

}

Link& NetDecoder::pin_Aclr(){
	return pin(0);
}

const Link& NetDecoder::pin_Aclr() const{
	return pin(0);
}

Link& NetDecoder::pin_Clock(){
	return pin(1);
}

const Link& NetDecoder::pin_Clock() const{
	return pin(1);
}

Link& NetDecoder::pin_Enable(){
	return pin(2);
}

const Link& NetDecoder::pin_Enable() const{
	return pin(2);
}

Link& NetDecoder::pin_Data(unsigned d)
{
	assert(d < width_);
	return pin(3+d);
}

const Link& NetDecoder::pin_Data(unsigned d) const
{
	assert(d < width_);
	return pin(3+d);
}

Link& NetDecoder::pin_Eq(unsigned r)
{
	assert(r < decodes_);
	return pin(3 + width_ + r);
}

const Link& NetDecoder::pin_Eq(unsigned r) const
{
	assert(r < decodes_);
	return pin(3 + width_ + r);
}


NetDemux::NetDemux(NetScope*s, perm_string name,
	unsigned bus_width, unsigned address_width, unsigned size)
	: NetNode(s, name, bus_width*2+address_width+bus_width/size)
{
	width_ = bus_width;
	awidth_ = address_width;
	size_ = size;

	for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
		pin_Q(idx).set_dir(Link::OUTPUT);
		pin_Q(idx).set_name(perm_string::literal("Q"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
		pin_Data(idx).set_dir(Link::INPUT);
		pin_Data(idx).set_name(perm_string::literal("Data"), idx);
	}
	for (unsigned idx = 0 ;  idx < awidth_ ;  idx += 1) {
		pin_Address(idx).set_dir(Link::INPUT);
		pin_Address(idx).set_name(perm_string::literal("Address"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_/size_ ;  idx += 1) {
		pin_WriteData(idx).set_dir(Link::INPUT);
		pin_WriteData(idx).set_name(perm_string::literal("Writedata"), idx);
	}
}

NetDemux::~NetDemux()
{
}

unsigned NetDemux::width() const
{
	return width_;
}

unsigned NetDemux::awidth() const
{
	return awidth_;
}

unsigned NetDemux::size() const
{
	return size_;
}

Link& NetDemux::pin_Q(unsigned idx)
{
	assert(idx < width_);
	return pin(idx);
}

const Link& NetDemux::pin_Q(unsigned idx) const
{
	assert(idx < width_);
	return pin(idx);
}

Link& NetDemux::pin_Data(unsigned idx)
{
	assert(idx < width_);
	return pin(width_+idx);
}

const Link& NetDemux::pin_Data(unsigned idx) const
{
	assert(idx < width_);
	return pin(width_+idx);
}

Link& NetDemux::pin_Address(unsigned idx)
{
	assert(idx < awidth_);
	return pin(width_+width_+idx);
}

const Link& NetDemux::pin_Address(unsigned idx) const
{
	assert(idx < awidth_);
	return pin(width_+width_+idx);
}

Link& NetDemux::pin_WriteData(unsigned idx)
{
	assert(idx < width_/size_);
	return pin(width_+width_+awidth_+idx);
}

const Link& NetDemux::pin_WriteData(unsigned idx) const
{
	assert(idx < width_/size_);
	return pin(width_+width_+awidth_+idx);
}

NetDecode* NetFF::get_demux()
{
	return demux_;
}

const NetDecode* NetFF::get_demux() const
{
	return demux_;
}

/*
* The NetAddSub class represents an LPM_ADD_SUB device. The pinout is
* assigned like so:
*    0  -- Add_Sub
*    1  -- Aclr
*    2  -- Clock
*    3  -- Cin
*    4  -- Cout
*    5  -- Overflow
*    6  -- DataA[0]
*    7  -- DataB[0]
*    8  -- Result[0]
*/
NetAddSub::NetAddSub(NetScope*s, perm_string n, unsigned w)
	: NetNode(s, n, w*3+6)
{
	pin(0).set_dir(Link::INPUT); pin(0).set_name(
		perm_string::literal("Add_Sub"), 0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(
		perm_string::literal("Aclr"), 0);
	pin(2).set_dir(Link::INPUT); pin(2).set_name(
		perm_string::literal("Clock"), 0);
	pin(3).set_dir(Link::INPUT); pin(3).set_name(
		perm_string::literal("Cin"), 0);
	pin(4).set_dir(Link::OUTPUT); pin(4).set_name(
		perm_string::literal("Cout"), 0);
	pin(5).set_dir(Link::OUTPUT); pin(5).set_name(
		perm_string::literal("Overflow"), 0);
	for (unsigned idx = 0 ;  idx < w ;  idx += 1) {
		pin_DataA(idx).set_dir(Link::INPUT);
		pin_DataB(idx).set_dir(Link::INPUT);
		pin_Result(idx).set_dir(Link::OUTPUT);
		pin_DataA(idx).set_name(perm_string::literal("DataA"), idx);
		pin_DataB(idx).set_name(perm_string::literal("DataB"), idx);
		pin_Result(idx).set_name(perm_string::literal("Result"), idx);
	}
}

NetAddSub::~NetAddSub()
{
}

unsigned NetAddSub::width()const
{
	return (pin_count() - 6) / 3;
}

Link& NetAddSub::pin_Cout()
{
	return pin(4);
}

const Link& NetAddSub::pin_Cout() const
{
	return pin(4);
}

Link& NetAddSub::pin_DataA(unsigned idx)
{
	idx = 6 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

const Link& NetAddSub::pin_DataA(unsigned idx) const
{
	idx = 6 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

Link& NetAddSub::pin_DataB(unsigned idx)
{
	idx = 7 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

const Link& NetAddSub::pin_DataB(unsigned idx) const
{
	idx = 7 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

Link& NetAddSub::pin_Result(unsigned idx)
{
	idx = 8 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

const Link& NetAddSub::pin_Result(unsigned idx) const
{
	idx = 8 + idx*3;
	assert(idx < pin_count());
	return pin(idx);
}

/*
* The pinout for the NetCLShift is:
*    0   -- Direction
*    1   -- Underflow
*    2   -- Overflow
*    3   -- Data(0)
*    3+W -- Result(0)
*    3+2W -- Distance(0)
*/
NetCLShift::NetCLShift(NetScope*s, perm_string n,
	unsigned width, unsigned width_dist,
	bool right_flag, bool signed_flag)
	: NetNode(s, n, 3+2*width+width_dist),
	width_(width), width_dist_(width_dist),
	right_flag_(right_flag), signed_flag_(signed_flag)
{
	pin(0).set_dir(Link::INPUT); pin(0).set_name(
		perm_string::literal("Direction"), 0);
	pin(1).set_dir(Link::OUTPUT); pin(1).set_name(
		perm_string::literal("Underflow"), 0);
	pin(2).set_dir(Link::OUTPUT); pin(2).set_name(
		perm_string::literal("Overflow"), 0);

	for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
		pin(3+idx).set_dir(Link::INPUT);
		pin(3+idx).set_name(perm_string::literal("Data"), idx);

		pin(3+width_+idx).set_dir(Link::OUTPUT);
		pin(3+width_+idx).set_name(perm_string::literal("Result"), idx);
	}

	for (unsigned idx = 0 ;  idx < width_dist_ ;  idx += 1) {
		pin(3+2*width_+idx).set_dir(Link::INPUT);
		pin(3+2*width_+idx).set_name(perm_string::literal("Distance"), idx);
	}
}

NetCLShift::~NetCLShift()
{
}

unsigned NetCLShift::width() const
{
	return width_;
}

unsigned NetCLShift::width_dist() const
{
	return width_dist_;
}

bool NetCLShift::right_flag() const
{
	return right_flag_;
}

bool NetCLShift::signed_flag() const
{
	return signed_flag_;
}

#if 0
Link& NetCLShift::pin_Direction()
{
	return pin(0);
}

const Link& NetCLShift::pin_Direction() const
{
	return pin(0);
}
#endif

Link& NetCLShift::pin_Underflow()
{
	return pin(1);
}

const Link& NetCLShift::pin_Underflow() const
{
	return pin(1);
}

Link& NetCLShift::pin_Overflow()
{
	return pin(2);
}

const Link& NetCLShift::pin_Overflow() const
{
	return pin(2);
}

Link& NetCLShift::pin_Data(unsigned idx)
{
	assert(idx < width_);
	return pin(3+idx);
}

const Link& NetCLShift::pin_Data(unsigned idx) const
{
	assert(idx < width_);
	return pin(3+idx);
}

Link& NetCLShift::pin_Result(unsigned idx)
{
	assert(idx < width_);
	return pin(3+width_+idx);
}

const Link& NetCLShift::pin_Result(unsigned idx) const
{
	assert(idx < width_);
	return pin(3+width_+idx);
}

Link& NetCLShift::pin_Distance(unsigned idx)
{
	assert(idx < width_dist_);
	return pin(3+2*width_+idx);
}

const Link& NetCLShift::pin_Distance(unsigned idx) const
{
	assert(idx < width_dist_);
	return pin(3+2*width_+idx);
}

NetCompare::NetCompare(NetScope*s, perm_string n, unsigned wi)
	: NetNode(s, n, 8+2*wi), width_(wi)
{
	signed_flag_ = false;
	pin(0).set_dir(Link::INPUT); pin(0).set_name(
		perm_string::literal("Aclr"));
	pin(1).set_dir(Link::INPUT); pin(1).set_name(
		perm_string::literal("Clock"));
	pin(2).set_dir(Link::OUTPUT); pin(2).set_name(
		perm_string::literal("AGB"));
	pin(3).set_dir(Link::OUTPUT); pin(3).set_name(
		perm_string::literal("AGEB"));
	pin(4).set_dir(Link::OUTPUT); pin(4).set_name(
		perm_string::literal("AEB"));
	pin(5).set_dir(Link::OUTPUT); pin(5).set_name(
		perm_string::literal("ANEB"));
	pin(6).set_dir(Link::OUTPUT); pin(6).set_name(
		perm_string::literal("ALB"));
	pin(7).set_dir(Link::OUTPUT); pin(7).set_name(
		perm_string::literal("ALEB"));
	for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
		pin(8+idx).set_dir(Link::INPUT);
		pin(8+idx).set_name(perm_string::literal("DataA"), idx);
		pin(8+width_+idx).set_dir(Link::INPUT);
		pin(8+width_+idx).set_name(perm_string::literal("DataB"), idx);
	}
}

NetCompare::~NetCompare()
{
}

unsigned NetCompare::width() const
{
	return width_;
}

bool NetCompare::get_signed() const
{
	return signed_flag_;
}

void NetCompare::set_signed(bool flag)
{
	signed_flag_ = flag;
}

Link& NetCompare::pin_Aclr()
{
	return pin(0);
}

const Link& NetCompare::pin_Aclr() const
{
	return pin(0);
}

Link& NetCompare::pin_Clock()
{
	return pin(1);
}

const Link& NetCompare::pin_Clock() const
{
	return pin(1);
}

Link& NetCompare::pin_AGB()
{
	return pin(2);
}

const Link& NetCompare::pin_AGB() const
{
	return pin(2);
}

Link& NetCompare::pin_AGEB()
{
	return pin(3);
}

const Link& NetCompare::pin_AGEB() const
{
	return pin(3);
}

Link& NetCompare::pin_AEB()
{
	return pin(4);
}

const Link& NetCompare::pin_AEB() const
{
	return pin(4);
}

Link& NetCompare::pin_ANEB()
{
	return pin(5);
}

const Link& NetCompare::pin_ANEB() const
{
	return pin(5);
}

Link& NetCompare::pin_ALB()
{
	return pin(6);
}

const Link& NetCompare::pin_ALB() const
{
	return pin(6);
}

Link& NetCompare::pin_ALEB()
{
	return pin(7);
}

const Link& NetCompare::pin_ALEB() const
{
	return pin(7);
}

Link& NetCompare::pin_DataA(unsigned idx)
{
	return pin(8+idx);
}

const Link& NetCompare::pin_DataA(unsigned idx) const
{
	return pin(8+idx);
}

Link& NetCompare::pin_DataB(unsigned idx)
{
	return pin(8+width_+idx);
}

const Link& NetCompare::pin_DataB(unsigned idx) const
{
	return pin(8+width_+idx);
}

NetDecode::NetDecode(NetScope*s, perm_string name, NetFF*mem,
	unsigned awid, unsigned word_width)
	: NetNode(s, name, awid)
{
	width_ = word_width;
	assert( mem->width() % width_ == 0 );

	ff_ = mem;
	ff_->demux_ = this;
	make_pins_(awid);
}

NetDecode::~NetDecode()
{
}

void NetDecode::make_pins_(unsigned awid)
{
	for (unsigned idx = 0 ;  idx < awid ;  idx += 1) {
		pin(idx).set_dir(Link::INPUT);
		pin(idx).set_name(perm_string::literal("Address"), idx);
	}
}

Link& NetDecode::pin_Address(unsigned idx)
{
	return pin(idx);
}

const Link& NetDecode::pin_Address(unsigned idx) const
{
	return pin(idx);
}

NetDivide::NetDivide(NetScope*sc, perm_string n, unsigned wr,
	unsigned wa, unsigned wb)
	: NetNode(sc, n, wr+wa+wb),
	width_r_(wr), width_a_(wa), width_b_(wb), signed_flag_(false)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_r_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("Result"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_a_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("DataA"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_b_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("DataB"), idx);
	}
}

NetDivide::~NetDivide()
{
}

unsigned NetDivide::width_r() const
{
	return width_r_;
}

unsigned NetDivide::width_a() const
{
	return width_a_;
}

unsigned NetDivide::width_b() const
{
	return width_b_;
}

void NetDivide::set_signed(bool flag)
{
	signed_flag_ = flag;
}

bool NetDivide::get_signed() const
{
	return signed_flag_;
}

Link& NetDivide::pin_Result(unsigned idx)
{
	assert(idx < width_r_);
	return pin(idx);
}

const Link& NetDivide::pin_Result(unsigned idx) const
{
	assert(idx < width_r_);
	return pin(idx);
}

Link& NetDivide::pin_DataA(unsigned idx)
{
	assert(idx < width_a_);
	return pin(idx+width_r_);
}

const Link& NetDivide::pin_DataA(unsigned idx) const
{
	assert(idx < width_a_);
	return pin(idx+width_r_);
}

Link& NetDivide::pin_DataB(unsigned idx)
{
	assert(idx < width_b_);
	return pin(idx+width_r_+width_a_);
}

const Link& NetDivide::pin_DataB(unsigned idx) const
{
	assert(idx < width_b_);
	return pin(idx+width_r_+width_a_);
}

NetMult::NetMult(NetScope*sc, perm_string n, unsigned wr,
	unsigned wa, unsigned wb, unsigned ws)
	: NetNode(sc, n, 2+wr+wa+wb+ws),
	signed_(false), width_r_(wr), width_a_(wa), width_b_(wb), width_s_(ws)
{
	pin(0).set_dir(Link::INPUT); pin(0).set_name(perm_string::literal("Aclr"), 0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("Clock"), 0);


	unsigned p = 2;
	for (unsigned idx = 0 ;  idx < width_r_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("Result"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_a_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("DataA"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_b_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("DataB"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_s_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("Sum"), idx);
	}
}

NetMult::~NetMult()
{
}

NetMem::NetMem(NetScope*sc, perm_string n,
	unsigned w_address_a, unsigned w_address_b, unsigned w_data_a,
	unsigned w_data_b, unsigned w_q_a, unsigned w_q_b)
	: NetNode(sc, n, 8 + w_address_a + w_address_b + w_data_a + w_data_b + w_q_a + w_q_b),
	width_address_a_(w_address_a), width_address_b_(w_address_b), width_data_a_(w_data_a),
	width_data_b_(w_data_b), width_q_a_(w_q_a), width_q_b_(w_q_b)
{
	pin(0).set_dir(Link::INPUT); pin(0).set_name(perm_string::literal("clocken0"), 0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("clocken1"), 0);
	pin(2).set_dir(Link::INPUT); pin(2).set_name(perm_string::literal("wren_a"), 0);
	pin(3).set_dir(Link::INPUT); pin(3).set_name(perm_string::literal("aclr0"), 0);
	pin(4).set_dir(Link::INPUT); pin(4).set_name(perm_string::literal("clock0"), 0);
	pin(5).set_dir(Link::INPUT); pin(5).set_name(perm_string::literal("wren_b"), 0);
	pin(6).set_dir(Link::INPUT); pin(6).set_name(perm_string::literal("aclr1"), 0);
	pin(7).set_dir(Link::INPUT); pin(7).set_name(perm_string::literal("clock1"), 0);


	unsigned p = 8;
	for (unsigned idx = 0 ;  idx < width_address_a_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("address_a_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_address_b_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("address_b_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_data_a_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("data_a_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_data_b_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("data_b_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_q_a_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("q_a_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_q_b_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("q_b_"), idx);
	}
}

NetMem::~NetMem()
{
}

NetSquare::NetSquare(NetScope*sc, perm_string n,
	unsigned w_data, unsigned w_result)
	: NetNode(sc, n, 3 + w_data + w_result),
	width_data_(w_data), width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_data_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("data_"), idx);
	}
	pin(p).set_dir(Link::INPUT); pin(p).set_name(perm_string::literal("clken"), 0);
	pin(p+1).set_dir(Link::INPUT); pin(p+1).set_name(perm_string::literal("aclr"), 0);
	pin(p+2).set_dir(Link::INPUT); pin(p+2).set_name(perm_string::literal("clock"), 0);

	p += 3;
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

NetSquare::~NetSquare()
{
}

NetLMult::NetLMult(NetScope*sc, perm_string n,
	unsigned w_dataa, unsigned w_datab, unsigned w_sum, unsigned w_result)
	: NetNode(sc, n, 3 + w_dataa + w_datab + w_sum + w_result),
	width_dataa_(w_dataa), width_datab_(w_datab), width_sum_(w_sum), width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab_"), idx);
	}
	pin(p).set_dir(Link::INPUT); pin(p).set_name(perm_string::literal("clken"), 0);
	pin(p+1).set_dir(Link::INPUT); pin(p+1).set_name(perm_string::literal("aclr"), 0);
	pin(p+2).set_dir(Link::INPUT); pin(p+2).set_name(perm_string::literal("clock"), 0);

	p += 3;
	for (unsigned idx = 0 ;  idx < width_sum_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("sum_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

NetLMult::~NetLMult()
{
}

NetLMultSimpleOUT::NetLMultSimpleOUT(NetScope*sc, perm_string n,unsigned w_dataa, unsigned w_result)
	: NetNode(sc, n, w_dataa+w_result),
	width_dataa_(w_dataa), width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

NetLMultSimpleOUT::~NetLMultSimpleOUT()
{
}


NetMultAdd::NetMultAdd(NetScope*sc, perm_string n,
	unsigned w_dataa, unsigned w_datab, unsigned w_scanouta, unsigned w_scanoutb, unsigned w_result)
	: NetNode(sc, n, 14 + w_dataa + w_datab + w_scanouta + w_scanoutb + w_result),
	width_dataa_(w_dataa), width_datab_(w_datab), width_scanouta_(w_scanouta), width_scanoutb_(w_scanoutb), width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab_"), idx);
	}
	pin(p).set_dir(Link::INPUT); pin(p).set_name(perm_string::literal("clock0"), 0);
	pin(p+1).set_dir(Link::INPUT); pin(p+1).set_name(perm_string::literal("clock1"), 0);
	pin(p+2).set_dir(Link::INPUT); pin(p+2).set_name(perm_string::literal("clock2"), 0);
	pin(p+3).set_dir(Link::INPUT); pin(p+3).set_name(perm_string::literal("clock3"), 0);
	pin(p+4).set_dir(Link::INPUT); pin(p+4).set_name(perm_string::literal("aclr0"), 0);
	pin(p+5).set_dir(Link::INPUT); pin(p+5).set_name(perm_string::literal("aclr1"), 0);
	pin(p+6).set_dir(Link::INPUT); pin(p+6).set_name(perm_string::literal("aclr2"), 0);
	pin(p+7).set_dir(Link::INPUT); pin(p+7).set_name(perm_string::literal("aclr3"), 0);
	pin(p+8).set_dir(Link::INPUT); pin(p+8).set_name(perm_string::literal("ena0"), 0);
	pin(p+9).set_dir(Link::INPUT); pin(p+9).set_name(perm_string::literal("ena1"), 0);
	pin(p+10).set_dir(Link::INPUT); pin(p+10).set_name(perm_string::literal("ena2"), 0);
	pin(p+11).set_dir(Link::INPUT); pin(p+11).set_name(perm_string::literal("ena3"), 0);
	pin(p+12).set_dir(Link::INPUT); pin(p+12).set_name(perm_string::literal("signa"), 0);
	pin(p+13).set_dir(Link::INPUT); pin(p+13).set_name(perm_string::literal("signb"), 0);
	p += 14;
	for (unsigned idx = 0 ;  idx < width_scanouta_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("scanouta_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_scanoutb_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("scanoutb_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

NetMultAdd::~NetMultAdd()
{
}

NetMultAccum::NetMultAccum(NetScope*sc, perm_string n,
	unsigned w_dataa, unsigned w_datab, unsigned w_scanouta, unsigned w_scanoutb, unsigned w_result)
	: NetNode(sc, n, 14 + w_dataa + w_datab + w_scanouta + w_scanoutb + w_result),
	width_dataa_(w_dataa), width_datab_(w_datab), width_scanouta_(w_scanouta), width_scanoutb_(w_scanoutb), width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab_"), idx);
	}
	pin(p).set_dir(Link::INPUT); pin(p).set_name(perm_string::literal("accum_sload"), 0);
	pin(p+1).set_dir(Link::INPUT); pin(p+1).set_name(perm_string::literal("clock0"), 0);
	pin(p+2).set_dir(Link::INPUT); pin(p+2).set_name(perm_string::literal("clock1"), 0);
	pin(p+3).set_dir(Link::INPUT); pin(p+3).set_name(perm_string::literal("clock2"), 0);
	pin(p+4).set_dir(Link::INPUT); pin(p+4).set_name(perm_string::literal("clock3"), 0);
	pin(p+5).set_dir(Link::INPUT); pin(p+5).set_name(perm_string::literal("aclr0"), 0);
	pin(p+6).set_dir(Link::INPUT); pin(p+6).set_name(perm_string::literal("aclr1"), 0);
	pin(p+7).set_dir(Link::INPUT); pin(p+7).set_name(perm_string::literal("aclr2"), 0);
	pin(p+8).set_dir(Link::INPUT); pin(p+8).set_name(perm_string::literal("aclr3"), 0);
	pin(p+9).set_dir(Link::INPUT); pin(p+9).set_name(perm_string::literal("ena0"), 0);
	pin(p+10).set_dir(Link::INPUT); pin(p+10).set_name(perm_string::literal("ena1"), 0);
	pin(p+11).set_dir(Link::INPUT); pin(p+11).set_name(perm_string::literal("ena2"), 0);
	pin(p+12).set_dir(Link::INPUT); pin(p+12).set_name(perm_string::literal("ena3"), 0);

	p += 13;
	for (unsigned idx = 0 ;  idx < width_scanouta_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("scanouta_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_scanoutb_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("scanoutb_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
	pin(p).set_dir(Link::OUTPUT); pin(p).set_name(perm_string::literal("overflow"), 0);
}

NetMultAccum::~NetMultAccum(){
}

HalfDSPBlock::HalfDSPBlock(NetScope*sc, perm_string n,
	unsigned w_dataa0, unsigned w_datab0, unsigned w_dataa1, unsigned w_datab1, 
	unsigned w_dataa2, unsigned w_datab2, unsigned w_dataa3, unsigned w_datab3, 
	unsigned w_scanina, unsigned w_scanouta,
	unsigned w_chainin, unsigned w_chainout,
	unsigned w_clock, unsigned w_ena, unsigned w_aclr, 
	unsigned w_zeroloopback, 
	unsigned w_accumsload, 
	unsigned w_zerochainout, 
	unsigned w_chainoutround, 
	unsigned w_chainsaturate, 
	unsigned w_signa, 
	unsigned w_signb, 
	unsigned w_outputround, 
	unsigned w_outsaturate, 
	unsigned w_rotate, 
	unsigned w_shiftright, 
	unsigned w_OverFlow, 
	unsigned w_ChainOutSatOverFlow, 
	unsigned w_result) : NetNode(sc, n, 
						w_dataa0 + w_datab0 + w_dataa1 + w_datab1
						+w_dataa2 + w_datab2 + w_dataa3 +w_datab3
						+ w_scanina + w_scanouta 
						+ w_chainin + w_chainout 
						+ w_clock + w_ena +w_aclr
						+ w_zeroloopback +w_accumsload+w_zerochainout+w_chainoutround+w_chainsaturate+w_signa+w_signb+w_outputround+w_outsaturate+w_rotate+ w_shiftright
						+ w_OverFlow + w_ChainOutSatOverFlow
						+ w_result),
	width_dataa0_(w_dataa0), width_datab0_(w_datab0), width_dataa1_(w_dataa1), width_datab1_(w_datab1), 
	width_dataa2_(w_dataa2), width_datab2_(w_datab2), width_dataa3_(w_dataa3), width_datab3_(w_datab3), 
	width_scanina_(w_scanina), width_scanouta_(w_scanouta), 
	width_chainin_(w_chainin), width_chainout_ (w_chainout), 
	width_clock_(w_clock),width_ena_(w_ena),width_aclr_(w_aclr),
	width_zeroloopback_(w_zeroloopback), 
	width_accumsload_ (w_accumsload), 
	width_zerochainout_ (w_zerochainout), 
	width_chainoutround_ (w_chainoutround), 
	width_chainsaturate_ (w_chainsaturate), 
	width_signa_ (w_signa), 
	width_signb_ (w_signb), 
	width_outputround_ (w_outputround), 
	width_outsaturate_ (w_outsaturate), 
	width_rotate_ (w_rotate), 
	width_shiftright_ (w_shiftright), 
	width_OverFlow_ (w_OverFlow), 
	width_ChainOutSatOverFlow_ (w_ChainOutSatOverFlow), 
	width_result_(w_result){
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa0_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa0_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab0_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab0_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_dataa1_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa1_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab1_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab1_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_dataa2_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa2_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab2_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab2_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_dataa3_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa3_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab3_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab3_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_scanina_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("scanina_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_scanouta_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("scanouta_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_chainin_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("chainin_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_chainout_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("chainout_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_clock_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("clock_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_ena_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("ena_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_aclr_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("aclr_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_zeroloopback_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("zeroloopback_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_accumsload_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("accumsload_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_zerochainout_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("zerochainout_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_chainoutround_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("chainoutround_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_chainsaturate_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("chainsaturate_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_signa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("signa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_signb_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("signb_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_outputround_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("outputround_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_outsaturate_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("outsaturate_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_rotate_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("rotate_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_shiftright_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("shiftright_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_OverFlow_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("OverFlow_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_ChainOutSatOverFlow_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("ChainOutSatOverFlow_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

HalfDSPBlock::~HalfDSPBlock(){
}

unsigned HalfDSPBlock::width_dataa0() const
{
	return width_dataa0_;
}
unsigned HalfDSPBlock::width_datab0() const
{
	return width_datab0_;
}
unsigned HalfDSPBlock::width_dataa1() const
{
	return width_dataa1_;
}
unsigned HalfDSPBlock::width_datab1() const
{
	return width_datab1_;
}
unsigned HalfDSPBlock::width_dataa2() const
{
	return width_dataa2_;
}
unsigned HalfDSPBlock::width_datab2() const
{
	return width_datab2_;
}
unsigned HalfDSPBlock::width_dataa3() const
{
	return width_dataa3_;
}
unsigned HalfDSPBlock::width_datab3() const
{
	return width_datab3_;
}

unsigned HalfDSPBlock::width_scanina() const{
	return width_scanina_;
}
unsigned HalfDSPBlock::width_scanouta() const{
	return width_scanouta_;
}
unsigned HalfDSPBlock::width_chainin() const{
	return width_chainin_;
}
unsigned HalfDSPBlock::width_chainout() const{
	return width_chainout_;
}
unsigned HalfDSPBlock::width_clock() const{
	return width_clock_;
}
unsigned HalfDSPBlock::width_ena() const{
	return width_ena_;
}
unsigned HalfDSPBlock::width_aclr() const{
	return width_aclr_;
}
unsigned HalfDSPBlock::width_zeroloopback() const{
	return width_zeroloopback_;
}
unsigned HalfDSPBlock::width_accumsload() const{
	return width_accumsload_;
}
unsigned HalfDSPBlock::width_zerochainout() const{
	return width_zerochainout_;
}
unsigned HalfDSPBlock::width_chainoutround() const{
	return width_chainoutround_;
}
unsigned HalfDSPBlock::width_chainsaturate() const{
	return width_chainsaturate_;
}
unsigned HalfDSPBlock::width_signa() const{
	return width_signa_;
}
unsigned HalfDSPBlock::width_signb() const{
	return width_signb_;
}
unsigned HalfDSPBlock::width_outputround() const{
	return width_outputround_;
}
unsigned HalfDSPBlock::width_outsaturate() const{
	return width_outsaturate_;
}
unsigned HalfDSPBlock::width_rotate() const{
	return width_rotate_;
}
unsigned HalfDSPBlock::width_shiftright() const{
	return width_shiftright_;
}
unsigned HalfDSPBlock::width_OverFlow() const{
	return width_OverFlow_;
}
unsigned HalfDSPBlock::width_ChainOutSatOverFlow() const{
	return width_ChainOutSatOverFlow_;
}
unsigned HalfDSPBlock::width_result() const{
	return width_result_;
}

Link& HalfDSPBlock::pin_dataa0(unsigned idx)
{
	assert(idx < width_dataa0_);
	return pin(idx);
}
Link& HalfDSPBlock::pin_datab0(unsigned idx)
{
	assert(idx < width_datab0_);
	return pin(idx + width_dataa0_);
}
Link& HalfDSPBlock::pin_dataa1(unsigned idx)
{
	assert(idx < width_dataa1_);
	return pin(idx + width_dataa0_ + width_datab0_);
}
Link& HalfDSPBlock::pin_datab1(unsigned idx)
{
	assert(idx < width_datab1_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_);
}
Link& HalfDSPBlock::pin_dataa2(unsigned idx)
{
	assert(idx < width_dataa2_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_);
}
Link& HalfDSPBlock::pin_datab2(unsigned idx)
{
	assert(idx < width_datab2_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
						   +width_dataa2_);
}
Link& HalfDSPBlock::pin_dataa3(unsigned idx)
{
	assert(idx < width_dataa3_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
							+width_dataa2_ + width_datab2_ );
}
Link& HalfDSPBlock::pin_datab3(unsigned idx)
{
	assert(idx < width_datab3_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
							+width_dataa2_ + width_datab2_  + width_dataa3_);
}

Link& HalfDSPBlock::pin_scanina(unsigned idx)
{
	assert(idx < width_scanina_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_);
}
Link& HalfDSPBlock::pin_scanoua(unsigned idx)
{
	assert(idx < width_scanouta_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_);
}
Link& HalfDSPBlock::pin_chainin(unsigned idx)
{
	assert(idx < width_chainin_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ );
}
Link& HalfDSPBlock::pin_chainout(unsigned idx)
{
	assert(idx < width_chainout_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ );
}
Link& HalfDSPBlock::pin_clock(unsigned idx)
{
	assert(idx < width_clock_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_);
}
Link& HalfDSPBlock::pin_ena(unsigned idx)
{
	assert(idx < width_ena_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_);
}
Link& HalfDSPBlock::pin_aclr(unsigned idx)
{
	assert(idx < width_aclr_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		                   +width_ena_);
}
Link& HalfDSPBlock::pin_zeroloopback(unsigned idx)
{
	assert(idx < width_zeroloopback_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_);
}
Link& HalfDSPBlock::pin_accumsload(unsigned idx)
{
	assert(idx < width_accumsload_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ );
}
Link& HalfDSPBlock::pin_zerochainout(unsigned idx)
{
	assert(idx < width_zerochainout_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_);
}
Link& HalfDSPBlock::pin_chainoutround(unsigned idx)
{
	assert(idx < width_chainoutround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_);
}
Link& HalfDSPBlock::pin_chainsaturate(unsigned idx)
{
	assert(idx < width_chainsaturate_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ );
}
Link& HalfDSPBlock::pin_signa(unsigned idx) 
{
	assert(idx < width_signa_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_);
}
Link& HalfDSPBlock::pin_signb(unsigned idx)  
{
	assert(idx < width_signb_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_);
}
Link& HalfDSPBlock::pin_outputround(unsigned idx) 
{
	assert(idx < width_outputround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_);
}
Link& HalfDSPBlock::pin_outsaturate(unsigned idx) 
{
	assert(idx < width_outsaturate_);
	return pin(idx < width_outputround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ );
}
Link& HalfDSPBlock::pin_rotate(unsigned idx) 
{
	assert(idx < width_rotate_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_);
}
Link& HalfDSPBlock::pin_shiftright(unsigned idx) 
{
	assert(idx < width_shiftright_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_);
}
Link& HalfDSPBlock::pin_OverFlow(unsigned idx) 
{
	assert(idx < width_OverFlow_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_);
}
Link& HalfDSPBlock::pin_ChainOutSatOverFlow(unsigned idx) 
{
	assert(idx < width_ChainOutSatOverFlow_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_
		+width_OverFlow_);
}
Link& HalfDSPBlock::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_
		+width_OverFlow_ + width_ChainOutSatOverFlow_);
}

const Link& HalfDSPBlock::pin_dataa0(unsigned idx) const
{
	assert(idx < width_dataa0_);
	return pin(idx);
}
const Link& HalfDSPBlock::pin_datab0(unsigned idx) const
{
	assert(idx < width_datab0_);
	return pin(idx + width_dataa0_);
}
const Link& HalfDSPBlock::pin_dataa1(unsigned idx) const
{
	assert(idx < width_dataa1_);
	return pin(idx + width_dataa0_ + width_datab0_);
}
const Link& HalfDSPBlock::pin_datab1(unsigned idx) const
{
	assert(idx < width_datab1_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_);
}
const Link& HalfDSPBlock::pin_dataa2(unsigned idx) const
{
	assert(idx < width_dataa2_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_);
}
const Link& HalfDSPBlock::pin_datab2(unsigned idx) const
{
	assert(idx < width_datab2_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_);
}
const Link& HalfDSPBlock::pin_dataa3(unsigned idx) const
{
	assert(idx < width_dataa3_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_ );
}
const Link& HalfDSPBlock::pin_datab3(unsigned idx) const
{
	assert(idx < width_datab3_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_);
}

const Link& HalfDSPBlock::pin_scanina(unsigned idx) const
{
	assert(idx < width_scanina_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_);
}
const Link& HalfDSPBlock::pin_scanoua(unsigned idx) const
{
	assert(idx < width_scanouta_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_);
}
const Link& HalfDSPBlock::pin_chainin(unsigned idx) const
{
	assert(idx < width_chainin_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ );
}
const Link& HalfDSPBlock::pin_chainout(unsigned idx) const
{
	assert(idx < width_chainout_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ );
}
const Link& HalfDSPBlock::pin_clock(unsigned idx) const
{
	assert(idx < width_clock_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_);
}
const Link& HalfDSPBlock::pin_ena(unsigned idx) const
{
	assert(idx < width_ena_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_);
}
const Link& HalfDSPBlock::pin_aclr(unsigned idx) const
{
	assert(idx < width_aclr_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_);
}
const Link& HalfDSPBlock::pin_zeroloopback(unsigned idx) const
{
	assert(idx < width_zeroloopback_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_);
}
const Link& HalfDSPBlock::pin_accumsload(unsigned idx) const
{
	assert(idx < width_accumsload_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ );
}
const Link& HalfDSPBlock::pin_zerochainout(unsigned idx) const
{
	assert(idx < width_zerochainout_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_);
}
const Link& HalfDSPBlock::pin_chainoutround(unsigned idx) const
{
	assert(idx < width_chainoutround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_);
}
const Link& HalfDSPBlock::pin_chainsaturate(unsigned idx) const
{
	assert(idx < width_chainsaturate_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ );
}
const Link& HalfDSPBlock::pin_signa(unsigned idx) const
{
	assert(idx < width_signa_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_);
}
const Link& HalfDSPBlock::pin_signb(unsigned idx) const
{
	assert(idx < width_signb_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_);
}
const Link& HalfDSPBlock::pin_outputround(unsigned idx) const
{
	assert(idx < width_outputround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_);
}
const Link& HalfDSPBlock::pin_outsaturate(unsigned idx) const
{
	assert(idx < width_outsaturate_);
	return pin(idx < width_outputround_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ );
}
const Link& HalfDSPBlock::pin_rotate(unsigned idx) const
{
	assert(idx < width_rotate_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_);
}
const Link& HalfDSPBlock::pin_shiftright(unsigned idx) const
{
	assert(idx < width_shiftright_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_);
}
const Link& HalfDSPBlock::pin_OverFlow(unsigned idx) const
{
	assert(idx < width_OverFlow_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_);
}
const Link& HalfDSPBlock::pin_ChainOutSatOverFlow(unsigned idx) const
{
	assert(idx < width_ChainOutSatOverFlow_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_
		+width_OverFlow_);
}
const Link& HalfDSPBlock::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx + width_dataa0_ + width_datab0_ + width_dataa1_ + width_datab1_
		+width_dataa2_ + width_datab2_  + width_dataa3_ + width_datab3_+width_scanina_ + width_scanouta_ + width_chainin_ + width_chainout_ +width_clock_
		+width_ena_ +width_aclr_ + width_zeroloopback_ + width_accumsload_ + width_zerochainout_ + width_chainoutround_ + width_chainsaturate_
		+width_signa_ + width_signb_ + width_outputround_ + width_outsaturate_ +width_rotate_ + width_shiftright_
		+width_OverFlow_ + width_ChainOutSatOverFlow_);
}

void NetMult::set_signed(bool flag)
{
	signed_ = flag;
}

bool NetMult::get_signed() const
{
	return signed_;
}

unsigned NetMult::width_r() const
{
	return width_r_;
}

unsigned NetMult::width_a() const
{
	return width_a_;
}

unsigned NetMult::width_b() const
{
	return width_b_;
}

unsigned NetMult::width_s() const
{
	return width_s_;
}

Link& NetMult::pin_Aclr()
{
	return pin(0);
}

const Link& NetMult::pin_Aclr() const
{
	return pin(0);
}

Link& NetMult::pin_Clock()
{
	return pin(1);
}

const Link& NetMult::pin_Clock() const
{
	return pin(1);
}

Link& NetMult::pin_Result(unsigned idx)
{
	assert(idx < width_r_);
	return pin(idx+2);
}

const Link& NetMult::pin_Result(unsigned idx) const
{
	assert(idx < width_r_);
	return pin(idx+2);
}

Link& NetMult::pin_DataA(unsigned idx)
{
	assert(idx < width_a_);
	return pin(idx+2+width_r_);
}

const Link& NetMult::pin_DataA(unsigned idx) const
{
	assert(idx < width_a_);
	return pin(idx+2+width_r_);
}

Link& NetMult::pin_DataB(unsigned idx)
{
	assert(idx < width_b_);
	return pin(idx+2+width_r_+width_a_);
}

const Link& NetMult::pin_DataB(unsigned idx) const
{
	assert(idx < width_b_);
	return pin(idx+2+width_r_+width_a_);
}

Link& NetMult::pin_Sum(unsigned idx)
{
	assert(idx < width_s_);
	return pin(idx+2+width_r_+width_a_+width_b_);
}

const Link& NetMult::pin_Sum(unsigned idx) const
{
	assert(idx < width_s_);
	return pin(idx+2+width_r_+width_a_+width_b_);
}

unsigned NetMem::width_address_a() const
{
	return width_address_a_;
}

unsigned NetMem::width_address_b() const
{
	return width_address_b_;
}

unsigned NetMem::width_data_a() const
{
	return width_data_a_;
}

unsigned NetMem::width_data_b() const
{
	return width_data_b_;
}

unsigned NetMem::width_q_a() const
{
	return width_q_a_;
}

unsigned NetMem::width_q_b() const
{
	return width_q_b_;
}

Link& NetMem::pin_clocken0()
{
	return pin(0);
}

const Link& NetMem::pin_clocken0() const
{
	return pin(0);
}

Link& NetMem::pin_clocken1()
{
	return pin(1);
}

const Link& NetMem::pin_clocken1() const
{
	return pin(1);
}

Link& NetMem::pin_wren_a()
{
	return pin(2);
}

const Link& NetMem::pin_wren_a() const
{
	return pin(2);
}

Link& NetMem::pin_aclr0()
{
	return pin(3);
}

const Link& NetMem::pin_aclr0() const
{
	return pin(3);
}

Link& NetMem::pin_clock0()
{
	return pin(4);
}

const Link& NetMem::pin_clock0() const
{
	return pin(4);
}

Link& NetMem::pin_wren_b()
{
	return pin(5);
}

const Link& NetMem::pin_wren_b() const
{
	return pin(5);
}

Link& NetMem::pin_aclr1()
{
	return pin(6);
}

const Link& NetMem::pin_aclr1() const
{
	return pin(6);
}

Link& NetMem::pin_clock1()
{
	return pin(7);
}

const Link& NetMem::pin_clock1() const
{
	return pin(7);
}

Link& NetMem::pin_address_a(unsigned idx)
{
	assert(idx < width_address_a_);
	return pin(idx+8);
}

const Link& NetMem::pin_address_a(unsigned idx) const
{
	assert(idx < width_address_a_);
	return pin(idx+8);
}

Link& NetMem::pin_address_b(unsigned idx)
{
	assert(idx < width_address_b_);
	return pin(idx+8+width_address_a_);
}

const Link& NetMem::pin_address_b(unsigned idx) const
{
	assert(idx < width_address_b_);
	return pin(idx+8+width_address_a_);
}

Link& NetMem::pin_data_a(unsigned idx)
{
	assert(idx < width_data_a_);
	return pin(idx+8+width_address_a_+width_address_b_);
}

const Link& NetMem::pin_data_a(unsigned idx) const
{
	assert(idx < width_data_a_);
	return pin(idx+8+width_address_a_+width_address_b_);
}

Link& NetMem::pin_data_b(unsigned idx)
{
	assert(idx < width_data_b_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_);
}

const Link& NetMem::pin_data_b(unsigned idx) const
{
	assert(idx < width_data_b_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_);
}

Link& NetMem::pin_q_a(unsigned idx)
{
	assert(idx < width_q_a_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_+width_data_b_);
}

const Link& NetMem::pin_q_a(unsigned idx) const
{
	assert(idx < width_q_a_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_+width_data_b_);
}

Link& NetMem::pin_q_b(unsigned idx)
{
	assert(idx < width_q_b_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_+width_data_b_+width_q_a_);
}

const Link& NetMem::pin_q_b(unsigned idx) const
{
	assert(idx < width_q_b_);
	return pin(idx+8+width_address_a_+width_address_b_+width_data_a_+width_data_b_+width_q_a_);
}

unsigned NetSquare::width_data() const
{
	return width_data_;
}

unsigned NetSquare::width_result() const
{
	return width_result_;
}

Link& NetSquare::pin_data(unsigned idx)
{
	assert(idx < width_data_);
	return pin(idx);
}

const Link& NetSquare::pin_data(unsigned idx) const
{
	assert(idx < width_data_);
	return pin(idx);
}

Link& NetSquare::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx+3+width_data_);
}

const Link& NetSquare::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+3+width_data_);
}

Link& NetSquare::pin_clken()
{
	return pin(width_data_);
}

const Link& NetSquare::pin_clken() const
{
	return pin(width_data_);
}

Link& NetSquare::pin_aclr()
{
	return pin(1+width_data_);
}

const Link& NetSquare::pin_aclr() const
{
	return pin(1+width_data_);
}

Link& NetSquare::pin_clock()
{
	return pin(2+width_data_);
}

const Link& NetSquare::pin_clock() const
{
	return pin(2+width_data_);
}

unsigned NetLMult::width_dataa() const
{
	return width_dataa_;
}

unsigned NetLMult::width_datab() const
{
	return width_datab_;
}

unsigned NetLMult::width_sum() const
{
	return width_sum_;
}

unsigned NetLMult::width_result() const
{
	return width_result_;
}

unsigned NetLMultSimpleOUT::width_dataa() const
{
	return width_dataa_;
}

unsigned NetLMultSimpleOUT::width_result() const
{
	return width_result_;
}

Link& NetLMult::pin_dataa(unsigned idx)
{
	assert(idx < width_dataa_);
	return pin(idx);
}

const Link& NetLMult::pin_dataa(unsigned idx) const
{
	assert(idx < width_dataa_);
	return pin(idx);
}
Link& NetLMultSimpleOUT::pin_dataa(unsigned idx)
{
	assert(idx < width_dataa_);
	return pin(idx);
}

const Link& NetLMultSimpleOUT::pin_dataa(unsigned idx) const
{
	assert(idx < width_dataa_);
	return pin(idx);
}

Link& NetLMult::pin_datab(unsigned idx)
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

const Link& NetLMult::pin_datab(unsigned idx) const
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

Link& NetLMult::pin_sum(unsigned idx)
{
	assert(idx < width_sum_);
	return pin(idx+3+width_dataa_+width_datab_);
}

const Link& NetLMult::pin_sum(unsigned idx) const
{
	assert(idx < width_sum_);
	return pin(idx+3+width_dataa_+width_datab_);
}

Link& NetLMult::pin_result(unsigned idx)
{
		assert(idx < width_result_);
		return pin(idx+3+width_dataa_+width_datab_+width_sum_);
}

const Link& NetLMult::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+3+width_dataa_+width_datab_+width_sum_);
}

Link& NetLMultSimpleOUT::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx+width_dataa_);
}

const Link& NetLMultSimpleOUT::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+width_dataa_);
}

Link& NetLMult::pin_clken()
{
	return pin(width_dataa_+width_datab_);
}

const Link& NetLMult::pin_clken() const
{
	return pin(width_dataa_+width_datab_);
}

Link& NetLMult::pin_aclr()
{
	return pin(1+width_dataa_+width_datab_);
}

const Link& NetLMult::pin_aclr() const
{
	return pin(1+width_dataa_+width_datab_);
}

Link& NetLMult::pin_clock()
{
	return pin(2+width_dataa_+width_datab_);
}

const Link& NetLMult::pin_clock() const
{
	return pin(2+width_dataa_+width_datab_);
}

unsigned NetMultAdd::width_dataa() const
{
	return width_dataa_;
}

unsigned NetMultAdd::width_datab() const
{
	return width_datab_;
}

unsigned NetMultAdd::width_scanouta() const
{
	return width_scanouta_;
}

unsigned NetMultAdd::width_scanoutb() const
{
	return width_scanoutb_;
}

unsigned NetMultAdd::width_result() const
{
	return width_result_;
}

Link& NetMultAdd::pin_dataa(unsigned idx)
{
	assert(idx < width_dataa_);
	return pin(idx);
}

const Link& NetMultAdd::pin_dataa(unsigned idx) const
{
	assert(idx < width_dataa_);
	return pin(idx);
}

Link& NetMultAdd::pin_datab(unsigned idx)
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

const Link& NetMultAdd::pin_datab(unsigned idx) const
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

Link& NetMultAdd::pin_scanouta(unsigned idx)
{
	assert(idx < width_scanouta_);
	return pin(idx+14+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_scanouta(unsigned idx) const
{
	assert(idx < width_scanouta_);
	return pin(idx+14+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_scanoutb(unsigned idx)
{
	assert(idx < width_scanoutb_);
	return pin(idx+14+width_dataa_+width_datab_+width_scanouta_);
}

const Link& NetMultAdd::pin_scanoutb(unsigned idx) const
{
	assert(idx < width_scanoutb_);
	return pin(idx+14+width_dataa_+width_datab_+width_scanoutb_);
}

Link& NetMultAdd::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx+14+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_);
}

const Link& NetMultAdd::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+14+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_);
}

Link& NetMultAdd::pin_clock0()
{
	return pin(width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_clock0() const
{
	return pin(width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_clock1()
{
	return pin(1+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_clock1() const
{
	return pin(1+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_clock2()
{
	return pin(2+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_clock2() const
{
	return pin(2+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_clock3()
{
	return pin(3+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_clock3() const
{
	return pin(3+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_aclr0()
{
	return pin(4+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_aclr0() const
{
	return pin(4+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_aclr1()
{
	return pin(5+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_aclr1() const
{
	return pin(5+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_aclr2()
{
	return pin(6+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_aclr2() const
{
	return pin(6+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_aclr3()
{
	return pin(7+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_aclr3() const
{
	return pin(7+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_ena0()
{
	return pin(8+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_ena0() const
{
	return pin(8+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_ena1()
{
	return pin(9+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_ena1() const
{
	return pin(9+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_ena2()
{
	return pin(10+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_ena2() const
{
	return pin(10+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_ena3()
{
	return pin(11+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_ena3() const
{
	return pin(11+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_signa()
{
	return pin(12+width_dataa_+width_datab_);
}

const Link& NetMultAdd::pin_signa() const
{
	return pin(12+width_dataa_+width_datab_);
}

Link& NetMultAdd::pin_signb()
{
	return pin(13+width_dataa_+width_datab_);
}
const Link& NetMultAdd::pin_signb() const
{
	return pin(13+width_dataa_+width_datab_);
}

/*****************************/
NetMultAddOut::NetMultAddOut(NetScope*sc, perm_string n,
		unsigned w_dataa, unsigned w_datab, unsigned w_datac, unsigned w_datad, 
		unsigned w_addnsub,unsigned w_accumsload,unsigned w_result)
	: NetNode(sc, n, 14 + w_dataa + w_datab + w_datac + w_datad +w_addnsub+w_accumsload+ w_result),
	width_dataa_(w_dataa), width_datab_(w_datab), width_datac_(w_datac), width_datad_(w_datad), width_addnsub_(w_addnsub),width_accumsload_(w_accumsload),width_result_(w_result)
{
	unsigned p = 0;
	for (unsigned idx = 0 ;  idx < width_dataa_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("dataa_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datab_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datab_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datac_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datac_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_datad_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("datad_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_addnsub_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("addnsub_"), idx);
	}
	for (unsigned idx = 0 ;  idx < width_accumsload_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::INPUT);
		pin(p).set_name(perm_string::literal("accum_sload"), idx);
	}

	pin(p).set_dir(Link::INPUT); pin(p).set_name(perm_string::literal("clock0"), 0);
	pin(p+1).set_dir(Link::INPUT); pin(p+1).set_name(perm_string::literal("clock1"), 0);
	pin(p+2).set_dir(Link::INPUT); pin(p+2).set_name(perm_string::literal("clock2"), 0);
	pin(p+3).set_dir(Link::INPUT); pin(p+3).set_name(perm_string::literal("clock3"), 0);
	pin(p+4).set_dir(Link::INPUT); pin(p+4).set_name(perm_string::literal("aclr0"), 0);
	pin(p+5).set_dir(Link::INPUT); pin(p+5).set_name(perm_string::literal("aclr1"), 0);
	pin(p+6).set_dir(Link::INPUT); pin(p+6).set_name(perm_string::literal("aclr2"), 0);
	pin(p+7).set_dir(Link::INPUT); pin(p+7).set_name(perm_string::literal("aclr3"), 0);
	pin(p+8).set_dir(Link::INPUT); pin(p+8).set_name(perm_string::literal("ena0"), 0);
	pin(p+9).set_dir(Link::INPUT); pin(p+9).set_name(perm_string::literal("ena1"), 0);
	pin(p+10).set_dir(Link::INPUT); pin(p+10).set_name(perm_string::literal("ena2"), 0);
	pin(p+11).set_dir(Link::INPUT); pin(p+11).set_name(perm_string::literal("ena3"), 0);
	pin(p+12).set_dir(Link::INPUT); pin(p+12).set_name(perm_string::literal("signa"), 0);
	pin(p+13).set_dir(Link::INPUT); pin(p+13).set_name(perm_string::literal("signb"), 0);
	p += 14;

	for (unsigned idx = 0 ;  idx < width_result_ ;  idx += 1, p += 1) {
		pin(p).set_dir(Link::OUTPUT);
		pin(p).set_name(perm_string::literal("result_"), idx);
	}
}

NetMultAddOut::~NetMultAddOut()
{
}
unsigned NetMultAddOut::width_dataa() const
{
return width_dataa_;
}

unsigned NetMultAddOut::width_datab() const
{
return width_datab_;
}

unsigned NetMultAddOut::width_datac() const
{
return width_datac_;
}

unsigned NetMultAddOut::width_datad() const
{
return width_datad_;
}

unsigned NetMultAddOut::width_addnsub() const
{
	return width_addnsub_;
}

unsigned NetMultAddOut::width_accumsload() const
{
	return width_accumsload_;
}

unsigned NetMultAddOut::width_result() const
{
return width_result_;
}


Link& NetMultAddOut::pin_dataa(unsigned idx)
{
assert(idx < width_dataa_);
return pin(idx);
}

const Link& NetMultAddOut::pin_dataa(unsigned idx) const
{
assert(idx < width_dataa_);
return pin(idx);
}

Link& NetMultAddOut::pin_datab(unsigned idx)
{
assert(idx < width_datab_);
return pin(idx+width_dataa_);
}

const Link& NetMultAddOut::pin_datab(unsigned idx) const
{
assert(idx < width_datab_);
return pin(idx+width_dataa_);
}

Link& NetMultAddOut::pin_datac(unsigned idx)
{
assert(idx < width_datac_);
return pin(idx+width_dataa_+width_datab_);
}

const Link& NetMultAddOut::pin_datac(unsigned idx) const
{
assert(idx < width_datac_);
return pin(idx+width_dataa_+width_datab_);
}

Link& NetMultAddOut::pin_datad(unsigned idx)
{
assert(idx < width_datad_);
return pin(idx+width_dataa_+width_datab_+width_datac_);
}

const Link& NetMultAddOut::pin_datad(unsigned idx) const
{
assert(idx < width_datad_);
return pin(idx+width_dataa_+width_datab_+width_datac_);
}

Link& NetMultAddOut::pin_addnsub(unsigned idx)
{
	assert(idx < width_addnsub_);
	return pin(idx+width_dataa_+width_datab_+width_datac_+width_datad_);
}

const Link& NetMultAddOut::pin_addnsub(unsigned idx) const
{
	assert(idx < width_addnsub_);
	return pin(idx+width_dataa_+width_datab_+width_datac_+width_datad_);
}

Link& NetMultAddOut::pin_accumsload(unsigned idx)
{
	assert(idx < width_accumsload_);
	return pin(idx+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_);
}

const Link& NetMultAddOut::pin_accumsload(unsigned idx) const
{
	assert(idx < width_accumsload_);
	return pin(idx+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_);
}

Link& NetMultAddOut::pin_clock0()
{
return pin(width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_clock0() const
{
return pin(width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_clock1()
{
return pin(1+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_clock1() const
{
return pin(1+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_clock2()
{
return pin(2+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_clock2() const
{
return pin(2+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_clock3()
{
return pin(3+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_clock3() const
{
return pin(3+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_aclr0()
{
return pin(4+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_aclr0() const
{
return pin(4+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_aclr1()
{
return pin(5+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_aclr1() const
{
return pin(5+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_aclr2()
{
return pin(6+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_aclr2() const
{
return pin(6+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_aclr3()
{
return pin(7+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_aclr3() const
{
return pin(7+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_ena0()
{
return pin(8+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_ena0() const
{
return pin(8+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_ena1()
{
return pin(9+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_ena1() const
{
return pin(9+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_ena2()
{
return pin(10+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}
const Link& NetMultAddOut::pin_ena2() const
{
return pin(10+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_ena3()
{
return pin(11+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_ena3() const
{
return pin(11+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_signa()
{
return pin(12+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_signa() const
{
return pin(12+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

Link& NetMultAddOut::pin_signb()
{
return pin(13+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}
const Link& NetMultAddOut::pin_signb() const
{
return pin(13+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}
Link& NetMultAddOut::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx+14+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}

const Link& NetMultAddOut::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+14+width_dataa_+width_datab_+width_datac_+width_datad_+width_addnsub_+width_accumsload_);
}
/*****************************/

unsigned NetMultAccum::width_dataa() const
{
	return width_dataa_;
}

unsigned NetMultAccum::width_datab() const
{
	return width_datab_;
}

unsigned NetMultAccum::width_scanouta() const
{
	return width_scanouta_;
}

unsigned NetMultAccum::width_scanoutb() const
{
	return width_scanoutb_;
}

unsigned NetMultAccum::width_result() const
{
	return width_result_;
}

Link& NetMultAccum::pin_dataa(unsigned idx)
{
	assert(idx < width_dataa_);
	return pin(idx);
}

const Link& NetMultAccum::pin_dataa(unsigned idx) const
{
	assert(idx < width_dataa_);
	return pin(idx);
}

Link& NetMultAccum::pin_datab(unsigned idx)
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

const Link& NetMultAccum::pin_datab(unsigned idx) const
{
	assert(idx < width_datab_);
	return pin(idx+width_dataa_);
}

Link& NetMultAccum::pin_scanouta(unsigned idx)
{
	assert(idx < width_scanouta_);
	return pin(idx+13+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_scanouta(unsigned idx) const
{
	assert(idx < width_scanouta_);
	return pin(idx+13+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_scanoutb(unsigned idx)
{
	assert(idx < width_scanoutb_);
	return pin(idx+13+width_dataa_+width_datab_+width_scanouta_);
}

const Link& NetMultAccum::pin_scanoutb(unsigned idx) const
{
	assert(idx < width_scanoutb_);
	return pin(idx+13+width_dataa_+width_datab_+width_scanoutb_);
}

Link& NetMultAccum::pin_result(unsigned idx)
{
	assert(idx < width_result_);
	return pin(idx+13+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_);
}

const Link& NetMultAccum::pin_result(unsigned idx) const
{
	assert(idx < width_result_);
	return pin(idx+13+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_);
}

Link& NetMultAccum::pin_accum_sload()
{
	return pin(width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_accum_sload() const
{
	return pin(width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_clock0()
{
	return pin(1+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_clock0() const
{
	return pin(1+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_clock1()
{
	return pin(2+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_clock1() const
{
	return pin(2+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_clock2()
{
	return pin(3+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_clock2() const
{
	return pin(3+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_clock3()
{
	return pin(4+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_clock3() const
{
	return pin(4+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_aclr0()
{
	return pin(5+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_aclr0() const
{
	return pin(5+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_aclr1()
{
	return pin(6+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_aclr1() const
{
	return pin(6+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_aclr2()
{
	return pin(7+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_aclr2() const
{
	return pin(7+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_aclr3()
{
	return pin(8+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_aclr3() const
{
	return pin(8+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_ena0()
{
	return pin(9+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_ena0() const
{
	return pin(9+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_ena1()
{
	return pin(10+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_ena1() const
{
	return pin(10+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_ena2()
{
	return pin(11+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_ena2() const
{
	return pin(11+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_ena3()
{
	return pin(12+width_dataa_+width_datab_);
}

const Link& NetMultAccum::pin_ena3() const
{
	return pin(12+width_dataa_+width_datab_);
}

Link& NetMultAccum::pin_overflow()
{
	return pin(13+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_+width_result_);
}

const Link& NetMultAccum::pin_overflow() const
{
	return pin(13+width_dataa_+width_datab_+width_scanouta_+width_scanoutb_+width_result_);
}

/*
* The NetMux class represents an LPM_MUX device. The pinout is assigned
* like so:
*    0   -- Aclr (optional)
*    1   -- Clock (optional)
*    2   -- Result[0]
*    2+N -- Result[N]
*/

NetMux::NetMux(NetScope*s, perm_string n,
	unsigned wi, unsigned si, unsigned sw)
	: NetNode(s, n, 2+wi+sw+wi*si),
	width_(wi), size_(si), swidth_(sw)
{
	if(wi == 232){
		cout<< "mux width = 232";
		//assert(0);
	}
	
	pin(0).set_dir(Link::INPUT); pin(0).set_name(perm_string::literal("Aclr"),  0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("Clock"), 0);

	for (unsigned idx = 0 ;  idx < width_ ;  idx += 1) {
		pin_Result(idx).set_dir(Link::OUTPUT);
		pin_Result(idx).set_name(perm_string::literal("Result"), idx);

		for (unsigned jdx = 0 ;  jdx < size_ ;  jdx += 1) {
			pin_Data(idx,jdx).set_dir(Link::INPUT);
			pin_Data(idx,jdx).set_name(perm_string::literal("Data"), jdx*width_+idx);
		}
	}

	for (unsigned idx = 0 ;  idx < swidth_ ;  idx += 1) {
		pin_Sel(idx).set_dir(Link::INPUT);
		pin_Sel(idx).set_name(perm_string::literal("Sel"), idx);
	}
}

NetMux::~NetMux()
{
}

unsigned NetMux::width()const
{
	return width_;
}

unsigned NetMux::size() const
{
	return size_;
}

unsigned NetMux::sel_width() const
{
	return swidth_;
}

Link& NetMux::pin_Aclr()
{
	return pin(0);
}

const Link& NetMux::pin_Aclr() const
{
	return pin(0);
}

Link& NetMux::pin_Clock()
{
	return pin(1);
}

const Link& NetMux::pin_Clock() const
{
	return pin(1);
}

Link& NetMux::pin_Result(unsigned w)
{
	assert(w < width_);
	return pin(2+w);
}

const Link& NetMux::pin_Result(unsigned w) const
{
	assert(w < width_);
	return pin(2+w);
}

Link& NetMux::pin_Sel(unsigned w)
{
	assert(w < swidth_);
	return pin(2+width_+w);
}

const Link& NetMux::pin_Sel(unsigned w) const
{
	assert(w < swidth_);
	return pin(2+width_+w);
}

Link& NetMux::pin_Data(unsigned w, unsigned s)
{
	assert(w < width_);
	assert(s < size_);
	return pin(2+width_+swidth_+s*width_+w);
}

const Link& NetMux::pin_Data(unsigned w, unsigned s) const
{
	assert(w < width_);
	assert(s < size_);
	return pin(2+width_+swidth_+s*width_+w);
}

void NetRamDq::make_pins_(unsigned wid)
{
	pin(0).set_dir(Link::INPUT); pin(0).set_name(perm_string::literal("InClock"), 0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("OutClock"), 0);
	pin(2).set_dir(Link::INPUT); pin(2).set_name(perm_string::literal("WE"), 0);

	for (unsigned idx = 0 ;  idx < awidth_ ;  idx += 1) {
		pin(3+idx).set_dir(Link::INPUT);
		pin(3+idx).set_name(perm_string::literal("RAddress"), idx);
	}

	for (unsigned idx = 0 ;  idx < awidth_ ;  idx += 1) {
		pin(3+awidth_+idx).set_dir(Link::INPUT);
		pin(3+awidth_+idx).set_name(perm_string::literal("WAddress"), idx);
	}

	for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
		pin(3+2*awidth_+idx).set_dir(Link::INPUT);
		pin(3+2*awidth_+idx).set_name(perm_string::literal("Data"), idx);
	}

	for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
		pin(3+2*awidth_+wid+idx).set_dir(Link::OUTPUT);
		pin(3+2*awidth_+wid+idx).set_name(perm_string::literal("Q"), idx);
	}
}


NetRamDq::NetRamDq(NetScope*s, perm_string n, NetMemory*mem, unsigned awid)
	: NetNode(s, n, 3+2*mem->width()+2*awid),
	mem_(mem), awidth_(awid)
{
	make_pins_(mem->width());
	next_ = mem_->ram_list_;
	mem_->ram_list_ = this;
}

NetRamDq::~NetRamDq()
{
	if (mem_) {
		if (mem_->ram_list_ == this) {
			mem_->ram_list_ = next_;

		} else {
			NetRamDq*cur = mem_->ram_list_;
			while (cur->next_ != this) {
				assert(cur->next_);
				cur = cur->next_;
			}
			assert(cur->next_ == this);
			cur->next_ = next_;
		}
	}
}

unsigned NetRamDq::width() const
{
	if (mem_) return mem_->width();
	return 0;
}

unsigned NetRamDq::awidth() const
{
	return awidth_;
}

unsigned NetRamDq::size() const
{
	if (mem_) return mem_->count();
	return 0;
}

NetMemory* NetRamDq::mem()
{
	return mem_;
}

const NetMemory* NetRamDq::mem() const
{
	return mem_;
}

unsigned NetRamDq::count_partners() const
{
	unsigned count = 0;
	for (NetRamDq*cur = mem_->ram_list_ ;  cur ;  cur = cur->next_)
		count += 1;

	return count;
}
//暂时注释
//void NetRamDq::absorb_partners()
//{
//	NetRamDq*cur, *tmp;
//	for (cur = mem_->ram_list_, tmp = 0
//		;  cur||tmp ;  cur = cur? cur->next_ : tmp) {
//			tmp = 0;
//			if (cur == this) continue;
//
//			bool ok_flag = true;
//			for (unsigned idx = 0 ;  idx < awidth() ;  idx += 1)
//				ok_flag &= pin_Address(idx).is_linked(cur->pin_Address(idx));
//
//			if (!ok_flag) continue;
//
//			if (pin_InClock().is_linked()
//				&& cur->pin_InClock().is_linked()
//				&& ! pin_InClock().is_linked(cur->pin_InClock()))
//				continue;
//
//			if (pin_OutClock().is_linked()
//				&& cur->pin_OutClock().is_linked()
//				&& ! pin_OutClock().is_linked(cur->pin_OutClock()))
//				continue;
//
//			if (pin_WE().is_linked()
//				&& cur->pin_WE().is_linked()
//				&& ! pin_WE().is_linked(cur->pin_WE()))
//				continue;
//
//			for (unsigned idx = 0 ;  idx < width() ;  idx += 1) {
//				if (!pin_Data(idx).is_linked()) continue;
//				if (! cur->pin_Data(idx).is_linked()) continue;
//
//				ok_flag &= pin_Data(idx).is_linked(cur->pin_Data(idx));
//			}
//
//			if (! ok_flag) continue;
//
//			for (unsigned idx = 0 ;  idx < width() ;  idx += 1) {
//				if (!pin_Q(idx).is_linked()) continue;
//				if (! cur->pin_Q(idx).is_linked()) continue;
//
//				ok_flag &= pin_Q(idx).is_linked(cur->pin_Q(idx));
//			}
//
//			if (! ok_flag) continue;
//
//			// I see no other reason to reject cur, so link up all
//			// my pins and delete it.
//			connect(pin_InClock(), cur->pin_InClock());
//			connect(pin_OutClock(), cur->pin_OutClock());
//			connect(pin_WE(), cur->pin_WE());
//
//			for (unsigned idx = 0 ;  idx < awidth() ;  idx += 1)
//				connect(pin_Address(idx), cur->pin_Address(idx));
//
//			for (unsigned idx = 0 ;  idx < width() ;  idx += 1) {
//				connect(pin_Data(idx), cur->pin_Data(idx));
//				connect(pin_Q(idx), cur->pin_Q(idx));
//			}
//
//			tmp = cur->next_;
//			delete cur;
//			cur = 0;
//	}
//}

Link& NetRamDq::pin_InClock()
{
	return pin(0);
}

const Link& NetRamDq::pin_InClock() const
{
	return pin(0);
}

Link& NetRamDq::pin_OutClock()
{
	return pin(1);
}

const Link& NetRamDq::pin_OutClock() const
{
	return pin(1);
}

Link& NetRamDq::pin_WE()
{
	return pin(2);
}

const Link& NetRamDq::pin_WE() const
{
	return pin(2);
}

Link& NetRamDq::pin_RAddress(unsigned idx)
{
	assert(idx < awidth_);
	return pin(3+idx);
}

const Link& NetRamDq::pin_RAddress(unsigned idx) const
{
	assert(idx < awidth_);
	return pin(3+idx);
}

Link& NetRamDq::pin_WAddress(unsigned idx)
{
	assert(idx < awidth_);
	return pin(3+awidth_+idx);
}

const Link& NetRamDq::pin_WAddress(unsigned idx) const
{
	assert(idx < awidth_);
	return pin(3+awidth_+idx);
}

Link& NetRamDq::pin_Data(unsigned idx)
{
	assert(idx < width());
	return pin(3+2*awidth_+idx);
}

const Link& NetRamDq::pin_Data(unsigned idx) const
{
	assert(idx < width());
	return pin(3+2*awidth_+idx);
}

Link& NetRamDq::pin_Q(unsigned idx)
{
	assert(idx < width());
	return pin(3+2*awidth_+width()+idx);
}

const Link& NetRamDq::pin_Q(unsigned idx) const
{
	assert(idx < width());
	return pin(3+2*awidth_+width()+idx);
}

NetBUFZ::NetBUFZ(NetScope*s, perm_string n)
	: NetNode(s, n, 2)
{
	pin(0).set_dir(Link::OUTPUT);
	pin(1).set_dir(Link::INPUT);
	pin(0).set_name(perm_string::literal("O"), 0);
	pin(1).set_name(perm_string::literal("I"), 0);
}

NetBUFZ::~NetBUFZ()
{
}


NetCaseCmp::NetCaseCmp(NetScope*s, perm_string n)
	: NetNode(s, n, 3)
{
	pin(0).set_dir(Link::OUTPUT); pin(0).set_name(perm_string::literal("O"),0);
	pin(1).set_dir(Link::INPUT); pin(1).set_name(perm_string::literal("I"),0);
	pin(2).set_dir(Link::INPUT); pin(2).set_name(perm_string::literal("I"),1);
}

NetCaseCmp::~NetCaseCmp()
{
}

NetCondit::NetCondit(NetExpr*ex, NetProc*i, NetProc*e)
	: expr_(ex), if_(i), else_(e)
{
}

NetCondit::~NetCondit()
{
	delete expr_;
	if (if_) delete if_;
	if (else_) delete else_;
}

const NetExpr* NetCondit::expr() const
{
	return expr_;
}

NetExpr* NetCondit::expr()
{
	return expr_;
}

void NetCondit::set_expr(NetExpr*ex)
{
	delete expr_;
	expr_ = ex;
}

NetProc* NetCondit::if_clause()
{
	return if_;
}

NetProc* NetCondit::else_clause()
{
	return else_;
}

NetConst::NetConst(NetScope*s, perm_string n, verinum::V v)
	: NetNode(s, n, 1)
{
	pin(0).set_dir(Link::OUTPUT);
	pin(0).set_name(perm_string::literal("O"), 0);
	value_ = new verinum::V[1];
	value_[0] = v;
}

NetConst::NetConst(NetScope*s, perm_string n, const verinum&val)
	: NetNode(s, n, val.len())
{
	value_ = new verinum::V[pin_count()];
	for (unsigned idx = 0 ;  idx < pin_count() ;  idx += 1) {
		pin(idx).set_dir(Link::OUTPUT);
		pin(idx).set_name(perm_string::literal("O"), idx);
		value_[idx] = val.get(idx);
	}
}

NetConst::~NetConst()
{
	delete[]value_;
}

verinum::V NetConst::value(unsigned idx) const
{
	assert(idx < pin_count());
	return value_[idx];
}

NetFuncDef::NetFuncDef(NetScope*s, NetNet*result, const svector<NetNet*>&po)
	: scope_(s), statement_(0), result_sig_(result), result_var_(0), ports_(po)
{
}

NetFuncDef::NetFuncDef(NetScope*s, NetVariable*result, const svector<NetNet*>&po)
	: scope_(s), statement_(0), result_sig_(0), result_var_(result), ports_(po)
{
}

NetFuncDef::~NetFuncDef()
{
}

const string NetFuncDef::name() const
{
	return scope_->name();
}

void NetFuncDef::set_proc(NetProc*st)
{
	assert(statement_ == 0);
	assert(st != 0);
	statement_ = st;
}

const NetProc* NetFuncDef::proc() const
{
	return statement_;
}

NetScope*NetFuncDef::scope()
{
	return scope_;
}

unsigned NetFuncDef::port_count() const
{
	return ports_.count();
}

const NetNet* NetFuncDef::port(unsigned idx) const
{
	assert(idx < ports_.count());
	return ports_[idx];
}

const NetNet* NetFuncDef::return_sig() const
{
	return result_sig_;
}

const NetVariable* NetFuncDef::return_var() const
{
	return result_var_;
}

NetSTask::NetSTask(const char*na, const svector<NetExpr*>&pa)
	: name_(0), parms_(pa)
{
	name_ = lex_strings.add(na);
	assert(name_[0] == '$');
}

NetSTask::~NetSTask()
{
	for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1)
		delete parms_[idx];

	/* The name_ string is perm-allocated in lex_strings. */
}

const char*NetSTask::name() const
{
	return name_;
}

unsigned NetSTask::nparms() const
{
	return parms_.count();
}

const NetExpr* NetSTask::parm(unsigned idx) const
{
	return parms_[idx];
}

NetEUFunc::NetEUFunc(NetScope*def, NetESignal*res, svector<NetExpr*>&p)
	: func_(def), result_sig_(res), result_var_(0), parms_(p)
{
	expr_width(result_sig_->expr_width());
}

NetEUFunc::NetEUFunc(NetScope*def, NetEVariable*res, svector<NetExpr*>&p)
	: func_(def), result_sig_(0), result_var_(res), parms_(p)
{
}

NetEUFunc::~NetEUFunc()
{
	for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1)
		delete parms_[idx];
}

const string NetEUFunc::name() const
{
	return func_->name();
}

const NetESignal*NetEUFunc::result_sig() const
{
	return result_sig_;
}

const NetEVariable*NetEUFunc::result_var() const
{
	return result_var_;
}

unsigned NetEUFunc::parm_count() const
{
	return parms_.count();
}

const NetExpr* NetEUFunc::parm(unsigned idx) const
{
	assert(idx < parms_.count());
	return parms_[idx];
}

const NetScope* NetEUFunc::func() const
{
	return func_;
}

NetExpr::TYPE NetEUFunc::expr_type() const
{
	if (result_sig_)
		return result_sig_->expr_type();
	if (result_var_)
		return result_var_->expr_type();

	return ET_VOID;
}

NetUTask::NetUTask(NetScope*def)
	: task_(def)
{
}

NetUTask::~NetUTask()
{
}

const string NetUTask::name() const
{
	return task_->name();
}

const NetScope* NetUTask::task() const
{
	return task_;
}

NetExpr::NetExpr(unsigned w)
	: width_(w), signed_flag_(false)
{
}

NetExpr::~NetExpr()
{
}

bool NetExpr::has_sign() const
{
	return signed_flag_;
}

void NetExpr::cast_signed(bool flag)
{
	signed_flag_ = flag;
}

bool NetExpr::has_width() const
{
	return true;
}

/*
* Create a bitwise operator node from the opcode and the left and
* right expressions. Don't worry about the width of the expression
* yet, we'll get that from the l-value, whatever that turns out to
* be. However, if we don't, our default will be the width of the
* largest operand.
*/
NetEBBits::NetEBBits(char op, NetExpr*l, NetExpr*r)
	: NetEBinary(op, l, r)
{
	if (r->expr_width() > l->expr_width())
		expr_width(r->expr_width());
	else
		expr_width(l->expr_width());
}

NetEBBits::~NetEBBits()
{
}

NetEBBits* NetEBBits::dup_expr() const
{
	NetEBBits*result = new NetEBBits(op_, left_->dup_expr(),
		right_->dup_expr());
	return result;
}

NetEBinary::NetEBinary(char op, NetExpr*l, NetExpr*r)
	: op_(op), left_(l), right_(r)
{
}

NetEBinary::~NetEBinary()
{
	delete left_;
	delete right_;
}

bool NetEBinary::has_width() const
{
	return left_->has_width() && right_->has_width();
}

NetEBinary* NetEBinary::dup_expr() const
{
	assert(0);
	return 0;
}

NetEBLogic::NetEBLogic(char op, NetExpr*l, NetExpr*r)
	: NetEBinary(op, l, r)
{
	expr_width(1);
}

NetEBLogic::~NetEBLogic()
{
}

NetEBLogic* NetEBLogic::dup_expr() const
{
	NetEBLogic*result = new NetEBLogic(op_, left_->dup_expr(),
		right_->dup_expr());
	return result;
}

NetEConst::NetEConst(const verinum&val)
	: NetExpr(val.len()), value_(val)
{
	cast_signed(value_.has_sign());
}

NetEConst::~NetEConst()
{
}

const verinum& NetEConst::value() const
{
	return value_;
}

bool NetEConst::has_width() const
{
	return value_.has_len();
}

NetEConstParam::NetEConstParam(NetScope*s, perm_string n, const verinum&v)
	: NetEConst(v), scope_(s), name_(n)
{
}

NetEConstParam::~NetEConstParam()
{
}

perm_string NetEConstParam::name() const
{
	return name_;
}

const NetScope* NetEConstParam::scope() const
{
	return scope_;
}


NetEMemory::NetEMemory(NetMemory*m, NetExpr*i)
	: NetExpr(m->width()), mem_(m), idx_(i)
{
}

NetEMemory::~NetEMemory()
{
}

perm_string NetEMemory::name() const
{
	return mem_->name();
}

const NetExpr* NetEMemory::index() const
{
	return idx_;
}

NetMemory::NetMemory(NetScope*sc, perm_string n, long w, long s, long e)
	: width_(w), idxh_(s), idxl_(e), ram_list_(0), scope_(sc)
{
	name_ = n;
	explode_ = 0;
	mem_ref_ = 0;
	scope_->add_memory(this);
}

NetMemory::~NetMemory()
{
	assert(scope_);
	scope_->rem_memory(this);
}

unsigned NetMemory::count() const
{
	if (idxh_ < idxl_)
		return idxl_ - idxh_ + 1;
	else
		return idxh_ - idxl_ + 1;
}

perm_string NetMemory::name() const
{
	return name_;
}

unsigned NetMemory::index_to_address(long idx) const
{
	if (idxh_ < idxl_)
		return idx - idxh_;
	else
		return idx - idxl_;
}

NetNet* NetMemory::explode_to_reg()
{
	if (explode_)
		return explode_;

	explode_ = new NetNet(scope_, name_, NetNet::REG, count()*width_);
	explode_->local_flag(true);
	//explode_->incr_lref();
	return explode_;
}

NetNet* NetMemory::reg_from_explode()
{
	return explode_;
}

const NetNet* NetMemory::reg_from_explode() const
{
	return explode_;
}

NetRamDq * NetMemory:: get_ramlist()
{
	return ram_list_;
}


NetEMemory* NetEMemory::dup_expr() const
{
	assert(0);
	return 0;
}


NetEEvent::NetEEvent(NetEvent*e)
	: event_(e)
{
	e->exprref_ += 1;
}

NetEEvent::~NetEEvent()
{
}

const NetEvent* NetEEvent::event() const
{
	return event_;
}

NetEScope::NetEScope(NetScope*s)
	: scope_(s)
{
}

NetEScope::~NetEScope()
{
}

const NetScope* NetEScope::scope() const
{
	return scope_;
}

NetESignal::NetESignal(NetNet*n)
	: NetExpr(n->pin_count()), net_(n)
{
	msi_ = n->pin_count() - 1;
	lsi_ = 0;
	net_->incr_eref();
	set_line(*n);
	cast_signed(net_->get_signed());
}

NetESignal::NetESignal(NetNet*n, unsigned m, unsigned l)
	: NetExpr(m - l + 1), net_(n)
{
	assert(m >= l);
	msi_ = m;
	lsi_ = l;
	net_->incr_eref();
	set_line(*n);
	cast_signed(net_->get_signed());
}

NetESignal::~NetESignal()
{
	net_->decr_eref();
}

perm_string NetESignal::name() const
{
	return net_->name();
}

unsigned NetESignal::bit_count() const
{
	return msi_ - lsi_ + 1;
}

Link& NetESignal::bit(unsigned idx)
{
	assert(idx <= (msi_ - lsi_));
	return net_->pin(idx + lsi_);
}

const NetNet* NetESignal::sig() const
{
	return net_;
}

unsigned NetESignal::lsi() const
{
	return lsi_;
}

unsigned NetESignal::msi() const
{
	return msi_;
}

NetEBitSel::NetEBitSel(NetESignal*sig, NetExpr*ex)
	: sig_(sig), idx_(ex)
{
	// This supports mux type indexing of an expression, so the
	// with is by definition 1 bit.
	expr_width(1);
}

NetEBitSel::~NetEBitSel()
{
	delete idx_;
}

perm_string NetEBitSel::name() const
{
	return sig_->name();
}

const NetNet* NetEBitSel::sig() const
{
	return sig_->sig();
}

NetEBitSel* NetEBitSel::dup_expr() const
{
	assert(0);
	return 0;
}

NetETernary::NetETernary(NetExpr*c, NetExpr*t, NetExpr*f)
	: cond_(c), true_val_(t), false_val_(f)
{
	// use widest result
	if (true_val_->expr_width() > false_val_->expr_width())
		expr_width(true_val_->expr_width());
	else
		expr_width(false_val_->expr_width());
	cast_signed(c->has_sign() && t->has_sign() && f->has_sign());
}

NetETernary::~NetETernary()
{
	delete cond_;
	delete true_val_;
	delete false_val_;
}

const NetExpr* NetETernary::cond_expr() const
{
	return cond_;
}

const NetExpr* NetETernary::true_expr() const
{
	return true_val_;
}

const NetExpr* NetETernary::false_expr() const
{
	return false_val_;
}

NetEUnary::NetEUnary(char op, NetExpr*ex)
	: NetExpr(ex->expr_width()), op_(op), expr_(ex)
{
	switch (op_) {
	case '!':
		expr_width(1);
		break;
	}
	switch (op_) {
	case '-':
	case '+':
		cast_signed(ex->has_sign());
		break;
	default:
		;
	}
}

NetEUnary::~NetEUnary()
{
	delete expr_;
}

NetEUBits::NetEUBits(char op, NetExpr*ex)
	: NetEUnary(op, ex)
{
}

NetEUBits::~NetEUBits()
{
}

NetEUReduce::NetEUReduce(char op, NetExpr*ex)
	: NetEUnary(op, ex)
{
	expr_width(1);
}

NetEUReduce::~NetEUReduce()
{
}

NetLogic::NetLogic(NetScope*s, perm_string n, unsigned pins, TYPE t)
	: NetNode(s, n, pins), type_(t)
{
	if ( t == PLL ) {
		for (unsigned idx = 0 ;  idx < 4 ;  idx += 1) {
			pin(idx).set_dir(Link::INPUT);
			pin(idx).set_name(perm_string::literal("I"), idx);
		}
		for (unsigned idx = 4 ;  idx < 8 ;  idx += 1) {
			pin(idx).set_dir(Link::OUTPUT);
			pin(idx).set_name(perm_string::literal("O"), idx-4);
		}
	}
	else {
		pin(0).set_dir(Link::OUTPUT);
		pin(0).set_name(perm_string::literal("O"), 0);
		for (unsigned idx = 1 ;  idx < pins ;  idx += 1) {
			pin(idx).set_dir(Link::INPUT);
			pin(idx).set_name(perm_string::literal("I"), idx-1);
		}
	}
}

NetTaskDef::NetTaskDef(const string&n, const svector<NetNet*>&po)
	: name_(n), proc_(0), ports_(po)
{
}

NetTaskDef::~NetTaskDef()
{
	delete proc_;
}

void NetTaskDef::set_proc(NetProc*p)
{
	assert(proc_ == 0);
	proc_ = p;
}

unsigned NetTaskDef::port_count() const
{
	return ports_.count();
}

NetNet* NetTaskDef::port(unsigned idx)
{
	assert(idx < ports_.count());
	return ports_[idx];
}

const string& NetTaskDef::name() const
{
	return name_;
}

const NetProc*NetTaskDef::proc() const
{
	return proc_;
}

/*
* $Log: netlist.cc,v $
* Revision 1.226.2.7  2006/04/23 04:26:14  steve
*  Constant propagate addresses through NetRamDq read ports.
*
* Revision 1.226.2.6  2006/04/16 19:26:38  steve
*  Fix handling of exploded memories with partial or missing resets.
*
* Revision 1.226.2.5  2006/03/26 23:09:22  steve
*  Handle asynchronous demux/bit replacements.
*
* Revision 1.226.2.4  2006/03/16 05:40:18  steve
*  Fix crash when memory exploding doesnot work
*
* Revision 1.226.2.3  2006/03/12 07:34:17  steve
*  Fix the memsynth1 case.
*
* Revision 1.226.2.2  2006/02/19 00:11:32  steve
*  Handle synthesis of FF vectors with l-value decoder.
*
* Revision 1.226.2.1  2006/01/18 01:23:23  steve
*  Rework l-value handling to allow for more l-value type flexibility.
*
* Revision 1.226  2004/10/04 01:10:54  steve
*  Clean up spurious trailing white space.
*
* Revision 1.225  2004/06/30 02:16:26  steve
*  Implement signed divide and signed right shift in nets.
*
* Revision 1.224  2004/06/13 04:56:54  steve
*  Add support for the default_nettype directive.
*
* Revision 1.223  2004/05/31 23:34:37  steve
*  Rewire/generalize parsing an elaboration of
*  function return values to allow for better
*  speed and more type support.
*
* Revision 1.222  2004/02/20 06:22:56  steve
*  parameter keys are per_strings.
*
* Revision 1.221  2004/02/18 17:11:56  steve
*  Use perm_strings for named langiage items.
*
* Revision 1.220  2003/11/10 19:44:30  steve
*  Fix return value warnings.
*
* Revision 1.219  2003/09/03 23:32:10  steve
*  Oops, missing pin_Sset implementation.
*
* Revision 1.218  2003/08/15 02:23:52  steve
*  Add synthesis support for synchronous reset.
*
* Revision 1.217  2003/07/05 20:42:08  steve
*  Fix some enumeration warnings.
*
* Revision 1.216  2003/06/18 03:55:18  steve
*  Add arithmetic shift operators.
*
* Revision 1.215  2003/05/01 01:13:57  steve
*  More complete bit range internal error message,
*  Better test of part select ranges on non-zero
*  signal ranges.
*/

