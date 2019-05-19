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

	if((net->port_type() == NetNet::POUTPUT) || (net->port_type() == NetNet::PINOUT)){//ֻ����������ж�
		for(unsigned i = 0; i < net->pin_count(); i ++){
			//link & pin = net->pin(i);
			Nexus*nex = net->pin(i).nexus();

			for (Link*cur = nex->first_nlink(); cur ;  cur = cur->next_nlink()) {
				if (cur == &net->pin(i))
					continue;

				NetNet*cursig = dynamic_cast<NetNet*>(cur->get_obj());
				if (cursig){
					if((cursig->port_type() == NetNet::PINPUT) && cursig->scope() == net->scope()){//ֻ�е�output�ź����ӵ�ͬһscope�ڵ�input�ź�ʱ����������
						net->pin(i).unlink();//�Ƚ���output���ŶϿ�

						NetNet* tmp_net_in= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);
						NetNet* tmp_net_out= new NetNet(net->scope(), net->scope()->local_symbol(), NetNet::WIRE, 1);//�½�����wire������BUF�����롢����˿ڣ�
						NetLogic* tmp_logic = new NetLogic(net->scope(), net->scope()->local_symbol(), 2, NetLogic::BUF);//�ٽ��½��������������ӵ�ouput�����ź�֮ǰ�Ͽ�����������
						des->add_node(tmp_logic);

						connect(net->pin(i), tmp_net_out->pin(0));
						connect(tmp_logic->pin(0), tmp_net_out->pin(0));
						connect(tmp_logic->pin(1), tmp_net_in->pin(0));
						connect(tmp_net_in->pin(0), *cur);

						break;//�������������ͬһscope�ڣ����ֻ��һ��input�˿�ֱ�����ӵ���output�˿ڵĵ�ǰpin�ϡ���˵������ҵ�����һ��input����ʱ������ѭ��
					}
				}else{
					continue;
				}
			}
		}
	}
}


/*���functor����Ҫ�����Ǽ����ۺϵĽ���У���ͬһscope���Ƿ�������˿�pin������˿ڵ�pinֱ�������������***
**����У�����������˿ڵĵ�ǰpin������˿ڵĵ�ǰpin֮�����һ��NetLogic::BUF��****************************
**�������ĳ�������blif�ļ����ʱ�������������˿�������˿�ֱ�����������ʱ����������������˿ڶ�ʧ�������
**��Ϊ��һ������net�У���������ж˿��źţ����Ǿ��øö˿��ź��������������net���������ͬһ����net�У�****
**ͬʱ���������˿ڣ���ʧ����һ���˿ڵ�������Ϣ��**********************************************************
**��bug�ڲ��Ե�·��iir1.vʱ���֡���*/
void detect_input_output_linked_derectly(Design*des)
{

	detect_input_output_linked_derectly_functor det_in_out_linked;
	des->functor(&det_in_out_linked);

}