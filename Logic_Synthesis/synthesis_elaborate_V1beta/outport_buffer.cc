#ifdef HAVE_CVS_IDENT
#ident "$Id: detect_input_output_linked_derectly,v 1.47.2.7 2013/10/24 10:50:38 ADU Exp $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
# include  <assert.h>



struct outport_buffer_functor  : public functor_t {

	virtual void signal(class Design*des, class NetNet*);
};

void outport_buffer_functor::signal(Design*des, NetNet* net)
{

	if(net->port_type() == NetNet::POUTPUT && !net->scope()->parent()){
		for(unsigned i = 0; i < net->pin_count(); i ++){
							
			NetNet* tmp_net_in= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);
			NetNet* tmp_net_out= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);
			NetLogic* buffer = new NetLogic(net->scope(), net->scope()->local_symbol(), 2, NetLogic::BUF);
			des->add_node(buffer);

			connect(buffer->pin(0), tmp_net_out->pin(0));
			connect(buffer->pin(1), tmp_net_in->pin(0));
			connect(tmp_net_in->pin(0), net->pin(i));

			net->pin(i).unlink();
			connect(net->pin(i), tmp_net_out->pin(0));
		}			
	}
}


void insert_outport_buffer(Design*des)
{

	outport_buffer_functor outport_buffer;
	des->functor(&outport_buffer);

}