#ifdef HAVE_CVS_IDENT
#ident "$Id: detect_input_output_linked_derectly,v 1.47.2.7 2013/10/24 10:50:38 ADU Exp $"
#endif

# include "config.h"

# include  "netlist.h"
# include  "netmisc.h"
# include  "functor.h"
# include  "compiler.h"
# include  <assert.h>



struct detect_input_output_linked_derectly_functor  : public functor_t {

	virtual void signal(class Design*des, class NetNet*);
};

void detect_input_output_linked_derectly_functor::signal(Design*des, NetNet* net)
{

	if((net->port_type() == NetNet::POUTPUT) || (net->port_type() == NetNet::PINOUT)){//只对输出引脚判断
		for(unsigned i = 0; i < net->pin_count(); i ++){
			//link & pin = net->pin(i);
			Nexus*nex = net->pin(i).nexus();

			for (Link*cur = nex->first_nlink(); cur ;  cur = cur->next_nlink()) {
				if (cur == &net->pin(i))
					continue;

				NetNet*cursig = dynamic_cast<NetNet*>(cur->get_obj());
				if (cursig){
					if((cursig->port_type() == NetNet::PINPUT) && cursig->scope() == net->scope()){//只有当output信号连接到同一scope内的input信号时，才作处理
						net->pin(i).unlink();//先将该output引脚断开

						NetNet* tmp_net_in= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);
						NetNet* tmp_net_out= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);//新建两条wire，连接BUF的输入、输出端口，
						NetLogic* tmp_logic = new NetLogic(net->scope(), net->scope()->local_symbol(), 2, NetLogic::BUF);//再将新建的两条线网连接到ouput的引脚和之前断开的连接线网
						des->add_node(tmp_logic);

						connect(net->pin(i), tmp_net_out->pin(0));
						connect(tmp_logic->pin(0), tmp_net_out->pin(0));
						connect(tmp_logic->pin(1), tmp_net_in->pin(0));
						connect(tmp_net_in->pin(0), *cur);

						break;//我们这里断言在同一scope内，最多只有一条input端口直接连接到该output端口的当前pin上。因此当我们找到这样一个input引脚时就跳出循环
					}
				}else{
					continue;
				}
			}
		}
	}
}


/*这个functor的主要作用是检验综合的结果中，在同一scope内是否有输出端口pin与输入端口的pin直接相连的情况，***
**如果有，我们在输出端口的当前pin与输入端口的当前pin之间插入一个NetLogic::BUF。****************************
**这样做的初衷是在blif文件输出时，如果出现输出端口与输入端口直接相连的情况时，会出现输入或输出端口丢失的情况，
**因为在一个线网net中，如果出现有端口信号，我们就用该端口信号名来代替该线网net名，如果在同一线网net中，****
**同时出现两个端口，则丢失其中一个端口的连接信息。**********************************************************
**该bug在测试电路“iir1.v时出现。”*/
void detect_input_output_linked_derectly(Design*des)
{

	detect_input_output_linked_derectly_functor det_in_out_linked;
	des->functor(&det_in_out_linked);

}