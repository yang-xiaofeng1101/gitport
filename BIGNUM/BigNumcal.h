//#include"BigNumcal.cpp"
#include<string>
#include<vector>
#include<stdio.h>
#include<iostream>
using namespace std;
#ifndef PHONE_H_INCLUDED
#define PHONE_H_INCLUDED
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
        
        ~largenum(){}
};
/*largenum:: largenum(const largenum &x);
largenum::largenum(const string s1);
largenum largenum:: operator+(const largenum & T);
largenum largenum:: operator-(const largenum & x);
bool largenum:: operator>(const largenum & x);
bool largenum:: operator ==(const largenum &x);
bool largenum:: operator <(const largenum &x);
largenum largenum::operator*(const largenum & x);
istream &operator>>(istream &is, largenum &x);
ostream &operator<<(ostream& os, largenum& x);
largenum & largenum::operator=(const largenum & n);
largenum::largenum(int n);*/
#endif