#include<iostream>   
#include<string> 
#include<memory.h>  
#include<string.h>
#include<iomanip>   
#include<algorithm>   
#include<stdio.h> 
#define maxn 10000
using namespace std;
class BigNum{
    private:
        int a[500];
        int len;
    public:
        BigNum(){len=1;memset(a,0,sizeof(a));}
        BigNum(const int b);
        BigNum(const char *);
        BigNum(const BigNum &);

        BigNum operator +(const BigNum &)const;
        BigNum operator -(const BigNum &)const;
        BigNum operator *(const BigNum &)const;
        BigNum operator /(const int )const;

        BigNum operator 
}