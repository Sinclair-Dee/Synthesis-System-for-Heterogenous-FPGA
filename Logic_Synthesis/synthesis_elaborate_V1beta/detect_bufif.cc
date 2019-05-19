#ifdef HAVE_CVS_IDENT
#ident "$Id: detect_bufif,v 1.47.2.7 2013/10/25 10:24:38 steve Exp $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
# include  <assert.h>



struct detect_bufif_functor  : public functor_t {

	virtual void lpm_logic(Design*des, NetLogic*obj);
};


void detect_bufif_functor::lpm_logic(Design*des, NetLogic*obj)
{
	if((obj->type() == NetLogic::BUFIF0) || ((obj->type() == NetLogic::BUFIF1))){
		assert(obj->pin_count() == 3);
		//
		Nexus*nex = obj->pin(0).nexus();

		for (Link*cur = nex->first_nlink(); cur ;  cur = cur->next_nlink()){
			if (cur == &obj->pin(0))
				continue;
			NetNet*cursig = dynamic_cast<NetNet*>(cur->get_obj());

			if(cursig){
				continue;//连接到NetNet跳过				
			}else{//连接到一个逻辑单元上时，warning并删除bufif，，断开sel信号的连接，并将数据端直接连接到输出端
				//cerr << obj->get_line() << ": warning: bufif is not in output pad, so we replace it with BUF." << endl;
				cerr <<"Warning: bufif is is not in output pad, so we replace it with BUF." << endl;

				NetNet * bufif_out_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
				NetNet * bufif_in_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
				NetLogic* tmp_logic = new NetLogic(obj->scope(), obj->scope()->local_symbol(), 2, NetLogic::BUF);
				des->add_node(tmp_logic);

				connect(bufif_out_sig->pin(0), obj->pin(0));
				connect(bufif_in_sig->pin(0), obj->pin(1));

				connect(bufif_out_sig->pin(0), tmp_logic->pin(0));
				connect(bufif_in_sig->pin(0), tmp_logic->pin(1));

				delete obj;

				return;
			}
		}

		if(obj->type() == NetLogic::BUFIF0){
			NetNet * bufif_out_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
			NetNet * bufif_in_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
			NetNet * not_in_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);
			NetNet * not_out_sig = new NetNet(obj->scope(), obj->scope()->local_symbol(), NetNet::WIRE, 1);

			NetLogic* bufif1 = new NetLogic(obj->scope(), obj->scope()->local_symbol(), 3, NetLogic::BUFIF1);
			NetLogic* not = new NetLogic(obj->scope(), obj->scope()->local_symbol(), 2, NetLogic::NOT);

			des->add_node(bufif1);
			des->add_node(not);

			connect(bufif_out_sig->pin(0), obj->pin(0));
			connect(bufif_in_sig->pin(0), obj->pin(1));
			connect(bufif_out_sig->pin(0), bufif1->pin(0));
			connect(bufif_in_sig->pin(0), bufif1->pin(1));

			connect(not_in_sig->pin(0), obj->pin(2));//连接bufif1的选通端
			connect(not_in_sig->pin(0), not->pin(1));//连接NOT门的输入端

			connect(not_out_sig->pin(0), not->pin(0));//连接NOT门的输出端
			connect(not_out_sig->pin(0), bufif1->pin(2));//连接bufif1的选通端

			delete obj;

			return;
		}
	}
}
/*detect_bufif_functor:
1）首先检测三态门的输出端是否直接连接到output pad，如果不是则warning，并断开BUFIF的sel选通端，并用BUF替代BUFIF；
2）用BUFIF1替换BUFIF0,并在数据选通端添加NOT逻辑；**************************************************************/

void detect_bufif(Design*des)
{

	detect_bufif_functor det_bufif;
	des->functor(&det_bufif);
}