module layer_inter_middle_control(

		 enable,
		 reset,
		 clock,

	//layer_former 2 enable reset signals
		  layer_former_enable,
		  layer_former_reset,	
		
		
		  layer_next_enable,
		  layer_next_reset,	

		 layer_former_done,
		 layer_next_done,
   //layer_former read write address signal
		  rden_a_layer_former,
		  rden_b_layer_former,
		  wren_a_layer_former,
		  wren_b_layer_former,

			address_a_layer_former,
			address_b_layer_former,
		
		  rden_a_former_after_mux,
		  rden_b_former_after_mux,
		  wren_a_former_after_mux,
		  wren_b_former_after_mux,
		

	
			address_a_former_after_mux,
			address_b_former_after_mux,
		
//layer_next 	read write address signal	
			address_a_layer_next ,
			address_b_layer_next ,


		 rden_a_layer_next  ,
		 rden_b_layer_next  ,
		 wren_a_layer_next  ,
		 wren_b_layer_next  ,


//layer next next
		layer_nextnext_done


);

	parameter   LAYER_FORMER_INFEATURE_ADDR_WIDTH=9;

	
	localparam	INITIAL=0;
	localparam	LAYER_NEXT_IDLE=1;
	localparam	LAYER_NEXT_COMPUT=2;

	localparam  NUM_STATES_WIDTH=2;
	//
		input enable;
		input reset;
		input clock;
		input layer_nextnext_done;
	//layer_former 2 enable reset signals
		input  layer_former_enable;
		input  layer_former_reset;	
		
		
		output reg layer_next_enable;
		output  layer_next_reset;	

		input layer_former_done;
		input layer_next_done;
   //layer_former read write address signal
		input  rden_a_layer_former;
		input  rden_b_layer_former;
		input  wren_a_layer_former;
		input  wren_b_layer_former;

		input [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_a_layer_former;
		input [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_b_layer_former;
		
		output reg rden_a_former_after_mux;
		output reg rden_b_former_after_mux;
		output reg wren_a_former_after_mux;
		output reg wren_b_former_after_mux;
		

	
		output reg [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_a_former_after_mux;
		output reg [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_b_former_after_mux;
		
//layer_next 	read write address signal	
		input [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_a_layer_next ;
		input [LAYER_FORMER_INFEATURE_ADDR_WIDTH-1:0] address_b_layer_next ;


		input rden_a_layer_next  ;
		input rden_b_layer_next  ;
		input wren_a_layer_next  ;
		input wren_b_layer_next  ;

	reg layer_nextnext_done_index;
	reg layer_nextnext_done_delay;
//internel signal

	reg [NUM_STATES_WIDTH - 1 : 0] state;
	reg layer_former_done_delay;
	reg layer_next_enable_delay;
	
	reg former_index;
	reg nextnext_index;
	reg index;
	reg index_delay;
	
	always @ (posedge clock)
	begin

		layer_next_enable_delay <= layer_next_enable;
		layer_former_done_delay <= layer_former_done;
		layer_nextnext_done_delay <= layer_nextnext_done;	
			index_delay<=index;
	end
	

always @ (posedge clock)
begin

	
	if(reset) begin
		state <= INITIAL;
		former_index <= 0;
		nextnext_index <= 0;
	
	end
	else begin
		case (state)
			
			INITIAL:
			begin 
				state <= LAYER_NEXT_IDLE;
					layer_nextnext_done_index <= 1;
			end
			LAYER_NEXT_IDLE: 
			begin
				if(layer_nextnext_done_index ==0  &&layer_former_done==1 &&layer_former_done_delay ==0 ) former_index <= 1;
				if(layer_nextnext_done_index ==0  &&layer_nextnext_done==1 &&layer_nextnext_done_delay ==0 ) nextnext_index <= 1;
				
				
				if ((layer_nextnext_done_index ==1 &&layer_former_done==1 )|| 
				(layer_nextnext_done_index ==0 && former_index==1 && nextnext_index ==1 ))
				
				
				begin state<=LAYER_NEXT_COMPUT; layer_nextnext_done_index <= 0; index <= 0 ;end
			end
			
			LAYER_NEXT_COMPUT:
			begin
				former_index <= 0;
				nextnext_index <=0;
				if (layer_next_done==1)  begin state<=LAYER_NEXT_IDLE;index <= 1;
													
												end
			end
			

			
			default:
			begin
				state <= INITIAL;
				
			end
		endcase
	end
end


always @ (*)
begin
	case (state)
		INITIAL:
		begin
		


			layer_next_enable <= 0;
		
			rden_a_former_after_mux <= 0;
			rden_b_former_after_mux <= 0;
			wren_a_former_after_mux <= 0;
			wren_b_former_after_mux <= 0;
	
			address_a_former_after_mux <= 0;
			address_b_former_after_mux <= 0;
		end
		LAYER_NEXT_IDLE: 
		begin
		

			layer_next_enable <= 0;
			

			rden_a_former_after_mux <= rden_a_layer_former;
			rden_b_former_after_mux <= rden_b_layer_former;
			wren_a_former_after_mux <= wren_a_layer_former;
			wren_b_former_after_mux <= wren_b_layer_former;
	
			address_a_former_after_mux <= address_a_layer_former;
			address_b_former_after_mux <= address_b_layer_former;
		end
		
		LAYER_NEXT_COMPUT: 
		begin
		
		
			layer_next_enable <= 1;

			
			rden_a_former_after_mux <= rden_a_layer_next;
			rden_b_former_after_mux <= rden_b_layer_next;
			wren_a_former_after_mux <= wren_a_layer_former;
			wren_b_former_after_mux <= wren_b_layer_former;
	
			address_a_former_after_mux <= address_a_layer_next;
			address_b_former_after_mux <= address_b_layer_next;

		end

		default:
		begin


			layer_next_enable <= 0;
			rden_a_former_after_mux <= 0;
			rden_b_former_after_mux <= 0;
			wren_a_former_after_mux <= 0;
			wren_b_former_after_mux <= 0;
	
			address_a_former_after_mux <= 0;
			address_b_former_after_mux <= 0;

		end 
	endcase
end


assign layer_next_reset=((layer_next_enable_delay==0) &&(layer_next_enable==1))? 1:0;

endmodule 


