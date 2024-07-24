#include <iostream>
#include <string>
using namespace std;
int main(){
	int a = 10;
	int b;
	float x = 3.14;
	string mensaje = \"Hola, mundo\";
	bool flag = true;
	a=5;
	b=a+2;
	x+=1.86;
	mensaje=\"Nuevo mensaje\";
	cin >> a;
	cout << \"El valor de a es: \" << a <<  endl;
	cout << \"El valor de x es: \" << x <<  endl;
	cout << mensaje <<  endl;
	if (a>b) {
		cout << \"a es mayor que b\" <<  endl;
	} else {
		cout << \"a no es mayor que b\" <<  endl;
	}
	switch (a) {
		case 1:
			cout << \"a es 1\" <<  endl;
	break;
		case 2:
			cout << \"a es 2\" <<  endl;
	break;
		default:
			cout << \"a no es ni 1 ni 2\" <<  endl;
	break;
}
	for (int i = 0;
