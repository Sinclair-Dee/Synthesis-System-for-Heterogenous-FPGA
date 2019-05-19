#ifdef HAVE_CVS_IDENT
#ident "$Id: del-redundance,v 1.47.2.7 2013/02/26 19:51:38 steve Exp $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
# include  <assert.h>



struct NetMemory2NetFF_functor  : public functor_t {

	virtual void lpm_memory(Design*des, NetMemory*mem);

};

void NetMemory2NetFF_functor::lpm_memory(Design*des, NetMemory*mem)
{
	
	NetMemoryRef* mem_ref = mem->get_ram_ref();
	NetNet * clk = 0;
	NetNet* waddr = 0;
	NetNet* datain = 0;
	NetNet* we = 0;
	if(mem_ref){
		NetNet * clk = mem_ref->get_clk();//write clock
		NetNet* waddr = mem_ref->get_waddr();
		NetNet* datain = mem_ref->get_datain();
		NetNet* we = mem_ref->get_we();
	}

	NetRamDq * NetRamDq_head = mem->get_ramlist();
	for(NetRamDq*ptr= NetRamDq_head; ptr; ptr = ptr->get_next_ramDq()){
		unsigned ff_count = ptr->size()*ptr->width();
		struct sync_accounting_cell*nex_ff = 
			new struct sync_accounting_cell[ff_count];
		for(unsigned idx = 0;idx < ff_count; idx++){
			NetFF *ff  = new NetFF(mem->scope(), mem->scope()->local_symbol(), 1U);
			des->add_node(ff);

			nex_ff[idx].ff = ff;
			nex_ff[idx].proc = 0;
			nex_ff[idx].pin = 0;
		}
		//connect datain
		for(unsigned idx = 0; idx < mem->count(); idx++){			
			assert(datain->pin_count() == mem->width());
			for(unsigned j = 0; j < mem->width(); j++){
				connect(datain->pin(j), nex_ff[idx*mem->width()+j].ff->pin_Data(0));
			}
		}
		
		//instantiate an decoder
		assert(mem->count() <= (1U << waddr->pin_count()));
		NetDecoder * decoder = new NetDecoder(mem->scope(), mem->scope()->local_symbol(), waddr->pin_count(), (1 << waddr->pin_count()));
		des->add_node(decoder);

		//decoder data

		for (unsigned idx = 0; idx < waddr->pin_count(); idx++){
			connect(waddr->pin(idx), decoder->pin_Data(idx));
		}
		


		//write enable

		for(unsigned idx = 0; idx < mem->count(); idx++){			

			if(we){
				NetLogic*ANDgate2 = new NetLogic(mem->scope(), mem->scope()->local_symbol(), 3, NetLogic::AND);
				des->add_node(ANDgate2);

				NetNet* decoder_out = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
				connect(decoder_out->pin(0),decoder->pin_Eq(idx));

				connect(decoder_out->pin(0), ANDgate2->pin(1));
				connect(we->pin(0), ANDgate2->pin(2));

				NetNet* ANDgate2_out = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
				connect(ANDgate2_out->pin(0), ANDgate2->pin(0));

				for (unsigned j = 0; j < mem->width(); j++){
					connect(ANDgate2_out->pin(0), nex_ff[idx*mem->width()+j].ff->pin_Enable());
				}
			}else{
				for (unsigned j = 0; j < mem->width(); j++){
					NetNet* decoder_result = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
					connect(decoder_result->pin(0), decoder->pin_Eq(idx));
					connect(decoder_result->pin(0), nex_ff[idx*mem->width()+j].ff->pin_Enable());
				}
			}
			
		}

		//mux
		unsigned sel_pins = ptr->awidth();
		unsigned data_with = mem->width();
		assert(mem->count() <= (1U << sel_pins));
		NetMux *mux = new NetMux(mem->scope(), mem->scope()->local_symbol(), data_with, 1U << sel_pins, sel_pins);

		//mux sel_pin
		for (unsigned idx = 0; idx < sel_pins; idx++){
			NetNet* sel_out = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
			connect(sel_out->pin(0), mux->pin_Sel(idx));
			connect(sel_out->pin(0), ptr->pin_RAddress(idx));
		}
		
		//mux data
		for (unsigned idx = 0; idx < mem->count(); idx++){
			for (unsigned j = 0; j < mem->width(); j++){
				NetNet* mux_datain = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
				connect(nex_ff[idx*mem->width()+j].ff->pin_Q(0), mux_datain->pin(0));
				connect(mux_datain->pin(0), mux->pin_Data(j, idx));
			}
		}
		
		//mux Q
		for (unsigned idx = 0; idx < mem->width(); idx++){
			NetNet* mux_dataout = new NetNet(mem->scope(), mem->scope()->local_symbol(),NetNet::WIRE, 1);
			connect(mux_dataout->pin(0), ptr->pin_Q(idx));
			connect(mux_dataout->pin(0), mux->pin_Result(idx));
		}
		//clk
		assert(clk->pin(0).is_linkedToSignificantSignal());
		for (unsigned idx = 0; idx < ff_count; idx++){
			connect(clk->pin(0), nex_ff[idx].ff->pin_Clock());
		}

		delete [] nex_ff;
	}

	NetRamDq*next = NetRamDq_head;
	while(next){
		NetRamDq *cur = next;
		next = next->get_next_ramDq();
		delete cur;
	}
	
}


void NetMemory2NetFF(Design*des)
{

	NetMemory2NetFF_functor memory2ff;
	des->functor(&memory2ff);
}