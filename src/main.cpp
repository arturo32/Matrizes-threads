#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <thread>


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

void adicionarResultadoTempo(std::string ordem, double tempo, std::string tipoExecucao){

	//Abrindo aquivo dos resultados de tempo
	std::string nomeArquivoResultados;
	if(tipoExecucao == "S"){
		nomeArquivoResultados = "resultados/S_" + ordem + "x" + ordem + "_resultados" + ".csv";
	}else{
        nomeArquivoResultados = "resultados/C_" + ordem + "x" + ordem + "_resultados" + ".csv";
    }
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
	arquivoCsv << std::setprecision(6) << tempo << ";" << std::endl;

	//Fechando arquivo de resultados de tempo
	arquivoCsv.close();
}

void criarArquivoMatrizResultante(int** matrizResultado, int ordem, std::string ordemStr){
	
	//Abrindo aquivo dos resultados de tempo
	std::string nomeArquivoResultados = "matrizes_resultantes/C" + ordemStr + "x" + ordemStr + ".txt";
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

void multiplicaMatrizesSequencial(int** matrizA, int** matrizB, int** matrizResultado, int ordem){
	for(int i{0}; i < ordem; ++i){
		for(int j{0}; j < ordem; ++j){
			int soma = 0;
			for(int k{0}; k < ordem; ++k){
				soma += matrizA[i][k] * matrizB[k][j];
			}
			matrizResultado[i][j] = soma;
		}
	}
}

void calculaLinha(int* linhaMatrizA, int numLinha, int** matrizB, int** matrizResultado, int ordem){
	for(int i{0}; i < ordem; ++i){
		int soma = 0;
		for(int j{0}; j < ordem; ++j) {
			soma += linhaMatrizA[j] * matrizB[j][i];
		}
		matrizResultado[numLinha][i] = soma;
    }
}

void multiplicaMatrizesConcorrente(int** matrizA, int** matrizB, int** matrizResultado, int ordem){
	std::thread threads[ordem];
	for(int i{0}; i < ordem; ++i){
		//Instancia e inicia as threads para calcular as linhas da matriz produto
		threads[i] = std::thread(calculaLinha, matrizA[i], i, matrizB, matrizResultado, ordem);
	}

	//Espera as threads finalizarem
	for(int i{0}; i < ordem; ++i){
		threads[i].join();
	}
}


int main(int argc, char const *argv[]){

	if(argc != 3){
		std::cerr << "Número de parâmetros errados!" << std::endl;
		return 1;
	}
	
	//Define a ordem da matriz especificada pelo usuário
	std::string ordemStr = argv[1]; 
	int ordem;
	try {
		ordem = std::stoi(ordemStr);
	}
	catch(std::invalid_argument& e){
		std::cerr << "Primeiro parâmetro deve ser um inteiro!" << std::endl;
		std::cerr << e.what();
	}

	if(ordem < 4 || ordem > 2048){
		std::cerr << "Primeiro parâmetro deve ser uma potência de 2 entre 4 e 2048!" << std::endl;
		return 1;
	}
	int ordemCheck = ordem;
	while(ordemCheck != 1){
		if(ordemCheck % 2 == 1){
			std::cerr << "Primeiro parâmetro deve ser uma potência de 2!" << std::endl;
			return 1;
		}
		ordemCheck >>= 1;
	}

	std::string tipoExecucao = argv[2];
	if(tipoExecucao != "S" && tipoExecucao != "C"){
		std::cerr << "Segundo parâmetro incorreto!" << std::endl;
		return 1;
	}

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
	void (*multiplicaMatrizes)(int**, int**, int**, int){nullptr};
	if(tipoExecucao == "S"){
		multiplicaMatrizes = &multiplicaMatrizesSequencial;
	}
	else{
		multiplicaMatrizes = &multiplicaMatrizesConcorrente;	
	}


	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//================================================================================


	multiplicaMatrizes(matrizes[0], matrizes[1], matrizResultado, ordem);


	//================================================================================
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();


	std::chrono::duration<double> diff = end - start;


	
	double tempo = std::chrono::duration<double, std::milli>(diff).count();

	//Acrescentando resultado de tempo em um arquivo csv
	try{
		adicionarResultadoTempo(ordemStr, tempo, tipoExecucao);	
	} catch(std::ios_base::failure& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	

	//Criando arquivo com matriz resultante
	try{
		criarArquivoMatrizResultante(matrizResultado, ordem, ordemStr);	
	} catch(std::ios_base::failure& e){
		std::cerr << e.what() << std::endl;
	}

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