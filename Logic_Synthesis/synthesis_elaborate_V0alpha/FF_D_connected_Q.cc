#ifdef HAVE_CVS_IDENT
#ident "$Id: FF_D_connected_Q,v 1.47.2.7 2013/11/22 17:15:38 ADU $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
#include "NetLatch.h"
# include  <assert.h>



struct FF_D_connected_Q_functor  : public functor_t {
	
	virtual void lpm_ff(Design*des, NetFF*obj);
	
};

void FF_D_connected_Q_functor::lpm_ff(Design*des, NetFF*obj)
{
	
	
	//这里假定ff的位宽是1bit
	if (obj->pin_Data(0).nexus() == obj->pin_Q(0).nexus()) {
		assert(obj->pin_Aclr().is_linked() || obj->pin_Aset().is_linked());
		
		//if(obj->pin_Enable().is_linked())
			//cerr << "Error: "
		NetLatch * latch = new NetLatch(obj->scope(), obj->scope()->local_symbol(), 1U);
		des->add_node(latch);
		if(obj->pin_Aclr().is_linked())
			connect(latch->pin_Aclr(), obj->pin_Aclr());
		if(obj->pin_Aset().is_linked())
			connect(latch->pin_Aset(), obj->pin_Aset());

		//ADU:connect clk en port in obj to zero const
		assert(!obj->pin_Enable().is_linked());
		NetConst*zero = new NetConst(obj->scope(), obj->scope()->local_symbol(), verinum::V0);
		zero->set_line(*obj);
		des->add_node(zero);

		NetNet*zsig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
		zsig->local_flag(true);
		zsig->set_line(*obj);
		connect(zsig->pin(0), zero->pin(0));

		connect(zsig->pin(0), latch->pin_Data(0));
		connect(zsig->pin(0), latch->pin_Gate());

		connect(latch->pin_Q(0), obj->pin_Q(0));
		
		delete obj;
		
		return;
	}


}


void FF_D_connected_Q(Design*des)
{

	FF_D_connected_Q_functor ff_d_connected_q;
	des->functor(&ff_d_connected_q);

}