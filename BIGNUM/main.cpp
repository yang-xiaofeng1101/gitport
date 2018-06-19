#include"BigNumcal.h"

int main()
{
	int flag = 0;
	largenum x1, x2, x3;
	char c;
	cout << "You can input a number,end with pressing \"enter\",and then input a kind of operations( + , - or * ),also end with enter;last input the other number, and then press \"Enter\" to coclulate them !" << endl;
	cin >> x1>>c>>x2;
	cout << endl << "x1=" << x1 << endl;
	cout << "x2=" << x2 << endl;
	cout << "x1" << c << "x2=";
	if ('+' == c)flag = 1;
	else if ('-' == c)flag = 2;
	else if ('*' == c)flag = 3;
	else cout << "type false!\n";
	switch (flag) {
	/*case 1: {x3 = (x1 + x2); cout << x3 << endl; break; }
	case 2: {x3 = (x1 - (x2)); cout << x3 << endl; break; }
	case 3: {x3 = (x1 * (x2)); cout << x3 <<endl; break; }
	default: cout << "error -1\n";*/
		case 1:{
			if((x1.sign==0&&x2.sign==0)||(x1.sign==1&&x2.sign==1)){
				x3 = (x1 + x2); 
				cout << x3 << endl; 
				break;
			}
			else if(x1.sign==0&&x2.sign==1){x3 = (x1 - (x2)); cout << x3 << endl; break; }
			else if(x1.sign==1&&x2.sign==0){x3 = (x2 - (x1)); cout << x3 << endl; break; }
		}
		case 2:{
			if(x1.sign==0&&x2.sign==0){
				x3 = (x1 + x2); 
				cout << x3 << endl; 
				break;
			}
			else if(x1.sign==1&&x2.sign==1){x3=(x2-(x1));cout<<x3<<endl;break;}
			else if(x1.sign==0&&x2.sign==1){x3 = (x1 + (x2)); cout << x3 << endl; break; }
			else if(x1.sign==1&&x2.sign==0){x3 = (x1 + (x2)); cout << x3 << endl; break; }
		
		}
		case 3:{x3=(x1*x2);cout<<x3<<endl;break;}
	}
	cout << endl;
	return 0;
}