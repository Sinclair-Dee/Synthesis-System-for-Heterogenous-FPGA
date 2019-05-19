#ifdef HAVE_CVS_IDENT
#ident "$Id: del-redundance,v 1.47.2.7 2013/02/26 19:51:38 steve Exp $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
# include  <assert.h>



struct del_redundance_functor  : public functor_t {

	unsigned count;

	virtual void lpm_ff(Design*des, NetFF*obj);
	virtual void lpm_logic(Design*des, NetLogic*obj);
};

void del_redundance_functor::lpm_ff(Design*des, NetFF*obj)
{
	//这里假定ff的位宽是1bit  
	//if (!obj->pin_Data(0).is_linkedToSignificantSignal()) {
	//	delete obj;
	//	count += 1;
	//	return;
	//}

	//if (!obj->pin_Q(0).is_linkedToSignificantSignal()) {
	//	delete obj;
	//	count += 1;
	//	return;
	//}

}

void del_redundance_functor::lpm_logic(Design*des, NetLogic*obj)
{
	if(obj->type() == NetLogic::BUF){
		if (!obj->pin(0).is_linkedToSignificantSignal()) {
			delete obj;
			count += 1;
			return;
		}

		unsigned unlinked_count = 0;
		for (unsigned idx = 1 ;  idx < obj->pin_count() ;  idx += 1) {
			if(!obj->pin(idx).is_linkedToSignificantSignal())
				unlinked_count++;
		}
		if(unlinked_count == obj->pin_count() - 1){
			delete obj;
			count += 1;
			return;
		}
	}
}

void del_redundance(Design*des)
{
	// Continually propagate constants until a scan finds nothing
	// to do.
	del_redundance_functor del_red;
	do {
		del_red.count = 0;
		des->functor(&del_red);
	} while (del_red.count > 0);
}