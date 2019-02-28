#include"BigNumcal.hpp"
int main()
{
	largenum x1, x2;
	char c;
	cout << "输入第一个数,第二行输入第二个数。(正负数均可)"<<endl;
	cin >> x1>>x2;
	cout << endl << "x1=" << x1 << endl;
	cout << "x2=" << x2 << endl;
	calculate(x1,'+',x2);
	calculate(x1,'-',x2);
	calculate(x1,'*',x2);
	return 0;
}