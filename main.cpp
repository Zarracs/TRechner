#include "main.h"

int main(int argc, char* argv[]) {
	rechner rech = rechner();
	cout<<rech.modify("(1+2)*(3+4)")<<endl;
	cout<<rech.convert(rech.modify("(1+2)*(3+4)"))<<endl;
	system("PAUSE");
	return 0;
}
