// Victorgonl - Caderno de Receitas - 20210321

// Universidade Federal de Lavras, 21 de março de 2021.
// GCC224 - Introdução aos Algoritmos.
// 2020/2 - Trabalho Prático.

// =====Caderno de Receitas===== //

#include <iostream>
#include <fstream>

// constantes para limitar os dados processados.
const int LIM_MARGEM = 50;
const int MAX_INGREDIENTES = 10;
const int MAX_NOME = 50;
const int MAX_TEXTO = 750;

// *structs tem seus tipos ordenados do maior para menor para evitar bytes a mais para alinhamento.

// structs onde as receitas serão manipuladas.
struct ingrediente{
	float quantidade;
	char nome[MAX_NOME];
	char unidade_medida[MAX_NOME];
};
struct receita{
	ingrediente ingredientes[MAX_INGREDIENTES];
	float rendimento;
	char modo_preparo[MAX_TEXTO];
	char nome[MAX_NOME];
};

// constante indicando a posição de um dado na struct.
const int LOCAL_NOME = sizeof(receita) - MAX_NOME;

// struct para receber dados temporários para eficiência das operações
struct mem{
	char nome[MAX_NOME];
};

// subprogramas para operações com arrays do tipo char nas entradas e saídas.
void lerNome(char c[MAX_NOME]){
	int temp;
	int i = 0;
	do{
		temp = getchar();
		if(temp != '\n'){
			c[i] = char(temp);
		}
		else{
			while(i < MAX_NOME){
				c[i] = '\0';
				i++;
			}
		}
		i++;
	}while(i < MAX_NOME);
}
void imprimirNome(char c[MAX_NOME]){
	int i = 0;
	while((i < MAX_NOME) and (c[i] != '\0')){
		std::cout << c[i];
		i++;
	}
}
bool nomesIguais(char a[MAX_NOME], char b[MAX_NOME]){
	for(int i = 0; i < MAX_NOME; i++){
		if(a[i] != b[i]){
			return false;
		}
	}
	return true;
}
void lerTexto(char c[MAX_TEXTO]){
	int temp;
	int i = 0;
	do{
		temp = getchar();
		if(temp != '\n'){
			c[i] = char(temp);
		}
		else{
			while(i < MAX_TEXTO){
				c[i] = '\0';
				i++;
			}
		}
		i++;
	}while(i < MAX_TEXTO);
}
void imprimirTexto(char c[MAX_TEXTO]){
	int j = 0;
	for(int i = 0; (c[i] != '\0') and (i < MAX_TEXTO); i++){
		std::cout << c[i];
		if((c[i] == ' ') and (j >= LIM_MARGEM - 1)){
			std::cout << std::endl;
			j = 0;
		}
		j++;
	}
}

// **em alguns subprogramas abaixo, foram explicitamente convertidos valores do tipo unsigned ou long para int.

// retorna o tamanho dos arquivos para simplificar em outros subprogramas.
int tamanhoArquivo(int escolha_arquivo){
	std::ifstream arquivo;
	switch(escolha_arquivo){
		case 1:{
			arquivo.open("Caderno de Receitas.bin");
			break;
		}
		case 2:{
			arquivo.open("Caderno de Receitas.bak");
			break;
		}
	}
	int total = 0;
	arquivo.seekg(0, std::ios::end);
	total = int(arquivo.tellg()); // **conversão para int.
	arquivo.close();
	return total;
}

// para zerar o arquivo e simplificar em outros subprogramas.
bool zerarArquivo(int escolha_arquivo){
	std::ofstream arquivo;
	switch(escolha_arquivo){
		case 1:{
			arquivo.open("Caderno de Receitas.bin", std::ios::trunc);
			break;
		}
		case 2:{
			arquivo.open("Caderno de Receitas.bak", std::ios::trunc);
			break;
		}
	}
	if(arquivo.is_open()){
		arquivo.close();
		return true;
	}
	return false;
}

// calcula e rotorna o número de receitas gravadas ou garante que o arquivo principal será criado.
int totalReceitas(int escolha_arquivo){
	std::ifstream arquivo;
	switch(escolha_arquivo){
		case 1:{
			arquivo.open("Caderno de Receitas.bin");
			break;
		}
		case 2:{
			arquivo.open("Caderno de Receitas.bak");
			break;
		}
	}
	if(arquivo.is_open()){
		arquivo.close();
		if(tamanhoArquivo(escolha_arquivo) > 0){
			return tamanhoArquivo(escolha_arquivo) / int(sizeof(receita)); // **conversão para int.
		}
	}
	else{
		if(!zerarArquivo(escolha_arquivo)){
			return -1;
		}
	}
	return 0;
}

// ***em alguns comandos de entrada em testes no Windows, alguns bytes a mais eram salvos no arquivo.
// assim, foi usada a flag ios::binary para evitar a corrupção da sequencia de dados.

// para salvar a receita da struct na posição especificada no arquivo.
bool salvarReceita(receita& x, int i, int escolha_arquivo){
	std::fstream arquivo;
	switch(escolha_arquivo){
		case 1:{
			arquivo.open("Caderno de Receitas.bin", std::ios::in | std::ios::out | std::ios::binary);
			break;
		}
		case 2:{
			arquivo.open("Caderno de Receitas.bak", std::ios::in | std::ios::out | std::ios::binary);
			break;
		}
	}
	if(arquivo.is_open()){
		arquivo.seekp(i * int(sizeof(receita)), std::ios_base::beg); // **conversão para int.
		arquivo.write((char*)(&x), int(sizeof(receita))); // **conversão para int.
	}
	else{
		return false;
	}
	arquivo.close();
	return true;
}

// carrega a receita especificada para struct.
bool carregarReceita(receita& x, int i, int escolha_arquivo){
	if(i < 0 or i >= totalReceitas(escolha_arquivo)){
		return false;
	}
	std::ifstream arquivo;
	switch(escolha_arquivo){
		case 1:{
			arquivo.open("Caderno de Receitas.bin", std::ios::binary);
			break;
		}
		case 2:{
			arquivo.open("Caderno de Receitas.bak", std::ios::binary);
			break;
		}
	}
	arquivo.seekg(i * int(sizeof(receita)), std::ios::beg); // **conversão para int.
	arquivo.read((char*)(&x), sizeof(receita));
	arquivo.close();
	return true;
}

// cria uma cópia do arquivo principal e auxilia em alguns subprogramas.
void backupArquivo(){
	zerarArquivo(2);
	int n = totalReceitas(1);
	for(int i = 0; i < n; i++){
		receita temp;
		carregarReceita(temp, i, 1);
		salvarReceita(temp, i, 2);
	}
}

// passa do arquivo para a memoria principal o nome das receitas gravadas para maior eficiência de busca.
bool atribuirID(mem id[]){
	std::ifstream arquivo("Caderno de Receitas.bin");
	if(arquivo.is_open()){
		int n = totalReceitas(1);
		for(int i = 0; i < n; i++){
			arquivo.seekg(i * int(sizeof(receita)) + LOCAL_NOME, std::ios::beg); // **conversão para int.
			arquivo.read(id[i].nome, MAX_NOME);
		}
	}
	else{
		return false;
	}
	arquivo.close();
	return true;
}

// para buscar com eficiência as receitas já organizadas na memória principal.
int binarysearchNome(mem id[], char busca[], int i, int j){
	int meio = ((i + j) / 2);
	int l = 0;
	while((id[meio].nome[l] == busca[l]) and (l < (MAX_NOME - 1))){
		l++;
	}
	if((busca[l] > id[meio].nome[l]) and (i < j)){
		return binarysearchNome(id, busca, (meio + 1), j);
	}
	else if((busca[l] < id[meio].nome[l]) and (i < j)){
		return binarysearchNome(id, busca, i, (meio - 1));
	}
	else if(busca[l] == id[meio].nome[l]){
		return meio;
	}
	else{
		return -1;
	}
}

// para exportar uma receita especificada em um arquivo de texto (.txt).
bool exportarReceita(receita x){
	using std::endl;
	char nome_txt[MAX_NOME + 4];
	for(int i = 0; i < (MAX_NOME + 4); i++){
		nome_txt[i] = x.nome[i];
	}
	for(int i = 0; i < (MAX_NOME + 4); i++){
		if((nome_txt[i] == 0) or (i == MAX_NOME)){
			nome_txt[i] = '.';
			nome_txt[i + 1] = 't';
			nome_txt[i + 2] = 'x';
			nome_txt[i + 3] = 't';
			i = MAX_NOME + 4;
		}
	}
	std::ofstream arquivo(nome_txt);
	if(arquivo.is_open()){
		arquivo << '\t' << "=====" << x.nome << "=====";
		arquivo << endl << endl;
		if(int(x.rendimento) <= 1){
			arquivo << "Rendimento: " << x.rendimento;
			arquivo << " porcao." << endl << endl;
		}
		else{
			arquivo << "Rendimento: " << x.rendimento;
			arquivo << " porcoes." << endl << endl;
		}
		arquivo << '\t' << "-----Ingredientes-----";
		arquivo << endl << endl;
		int i = 0;
		while((i < MAX_INGREDIENTES) and (x.ingredientes[i].nome[0] != '\0')){
		arquivo << (i + 1) << ". ";
		arquivo << x.ingredientes[i].quantidade << ' ';
		arquivo << x.ingredientes[i].unidade_medida << " de ";
		arquivo << x.ingredientes[i].nome << endl;
		i++;
		}
		arquivo << endl << '\t' << "-----Modo de Preparo-----";
		arquivo << endl << endl;
		int k = 0;
		for(int j = 0; (x.modo_preparo[j] != '\0') and (j < MAX_TEXTO); j++){
			arquivo << x.modo_preparo[j];
			if((x.modo_preparo[j] == ' ') and (k >= MAX_NOME - 1)){
				arquivo << endl;
				k = 0;
			}
			k++;
		}
	}
	else{
		return false;
	}
	arquivo.close();
	return true;
}

// para apagar a receita especificada no arquivo principal.
void apagarReceita(int k){
	backupArquivo();
	zerarArquivo(1);
	for(int i = 0; i < k; i++){
		receita temp;
		carregarReceita(temp, i, 2);
		salvarReceita(temp, i, 1);
	}
	int n = totalReceitas(2);
	for(int i = k + 1; i < n; i++){
		receita temp;
		carregarReceita(temp, i, 2);
		salvarReceita(temp, i - 1, 1);
	}
}

// para ordenar as receitas foi utilizado o InsertionSort por sua melhor eficiência em listas quase organizadas.
void insertionsortReceitas(){
	int n = totalReceitas(1);
	for(int i = 1; i < n; i++){
		int j = i - 1;
		receita x, y;
		carregarReceita(x, i, 1);
		carregarReceita(y, j, 1);
		int l = 0;
		while((x.nome[l] == y.nome[l]) and (l < (MAX_NOME - 1))){
			l++;
		}
		while((x.nome[l] < y.nome[l]) and (j >= 0)){
			carregarReceita(y, j, 1);
			salvarReceita(y, j + 1, 1);
			j--;
			if(j >= 0){
				carregarReceita(y, j, 1);
				l = 0;
				while((x.nome[l] == y.nome[l]) and (l < (MAX_NOME - 1))){
					l++;
				}
			}
		}
		salvarReceita(x, j + 1, 1);
	}
}

// compara o nome da receita sendo editada com as da memória principal evitando nomes repetidos.
bool nomeRepetido(mem id[], char busca[]){
	int n = totalReceitas(1);
	for(int i = 0; i < n; i++){
		if(nomesIguais(busca, id[i].nome)){
				return true;
			}
	}
	return false;
}

// subprograma que interage com usuario para criar e editar receitas.
bool editarReceita(receita& x, int comando, mem id[], bool editando){
	using std::cout;
	using std::cin;
	using std::endl;
	switch(comando){
		case 1:{
			cout << "Nome da receita: ";
			lerNome(x.nome);
			while(nomeRepetido(id, x.nome)){
				cout << endl << "Este nome de receita ja existe. Escolha outro." << endl;
				cout << endl;
				cout << "Nome da receita: ";
				lerNome(x.nome);
			}
			if(editando){
				break;
			}
			else{
				cout << endl;
			}
		}
		case 2:{
			cout << "Rendimento da receita: ";
			cin >> x.rendimento;
			cin.ignore();
			if(editando){
				break;
			}
			else{
				cout << endl;
			}
		}
		case 3:{
			int i = 0;
			cout << "Nome do ingrediente: ";
			lerNome(x.ingredientes[i].nome);
			while(x.ingredientes[i].nome[0] != '\0' and i < MAX_INGREDIENTES){
				cout << "Unidade de medida: ";
				lerNome(x.ingredientes[i].unidade_medida);
				cout << "Quantidade: ";
				cin >> x.ingredientes[i].quantidade;
				cin.ignore();
				i++;
				if(i < MAX_INGREDIENTES){
					cout << "Nome do Ingrediente: ";
					lerNome(x.ingredientes[i].nome);
				}
			}
			if(editando){
				break;
			}
			else{
				cout << endl;
			}
		}
		case 4:{
			cout << "Modo de preparo: ";
			lerTexto(x.modo_preparo);
				break;
		}
		default:{
			return false;
		}
	}
	return true;
}

// subprograma de saída que mostra a lista de receitas carregadas na memória principal
bool listaReceitas(mem id[]){
	using std::cout;
	using std::endl;
	int n = totalReceitas(1);
	if(n == 0){
		return false;
	}
	cout << "Receitas salvas: " << n << '.' << endl << endl;
	for(int i = 0; i < n; i++){
		cout << i + 1 << ". ";
		imprimirNome(id[i].nome);
		if(!(i == (n - 1))){
			cout << ';' << endl;
		}
		else{
			cout << '.' << endl;
		}
		
	}
	return true;
}

// subprograma de saída para ver a receita especificada.
void verReceita(receita& x){
	using std::cout;
	using std::endl;
	cout << '\t' << "=====";
	imprimirNome(x.nome);
	cout << "=====" << endl << endl;
	if(int(x.rendimento) <= 1){
		cout << "Rendimento: " << x.rendimento;
		cout << " porcao." << endl << endl;
	}
	else{
		cout << "Rendimento: " << x.rendimento;
		cout << " porcoes." << endl << endl;
	}
	cout << '\t' << "-----Ingredientes-----";
	cout << endl << endl;
	int i = 0;
	while((i < MAX_INGREDIENTES) and (x.ingredientes[i].nome[0] != '\0')){
		cout << (i + 1) << ". ";
		cout << x.ingredientes[i].quantidade << ' ';
		imprimirNome(x.ingredientes[i].unidade_medida);
		cout << " de ";
		imprimirNome(x.ingredientes[i].nome);
		cout << endl;
		i++;
	}
	cout << endl << '\t';
	cout << "-----Modo de Preparo-----";
	cout << endl << endl;
	imprimirTexto(x.modo_preparo);
	cout << endl;
}

// subprograma para as saidas de interface que interagem com o usuário.
void menu(int submenu){
	using std::cout;
	using std::endl;
	switch(submenu){
		case -4:{
			cout << "================CADERNO DE RECEITAS===============";
			cout << endl << endl;
			cout << "Um trabalho de victor.lima4 e leonardo.rodrigues4.";
			cout << endl;
			break;
		}
		case -3:{
			cout << endl;
			cout << "==================MENU PRINCIPAL==================" << endl;
			cout << endl << "Digite o numero da escolha." << endl;
			cout << endl;
			cout << "0. Informacoes sobre o programa." << endl;
			cout << "1. Adicionar receita." << endl;
			cout << "2. Ver receita." << endl;
			cout << "3. Editar receita." << endl;
			cout << "4. Apagar receita." << endl;
			cout << "5. Exportar receita." << endl;
			cout << "6. Apagar todas as receitas." << endl;
			cout << "7. Realizar backup das receitas." << endl;
			cout << "X. Outro valor para fechar o programa." << endl;
			menu(-2);
			break;
		}
		case -2:{
			menu(-1);
			cout << "Escolha: ";
			break;
		}
		case -1:{
			cout << endl;
			cout << "==================================================";
			cout << endl << endl;
			break;
		}
		case 0:{
			menu(-1);
			cout << "Programa elaborado por Victor Goncalves Lima e" << endl;
			cout << "Leonardo Elias Rodrigues, alunos da" << endl;
			cout << "Universidade Federal de Lavras na disciplina" << endl;
			cout << "Introducao aos Algoritmos." << endl << endl;
			cout << "O programa permite armazenar, procurar, editar e" << endl;
			cout << "apagar receitas. Ha certas limitacoes nessas" << endl;
			cout << "funcoes de acordo com os recursos usados, assim" << endl;
			cout << "e importante seguir os menus. O programa tambem" << endl;
			cout << "permite exportar receitas para arquivo de texto e" << endl;
			cout << "realiza backup dos dados sempre que requerido ou" << endl;
			cout << "quando o programa for encerrado corretamente." << endl;
			break;
		}
		case 1:{
			menu(-1);
			cout << "1. Ver lista completa de receitas." << endl;
			cout << "2. Procurar por nome." << endl;
			menu(-2);
			break;
		}
		case 2:{
			menu(-1);
			cout << "Digite cada campo da receita e pressione ENTER." << endl;
			cout << "Nao devem conter caracteres especiais como" << endl;
			cout << "acentos ou cedilha." << endl;
			cout << "A quantidade do rendimento e ingrediente sao" << endl;
			cout << "apenas numeros reais sem letras." << endl;
			cout << "A separacao de casa decimal e feita por ponto." << endl;
			cout << "Deixe o nome do ingrediente vazio se suficinte." << endl;
			cout << "Formatacao dos ingredientes: quantidade +" << endl;
			cout << "unidade de medida + \"de\" + ingrediente." << endl;
			cout << "Maximo de ingredientes a ser adicionados: " << MAX_INGREDIENTES << '.' << endl;
			cout << "Maximo de caracteres permitidos para nomes: " << MAX_NOME << '.' << endl;
			cout << "Maximo de caracteres permitidos para texto: " << MAX_TEXTO << '.' << endl;
			menu(-1);
			break;
		}
		case 3:{
			menu(-1);
			cout << "Digite o campo que deseja editar." << endl << endl;
			cout << "1. Nome." << endl;
			cout << "2. Rendimento." << endl;
			cout << "3. Ingredientes." << endl;
			cout << "4. Modo de Preparo." << endl;
			menu(-2);
			break;
		}
		case 4:{
			cout << "Receita deletada!" << endl;
			break;
		}
		case 5:{
			cout << "Receita exportada!" << endl;
			break;
		}
		case 6:{
			menu(-1);
			cout << "Tem certeza?" << endl << endl;
			cout << "1. Sim." << endl;
			cout << "X. Outro valor para cancelar." << endl;
			menu(-2);
			break;
		}
		case 7:{
			menu(-1);
			cout << "Backup realizado!" << endl;
			break;
		}
		case 8:{
			menu(-1);
			cout << "Receitas deletadas!" << endl;
			break;
		}
		case 9:{
			cout << "Escolha invalida!" << endl;
			break;
		}
		case 10:{
			menu(-1);
			cout << "Programa encerrado!" << endl;
			menu(-1);
			break;
		}
		case 11:{
			menu(-1);
			cout << "Receita adicionada!" << endl;
			break;
		}
		case 12:{
			menu(-1);
			cout << "Receita editada!" << endl;
			break;
		}
		case 13:{
			cout << "Nenhuma receita salva!" << endl;
			break;
		}
		case 14:{
			menu(-1);
			cout << "Erro com o arquivo!" << endl;
			break;
		}
		case 15:{
			cout << "Receita invalida ou nao existente!" << endl;
			break;
		}
		case 16:{
			menu(-1);
			cout << "Digite o nome da receita: ";
			break;
		}
		default:{
			break;
		}
	}
}

// subprograma onde os comandos são lidos, direcionados e chamados para execução.
int main(){
	using std::cin;
	menu(-4);
	bool continuar = true;
	while(continuar){
		int escolha = -1, subescolha = -1;
		mem* id = new mem[totalReceitas(1)]; // chama o subprograma que verifica o arquivo ou tenta criar.
		if(atribuirID(id)){ // lê para a memória principal o nome das receitas do arquivo.
			menu(-3);
			cin >> subescolha;
			cin.ignore();
			if((subescolha >= 2) and (subescolha <= 5)){ // se a escolha for ver, editar, apagar ou exportar.
				escolha = 2;
			}
			else{ // demais escolhas.
				escolha = subescolha; 
			}
		}
		else{ // falha ao criar o arquivo.
			menu(14);
		}
		switch(escolha){
			default:{ // encerrar o programa.
				backupArquivo();
				menu(10);
				continuar = false;
				break;
			}
			case 0:{ // informações sobre o programa.
				menu(0);
				break;
			}
			case 1:{ // adicionar receita.
				menu(2);
				receita x;
				editarReceita(x, 1, id, false);
				if(salvarReceita(x, totalReceitas(1), 1)){
					menu(11);
					insertionsortReceitas();
				}
				else{
					menu(14);
				}
				break;
			}
			case 2:{ // ver, editar, apagar ou exportar receita.
				menu(1);
				cin >> escolha;
				cin.ignore();
				bool buscar;
				if(escolha == 1){ // ver receitas carregadas em lista.
					buscar = false;
				}
				else if(escolha == 2){ // procurar diretamente por nome.
					buscar = true;
				}
				else{ // opção inválida.
					menu(-1);
					menu(9);
					break;
				}
				int posicao = -1;
				if(!buscar){ // ver lista de receitas.
					menu(-1);
					if(!listaReceitas(id)){ 
						menu(13);
						break;
					}
					menu(-2);
					cin >> posicao;
					cin.ignore();
					posicao--;
				}
				else if(buscar){ // procura receita por nome.
					menu(16);
					char busca[MAX_NOME];
					lerNome(busca);
					posicao = binarysearchNome(id, busca, 0, totalReceitas(1) - 1);
				}
				receita x;
				menu(-1);
				if(carregarReceita(x, posicao, 1)){ // receita válida.
					switch(subescolha){
						default:{
							break;
						}
						case 2:{ // ver a receita.
							verReceita(x);
							break;
						}
						case 3:{ // editar a receita.
							verReceita(x);
							menu(3);
							cin >> escolha;
							cin.ignore();
							menu(-1);
							if(editarReceita(x, escolha, id, true)){
								if(salvarReceita(x, posicao, 1)){
									if(escolha == 1){
										insertionsortReceitas();
									}
									menu(12);
								}
								else{
									menu(14);
								}
							}
							else{ // opção inválida.
								menu(9);
							}
							break;
						}
						case 4:{ // apagar a areceita.
							apagarReceita(posicao);
							menu(4);
							break;
						}
						case 5:{ // exportar a receita.
							if(exportarReceita(x)){
								menu(5);
							}
							else{
								menu(14);
							}
							break;
						}
					}
				}
				else{ // receita inválida.
					menu(15);
				}
				break;
			}
			case 6:{ // apagar todas as receitas.
				menu(6);
				cin >> escolha;
				cin.ignore();
				if(escolha == 1){
					if(zerarArquivo(1)){
						menu(8);
					}
				}
				break;
			}
			case 7:{ // realizar backup do arquivo principal.
				backupArquivo();
				menu(7);
				break;
			}
		}
		delete[] id;
	}
	return 0;
}
