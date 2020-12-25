#ifndef BIG_H
#define BIG_H

#include<iostream>
#include<vector>
using namespace std;
class big
{
public:
	const static int digit = 50;	//�����λ
	bool sign = 1;	//������
	bool overflow = 0;	//�Ƿ����
	bool isDivided = 1;	//�Ƿ�ɳ�
	int pos = 0;	//С����λ�ã��ȼ���С������
	int size = 1;	//���ָ���
	vector<short>num;	//���ִ洢
	big();
	big(int x);
	big(long long x);
	big(double x);
	big(string x);
	void toBig(int x);
	void toBig(long long x);
	void toBig(double x);
	void toBig(string x);
	string attribute();
	friend ostream& operator<<(ostream& output, big obj);
	big operator+(big obj);
	big operator-(big obj);
	big operator*(big obj);
	big operator/(big obj);
	big operator+=(big obj);
	big operator-=(big obj);
	big operator*=(big obj);
	big operator/=(big obj);
	bool operator<(big obj);
	bool operator>(big obj);
	bool operator==(big obj);
	bool operator<=(big obj);
	bool operator>=(big obj);
	void printArray(vector<short>vec);
	static big bigPow(big, int);
	big toDec(string x,int b);
};

#endif
