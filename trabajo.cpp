#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

vector<int> split (string s, string delimiter);

vector<vector<int>> busquedaExhaustiva(vector<int> X, vector<vector<int>> F);

vector<int> sumarUno(vector<int> lista);
int contarUnos(vector<int> lista);
vector<vector<int>> conjuntosUtilizados(vector<int> Faceptados, vector<vector<int>> F);

vector<vector<int>> beOptimizada(vector<int> X, vector<vector<int>> F);
vector<vector<int>> greedyMSCP(vector<int> X, vector<vector<int>> F);
vector<vector<int>> greedyOptimizado(vector<int> X, vector<vector<int>> F, int k);
vector<vector<int>> conjuntosConElementoUnico(vector<int> &X, vector<vector<int>> &F);

vector<int> unionConjuntos(vector<vector<int>> F);
vector<int> diferenciaConjuntos(vector<int> A, vector<int> B);
vector<int> interseccionConjuntos(vector<int> A, vector<int> B);

void imprimirConjunto(vector<int> v);

/*--------------------------------------------------------------------------------*/

int main(int argc, char **argv){
    
    if(argc != 2){
		cout << "Error. Debe ejecutarse como ./trabajo archivo" << endl;
		exit(EXIT_FAILURE);
	}

	string file = argv[1];
	ifstream archivo;
    archivo.open("data/" + file, ios::in);
    string linea;
	string palabra;
	vector<int> numeros;
	vector<vector<int>> F;

    if(archivo.fail()){
        cout << "Error" << endl;
        exit(1);
    }
    while (getline(archivo, linea)) {
		// stringstream text_stream(linea);
		// string item;
		// while (getline(text_stream, item, ' ')) {
		// 	numeros.push_back(stoi(item));
		// }

		numeros = split(linea, " ");

		// cout << numeros.size() << endl;
		// imprimirConjunto(numeros);
		F.push_back(numeros);
		numeros.clear();
    }
    archivo.close();


	//------Ejemplo 1--------- SOL = {S1, S2, S5}
	// vector<int> S1 = {1,2,3};
	// vector<int> S2 = {5,6,4};
	// vector<int> S3 = {2,4,3,5};
	// vector<int> S4 = {3,5,7};
	// vector<int> S5 = {7,8};
	
	// vector<vector<int>> F = {S1,S2,S3,S4,S5};

	//------Ejemplo 2--------- SOL = {S2,S3,S5}
	// Conjuntos
	// vector<int> S1 = {1,2,3,4,5,6};
	// vector<int> S2 = {1,4,7,10};
	// vector<int> S3 = {2,5,7,8,11};
	// vector<int> S4 = {10,11};
	// vector<int> S5 = {3,6,9,12};
	// vector<int> S6 = {5,6,8,9};

	// //Familia de conjuntos
	// vector<vector<int>> F = {S1,S2,S3,S4,S5,S6};

	//------Ejemplo 3--------- SOL = {S6,S4,S5}
	// vector<int> S1 = {1,2,3,4,5,6};
	// vector<int> S2 = {5,6,8,9};
	// vector<int> S3 = {10,11,12};
	// vector<int> S4 = {1,4,7,10};
	// vector<int> S5 = {2,5,8,11};
	// vector<int> S6 = {3,6,9,12};

	// vector<vector<int>> F = {S1,S2,S3,S4,S5,S6};



	//Universo
	vector<int> X = unionConjuntos(F);
	sort(X.begin(), X.end());
	cout << "Universo: ";
	imprimirConjunto(X);

	vector<bool> sol;
	for(int i=0; i<F.size(); i++) {
		sol.push_back(false);
	}

	// //SC
	// vector<vector<int>> C = busquedaExhaustiva(X, F); // Solución 1
	vector<vector<int>> C = beOptimizada(X, F); // Solución 2
	// vector<vector<int>> C = greedyMSCP(X,F); // Solución 3
	// vector<vector<int>> C = greedyOptimizado(X, F, 1); // Solución 4


	cout << "Número de subconjuntos en C: " << C.size() << endl;
	for(vector<int> S : C) {
		imprimirConjunto(S);
	}


	return EXIT_SUCCESS;
}

vector<int> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<int> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (stoi(token));
    }

    res.push_back (stoi( s.substr(pos_start) ));
    return res;
}

/* --------------- Solución 1 -------------- */
vector<vector<int>> busquedaExhaustiva(vector<int> X, vector<vector<int>> F) {
	int c = 0;
	int n = X.size();
	int csol = F.size() + 99;
	vector<int> Faceptados;
	//Inicializar en 0
	for(int i=0; i<F.size(); i++) {
		Faceptados.push_back(0);
	}
	vector<vector<int>> sol;
	vector<vector<int>> cUtilizados;

	while(c<F.size()) {
		Faceptados = sumarUno(Faceptados);
		c = contarUnos(Faceptados);
		cUtilizados = conjuntosUtilizados(Faceptados, F);
		if(Faceptados.size() < csol && interseccionConjuntos(unionConjuntos(cUtilizados),X).size() == n) {
			sol = cUtilizados;
			csol = c;
		}
	}

	return sol;
}

vector<int> sumarUno(vector<int> lista) { //O(n)
	int i = lista.size() - 1;

	while(i >= 0 && lista[i] == 1) {
		lista[i] = 0;
		i -= 1;
	}

	if(i>=0) {
		lista[i] = 1;
	}

	return lista;
}

int contarUnos(vector<int> lista) {
	int c = 0;

	for(int i : lista) {
		if(i == 1) {
			c++;
		}
	}
	
	return c;
}

vector<vector<int>> conjuntosUtilizados(vector<int> Faceptados, vector<vector<int>> F) { //O(n)
	vector<vector<int>> res;

	for(int i=0; i<Faceptados.size(); i++) {
		if(Faceptados[i] == 1) {
			res.push_back(F[i]);
		}
	}

	return res;
}

/* --------------- Solución 2 -------------- */
vector<vector<int>> beOptimizada(vector<int> X, vector<vector<int>> F) {
	cout << "Buscando conjuntos con elemento único..." << endl;
	vector<vector<int>> C = conjuntosConElementoUnico(X,F);
	vector<vector<int>> CFaltante;
	vector<int> numEncontrados;

	//Números encontrados
	cout << "Números encontrados: ";
	numEncontrados = unionConjuntos(C);
	imprimirConjunto(numEncontrados);

	//Nuevo universo
	cout << "Números Faltantes (Nuevo X): ";
	imprimirConjunto(X);

	cout << "F restante: " << endl;
	for(vector<int> S : F)
		imprimirConjunto(S);

	if(!X.empty()) {
		cout << "Se realizará busqueda exhaustiva..." << endl;
		CFaltante = busquedaExhaustiva(X, F);
		for(vector<int> s : CFaltante) {
			imprimirConjunto(s);
			C.push_back(s);
		}
	}

	return C;
}

/* --------------- Solución 3 -------------- */
vector<vector<int>> greedyMSCP(vector<int> X, vector<vector<int>> F) {
	vector<int> U = X;
	vector<vector<int>> C = {};
	vector<int> maxS;
	int interseccionMax;
	int interseccion;

	while(!U.empty()) {
		maxS = {};
		interseccionMax = 0;
		for(vector<int> S : F) {
			interseccion = interseccionConjuntos(U,S).size();
			// cout << "Interseccion: " << endl;
			// imprimirConjunto(Sconjunto);
			if(interseccion > interseccionMax) {
				interseccionMax = interseccion;
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

/* --------------- Solución 4 -------------- */
vector<vector<int>> greedyOptimizado(vector<int> X, vector<vector<int>> F, int k) {
	vector<vector<int>> C = conjuntosConElementoUnico(X,F);
	vector<int> numEncontrados;

	//Números encontrados
	cout << "Números encontrados: ";
	numEncontrados = unionConjuntos(C);
	imprimirConjunto(numEncontrados);

	//Nuevo universo
	cout << "Números Faltantes (Nuevo X): ";
	imprimirConjunto(X);

	cout << "F restante: " << endl;
	for(vector<int> S : F)
		imprimirConjunto(S);

	if(!X.empty()) {
		busquedaExhaustiva(X, F);
		//Falta terminarlo
	}

	//Algoritmo del greedy normal, hay que cambiarlo para cualquier k

	vector<int> U = X;
	vector<vector<int>> kconjuntos;
	vector<int> maxS;
	int interseccionMax;
	int interseccion;

	while(!U.empty()) {
		maxS = {};
		interseccionMax = 0;
		for(int i=0; i<=F.size()-k; i++) {
			kconjuntos = {};

			interseccion = interseccionConjuntos(U,F[i]).size();
			// cout << "Interseccion: " << endl;
			// imprimirConjunto(Sconjunto);
			if(interseccion > interseccionMax) {
				interseccionMax = interseccion;
				maxS = F[i];
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

vector<vector<int>> conjuntosConElementoUnico(vector<int> &X, vector<vector<int>> &F) {
	vector<vector<int>> C = {};
	vector<int> newX = X;
	int contador;
	vector<int> subconj;

	for(int numX : X) {
		if(find(newX.begin(), newX.end(), numX) != newX.end()) {
			contador = 0;
			subconj = {};

			for(vector<int> S : F) { //Busca en cada subconjunto el número del universo X
				if(find(S.begin(), S.end(), numX) != S.end()) {
					contador += 1;
					subconj = S;
				}
				
			}

			//Si solo un subconjunto contiene el número, entonces se almacena en el conjunto C
			if(contador == 1) {
				// imprimirConjunto(subconj);
				C.push_back(subconj);
				F.erase(find(F.begin(), F.end(), subconj));
				newX = diferenciaConjuntos(newX, subconj);
				// imprimirConjunto(newX);
			}
		}
	}

	X = newX;

	return C;
}

/* --------------- Operaciones de conjuntos -------------- */
vector<int> unionConjuntos(vector<vector<int>> F) {
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

void imprimirConjunto(vector<int> v) {
	if(v.empty()) {
		cout << "{}" << endl;
		return;
	}
	int i;
	cout << "{";
	for(i=0; i<v.size()-1; i++) {
		cout << v[i] << ", ";
	}
	cout << v[v.size()-1] << "}" << endl;
}