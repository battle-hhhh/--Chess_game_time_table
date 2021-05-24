#define	u8	unsigned char
#define u16 unsigned int
#define T1MS (65536-FOSC/12/500)   /*0.5ms timer calculation method in 12Tmode*/
 
#define	const_voice_short	40	/*蜂鸣器短叫的持续时间*/
#define	const_voice_long	900	/*蜂鸣器长叫的持续时间*/
#define const_key_time 9	/*按键去抖动延时的时间*/

#define const_1s  96	/*大概产生一秒钟的时间基准*/

extern void outportb( unsigned int, char);						//写I/O
extern char inportb( unsigned int );							//读I/O
extern void enable( void );										//开中断
extern void disable( void );									//关中断
extern void setvect( int, void interrupt(*isr)( void ) );		//写中断向量
extern void interrupt( far *  getvect( int __interruptno ))( );	//读中断向量

//对键盘、数码管管理器初始化
extern void cInitKeyDisplay();
//将pBuffer指向的8字节缓冲区内容显示于G5区数码管上
extern void cDisplay8(u8* pBuffer);
//接收一个16 进制键值，如果没有按键，返回0xff
extern u8 cGetKeyA();

#define	IO8259_0	0x250
#define IO8259_1	0x251
#define Con_8253	0x263				//8253控制口
#define T0_8253		0x260
//宏定义重复

u8 halfsec;			//0.5秒计数
u8 bNeedDisplay;	//需要刷新显示
u8 bFlash;			//设置时是否需要刷新
u8 time_pulse = 0;		//用来决定在暂停的时候(round == 3)送上升沿脉的时间间隔冲

/*每方需在10分钟内走满40着。
以后每5分钟必须走满30着，直至对局结束*/

u8 red_count = 0x00;//表示红棋目前走的步数，移动一次或者吃掉对方一子都算一次移动
u8 red_round_count = 0x14;//该阶段红旗所走的步数
u8 black_count = 0x00;//表示黑棋目前走的步数
u8 black_round_count = 0x14;

u8 count_limit = 40;	//当前回合的步数限制
u8 round_flag = 0;	//阶段号

u8 red_min = 0x00;		//红棋初始时间默认为10分钟0秒
u8 red_sec = 0x14;
u8 black_min = 0x00;		//黑棋初始时间默认为10分钟0秒	
u8 black_sec = 0x14;

u16 round = 0x00;				//值为0表示红棋的回合，值为1表示黑棋的回合，象棋中默认红棋先走，初始状态round值为0
u8 PauseFlag = 0x00;			//暂停标志，值为1表示已经暂停红黑双方的计时，默认为0，则表示计时状态，此时有且仅有一位选手的时钟在计时
u8 ResetFlag = 0x00;			//复位标志，值为1时复位，反之不复位
u8 Winner = 3;					//值为0表示红方胜利，值为1表示黑方胜利；其它值表示未决出胜负

#define WR_COM_AD_L		0x234				//写左半屏指令地址
#define WR_COM_AD_R		0x230				//写右半屏指令地址
#define WR_DATA_AD_L 	0x236				//写左半屏数据地址	
#define WR_DATA_AD_R	0x232				//写右半屏数据地址
#define RD_BUSY_AD 		0x231				//查忙地址
#define RD_DATA_AD 		0x233				//读数据地址

#define X 			0xB8							//起始显示行基址
#define Y			0x40							//起始显示列基址
#define FirstLine	0x00							//起始显示行

/*--  文字:  棋  --*/
/*--  幼圆15;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Title_1[] = {
	0x00,0x10,0x90,0xFE,0x90,0x08,0x08,0xFC,0x48,0x48,0x48,0x48,0xFC,0x08,0x08,0x00,
	0x08,0x06,0x01,0xFF,0x01,0x90,0x90,0x5F,0x32,0x12,0x12,0x32,0x5F,0x50,0x90,0x00};

/*--  文字:  王  --*/
/*--  幼圆15;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Title_2[] = {
	0x00,0x00,0x04,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,
	0x00,0x80,0x81,0x81,0x81,0x81,0x81,0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x80,0x00};

//0x00,0x40,0x20,0x50,0x4C,0x4A,0x48,0x48,0xC8,0x68,0x58,0x40,0x40,0x80,0x00,0x00,
//0x00,0x00,0x02,0x12,0x92,0x92,0x12,0x92,0x7F,0x12,0x12,0x12,0x0A,0x07,0x00,0x00,

/*--  文字:  争  --*/
/*--  幼圆15;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Title_3_L[] = {
	0x00,0x40,0x20,0x50,0x4C,0x4A,0x48,0x48, 0x00,0x00,0x02,0x12,0x92,0x92,0x12,0x92};
	
unsigned char Title_3_R[] = {
	0xC8,0x68,0x58,0x40,0x40,0x80,0x00,0x00, 0x7F,0x12,0x12,0x12,0x0A,0x07,0x00,0x00};

/*--  文字:  霸  --*/
/*--  幼圆15;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Title_4[] = {
	0x00,0xF8,0x8A,0x9A,0xDA,0xDA,0x9A,0x8A,0x7E,0x5A,0xDA,0xDA,0xDA,0xDA,0x38,0x00,
	0x00,0x48,0x55,0x56,0xFE,0x56,0x67,0x5C,0x81,0x7F,0x14,0x94,0x94,0xFF,0x00,0x00};

/*--  文字:  赛  --*/
/*--  幼圆15;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Title_5[] = {
	0x00,0x38,0x58,0x58,0x58,0xF8,0x58,0x5E,0x58,0x58,0xF8,0x58,0x58,0x58,0x38,0x00,
	0x00,0x09,0x05,0xBB,0x85,0x85,0x45,0x3D,0x05,0x25,0x45,0x85,0xBB,0x05,0x09,0x00};


	
/*--  文字:  红  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Red_1[] = {
	0x00,0x20,0x30,0xAC,0x63,0x20,0x10,0x00,0x20,0x20,0xF0,0x10,0x10,0x00,0x00,0x00,
	0x00,0x10,0x13,0x0A,0x09,0x05,0x10,0x10,0x10,0x10,0x0F,0x08,0x08,0x08,0x08,0x00};

/*--  文字:  方  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Red_2[] = {
	0x00,0x20,0x20,0x20,0x20,0x20,0x22,0xE2,0x94,0x90,0x90,0x10,0x10,0x10,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x11,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x00,0x00};
/*--  文字:  左  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Red_3[] = {
	0x00,0x00,0x00,0x20,0x20,0xA0,0x60,0x1F,0x10,0x90,0x90,0x90,0x00,0x00,0x00,0x00,
	0x00,0x10,0x08,0x24,0x22,0x21,0x21,0x21,0x1F,0x10,0x10,0x10,0x10,0x10,0x00,0x00};
	

/*--  文字:  黑  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Black_1[] = {
	0x00,0x00,0x00,0x3C,0x44,0x54,0xFE,0xA2,0xAA,0xA2,0x32,0x0E,0x00,0x00,0x00,0x00,
	0x00,0x20,0x18,0x04,0x15,0x25,0x03,0x0A,0x12,0x02,0x02,0x08,0x10,0x30,0x00,0x00};

/*--  文字:  方  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/	
unsigned char Black_2[] = {
	0x00,0x20,0x20,0x20,0x20,0x20,0x22,0xE2,0x94,0x90,0x90,0x10,0x10,0x10,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x11,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x00,0x00};
/*--  文字:  右  --*/
/*--  幼圆12;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Black_3[] = {
	0x00,0x20,0x20,0x20,0x20,0xA0,0x60,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
	0x10,0x10,0x08,0x04,0x02,0x3F,0x12,0x12,0x12,0x11,0x19,0x07,0x00,0x00,0x00,0x00};

/*--  文字:  按  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Start_1[] = {
	0x00,0x20,0x20,0xFF,0x90,0x00,0x60,0x20,0x20,0xD2,0x14,0x10,0x50,0x30,0x00,0x00,
	0x02,0x02,0x11,0x3F,0x00,0x22,0x22,0x12,0x17,0x09,0x0D,0x13,0x21,0x21,0x01,0x00};

/*--  文字:  d  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Start_2[] = {
	0x00,0x80,0x40,0x40,0x48,0xF8,0x00,0x00,0x00,0x1F,0x10,0x20,0x20,0x1F,0x10,0x00};
	
/*--  文字:  开  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Start_3[] = {
	0x00,0x80,0x80,0x84,0x84,0xFC,0x44,0x44,0xFE,0x22,0x22,0x20,0x20,0x20,0x00,0x00,
	0x00,0x20,0x10,0x08,0x06,0x01,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*--  文字:  始  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Start_4[] = {
	0x80,0x80,0x80,0xE0,0x5E,0x40,0xE0,0x00,0xC0,0xB0,0x8E,0x40,0x60,0xC0,0x00,0x00,
	0x00,0x10,0x10,0x0B,0x04,0x0B,0x00,0x00,0x1C,0x12,0x12,0x12,0x0E,0x00,0x00,0x00};
/*--  文字:  ,  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Start_5[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00};
/*--  文字:  先  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Start_6[] = {
	0x00,0x00,0x80,0xA0,0x9C,0x90,0x90,0xFF,0x48,0x48,0x48,0x40,0x00,0x00,0x00,0x00,
	0x20,0x20,0x10,0x08,0x04,0x03,0x00,0x1F,0x20,0x20,0x20,0x20,0x20,0x38,0x00,0x00};
/*--  文字:  走  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Start_7[] = {
	0x00,0x80,0x80,0x80,0x90,0x90,0x90,0xFF,0x48,0x48,0x48,0x40,0x00,0x00,0x00,0x00,
	0x20,0x10,0x08,0x04,0x03,0x04,0x08,0x0F,0x11,0x31,0x21,0x20,0x20,0x20,0x20,0x00};

/*--  文字:  0  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_0[] = {
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00};

/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_1[] = {
	0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00};

/*--  文字:  2  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_2[] = {
	0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00};

/*--  文字:  3  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_3[] = {
	0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00};

/*--  文字:  4  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_4[] = {
	0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00};

/*--  文字:  5  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_5[] = {
	0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00};

/*--  文字:  6  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_6[] = {
	0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00};

/*--  文字:  7  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_7[] = {
	0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00};

/*--  文字:  8  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_8[] = {
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00};

/*--  文字:  9  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char Number_9[] = {
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00};

/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
unsigned char MaoHao[] = {
	0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00};

/*--  文字:  游  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Over_1[] = {
	0x00,0x20,0x44,0x08,0x20,0xE0,0x92,0x94,0x10,0x28,0xAE,0x68,0x24,0x04,0x00,0x00,
	0x00,0x0C,0x03,0x04,0x02,0x19,0x0C,0x03,0x02,0x12,0x22,0x1F,0x01,0x01,0x01,0x00};
/*--  文字:  戏  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Over_2[] = {
	0x00,0x20,0xA0,0x90,0x10,0xF0,0x00,0x40,0x7F,0xC0,0x20,0x24,0x88,0x00,0x00,0x00,
	0x10,0x08,0x04,0x02,0x01,0x02,0x14,0x10,0x08,0x05,0x06,0x09,0x10,0x20,0x38,0x00};
/*--  文字:  结  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/	
unsigned char Over_3[] = {
	0x00,0x60,0x50,0xCC,0x40,0x30,0x40,0x40,0x40,0xFE,0x20,0x20,0x20,0x20,0x00,0x00,
	0x00,0x12,0x13,0x0A,0x09,0x05,0x00,0x3A,0x2A,0x25,0x25,0x15,0x1D,0x00,0x00,0x00};
/*--  文字:  束  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Over_4[] = {
	0x00,0x00,0x00,0x60,0xA8,0xA8,0xA8,0xFF,0x94,0x54,0x70,0x00,0x00,0x00,0x00,0x00,
	0x10,0x10,0x08,0x08,0x04,0x02,0x01,0x7F,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x00};
/*--  文字:  胜  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Win[] = {
	0x00,0x00,0xF8,0x24,0x24,0xFC,0x40,0x38,0x20,0x20,0xFE,0x90,0x90,0x00,0x00,0x00,
	0x20,0x18,0x07,0x01,0x11,0x3F,0x10,0x10,0x11,0x11,0x0F,0x08,0x08,0x08,0x00,0x00};

/*--  文字:  休  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char PauseTip_1[] = {
	0x00,0x80,0xE0,0x18,0x06,0x20,0x20,0xA0,0xFF,0x90,0x10,0x10,0x00,0x00,0x00,0x00,
	0x01,0x00,0x3F,0x00,0x08,0x04,0x03,0x00,0x7F,0x00,0x01,0x02,0x04,0x04,0x04,0x00};
/*--  文字:  赛  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char PauseTip_2[] = {
	0x00,0x00,0x8C,0x94,0x54,0xFC,0x55,0x7E,0x4A,0xCA,0x42,0x46,0x00,0x00,0x00,0x00,
	0x08,0x08,0x44,0x42,0x2F,0x11,0x0D,0x11,0x2F,0x60,0x01,0x02,0x06,0x04,0x04,0x00};

/*--  文字:  按  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_1[] = {
	0x00,0x20,0x20,0xFF,0x90,0x00,0x60,0x20,0x20,0xD2,0x14,0x10,0x50,0x30,0x00,0x00,
	0x02,0x02,0x11,0x3F,0x00,0x22,0x22,0x12,0x17,0x09,0x0D,0x13,0x21,0x21,0x01,0x00};
/*--  文字:  b  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_2[] = {
	0x00,0x08,0xF8,0x40,0x40,0xC0,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x0F,0x00};
/*--  文字:  键  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_3[] = {
	0x80,0x40,0x70,0xCE,0x48,0x18,0xF0,0x00,0xA8,0xA8,0xFF,0x54,0x7C,0x10,0x10,0x00,
	0x00,0x02,0x02,0x1F,0x29,0x14,0x0D,0x0B,0x14,0x15,0x1F,0x25,0x24,0x20,0x20,0x00};
/*--  文字:  再  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_4[] = {
	0x00,0x00,0x00,0x02,0xF2,0x12,0x52,0xFE,0x29,0x09,0xF9,0x01,0x00,0x00,0x00,0x00,
	0x02,0x02,0x02,0x02,0x1F,0x02,0x02,0x01,0x11,0x21,0x1F,0x01,0x01,0x01,0x01,0x00};
/*--  文字:  来  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_5[] = {
	0x00,0x80,0x80,0xA8,0xC8,0x88,0xFF,0x84,0x64,0x54,0x40,0x40,0x00,0x00,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x7F,0x01,0x02,0x04,0x0C,0x08,0x08,0x08,0x08,0x00};
/*--  文字:  一  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_6[] = {
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x40,0x40,0x40,0x40,0xC0,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*--  文字:  局  --*/
/*--  楷体_GB231212;  此字体下对应的点阵为：宽x高=16x16   --*/
unsigned char Restart_7[] = {
	0x00,0x00,0x00,0x00,0xFC,0x94,0x92,0x92,0x52,0x4E,0x40,0x40,0xC0,0x00,0x00,0x00,
	0x10,0x08,0x04,0x03,0x0E,0x0A,0x0A,0x0A,0x06,0x20,0x40,0x20,0x1F,0x00,0x00,0x00};
	
	
//延时程序
void DelayTime()
{
	unsigned char i;
	unsigned int j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 0xffff; j++)
		{;}
	}
}

//***************************************
//基本控制		
//***************************************					
//写左半屏控制指令
void WRComL(unsigned char _data)
{
	outportb(WR_COM_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏控制指令
void WRComR(unsigned char _data)
{
	outportb(WR_COM_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写左半屏数据
void WRDataL(unsigned char _data)
{
	outportb(WR_DATA_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏数据
void WRDataR(unsigned char _data)
{
	outportb(WR_DATA_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}
//显示左半屏数据，count-显示数据个数
void DisplayL(unsigned char *pt, char count)
{
	while (count--)
	{
		WRDataL(*pt++);			//写左半屏数据
	}
}

//显示右半屏数据，count-显示数据个数
void DisplayR(unsigned char * pt, char count)
{
	while (count--)
	{
		WRDataR(*pt++);			//写右半屏数据
	}
}

//设置左半屏起始显示行列地址,x-X起始行序数(0-7)，y-Y起始列序数(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x+X);				//行地址=行序数+行基址
	WRComL(y+Y);				//列地址=列序数+列基址
}

//设置右半屏起始显示行列地址,x:X起始行序数(0-7)，y:Y起始列序数(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x+X);				//行地址=行序数+行基址
	WRComR(y+Y);				//列地址=列序数+列基址
}
//***************************************
//显示图形		
//***************************************
//显示左半屏一行图形,A-X起始行序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYL(x,y);				//设置起始显示行列
	DisplayL(pt, 64);			//显示数据
}

//显示右半屏一行图形,A-X起始行地址序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYR(x,y);				//设置起始显示行列
	DisplayR(pt, 64);			//显示数据
}

//***************************************
//显示字体，显示一个数据要占用X行两行位置
//***************************************
//右半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 8);			//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+8, 8);		//显示下半行数据
}
void ByteDisR_num(unsigned char x, unsigned char y, u8 num)
{
	SETXYR(x,y);			//设置起始显示行列地址
	if(num == 0)
	{
		DisplayR(Number_0, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_0+8, 8);		//显示下半行数据
	}
	else if(num == 1)
	{
		DisplayR(Number_1, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_1+8, 8);		//显示下半行数据
	}
	else if(num == 2)
	{
		DisplayR(Number_2, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_2+8, 8);		//显示下半行数据
	}
	else if(num == 3)
	{
		DisplayR(Number_3, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_3+8, 8);		//显示下半行数据
	}
	else if(num == 4)
	{
		DisplayR(Number_4, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_4+8, 8);		//显示下半行数据
	}
	else if(num == 5)
	{
		DisplayR(Number_5, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_5+8, 8);		//显示下半行数据
	}
	else if(num == 6)
	{
		DisplayR(Number_6, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_6+8, 8);		//显示下半行数据
	}
	else if(num == 7)
	{
		DisplayR(Number_7, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_7+8, 8);		//显示下半行数据
	}
	else if(num == 8)
	{
		DisplayR(Number_8, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_8+8, 8);		//显示下半行数据
	}
	else if(num == 9)
	{
		DisplayR(Number_9, 8);			//显示上半行数据
		SETXYR(x+1,y);			//设置起始显示行列地址
		DisplayR(Number_9+8, 8);		//显示下半行数据
	}
}

void WordDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 16);		//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+16, 16);	//显示下半行数据
}

//左半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 8);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+8, 8);		//显示下半行数据
}
void ByteDisL_num(unsigned char x, unsigned char y, u8 num)
{
	SETXYL(x,y);			//设置起始显示行列地址
	if(num == 0)
	{
		DisplayL(Number_0, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_0+8, 8);		//显示下半行数据
	}
	else if(num == 1)
	{
		DisplayL(Number_1, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_1+8, 8);		//显示下半行数据
	}
	else if(num == 2)
	{
		DisplayL(Number_2, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_2+8, 8);		//显示下半行数据
	}
	else if(num == 3)
	{
		DisplayL(Number_3, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_3+8, 8);		//显示下半行数据
	}
	else if(num == 4)
	{
		DisplayL(Number_4, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_4+8, 8);		//显示下半行数据
	}
	else if(num == 5)
	{
		DisplayL(Number_5, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_5+8, 8);		//显示下半行数据
	}
	else if(num == 6)
	{
		DisplayL(Number_6, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_6+8, 8);		//显示下半行数据
	}
	else if(num == 7)
	{
		DisplayL(Number_7, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_7+8, 8);		//显示下半行数据
	}
	else if(num == 8)
	{
		DisplayL(Number_8, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_8+8, 8);		//显示下半行数据
	}
	else if(num == 9)
	{
		DisplayL(Number_9, 8);			//显示上半行数据
		SETXYL(x+1,y);			//设置起始显示行列地址
		DisplayL(Number_9+8, 8);		//显示下半行数据
	}
}

void WordDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 16);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+16, 16);	//显示下半行数据
}
//清屏
void LCDClear()
{
//清左半屏
	unsigned char x,y;
	char j;
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYL(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataL(0);
	}
//清右半屏
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYR(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataR(0);
	}
}
//清屏
void LCDClear_line(unsigned char line)
{
//清左半屏
	unsigned char x,y;
	char j;
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = line; x < 8; x++)		//共8行
	{
		SETXYL(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataL(0);
	}
//清右半屏
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = line; x < 8; x++)		//共8行
	{
		SETXYR(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataR(0);
	}
}

//hour min sec转化成可显示格式
void TimeToBuffer(u8* pBuffer)
{
	*pBuffer++ = black_sec % 10;	//黑子剩余的秒数
	*pBuffer++ = black_sec / 10; 
	*pBuffer++ = black_min % 10;	//黑子剩余的分钟数
	*pBuffer++ = black_min / 10;
	
	*pBuffer++ = red_sec % 10;		//红子剩余的秒数
	*pBuffer++ = red_sec / 10;
	*pBuffer++ = red_min % 10;		//红子剩余的分钟数
	*pBuffer++ = red_min / 10;	
}
//显示时分秒
void Display_LED()
{
	u8 buffer[8];
	TimeToBuffer(buffer);
	cDisplay8(buffer);
}
void AddCount()
{
	u8 i;
	u8 j;
	u8 last_pause_round = round;
	u8 buffer[8];
	u8 buffer1[8];
	u8 bContinue = 1;
	TimeToBuffer(buffer1);

	while (bContinue)
	{
		if (bFlash)
		{
			for (i = 0; i < 8; i++)
				buffer[i] = buffer1[i];
			if (halfsec == 0)
			{
				if(round == 0)	//红子的回合
				{
					buffer[7] = 0x10;
					buffer[6] = 0x10;
					buffer[5] = 0x10;
					buffer[4] = 0x10;//高4位闪烁
					TimeToBuffer(buffer1);
				}
				else if(round == 1)	//黑子的回合
				{
					buffer[3] = 0x10;
					buffer[2] = 0x10;
					buffer[1] = 0x10;
					buffer[0] = 0x10;//低四位闪烁
					TimeToBuffer(buffer1);
				}
				//如果现在是红子的回合，则接下来轮到黑子；反之亦然
				//if(round == 0)
				//	round = 1;
				//else if(round == 1)
				//	round = 0;	
				//bContinue = 0;
				
			}
			cDisplay8(buffer);
		}
		i = cGetKeyA();
			
		if(round == 0 && i == 0xc)
		{	
			red_count++;
			red_round_count++;
			round = 1;
			i = 0;
			TimeToBuffer(buffer1);
		}
		else if(round == 1 && i == 0xf)
		{
			round = 0;
			black_count++;
			black_round_count++;
			i = 0;	
			TimeToBuffer(buffer1);
		}
		if(round != 3)	//暂存数据，last_pause_round表示上一个回合是谁的回合，值为0表示上一个回合是红方回合，值为0表示上一个回合是黑方回合
			last_pause_round = round;
		
		
		
		if(i == 0xe)
		{
			if(round == 3)
			{
				round = last_pause_round;
				i = 0;//下一次则无法进入该分支
				LCDClear_line(3);//只对第3行以下的部分清屏
				TimeToBuffer(buffer);
				//outport(0x272, 0x20);//给PC6先送一个低电平再送一个上升沿使机器可以在暂停的时候播放歌
				outport(0x272, 0xe0);
			}
			else
			{
				round = 3;
				i = 0;
				LCDClear_line(3);//只对第3行以下的部分清屏
				WordDisL(4, 48, PauseTip_1);
				WordDisR(4, 0, PauseTip_2);//休赛提示信息
				TimeToBuffer(buffer);
				if(last_pause_round == 0)
				{
					ByteDisL_num(6, 0, buffer[7]);
					ByteDisL_num(6, 8, buffer[6]);
					ByteDisL(6, 16, MaoHao);
					ByteDisL_num(6, 24, buffer[5]);
					ByteDisL_num(6, 32, buffer[4]);
				}	
				else if(last_pause_round == 1)
				{
					ByteDisR_num(6, 24, buffer[3]);
					ByteDisR_num(6, 32, buffer[2]);
					ByteDisR(6, 40, MaoHao);
					ByteDisR_num(6, 48, buffer[1]);
					ByteDisR_num(6, 56, buffer[0]);
				}
				if(time_pulse == 0)
				{
					outport(0x272, 0xe0);//给PC7先送一个低电平再送一个上升沿使机器可以在暂停的时候播放歌
					outport(0x272, 0x60);
				}
			}
		}
		else if(round == 4 && i == 0xb) //分出胜负后再按b键可以再来一局
		{
			round = 0;
			//LCD_INIT();
			outport(0x272, 0xe0);
			LCDClear_line(0);
			DelayTime();
			WordDisL(0, 0, Red_1);
			WordDisL(0, 16, Red_2);
			WordDisL(0, 36, Red_3);
	
			WordDisR(0, 12, Black_1);
			WordDisR(0, 28, Black_2);
			WordDisR(0, 48, Black_3);
			//DisLine2_up();
			//start_flag = 1;
			//AddCount();
			count_limit = 40;
			red_count = 0;
			black_count = 0;
			
			red_min = 0x0a;
			red_sec = 0x00;
			black_min = 0x0a;
			black_sec = 0x00;
			Winner = 3;
			i = 0;
		}
		
		if(round == 0 && Winner == 3)
		{
			cDisplay8(buffer);
			LCDClear_line(6);//只对第6行以下的部分清屏
			ByteDisL_num(6, 0, buffer[7]);
			ByteDisL_num(6, 8, buffer[6]);
			ByteDisL(6, 16, MaoHao);
			ByteDisL_num(6, 24, buffer[5]);
			ByteDisL_num(6, 32, buffer[4]);
			//LCDClear_line(6);//只对第6行以下的部分清屏
			cDisplay8(buffer);               
			ByteDisR_num(6, 24, buffer[3]);
			ByteDisR_num(6, 32, buffer[2]);
			ByteDisR(6, 40, MaoHao);
			ByteDisR_num(6, 48, buffer[1]);
			ByteDisR_num(6, 56, buffer[0]);
			cDisplay8(buffer);
		}
		else if(round == 1 && Winner == 3)
		{
			cDisplay8(buffer);
			LCDClear_line(6);//只对第6行以下的部分清屏
			ByteDisL_num(6, 0, buffer[7]);
			ByteDisL_num(6, 8, buffer[6]);
			ByteDisL(6, 16, MaoHao);
			ByteDisL_num(6, 24, buffer[5]);
			ByteDisL_num(6, 32, buffer[4]);
			cDisplay8(buffer);
			//LCDClear_line(6);//只对第6行以下的部分清屏
			ByteDisR_num(6, 24, buffer[3]);
			ByteDisR_num(6, 32, buffer[2]);
			ByteDisR(6, 40, MaoHao);
			ByteDisR_num(6, 48, buffer[1]);
			ByteDisR_num(6, 56, buffer[0]);
			cDisplay8(buffer);
		}	
	
	}
}
void DisGameOver()
{
	WordDisL(0, 32, Over_1);
	WordDisL(0, 48, Over_2);
	
	WordDisR(0, 0, Over_3);
	WordDisR(0, 16, Over_4);
	
	WordDisL(3, 24, Win);
	WordDisL(3, 40, Red_2);
	ByteDisL(3, 56, MaoHao);
	
	if(Winner == 0)
	{
		WordDisR(3, 0, Red_1);
		WordDisR(3, 16, Red_2);
	}
	else if(Winner == 1)
	{
		WordDisR(3, 0, Black_1);
		WordDisR(3, 16, Black_2);
	}
	
	//WordDisL(6,8,Restart_1);
	//ByteDisL(6,24,Restart_2);
	//WordDisL(6,32,Restart_3);
	//WordDisL(6,48,Restart_4);
	
	//WordDisR(6,0,Restart_5);
	//WordDisR(6,16,Restart_6);
	//WordDisR(6,32,Restart_7);
}
void GameOver()
{
	u8 buffer[8];
	if(round != 4)
	{
		TimeToBuffer(buffer);
		round = 4;
		//disable();//关中断
	}
	TimeToBuffer(buffer);
	cDisplay8(buffer);
	round = 4;
	LCDClear();				//清屏
	DisGameOver();
}


//0.5s产生一次中断
void interrupt Timer0Int(void)
{
	bFlash = 1;
	halfsec++;
	if (halfsec == 2)
	{
		bNeedDisplay = 1;
		halfsec = 0;
		if(round == 0)	//红子的回合
		{
			if(red_min > 0)
			{
				if(red_sec > 0)
				{
					red_sec--;
				}
				else if(red_sec == 0)
				{
					red_sec = 59;
					red_min--;
				}
			}
			else
			{
				if(red_sec > 0)	//剩余时间不到1分钟
				{
					red_sec--;
				}
				else//发出警报，红子的选手用时为0
				{
					if(red_count < count_limit)	//未走满40着，判负出局
					{
						if(Winner == 3)
						{
							Winner = 1;
							GameOver();
							outportb(0x272, 0x80);
							DelayTime();//延时
							outportb(0x272, 0xa0);
							//count_limit = 0;//结束
						}
					}
					else
					{
						red_sec = 0;
						red_min = 5;
						red_round_count = 0;
						count_limit = 30;
					}
				}	
			}
		}
		else if(round == 1)	//黑子的回合
		{
			if(black_min > 0)
			{
				if(black_sec > 0)
				{
					black_sec--;
				}
				else if(black_sec == 0)
				{
					black_sec = 59;
					black_min--;
				}
			}
			else
			{
				if(black_sec > 0)	//剩余时间不到1分钟
				{
					black_sec--;
				}
				else//发出警报，红子的选手用时为0
				{
					if(black_count < count_limit)	//未走满40着，判负出局
					{
						if(Winner == 3)
						{
							Winner = 0;
							GameOver();
							outportb(0x272, 0x80);
							DelayTime();//延时
							outportb(0x272, 0xa0);
							//count_limit = 0;//结束
						}
					}
					else
					{
						red_sec = 0;
						red_min = 5;
						red_round_count = 0;
						count_limit = 30;
					}
				}	
			}
		}
	}
	outportb(IO8259_0,0x20);
}
			
void Init8253()
{
	outportb(Con_8253,0x34);				//计数器T0设置在模式2状态,HEX计数,二进制，计数器0先低后高
	outportb(T0_8253,0x12);					//CLK0=62.5kHz,0.5s定时
	outportb(T0_8253,0x7a);
}

void Init8259()
{
	outportb(IO8259_0,0x13);				//写ICW1，D4必须为1,0011表示D3边沿触发，D1单片，D0要写入ICW4
	outportb(IO8259_1,0x8);					//写ICW2，中断向量字，00001000表示用IR0
	outportb(IO8259_1,0x9);					//写ICW4，D3表示用缓冲方式，D4普通全嵌套，D0为1表示微处理器为16位
	outportb(IO8259_1,0xfe);				//写OCW1，特殊屏蔽字。只有M0为0表示只允许IRO中断
}
void Init8255()
{
	outportb(0x270, 0x81);
}




//液晶初始化	
void LCD_INIT()
{
	WRComL(0x3e);			//初始化左半屏，关显示
	WRComL(FirstLine);		//设置起始显示行，第0行
	WRComR(0x3e);			//初始化右半屏，关显示
	WRComR(FirstLine);		//设置起始显示行，第0行
	LCDClear();				//清屏
	WRComL(0x3f);			//开显示
	WRComR(0x3f);			//开显示
}

//第3行显示红黑棋手个人情况
void DisLine2()
{
	WordDisL(3, 0, Red_1);
	WordDisL(3, 16, Red_2);
	WordDisL(3, 36, Red_3);
	
	WordDisR(3, 12, Black_1);
	WordDisR(3, 28, Black_2);
	WordDisR(3, 48, Black_3);
}

//第3行显示红黑棋手个人情况
void DisLine2_up()
{
	WordDisL(0, 0, Red_1);
	WordDisL(0, 16, Red_2);
	WordDisL(0, 36, Red_3);
	
	WordDisR(0, 12, Black_1);
	WordDisR(0, 28, Black_2);
	WordDisR(0, 48, Black_3);
}
//第1行显示"棋王争霸赛"
void DisLine1()
{
	WordDisL(0,24,Title_1);		
	WordDisL(0,40,Title_2);
	ByteDisL(0,56,Title_3_L);	//这里由于标题是五个字，所以要将第三个字拆开，用ByteDisL和ByteDisR显示
	ByteDisR(0,0, Title_3_R);	
	WordDisR(0,8,Title_4);
	WordDisR(0,24,Title_5);
}

//第4行简单介绍游戏规则
void DisLine3()
{
	WordDisL(6,0, Start_1);
	ByteDisL(6,16, Start_2);	//E键开始，E为8x16
	WordDisL(6,24, Start_3);
	WordDisL(6,40, Start_4);
	ByteDisL(6,56, Start_5);	//,大小为8x16
	
	WordDisR(6,0, Red_1);
	WordDisR(6,16, Red_2);
	WordDisR(6,32, Start_6);
	WordDisR(6,48, Start_7);
}
		
void main()
{
	u8 keyResult;
	u8 start_flag = 0;
	bNeedDisplay = 1;				//显示初始值
	cInitKeyDisplay();				//键盘、数码管管理器初始化
	outportb(0x272, 0xe0);
	Init8253();
	Init8259();
	setvect(0x8, Timer0Int);		//设置中断向量
	//enable();						//开中断	
	LCD_INIT();					//液晶初始化	
	while (1)
	{
		keyResult = cGetKeyA();	//按键扫描
		if(!start_flag)
		{
			DisLine1();					//第1行显示"棋王争霸赛"
			DisLine2();					//第3行显示红黑棋手个人情况
			DisLine3();					//第4行简要介绍规则	
		}
		if (keyResult == 0xd)
		{
			enable();
			DelayTime();				//延时
			LCD_INIT();
			DisLine2_up();
			start_flag = 1;
			AddCount();
		}
		if (bNeedDisplay)
		{
			bNeedDisplay = 0;
			Display_LED();		//显示时分秒
		}
	}
}















