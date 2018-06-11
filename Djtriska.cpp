#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <stack>

using namespace std;

class Vertice;

class Arco {
private:
	class Vertice *U;
    class Vertice *V;

public:
	Arco(Vertice *u, Vertice *v) {
		U = u;
		V = v;
    }

    Arco(Arco *a) {
		U = a->getInicio();
		V = a->getFinal();
    }

	Vertice * getInicio() {
		return U;
	}

	Vertice * getFinal() {
		return V;
	}
};

class Vertice {
private:
	int Indice;
	int GrauDeEntrada;
	int GrauDeSaida;
	list<Arco*> *ListaDeAdjacencia;
	vector<double> *Custo;
	vector<bool> *NoDownset;

public:
    Vertice(int i, int totalDeNos){
		Indice = i;
		GrauDeEntrada = 0;
		GrauDeSaida = 0;
		ListaDeAdjacencia = new list<Arco*>;
		Custo = new vector<double>(totalDeNos);
		NoDownset = new vector<bool>(totalDeNos);
		for (int i=0; i<totalDeNos; i++) {
			Custo->at(i) = DBL_MAX;
			NoDownset->at(i) = false;
		}
	}

	void adicionarCusto(Vertice* vertice, double custo){
		Custo->at(vertice->getIndice()) = custo;
		NoDownset->at(vertice->getIndice()) = true;
	}

	inline void adicionarRelacao(Vertice* vertice){
		NoDownset->at(vertice->getIndice()) = true;
		Custo->at(vertice->getIndice()) = 0.0;
	}

	inline void adicionarRelacao(int v){
		NoDownset->at(v) = true;
		Custo->at(v) = 0.0;
	}

	bool obterCustoParaVertice(int v, double &custo) {
		if (NoDownset->at(v) == false)
			return false;
		else {
			custo = Custo->at(v);
			return true;
		}
	}

	inline bool estaNoDownset(int v) {
		return NoDownset->at(v);
	}

	void adicionarArcoDeSaida(Arco *a) {
         Arco *A = new Arco(a);
         ListaDeAdjacencia->push_back(A);
		 adicionarRelacao(a->getFinal());
	}

	void adicionarArcoDeSaida(Arco *a, double custo) {
         Arco *A = new Arco(a);
		 adicionarCusto(a->getFinal(),custo);
	}

	void removerArcoDeSaida(Arco *a) {
		ListaDeAdjacencia->remove(a);
	}

	int  getGrauDeEntrada() {
		return GrauDeEntrada;
	}

	void incrementarGrauDeEntrada() {
		GrauDeEntrada ++;
	}

	void decrementarGrauDeEntrada() {
		GrauDeEntrada --;
	}

	int  getGrauDeSaida() {
		return GrauDeSaida;
	}

	void incrementarGrauDeSaida() {
		GrauDeSaida ++;
	}

	void decrementarGrauDeSaida() {
		GrauDeSaida --;
	}

	int  getIndice() {
		return Indice;
	}

	list<Arco*>* getListaDeAdjacencia() {
		return ListaDeAdjacencia;
	}

};

class GrafoDirecionado {
private:
	vector<Vertice*> *Vertices;
	list<Arco*> *Arcos;

public:
	GrafoDirecionado(int numVertices){
		Vertices = new vector<Vertice*>(numVertices);
		for (int i = 0; i < numVertices; i++) {
			Vertices->at(i) = new Vertice(i, numVertices);
		}
		Arcos = new list<Arco*>();
	}

	int getNumeroDeVertices() {
		return Vertices->size();
	}

	int getNumeroDeArcos() {
		return Arcos->size();
	}

	Vertice* getVertice(int i) {
		if (i < getNumeroDeVertices())
			return Vertices->at(i);
		return NULL;
	}

	Arco* adicionarArco(int origem, int destino,double custo) {
		int tamanho = getNumeroDeVertices();
		if ((origem < tamanho) && (destino < tamanho)) {
			Vertice *verticeOrigem = getVertice(origem);
			Vertice *verticeDestino = getVertice(destino);
			if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
				for(list<Arco*>::iterator it = verticeDestino->getListaDeAdjacencia()->begin(); it != verticeDestino->getListaDeAdjacencia()->end();it++)
					if((*it)->getInicio() == verticeOrigem)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarArcoDeSaida(arco,custo);
				verticeOrigem->incrementarGrauDeSaida();
				verticeDestino->incrementarGrauDeEntrada();
				Arcos->push_back(arco);
				return arco;
			}
		}
		return NULL;
	}

	void removerArco(Arco *arco) {
		Vertice *fim = arco->getFinal();
		fim->removerArcoDeSaida(arco);
		Vertice *inicio = arco->getInicio();
		inicio->decrementarGrauDeEntrada();
		fim->decrementarGrauDeSaida();
		Arcos->remove(arco);
	}

	list<Arco*>* getArcos(){
		return Arcos;
	}

	vector<Vertice*>* getVertices(){
		return Vertices;
	}

};

void troca(double &a, double &b) {
	double t = a;
	a = b;
	b = t;
}

void trocaInt(int &a, int &b) {
	int t = a;
	a = b;
	b = t;
}

class HeapBin{

private:

	int tamanhoAtual;
	int tamanhoMax;
	double *dados;

public:

	HeapBin(int tamanho){
		tamanhoAtual = 0;
		tamanhoMax = tamanho;
		dados = new double[tamanho];
	}

	void moverParaCima(int i,vector<int> &PosicaoNaHeap){
		if((i-1)/2 < 0)
			return;
		if(dados[i] < dados[(i-1)/2]){
			troca(dados[i],dados[(i-1)/2]);
			trocaInt(PosicaoNaHeap[i],PosicaoNaHeap[(i-1)/2]);
			moverParaCima((i-1)/2,PosicaoNaHeap);
		}
	}

	void inserir(double valor,vector<int> &PosicaoNaHeap, int indice){
		if(tamanhoAtual < tamanhoMax){
			PosicaoNaHeap[indice] = tamanhoAtual;
			dados[tamanhoAtual] = valor;
			tamanhoAtual++;
			moverParaCima(tamanhoAtual-1,PosicaoNaHeap);
		}
	}
	
	void moverParaBaixo(int i,vector<int> &PosicaoNaHeap){
		int se = 2*i+1;
		int sd = 2*i+2;
		if (sd >= tamanhoAtual) {
			if (se >= tamanhoAtual)
				return;
			if (dados[se] < dados[i]){
				troca(dados[i],dados[se]);
				trocaInt(PosicaoNaHeap[i],PosicaoNaHeap[se]);
			}
		}
		else { 
			int menorSucessor = se;
			if (dados[sd] < dados[se])
				menorSucessor = sd;
			if (dados[menorSucessor] < dados[i]) {
				troca(dados[i],dados[menorSucessor]);
				trocaInt(PosicaoNaHeap[i],PosicaoNaHeap[menorSucessor]);
				moverParaBaixo(menorSucessor,PosicaoNaHeap);
			}
		}
	}
	
	double selecionarHeap(){
		return dados[0];
	}

	double VerPrioridade(int i){
		return dados[i];
	}

	void removerMinimo(vector<int> &PosicaoNaHeap){
		if(tamanhoAtual > 0){
			dados[0] = dados[tamanhoAtual];
			trocaInt(PosicaoNaHeap[0],PosicaoNaHeap[tamanhoAtual]);
			tamanhoAtual--;
			PosicaoNaHeap.resize(tamanhoAtual);
			moverParaBaixo(0,PosicaoNaHeap);
		}
	}

	void imprimir(){
		for(int i = 0;i < tamanhoAtual;i++)
			cout << "Indice: " << i << " : " << dados[i] << endl;
		system("pause");
		system("cls"); 
	}

	void alterarPrioridade(int i,double chave,vector<int> &PosicaoNaHeap){
		double x = dados[i];
		dados[i] = chave;
		if(chave < x)
			moverParaCima(i,PosicaoNaHeap);
		else
			moverParaBaixo(i,PosicaoNaHeap);
	}
};

void DijstrkaComLista(GrafoDirecionado d,int indice){

	list<int> W;
	W.push_back(indice);
	list<int> V;
	
	for(int i = 0; i < d.getNumeroDeVertices(); i++)
		V.push_back(i);

	V.remove(indice);

	vector<double> Predecessor;
	Predecessor.resize(d.getNumeroDeVertices());
	Predecessor[indice] = indice;

	vector<double> custo;
	custo.resize(d.getNumeroDeVertices());
	for(int i = 0; i < d.getNumeroDeVertices() ; i++)
		d.getVertice(indice)->obterCustoParaVertice(i,custo[i]);
	
	custo[indice] = 0;
	
	while(W.size() < d.getNumeroDeVertices()){
		double min = 999999;
		int indiceAux;

		for(list<int>::iterator it = V.begin() ; it != V.end() ; it++)
			if(custo[(*it)] < min){
				min = custo[(*it)];
				indiceAux = (*it);
			}

		V.remove(indiceAux);
		W.push_back(indiceAux);
		double auxiliar;
		for(int i = 0; i < d.getNumeroDeVertices() ; i++){
			if(d.getVertice(indiceAux)->obterCustoParaVertice(i,auxiliar))
				if(auxiliar < custo[i]){
					custo[i] = auxiliar;
					Predecessor[i] = indiceAux;
				}
		}
	};

	for(int i = 0; i < d.getNumeroDeVertices(); i++)
		cout << "Predecessor de " << i << " eh " << Predecessor[i] << endl;
	cout << endl;
}

void DijstrkaComHeap(GrafoDirecionado d,int indice){

	list<int> W;
	list<int> V;

	for(int i = 0; i < d.getNumeroDeVertices(); i++)
		V.push_back(i);
	V.remove(indice);
	W.push_back(indice);

	vector<double> Predecessor;
	Predecessor.resize(d.getNumeroDeVertices());
	Predecessor[indice] = indice;

	//Heap
	vector<int> PosicaoNaHeap;
	PosicaoNaHeap.resize(d.getNumeroDeVertices());
	HeapBin HeapDeCusto(d.getNumeroDeVertices());
	double custo;

	/*for(list<int>::iterator it = V.begin(); it != V.end(); it++)
		if(d.getVertice(indice)->obterCustoParaVertice(*it,custo))
			HeapDeCusto.inserir(custo,PosicaoNaHeap, *it);
		else
			HeapDeCusto.inserir(999999,PosicaoNaHeap, *it);
			*/
	HeapDeCusto.inserir(2,PosicaoNaHeap, 1);
	HeapDeCusto.inserir(1,PosicaoNaHeap, 2);
	cout << PosicaoNaHeap[1] << endl;
	cout << PosicaoNaHeap[2] << endl;
	//APARTI DAKI

	while(W.size() < d.getNumeroDeVertices()){

		for(list<int>::iterator it = W.begin(); it != W.end(); it++)
			cout << *it << " ";
		cout << endl;
		for(list<int>::iterator it = V.begin(); it != V.end(); it++)
			cout << *it << " " << PosicaoNaHeap[*it] << " " << HeapDeCusto.VerPrioridade(PosicaoNaHeap[*it]) << endl;
		cout << endl;

		system("pause");
		system("cls");

		int indiceAux;
		for(list<int>::iterator it = V.begin(); it != V.end(); it++)
			if (PosicaoNaHeap[*it] == 0){
				indiceAux = *it;
				break;
			}
			
		HeapDeCusto.removerMinimo(PosicaoNaHeap);

		cout << indiceAux << endl;
		V.remove(indiceAux);
		W.push_back(indiceAux);
		double auxiliar;
		system("pause");
		system("cls");

		for(list<int>::iterator it = V.begin(); it != V.end(); it++){
			cout << *it << endl;
			if(d.getVertice(indiceAux)->obterCustoParaVertice((*it),auxiliar)){
				int a = PosicaoNaHeap[(*it)];
				if(auxiliar < HeapDeCusto.VerPrioridade(a)){
					HeapDeCusto.alterarPrioridade(a,auxiliar,PosicaoNaHeap);
					Predecessor[(*it)] = indiceAux;
				}
			}
		}
		system("pause");
	};

	for(int i = 0; i < d.getNumeroDeVertices(); i++)
		cout << "Predecessor de " << i << " eh " << Predecessor[i] << endl;
	cout << endl;

}

int main() {

	GrafoDirecionado d(6);
	d.adicionarArco(0,1,2);
	d.adicionarArco(0,2,1);
	d.adicionarArco(1,2,3);
	d.adicionarArco(1,3,3);
	d.adicionarArco(2,4,1);
	d.adicionarArco(3,5,2);
	d.adicionarArco(4,3,2);
	d.adicionarArco(4,5,5);
	DijstrkaComHeap(d,0);
	system("Pause");
}
