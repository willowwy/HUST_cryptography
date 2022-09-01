#include <iostream>
using namespace std;
#include <string>
#include <cstdio>
#include <cstdlib>
#include <math.h>

//设置标志位，若为0则进行加密，为1则进行解密
int PAI_sz[2][16] = { {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7},
					 {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5} };
int PAI_pz[2][17] = { {0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16},
					 {0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16} };

unsigned long int K;
unsigned int k[6];
unsigned int kNow;
int times;

unsigned int cal_K(int i) //计算第n轮的密钥
{
	k[i] = K << ((i - 1) * 4) >> (4 * 4);
	return k[i];
}

//计算u值，用此轮的K与上一轮的w进行亦或运算
unsigned int cal_xor(unsigned int w)
{
	unsigned int u;
	u = w ^ kNow; //亦或运算
	return u;
}

//计算v值，根据表PAI_sz进行替换
unsigned int cal_BoxS(unsigned int u, int flag)
{
	unsigned int u_temp[4];
	unsigned int v;
	//拆分u的各个部分进行替换运算
	u_temp[0] = (u & 0xf000) >> 12;
	u_temp[1] = (u & 0x0f00) >> 8;
	u_temp[2] = (u & 0x00f0) >> 4;
	u_temp[3] = (u & 0x000f) >> 0;

	//根据替换表，对各个部分进行替换后拼凑还原
	v = (PAI_sz[flag][u_temp[0]] << 12) +
		(PAI_sz[flag][u_temp[1]] << 8) +
		(PAI_sz[flag][u_temp[2]] << 4) +
		(PAI_sz[flag][u_temp[3]] << 0);

	return v;
}

//计算w值：利用此轮v值，根据表PAI_pz进行各个数据位的交换
unsigned int cal_BoxP(unsigned int qian, int flag)
{
	//存入数组，方便后续操作
	int table_qian[17];
	int table_hou[17];
	unsigned int hou = 0;
	int j = 1;
	for (j = 1; j <= 16; j++)
	{
		table_qian[j] = (qian >> (16 - j)) & 1;
	}

	//进行交换
	int i = 1;
	for (; i <= 16; i++)
	{
		//进行顺序变换
		table_hou[PAI_pz[flag][i]] = table_qian[i];
	}

	//将二进制字符串table_w 转换为整型数 w
	for (j = 1; j <= 16; j++)
	{
		hou = hou * 2 + (table_hou[j]);
	}
	return hou;
}

void output(unsigned int* a, unsigned int* b)
{
	for (int i = 0; i < times; ++i)
	{
		char s[5] = { '0', '0', '0', '0', '\0' };
		if (a[i] <= 0x000f)
			sprintf(&s[3], "%x", a[i]);
		else if (a[i] <= 0x00ff)
			sprintf(&s[2], "%x", a[i]);
		else if (a[i] <= 0x0fff)
			sprintf(&s[1], "%x", a[i]);
		else
			sprintf(&s[0], "%x", a[i]);
		for (int i = 0; i < 4; i++)
		{
			putchar(s[i]);
		}
		putchar(' ');
		//b
		if (b[i] <= 0x000f)
			sprintf(&s[3], "%x", b[i]);
		else if (b[i] <= 0x00ff)
			sprintf(&s[2], "%x", b[i]);
		else if (b[i] <= 0x0fff)
			sprintf(&s[1], "%x", b[i]);
		else
			sprintf(&s[0], "%x", b[i]);

		for (int i = 0; i < 4; i++)
		{
			putchar(s[i]);
		}
		putchar('\n');
	}
	
	return;
}

unsigned int CharToInt(char* s, int size)
{
	unsigned int sum = 0;
	int a = 0;
	for (int i = 0; i < size; ++i)
	{
		if (s[i] >= '0' && s[i] <= '9')
			a = s[i] - '0';
		else if (s[i] >= 'a' && s[i] <= 'f')
			a = s[i] - 'a' + 10;
		sum += (a * pow(16, size - 1 - i));
	}
	return sum;
}

int main()
{
	unsigned int w_0;
	unsigned int uTemp, vTemp, wTemp;

	cin >> times;//输入需要计算的行数
	getchar();

	unsigned int* result_0 = new unsigned int[times];
	unsigned int* result_1 = new unsigned int[times];

	for (int m = 0; m < times; ++m)
	{
		char c_K[9];
		for (int i = 0; i < 8; i++)
		{
			c_K[i] = getchar();
		}
		c_K[8] = '\0';
		getchar();
		K = CharToInt(c_K, 8);

		char c_w0[5];
		for (int i = 0; i < 4; i++)
		{
			c_w0[i] = getchar();
		}
		c_w0[4] = '\0';
		getchar();
		w_0 = CharToInt(c_w0, 4);

		wTemp = w_0;
		for (int i = 1; i <= 5; i++)
		{
			kNow = cal_K(i);
			uTemp = cal_xor(wTemp); //轮密钥异或运算

			vTemp = cal_BoxS(uTemp, 0); // S盒进行代换

			if (i == 4)
			{
				kNow = cal_K(++i);
				result_0[m] = cal_xor(vTemp); //最后一轮进行亦或
			}
			else
				wTemp = cal_BoxP(vTemp, 0); // P盒进行置换
		}

		//解密运算↓
		unsigned int ciphertext = result_0[m] ^ 1;
		for (int i = 5; i >= 1; i--)
		{
			kNow = k[i];
			if (i == 5)
			{
				vTemp = cal_xor(ciphertext);
				kNow = k[--i];
			}
			else
				vTemp = cal_BoxP(wTemp, 1);

			uTemp = cal_BoxS(vTemp, 1);

			wTemp = cal_xor(uTemp);
		}
		result_1[m] = wTemp;
	}
	output(result_0, result_1);
	system("pause");
	return 0;
}

/*
1
3a94d63f 26b7
*/

/*
3
3a94d63f 26b7
3a94d63f 26b7
3a94d63f 26b7
*/