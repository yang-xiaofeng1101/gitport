#include"BigNumcal.h"
int main()
{
	int flag = 0;
	largenum x1, x2;
	char c;
	cout << "输入第一个数，第二行输入运算符（+,-,*），第三行输入第二个数。(正负数均可)"<<endl;
	cin >> x1>>c>>x2;
	calculate(x1,c,x2);
	
	return 0;
}