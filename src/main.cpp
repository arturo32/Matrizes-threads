#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>


std::ifstream abrirArquivo(std::string nome){
	std::ifstream arquivo(nome);		

	//Se não foi possível abrir o arquivo
	if(!arquivo){
		throw std::ios_base::failure("Erro ao abrir arquivo " + nome);
	}
	return arquivo;
}


void lerMatriz(int** matriz, int ordem, std::ifstream& arquivo){
	for(int i{0}; i < ordem; ++i){
		for(int j{0}; j < ordem; ++j){
			arquivo >> matriz[i][j];			
		}
	}
	arquivo.close();
}

void adicionarResultadoTempo(std::string ordem, double tempo){

	//Abrindo aquivo dos resultados de tempo
	std::string nomeArquivoResultados = "resultados/" + ordem + "x" + ordem + "_resultados" + ".csv";
	std::ofstream arquivoCsv(nomeArquivoResultados, std::ios_base::app);

	//Se não foi possível abrir o arquivo
	if(!arquivoCsv){
		std::cerr << "Erro ao abrir arquivo " + nomeArquivoResultados << std::endl;
		std::cout << std::fixed;

		//Imprimindo resultado no terminal
		std::cout << std::setprecision(6);
		std::cout << "Resultado: " << tempo << std::endl;
		throw std::ios_base::failure("Erro ao abrir arquivo " + nomeArquivoResultados);
	}

	//Adicionando resultado no fim do arquivo
	arquivoCsv << std::fixed;
	arquivoCsv << std::setprecision(6) << tempo << "; ";

	//Fechando arquivo de resultados de tempo
	arquivoCsv.close();
}

void criarArquivoMatrizResultante(int** matrizResultado, int ordem, std::string ordemStr){
	
	//Abrindo aquivo dos resultados de tempo
	std::string nomeArquivoResultados = "matrizes_resultantes/" + ordemStr + "x" + ordemStr + ".txt";
	std::ofstream arquivoCsv(nomeArquivoResultados);

	//Se não foi possível abrir o arquivo
	if(!arquivoCsv){
		std::cerr << "Erro ao abrir arquivo " + nomeArquivoResultados << std::endl;

		//Imprimindo resultado no terminal
		std::cout << "Resultado: " << std::endl;
		for(int i{0}; i < ordem; ++i){
			for(int j{0}; j < ordem; ++j){
				std::cout << matrizResultado[i][j] << " ";
			}
			std::cout << std::endl;
		}

		throw std::ios_base::failure("Erro ao abrir arquivo " + nomeArquivoResultados);
	}

	//Escrevendo matriz no arquivo
	for(int i{0}; i < ordem; ++i){
		for(int j{0}; j < ordem; ++j){
			arquivoCsv << matrizResultado[i][j] << " ";
		}
		arquivoCsv << std::endl;
	}
	
	arquivoCsv.close();
}

void multiplicaMatrizesSequencial(int** matrizA, int** matrizB, int** matrizResultado){}

void multiplicaMatrizesConcorrente(int** matrizA, int** matrizB, int** matrizResultado){}


int main(int argc, char const *argv[]){

	if(argc != 3){
		std::cout << "Número de parâmetros errados!" << std::endl;
		return 1;
	}

	std::string ordemStr = argv[1]; 
	int ordem = std::stoi(ordemStr);

	//Abrindo arquivos de matrizes
	std::ifstream arquivoMatrizA;
	std::ifstream arquivoMatrizB;
	try{
		arquivoMatrizA = abrirArquivo("Matrizes/A" + ordemStr + "x" + ordemStr + ".txt");
		arquivoMatrizB = abrirArquivo("Matrizes/B" + ordemStr + "x" + ordemStr + ".txt");	
	} catch(std::ios_base::failure& e){
		std::cerr << e.what() << std::endl;
		return 1;
	}
	

	//Criando array de matrizes alocadas dinâmicamente
	int*** matrizes = new int**[2];
	for(int i{0}; i < 2; ++i){
		matrizes[i] = new int*[ordem];
		for(int j{0}; j < ordem; ++j){
			matrizes[i][j] = new int[ordem];
		}
	}

	//Pula primeira linha
	std::string number;
	std::getline(arquivoMatrizA, number);	

	//Lendo matriz A
	lerMatriz(matrizes[0], ordem, arquivoMatrizA);

	//Lendo matriz B
	lerMatriz(matrizes[1], ordem, arquivoMatrizB);


	//Criando matriz de resultado alocada dinâmicamente
	int** matrizResultado = new int*[ordem];
	for(int i{0}; i < ordem; ++i){
		matrizResultado[i] = new int[ordem];
	}


	/*Criando ponteiro de função que pode tanto apontar para versão sequencial 
	como para a versão concorrente do algoritmo de multiplicação de matrizes*/
	void (*multiplicaMatrizes)(int**, int**, int**){nullptr};
	if(argv[3] == "S"){
		multiplicaMatrizes = &multiplicaMatrizesSequencial;
	}
	else{
		multiplicaMatrizes = &multiplicaMatrizesConcorrente;	
	}


	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//================================================================================


	multiplicaMatrizes(matrizes[0], matrizes[1], matrizResultado);


	//================================================================================
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();


	std::chrono::duration<double> diff = end - start;


	
	double tempo = std::chrono::duration<double, std::milli>(diff).count();

	//Acrescentando resultado de tempo em um arquivo csv
	try{
		adicionarResultadoTempo(ordemStr, tempo);	
	} catch(std::ios_base::failure& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	

	//Criando arquivo com matriz resultante
	/*try{
		criarArquivoMatrizResultante(matrizResultado, ordem, ordemStr);	
	} catch(std::ios_base::failure& e){
		std::cerr << e.what() << std::endl;
	}*/

	//Liberando espaço alocado dinâmicamente na memória
	for(int i{0}; i < 2; ++i){
		for(int j{0}; j < ordem; ++j){
			delete[] matrizes[i][j];
		}
		delete[] matrizes[i];
	}
	delete[] matrizes;

	for(int i{0}; i < ordem; ++i){
		delete[] matrizResultado[i];
	}
	delete[] matrizResultado;

	return 0;
}