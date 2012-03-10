
module DE2
	(
		////////////////////	Clock Input	 	////////////////////	 
		OSC_27,							//	27 MHz
		OSC_50,							//	50 MHz
		EXT_CLOCK,						//	External Clock
		////////////////////	Push Button		////////////////////
		KEY,							//	Button[3:0]
		////////////////////	DPDT Switch		////////////////////
		DPDT_SW,						//	DPDT Switch[17:0]
		////////////////////	7-SEG Dispaly	////////////////////
		HEX0,							//	Seven Segment Digital 0
		HEX1,							//	Seven Segment Digital 1
		HEX2,							//	Seven Segment Digital 2
		HEX3,							//	Seven Segment Digital 3
		HEX4,							//	Seven Segment Digital 4
		HEX5,							//	Seven Segment Digital 5
		HEX6,							//	Seven Segment Digital 6
		HEX7,							//	Seven Segment Digital 7
		////////////////////////	LED		////////////////////////
		LED_GREEN,						//	LED Green[8:0]
		LED_RED,						//	LED Red[17:0]
		////////////////////////	UART	////////////////////////
		UART_TXD,						//	UART Transmitter
		UART_RXD,						//	UART Rceiver
		TD_RESET
		
	);

////////////////////////	Clock Input	 	////////////////////////
input			OSC_27;					//	27 MHz
input			OSC_50;					//	50 MHz
input			EXT_CLOCK;				//	External Clock
////////////////////////	Push Button		////////////////////////
input	[3:0]	KEY;					//	Button[3:0]
////////////////////////	DPDT Switch		////////////////////////
input	[17:0]	DPDT_SW;				//	DPDT Switch[17:0]
////////////////////////	7-SEG Dispaly	////////////////////////
output	[6:0]	HEX0;					//	Seven Segment Digital 0
output	[6:0]	HEX1;					//	Seven Segment Digital 1
output	[6:0]	HEX2;					//	Seven Segment Digital 2
output	[6:0]	HEX3;					//	Seven Segment Digital 3
output	[6:0]	HEX4;					//	Seven Segment Digital 4
output	[6:0]	HEX5;					//	Seven Segment Digital 5
output	[6:0]	HEX6;					//	Seven Segment Digital 6
output	[6:0]	HEX7;					//	Seven Segment Digital 7
////////////////////////////	LED		////////////////////////////
output	[8:0]	LED_GREEN;				//	LED Green[8:0]
output	[17:0]	LED_RED;				//	LED Red[17:0]
////////////////////////////	UART	////////////////////////////
output			UART_TXD;				//	UART Transmitter
input			UART_RXD;				//	UART Rceiver
output TD_RESET;
//	USB JTAG
wire [7:0] mRXD_DATA,mTXD_DATA;
wire mRXD_Ready,mTXD_Done,mTXD_Start;
wire mTCK;
//	SEG7
wire [31:0] mSEG7_DIG;
//	AI
wire [63:0] DATA_from_AI,DATA_to_AI;
wire mAI_Start,mAI_Done;
wire [7:0] mCOLOR;

assign TD_RESET = 1'b1;




SEG7_LUT_8 			u0	(	HEX0,HEX1,HEX2,HEX3,HEX4,HEX5,HEX6,HEX7,mSEG7_DIG );

wire mTXD_Done_not;
RS232_Controller 	u1_bis(		.iDATA(mTXD_DATA),.iTxD_Start(mTXD_Start),.oTxD_Busy(mTXD_Done_not),
							.oDATA(mRXD_DATA),.oRxD_Ready(mRXD_Ready),.iCLK(OSC_27),.RST_n(KEY[0]),
							.oTxD(UART_TXD),.iRxD(UART_RXD));
assign mTXD_Done = !mTXD_Done_not;
assign LED_RED[9] = mTXD_Done_not;
assign LED_RED[10] = ~mAI_Done;



assign UART_RXD_JP1_7 = UART_RXD;
assign UART_TXD_JP1_50 = UART_TXD;


CMD_Decode			u5	(	//	USB JTAG
							.iRXD_DATA(mRXD_DATA),.iRXD_Ready(mRXD_Ready),
						 	.oTXD_DATA(mTXD_DATA),.oTXD_Start(mTXD_Start),.iTXD_Done(mTXD_Done),
							//	Control
						 	.iCLK(OSC_27),.iRST_n(KEY[0]), .oAI_RSTn(mAI_RSTn),
							//AI
							.oAI_DATA(DATA_to_AI),
							.iAI_DATA(DATA_from_AI),
							.oAI_Start(mAI_Start),
							.iAI_Done(mAI_Done),.oCOLOR(mCOLOR),.d_cmd(CMD_Tmp[16:0]) );
AI 			inst_AI (
							.oAI_DATA(DATA_from_AI),
							.iAI_DATA(DATA_to_AI),
							.iCOLOR(mCOLOR),
							.imovecount(CMD_Tmp[16:0]),
							.iAI_Start(mAI_Start),
							.oAI_Done(mAI_Done),
							
							//	Control
						 	.iCLK(OSC_27),.iRST_n(mAI_RSTn)	);
wire [63:0] CMD_Tmp;					

//assign	mSEG7_DIG	=	{	CMD_Tmp[31:28],CMD_Tmp[27:24],CMD_Tmp[23:20],CMD_Tmp[19:16],
//							CMD_Tmp[15:12],CMD_Tmp[11:8],CMD_Tmp[7:4],CMD_Tmp[3:0]	};
assign	mSEG7_DIG	=	{	
							DATA_to_AI[63:60],DATA_to_AI[59:56],DATA_to_AI[55:52],DATA_to_AI[51:48],
							DATA_to_AI[47:44],DATA_to_AI[43:40],DATA_to_AI[39:36],DATA_to_AI[35:32]	}
//							DATA_from_AI[31:28],DATA_from_AI[27:24],DATA_from_AI[23:20],DATA_from_AI[19:16],
//							DATA_from_AI[15:12],DATA_from_AI[11:8],DATA_from_AI[7:4],DATA_from_AI[3:0]	}
				;
							
							
endmodule
