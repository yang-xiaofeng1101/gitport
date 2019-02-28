#include<memory.h>
#include<algorithm>
#include"BigNumcal.hpp"
using namespace std;

largenum::largenum(int n)
{
	int len = 0;
	a.resize(500,0);
	while (n >= 10000) {
		a[len++] = n % 10000;
		n /= 10000;
	}
	a[len++] = n;
}
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
		else if(*this==x){
			t1 = *this;
			t2 = x;
		}
		else {
			t1 = x;
			t2 = *this;
			t1.sign=1;
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
	if((a[0]==0&&len==1)||(x.a[0]==0&&x.len==1)){
		aim=largenum();
		return aim;
	}
	if((sign==1&&x.sign==0)||(sign==0&&x.sign==1))aim.sign=1;
	else aim.sign=0;
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
istream &operator>>(istream &is, largenum &x)
{
	string s1;
	is >> s1;
	string s(s1);
	if(s[0]=='-'){
		x.sign=1;
		s.erase(0,1);
	}
	else {
		x.sign=0;
	}
	
	x.a.resize(500,0);	
	int alen, cnt = 0, slen, temp, i, k;
	slen = s.size();
	alen = slen / 4;    
	x.len = alen;       
	if (slen % 4)x.len++;
	for (i = slen - 1; i >= 0; i -= 4) {
		temp = 0; 
		k = i - 4 + 1;
		if (k<0)k = 0;
		for (; k <= i; k++) {
			temp = temp * 10 + s[k] - '0';
		}
		x.a[cnt++]=temp;
	}
	return is;
}

ostream &operator<<(ostream& os, largenum& x)
{
	int i;
	
	if(x.sign==1){
		cout<<'-';
	}
	cout << x.a[x.len - 1];
	for (i = x.len - 2; i >= 0; i--) {
		printf("%04d", x.a[i]);
	}
	return os;
}

largenum & largenum::operator=(const largenum & n)
{
	int i;
	len = n.len;
	sign=n.sign;
	for (i = 0; i < len; i++)
		a[i] = n.a[i];
	return *this;
} 

void calculate(largenum x1,char c,largenum x2 ){
	int flag = 0;
	largenum x3;
	
	if ('+' == c)flag = 1;
	else if ('-' == c)flag = 2;
	else if ('*' == c)flag = 3;
	else cout << "type false!\n";
	switch (flag) {
		case 1:{
			cout << "x1" << c << "x2=";
			if((x1.sign==0&&x2.sign==0)||(x1.sign==1&&x2.sign==1)){
				x3 = (x1 + x2);
				cout << x3 << endl; 
				break;
			}
			else if(x1.sign==0&&x2.sign==1){
				x2.sign=0;
				x3 = (x1 - (x2)); 
				cout << x3 << endl; 
				break; 
			}
			else if(x1.sign==1&&x2.sign==0){
				x1.sign=0;
				x3 = (x2 - (x1)); 
				cout << x3 << endl; 
				break; 
			}
		}
		case 2:{
			cout << "x1" << c << "x2=";
			if(x1.sign==0&&x2.sign==0){
				x3 = (x1- x2); 
				cout << x3 << endl; 
				break;
			}
			else if(x1.sign==1&&x2.sign==1){
				x1.sign=x2.sign=0;
				x3=(x2-(x1));
				cout<<x3<<endl;
				break;
				}
			else {x3 = (x1 + (x2)); cout << x3 << endl; break; }
		}
		case 3:{
			cout << "x1" << c << "x2=";
			x3 = x1 * x2; 
			cout<<x3<<endl;
			 break;}
	}
	cout << endl;
}