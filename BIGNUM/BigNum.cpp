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
		len = 1; 
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
	
	~largenum(){}
};
