#include<iostream>
#include<sstream>
#include<iomanip>
#include<cmath>
#include"big.h"
using namespace std;

vector<string> split(const  std::string& s, const std::string& delim){
	vector<string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

big::big(){
	num.resize(digit);
}

big::big(int x){
	num.resize(digit);
	toBig(x);
}

big::big(long long x){
	num.resize(digit);
	toBig(x);
}

big::big(double x){
	num.resize(digit);
	toBig(x);
}

big::big(string x){
	num.resize(digit);
	toBig(x);
}

void big::toBig(int x){
	big obj;
	*this = obj;
	if (!x) return;
	if (x < 0) sign = 0,x=-x;
	num.clear();
	while (x){
		num.push_back( x % 10);
		x /= 10;
	}
	if (num.size() > digit) num.erase(num.begin(),num.begin()+num.size()-digit),overflow=1;
	size = num.size();
	num.resize(digit);
}

void big::toBig(long long x){
	big obj;
	*this = obj;
	if (!x) return;
	if (x < 0) sign = 0, x = -x;
	num.clear();
	while (x){
		num.push_back(x % 10);
		x /= 10;
	}
	if (num.size() > digit) num.erase(num.begin(), num.begin() + num.size() - digit), overflow = 1;
	size = num.size();
	num.resize(digit);
}

void big::toBig(double x){
	string str;
	stringstream ss;
	ss << fixed<<setprecision(digit) << x;
	ss >> str;
	toBig(str);	
}

void big::toBig(string x){
	big obj;
	*this = obj;
	if (x[0] == '-') sign = 0, x = x.substr(1, x.size() - 1);
	vector<string>vec = split(x, ".");
	string str;
	for (auto it : vec)
		str+= it;
	if (vec.size() == 2 && vec[0].size() > digit) overflow = 1;
	size = str.size()>digit?digit:str.size();
	pos = size - vec.front().size();
	if (pos < 0) pos = 0;
	for (int i = 0; i < size; i++)
		num[size-1-i] = str[i]-'0';
	while (num[0] == 0 && pos > 0) num.erase(num.begin()), size--, pos--;
}

string big::attribute(){
	return   "(" + to_string(sign) + "," + to_string(size) + "," + to_string(pos) + "," + to_string(overflow) + ")";
}

ostream& operator<<(ostream& output, big obj){
	if (!obj.sign) output << '-';
	for (int i = obj.size - 1; i > 0; i--){
		output << obj.num[i];
		if (i == obj.pos) output << '.';
	}
	output << obj.num[0];
	return output;
}

big big::operator+(big obj){
	big obj0;
	obj0.overflow = overflow || obj.overflow;
	big round;
	bool judge = false;
	big obj1=*this, obj2=obj;
	if (obj1.sign == 1 && obj2.sign == 0){
		obj2.sign = 1;
		return obj1-obj2;
	}
	if (obj1.sign == 0 && obj2.sign == 1){
		obj1.sign = 1;
		return obj2 - obj1;
	}
	vector<short>vec0;
	vector<short>vec1=num;
	vector<short>vec2=obj.num;
	int size0,size1=size,size2=obj.size,pos0,pos1=pos,pos2=obj.pos;
	int overflow0 = 0;
	if (pos1 >= pos2){
		pos0 = pos1;
		vec2.insert(vec2.begin(), pos1 - pos2, 0);
		size2 += pos1 - pos2;
	}else{
		pos0 = pos2;
		vec1.insert(vec1.begin(), pos2 - pos1, 0);
		size1 += pos2 - pos1;
	}
	size0 = size1 >= size2 ? size1 : size2;
	vec1.resize(size0);
	vec2.resize(size0);
	int carry = 0;
	for (int i = 0; i < size0; i++){
		int sum = vec1[i] + vec2[i] + carry;
		vec0.push_back(sum % 10);
		if (sum / 10) carry = 1;
		else carry = 0;
	}
	if(carry) vec0.push_back(1),size0++;
	while (vec0[0] == 0&&pos0>0) vec0.erase(vec0.begin()), size0--, pos0--;
	if (size0 > digit) {
		if(size0-pos0>digit) obj0.overflow = 1;
		pos0 -= size0 - digit;
		if (pos0 < 0) pos0 = 0;
		judge = vec0[size0 - digit - 1] > 4;
		vec0.erase(vec0.begin(), vec0.begin() + size0 - digit );
		size0 = digit;
	}
	vec0.resize(digit);
	obj0.size = size0;
	obj0.pos = pos0;
	obj0.num = vec0;
	if (sign == 0 && obj.sign == 0) obj0.sign=0;
	if (judge) round.size = obj.size, round.pos = obj0.pos, round.num[0] = 1;
	return judge?obj0+round:obj0;
}

big big::operator-(big obj)
{
	int sign0=1,size0,pos0;
	big obj0, obj1, obj2;
	obj0.overflow = overflow || obj.overflow;
	if (sign){
		if (obj.sign){
			if (*this == obj) return big(0);
			if (*this > obj) obj1 = *this, obj2 = obj,sign0=1;
			if (*this < obj) obj1 = obj, obj2 = *this,sign0=0;
		}else{
			obj0 = obj;
			obj0.sign = 1;
			return *this + obj0;
		}
	}else{
		if (obj.sign)
		{
			obj0 = *this;
			obj0.sign = 1;
			return big(0)-(obj0 + obj);
		}else{
			obj1 = *this, obj1.sign = 1;
			obj2 = obj, obj2.sign = 1;
			return obj2 - obj1;
		}
	}
	pos0 = obj1.pos >= obj2.pos ? obj1.pos : obj2.pos;
	size0 = pos0 + (obj1.size-obj1.pos>=obj2.size-obj2.pos?obj1.size-obj1.pos:obj2.size-obj2.pos);
	obj1.num.insert(obj1.num.begin(), pos0 - obj1.pos, 0);
	obj2.num.insert(obj2.num.begin(), pos0 - obj2.pos, 0);
	obj1.num.resize(size0);
	obj2.num.resize(size0);
	int carry=0;
	obj0.num.resize(0);
	for (int i = 0; i < size0; i++){
		if (carry){
			if (obj1.num[i] == 0) obj1.num[i] = 9;
			else obj1.num[i]--,carry=0;
		}
		if (obj1.num[i] < obj2.num[i]){
			obj1.num[i] += 10;
			carry = 1;
		}
		obj0.num.push_back(obj1.num[i] - obj2.num[i]);
	}
	while (obj0.num[0] == 0&&pos0>0) obj0.num.erase(obj0.num.begin()),size0--,pos0--;
	for (int i = size0 - 1; i > pos0 && obj0.num[i] == 0; i--) size0--;
	if (size0 > obj0.digit){
		obj0.num.erase(obj0.num.begin(), obj0.num.begin() + size0 - obj0.digit);
		pos0 -= size0 - obj0.digit;
		size0 = obj0.digit;
	}
	obj0.num.resize(obj0.digit);
	obj0.sign = sign0;
	obj0.size = size0;
	obj0.pos = pos0;
	return obj0;
}

big big::operator*(big obj){
	big obj0;
	obj0.overflow = overflow || obj.overflow;
	big round;
	bool judge=false;
	int m = size, n = obj.size;
	vector<vector<short>>vec(m,vector<short>(m+n));
	for (int i = 0; i < m; i++){
		int carry = 0;
		int j;
		for (j = 0; j < n; j++){
			int sum = num[i] * obj.num[j]+carry;

			vec[i][i + j] = sum % 10;
			carry = sum / 10;
		}
		if (carry) vec[i][i+j] = carry;
	}
	vector<short>vec0;
	
	int carry = 0;
	for (int j = 0; j < m + n; j++){
		for (int i = 0; i < m; i++)
			carry += vec[i][j];
		vec0.push_back(carry % 10);
		carry /= 10;
	}
	while(carry) vec0.push_back(carry % 10),carry /= 10;

	obj0.sign = (sign == obj.sign);
	round.sign = obj0.sign;
	obj0.pos = pos + obj.pos;
	while (vec0.back() == 0 && vec0.size() > obj0.pos + 1) vec0.pop_back();
	if (vec0.size() > digit){
		judge = vec0[vec0.size() - digit - 1] > 4;
		obj0.pos -= vec0.size() - digit;
		vec0.erase(vec0.begin(), vec0.begin() + vec0.size() - digit);
		if (obj0.pos < 0) obj0.pos = 0,obj0.overflow=1;
		if (judge) round.size = vec0.size(), round.pos = obj0.pos,round.num[0]=1;
	}
	while (vec0[0] == 0 && obj0.pos > 0) vec0.erase(vec0.begin()), obj0.pos--;
	
	obj0.size = vec0.size();
	obj0.num = vec0;
	return judge?obj0+round:obj0;
}

big big::operator/(big obj){
	if (obj<big(1) && obj>big(-1)&&obj.isDivided){
		obj.isDivided = 0;
		return *this * (big(1) / obj);
	}
	big obj0, obj1=*this, obj2=obj,obj3,round;
	bool judge = false;
	obj1.sign = 1, obj2.sign = 1;
	obj1.overflow = 0, obj1.overflow = 0;
	obj0.overflow = (overflow || obj.overflow);
	if (size == 1 && num[0] == 0) return obj0;
	obj0.sign = (sign == obj.sign);
	obj0.num.clear();
	obj0.pos = obj1.pos>=obj2.pos?obj1.pos:obj2.pos;
	if (obj0.pos >= obj1.pos)  obj1.num.insert(obj1.num.begin(), obj0.pos - obj1.pos, 0), obj1.size += obj0.pos - obj1.pos, obj1.pos = 0;
	if (obj0.pos >= obj2.pos)  obj2.num.insert(obj2.num.begin(), obj0.pos - obj2.pos, 0), obj2.size += obj0.pos - obj2.pos, obj2.pos = 0;
	while (obj1.num[obj1.size - 1] == 0) obj1.size--;
	while (obj2.num[obj2.size - 1] == 0) obj2.size--;
	obj1.num.resize(obj1.size);
	obj2.num.resize(obj2.size);
	if (obj2.size > digit - 2){	//obj2.size最大只能为digit-2，否则后面的-和*会溢出
		int val = abs(obj2.size-digit+2);
		obj1.num.erase(obj1.num.begin(), obj1.num.begin() + val);
		obj1.size-=val;
		obj2.num.erase(obj2.num.begin(), obj2.num.begin() + val);
		obj2.size -= val;
	}
	obj1.num.insert(obj1.num.begin(), digit + obj2.size - obj1.size+1, 0);
	bool p = false;
	int count=0;
	obj0.pos = 0;
	for (int i = obj1.num.size()-1; i>=0; i--){	
		obj3=obj3*big(10)+big(obj1.num[i]);
		int carry = 0;
		for (int j=1; j <= 10; j++)		
			if (big(j) * obj2 >= obj3){	
				carry = 1;
				if (big(j) * obj2 > obj3){
					j--;
					obj3 = obj3 - big(j) * obj2;				
				}
				else obj3 = big(0);
				if (j){
					obj0.num.insert(obj0.num.begin(), j);
					if (i >= obj1.num.size() - obj1.size) obj0.pos++;
				}
				else carry=0;
				break;			
			}	
		if (!carry){
			if (i == obj1.num.size() - obj1.size) obj0.pos++;
			if (obj0.pos){
				if (i > obj1.num.size() - obj1.size) obj0.pos++;
				obj0.num.insert(obj0.num.begin(), 0);
			}
		}
	}
	
	obj0.size = obj0.num.size();
	int real = obj0.pos;
	obj0.pos = obj0.size - real;
	while (obj0.num.back() == 0 && obj0.size > obj0.pos + 1) obj0.num.pop_back(),obj0.size--;
	if (obj0.size >digit){
		if (obj0.num[obj0.size - digit - 1] > 4){
			judge = true;
		}
		obj0.num.erase(obj0.num.begin(), obj0.num.begin() + obj0.size - digit);
		obj0.pos -= obj0.size - digit,obj0.size = digit;

		if (obj0.pos < 0) obj0.pos = 0;
		round.size = obj0.size, round.pos = obj0.pos, round.num[0] = 1;
	}
	while (obj0.num[0] == 0 && obj0.pos > 0) obj0.num.erase(obj0.num.begin()), obj0.pos--,obj0.size--;
	obj0.num.resize(digit);
	

	return judge?obj0+round:obj0;
}

big big::operator+=(big obj){
	*this = *this + obj;
	return *this;
}

big big::operator-=(big obj){
	*this = *this - obj;
	return *this;
}

big big::operator*=(big obj){
	*this = *this * obj;
	return *this;
}

big big::operator/=(big obj){
	*this = *this / obj;
	return *this;
}

bool big::operator<(big obj){
	if (sign == 0){
		if (obj.sign == 0){
			int  a = (size - pos) - (obj.size - obj.pos);
			if (a < 0)	return false;
			if (a > 0) return true;
			if (a == 0){
				int who,pos0;
				if (pos <= obj.pos) pos0 = pos, who = 2;
				else pos0 = obj.pos, who = 1;
				for (int i = size - 1,  j = obj.size - 1; i >= pos-pos0; i--, j--){
					if (num[i] < obj.num[j]) return false;
					if (num[i] > obj.num[j]) return true;
				}
				if (who == 1) return true;
				else return false;
			}
		}
		else return true;	
	}
	else{
		if (obj.sign == 0) return false;
		else{
			int  a = (size - pos) - (obj.size - obj.pos);
			if (a < 0)	return true;
			if (a > 0) return false;
			if (a == 0){
				int who, pos0;
				if (pos < obj.pos) pos0 = pos, who = 2;
				else pos0 = obj.pos, who = 1;
				for (int i = size - 1,  j = obj.size - 1; i >= pos - pos0; i--, j--){
					if (num[i] < obj.num[j]) return true;
					if (num[i] > obj.num[j]) return false;
				}
				if (who == 1) return false;
				else return true;
			}
		}
	}
	return true;
}

bool big::operator>(big obj){
	return obj<*this;
}

bool big::operator==(big obj){
	return sign == obj.sign && overflow == obj.overflow && size == obj.size && pos == obj.pos && num == obj.num;
}

bool big::operator<=(big obj){
	return !(*this>obj);
}

bool big::operator>=(big obj){
	return !(*this<obj);
}

void big::printArray(vector<short>vec){
	for (int i = vec.size() - 1; i >= 0; i--) cout << vec[i];
	cout << endl;
}

big big::bigPow(big obj, int n){
	big obj0(1);
	for (int i = 0; i < n; i++) obj0 *= obj;
	return obj0;
}

big big::toDec(string x,int b){
	big a;
	for (int i = 0; i < x.length(); i++) {
		char c = x[i];
		if (c >= 'a' && c <= 'f') a = a*big(b)+big(10 + c - 'a');
		else if (c >= 'A' && c <= 'F') a = a * big(b) + big(10 + c - 'A');
		else a = a * big(b) + big(c - '0');
	}
	return a;
}