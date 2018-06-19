#include <iostream>
#include <cstdio>
#include <string>
#include<memory.h>
#include<string.h>
#include<algorithm>
#include<vector>
using namespace std;

class largenum {

private:
	vector<int> a;
	int len;
	

public:
	bool sign;
	largenum() { 
		len = 1; //memset(a, 0, sizeof(a)); 
		a.resize(500,0);
		sign=0;
	}
	largenum(const string s);
	largenum(const int n);
	largenum(const largenum &);
	largenum &operator=(const largenum &);

	friend istream &operator>>(istream &is, largenum &x);
	friend ostream &operator<<(ostream &is, largenum &x);

	largenum operator+(const largenum &x);
	largenum operator-(const largenum &x);
	largenum operator*(const largenum &x);
	bool operator>(const largenum &);
	bool operator<(const largenum &);
	bool operator==(const largenum &);
	
	~largenum()
	{
		//cout << "xigouhanshu has been called!\n";
	}
};
largenum:: largenum(const largenum &x){
	sign=x.sign;
	len=x.len;
	a.resize(500,0);
	for(int i=0;i<len;i++){
		a[i]=x.a[i];
	}
}
largenum::largenum(const string s1)
	{
		int alen, cnt = 0, slen, temp, i, k;
		a.resize(500,0);
		string s(s1);
		if(s[0]=='-'){
			sign=1;
			s.erase(0,1);
		}
		else sign=0;
		slen = s.size();
		alen = slen / 4;
		len = alen; 
		if (slen % 4)alen++;
		for (i = slen - 1; i >= 0; i -= 4) {
			temp = 0; 
			k = i - 4 + 1;
			if (k<0)k = 0;
			for (; k <= i; k++) {
				temp = temp * 10 + s[k] - '0';
			}
			a[cnt++]=temp;
		}
	}

largenum largenum:: operator+(const largenum & T)
	{
		largenum t(*this);
		int i, l;       
		l = T.len > len ? T.len : len;
		for (i = 0; i < l; i++)
		{
			t.a[i] += T.a[i];
			if (t.a[i] > 9999)
			{
				//if(i+1==)
				t.a[i + 1]++;
				t.a[i] -= 10000;
			}
		}
		if (t.a[l] != 0)
			t.len = l + 1;
		else
			t.len = l;
		return t;
	}

largenum largenum:: operator-(const largenum & x)
	{
		int i, j, l, flag = 1;
		largenum t1(*this), t2;
		if (*this>(x)) {
			t1 = *this;
			t2 = x;
		}
		else {
			t1 = x;
			t2 = *this;
			flag = -1;
		}
		l = t1.len;	
		for (i = 0; i<len; i++)
		{
			if (t1.a[i]<t2.a[i]) {
				j = i + 1;
				while (t1.a[j] == 0) {
					j++;
				}
				t1.a[j--]--;
				while (j>i)
					t1.a[j--] += 9999;
				t1.a[i] += 10000 - t2.a[i];
			}
			else t1.a[i] -= t2.a[i];
		}
		while (0 == t1.a[t1.len - 1] && t1.len>0)
		{
			t1.len--;
		}
		if (flag == -1)cout << '-';
		return t1;
	}

bool largenum:: operator>(const largenum & x)
{
		int l;
		if(len!=x.len)return len>x.len;
		else{
			for(int i=len-1;i>=0;i--){
				if(a[i]!=x.a[i])return a[i]>x.a[i];
			}
		}
		return false;
}
bool largenum:: operator ==(const largenum &x){
	if(sign!=x.sign)return false;
	if(len!=x.len)return false;
	else {
		for(int i=len-1;i>=0;i--){
			if(a[i]!=x.a[i])return false;
		}
	}
	return true;
}
bool largenum:: operator <(const largenum &x){
	return (!(*this>x))&&(!(*this==x));
}

largenum largenum::operator*(const largenum & x)
{
	largenum aim;
	int i, j;
	for (i = 0; i<len; i++) {
		for (j = 0; j<x.len; j++) {
			aim.a[i + j] += a[i] * x.a[j];
		}
	}
	for (i = 0; i<len + x.len - 1; i++) {
		aim.a[i + 1] += aim.a[i] / 10000;
		aim.a[i] %= 10000;
	}
	aim.len = x.len+len;
	while(aim.a[aim.len-1]==0)aim.len--;
	return aim;
}
