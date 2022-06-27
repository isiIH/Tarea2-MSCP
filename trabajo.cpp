#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
#include <random>
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
vector<int> union2Conjuntos(vector<int> A, vector<int> B);
vector<int> diferenciaConjuntos(vector<int> A, vector<int> B);
vector<int> interseccionConjuntos(vector<int> A, vector<int> B);

void imprimirConjunto(vector<int> v);

/*--------------------------------------------------------------------------------*/

int main(int argc, char **argv){
    
    if(argc != 3){
		cout << "Error. Debe ejecutarse como ./trabajo solucion archivo" << endl;
		exit(EXIT_FAILURE);
	}

	int exp;
	cout << "Experimentacion a realizar:" << endl;
	cout << "Normal uniforme aleatoria (0)" << endl;
	cout << "Datos reales (1)" << endl;
	cin >> exp;

	int solucion = stoi(argv[1]);
	clock_t start, end;
	vector<vector<int>> F;
	int k;

	if(exp == 0) {
		int m = 100;
		double media = 50.0;
		double desviacion = 10.0;
		double p = 0.05;
		int cElementoUnico = m*p;
		int limS = 10;
		default_random_engine generator;
		normal_distribution<double> distribution(media,desviacion);
		int limiteSubconjunto, num;
		vector<int> temp;

		for(int i=0; i<m; i++) {
			limiteSubconjunto = rand()%limS + 1;
			for(int j=0; j<limiteSubconjunto; j++) {
				num = distribution(generator);
				temp.push_back(num);
			}
			// imprimirConjunto(temp);
			F.push_back(temp);
			temp =  {};
		}

		cout << cElementoUnico << endl;
	}
	else {
		string file = argv[2];
		ifstream archivo;
		archivo.open("data/" + file, ios::in);
		string linea;
		vector<int> numeros;

		if(archivo.fail()){
			cout << "Error" << endl;
			exit(1);
		}
		while (getline(archivo, linea)) {
			stringstream text_stream(linea);
			string item;
			while (getline(text_stream, item, ' ')) {
				if(!item.empty() && find(numeros.begin(), numeros.end(), stoi(item)) == numeros.end()) numeros.push_back(stoi(item));
			}
			// cout << numeros.size() << endl;
			// imprimirConjunto(numeros);
			F.push_back(numeros);
			numeros.clear();
		}
		archivo.close();
	}

	//Universo
	vector<int> X = unionConjuntos(F);
	sort(X.begin(), X.end());
	cout << "Tamaño del universo (n): " << X.size() << endl;
	cout << "Cantidad de subconjuntos (m): " << F.size() << endl;

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

	// for(vector<int> S : C) {
	// 	imprimirConjunto(S);
	// }
	// if(unionConjuntos(C).size() == X.size()) cout << "\nC es un SC" << endl;
	cout << "Número de subconjuntos en C: " << C.size() << endl;


	
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
		if(interseccionConjuntos(unionConjuntos(cUtilizados),X).size() == n && c < csol) {
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
	vector<vector<int>> C = conjuntosConElementoUnico(F);

	//Nuevo universo
	X = diferenciaConjuntos(X,unionConjuntos(C));

	if(!X.empty()) {
		int c = 0;
		int n = X.size();
		int csol = F.size() + 99;
		vector<int> Faceptados;
		//Inicializar en 0
		for(int i=0; i<F.size(); i++) {
			Faceptados.push_back(0);
		}
		vector<vector<int>> sol, cUtilizados;

		while(c<F.size()) {
			Faceptados = sumarUno(Faceptados);
			c = contarUnos(Faceptados);
			if(c < csol) {
				cUtilizados = conjuntosUtilizados(Faceptados, F);
				if(interseccionConjuntos(unionConjuntos(cUtilizados),X).size() == n) {
					sol = cUtilizados;
					csol = c;
				}
			}
		}

		for(vector<int> s : sol) {
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
	int interseccionMax, interseccion;

	while(!U.empty()) {
		maxS = {};
		interseccionMax = 0;
		for(vector<int> S : F) {
			interseccion = interseccionConjuntos(U,S).size();

			if(interseccion > interseccionMax) {
				interseccionMax = interseccion;
				maxS = S;
			}
		}

		U = diferenciaConjuntos(U,maxS);
		C.push_back(maxS);
	}

	return C;

}

/* --------------- Solución 4 -------------- */
vector<vector<int>> greedyOptimizado(vector<int> X, vector<vector<int>> F, int k) {
	vector<vector<int>> C = conjuntosConElementoUnico(F);

	//Nuevo universo
	X = diferenciaConjuntos(X,unionConjuntos(C));

	// Greedy con k-conjuntos
	sort(F.begin(), F.end(), [] (vector<int> &x, vector<int> &y) { return x.size() > y.size(); });
	// for(vector<int> S : F) {
	// 	imprimirConjunto(S);
	// }

	if(!X.empty()) {
		vector<int> U = X;
		vector<vector<int>> maxS(k);
		int interseccionMax;
		int interseccion;
		int i;

		vector<int> cUnion;

		while(!U.empty()) {
			cUnion = {};
			interseccionMax = 0;

			for(int c=0; c<k; c++){
				i = 0;
				while(i<F.size() && union2Conjuntos(cUnion,F[i]).size() >= interseccionMax) {
					interseccion = interseccionConjuntos(U,union2Conjuntos(cUnion,F[i])).size();

					if(interseccion > interseccionMax) {
						interseccionMax = interseccion;
						maxS[c] = F[i];
					}

					i++;
				}

				cUnion = union2Conjuntos(cUnion,maxS[c]);
			}

			U = diferenciaConjuntos(U,cUnion);

			for(int c=0; c<k; c++) {
				C.push_back(maxS[c]);
				maxS[c] = {};
			}
			if(U.size() < k) {
				k = U.size();
				maxS.resize(k);
			}
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

vector<int> union2Conjuntos(vector<int> A, vector<int> B) {
	vector<int> v = A;
	int j;

	for(int b : B) {
		bool encontrado = false;

		j = 0;
		while(j<v.size() && !encontrado) {
			if(b == v[j]) encontrado = true;
			j++;
		}

		if(!encontrado) v.push_back(b);
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