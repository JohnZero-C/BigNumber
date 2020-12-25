#include<stdc++.h>
#include"big.h"
using namespace std;
int main() {
	big a = a.toDec("1CA20C20F4781C", 16);
	big b = b.toDec("A20F4A20F4781C", 16);
	big c = a * b;
	cout << c<<endl;
	cout << c.attribute() << endl;
}