#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> busquedaExhaustiva(vector<int> &X, vector<vector<int>> F);
vector<vector<int>> beOptimizada(vector<int> &X, vector<vector<int>> F);
vector<vector<int>> greedyMSCP(vector<int> &X, vector<vector<int>> F);

vector<int> unionConjuntos(vector<vector<int>> &F);
vector<int> diferenciaConjuntos(vector<int> A, vector<int> B);
vector<int> interseccionConjuntos(vector<int> A, vector<int> B);

void imprimirConjunto(vector<int> &v);

/*--------------------------------------------------------------------------------*/

int main(int argc, char **argv){
    
    if(argc != 1){
		cout << "Error. Debe ejecutarse como ./trabajo" << endl;
		exit(EXIT_FAILURE);
	}

	//------Ejemplo 1---------
	// vector<int> S1 = {1,2,3};
	// vector<int> S2 = {5,6,4};
	// vector<int> S3 = {2,4,3,5};
	// vector<int> S4 = {3,5,7};
	// vector<int> S5 = {7,8};
	
	// vector<vector<int>> F = {S1,S2,S3,S4,S5};

	//------Ejemplo 2---------
	//Conjuntos
	// vector<int> S1 = {1,2,3,4,5,6};
	// vector<int> S2 = {1,4,7,10};
	// vector<int> S3 = {2,5,7,8,11};
	// vector<int> S4 = {10,11};
	// vector<int> S5 = {3,6,9,12};
	// vector<int> S6 = {5,6,8,9};

	// //Familia de conjuntos
	// vector<vector<int>> F = {S1,S2,S3,S4,S5,S6};

	vector<int> S1 = {1,2,3,4,5,6};
	vector<int> S2 = {5,6,8,9};
	vector<int> S3 = {10,11,12};
	vector<int> S4 = {1,4,7,10};
	vector<int> S5 = {2,5,8,11};
	vector<int> S6 = {3,6,9,12};

	vector<vector<int>> F = {S1,S2,S3,S4,S5,S6};

	//Universo
	vector<int> X = unionConjuntos(F);
	sort(X.begin(), X.end());

	//SC
	// vector<vector<int>> C = beOptimizada(X, F);
	vector<vector<int>> C = greedyMSCP(X,F);

	cout << "Número de subconjuntos en C: " << C.size() << endl;
	for(vector<int> S : C) {
		imprimirConjunto(S);
	}


	return EXIT_SUCCESS;
}

/* --------------- Solución 1 -------------- */
vector<vector<int>> busquedaExhaustiva(vector<int> &X, vector<vector<int>> F) {

	
	return F;

}

/* --------------- Solución 2 -------------- */
vector<vector<int>> beOptimizada(vector<int> &X, vector<vector<int>> F) {
	vector<vector<int>> C = {};
	vector<vector<int>> Fcopy = F;
	vector<int> newX;
	vector<int> numFaltantes;

	for(int numX : X) {
		int contador = 0;
		vector<int> subconj = {};

		for(vector<int> S : Fcopy) { //Busca en cada subconjunto el número del universo X
			if(find(S.begin(), S.end(), numX) != S.end()) {
				contador += 1;
				subconj = S;
			}
			
		}

		//Si solo un subconjunto contiene el número y no está en el conjunto, entonces se almacena en el conjunto C
		if(contador == 1 && find(C.begin(), C.end(), subconj) == C.end()) {
			C.push_back(subconj);
			F.erase(find(F.begin(), F.end(), subconj));
		}

	}

	//Números encontrados
	cout << "Números encontrados: ";
	newX = unionConjuntos(C);
	imprimirConjunto(newX);

	//Nuevo universo
	cout << "Números Faltantes: ";
	numFaltantes = diferenciaConjuntos(X,newX);
	imprimirConjunto(numFaltantes);


	cout << "F: ";
	for(vector<int> S : F)
		imprimirConjunto(S);

	if(!numFaltantes.empty()) busquedaExhaustiva(numFaltantes, F);

	//Aquí falta terminarlo

	return C;
}

/* --------------- Solución 3 -------------- */
vector<vector<int>> greedyMSCP(vector<int> &X, vector<vector<int>> F) {
	vector<int> U = X;
	vector<vector<int>> C = {};
	vector<int> maxS;
	vector<int> interseccionMax;
	vector<int> Sconjunto;

	while(!U.empty()) {
		maxS = {};
		interseccionMax = {};
		for(vector<int> S : F) {
			Sconjunto = interseccionConjuntos(U,S);
			// cout << "Interseccion: " << endl;
			// imprimirConjunto(Sconjunto);
			if(Sconjunto.size() > interseccionMax.size()) {
				interseccionMax = Sconjunto;
				maxS = S;
			}
		}

		// cout << "maxS: " << endl;
		// imprimirConjunto(maxS);
		U = diferenciaConjuntos(U,maxS);
		// cout << "U sin maxS: " << endl;
		// imprimirConjunto(U);
		C.push_back(maxS);
	}

	return C;

}

/* --------------- Operaciones de conjuntos -------------- */
vector<int> unionConjuntos(vector<vector<int>> &F) {
	vector<int> X = {};
	if(F.empty()) return X;
	X = F[0];

	for(vector<int> S : F) {
		for(int numS : S) {
			bool encontrado = false;

			for(int numX : X) {
				if(numX == numS) encontrado = true;
			}

			if(!encontrado) X.push_back(numS);
		}
	}

	return X;
}

vector<int> diferenciaConjuntos(vector<int> A, vector<int> B) {
	vector<int> C = {};

	for(int a : A) {
		bool encontrado = false;

		int j = 0;
		while(j<B.size() && !encontrado) {
			encontrado = a == B[j];
			j++;
		}

		if (!encontrado) {
			C.push_back(a);
		}
	}

	return C;
}

vector<int> interseccionConjuntos(vector<int> A, vector<int> B) {
	vector<int> C = {};

	for(int a : A) {
		for(int b : B) {
			if(a == b) C.push_back(a);
		}
	}

	return C;
}

void imprimirConjunto(vector<int> &v) {
	cout << "{";
	for(int num : v) {
		if(num == v[v.size()-1]) cout << num;
		else cout << num << ", ";
	}
	cout << "}" << endl;
}