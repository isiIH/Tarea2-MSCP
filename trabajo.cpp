#include <iostream>
using namespace std;

int main(int argc, char **argv){
    
    if(argc != 1){
		cout << "Error. Debe ejecutarse como ./trabajo" << endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}