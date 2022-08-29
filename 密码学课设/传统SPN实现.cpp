#include<iostream>
using namespace std;
#include<string>
#include <cstdio>
#include <cstdlib>

//���ñ�־λ����Ϊ0����м��ܣ�Ϊ1����н���
int PAI_sz[16] = { 0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7 };
int PAI_pz[17] = { 0, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 };
int rePAI_sz[16] = { 0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 };
int rePAI_pz[17] = { 0, 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };

unsigned long int K;
unsigned int w_0;
unsigned int result;

unsigned int k[6];
unsigned int kNow;

unsigned int cal_K(int i)//�����n�ֵ���Կ
{
	k[i] = K << ((i - 1) * 4) >> (4 * 4);
	return k[i];
}

//����uֵ���ô��ֵ�K����һ�ֵ�w�����������
unsigned int cal_xor(unsigned int w)
{
	unsigned int u;
	u = w ^ kNow;//�������
	printf("u = %x\n", u);
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
	v = (PAI_sz[u_temp[0]] << 12) +
		(PAI_sz[u_temp[1]] << 8) +
		(PAI_sz[u_temp[2]] << 4) +
		(PAI_sz[u_temp[3]] << 0);

	/*for (int i = 0; i < 4; i++)
	{
		printf("*************%x\n", PAI_sz[u_temp[i]]);
	}*/
	printf("v = %x\n", v);
	return v;
}

//����wֵ�����ô���vֵ�����ݱ�PAI_pz���и�������λ�Ľ���
unsigned int cal_BoxP(unsigned int v, int flag)
{
	//�������飬�����������
	int table_v[17];
	int table_w[17];
	unsigned int w = 0;
	int j = 1;
	for (j = 1; j <= 16; j++)
	{
		table_v[j] = (v >> (16 - j)) & 1;
		printf("*%x", table_v[j]);
	}

	//���н���
	int i = 1;
	for (; i <= 16; i++)
	{
		//����˳��任
		table_w[PAI_pz[i]] = table_v[i];
	}

	//���������ַ���table_w ת��Ϊ������ w
	for (j = 1; j <= 16; j++)
	{
		w = w * 2 + (table_w[j]);
	}
	printf("w = %x\n", w);
	return w;
}

int main()
{
	int times;
	cin >> times;
	unsigned int uTemp, vTemp, wTemp;
	while (times--)
	{
		scanf_s("%x", &K);
		scanf_s("%x", &w_0);

		for (int i = 1; i <= 4; i++)
		{
			kNow = cal_K(i);

			if (i == 1)
				uTemp = cal_xor(w_0);
			else
				uTemp = cal_xor(wTemp);

			vTemp = cal_BoxS(uTemp, 0);

			if (i != 4)
				wTemp = cal_BoxP(vTemp, 0);
			else
			{
				kNow = cal_K(5);
				result = cal_xor(vTemp);
				break;
			}
		}
		printf("%x", result);
		//&=*******************************************
		//��������
		unsigned ciphertext = result ^ 1;
		for (int i = 4; i >= 1; i++)
		{
			if (i == 4)
			{
				kNow = k[5];
				result = cal_xor(ciphertext);
			}
			else
				wTemp = cal_BoxP(vTemp, 1);
			else
			{
				
			}

			if (i == 1)
				uTemp = cal_xor(w_0);
			else
				uTemp = cal_xor(wTemp);

			vTemp = cal_BoxS(uTemp, 1);

			
		}
	}

	system("pause");
	return 0;
}

/*
1
3A94D63F 26b7
*/