#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

vector<vector<int>> busquedaExhaustiva(vector<int> X, vector<vector<int>> F);
vector<int> sumarUno(vector<int> lista);
int contarUnos(vector<int> lista);
vector<vector<int>> conjuntosUtilizados(vector<int> Faceptados, vector<vector<int>> F);

vector<vector<int>> beOptimizada(vector<int> X, vector<vector<int>> F);
vector<vector<int>> greedyMSCP(vector<int> X, vector<vector<int>> F);
vector<vector<int>> greedyOptimizado(vector<int> X, vector<vector<int>> F, int k);

vector<vector<int>> conjuntosConElementoUnico(vector<vector<int>> &F);
vector<int> unionConjuntos(vector<vector<int>> F);
vector<int> diferenciaConjuntos(vector<int> A, vector<int> B);
vector<int> interseccionConjuntos(vector<int> A, vector<int> B);

void imprimirConjunto(vector<int> v);

/*--------------------------------------------------------------------------------*/

int main(int argc, char **argv){
    
    if(argc != 3){
		cout << "Error. Debe ejecutarse como ./trabajo solucion archivo" << endl;
		exit(EXIT_FAILURE);
	}

	int solucion = stoi(argv[1]);
	string file = argv[2];
	clock_t start, end;
	ifstream archivo;
    archivo.open("data/" + file, ios::in);
    string linea;
	vector<int> numeros;
	vector<vector<int>> F;
	int k;

    // if(archivo.fail()){
    //     cout << "Error" << endl;
    //     exit(1);
    // }
    // while (getline(archivo, linea)) {
	// 	stringstream text_stream(linea);
	// 	string item;
	// 	while (getline(text_stream, item, ' ')) {
	// 		if(!item.empty()) numeros.push_back(stoi(item));
	// 	}
	// 	// cout << numeros.size() << endl;
	// 	// imprimirConjunto(numeros);
	// 	F.push_back(numeros);
	// 	numeros.clear();
    // }
    // archivo.close();

	vector<int> S1 = {1,2,3,4,5,6};
	vector<int> S2 = {5,6,8,9};
	vector<int> S3 = {10,11,12};
	vector<int> S4 = {1,4,7,10};
	vector<int> S5 = {2,5,8,11};
	vector<int> S6 = {3,6,9,12};

	F = {S1,S2,S3,S4,S5,S6};


	//Universo
	vector<int> X = unionConjuntos(F);
	sort(X.begin(), X.end());
	// cout << "Universo: ";
	// imprimirConjunto(X);

	//SC
	vector<vector<int>> C;
	switch(solucion) {
		case 1:
			start = clock();
			C = busquedaExhaustiva(X, F); // Solución 1
			end = clock();
			cout << "Tiempo de ejecución de la búsqueda exhaustiva: " << (double(end - start))/(CLOCKS_PER_SEC) << "s" << endl;
			break;
		case 2:
			start = clock();
			C = beOptimizada(X, F); // Solución 2
			end = clock();
			cout << "Tiempo de ejecución de la búsqueda exhaustiva optimizada: " << (double(end - start))/(CLOCKS_PER_SEC) << "s" << endl;
			break;
		case 3:
			start = clock();
			C = greedyMSCP(X,F); // Solución 3
			end = clock();
			cout << "Tiempo de ejecución del algoritmo greedy: " << (double(end - start))/(CLOCKS_PER_SEC) << "s" << endl;
			break;
		case 4:
			cout << "Ingrese un número k para el algoritmo greedy con k-conjuntos: ";
			cin >> k; 
			start = clock();
			C = greedyOptimizado(X, F, k); // Solución 4
			end = clock();
			cout << "Tiempo de ejecución del algoritmo greedy optimizado: " << (double(end - start))/(CLOCKS_PER_SEC) << "s" << endl;
			break;
		default:
			cout << "Los números de solución pueden ser: 1,2,3,4. Ejecute nuevamente" << endl;
			exit(EXIT_FAILURE);
	}

	cout << "Número de subconjuntos en C: " << C.size() << endl;
	for(vector<int> S : C) {
		imprimirConjunto(S);
	}

	return EXIT_SUCCESS;
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

int contarUnos(vector<int> lista) { //O(n)
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
	vector<vector<int>> C = conjuntosConElementoUnico(F);
	vector<vector<int>> CFaltante;
	vector<int> numEncontrados;

	//Números encontrados
	cout << "Números encontrados: ";
	numEncontrados = unionConjuntos(C);
	imprimirConjunto(numEncontrados);

	//Nuevo universo
	cout << "Números Faltantes (Nuevo X): ";
	X = diferenciaConjuntos(X,numEncontrados);
	imprimirConjunto(X);

	cout << "F restante: " << endl;
	for(vector<int> S : F)
		imprimirConjunto(S);

	if(!X.empty()) {
		cout << "Se realizará busqueda exhaustiva..." << endl;
		CFaltante = busquedaExhaustiva(X, F);
		for(vector<int> s : CFaltante) {
			// imprimirConjunto(s);
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
	vector<vector<int>> C = conjuntosConElementoUnico(F);
	vector<int> numEncontrados;

	//Números encontrados
	cout << "Números encontrados: ";
	numEncontrados = unionConjuntos(C);
	imprimirConjunto(numEncontrados);

	//Nuevo universo
	cout << "Números Faltantes (Nuevo X): ";
	X = diferenciaConjuntos(X,numEncontrados);
	imprimirConjunto(X);

	cout << "F restante: " << endl;
	for(vector<int> S : F)
		imprimirConjunto(S);

	//Greedy con k-conjuntos

	if(!X.empty()) {
		vector<int> U = X;
		vector<vector<int>> maxS;
		int interseccionMax;
		int interseccion;
		
		vector<int> kconjuntos;
		vector<vector<int>> cUtilizados;
		for(int i=0; i<F.size(); i++) {
			kconjuntos.push_back(0);
		}
		int c = 0;
		
		while(!U.empty()) {
			interseccionMax = 0;

			while(c != F.size()) {
				kconjuntos = sumarUno(kconjuntos);
				c = contarUnos(kconjuntos);

				if(c == k) {
					cUtilizados = conjuntosUtilizados(kconjuntos, F);
					interseccion = interseccionConjuntos(U,unionConjuntos(cUtilizados)).size();

					if(interseccion > interseccionMax) {
						interseccionMax = interseccion;
						maxS = cUtilizados;
					}
				}
			}

			// imprimirConjunto(unionConjuntos(maxS));
			U = diferenciaConjuntos(U,unionConjuntos(maxS));

			for(vector<int> S : maxS) {
				C.push_back(S);
			}

			maxS.clear();
		}
	}

	return C;
}

vector<vector<int>> conjuntosConElementoUnico(vector<vector<int>> &F) {
	vector<int> unico;
	vector<vector<int>> Fcopy = F;
	vector<vector<int>> C;

	for(vector<int> Sx : Fcopy) {
		unico = Sx;

		for(vector<int> Sy : Fcopy) {
			if(Sx != Sy) {
				unico = diferenciaConjuntos(unico, Sy);
			}
		}
		// cout << "----------" << endl;
		// imprimirConjunto(Sx);
		// imprimirConjunto(unico);

		if(unico.size() == 1) {
			C.push_back(Sx);
			F.erase(find(F.begin(), F.end(), Sx));
		}
	}

	return C;
}

/* --------------- Operaciones de conjuntos -------------- */
vector<int> unionConjuntos(vector<vector<int>> F) {
	vector<int> v = {};
	if(F.empty()) return v;
	int j;

	for(vector<int> S : F) { //m
		for(int numS : S) { //l <= n
			bool encontrado = false;

			j = 0;
			while(j<v.size() && !encontrado) {
				if(numS == v[j]) encontrado = true;
				j++;
			}

			if(!encontrado) v.push_back(numS);
		}
	}

	return v;
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