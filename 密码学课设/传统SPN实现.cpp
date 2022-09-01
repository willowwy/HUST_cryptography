#include <iostream>
using namespace std;
#include <string>
#include <cstdio>
#include <cstdlib>
#include <math.h>

//���ñ�־λ����Ϊ0����м��ܣ�Ϊ1����н���
int PAI_sz[2][16] = { {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7},
					 {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5} };
int PAI_pz[2][17] = { {0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16},
					 {0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16} };

unsigned long int K;
unsigned int k[6];
unsigned int kNow;
int times;

unsigned int cal_K(int i) //�����n�ֵ���Կ
{
	k[i] = K << ((i - 1) * 4) >> (4 * 4);
	return k[i];
}

//����uֵ���ô��ֵ�K����һ�ֵ�w�����������
unsigned int cal_xor(unsigned int w)
{
	unsigned int u;
	u = w ^ kNow; //�������
	return u;
}

//����vֵ�����ݱ�PAI_sz�����滻
unsigned int cal_BoxS(unsigned int u, int flag)
{
	unsigned int u_temp[4];
	unsigned int v;
	//���u�ĸ������ֽ����滻����
	u_temp[0] = (u & 0xf000) >> 12;
	u_temp[1] = (u & 0x0f00) >> 8;
	u_temp[2] = (u & 0x00f0) >> 4;
	u_temp[3] = (u & 0x000f) >> 0;

	//�����滻���Ը������ֽ����滻��ƴ�ջ�ԭ
	v = (PAI_sz[flag][u_temp[0]] << 12) +
		(PAI_sz[flag][u_temp[1]] << 8) +
		(PAI_sz[flag][u_temp[2]] << 4) +
		(PAI_sz[flag][u_temp[3]] << 0);

	return v;
}

//����wֵ�����ô���vֵ�����ݱ�PAI_pz���и�������λ�Ľ���
unsigned int cal_BoxP(unsigned int qian, int flag)
{
	//�������飬�����������
	int table_qian[17];
	int table_hou[17];
	unsigned int hou = 0;
	int j = 1;
	for (j = 1; j <= 16; j++)
	{
		table_qian[j] = (qian >> (16 - j)) & 1;
	}

	//���н���
	int i = 1;
	for (; i <= 16; i++)
	{
		//����˳��任
		table_hou[PAI_pz[flag][i]] = table_qian[i];
	}

	//���������ַ���table_w ת��Ϊ������ w
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

	cin >> times;//������Ҫ���������
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
			uTemp = cal_xor(wTemp); //����Կ�������

			vTemp = cal_BoxS(uTemp, 0); // S�н��д���

			if (i == 4)
			{
				kNow = cal_K(++i);
				result_0[m] = cal_xor(vTemp); //���һ�ֽ������
			}
			else
				wTemp = cal_BoxP(vTemp, 0); // P�н����û�
		}

		//���������
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