#define	u8	unsigned char
#define u16 unsigned int
#define T1MS (65536-FOSC/12/500)   /*0.5ms timer calculation method in 12Tmode*/
 
#define	const_voice_short	40	/*�������̽еĳ���ʱ��*/
#define	const_voice_long	900	/*���������еĳ���ʱ��*/
#define const_key_time 9	/*����ȥ������ʱ��ʱ��*/

#define const_1s  96	/*��Ų���һ���ӵ�ʱ���׼*/

extern void outportb( unsigned int, char);						//дI/O
extern char inportb( unsigned int );							//��I/O
extern void enable( void );										//���ж�
extern void disable( void );									//���ж�
extern void setvect( int, void interrupt(*isr)( void ) );		//д�ж�����
extern void interrupt( far *  getvect( int __interruptno ))( );	//���ж�����

//�Լ��̡�����ܹ�������ʼ��
extern void cInitKeyDisplay();
//��pBufferָ���8�ֽڻ�����������ʾ��G5���������
extern void cDisplay8(u8* pBuffer);
//����һ��16 ���Ƽ�ֵ�����û�а���������0xff
extern u8 cGetKeyA();

#define	IO8259_0	0x250
#define IO8259_1	0x251
#define Con_8253	0x263				//8253���ƿ�
#define T0_8253		0x260
//�궨���ظ�

u8 halfsec;			//0.5�����
u8 bNeedDisplay;	//��Ҫˢ����ʾ
u8 bFlash;			//����ʱ�Ƿ���Ҫˢ��
u8 time_pulse = 0;		//������������ͣ��ʱ��(round == 3)������������ʱ������

/*ÿ������10����������40�š�
�Ժ�ÿ5���ӱ�������30�ţ�ֱ���Ծֽ���*/

u8 red_count = 0x00;//��ʾ����Ŀǰ�ߵĲ������ƶ�һ�λ��߳Ե��Է�һ�Ӷ���һ���ƶ�
u8 red_round_count = 0x14;//�ý׶κ������ߵĲ���
u8 black_count = 0x00;//��ʾ����Ŀǰ�ߵĲ���
u8 black_round_count = 0x14;

u8 count_limit = 40;	//��ǰ�غϵĲ�������
u8 round_flag = 0;	//�׶κ�

u8 red_min = 0x00;		//�����ʼʱ��Ĭ��Ϊ10����0��
u8 red_sec = 0x14;
u8 black_min = 0x00;		//�����ʼʱ��Ĭ��Ϊ10����0��	
u8 black_sec = 0x14;

u16 round = 0x00;				//ֵΪ0��ʾ����Ļغϣ�ֵΪ1��ʾ����Ļغϣ�������Ĭ�Ϻ������ߣ���ʼ״̬roundֵΪ0
u8 PauseFlag = 0x00;			//��ͣ��־��ֵΪ1��ʾ�Ѿ���ͣ���˫���ļ�ʱ��Ĭ��Ϊ0�����ʾ��ʱ״̬����ʱ���ҽ���һλѡ�ֵ�ʱ���ڼ�ʱ
u8 ResetFlag = 0x00;			//��λ��־��ֵΪ1ʱ��λ����֮����λ
u8 Winner = 3;					//ֵΪ0��ʾ�췽ʤ����ֵΪ1��ʾ�ڷ�ʤ��������ֵ��ʾδ����ʤ��

#define WR_COM_AD_L		0x234				//д�����ָ���ַ
#define WR_COM_AD_R		0x230				//д�Ұ���ָ���ַ
#define WR_DATA_AD_L 	0x236				//д��������ݵ�ַ	
#define WR_DATA_AD_R	0x232				//д�Ұ������ݵ�ַ
#define RD_BUSY_AD 		0x231				//��æ��ַ
#define RD_DATA_AD 		0x233				//�����ݵ�ַ

#define X 			0xB8							//��ʼ��ʾ�л�ַ
#define Y			0x40							//��ʼ��ʾ�л�ַ
#define FirstLine	0x00							//��ʼ��ʾ��

/*--  ����:  ��  --*/
/*--  ��Բ15;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Title_1[] = {
	0x00,0x10,0x90,0xFE,0x90,0x08,0x08,0xFC,0x48,0x48,0x48,0x48,0xFC,0x08,0x08,0x00,
	0x08,0x06,0x01,0xFF,0x01,0x90,0x90,0x5F,0x32,0x12,0x12,0x32,0x5F,0x50,0x90,0x00};

/*--  ����:  ��  --*/
/*--  ��Բ15;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Title_2[] = {
	0x00,0x00,0x04,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,
	0x00,0x80,0x81,0x81,0x81,0x81,0x81,0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x80,0x00};

//0x00,0x40,0x20,0x50,0x4C,0x4A,0x48,0x48,0xC8,0x68,0x58,0x40,0x40,0x80,0x00,0x00,
//0x00,0x00,0x02,0x12,0x92,0x92,0x12,0x92,0x7F,0x12,0x12,0x12,0x0A,0x07,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ��Բ15;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Title_3_L[] = {
	0x00,0x40,0x20,0x50,0x4C,0x4A,0x48,0x48, 0x00,0x00,0x02,0x12,0x92,0x92,0x12,0x92};
	
unsigned char Title_3_R[] = {
	0xC8,0x68,0x58,0x40,0x40,0x80,0x00,0x00, 0x7F,0x12,0x12,0x12,0x0A,0x07,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ��Բ15;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Title_4[] = {
	0x00,0xF8,0x8A,0x9A,0xDA,0xDA,0x9A,0x8A,0x7E,0x5A,0xDA,0xDA,0xDA,0xDA,0x38,0x00,
	0x00,0x48,0x55,0x56,0xFE,0x56,0x67,0x5C,0x81,0x7F,0x14,0x94,0x94,0xFF,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ��Բ15;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Title_5[] = {
	0x00,0x38,0x58,0x58,0x58,0xF8,0x58,0x5E,0x58,0x58,0xF8,0x58,0x58,0x58,0x38,0x00,
	0x00,0x09,0x05,0xBB,0x85,0x85,0x45,0x3D,0x05,0x25,0x45,0x85,0xBB,0x05,0x09,0x00};


	
/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Red_1[] = {
	0x00,0x20,0x30,0xAC,0x63,0x20,0x10,0x00,0x20,0x20,0xF0,0x10,0x10,0x00,0x00,0x00,
	0x00,0x10,0x13,0x0A,0x09,0x05,0x10,0x10,0x10,0x10,0x0F,0x08,0x08,0x08,0x08,0x00};

/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Red_2[] = {
	0x00,0x20,0x20,0x20,0x20,0x20,0x22,0xE2,0x94,0x90,0x90,0x10,0x10,0x10,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x11,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Red_3[] = {
	0x00,0x00,0x00,0x20,0x20,0xA0,0x60,0x1F,0x10,0x90,0x90,0x90,0x00,0x00,0x00,0x00,
	0x00,0x10,0x08,0x24,0x22,0x21,0x21,0x21,0x1F,0x10,0x10,0x10,0x10,0x10,0x00,0x00};
	

/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Black_1[] = {
	0x00,0x00,0x00,0x3C,0x44,0x54,0xFE,0xA2,0xAA,0xA2,0x32,0x0E,0x00,0x00,0x00,0x00,
	0x00,0x20,0x18,0x04,0x15,0x25,0x03,0x0A,0x12,0x02,0x02,0x08,0x10,0x30,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/	
unsigned char Black_2[] = {
	0x00,0x20,0x20,0x20,0x20,0x20,0x22,0xE2,0x94,0x90,0x90,0x10,0x10,0x10,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x11,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ��Բ12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Black_3[] = {
	0x00,0x20,0x20,0x20,0x20,0xA0,0x60,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
	0x10,0x10,0x08,0x04,0x02,0x3F,0x12,0x12,0x12,0x11,0x19,0x07,0x00,0x00,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Start_1[] = {
	0x00,0x20,0x20,0xFF,0x90,0x00,0x60,0x20,0x20,0xD2,0x14,0x10,0x50,0x30,0x00,0x00,
	0x02,0x02,0x11,0x3F,0x00,0x22,0x22,0x12,0x17,0x09,0x0D,0x13,0x21,0x21,0x01,0x00};

/*--  ����:  d  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Start_2[] = {
	0x00,0x80,0x40,0x40,0x48,0xF8,0x00,0x00,0x00,0x1F,0x10,0x20,0x20,0x1F,0x10,0x00};
	
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Start_3[] = {
	0x00,0x80,0x80,0x84,0x84,0xFC,0x44,0x44,0xFE,0x22,0x22,0x20,0x20,0x20,0x00,0x00,
	0x00,0x20,0x10,0x08,0x06,0x01,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*--  ����:  ʼ  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Start_4[] = {
	0x80,0x80,0x80,0xE0,0x5E,0x40,0xE0,0x00,0xC0,0xB0,0x8E,0x40,0x60,0xC0,0x00,0x00,
	0x00,0x10,0x10,0x0B,0x04,0x0B,0x00,0x00,0x1C,0x12,0x12,0x12,0x0E,0x00,0x00,0x00};
/*--  ����:  ,  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Start_5[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Start_6[] = {
	0x00,0x00,0x80,0xA0,0x9C,0x90,0x90,0xFF,0x48,0x48,0x48,0x40,0x00,0x00,0x00,0x00,
	0x20,0x20,0x10,0x08,0x04,0x03,0x00,0x1F,0x20,0x20,0x20,0x20,0x20,0x38,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Start_7[] = {
	0x00,0x80,0x80,0x80,0x90,0x90,0x90,0xFF,0x48,0x48,0x48,0x40,0x00,0x00,0x00,0x00,
	0x20,0x10,0x08,0x04,0x03,0x04,0x08,0x0F,0x11,0x31,0x21,0x20,0x20,0x20,0x20,0x00};

/*--  ����:  0  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_0[] = {
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00};

/*--  ����:  1  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_1[] = {
	0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00};

/*--  ����:  2  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_2[] = {
	0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00};

/*--  ����:  3  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_3[] = {
	0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00};

/*--  ����:  4  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_4[] = {
	0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00};

/*--  ����:  5  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_5[] = {
	0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00};

/*--  ����:  6  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_6[] = {
	0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00};

/*--  ����:  7  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_7[] = {
	0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00};

/*--  ����:  8  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_8[] = {
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00};

/*--  ����:  9  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char Number_9[] = {
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00};

/*--  ����:  :  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
unsigned char MaoHao[] = {
	0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Over_1[] = {
	0x00,0x20,0x44,0x08,0x20,0xE0,0x92,0x94,0x10,0x28,0xAE,0x68,0x24,0x04,0x00,0x00,
	0x00,0x0C,0x03,0x04,0x02,0x19,0x0C,0x03,0x02,0x12,0x22,0x1F,0x01,0x01,0x01,0x00};
/*--  ����:  Ϸ  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Over_2[] = {
	0x00,0x20,0xA0,0x90,0x10,0xF0,0x00,0x40,0x7F,0xC0,0x20,0x24,0x88,0x00,0x00,0x00,
	0x10,0x08,0x04,0x02,0x01,0x02,0x14,0x10,0x08,0x05,0x06,0x09,0x10,0x20,0x38,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/	
unsigned char Over_3[] = {
	0x00,0x60,0x50,0xCC,0x40,0x30,0x40,0x40,0x40,0xFE,0x20,0x20,0x20,0x20,0x00,0x00,
	0x00,0x12,0x13,0x0A,0x09,0x05,0x00,0x3A,0x2A,0x25,0x25,0x15,0x1D,0x00,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Over_4[] = {
	0x00,0x00,0x00,0x60,0xA8,0xA8,0xA8,0xFF,0x94,0x54,0x70,0x00,0x00,0x00,0x00,0x00,
	0x10,0x10,0x08,0x08,0x04,0x02,0x01,0x7F,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x00};
/*--  ����:  ʤ  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Win[] = {
	0x00,0x00,0xF8,0x24,0x24,0xFC,0x40,0x38,0x20,0x20,0xFE,0x90,0x90,0x00,0x00,0x00,
	0x20,0x18,0x07,0x01,0x11,0x3F,0x10,0x10,0x11,0x11,0x0F,0x08,0x08,0x08,0x00,0x00};

/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char PauseTip_1[] = {
	0x00,0x80,0xE0,0x18,0x06,0x20,0x20,0xA0,0xFF,0x90,0x10,0x10,0x00,0x00,0x00,0x00,
	0x01,0x00,0x3F,0x00,0x08,0x04,0x03,0x00,0x7F,0x00,0x01,0x02,0x04,0x04,0x04,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char PauseTip_2[] = {
	0x00,0x00,0x8C,0x94,0x54,0xFC,0x55,0x7E,0x4A,0xCA,0x42,0x46,0x00,0x00,0x00,0x00,
	0x08,0x08,0x44,0x42,0x2F,0x11,0x0D,0x11,0x2F,0x60,0x01,0x02,0x06,0x04,0x04,0x00};

/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_1[] = {
	0x00,0x20,0x20,0xFF,0x90,0x00,0x60,0x20,0x20,0xD2,0x14,0x10,0x50,0x30,0x00,0x00,
	0x02,0x02,0x11,0x3F,0x00,0x22,0x22,0x12,0x17,0x09,0x0D,0x13,0x21,0x21,0x01,0x00};
/*--  ����:  b  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_2[] = {
	0x00,0x08,0xF8,0x40,0x40,0xC0,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x0F,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_3[] = {
	0x80,0x40,0x70,0xCE,0x48,0x18,0xF0,0x00,0xA8,0xA8,0xFF,0x54,0x7C,0x10,0x10,0x00,
	0x00,0x02,0x02,0x1F,0x29,0x14,0x0D,0x0B,0x14,0x15,0x1F,0x25,0x24,0x20,0x20,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_4[] = {
	0x00,0x00,0x00,0x02,0xF2,0x12,0x52,0xFE,0x29,0x09,0xF9,0x01,0x00,0x00,0x00,0x00,
	0x02,0x02,0x02,0x02,0x1F,0x02,0x02,0x01,0x11,0x21,0x1F,0x01,0x01,0x01,0x01,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_5[] = {
	0x00,0x80,0x80,0xA8,0xC8,0x88,0xFF,0x84,0x64,0x54,0x40,0x40,0x00,0x00,0x00,0x00,
	0x00,0x10,0x10,0x08,0x04,0x02,0x7F,0x01,0x02,0x04,0x0C,0x08,0x08,0x08,0x08,0x00};
/*--  ����:  һ  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_6[] = {
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x40,0x40,0x40,0x40,0xC0,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*--  ����:  ��  --*/
/*--  ����_GB231212;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
unsigned char Restart_7[] = {
	0x00,0x00,0x00,0x00,0xFC,0x94,0x92,0x92,0x52,0x4E,0x40,0x40,0xC0,0x00,0x00,0x00,
	0x10,0x08,0x04,0x03,0x0E,0x0A,0x0A,0x0A,0x06,0x20,0x40,0x20,0x1F,0x00,0x00,0x00};
	
	
//��ʱ����
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
//��������		
//***************************************					
//д���������ָ��
void WRComL(unsigned char _data)
{
	outportb(WR_COM_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д�Ұ�������ָ��
void WRComR(unsigned char _data)
{
	outportb(WR_COM_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д���������
void WRDataL(unsigned char _data)
{
	outportb(WR_DATA_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}

//д�Ұ�������
void WRDataR(unsigned char _data)
{
	outportb(WR_DATA_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//���Һ����ʾ�Ƿ���æ״̬
	{;}
}
//��ʾ��������ݣ�count-��ʾ���ݸ���
void DisplayL(unsigned char *pt, char count)
{
	while (count--)
	{
		WRDataL(*pt++);			//д���������
	}
}

//��ʾ�Ұ������ݣ�count-��ʾ���ݸ���
void DisplayR(unsigned char * pt, char count)
{
	while (count--)
	{
		WRDataR(*pt++);			//д�Ұ�������
	}
}

//�����������ʼ��ʾ���е�ַ,x-X��ʼ������(0-7)��y-Y��ʼ������(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x+X);				//�е�ַ=������+�л�ַ
	WRComL(y+Y);				//�е�ַ=������+�л�ַ
}

//�����Ұ�����ʼ��ʾ���е�ַ,x:X��ʼ������(0-7)��y:Y��ʼ������(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x+X);				//�е�ַ=������+�л�ַ
	WRComR(y+Y);				//�е�ַ=������+�л�ַ
}
//***************************************
//��ʾͼ��		
//***************************************
//��ʾ�����һ��ͼ��,A-X��ʼ������(0-7)��B-Y��ʼ�е�ַ����(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYL(x,y);				//������ʼ��ʾ����
	DisplayL(pt, 64);			//��ʾ����
}

//��ʾ�Ұ���һ��ͼ��,A-X��ʼ�е�ַ����(0-7)��B-Y��ʼ�е�ַ����(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYR(x,y);				//������ʼ��ʾ����
	DisplayR(pt, 64);			//��ʾ����
}

//***************************************
//��ʾ���壬��ʾһ������Ҫռ��X������λ��
//***************************************
//�Ұ�����ʾһ���ֽ�/�֣�x-��ʼ��ʾ������X(0-7)��y-��ʼ��ʾ������Y(0-63)��pt-��ʾ�������׵�ַ
void ByteDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt, 8);			//��ʾ�ϰ�������
	SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt+8, 8);		//��ʾ�°�������
}
void ByteDisR_num(unsigned char x, unsigned char y, u8 num)
{
	SETXYR(x,y);			//������ʼ��ʾ���е�ַ
	if(num == 0)
	{
		DisplayR(Number_0, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_0+8, 8);		//��ʾ�°�������
	}
	else if(num == 1)
	{
		DisplayR(Number_1, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_1+8, 8);		//��ʾ�°�������
	}
	else if(num == 2)
	{
		DisplayR(Number_2, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_2+8, 8);		//��ʾ�°�������
	}
	else if(num == 3)
	{
		DisplayR(Number_3, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_3+8, 8);		//��ʾ�°�������
	}
	else if(num == 4)
	{
		DisplayR(Number_4, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_4+8, 8);		//��ʾ�°�������
	}
	else if(num == 5)
	{
		DisplayR(Number_5, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_5+8, 8);		//��ʾ�°�������
	}
	else if(num == 6)
	{
		DisplayR(Number_6, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_6+8, 8);		//��ʾ�°�������
	}
	else if(num == 7)
	{
		DisplayR(Number_7, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_7+8, 8);		//��ʾ�°�������
	}
	else if(num == 8)
	{
		DisplayR(Number_8, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_8+8, 8);		//��ʾ�°�������
	}
	else if(num == 9)
	{
		DisplayR(Number_9, 8);			//��ʾ�ϰ�������
		SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayR(Number_9+8, 8);		//��ʾ�°�������
	}
}

void WordDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt, 16);		//��ʾ�ϰ�������
	SETXYR(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayR(pt+16, 16);	//��ʾ�°�������
}

//�������ʾһ���ֽ�/�֣�x-��ʼ��ʾ������X(0-7)��y-��ʼ��ʾ������Y(0-63)��pt-��ʾ�������׵�ַ
void ByteDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt, 8);		//��ʾ�ϰ�������
	SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt+8, 8);		//��ʾ�°�������
}
void ByteDisL_num(unsigned char x, unsigned char y, u8 num)
{
	SETXYL(x,y);			//������ʼ��ʾ���е�ַ
	if(num == 0)
	{
		DisplayL(Number_0, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_0+8, 8);		//��ʾ�°�������
	}
	else if(num == 1)
	{
		DisplayL(Number_1, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_1+8, 8);		//��ʾ�°�������
	}
	else if(num == 2)
	{
		DisplayL(Number_2, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_2+8, 8);		//��ʾ�°�������
	}
	else if(num == 3)
	{
		DisplayL(Number_3, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_3+8, 8);		//��ʾ�°�������
	}
	else if(num == 4)
	{
		DisplayL(Number_4, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_4+8, 8);		//��ʾ�°�������
	}
	else if(num == 5)
	{
		DisplayL(Number_5, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_5+8, 8);		//��ʾ�°�������
	}
	else if(num == 6)
	{
		DisplayL(Number_6, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_6+8, 8);		//��ʾ�°�������
	}
	else if(num == 7)
	{
		DisplayL(Number_7, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_7+8, 8);		//��ʾ�°�������
	}
	else if(num == 8)
	{
		DisplayL(Number_8, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_8+8, 8);		//��ʾ�°�������
	}
	else if(num == 9)
	{
		DisplayL(Number_9, 8);			//��ʾ�ϰ�������
		SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
		DisplayL(Number_9+8, 8);		//��ʾ�°�������
	}
}

void WordDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt, 16);		//��ʾ�ϰ�������
	SETXYL(x+1,y);			//������ʼ��ʾ���е�ַ
	DisplayL(pt+16, 16);	//��ʾ�°�������
}
//����
void LCDClear()
{
//�������
	unsigned char x,y;
	char j;
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = 0; x < 8; x++)		//��8��
	{
		SETXYL(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataL(0);
	}
//���Ұ���
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = 0; x < 8; x++)		//��8��
	{
		SETXYR(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataR(0);
	}
}
//����
void LCDClear_line(unsigned char line)
{
//�������
	unsigned char x,y;
	char j;
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = line; x < 8; x++)		//��8��
	{
		SETXYL(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataL(0);
	}
//���Ұ���
	x = 0;						//��ʼ�У���0��
	y = 0;						//��ʼ�У���0��
	for (x = line; x < 8; x++)		//��8��
	{
		SETXYR(x,y);			//������ʼ��ʾ���е�ַ
		j = 64;
		while (j--)
			WRDataR(0);
	}
}

//hour min secת���ɿ���ʾ��ʽ
void TimeToBuffer(u8* pBuffer)
{
	*pBuffer++ = black_sec % 10;	//����ʣ�������
	*pBuffer++ = black_sec / 10; 
	*pBuffer++ = black_min % 10;	//����ʣ��ķ�����
	*pBuffer++ = black_min / 10;
	
	*pBuffer++ = red_sec % 10;		//����ʣ�������
	*pBuffer++ = red_sec / 10;
	*pBuffer++ = red_min % 10;		//����ʣ��ķ�����
	*pBuffer++ = red_min / 10;	
}
//��ʾʱ����
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
				if(round == 0)	//���ӵĻغ�
				{
					buffer[7] = 0x10;
					buffer[6] = 0x10;
					buffer[5] = 0x10;
					buffer[4] = 0x10;//��4λ��˸
					TimeToBuffer(buffer1);
				}
				else if(round == 1)	//���ӵĻغ�
				{
					buffer[3] = 0x10;
					buffer[2] = 0x10;
					buffer[1] = 0x10;
					buffer[0] = 0x10;//����λ��˸
					TimeToBuffer(buffer1);
				}
				//��������Ǻ��ӵĻغϣ���������ֵ����ӣ���֮��Ȼ
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
		if(round != 3)	//�ݴ����ݣ�last_pause_round��ʾ��һ���غ���˭�Ļغϣ�ֵΪ0��ʾ��һ���غ��Ǻ췽�غϣ�ֵΪ0��ʾ��һ���غ��Ǻڷ��غ�
			last_pause_round = round;
		
		
		
		if(i == 0xe)
		{
			if(round == 3)
			{
				round = last_pause_round;
				i = 0;//��һ�����޷�����÷�֧
				LCDClear_line(3);//ֻ�Ե�3�����µĲ�������
				TimeToBuffer(buffer);
				//outport(0x272, 0x20);//��PC6����һ���͵�ƽ����һ��������ʹ������������ͣ��ʱ�򲥷Ÿ�
				outport(0x272, 0xe0);
			}
			else
			{
				round = 3;
				i = 0;
				LCDClear_line(3);//ֻ�Ե�3�����µĲ�������
				WordDisL(4, 48, PauseTip_1);
				WordDisR(4, 0, PauseTip_2);//������ʾ��Ϣ
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
					outport(0x272, 0xe0);//��PC7����һ���͵�ƽ����һ��������ʹ������������ͣ��ʱ�򲥷Ÿ�
					outport(0x272, 0x60);
				}
			}
		}
		else if(round == 4 && i == 0xb) //�ֳ�ʤ�����ٰ�b����������һ��
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
			LCDClear_line(6);//ֻ�Ե�6�����µĲ�������
			ByteDisL_num(6, 0, buffer[7]);
			ByteDisL_num(6, 8, buffer[6]);
			ByteDisL(6, 16, MaoHao);
			ByteDisL_num(6, 24, buffer[5]);
			ByteDisL_num(6, 32, buffer[4]);
			//LCDClear_line(6);//ֻ�Ե�6�����µĲ�������
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
			LCDClear_line(6);//ֻ�Ե�6�����µĲ�������
			ByteDisL_num(6, 0, buffer[7]);
			ByteDisL_num(6, 8, buffer[6]);
			ByteDisL(6, 16, MaoHao);
			ByteDisL_num(6, 24, buffer[5]);
			ByteDisL_num(6, 32, buffer[4]);
			cDisplay8(buffer);
			//LCDClear_line(6);//ֻ�Ե�6�����µĲ�������
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
		//disable();//���ж�
	}
	TimeToBuffer(buffer);
	cDisplay8(buffer);
	round = 4;
	LCDClear();				//����
	DisGameOver();
}


//0.5s����һ���ж�
void interrupt Timer0Int(void)
{
	bFlash = 1;
	halfsec++;
	if (halfsec == 2)
	{
		bNeedDisplay = 1;
		halfsec = 0;
		if(round == 0)	//���ӵĻغ�
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
				if(red_sec > 0)	//ʣ��ʱ�䲻��1����
				{
					red_sec--;
				}
				else//�������������ӵ�ѡ����ʱΪ0
				{
					if(red_count < count_limit)	//δ����40�ţ��и�����
					{
						if(Winner == 3)
						{
							Winner = 1;
							GameOver();
							outportb(0x272, 0x80);
							DelayTime();//��ʱ
							outportb(0x272, 0xa0);
							//count_limit = 0;//����
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
		else if(round == 1)	//���ӵĻغ�
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
				if(black_sec > 0)	//ʣ��ʱ�䲻��1����
				{
					black_sec--;
				}
				else//�������������ӵ�ѡ����ʱΪ0
				{
					if(black_count < count_limit)	//δ����40�ţ��и�����
					{
						if(Winner == 3)
						{
							Winner = 0;
							GameOver();
							outportb(0x272, 0x80);
							DelayTime();//��ʱ
							outportb(0x272, 0xa0);
							//count_limit = 0;//����
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
	outportb(Con_8253,0x34);				//������T0������ģʽ2״̬,HEX����,�����ƣ�������0�ȵͺ��
	outportb(T0_8253,0x12);					//CLK0=62.5kHz,0.5s��ʱ
	outportb(T0_8253,0x7a);
}

void Init8259()
{
	outportb(IO8259_0,0x13);				//дICW1��D4����Ϊ1,0011��ʾD3���ش�����D1��Ƭ��D0Ҫд��ICW4
	outportb(IO8259_1,0x8);					//дICW2���ж������֣�00001000��ʾ��IR0
	outportb(IO8259_1,0x9);					//дICW4��D3��ʾ�û��巽ʽ��D4��ͨȫǶ�ף�D0Ϊ1��ʾ΢������Ϊ16λ
	outportb(IO8259_1,0xfe);				//дOCW1�����������֡�ֻ��M0Ϊ0��ʾֻ����IRO�ж�
}
void Init8255()
{
	outportb(0x270, 0x81);
}




//Һ����ʼ��	
void LCD_INIT()
{
	WRComL(0x3e);			//��ʼ�������������ʾ
	WRComL(FirstLine);		//������ʼ��ʾ�У���0��
	WRComR(0x3e);			//��ʼ���Ұ���������ʾ
	WRComR(FirstLine);		//������ʼ��ʾ�У���0��
	LCDClear();				//����
	WRComL(0x3f);			//����ʾ
	WRComR(0x3f);			//����ʾ
}

//��3����ʾ������ָ������
void DisLine2()
{
	WordDisL(3, 0, Red_1);
	WordDisL(3, 16, Red_2);
	WordDisL(3, 36, Red_3);
	
	WordDisR(3, 12, Black_1);
	WordDisR(3, 28, Black_2);
	WordDisR(3, 48, Black_3);
}

//��3����ʾ������ָ������
void DisLine2_up()
{
	WordDisL(0, 0, Red_1);
	WordDisL(0, 16, Red_2);
	WordDisL(0, 36, Red_3);
	
	WordDisR(0, 12, Black_1);
	WordDisR(0, 28, Black_2);
	WordDisR(0, 48, Black_3);
}
//��1����ʾ"����������"
void DisLine1()
{
	WordDisL(0,24,Title_1);		
	WordDisL(0,40,Title_2);
	ByteDisL(0,56,Title_3_L);	//�������ڱ���������֣�����Ҫ���������ֲ𿪣���ByteDisL��ByteDisR��ʾ
	ByteDisR(0,0, Title_3_R);	
	WordDisR(0,8,Title_4);
	WordDisR(0,24,Title_5);
}

//��4�м򵥽�����Ϸ����
void DisLine3()
{
	WordDisL(6,0, Start_1);
	ByteDisL(6,16, Start_2);	//E����ʼ��EΪ8x16
	WordDisL(6,24, Start_3);
	WordDisL(6,40, Start_4);
	ByteDisL(6,56, Start_5);	//,��СΪ8x16
	
	WordDisR(6,0, Red_1);
	WordDisR(6,16, Red_2);
	WordDisR(6,32, Start_6);
	WordDisR(6,48, Start_7);
}
		
void main()
{
	u8 keyResult;
	u8 start_flag = 0;
	bNeedDisplay = 1;				//��ʾ��ʼֵ
	cInitKeyDisplay();				//���̡�����ܹ�������ʼ��
	outportb(0x272, 0xe0);
	Init8253();
	Init8259();
	setvect(0x8, Timer0Int);		//�����ж�����
	//enable();						//���ж�	
	LCD_INIT();					//Һ����ʼ��	
	while (1)
	{
		keyResult = cGetKeyA();	//����ɨ��
		if(!start_flag)
		{
			DisLine1();					//��1����ʾ"����������"
			DisLine2();					//��3����ʾ������ָ������
			DisLine3();					//��4�м�Ҫ���ܹ���	
		}
		if (keyResult == 0xd)
		{
			enable();
			DelayTime();				//��ʱ
			LCD_INIT();
			DisLine2_up();
			start_flag = 1;
			AddCount();
		}
		if (bNeedDisplay)
		{
			bNeedDisplay = 0;
			Display_LED();		//��ʾʱ����
		}
	}
}















