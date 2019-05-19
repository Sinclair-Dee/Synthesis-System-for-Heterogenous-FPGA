/*
* Copyright (c) 2000-2004 Stephen Williams (steve@icarus.com)
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
#ident "$Id: t-dll.cc,v 1.131.2.8 2006/07/23 19:42:35 steve Exp $"
#endif

# include "config.h"

# include  <iostream>

# include  <stdio.h>   // sprintf()
# include  "compiler.h"
# include  "t-dll.h"
# include  "netmisc.h"
#ifdef HAVE_MALLOC_H
# include  <malloc.h>
#endif
# include  <stdlib.h>

#include <new> // standard operator new
using std::bad_alloc;

#define __WIN32__ 1
#if defined(__WIN32__)

inline ivl_dll_t ivl_dlopen(const char *name)
{
	ivl_dll_t res =  (ivl_dll_t) LoadLibraryA(name);
	return res;
}


inline void * ivl_dlsym(ivl_dll_t dll, const char *nm)
{
	FARPROC sym;
	return (void*)GetProcAddress((HMODULE)dll, nm);
}

inline void ivl_dlclose(ivl_dll_t dll)
{
	FreeLibrary((HMODULE)dll);
}

const char *dlerror(void)
{
	static char msg[256];
	unsigned long err = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &msg,
		sizeof(msg) - 1,
		NULL
		);
	return msg;
}
#elif defined(HAVE_DLFCN_H)
inline ivl_dll_t ivl_dlopen(const char*name)
{ return dlopen(name,RTLD_LAZY); }

inline void* ivl_dlsym(ivl_dll_t dll, const char*nm)
{
	void*sym = dlsym(dll, nm);
	/* Not found? try without the leading _ */
	if (sym == 0 && nm[0] == '_')
		sym = dlsym(dll, nm+1);
	return sym;
}

inline void ivl_dlclose(ivl_dll_t dll)
{ dlclose(dll); }

#elif defined(HAVE_DL_H)
inline ivl_dll_t ivl_dlopen(const char*name)
{ return shl_load(name, BIND_IMMEDIATE, 0); }

inline void* ivl_dlsym(ivl_dll_t dll, const char*nm)
{
	void*sym;
	int rc = shl_findsym(&dll, nm, TYPE_PROCEDURE, &sym);
	return (rc == 0) ? sym : 0;
}

inline void ivl_dlclose(ivl_dll_t dll)
{ shl_unload(dll); }

inline const char*dlerror(void)
{ return strerror( errno ); }
#endif

inline static const char *basename(ivl_scope_t scope, const char *inst)
{
	inst += strlen(ivl_scope_name(scope));
	assert(*inst == '.');
	return inst+1;
}

static struct dll_target dll_target_obj;

static void drive_from_link(const Link&lnk, ivl_drive_t&drv0, ivl_drive_t&drv1)
{
	switch (lnk.drive0()) {
	case Link::HIGHZ:
		drv0 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		drv0 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		drv0 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		drv0 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		drv0 = IVL_DR_SUPPLY;
		break;
	}

	switch (lnk.drive1()) {
	case Link::HIGHZ:
		drv1 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		drv1 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		drv1 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		drv1 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		drv1 = IVL_DR_SUPPLY;
		break;
	}
}

ivl_attribute_s* dll_target::fill_in_attributes(const Attrib*net)
{
	ivl_attribute_s*attr;
	unsigned nattr = net->attr_cnt();

	if (nattr == 0)
		return 0;

	attr = new struct ivl_attribute_s[nattr];

	for (unsigned idx = 0 ;  idx < nattr ;  idx += 1) {
		verinum tmp = net->attr_value(idx);
		attr[idx].key = net->attr_key(idx);
		if (tmp.is_string()) {
			attr[idx].type = IVL_ATT_STR;
			attr[idx].val.str = strings_.add(tmp.as_string().c_str());

		} else if (tmp == verinum()) {
			attr[idx].type = IVL_ATT_VOID;

		} else {
			attr[idx].type = IVL_ATT_NUM;
			attr[idx].val.num = tmp.as_long();
		}
	}

	return attr;
}

/*
* This function locates an ivl_scope_t object that matches the
* NetScope object. The search works by looking for the parent scope,
* then scanning the parent scope for the NetScope object.
*/
static ivl_scope_t find_scope_from_root(ivl_scope_t root, const NetScope*cur)
{
	ivl_scope_t parent, tmp;

	if (const NetScope*par = cur->parent()) {
		parent = find_scope_from_root(root, par);
		if (parent == 0)
			return 0;

		for (tmp = parent->child_ ;  tmp ;  tmp = tmp->sibling_)
			if (strcmp(tmp->name_, cur->basename()) == 0)
				return tmp;

	} else {
		if (strcmp(root->name_, cur->basename()) == 0)
			return root;
	}

	return 0;
}

ivl_scope_t dll_target::find_scope(ivl_design_s &des, const NetScope*cur)
{
	assert(cur);

	ivl_scope_t scope = 0;
	for (unsigned i = 0; i < des.nroots_ && scope == 0; i += 1) {
		assert(des.roots_[i]);
		scope = find_scope_from_root(des.roots_[i], cur);
	}
	return scope;
}

ivl_scope_t dll_target::lookup_scope_(const NetScope*cur)
{
	return find_scope(des_, cur);
}

/*
* This is a convenience function to locate an ivl_signal_t object
* given the NetESignal that has the signal name.
*/
ivl_signal_t dll_target::find_signal(ivl_design_s &des, const NetNet*net)
{
	ivl_scope_t scope = find_scope(des, net->scope());
	assert(scope);

	const char*nname = net->name();

	for (unsigned idx = 0 ;  idx < scope->nsigs_ ;  idx += 1) {
		if (strcmp(scope->sigs_[idx]->name_, nname) == 0)
			return scope->sigs_[idx];
	}

	assert(0);
	return 0;
}

ivl_variable_t dll_target::find_variable(ivl_design_s&des,
	const NetVariable*net)
{
	ivl_scope_t scope = find_scope(des, net->scope());
	assert(scope);

	const char*nname = net->basename();
	for (unsigned idx = 0 ;  idx < scope->nvar_ ;  idx += 1) {
		if (strcmp(scope->var_[idx]->name, nname) == 0)
			return scope->var_[idx];
	}

	assert(0);
	return 0;
}

/*
* This function locates an ivl_memory_t object that matches the
* NetMemory object. The search works by looking for the parent scope,
* then scanning the parent scope for the NetMemory object.
*/
ivl_memory_t dll_target::find_memory(ivl_design_s &des, const NetMemory*net)
{
	ivl_scope_t scope = find_scope(des, net->scope());
	assert(scope);

	const char*nname = net->name();

	for (unsigned idx = 0 ;  idx < scope->nmem_ ;  idx += 1) {
		if (strcmp(scope->mem_[idx]->basename_, nname) == 0)
			return scope->mem_[idx];
	}

	assert(0);
	return 0;
}

static ivl_nexus_t nexus_sig_make(ivl_signal_t net, unsigned pin)
{
	ivl_nexus_t tmp = new struct ivl_nexus_s;
	tmp->private_data = 0;
	tmp->nptr_ = 1;
	tmp->ptrs_ = (struct ivl_nexus_ptr_s*)
		malloc(sizeof(struct ivl_nexus_ptr_s));
	tmp->ptrs_[0].pin_   = pin;
	tmp->ptrs_[0].type_  = __NEXUS_PTR_SIG;
	tmp->ptrs_[0].l.sig  = net;

	ivl_drive_t drive = IVL_DR_HiZ;
	switch (ivl_signal_type(net)) {
	case IVL_SIT_REG:
		drive = IVL_DR_STRONG;
		break;
	case IVL_SIT_SUPPLY0:
	case IVL_SIT_SUPPLY1:
		drive = IVL_DR_SUPPLY;
		break;
	default:
		break;
	}
	tmp->ptrs_[0].drive0 = drive;
	tmp->ptrs_[0].drive1 = drive;

	return tmp;
}

static void nexus_sig_add(ivl_nexus_t nex, ivl_signal_t net, unsigned pin)
{
	unsigned top = nex->nptr_ + 1;
	nex->ptrs_ = (struct ivl_nexus_ptr_s*)
		realloc(nex->ptrs_, top * sizeof(struct ivl_nexus_ptr_s));
	nex->nptr_ = top;

	ivl_drive_t drive = IVL_DR_HiZ;
	switch (ivl_signal_type(net)) {
	case IVL_SIT_REG:
		drive = IVL_DR_STRONG;
		break;
	case IVL_SIT_SUPPLY0:
	case IVL_SIT_SUPPLY1:
		drive = IVL_DR_SUPPLY;
		break;
	default:
		break;
	}

	nex->ptrs_[top-1].type_= __NEXUS_PTR_SIG;
	nex->ptrs_[top-1].drive0 = drive;
	nex->ptrs_[top-1].drive1 = drive;
	nex->ptrs_[top-1].pin_ = pin;
	nex->ptrs_[top-1].l.sig= net;
}

/*
* Add the pin of the logic object to the nexus, and return the nexus
* pointer used for the pin.
*
* NOTE: This pointer is only valid until another pin is added to the
* nexus.
*/
static ivl_nexus_ptr_t nexus_log_add(ivl_nexus_t nex,
	ivl_net_logic_t net,
	unsigned pin)
{
	unsigned top = nex->nptr_ + 1;
	nex->ptrs_ = (struct ivl_nexus_ptr_s*)
		realloc(nex->ptrs_, top * sizeof(struct ivl_nexus_ptr_s));
	nex->nptr_ = top;

	nex->ptrs_[top-1].type_= __NEXUS_PTR_LOG;
	nex->ptrs_[top-1].drive0 = (pin == 0)? IVL_DR_STRONG : IVL_DR_HiZ;
	nex->ptrs_[top-1].drive1 = (pin == 0)? IVL_DR_STRONG : IVL_DR_HiZ;
	nex->ptrs_[top-1].pin_ = pin;
	nex->ptrs_[top-1].l.log= net;

	return nex->ptrs_ + top - 1;
}

static void nexus_con_add(ivl_nexus_t nex, ivl_net_const_t net, unsigned pin,
	ivl_drive_t drive0, ivl_drive_t drive1)
{
	unsigned top = nex->nptr_ + 1;
	nex->ptrs_ = (struct ivl_nexus_ptr_s*)
		realloc(nex->ptrs_, top * sizeof(struct ivl_nexus_ptr_s));
	nex->nptr_ = top;

	nex->ptrs_[top-1].type_= __NEXUS_PTR_CON;
	nex->ptrs_[top-1].drive0 = drive0;
	nex->ptrs_[top-1].drive1 = drive1;
	nex->ptrs_[top-1].pin_ = pin;
	nex->ptrs_[top-1].l.con= net;
}

static void nexus_lpm_add(ivl_nexus_t nex, ivl_lpm_t net, unsigned pin,
	ivl_drive_t drive0, ivl_drive_t drive1)
{
	unsigned top = nex->nptr_ + 1;
	nex->ptrs_ = (struct ivl_nexus_ptr_s*)
		realloc(nex->ptrs_, top * sizeof(struct ivl_nexus_ptr_s));
	nex->nptr_ = top;

	nex->ptrs_[top-1].type_= __NEXUS_PTR_LPM;
	nex->ptrs_[top-1].drive0 = drive0;
	nex->ptrs_[top-1].drive1 = drive0;
	nex->ptrs_[top-1].pin_ = pin;
	nex->ptrs_[top-1].l.lpm= net;
}


void scope_add_logic(ivl_scope_t scope, ivl_net_logic_t net)
{
	if (scope->nlog_ == 0) {
		scope->nlog_ = 1;
		scope->log_ = (ivl_net_logic_t*)malloc(sizeof(ivl_net_logic_t));
		scope->log_[0] = net;

	} else {
		scope->nlog_ += 1;
		scope->log_ = (ivl_net_logic_t*)
			realloc(scope->log_, scope->nlog_*sizeof(ivl_net_logic_t));
		scope->log_[scope->nlog_-1] = net;
	}

}

void scope_add_event(ivl_scope_t scope, ivl_event_t net)
{
	if (scope->nevent_ == 0) {
		scope->nevent_ = 1;
		scope->event_ = (ivl_event_t*)malloc(sizeof(ivl_event_t));
		scope->event_[0] = net;

	} else {
		scope->nevent_ += 1;
		scope->event_ = (ivl_event_t*)
			realloc(scope->event_, scope->nevent_*sizeof(ivl_event_t));
		scope->event_[scope->nevent_-1] = net;
	}

}

static void scope_add_lpm(ivl_scope_t scope, ivl_lpm_t net)
{
	if (scope->nlpm_ == 0) {
		assert(scope->lpm_ == 0);
		scope->nlpm_ = 1;
		scope->lpm_ = (ivl_lpm_t*)malloc(sizeof(ivl_lpm_t));
		scope->lpm_[0] = net;

	} else {
		assert(scope->lpm_);
		scope->nlpm_ += 1;
		scope->lpm_   = (ivl_lpm_t*)
			realloc(scope->lpm_,
			scope->nlpm_*sizeof(ivl_lpm_t));
		scope->lpm_[scope->nlpm_-1] = net;
	}
}

static void scope_add_mem(ivl_scope_t scope, ivl_memory_t net)
{
	scope->nmem_ += 1;
	scope->mem_   = (ivl_memory_t*)
		realloc(scope->mem_, scope->nmem_*sizeof(ivl_memory_t));
	scope->mem_[scope->nmem_-1] = net;
}

static void scope_add_var(ivl_scope_t scope, ivl_variable_t net)
{
	scope->nvar_ += 1;
	scope->var_   = (ivl_variable_t*)
		realloc(scope->var_, scope->nvar_*sizeof(ivl_variable_t));
	scope->var_[scope->nvar_-1] = net;
}

ivl_parameter_t dll_target::scope_find_param(ivl_scope_t scope,
	const char*name)
{
	unsigned idx = 0;
	while (idx < scope->nparam_) {
		if (strcmp(name, scope->param_[idx].basename) == 0)
			return scope->param_ + idx;

		idx += 1;
	}

	return 0;
}

/*
* This method scans the parameters of the scope, and makes
* ivl_parameter_t objects. This involves saving the name and scanning
* the expression value.
*/
void dll_target::make_scope_parameters(ivl_scope_t scope, const NetScope*net)
{
	scope->nparam_ = net->parameters.size();
	if (scope->nparam_ == 0) {
		scope->param_ = 0;
		return;
	}

	scope->param_ = new struct ivl_parameter_s [scope->nparam_];

	unsigned idx = 0;
	typedef map<perm_string,NetScope::param_expr_t>::const_iterator pit_t;

	for (pit_t cur_pit = net->parameters.begin()
		; cur_pit != net->parameters.end() ;  cur_pit ++) {

			assert(idx < scope->nparam_);
			ivl_parameter_t cur_par = scope->param_ + idx;
			cur_par->basename = (*cur_pit).first;
			cur_par->scope = scope;

			NetExpr*etmp = (*cur_pit).second.expr;

			if (const NetEConst*e = dynamic_cast<const NetEConst*>(etmp)) {

				expr_const(e);
				assert(expr_);

				switch (expr_->type_) {
				case IVL_EX_STRING:
					expr_->u_.string_.parameter = cur_par;
					break;
				case IVL_EX_NUMBER:
					expr_->u_.number_.parameter = cur_par;
					break;
				default:
					assert(0);
				}

			} else if (const NetECReal*e = dynamic_cast<const NetECReal*>(etmp)) {

				expr_creal(e);
				assert(expr_);
				assert(expr_->type_ == IVL_EX_REALNUM);
				expr_->u_.real_.parameter = cur_par;

			}

			cur_par->value = expr_;
			expr_ = 0;

			idx += 1;
	}
}

void dll_target::add_root(ivl_design_s &des_, const NetScope *s)
{
	ivl_scope_t root_ = new struct ivl_scope_s;
	perm_string name = s->basename();
	root_->name_ = name;
	root_->child_ = 0;
	root_->sibling_ = 0;
	root_->parent = 0;
	root_->nsigs_ = 0;
	root_->sigs_ = 0;
	root_->nlog_ = 0;
	root_->log_ = 0;
	root_->nevent_ = 0;
	root_->event_ = 0;
	root_->nlpm_ = 0;
	root_->lpm_ = 0;
	root_->nmem_ = 0;
	root_->mem_ = 0;
	root_->nvar_ = 0;
	root_->var_ = 0;
	make_scope_parameters(root_, s);
	root_->type_ = IVL_SCT_MODULE;
	root_->tname_ = root_->name_;
	root_->time_units = s->time_unit();
	root_->nattr = s->attr_cnt();
	root_->attr  = fill_in_attributes(s);

	des_.nroots_++;
	if (des_.roots_)
		des_.roots_ = (ivl_scope_t *)realloc(des_.roots_, des_.nroots_ * sizeof(ivl_scope_t));
	else
		des_.roots_ = (ivl_scope_t *)malloc(des_.nroots_ * sizeof(ivl_scope_t));
	des_.roots_[des_.nroots_ - 1] = root_;
}

bool dll_target::start_design(const Design*des)
{
	list<NetScope *> root_scopes;
	const char*dll_path_ = des->get_flag("DLL");

	//dll_ = ivl_dlopen(dll_path_);

	// if ((dll_ == 0) && (dll_path_[0] != '/')) {
	//size_t len = strlen(basedir) + 1 + strlen(dll_path_) + 1;
	//char*tmp = new char[len];
	//sprintf(tmp, "%s/%s", basedir, dll_path_);
	//dll_ = ivl_dlopen(tmp);
	//delete[]tmp;
	// }

	// if (dll_ == 0) {
	//cerr << "error: " << dll_path_ << " failed to load." << endl;
	//cerr << dll_path_ << ": " << dlerror() << endl;
	//return false;
	// }

	stmt_cur_ = 0;

	// Initialize the design object.
	des_.self = des;
	des_.time_precision = des->get_precision();
	des_.nroots_ = 0;
	des_.roots_ = NULL;

	root_scopes = des->find_root_scopes();
	for (list<NetScope*>::const_iterator scope = root_scopes.begin();
		scope != root_scopes.end(); scope++)
		add_root(des_, *scope);

	des_.consts  = (ivl_net_const_t*)
		malloc(sizeof(ivl_net_const_t));
	des_.nconsts = 0;

	target_ = (target_design_f)ivl_dlsym(dll_, LU "target_design" TU);
	//  if (target_ == 0) {
	// cerr << dll_path_ << ": error: target_design entry "
	//"point is missing." << endl;
	// return false;
	//  }

	return true;
}

/*
* Here ivl is telling us that the design is scanned completely, and
* here is where we call the API to process the constructed design.
*/
int dll_target::end_design(const Design*)
{
	if (verbose_flag) {
		cout << " ... invoking target_design" << endl;
	}

	//int rc = (target_)(&des_);
	int rc = target_design(&des_);
	//ivl_dlclose(dll_);
	return rc;
}

void dll_target::logic_attributes(struct ivl_net_logic_s *obj,
	const NetNode*net)
{
	obj->nattr = net->attr_cnt();
	obj->attr  = fill_in_attributes(net);
}

//adu
void dll_target::lpm_attributes(struct ivl_lpm_s *obj,
	const NetNode*net)
{
	obj->nattr = net->attr_cnt();
	obj->attr  = fill_in_attributes(net);
}

/*
* Add a bufz object to the scope that contains it.
*
* Note that in the ivl_target API a BUFZ device is a special kind of
* ivl_net_logic_t device, so create an ivl_net_logic_t cookie to
* handle it.
*/
bool dll_target::bufz(const NetBUFZ*net)
{
	struct ivl_net_logic_s *obj = new struct ivl_net_logic_s;

	assert(net->pin_count() == 2);

	obj->type_ = IVL_LO_BUFZ;

	obj->npins_ = 2;
	obj->pins_ = new ivl_nexus_t[2];

	/* Get the ivl_nexus_t objects connected to the two pins.

	(We know a priori that the ivl_nexus_t objects have been
	allocated, because the signals have been scanned before
	me. This saves me the trouble of allocating them.) */

	assert(net->pin(0).nexus()->t_cookie());
	obj->pins_[0] = (ivl_nexus_t) net->pin(0).nexus()->t_cookie();
	ivl_nexus_ptr_t out_ptr = nexus_log_add(obj->pins_[0], obj, 0);


	switch (net->pin(0).drive0()) {
	case Link::HIGHZ:
		out_ptr->drive0 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		out_ptr->drive0 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		out_ptr->drive0 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		out_ptr->drive0 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		out_ptr->drive0 = IVL_DR_SUPPLY;
		break;
	}

	switch (net->pin(0).drive1()) {
	case Link::HIGHZ:
		out_ptr->drive1 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		out_ptr->drive1 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		out_ptr->drive1 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		out_ptr->drive1 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		out_ptr->drive1 = IVL_DR_SUPPLY;
		break;
	}

	assert(net->pin(1).nexus()->t_cookie());
	obj->pins_[1] = (ivl_nexus_t) net->pin(1).nexus()->t_cookie();
	nexus_log_add(obj->pins_[1], obj, 1);

	/* Attach the logic device to the scope that contains it. */

	assert(net->scope());
	ivl_scope_t scope = find_scope(des_, net->scope());
	assert(scope);

	obj->scope_ = scope;

	obj->name_ = net->name();
	logic_attributes(obj, net);

	obj->delay[0] = net->rise_time();
	obj->delay[1] = net->fall_time();
	obj->delay[2] = net->decay_time();

	scope_add_logic(scope, obj);

	return true;
}

void dll_target::event(const NetEvent*net)
{
	struct ivl_event_s *obj = new struct ivl_event_s;

	ivl_scope_t scope = find_scope(des_, net->scope());
	obj->name = net->name();
	obj->scope = scope;
	scope_add_event(scope, obj);

	obj->nany = 0;
	obj->nneg = 0;
	obj->npos = 0;

	if (net->nprobe() >= 1) {

		for (unsigned idx = 0 ;  idx < net->nprobe() ;  idx += 1) {
			const NetEvProbe*pr = net->probe(idx);
			switch (pr->edge()) {
			case NetEvProbe::ANYEDGE:
				obj->nany += pr->pin_count();
				break;
			case NetEvProbe::NEGEDGE:
				obj->nneg += pr->pin_count();
				break;
			case NetEvProbe::POSEDGE:
				obj->npos += pr->pin_count();
				break;
			}
		}

		unsigned npins = obj->nany + obj->nneg + obj->npos;
		obj->pins = (ivl_nexus_t*)calloc(npins, sizeof(ivl_nexus_t));

	} else {
		obj->pins  = 0;
	}

}

void dll_target::variable(const NetVariable*net)
{
	struct ivl_variable_s *obj = new struct ivl_variable_s;

	ivl_scope_t scope = find_scope(des_, net->scope());
	obj->type = IVL_VT_REAL;
	obj->name = net->basename();
	obj->scope = scope;

	scope_add_var(scope, obj);
}

void dll_target::logic(const NetLogic*net)
{
	struct ivl_net_logic_s *obj = new struct ivl_net_logic_s;

	switch (net->type()) {
	case NetLogic::AND:
		obj->type_ = IVL_LO_AND;
		break;
	case NetLogic::BUF:
		obj->type_ = IVL_LO_BUF;
		break;
	case NetLogic::BUFIF0:
		obj->type_ = IVL_LO_BUFIF0;
		break;
	case NetLogic::BUFIF1:
		obj->type_ = IVL_LO_BUFIF1;
		break;
	case NetLogic::NAND:
		obj->type_ = IVL_LO_NAND;
		break;
	case NetLogic::NMOS:
		obj->type_ = IVL_LO_NMOS;
		break;
	case NetLogic::NOR:
		obj->type_ = IVL_LO_NOR;
		break;
	case NetLogic::NOT:
		obj->type_ = IVL_LO_NOT;
		break;
	case NetLogic::NOTIF0:
		obj->type_ = IVL_LO_NOTIF0;
		break;
	case NetLogic::NOTIF1:
		obj->type_ = IVL_LO_NOTIF1;
		break;
	case NetLogic::OR:
		obj->type_ = IVL_LO_OR;
		break;
	case NetLogic::PULLDOWN:
		obj->type_ = IVL_LO_PULLDOWN;
		break;
	case NetLogic::PULLUP:
		obj->type_ = IVL_LO_PULLUP;
		break;
	case NetLogic::RNMOS:
		obj->type_ = IVL_LO_RNMOS;
		break;
	case NetLogic::RPMOS:
		obj->type_ = IVL_LO_RPMOS;
		break;
	case NetLogic::PMOS:
		obj->type_ = IVL_LO_PMOS;
		break;
	case NetLogic::XNOR:
		obj->type_ = IVL_LO_XNOR;
		break;
	case NetLogic::XOR:
		obj->type_ = IVL_LO_XOR;
		break;
	case NetLogic::PLL:
		obj->type_ = IVL_LO_PLL;
		break;
	default:
		assert(0);
		obj->type_ = IVL_LO_NONE;
		break;
	}

	/* Connect all the ivl_nexus_t objects to the pins of the
	device. */

	obj->npins_ = net->pin_count();
	obj->pins_ = new ivl_nexus_t[obj->npins_];

	ivl_nexus_ptr_t out_ptr = 0;

	for (unsigned idx = 0 ;  idx < obj->npins_ ;  idx += 1) {
		const Nexus*nex = net->pin(idx).nexus();
		assert(nex->t_cookie());
		obj->pins_[idx] = (ivl_nexus_t) nex->t_cookie();
		ivl_nexus_ptr_t tmp = nexus_log_add(obj->pins_[idx], obj, idx);
		if (idx == 0)
			out_ptr = tmp;
	}

	switch (net->pin(0).drive0()) {
	case Link::HIGHZ:
		out_ptr->drive0 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		out_ptr->drive0 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		out_ptr->drive0 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		out_ptr->drive0 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		out_ptr->drive0 = IVL_DR_SUPPLY;
		break;
	}

	switch (net->pin(0).drive1()) {
	case Link::HIGHZ:
		out_ptr->drive1 = IVL_DR_HiZ;
		break;
	case Link::WEAK:
		out_ptr->drive1 = IVL_DR_WEAK;
		break;
	case Link::PULL:
		out_ptr->drive1 = IVL_DR_PULL;
		break;
	case Link::STRONG:
		out_ptr->drive1 = IVL_DR_STRONG;
		break;
	case Link::SUPPLY:
		out_ptr->drive1 = IVL_DR_SUPPLY;
		break;
	}

	assert(net->scope());
	ivl_scope_t scope = find_scope(des_, net->scope());
	assert(scope);

	obj->scope_= scope;
	obj->name_ = net->name();

	logic_attributes(obj, net);

	obj->delay[0] = net->rise_time();
	obj->delay[1] = net->fall_time();
	obj->delay[2] = net->decay_time();

	scope_add_logic(scope, obj);
}

void dll_target::net_case_cmp(const NetCaseCmp*net)
{
	struct ivl_net_logic_s *obj = new struct ivl_net_logic_s;

	obj->type_ = IVL_LO_EEQ;

	/* Connect all the ivl_nexus_t objects to the pins of the
	device. */

	obj->npins_ = 3;
	obj->pins_ = new ivl_nexus_t[obj->npins_];
	for (unsigned idx = 0 ;  idx < obj->npins_ ;  idx += 1) {
		const Nexus*nex = net->pin(idx).nexus();
		assert(nex->t_cookie());
		obj->pins_[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_log_add(obj->pins_[idx], obj, idx);
	}

	//assert(net->scope());
	//ivl_scope_t scope = find_scope(des_, net->scope());
	//assert(scope);
	ivl_scope_t scope = des_.roots_[0];

	obj->scope_= scope;
	obj->name_ = net->name();

	obj->delay[0] = net->rise_time();
	obj->delay[1] = net->fall_time();
	obj->delay[2] = net->decay_time();

	logic_attributes(obj,net);

	scope_add_logic(scope, obj);
}

bool dll_target::net_cassign(const NetCAssign*)
{
	return false;
}

bool dll_target::net_force(const NetForce*net)
{
	return true;
}

/*
* An IVL_LPM_UFUNC represents a node in a combinational expression
* that calls a user defined function. I create an LPM object that has
* the right connections, and refers to the ivl_scope_t of the
* definition.
*/
bool dll_target::net_function(const NetUserFunc*net)
{
	struct ivl_lpm_s*obj = new struct ivl_lpm_s;
	obj->type = IVL_LPM_UFUNC;
	obj->name  = net->name();
	obj->scope = find_scope(des_, net->scope());
	obj->attr = 0;
	obj->nattr = 0;
	assert(obj->scope);

	/* Get the definition of the function and save it. */
	const NetScope*def = net->def();
	assert(def);

	obj->u_.ufunc.def = lookup_scope_(def);

	/* Save information about the ports in the ivl_lpm_s
	structure. Note that port 0 is the return value. */
	obj->u_.ufunc.ports = net->port_count();
	obj->u_.ufunc.port_wid = new unsigned[net->port_count()];
	for (unsigned idx = 0 ;  idx < obj->u_.ufunc.ports ;  idx += 1)
		obj->u_.ufunc.port_wid[idx] = net->port_width(idx);

	/* Now collect all the pins and connect them to the nexa of
	the net. The output pins have strong drive, and the
	remaining input pins are HiZ. */

	unsigned pin_count = net->pin_count();
	obj->u_.ufunc.pins = new ivl_nexus_t[pin_count];

	for (unsigned idx = 0 ;  idx < pin_count ;  idx += 1) {
		const Nexus*nex = net->pin(idx).nexus();
		assert(nex->t_cookie());
		ivl_nexus_t nn = (ivl_nexus_t)nex->t_cookie();
		assert(nn);

		obj->u_.ufunc.pins[idx] = nn;
		ivl_drive_t drive = idx < obj->u_.ufunc.port_wid[0]
		? IVL_DR_STRONG
			: IVL_DR_HiZ;
		nexus_lpm_add(obj->u_.ufunc.pins[idx], obj, idx, drive, drive);
	}

	/* All done. Add this LPM to the scope. */
	scope_add_lpm(obj->scope, obj);

	return true;
}

void dll_target::udp(const NetUDP*net)
{
	struct ivl_net_logic_s *obj = new struct ivl_net_logic_s;

	obj->type_ = IVL_LO_UDP;

	static map<perm_string,ivl_udp_t> udps;
	ivl_udp_t u;

	if (udps.find(net->udp_name()) != udps.end())
	{
		u = udps[net->udp_name()];
	}
	else
	{
		u = new struct ivl_udp_s;
		u->nrows = net->rows();
		u->table = (ivl_udp_s::ccharp_t*)malloc((u->nrows+1)*sizeof(char*));
		assert(u->table);
		u->table[u->nrows] = 0x0;
		u->nin = net->nin();
		u->sequ = net->is_sequential();
		if (u->sequ)
			u->init = net->get_initial();
		u->name = net->udp_name();
		string inp;
		char out;
		unsigned int i = 0;
		if (net->first(inp, out))
			do
			{
				string tt = inp+out;
				u->table[i++] = strings_.add(tt.c_str());
			} while (net->next(inp, out));
			assert(i==u->nrows);

			udps[net->udp_name()] = u;
	}

	obj->udp = u;

	// Some duplication of code here, see: dll_target::logic()

	/* Connect all the ivl_nexus_t objects to the pins of the
	device. */

	obj->npins_ = net->pin_count();
	obj->pins_ = new ivl_nexus_t[obj->npins_];
	for (unsigned idx = 0 ;  idx < obj->npins_ ;  idx += 1) {
		const Nexus*nex = net->pin(idx).nexus();

		/* Skip unconnected input pins. These will take on HiZ
		values by the code generators. */
		if (nex->t_cookie() == 0) {
			obj->pins_[idx] = 0;
			continue;
		}

		assert(nex->t_cookie());
		obj->pins_[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_log_add(obj->pins_[idx], obj, idx);
	}

	assert(net->scope());
	ivl_scope_t scope = find_scope(des_, net->scope());
	assert(scope);

	obj->scope_= scope;
	obj->name_ = net->name();

	obj->delay[0] = net->rise_time();
	obj->delay[1] = net->fall_time();
	obj->delay[2] = net->decay_time();

	scope_add_logic(scope, obj);
}

void dll_target::memory(const NetMemory*net)
{
	ivl_memory_t obj = new struct ivl_memory_s;

	obj->scope_    = find_scope(des_, net->scope());
	obj->basename_ = net->name();
	obj->width_    = net->width();
	obj->signed_   = 0;
	obj->size_     = net->count();
	obj->root_     = -net->index_to_address(0);

	scope_add_mem(obj->scope_, obj);
}

void dll_target::lpm_add_sub(const NetAddSub*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	if (net->attribute(perm_string::literal("LPM_Direction")) == verinum("SUB"))
		obj->type = IVL_LPM_SUB;
	else
		obj->type = IVL_LPM_ADD;
	obj->name = net->name(); // NetAddSub names are permallocated
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	obj->attr  = 0;
	obj->nattr = 0;
	assert(obj->scope);

	obj->u_.arith.signed_flag = 0;

	/* Choose the width of the adder. If the carry bit is
	connected, then widen the adder by one and plan on leaving
	the fake inputs unconnected. */
	obj->u_.arith.width = net->width();
	if (net->pin_Cout().is_linked()) {
		obj->u_.arith.width += 1;
	}

	obj->u_.arith.q = new ivl_nexus_t[3 * obj->u_.arith.width];
	obj->u_.arith.a = obj->u_.arith.q + obj->u_.arith.width;
	obj->u_.arith.b = obj->u_.arith.a + obj->u_.arith.width;

	for (unsigned idx = 0 ;  idx < net->width() ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Result(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.arith.q[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		nex = net->pin_DataA(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.arith.a[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.a[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);

		nex = net->pin_DataB(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.arith.b[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.b[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	/* If the carry output is connected, then connect the extra Q
	pin to the carry nexus and zero the a and b inputs. */
	if (net->pin_Cout().is_linked()) {
		unsigned carry = obj->u_.arith.width - 1;
		const Nexus*nex = net->pin_Cout().nexus();
		assert(nex->t_cookie());

		obj->u_.arith.q[carry] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[carry], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		obj->u_.arith.a[carry] = 0;
		obj->u_.arith.b[carry] = 0;
	}

	scope_add_lpm(obj->scope, obj);
}

/*
* The lpm_clshift device represents both left and right shifts,
* depending on what is connected to the Direction pin. We convert
* this device into SHIFTL or SHIFTR devices.
*/
void dll_target::lpm_clshift(const NetCLShift*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type = IVL_LPM_SHIFTL;
	obj->name = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	/* Look at the direction input of the device, and select the
	shift direction accordingly. */
	if (net->right_flag())
		obj->type = IVL_LPM_SHIFTR;
	if (net->signed_flag())
		obj->u_.shift.signed_flag = 1;
	else
		obj->u_.shift.signed_flag = 0;

	obj->u_.shift.width = net->width();
	obj->u_.shift.select = net->width_dist();
	unsigned nex_count = obj->u_.shift.width * 2 + obj->u_.shift.select;
	obj->u_.shift.q = new ivl_nexus_t[nex_count];
	obj->u_.shift.d = obj->u_.shift.q + obj->u_.shift.width;
	obj->u_.shift.s = obj->u_.shift.d + obj->u_.shift.width;

	for (unsigned idx = 0 ;  idx < nex_count ;  idx += 1)
		obj->u_.shift.q[idx] = 0;

	for (unsigned idx = 0 ;  idx < net->width() ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Result(idx).nexus();
		assert(nex && nex->t_cookie());

		obj->u_.shift.q[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.shift.q[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < net->width() ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Data(idx).nexus();
		assert(nex && nex->t_cookie());

		obj->u_.shift.d[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.shift.q[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < net->width_dist() ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Distance(idx).nexus();
		assert(nex && nex->t_cookie());

		obj->u_.shift.s[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.shift.s[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	scope_add_lpm(obj->scope, obj);
}

/*
* Make out of the NetCompare object an ivl_lpm_s object. The
* comparators in ivl_target do not support < or <=, but they can be
* trivially converted to > and >= by swapping the operands.
*/
void dll_target::lpm_compare(const NetCompare*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->attr  = 0;
	obj->nattr = 0;
	obj->name = net->name(); // NetCompare names are permallocated
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	bool swap_operands = false;

	obj->u_.arith.width = net->width();
	obj->u_.arith.signed_flag = net->get_signed()? 1 : 0;

	obj->u_.arith.q = new ivl_nexus_t[1 + 2 * obj->u_.arith.width];
	obj->u_.arith.a = obj->u_.arith.q + 1;
	obj->u_.arith.b = obj->u_.arith.a + obj->u_.arith.width;

	if (net->pin_AGEB().is_linked()) {
		const Nexus*nex = net->pin_AGEB().nexus();
		obj->type = IVL_LPM_CMP_GE;

		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

	} else if (net->pin_AGB().is_linked()) {
		const Nexus*nex = net->pin_AGB().nexus();
		obj->type = IVL_LPM_CMP_GT;

		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

	} else if (net->pin_ALEB().is_linked()) {
		const Nexus*nex = net->pin_ALEB().nexus();
		obj->type = IVL_LPM_CMP_GE;

		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		swap_operands = true;

	} else if (net->pin_ALB().is_linked()) {
		const Nexus*nex = net->pin_ALB().nexus();
		obj->type = IVL_LPM_CMP_GT;

		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		swap_operands = true;

	} else if (net->pin_AEB().is_linked()) {
		const Nexus*nex = net->pin_AEB().nexus();
		obj->type = IVL_LPM_CMP_EQ;

		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

	} else if (net->pin_ANEB().is_linked()) {
		const Nexus*nex = net->pin_ANEB().nexus();
		obj->type = IVL_LPM_CMP_NE;

		if (! nex->t_cookie()) {
			cerr << "internal error: COMPARE_NE device " <<
				net->name()<<" ANEB pin nexus has no cookie."<<endl;
			assert(0);
		}
		assert(nex->t_cookie());
		obj->u_.arith.q[0] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[0], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

	} else {
		assert(0);
	}

	for (unsigned idx = 0 ;  idx < net->width() ;  idx += 1) {
		const Nexus*nex;

		nex = swap_operands
			? net->pin_DataB(idx).nexus()
			: net->pin_DataA(idx).nexus();

		assert(nex->t_cookie());

		obj->u_.arith.a[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.a[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);

		nex = swap_operands
			? net->pin_DataA(idx).nexus()
			: net->pin_DataB(idx).nexus();

		assert(nex->t_cookie());

		obj->u_.arith.b[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.b[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}


	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_divide(const NetDivide*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_DIVIDE;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid = net->width_r();

	obj->u_.arith.width = wid;
	obj->u_.arith.signed_flag = net->get_signed()? 1 : 0;

	obj->u_.arith.q = new ivl_nexus_t[3 * obj->u_.arith.width];
	obj->u_.arith.a = obj->u_.arith.q + obj->u_.arith.width;
	obj->u_.arith.b = obj->u_.arith.a + obj->u_.arith.width;

	for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Result(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.arith.q[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.arith.q[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		if (idx < net->width_a()) {
			nex = net->pin_DataA(idx).nexus();
			assert(nex);
			assert(nex->t_cookie());

			obj->u_.arith.a[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.a[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);

		} else if (obj->u_.arith.signed_flag) {
			/* If this is signed divide, sign extend the perand. */
			nex = net->pin_DataA(net->width_a()-1).nexus();
			assert(nex);
			assert(nex->t_cookie());

			obj->u_.arith.a[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.a[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			/* Unsigned divide: pad the operand. */
			obj->u_.arith.a[idx] = 0;
		}


		if (idx < net->width_b()) {
			nex = net->pin_DataB(idx).nexus();
			assert(nex);
			assert(nex->t_cookie());

			obj->u_.arith.b[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.b[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			obj->u_.arith.b[idx] = 0;
		}
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_modulo(const NetModulo*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MOD;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid = net->width_r();
	if (wid < net->width_a())
		wid = net->width_a();
	if (wid < net->width_b())
		wid = net->width_b();

	obj->u_.arith.width = wid;

	obj->u_.arith.q = new ivl_nexus_t[3 * obj->u_.arith.width];
	obj->u_.arith.a = obj->u_.arith.q + obj->u_.arith.width;
	obj->u_.arith.b = obj->u_.arith.a + obj->u_.arith.width;

	for (unsigned idx = 0 ;  idx < wid ;  idx += 1) {
		const Nexus*nex;

		if (idx < net->width_r()) {
			nex = net->pin_Result(idx).nexus();
			assert(nex->t_cookie());

			obj->u_.arith.q[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.q[idx], obj, 0,
				IVL_DR_STRONG, IVL_DR_STRONG);

		} else {
			obj->u_.arith.q[idx] = 0;
		}

		if (idx < net->width_a()) {
			nex = net->pin_DataA(idx).nexus();
			assert(nex);
			assert(nex->t_cookie());

			obj->u_.arith.a[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.a[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			obj->u_.arith.a[idx] = 0;
		}


		if (idx < net->width_b()) {
			nex = net->pin_DataB(idx).nexus();
			assert(nex);
			assert(nex->t_cookie());

			obj->u_.arith.b[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.arith.b[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			obj->u_.arith.b[idx] = 0;
		}
	}

	scope_add_lpm(obj->scope, obj);
}

bool dll_target::lpm_decode(const NetDecode*net)
{
	return true;
}

//ADU:
void dll_target::lpm_decoder(const NetDecoder*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_DECODER;
	obj->name  = net->name(); // NetMux names are permallocated
	obj->attr  = 0;
	obj->nattr = 0;
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	obj->u_.decoder.width = net->width();
	obj->u_.decoder.decodes = net->decodes();

	scope_add_lpm(obj->scope, obj);

	const Nexus*nex;

	assert(obj->u_.decoder.width);

	obj->u_.decoder.data = new ivl_nexus_t [obj->u_.decoder.width];

	for (unsigned idx = 0 ;  idx < obj->u_.decoder.width ;  idx += 1) {
		nex = net->pin_Data(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.decoder.data[idx] = (ivl_nexus_t) nex->t_cookie();

		if (obj->u_.decoder.data[idx])
			nexus_lpm_add(obj->u_.decoder.data[idx], obj, 0, 
			IVL_DR_STRONG, IVL_DR_STRONG);
	}
	
	obj->u_.decoder.eq = new ivl_nexus_t [obj->u_.decoder.decodes];

	for (unsigned idx = 0 ;  idx < obj->u_.decoder.decodes ;  idx += 1) {
		nex = net->pin_Eq(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.decoder.eq[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.decoder.eq[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}
}

ivl_lpm_t dll_target::lpm_decode_ff_(const NetDecode*net)
{
	if (net == 0)
		return 0;

	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_DECODE;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	obj->scope = find_scope(des_, net->scope());

	obj->u_.mux.swid  = net->awidth();
	obj->u_.mux.size  = 0;
	obj->u_.mux.width = net->width();
	obj->u_.mux.d = 0;

	if (obj->u_.mux.swid > 1) {
		obj->u_.mux.s.pins = new ivl_nexus_t[obj->u_.mux.swid];

		for (unsigned idx = 0 ;  idx < obj->u_.mux.swid ; idx += 1) {
			const Nexus*nex = net->pin_Address(idx).nexus();
			assert(nex->t_cookie());
			obj->u_.mux.s.pins[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.mux.s.pins[idx], obj, idx,
				IVL_DR_HiZ, IVL_DR_HiZ);
		}
	} else {
		assert(obj->u_.mux.swid == 1);
		const Nexus*nex = net->pin_Address(0).nexus();
		assert(nex->t_cookie());
		obj->u_.mux.s.pin = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mux.s.pin, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	scope_add_lpm(obj->scope, obj);
	return obj;
}

bool dll_target::lpm_demux(const NetDemux*net)
{
	unsigned idx;
	unsigned width = net->width();
	unsigned awid = net->awidth();
	unsigned size = net->size();
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_DEMUX;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	obj->u_.demux.width = width;
	obj->u_.demux.awid  = awid;
	obj->u_.demux.size  = net->size();

	ivl_nexus_t*tmp = new ivl_nexus_t [2*width + awid + width/size];
	obj->u_.demux.q = tmp;
	obj->u_.demux.d = tmp + width;
	obj->u_.demux.a = tmp + 2*width;
	obj->u_.demux.bit_in = tmp + 2*width + awid;

	for (idx = 0 ;  idx < width ;  idx += 1) {
		const Nexus*nex = net->pin_Q(idx).nexus();
		obj->u_.demux.q[idx] = (ivl_nexus_t) nex->t_cookie();

		/* It is possible, but unlikely, that the Q is unused. */
		if (obj->u_.demux.q[idx])
			nexus_lpm_add(obj->u_.demux.q[idx], obj, idx,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (idx = 0 ;  idx < width ;  idx += 1) {
		const Nexus*nex = net->pin_Data(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.demux.d[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.demux.d[idx], obj, idx,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (idx = 0 ;  idx < awid ;  idx += 1) {
		const Nexus*nex = net->pin_Address(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.demux.a[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.demux.a[idx], obj, idx,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (idx = 0 ;  idx < width/size ;  idx += 1) {
		const Nexus*nex = net->pin_WriteData(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.demux.bit_in[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.demux.bit_in[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	obj->nattr = net->attr_cnt();
	obj->attr = fill_in_attributes(net);

	scope_add_lpm(obj->scope, obj);
	return true;
}

void dll_target::lpm_ff(const NetFF*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_FF;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	obj->scope = find_scope(des_, net->scope());
	obj->u_.ff.a.decode = lpm_decode_ff_(net->get_demux());
	assert(obj->scope);

	obj->u_.ff.width = net->width();

	obj->nattr = net->attr_cnt();
	obj->attr = fill_in_attributes(net);

	scope_add_lpm(obj->scope, obj);

	const Nexus*nex;

	/* Set the clk signal to point to the nexus, and the nexus to
	point back to this device. */
	nex = net->pin_Clock().nexus();
	assert(nex->t_cookie());
	obj->u_.ff.clk = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.ff.clk);
	nexus_lpm_add(obj->u_.ff.clk, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	/* If there is a clock enable, then connect it up to the FF
	device. */
	if (net->pin_Enable().is_linked()) {
		nex = net->pin_Enable().nexus();
		assert(nex->t_cookie());
		obj->u_.ff.we = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.ff.we);
		nexus_lpm_add(obj->u_.ff.we, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	} else {
		obj->u_.ff.we = 0;
	}

	if (net->pin_Aclr().is_linked()) {
		nex = net->pin_Aclr().nexus();
		assert(nex->t_cookie());
		obj->u_.ff.aclr = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.ff.aclr);
		nexus_lpm_add(obj->u_.ff.aclr, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	} else {
		obj->u_.ff.aclr = 0;
	}

	if (net->pin_Aset().is_linked()) {
		nex = net->pin_Aset().nexus();
		assert(nex->t_cookie());
		obj->u_.ff.aset = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.ff.aset);
		nexus_lpm_add(obj->u_.ff.aset, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

		verinum tmp = net->aset_value();
		obj->u_.ff.aset_value = expr_from_value_(tmp);

	} else {
		obj->u_.ff.aset = 0;
		obj->u_.ff.aset_value = 0;
	}

	if (net->pin_Sclr().is_linked()) {
		nex = net->pin_Sclr().nexus();
		assert(nex->t_cookie());
		obj->u_.ff.sclr = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.ff.sclr);
		nexus_lpm_add(obj->u_.ff.sclr, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	} else {
		obj->u_.ff.sclr = 0;
	}

	if (net->pin_Sset().is_linked()) {
		nex = net->pin_Sset().nexus();
		assert(nex->t_cookie());
		obj->u_.ff.sset = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.ff.sset);
		nexus_lpm_add(obj->u_.ff.sset, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

		verinum tmp = net->sset_value();
		obj->u_.ff.sset_value = expr_from_value_(tmp);

	} else {
		obj->u_.ff.sset = 0;
		obj->u_.ff.sset_value = 0;
	}

	if (obj->u_.ff.width == 1) {
		nex = net->pin_Q(0).nexus();
		assert(nex->t_cookie());
		obj->u_.ff.q.pin = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ff.q.pin, obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

		nex = net->pin_Data(0).nexus();
		assert(nex->t_cookie());
		obj->u_.ff.d.pin = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ff.d.pin, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	} else {
		obj->u_.ff.q.pins = new ivl_nexus_t [obj->u_.ff.width * 2];
		obj->u_.ff.d.pins = obj->u_.ff.q.pins + obj->u_.ff.width;

		for (unsigned idx = 0 ;  idx < obj->u_.ff.width ;  idx += 1) {
			nex = net->pin_Q(idx).nexus();
			assert(nex->t_cookie());
			obj->u_.ff.q.pins[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.ff.q.pins[idx], obj, 0,
				IVL_DR_STRONG, IVL_DR_STRONG);

			nex = net->pin_Data(idx).nexus();
			assert(nex->t_cookie());
			obj->u_.ff.d.pins[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.ff.d.pins[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);
		}
	}
}

void dll_target::lpm_latch( const NetLatch *latchPtr )
{
	try
	{
		ivl_lpm_s *objPtr = new ivl_lpm_s;
		objPtr->type = IVL_LPM_LATCH;
		objPtr->name = latchPtr->name();
		objPtr->scope = find_scope( des_, latchPtr->scope() );
		assert( objPtr->scope ); // C++ programmers prefer using exceptions rather than assertions.

		objPtr->u_.latch.width = latchPtr->width();

		objPtr->nattr = latchPtr->attr_cnt();
		objPtr->attr = fill_in_attributes( latchPtr );

		scope_add_lpm( objPtr->scope, objPtr );

		//// Set the gate signal to point to the nexus, and the nexus to point back to this device.
		//const Nexus *const nexPtr = latchPtr->pin_Gate().nexus();

		/*start------ADU*/
		/* Set the Gate signal to point to the nexus, and the nexus to
		point back to this device. */

		const Nexus*nex;

		nex = latchPtr->pin_Gate().nexus();
		assert(nex->t_cookie());
		objPtr->u_.latch.gatePtr = (ivl_nexus_t) nex->t_cookie();
		assert(objPtr->u_.latch.gatePtr);
		nexus_lpm_add(objPtr->u_.latch.gatePtr, objPtr, 0, IVL_DR_HiZ, IVL_DR_HiZ);

		if (latchPtr->pin_Aclr().is_linked()) {
			nex = latchPtr->pin_Aclr().nexus();
			assert(nex->t_cookie());
			objPtr->u_.latch.AclrPtr = (ivl_nexus_t) nex->t_cookie();
			assert(objPtr->u_.latch.AclrPtr);
			nexus_lpm_add(objPtr->u_.latch.AclrPtr, objPtr, 0, IVL_DR_HiZ, IVL_DR_HiZ);
		} else {
			objPtr->u_.latch.AclrPtr = 0;
		}

		if (latchPtr->pin_Aset().is_linked()) {
			nex = latchPtr->pin_Aset().nexus();
			assert(nex->t_cookie());
			objPtr->u_.latch.AsetPtr = (ivl_nexus_t) nex->t_cookie();
			assert(objPtr->u_.latch.AsetPtr);
			nexus_lpm_add(objPtr->u_.latch.AsetPtr, objPtr, 0, IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			objPtr->u_.latch.AsetPtr = 0;
		}


		if (objPtr->u_.latch.width == 1) {
			nex = latchPtr->pin_Q(0).nexus();
			assert(nex->t_cookie());
			objPtr->u_.latch.qPtr.pin = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(objPtr->u_.latch.qPtr.pin, objPtr, 0,
				IVL_DR_STRONG, IVL_DR_STRONG);

			nex = latchPtr->pin_Data(0).nexus();
			assert(nex->t_cookie());
			objPtr->u_.latch.dataPtr.pin = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(objPtr->u_.latch.dataPtr.pin, objPtr, 0, IVL_DR_HiZ, IVL_DR_HiZ);

		} else {
			objPtr->u_.latch.qPtr.pins = new ivl_nexus_t [objPtr->u_.latch.width * 2];
			objPtr->u_.latch.dataPtr.pins = objPtr->u_.latch.qPtr.pins + objPtr->u_.latch.width;

			for (unsigned idx = 0 ;  idx < objPtr->u_.latch.width ;  idx += 1) {
				nex = latchPtr->pin_Q(idx).nexus();
				assert(nex->t_cookie());
				objPtr->u_.latch.qPtr.pins[idx] = (ivl_nexus_t) nex->t_cookie();
				nexus_lpm_add(objPtr->u_.latch.qPtr.pins[idx], objPtr, 0,
					IVL_DR_STRONG, IVL_DR_STRONG);

				nex = latchPtr->pin_Data(idx).nexus();
				assert(nex->t_cookie());
				objPtr->u_.latch.dataPtr.pins[idx] = (ivl_nexus_t) nex->t_cookie();
				nexus_lpm_add(objPtr->u_.latch.dataPtr.pins[idx], objPtr, 0,
					IVL_DR_HiZ, IVL_DR_HiZ);
			}
		}
		/*end------ADU*/
	}
	catch ( bad_alloc &memoryAllocationException )
	{
		cerr << "Exception occurred: " << memoryAllocationException.what() << endl;
	}

} // end function lpm_latch

//ADU��д�ú���
//void dll_target::lpm_ram_dq(const NetRamDq*net)
//{
//	ivl_memory_t mem = find_memory(des_, net->mem());
//	assert(mem);
//
//	const NetNet*ereg = net->mem()->reg_from_explode();
//
//	ivl_lpm_t obj = new struct ivl_lpm_s;
//	obj->type  = IVL_LPM_RAM;
//	obj->name  = net->name();
//	obj->attr  = 0;
//	obj->nattr = 0;
//	obj->u_.ff.a.mem = ereg? 0 : mem;
//	obj->scope = find_scope(des_, net->mem()->scope());
//	assert(obj->scope);
//
//	obj->u_.ff.width = net->width();
//	obj->u_.ff.swid = net->awidth();
//	obj->u_.ff.scnt = net->mem()->count();
//
//	scope_add_lpm(obj->scope, obj);
//
//	const Nexus*nex;
//
//	// A write port is present only if something is connected to
//	// the clock input.
//
//	bool has_write_port = net->pin_InClock().is_linked();
//	assert( ereg? !has_write_port : 1 );
//
//	// Connect the write clock and write enable
//
//	if (has_write_port) {
//		nex = net->pin_InClock().nexus();
//		assert(nex->t_cookie());
//		obj->u_.ff.clk = (ivl_nexus_t) nex->t_cookie();
//		assert(obj->u_.ff.clk);
//		nexus_lpm_add(obj->u_.ff.clk, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
//
//		nex = net->pin_WE().nexus();
//		if (nex && nex->t_cookie()) {
//			obj->u_.ff.we = (ivl_nexus_t) nex->t_cookie();
//			assert(obj->u_.ff.we);
//			nexus_lpm_add(obj->u_.ff.we, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
//		}
//		else
//			obj->u_.ff.we = 0x0;
//	}
//	else {
//		obj->u_.ff.clk = 0x0;
//		obj->u_.ff.we = 0x0;
//	}
//
//	// Connect the address bus
//
//	if (obj->u_.ff.swid == 1) {
//		nex = net->pin_Address(0).nexus();
//		assert(nex->t_cookie());
//		obj->u_.ff.s.pin = (ivl_nexus_t) nex->t_cookie();
//		nexus_lpm_add(obj->u_.ff.s.pin, obj, 0,
//			IVL_DR_HiZ, IVL_DR_HiZ);
//	}
//	else {
//		obj->u_.ff.s.pins = new ivl_nexus_t [obj->u_.ff.swid];
//
//		for (unsigned idx = 0 ;  idx < obj->u_.ff.swid ;  idx += 1) {
//			nex = net->pin_Address(idx).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.s.pins[idx] = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.s.pins[idx], obj, 0,
//				IVL_DR_HiZ, IVL_DR_HiZ);
//		}
//	}
//
//	// Connect the data busses
//
//	if (obj->u_.ff.width == 1) {
//		nex = net->pin_Q(0).nexus();
//		assert(nex->t_cookie());
//		obj->u_.ff.q.pin = (ivl_nexus_t) nex->t_cookie();
//		nexus_lpm_add(obj->u_.ff.q.pin, obj, 0,
//			IVL_DR_STRONG, IVL_DR_STRONG);
//
//		if (has_write_port) {
//			nex = net->pin_Data(0).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.d.pin = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.d.pin, obj,
//				0, IVL_DR_HiZ, IVL_DR_HiZ);
//		}
//	}
//	else if (has_write_port) {
//		obj->u_.ff.q.pins = new ivl_nexus_t [obj->u_.ff.width * 2];
//		obj->u_.ff.d.pins = obj->u_.ff.q.pins + obj->u_.ff.width;
//
//		for (unsigned idx = 0 ;  idx < obj->u_.ff.width ;  idx += 1) {
//			nex = net->pin_Q(idx).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.q.pins[idx] = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.q.pins[idx], obj, 0,
//				IVL_DR_STRONG, IVL_DR_STRONG);
//
//			nex = net->pin_Data(idx).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.d.pins[idx] = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.d.pins[idx], obj, 0,
//				IVL_DR_HiZ, IVL_DR_HiZ);
//		}
//	}
//	else {
//		obj->u_.ff.q.pins = new ivl_nexus_t [obj->u_.ff.width];
//
//		for (unsigned idx = 0 ;  idx < obj->u_.ff.width ;  idx += 1) {
//			nex = net->pin_Q(idx).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.q.pins[idx] = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.q.pins[idx], obj, 0,
//				IVL_DR_STRONG, IVL_DR_STRONG);
//		}
//	}
//
//	if (ereg) {
//		unsigned count = obj->u_.ff.width * obj->u_.ff.scnt;
//		assert(ereg->pin_count() == count);
//
//		obj->u_.ff.d.pins = new ivl_nexus_t [count];
//
//		for (unsigned idx = 0 ;  idx < count ;  idx += 1) {
//			nex = ereg->pin(idx).nexus();
//			assert(nex->t_cookie());
//			obj->u_.ff.d.pins[idx] = (ivl_nexus_t) nex->t_cookie();
//			nexus_lpm_add(obj->u_.ff.d.pins[idx], obj, 0,
//				IVL_DR_HiZ, IVL_DR_HiZ);
//		}
//	}
//}


//ADU
void dll_target::lpm_ram_dq(const NetRamDq*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_RAM;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned awidth = net->awidth();
	unsigned dwidth = net->width();
	unsigned depth = net->size();
	
	obj->u_.ramDq.awidth = awidth;
	obj->u_.ramDq.dwith = dwidth;
	obj->u_.ramDq.depth = depth;

	obj->u_.ramDq.raddr = new ivl_nexus_t[2*awidth + 2*dwidth];
	obj->u_.ramDq.waddr = obj->u_.ramDq.raddr + awidth;
	obj->u_.ramDq.data_in = obj->u_.ramDq.waddr + awidth;
	obj->u_.ramDq.data_out = obj->u_.ramDq.data_in + dwidth;

	const Nexus*nex;
	//clock_in
	nex = net->pin_InClock().nexus();
	assert(nex->t_cookie());

	obj->u_.ramDq.clk_in = (ivl_nexus_t) nex->t_cookie();
	nexus_lpm_add(obj->u_.ramDq.clk_in, obj, 0,
		IVL_DR_STRONG, IVL_DR_STRONG);

	//clock_out
	nex = net->pin_OutClock().nexus();
	assert(nex->t_cookie());

	obj->u_.ramDq.clk_out = (ivl_nexus_t) nex->t_cookie();
	nexus_lpm_add(obj->u_.ramDq.clk_out, obj, 0,
		IVL_DR_STRONG, IVL_DR_STRONG);
	//we
	nex = net->pin_WE().nexus();
	assert(nex->t_cookie());

	obj->u_.ramDq.we = (ivl_nexus_t) nex->t_cookie();
	nexus_lpm_add(obj->u_.ramDq.we, obj, 0,
		IVL_DR_STRONG, IVL_DR_STRONG);

	//RAddress
	for (unsigned idx = 0 ;  idx < awidth ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_RAddress(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.ramDq.raddr[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ramDq.raddr[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < awidth ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_WAddress(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.ramDq.waddr[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ramDq.waddr[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < dwidth ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Data(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.ramDq.data_in[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ramDq.data_in[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < dwidth ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Q(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.ramDq.data_out[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.ramDq.data_out[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_mult(const NetMult*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MULT;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_r = net->width_r();
	unsigned wid_a = net->width_a();
	unsigned wid_b = net->width_b();

	obj->u_.mult.width_result = wid_r;
	obj->u_.mult.width_dataa = wid_a;
	obj->u_.mult.width_datab = wid_b;

	obj->u_.mult.result = new ivl_nexus_t[wid_r + wid_a + wid_b];
	obj->u_.mult.dataa = obj->u_.mult.result + wid_r;
	obj->u_.mult.datab = obj->u_.mult.dataa + wid_a;

	for (unsigned idx = 0 ;  idx < wid_r ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_Result(idx).nexus();
		assert(nex->t_cookie());

		obj->u_.mult.result[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mult.result[idx], obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_a ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_DataA(idx).nexus();
		assert(nex);
		assert(nex->t_cookie());

		obj->u_.mult.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mult.dataa[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_b ;  idx += 1) {
		const Nexus*nex;

		nex = net->pin_DataB(idx).nexus();
		assert(nex);
		assert(nex->t_cookie());

		obj->u_.mult.datab[idx] = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mult.datab[idx], obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_mem(const NetMem*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MIP;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_address_a = net->width_address_a();
	unsigned wid_address_b = net->width_address_b();
	unsigned wid_data_a = net->width_data_a();
	unsigned wid_data_b = net->width_data_b();
	unsigned wid_q_a = net->width_q_a();
	unsigned wid_q_b = net->width_q_b();

	obj->u_.mip.width_address_a = wid_address_a;
	obj->u_.mip.width_address_b = wid_address_b;
	obj->u_.mip.width_data_a = wid_data_a;
	obj->u_.mip.width_data_b = wid_data_b;
	obj->u_.mip.width_q_a = wid_q_a;
	obj->u_.mip.width_q_b = wid_q_b;

	obj->u_.mip.address_a = new ivl_nexus_t[wid_address_a + wid_address_b + wid_data_a + wid_data_b + wid_q_a + wid_q_b];
	obj->u_.mip.address_b = obj->u_.mip.address_a + wid_address_a;
	obj->u_.mip.data_a = obj->u_.mip.address_b + wid_address_b;
	obj->u_.mip.data_b = obj->u_.mip.data_a + wid_data_a;
	obj->u_.mip.q_a = obj->u_.mip.data_b + wid_data_b;
	obj->u_.mip.q_b = obj->u_.mip.q_a + wid_q_a;

	const Nexus*nex;

	nex = net->pin_clocken0().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.clocken0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.clocken0);
	nexus_lpm_add(obj->u_.mip.clocken0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clocken1().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.clocken1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.clocken1);
	nexus_lpm_add(obj->u_.mip.clocken1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_wren_a().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.wren_a = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.wren_a);
	nexus_lpm_add(obj->u_.mip.wren_a, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr0().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.aclr0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.aclr0);
	nexus_lpm_add(obj->u_.mip.aclr0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock0().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.clock0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.clock0);
	nexus_lpm_add(obj->u_.mip.clock0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_wren_b().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.wren_b = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.wren_b);
	nexus_lpm_add(obj->u_.mip.wren_b, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr1().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.aclr1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.aclr1);
	nexus_lpm_add(obj->u_.mip.aclr1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock1().nexus();
	assert(nex->t_cookie());
	obj->u_.mip.clock1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mip.clock1);
	nexus_lpm_add(obj->u_.mip.clock1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	for (unsigned idx = 0 ;  idx < wid_address_a ;  idx += 1) {
		nex = net->pin_address_a(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.address_a[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.address_a[idx]);
		nexus_lpm_add(obj->u_.mip.address_a[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_address_b ;  idx += 1) {
		nex = net->pin_address_b(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.address_b[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.address_b[idx]);
		nexus_lpm_add(obj->u_.mip.address_b[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_data_a ;  idx += 1) {
		nex = net->pin_data_a(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.data_a[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.data_a[idx]);
		nexus_lpm_add(obj->u_.mip.data_a[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_data_b ;  idx += 1) {
		nex = net->pin_data_b(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.data_b[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.data_b[idx]);
		nexus_lpm_add(obj->u_.mip.data_b[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_q_a ;  idx += 1) {
		nex = net->pin_q_a(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.q_a[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.q_a[idx]);
		nexus_lpm_add(obj->u_.mip.q_a[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_q_b ;  idx += 1) {
		nex = net->pin_q_b(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mip.q_b[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mip.q_b[idx]);
		nexus_lpm_add(obj->u_.mip.q_b[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}
	lpm_attributes(obj, net);
	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_square(const NetSquare*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_SQUARE;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_data = net->width_data();
	unsigned wid_result = net->width_result();

	obj->u_.mult.width_data = wid_data;
	obj->u_.mult.width_result = wid_result;

	obj->u_.mult.data = new ivl_nexus_t[wid_data + wid_result];
	obj->u_.mult.result = obj->u_.mult.data + wid_data;

	const Nexus*nex;

	nex = net->pin_clken().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clken = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clken);
	nexus_lpm_add(obj->u_.mult.clken, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr);
	nexus_lpm_add(obj->u_.mult.aclr, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock);
	nexus_lpm_add(obj->u_.mult.clock, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	for (unsigned idx = 0 ;  idx < wid_data ;  idx += 1) {
		nex = net->pin_data(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.data[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.data[idx]);
		nexus_lpm_add(obj->u_.mult.data[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.result[idx]);
		nexus_lpm_add(obj->u_.mult.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_lmult(const NetLMult*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_LMULT;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa = net->width_dataa();
	unsigned wid_datab = net->width_datab();
	unsigned wid_sum = net->width_sum();
	unsigned wid_result = net->width_result();

	obj->u_.mult.width_dataa = wid_dataa;
	obj->u_.mult.width_datab = wid_datab;
	obj->u_.mult.width_sum = wid_sum;
	obj->u_.mult.width_result = wid_result;

	obj->u_.mult.dataa = new ivl_nexus_t[wid_dataa + wid_datab + wid_sum + wid_result];
	obj->u_.mult.datab = obj->u_.mult.dataa + wid_dataa;
	obj->u_.mult.sum = obj->u_.mult.datab + wid_datab;
	obj->u_.mult.result = obj->u_.mult.sum + wid_sum;

	const Nexus*nex;

	nex = net->pin_clken().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clken = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clken);
	nexus_lpm_add(obj->u_.mult.clken, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr);
	nexus_lpm_add(obj->u_.mult.aclr, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock);
	nexus_lpm_add(obj->u_.mult.clock, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	for (unsigned idx = 0 ;  idx < wid_dataa ;  idx += 1) {
		nex = net->pin_dataa(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.dataa[idx]);
		nexus_lpm_add(obj->u_.mult.dataa[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab ;  idx += 1) {
		nex = net->pin_datab(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.datab[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.datab[idx]);
		nexus_lpm_add(obj->u_.mult.datab[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_sum ;  idx += 1) {
		nex = net->pin_sum(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.sum[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.sum[idx]);
		nexus_lpm_add(obj->u_.mult.sum[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.result[idx]);
		nexus_lpm_add(obj->u_.mult.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_lmultout(const NetLMultSimpleOUT*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_LMULTOUT;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa = net->width_dataa();
	unsigned wid_result = net->width_result();

	obj->u_.multout.width_dataa = wid_dataa;
	obj->u_.multout.width_result = wid_result;

	obj->u_.multout.dataa = new ivl_nexus_t[wid_dataa + wid_result];
	obj->u_.multout.result = obj->u_.multout.dataa + wid_dataa;

	const Nexus*nex;

	
	for (unsigned idx = 0 ;  idx < wid_dataa ;  idx += 1) {
		nex = net->pin_dataa(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.dataa[idx]);
		nexus_lpm_add(obj->u_.multout.dataa[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}
	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.result[idx]);
		nexus_lpm_add(obj->u_.multout.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}


void dll_target::lpm_mult_add(const NetMultAdd*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MULT_ADD;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa = net->width_dataa();
	unsigned wid_datab = net->width_datab();
	unsigned wid_scanouta = net->width_scanouta();
	unsigned wid_scanoutb = net->width_scanoutb();
	unsigned wid_result = net->width_result();

	obj->u_.mult.width_dataa = wid_dataa;
	obj->u_.mult.width_datab = wid_datab;
	obj->u_.mult.width_scanouta = wid_scanouta;
	obj->u_.mult.width_scanoutb = wid_scanoutb;
	obj->u_.mult.width_result = wid_result;

	obj->u_.mult.dataa = new ivl_nexus_t[wid_dataa + wid_datab + wid_scanouta + wid_scanoutb + wid_result];
	obj->u_.mult.datab = obj->u_.mult.dataa + wid_dataa;
	obj->u_.mult.scanouta = obj->u_.mult.datab + wid_datab;
	obj->u_.mult.scanoutb = obj->u_.mult.scanouta + wid_scanouta;
	obj->u_.mult.result = obj->u_.mult.scanoutb + wid_scanoutb;

	const Nexus*nex;

	nex = net->pin_clock0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock0);
	nexus_lpm_add(obj->u_.mult.clock0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock1);
	nexus_lpm_add(obj->u_.mult.clock1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock2);
	nexus_lpm_add(obj->u_.mult.clock2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock3);
	nexus_lpm_add(obj->u_.mult.clock3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr0);
	nexus_lpm_add(obj->u_.mult.aclr0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr1);
	nexus_lpm_add(obj->u_.mult.aclr1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr2);
	nexus_lpm_add(obj->u_.mult.aclr2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr3);
	nexus_lpm_add(obj->u_.mult.aclr3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena0);
	nexus_lpm_add(obj->u_.mult.ena0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena1);
	nexus_lpm_add(obj->u_.mult.ena1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena2);
	nexus_lpm_add(obj->u_.mult.ena2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena3);
	nexus_lpm_add(obj->u_.mult.ena3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_signa().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.signa = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.signa);
	nexus_lpm_add(obj->u_.mult.signa, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_signb().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.signb = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.signb);
	nexus_lpm_add(obj->u_.mult.signb, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	for (unsigned idx = 0 ;  idx < wid_dataa ;  idx += 1) {
		nex = net->pin_dataa(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.dataa[idx]);
		nexus_lpm_add(obj->u_.mult.dataa[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab ;  idx += 1) {
		nex = net->pin_datab(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.datab[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.datab[idx]);
		nexus_lpm_add(obj->u_.mult.datab[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_scanouta ;  idx += 1) {
		nex = net->pin_scanouta(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.scanouta[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.scanouta[idx]);
		nexus_lpm_add(obj->u_.mult.scanouta[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_scanoutb ;  idx += 1) {
		nex = net->pin_scanoutb(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.scanoutb[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.scanoutb[idx]);
		nexus_lpm_add(obj->u_.mult.scanoutb[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.result[idx]);
		nexus_lpm_add(obj->u_.mult.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_mult_addout(const NetMultAddOut*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_LMULTADDOUT;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa = net->width_dataa();
	unsigned wid_datab = net->width_datab();
	unsigned wid_datac = net->width_datac();
	unsigned wid_datad = net->width_datad();
	unsigned wid_addnsub = net->width_addnsub();
	unsigned wid_accumsload = net->width_accumsload();
	unsigned wid_result = net->width_result();

	obj->u_.multout.width_dataa = wid_dataa;
	obj->u_.multout.width_datab = wid_datab;
	obj->u_.multout.width_datac = wid_datac;
	obj->u_.multout.width_datad = wid_datad;
	obj->u_.multout.width_addnsub = wid_addnsub;
	obj->u_.multout.width_accumsload = wid_accumsload;
	obj->u_.multout.width_result = wid_result;

	obj->u_.multout.dataa = new ivl_nexus_t[wid_dataa + wid_datab + wid_datac + wid_datad+wid_addnsub+wid_accumsload + wid_result];
	obj->u_.multout.datab = obj->u_.multout.dataa + wid_dataa;
	obj->u_.multout.datac = obj->u_.multout.datab + wid_datab;
	obj->u_.multout.datad = obj->u_.multout.datac + wid_datac;
	obj->u_.multout.addnsub = obj->u_.multout.datad + wid_datad;
	obj->u_.multout.accumsload = obj->u_.multout.addnsub + wid_addnsub;
	obj->u_.multout.result = obj->u_.multout.accumsload + wid_accumsload;

	const Nexus*nex;

	nex = net->pin_clock0().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.clock0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.clock0);
	nexus_lpm_add(obj->u_.multout.clock0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock1().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.clock1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.clock1);
	nexus_lpm_add(obj->u_.multout.clock1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock2().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.clock2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.clock2);
	nexus_lpm_add(obj->u_.multout.clock2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock3().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.clock3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.clock3);
	nexus_lpm_add(obj->u_.multout.clock3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr0().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.aclr0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.aclr0);
	nexus_lpm_add(obj->u_.multout.aclr0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr1().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.aclr1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.aclr1);
	nexus_lpm_add(obj->u_.multout.aclr1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr2().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.aclr2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.aclr2);
	nexus_lpm_add(obj->u_.multout.aclr2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr3().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.aclr3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.aclr3);
	nexus_lpm_add(obj->u_.multout.aclr3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena0().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.ena0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.ena0);
	nexus_lpm_add(obj->u_.mult.ena0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena1().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.ena1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.ena1);
	nexus_lpm_add(obj->u_.multout.ena1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena2().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.ena2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.ena2);
	nexus_lpm_add(obj->u_.multout.ena2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena3().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.ena3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.ena3);
	nexus_lpm_add(obj->u_.multout.ena3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_signa().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.signa = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.signa);
	nexus_lpm_add(obj->u_.multout.signa, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_signb().nexus();
	assert(nex->t_cookie());
	obj->u_.multout.signb = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.multout.signb);
	nexus_lpm_add(obj->u_.multout.signb, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	for (unsigned idx = 0 ;  idx < wid_dataa ;  idx += 1) {
		nex = net->pin_dataa(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.dataa[idx]);
		nexus_lpm_add(obj->u_.multout.dataa[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab ;  idx += 1) {
		nex = net->pin_datab(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.datab[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.datab[idx]);
		nexus_lpm_add(obj->u_.multout.datab[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}
	for (unsigned idx = 0 ;  idx < wid_datac ;  idx += 1) {
		nex = net->pin_datac(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.datac[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.datac[idx]);
		nexus_lpm_add(obj->u_.multout.datac[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}
	for (unsigned idx = 0 ;  idx < wid_datad ;  idx += 1) {
		nex = net->pin_datad(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.datad[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.datad[idx]);
		nexus_lpm_add(obj->u_.multout.datad[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_addnsub;  idx += 1) {
		nex = net->pin_addnsub(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.addnsub[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.addnsub[idx]);
		nexus_lpm_add(obj->u_.multout.addnsub[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_accumsload ;  idx += 1) {
		nex = net->pin_accumsload(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.accumsload[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.multout.accumsload[idx]);
		nexus_lpm_add(obj->u_.multout.accumsload[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}
	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.multout.result[idx] = (ivl_nexus_t) nex->t_cookie();//�����multout�������죬̫�����ˡ�
		assert(obj->u_.multout.result[idx]);
		nexus_lpm_add(obj->u_.multout.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_mult_accum(const NetMultAccum*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MULT_ACCUM;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa = net->width_dataa();
	unsigned wid_datab = net->width_datab();
	unsigned wid_scanouta = net->width_scanouta();
	unsigned wid_scanoutb = net->width_scanoutb();
	unsigned wid_result = net->width_result();

	obj->u_.mult.width_dataa = wid_dataa;
	obj->u_.mult.width_datab = wid_datab;
	obj->u_.mult.width_scanouta = wid_scanouta;
	obj->u_.mult.width_scanoutb = wid_scanoutb;
	obj->u_.mult.width_result = wid_result;

	obj->u_.mult.dataa = new ivl_nexus_t[wid_dataa + wid_datab + wid_scanouta + wid_scanoutb + wid_result];
	obj->u_.mult.datab = obj->u_.mult.dataa + wid_dataa;
	obj->u_.mult.scanouta = obj->u_.mult.datab + wid_datab;
	obj->u_.mult.scanoutb = obj->u_.mult.scanouta + wid_scanouta;
	obj->u_.mult.result = obj->u_.mult.scanoutb + wid_scanoutb;

	const Nexus*nex;

	nex = net->pin_accum_sload().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.accum_sload = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.accum_sload);
	nexus_lpm_add(obj->u_.mult.accum_sload, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock0);
	nexus_lpm_add(obj->u_.mult.clock0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock1);
	nexus_lpm_add(obj->u_.mult.clock1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock2);
	nexus_lpm_add(obj->u_.mult.clock2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_clock3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.clock3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.clock3);
	nexus_lpm_add(obj->u_.mult.clock3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr0);
	nexus_lpm_add(obj->u_.mult.aclr0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr1);
	nexus_lpm_add(obj->u_.mult.aclr1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr2);
	nexus_lpm_add(obj->u_.mult.aclr2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_aclr3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.aclr3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.aclr3);
	nexus_lpm_add(obj->u_.mult.aclr3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena0().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena0 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena0);
	nexus_lpm_add(obj->u_.mult.ena0, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena1().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena1 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena1);
	nexus_lpm_add(obj->u_.mult.ena1, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena2().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena2 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena2);
	nexus_lpm_add(obj->u_.mult.ena2, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_ena3().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.ena3 = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.ena3);
	nexus_lpm_add(obj->u_.mult.ena3, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);

	nex = net->pin_overflow().nexus();
	assert(nex->t_cookie());
	obj->u_.mult.overflow = (ivl_nexus_t) nex->t_cookie();
	assert(obj->u_.mult.overflow);
	nexus_lpm_add(obj->u_.mult.overflow, obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);

	for (unsigned idx = 0 ;  idx < wid_dataa ;  idx += 1) {
		nex = net->pin_dataa(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.dataa[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.dataa[idx]);
		nexus_lpm_add(obj->u_.mult.dataa[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab ;  idx += 1) {
		nex = net->pin_datab(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.datab[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.datab[idx]);
		nexus_lpm_add(obj->u_.mult.datab[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_scanouta ;  idx += 1) {
		nex = net->pin_scanouta(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.scanouta[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.scanouta[idx]);
		nexus_lpm_add(obj->u_.mult.scanouta[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_scanoutb ;  idx += 1) {
		nex = net->pin_scanoutb(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.scanoutb[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.scanoutb[idx]);
		nexus_lpm_add(obj->u_.mult.scanoutb[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.mult.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.mult.result[idx]);
		nexus_lpm_add(obj->u_.mult.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	scope_add_lpm(obj->scope, obj);
}

void dll_target::lpm_HalfDSPBlock(const HalfDSPBlock*net){
	//todo
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_HalfDSPBlock;
	obj->name  = net->name();
	obj->attr  = 0;
	obj->nattr = 0;
	assert(net->scope());
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	unsigned wid_dataa0 = net->width_dataa0();
	unsigned wid_datab0 = net->width_datab0();
	unsigned wid_dataa1 = net->width_dataa1();
	unsigned wid_datab1 = net->width_datab1();
	unsigned wid_result   = net->width_result();
	unsigned wid_clock   = net->width_clock();
	unsigned wid_ena      = net->width_ena();
	unsigned wid_accumsload = net->width_accumsload();

	obj->u_.halfdspblock.width_dataa0 = wid_dataa0;
	obj->u_.halfdspblock.width_datab0 = wid_datab0;
	obj->u_.halfdspblock.width_dataa1 = wid_dataa1;
	obj->u_.halfdspblock.width_datab1 = wid_datab1;
	obj->u_.halfdspblock.width_result   = wid_result;
	obj->u_.halfdspblock.width_clock   = wid_clock;
	obj->u_.halfdspblock.width_ena      = wid_ena;
	obj->u_.halfdspblock.width_accumsload = wid_accumsload;




	obj->u_.halfdspblock.dataa0 = new ivl_nexus_t[wid_dataa0 + wid_datab0 + wid_dataa1 + wid_datab1 + wid_result+wid_clock+wid_ena+wid_accumsload];
	obj->u_.halfdspblock.datab0 = obj->u_.halfdspblock.dataa0 + wid_dataa0;
	obj->u_.halfdspblock.dataa1 = obj->u_.halfdspblock.datab0 + wid_datab0;
	obj->u_.halfdspblock.datab1 = obj->u_.halfdspblock.dataa1 + wid_dataa1;
	obj->u_.halfdspblock.result   = obj->u_.halfdspblock.datab1  + wid_datab1;
	obj->u_.halfdspblock.clock   = obj->u_.halfdspblock.result  +wid_result;
    obj->u_.halfdspblock.ena      = obj->u_.halfdspblock.clock + wid_clock;
    obj->u_.halfdspblock.accumsload = obj->u_.halfdspblock.ena + wid_ena;

	const Nexus*nex;

	for (unsigned idx = 0 ;  idx < wid_dataa0 ;  idx += 1) {
		nex = net->pin_dataa0(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.dataa0[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.dataa0[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.dataa0[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab0 ;  idx += 1) {
		nex = net->pin_datab0(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.datab0[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.datab0[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.datab0[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_dataa1 ;  idx += 1) {
		nex = net->pin_dataa1(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.dataa1[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.dataa1[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.dataa1[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_datab1 ;  idx += 1) {
		nex = net->pin_datab1(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.datab1[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.datab1[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.datab1[idx], obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
	}

	for (unsigned idx = 0 ;  idx < wid_result ;  idx += 1) {
		nex = net->pin_result(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.result[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.result[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.result[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_clock ;  idx += 1) {
		nex = net->pin_clock(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.clock[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.clock[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.clock[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_ena ;  idx += 1) {
		nex = net->pin_ena(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.ena[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.ena[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.ena[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}

	for (unsigned idx = 0 ;  idx < wid_accumsload ;  idx += 1) {
		nex = net->pin_accumsload(idx).nexus();
		assert(nex->t_cookie());
		obj->u_.halfdspblock.accumsload[idx] = (ivl_nexus_t) nex->t_cookie();
		assert(obj->u_.halfdspblock.accumsload[idx]);
		nexus_lpm_add(obj->u_.halfdspblock.accumsload[idx], obj, 0, IVL_DR_STRONG, IVL_DR_STRONG);
	}


	scope_add_lpm(obj->scope, obj);
}

/*
* Hook up the mux devices so that the select expression selects the
* correct sub-expression with the ivl_lpm_data2 function.
*/
void dll_target::lpm_mux(const NetMux*net)
{
	ivl_lpm_t obj = new struct ivl_lpm_s;
	obj->type  = IVL_LPM_MUX;
	obj->name  = net->name(); // NetMux names are permallocated
	obj->attr  = 0;
	obj->nattr = 0;
	obj->scope = find_scope(des_, net->scope());
	assert(obj->scope);

	obj->u_.mux.width = net->width();
	obj->u_.mux.size  = net->size();
	obj->u_.mux.swid  = net->sel_width();

	scope_add_lpm(obj->scope, obj);

	const Nexus*nex;

	/* Connect the output bits. */
	if (obj->u_.mux.width == 1) {
		nex = net->pin_Result(0).nexus();
		assert(nex->t_cookie());
		obj->u_.mux.q.pin = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mux.q.pin, obj, 0,
			IVL_DR_STRONG, IVL_DR_STRONG);

	} else {
		obj->u_.mux.q.pins = new ivl_nexus_t [obj->u_.mux.width];

		for (unsigned idx = 0 ;  idx < obj->u_.mux.width ;  idx += 1) {
			nex = net->pin_Result(idx).nexus();
#if 0
			if (! nex->t_cookie()) {
				cerr << net->get_line() << ": internal error: "
					<< "broken mux. name=" << net->name()
					<< ", width=" << obj->u_.mux.width
					<< ", swid=" << obj->u_.mux.swid << endl;
				for (unsigned tmp = 0; tmp < obj->u_.mux.width; tmp += 1) {
					const Nexus*tmpn = net->pin_Result(tmp).nexus();
					if (tmpn->t_cookie() == 0)
						continue;

					cerr << net->get_line() << ": XXXX "
						<< "Result(" << tmp << ") : "
						<< tmpn->name() << endl;
				}
			}
#endif
			obj->u_.mux.q.pins[idx] = (ivl_nexus_t) nex->t_cookie();

			/* It is possible (although unlikely) that the
			result bit of the mux is not used. This can
			happen, for example, if the output is
			overridden in the HDL. In that case, skip
			it. But if the bit is used, then add it to the
			nexus. */
			if (obj->u_.mux.q.pins[idx])
				nexus_lpm_add(obj->u_.mux.q.pins[idx], obj, 0,
				IVL_DR_STRONG, IVL_DR_STRONG);
		}
	}

	/* Connect the select bits. */
	if (obj->u_.mux.swid == 1) {
		nex = net->pin_Sel(0).nexus();
		assert(nex->t_cookie());
		obj->u_.mux.s.pin = (ivl_nexus_t) nex->t_cookie();
		nexus_lpm_add(obj->u_.mux.s.pin, obj, 0,
			IVL_DR_HiZ, IVL_DR_HiZ);

	} else {
		obj->u_.mux.s.pins = new ivl_nexus_t [obj->u_.mux.swid];

		for (unsigned idx = 0 ;  idx < obj->u_.mux.swid ;  idx += 1) {
			nex = net->pin_Sel(idx).nexus();
			assert(nex->t_cookie());
			obj->u_.mux.s.pins[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_lpm_add(obj->u_.mux.s.pins[idx], obj, 0,
				IVL_DR_HiZ, IVL_DR_HiZ);
		}
	}

	unsigned width = obj->u_.mux.width;
	unsigned selects = obj->u_.mux.size;

	obj->u_.mux.d = new ivl_nexus_t [width * selects];

	for (unsigned sdx = 0 ;  sdx < selects ;  sdx += 1)
		for (unsigned ddx = 0 ;  ddx < width ;  ddx += 1) {
			nex = net->pin_Data(ddx, sdx).nexus();
			ivl_nexus_t tmp = (ivl_nexus_t) nex->t_cookie();
			obj->u_.mux.d[sdx*width + ddx] = tmp;
			nexus_lpm_add(tmp, obj, 0, IVL_DR_HiZ, IVL_DR_HiZ);
		}

}

/*
* The assignment l-values are captured by the assignment statements
* themselves in the process handling.
*/
void dll_target::net_assign(const NetAssign_*)
{
}

bool dll_target::net_const(const NetConst*net)
{
	unsigned idx;
	char*bits;

	struct ivl_net_const_s *obj = new struct ivl_net_const_s;

	obj->width_ = net->pin_count();
	if (obj->width_ <= sizeof(char*)) {
		bits = obj->b.bit_;

	} else {
		obj->b.bits_ = (char*)malloc(obj->width_);
		bits = obj->b.bits_;
	}

	for (idx = 0 ;  idx < obj->width_ ;  idx += 1)
		switch (net->value(idx)) {
		case verinum::V0:
			bits[idx] = '0';
			break;
		case verinum::V1:
			bits[idx] = '1';
			break;
		case verinum::Vx:
			bits[idx] = 'x';
			break;
		case verinum::Vz:
			bits[idx] = 'z';
			break;
	}

	/* Connect to all the nexus objects. Note that the one-bit
	case can be handled more efficiently without allocating
	array space. */
	if (obj->width_ == 1) {
		ivl_drive_t drv0, drv1;
		drive_from_link(net->pin(0), drv0, drv1);
		const Nexus*nex = net->pin(0).nexus();
		assert(nex->t_cookie());
		obj->n.pin_ = (ivl_nexus_t) nex->t_cookie();
		nexus_con_add(obj->n.pin_, obj, 0, drv0, drv1);

	} else {
		obj->n.pins_ = new ivl_nexus_t[obj->width_];
		for (unsigned idx = 0 ;  idx < obj->width_ ;  idx += 1) {
			if (! net->pin(idx).is_linked())
				continue;

			ivl_drive_t drv0, drv1;
			drive_from_link(net->pin(idx), drv0, drv1);

			const Nexus*nex = net->pin(idx).nexus();
			assert(nex->t_cookie());

			obj->n.pins_[idx] = (ivl_nexus_t) nex->t_cookie();
			nexus_con_add(obj->n.pins_[idx], obj, idx, drv0, drv1);
		}
	}

	des_.nconsts += 1;
	des_.consts = (ivl_net_const_t*)
		realloc(des_.consts, des_.nconsts * sizeof(ivl_net_const_t));
	des_.consts[des_.nconsts-1] = obj;

	return true;
}

void dll_target::net_probe(const NetEvProbe*net)
{
}

void dll_target::scope(const NetScope*net)
{
	ivl_scope_t scope;

	if (net->parent() == 0) {
		unsigned i;
		scope = NULL;
		for (i = 0; i < des_.nroots_ && scope == NULL; i++) {
			if (strcmp(des_.roots_[i]->name_, net->name().c_str()) == 0)
				scope = des_.roots_[i];
		}
		assert(scope);

	} else {
		scope = new struct ivl_scope_s;
		scope->name_ = net->basename();
		scope->child_ = 0;
		scope->sibling_ = 0;
		scope->parent = find_scope(des_, net->parent());
		assert(scope->parent);
		scope->nsigs_ = 0;
		scope->sigs_ = 0;
		scope->nlog_ = 0;
		scope->log_ = 0;
		scope->nevent_ = 0;
		scope->event_ = 0;
		scope->nlpm_ = 0;
		scope->lpm_ = 0;
		scope->nmem_ = 0;
		scope->mem_ = 0;
		scope->nvar_ = 0;
		scope->var_ = 0;
		make_scope_parameters(scope, net);
		scope->time_units = net->time_unit();
		scope->nattr = net->attr_cnt();
		scope->attr  = fill_in_attributes(net);

		switch (net->type()) {
		case NetScope::MODULE:
			scope->type_ = IVL_SCT_MODULE;
			scope->tname_ = net->module_name();
			break;
		case NetScope::TASK: {
			const NetTaskDef*def = net->task_def();
			if (def == 0) {
				cerr <<  "?:?" << ": internal error: "
					<< "task " << scope->name_
					<< " has no definition." << endl;
			}
			assert(def);
			scope->type_ = IVL_SCT_TASK;
			scope->tname_ = strings_.make(def->name().c_str());
			break;
							 }
		case NetScope::FUNC:
			scope->type_ = IVL_SCT_FUNCTION;
			scope->tname_ = strings_.make(net->func_def()->name().c_str());
			break;
		case NetScope::BEGIN_END:
			scope->type_ = IVL_SCT_BEGIN;
			scope->tname_ = scope->name_;
			break;
		case NetScope::FORK_JOIN:
			scope->type_ = IVL_SCT_FORK;
			scope->tname_ = scope->name_;
			break;
		}

		assert(scope->parent != 0);

		scope->sibling_= scope->parent->child_;
		scope->parent->child_ = scope;
	}
}

void dll_target::signal(const NetNet*net)
{
	ivl_signal_t obj = new struct ivl_signal_s;

	obj->name_ = net->name();

	/* Attach the signal to the ivl_scope_t object that contains
	it. This involves growing the sigs_ array in the scope
	object, or creating the sigs_ array if this is the first
	signal. */
	obj->scope_ = find_scope(des_, net->scope());
	assert(obj->scope_);

	if (obj->scope_->nsigs_ == 0) {
		assert(obj->scope_->sigs_ == 0);
		obj->scope_->nsigs_ = 1;
		obj->scope_->sigs_ = (ivl_signal_t*)malloc(sizeof(ivl_signal_t));

	} else {
		assert(obj->scope_->sigs_);
		obj->scope_->nsigs_ += 1;
		obj->scope_->sigs_ = (ivl_signal_t*)
			realloc(obj->scope_->sigs_,
			obj->scope_->nsigs_*sizeof(ivl_signal_t));
	}

	obj->scope_->sigs_[obj->scope_->nsigs_-1] = obj;


	/* Save the primitive properties of the signal in the
	ivl_signal_t object. */

	obj->width_ = net->pin_count();
	obj->signed_= net->get_signed()? 1 : 0;
	obj->lsb_index = net->lsb();
	obj->lsb_dist  = net->msb() >= net->lsb() ? 1 : -1;
	obj->isint_ = false;
	obj->local_ = (net->local_flag() && (net->peek_eref() == 0))? 1 : 0;

	switch (net->port_type()) {

	case NetNet::PINPUT:
		obj->port_ = IVL_SIP_INPUT;
		break;

	case NetNet::POUTPUT:
		obj->port_ = IVL_SIP_OUTPUT;
		break;

	case NetNet::PINOUT:
		obj->port_ = IVL_SIP_INOUT;
		break;

	default:
		obj->port_ = IVL_SIP_NONE;
		break;
	}

	switch (net->type()) {

	case NetNet::REG:
		obj->type_ = IVL_SIT_REG;
		obj->isint_ = net->get_isint();
		break;

	case NetNet::SUPPLY0:
		obj->type_ = IVL_SIT_SUPPLY0;
		break;

	case NetNet::SUPPLY1:
		obj->type_ = IVL_SIT_SUPPLY1;
		break;

	case NetNet::TRI:
	case NetNet::WIRE:
	case NetNet::IMPLICIT:
		obj->type_ = IVL_SIT_TRI;
		break;

	case NetNet::TRI0:
		obj->type_ = IVL_SIT_TRI0;
		break;

	case NetNet::TRI1:
		obj->type_ = IVL_SIT_TRI1;
		break;

	case NetNet::TRIAND:
	case NetNet::WAND:
		obj->type_ = IVL_SIT_TRIAND;
		break;

	case NetNet::TRIOR:
	case NetNet::WOR:
		obj->type_ = IVL_SIT_TRIOR;
		break;

	default:
		obj->type_ = IVL_SIT_NONE;
		break;
	}

	obj->nattr = net->attr_cnt();
	obj->attr = fill_in_attributes(net);


	/* Get the nexus objects for all the pins of the signal. If
	the signal has only one pin, then write the single
	ivl_nexus_t object into n.pin_. Otherwise, make an array of
	ivl_nexus_t cookies.

	When I create an ivl_nexus_t object, store it in the
	t_cookie of the Nexus object so that I find it again when I
	next encounter the nexus. */

	if (obj->width_ == 1) {
		const Nexus*nex = net->pin(0).nexus();
		if (nex->t_cookie()) {
			obj->n.pin_ = (ivl_nexus_t)nex->t_cookie();
			nexus_sig_add(obj->n.pin_, obj, 0);

		} else {
			ivl_nexus_t tmp = nexus_sig_make(obj, 0);
			tmp->name_ = strings_.add(nex->name());
			nex->t_cookie(tmp);
			obj->n.pin_ = tmp;
		}

	} else {
		unsigned idx;

		obj->n.pins_ = (ivl_nexus_t*)
			calloc(obj->width_, sizeof(ivl_nexus_t));

		for (idx = 0 ;  idx < obj->width_ ;  idx += 1) {
			const Nexus*nex = net->pin(idx).nexus();
			if (nex->t_cookie()) {
				obj->n.pins_[idx] = (ivl_nexus_t)nex->t_cookie();
				nexus_sig_add(obj->n.pins_[idx], obj, idx);

			} else {
				ivl_nexus_t tmp = nexus_sig_make(obj, idx);
				tmp->name_ = strings_.add(nex->name());
				nex->t_cookie(tmp);
				obj->n.pins_[idx] = tmp;
			}
		}
	}
}

extern const struct target tgt_dll = { "dll", &dll_target_obj };


/*
* $Log: t-dll.cc,v $
* Revision 1.131.2.8  2006/07/23 19:42:35  steve
*  Handle statement output override better in blocks.
*
* Revision 1.131.2.7  2006/04/16 19:26:40  steve
*  Fix handling of exploded memories with partial or missing resets.
*
* Revision 1.131.2.6  2006/03/26 23:09:24  steve
*  Handle asynchronous demux/bit replacements.
*
* Revision 1.131.2.5  2006/03/12 07:34:19  steve
*  Fix the memsynth1 case.
*
* Revision 1.131.2.4  2006/02/25 05:03:29  steve
*  Add support for negedge FFs by using attributes.
*
* Revision 1.131.2.3  2006/02/19 00:11:34  steve
*  Handle synthesis of FF vectors with l-value decoder.
*
* Revision 1.131.2.2  2006/01/21 21:42:33  steve
*  When mux has wide select but sparse choices, use 1hot translation.
*
* Revision 1.131.2.1  2005/02/19 16:39:31  steve
*  Spellig fixes.
*
* Revision 1.131  2004/10/04 01:10:55  steve
*  Clean up spurious trailing white space.
*
* Revision 1.130  2004/06/30 02:16:27  steve
*  Implement signed divide and signed right shift in nets.
*
* Revision 1.129  2004/02/20 18:53:35  steve
*  Addtrbute keys are perm_strings.
*
* Revision 1.128  2004/02/20 06:22:58  steve
*  parameter keys are per_strings.
*
* Revision 1.127  2004/02/19 06:57:10  steve
*  Memory and Event names use perm_string.
*
* Revision 1.126  2004/02/18 17:11:58  steve
*  Use perm_strings for named langiage items.
*
* Revision 1.125  2003/12/12 05:43:08  steve
*  Some systems dlsym requires leading _ or not on whim.
*
* Revision 1.124  2003/11/26 01:37:38  steve
*  Warning about sprintf.
*
* Revision 1.123  2003/11/13 05:55:33  steve
*  Move the DLL= flag to target config files.
*
* Revision 1.122  2003/11/10 20:59:04  steve
*  Design::get_flag returns const char* instead of string.
*
* Revision 1.121  2003/09/03 23:33:29  steve
*  Pass FF synchronous set values to code generator.
*
* Revision 1.120  2003/08/22 04:14:33  steve
*  Fix uninitialized sset member.
*
* Revision 1.119  2003/08/15 02:23:53  steve
*  Add synthesis support for synchronous reset.
*/

