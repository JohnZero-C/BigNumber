#ifndef BIG_H
#define BIG_H

#include<iostream>
#include<vector>
using namespace std;
class big
{
public:
	const static int digit = 50;	//最大数位
	bool sign = 1;	//正负性
	bool overflow = 0;	//是否溢出
	bool isDivided = 1;	//是否可除
	int pos = 0;	//小数点位置，等价于小数个数
	int size = 1;	//数字个数
	vector<short>num;	//数字存储
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
