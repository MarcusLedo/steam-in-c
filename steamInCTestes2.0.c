// PROJETO FINAL (v1)
// Data: 08/11/2020
// Alunos: Arthur Martins Saraiva      UC20102454
//	       Marcus Vinicius Saba Ledo   UC19201305
// Objetivo: Montar um sistema visando público geral para entretenimento e marketing de mídias
// digitais, dando ênfase em jogos e softwares.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "conio.h"
// ********************************* ENTIDADES: ***************************************************

struct tCartao{
	char numero[20];
	int codigo;// 3 numeros atrás
};

struct tAdministrador{
	int codigo;
	char login[30];
	char senha[30];
};

struct tUser{
	int codigo;
	char login[30];
	int biblioteca;
	float valorGasto;
	float carteira;
	char senha[30];
	char deletado;
	struct tCartao cartao;
};

struct tJogo{
	int codigo;
	char nome[30];
	char descricao[500];
	char nomeDev[30];
	float valor;
	int desenvolvedora;
	char deletado;
};

struct tDesenvolvedor{
	int codigo;
	char nome[30];
	char deletado;
};

struct tModeracao{
	int moderacao;
	char entidade[30];
	char dataHora[80];
	
};

struct tRelacoes{// Usuário x Loja
	int userCodigo;//Código jogo (09:45  10/11)
	int jogoCodigo;
};
// ********************************* PROTOTIPO DE FUNÇÕES: ****************************************

// ________________________ Apresentações: _________________________________________________________

int menuLogin();
int menuAdm ();
int menuUser ();

// ________________________ Funções utilitárias: __________________________________________________

FILE *aberturaDeArquivo (char[]);// Abre ou cria arquivos.
void skip ();// Pausa o código até que uma tecla seja apertada.
int cadastroLogin(char[]);// Faz o cadastro do login de usuário.
int cadastroSenha(char[]);// Faz o cadastro da senha do usuário.
int validaEntrada (char[], char[], FILE*);// Faz validação entre usuário e senha.

int validaEntradaUser (FILE*, char[], char[]);
int validaCodigoJogo (int, FILE*); // Possível fusão com a função "validaEntrada".
int validaCodigoDev (int, FILE *);
int validaCodCartao (int);
int validaNumCartao(char[]);

// ________________________ Funções de listagem: __________________________________________________

void leituraAdm (FILE*);
void listarJogos (FILE*, FILE*);
void listarDev (FILE*);
void listarUsers (FILE*);
void moderacao (FILE*);

// ________________________  Função para gravação de dados: _______________________________________

void gravaDadosAdm (FILE*, struct tAdministrador);
void gravaDadosDev (FILE*, struct tDesenvolvedor, int);
void gravaDadosJogo (FILE*, struct tJogo, int);
void gravaDadosUser (FILE*, struct tUser, int);
void gravaDadosRel (FILE*, struct tRelacoes, int);
void gravaDadosModeracao (FILE*, struct tModeracao);

// ________________________ Funções dedicadas para jogos: _________________________________________

struct tJogo lerJogo (int, FILE*);// Lê a posição do jogo para fazer consultas ou alterações.
void excluirJogo (FILE*, int);// Faz a exclusão lógica dos jogos.
int retornaPosicao (FILE*, int);// Retorna a posição 

// ________________________ Funções dedicadas para dev: ___________________________________________

struct tDesenvolvedor lerDev (int, FILE*);// Lê a posição do dev para fazer consultas ou alterações.
void excluirDev (FILE*, int);// Faz a exclusão lógica dos desenvolvedores.
int retornaPosicaoD (FILE*, int);// Retorna a posição

// ________________________ Funções dedicadas para User: ___________________________________________

int contUser (FILE*);// Faz a contagem de usuários
struct tUser lerUser (FILE*,  int);
int verifExistenciaLogin (FILE*, char[]);
int retornaPosicaoUser (FILE*, int);
void excluirUser (FILE*, int);

// ________________________ Funções dedicadas para Relações: ______________________________________

int library (FILE*, FILE*, int);
int buscaRelJogo (FILE*, int, int);

// ________________________ Funções dedicadas para Exclusões fisica: ______________________________

int exclusaoFisicaJogo (FILE*, char []);
int exclusaoFisicaDev (FILE*, char[]);
int exclusaoFisicaUser (FILE*, char[]);

// ********************************* MAIN *********************************************************

int main (void){
	FILE *arqAdm, *arqUser, *arqJogo, *arqDev, *arqRel, *arqMod;
	int opc1, opc2, opc3, flag, chave, posicao, cont;
	struct tAdministrador adm;
	struct tJogo jogo;
	struct tDesenvolvedor dev;
	struct tUser user;
	struct tRelacoes rel;
	struct tModeracao mod;
	char inputLogin[30], inputSenha[30], chave2;
	float computSaldo;
	
	time_t salvaTempo;
    struct tm *info;
	time(&salvaTempo);
    info = localtime(&salvaTempo);
    
	arqJogo = aberturaDeArquivo ("jogosT.dat");
	arqUser = aberturaDeArquivo ("userT.dat");
	arqDev = aberturaDeArquivo("desenvolvedorasT.dat");
	arqRel = aberturaDeArquivo("userXjogo.dat");
	arqMod = aberturaDeArquivo("moderacao.dat");
	
	do{
		opc1 = menuLogin();
		switch (opc1){
			case 0:// _______ Administrador _______________________________________________________
				arqAdm = fopen("administradorT.dat", "rb+");
				if(arqAdm == NULL){// Adm não encontrado: ciar um arquivo e registra o adm.
					arqAdm = aberturaDeArquivo("administradorT.dat");
					gotoxy(32, 3); printf("Nao foi encontrado nenhum administrador");
					gotoxy(32, 4); printf("Cadastre um agora:");
					cadastroLogin(adm.login);
					cadastroSenha(adm.senha);
					adm.codigo = 1;
					gotoxy(35, 9); printf("Novo administrador cadastrado com sucesso!!!");
					gravaDadosAdm(arqAdm, adm);	
					skip();
				}
				//Adm encontrado: pede login e senha.
				gotoxy(32, 3); printf("-> Digite seu login: ");
				fflush(stdin);
				gotoxy(35, 4); gets(inputLogin);
				gotoxy(32, 5); printf("   Digite sua senha: ");
				fflush(stdin);
				gotoxy(35, 6); gets(inputSenha);
				if(validaEntrada(inputLogin, inputSenha, arqAdm) == 1){
					clrscr();
					do{
						opc2 = menuAdm();
						switch (opc2){
							case 0:
								gotoxy(32, 3); printf("-> Digite o codigo do jogo: ");
								gotoxy(35, 4); scanf("%d", &jogo.codigo);
								if(validaCodigoJogo(jogo.codigo, arqJogo) == 1){
									gotoxy(35, 5); printf("Digite o nome do jogo: ");
									fflush(stdin);
									gotoxy(35, 6); gets(jogo.nome);
									gotoxy(35, 7); printf("Digite a descricao do jogo: ");
									fflush(stdin);
									gotoxy(35, 8); gets(jogo.descricao);
									gotoxy(35, 9); printf("Digite o valor do jogo: ");
									gotoxy(35, 10); scanf("%f", &jogo.valor);
									gotoxy(9, 18); textcolor(5); printf("DESENVOLVEDORAS:");
									listarDev(arqDev);
									textcolor(15); gotoxy(35, 11); printf("Selecione a desenvolvedora: ");
									gotoxy(63, 11); scanf("%d", &jogo.desenvolvedora);
									posicao = retornaPosicaoD(arqDev, jogo.desenvolvedora);
								} if(posicao == -1){
										gotoxy(35, 12);	printf("Desenvolvedora nao encontrada!");
										gotoxy(35, 13);	printf("Jogo nao cadastrado!");
										skip();
									}else{
										dev = lerDev(posicao, arqDev);
										strcpy(jogo.nomeDev, dev.nome);
										jogo.deletado = '-';
										gravaDadosJogo(arqJogo, jogo, -1);
										gotoxy(35, 12); printf("Jogo cadastrado com sucesso!");
										time(&salvaTempo);
										info = localtime(&salvaTempo);
										strcpy(mod.entidade, jogo.nome);
										strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
										mod.moderacao = 11;
										gravaDadosModeracao(arqMod, mod);
										skip();
									}
							break;
							case 1:
								listarJogos(arqJogo, arqDev);
								skip();
							break;
							case 2:
								gotoxy(32, 5); printf("-> Digite o codigo do jogo que esta buscando: ");
								scanf("%d", &chave);
								posicao = retornaPosicao(arqJogo, chave);
								if(posicao != -1){
									jogo = lerJogo(posicao, arqJogo);
									gotoxy(35, 6); printf("Nome......: %s", jogo.nome);
									gotoxy(35, 7); printf("Preco.....: R$ %.2f", jogo.valor);
									gotoxy(35, 8); printf("Descricao.: %s", jogo.descricao);
									skip();
								}else{
									gotoxy(35, 6); printf("O jogo nao foi econtrado!!!");
									skip();
								}
							break;
							case 3:
								gotoxy(32, 6); printf("-> Digite o codigo do jogo que deseja buscar: ");
								gotoxy(35, 7); scanf("%d", &chave);
								posicao = retornaPosicao(arqJogo, chave);
								if(posicao != -1){
									jogo = lerJogo(posicao, arqJogo);
									gotoxy(35, 8); printf("Novo nome.....: ");
									fflush(stdin);
									gotoxy(35, 9);gets(jogo.nome);
									gotoxy(35, 10); printf("Novo preco....: ");
									gotoxy(35, 11);scanf("%f", &jogo.valor);
									gotoxy(35, 12); printf("Nova descricao: ");
									fflush(stdin);
									gotoxy(35, 13);gets(jogo.descricao);
									gravaDadosJogo(arqJogo, jogo, posicao);
									time(&salvaTempo);
									info = localtime(&salvaTempo);
									strcpy(mod.entidade, jogo.nome);
									strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
									mod.moderacao = 12;
									gravaDadosModeracao(arqMod, mod);
									gotoxy(35, 14); printf("Dados alterados!");
								}else{
									gotoxy(35, 8); printf("O jogo nao foi econtrado!!!");
									skip();
								}
							break;
							case 4:
								gotoxy(32, 7); printf("-> Digite o codigo do jogo que deseja buscar: ");
								gotoxy(35, 8); scanf("%d", &chave);
								posicao = retornaPosicao(arqJogo, chave);//Retorna a posição do jogo.
								if(posicao != -1){
									jogo = lerJogo(posicao, arqJogo);
									excluirJogo(arqJogo, posicao);
									time(&salvaTempo);
									info = localtime(&salvaTempo);
									strcpy(mod.entidade, jogo.nome);
									strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
									mod.moderacao = 13;
									gravaDadosModeracao(arqMod, mod);
									gotoxy(35, 9); printf("Dados excluidos!!!");
									skip();
								}else{
									gotoxy(35, 9); printf("O jogo nao foi econtrado!!!");
									skip();
								}
							break;
							case 5:
								gotoxy(32, 8); printf("-> Codigo da desenvolvedora: ");
								gotoxy(35, 9); scanf("%d", &dev.codigo);
								if (validaCodigoDev (dev.codigo, arqDev) == 1){
									gotoxy(35, 10); printf("Nome da desenvolvedora: ");
									fflush(stdin);
									gotoxy(35, 11); gets(dev.nome);
									dev.deletado = '-';
									gravaDadosDev(arqDev, dev, -1);
									time(&salvaTempo);
									info = localtime(&salvaTempo);
									strcpy(mod.entidade, dev.nome);
									strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
									mod.moderacao = 21;
									gravaDadosModeracao(arqMod, mod);
									gotoxy(35, 11); printf("desenvolvedora adicionada com sucesso!");
									skip();
								}
							break;
							case 6:
								listarDev(arqDev);
							break;
							case 7:
								gotoxy(32, 10); printf("-> Digite o codigo do desenvolvedor que esta buscando: ");
								gotoxy(35, 11); scanf("%d", &chave);
								posicao = retornaPosicaoD(arqDev, chave);
								if(posicao != -1){
									dev = lerDev(posicao, arqDev);
									gotoxy(35, 12); printf("Codigo.: %d", dev.codigo);
									gotoxy(35, 13); printf("Nome...: %s", dev.nome);
									skip();
								}else{
									gotoxy(35, 12); printf("O jogo nao foi econtrado!!!");
									skip();
								}
							break;
							case 8:
								gotoxy(32, 11); printf("-> Digite o codigo do desenvolvedor que deseja buscar: ");
								gotoxy(35, 12); scanf("%d", &chave);
								posicao = retornaPosicaoD(arqDev, chave);//Retorna a posição do dev.
								if(posicao != -1){
									dev = lerDev(posicao, arqDev);
									gotoxy(35, 13); printf("Nome......: %s", dev.nome);
									excluirDev(arqDev, posicao);
									gotoxy(35, 14); printf("Desenvolvedor removido.");
									time(&salvaTempo);
									info = localtime(&salvaTempo);
									strcpy(mod.entidade, dev.nome);
									strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
									mod.moderacao = 23;
									gravaDadosModeracao(arqMod, mod);
									skip();
								}else{
									gotoxy(35, 13); printf("Desenvolvedor nao foi econtrado.");
									skip();
								}
							break;
							case 9:
								listarUsers(arqUser);
								skip();
							break;
							case 10:
								gotoxy(32, 13); printf("-> Digite o codigo do usuario que deseja buscar: ");
								gotoxy(35, 14); scanf("%d", &chave);
								posicao = retornaPosicaoUser (arqUser, chave);
								if(posicao != -1){
									user = lerUser(arqUser, posicao);
									gotoxy(35, 15); printf("Nome......: %s", user.login);
									excluirUser(arqUser, posicao);
									gotoxy(35, 16); printf("Usuario removido.");
									time(&salvaTempo);
									info = localtime(&salvaTempo);
									strcpy(mod.entidade, user.login);
									strftime(mod.dataHora,80,"%d/%m/%Y - %H:%M", info);
									mod.moderacao = 33;
									gravaDadosModeracao(arqMod, mod);
									skip();
								}else{
									gotoxy(35, 15); printf("Usuario nao foi econtrado.");
									skip();
								}
							break;
							case 11:
								exclusaoFisicaJogo(arqJogo, "jogosT.dat");
								exclusaoFisicaDev (arqDev, "desenvolvedorasT.dat");
								exclusaoFisicaUser (arqUser, "userT.dat");
								gotoxy(32, 14); printf("-> Dados excluidos com sucesso!!!");
								skip();
							break;
							case 12:
								moderacao(arqMod);
						}
					}while(opc2 != 13);
				}else{
					printf("\nDados incorretos!!!");
					skip();
				}
				fclose(arqAdm);	
				clrscr();
			break;
			case 1:// ______ Usuário ______________________________________________________________ 
				//User login.
				gotoxy(32, 4); printf("-> Digite seu login: ");
				fflush(stdin);
				gotoxy(35, 5); gets(user.login);
				gotoxy(32, 6); printf("   Digite sua senha: ");
				fflush(stdin);
				gotoxy(35, 7); gets(user.senha);
				if(validaEntradaUser(arqUser, user.login, user.senha) != -1){
					posicao = validaEntradaUser(arqUser, user.login, user.senha);
					user = lerUser(arqUser, posicao);
					clrscr();
					do{
						opc3 = menuUser();
						switch (opc3){
							case 0: // loja
								do{
									system("cls");
									gotoxy(32, 3); printf("-> Digite -1 para sair");
									listarJogos(arqJogo, arqDev);
									gotoxy(35, 4); printf("Digite o codigo do Jogo que esta buscando: ");
									gotoxy(35, 5); scanf("%d", &chave);
									posicao = retornaPosicao(arqJogo, chave);
									if(posicao != -1){
										jogo = lerJogo(posicao, arqJogo);
										gotoxy(45, 5); printf("Saldo Atual: %.2f", user.carteira);
										gotoxy(35, 6);printf("Nome......: %s", jogo.nome);
										gotoxy(35, 7);printf("Preco.....: R$ %.2f", jogo.valor);
										gotoxy(35, 8);printf("Descricao.: %s", jogo.descricao);
										gotoxy(35, 9);printf("(1. Comprar)");
										gotoxy(48, 9);printf("(2. Voltar)");
										fflush(stdin);
										gotoxy(35, 10); chave2 = getche();
										if(chave2 == '1'){
											if(buscaRelJogo(arqRel, user.codigo, jogo.codigo) == 0){
												if(user.carteira >= jogo.valor){
													gotoxy(35, 15); printf("Compra efetuda com sucesso!");
													rel.jogoCodigo = jogo.codigo;
													rel.userCodigo = user.codigo;
													user.carteira -= jogo.valor;
													gravaDadosUser(arqUser, user, posicao);
													gravaDadosRel(arqRel, rel, -1);
													skip();
												}else{
													gotoxy(35, 15); printf("Saldo Insuficiente!");
													skip();
													}
											}else{
												gotoxy(35, 11); printf("Voce nao pode comprar o mesmo jogo!");
												skip();
												}
										}
									}else{
										gotoxy(35, 6); printf("O jogo nao foi econtrado!!!");
										chave = -1;
										skip();
									}
								}while(chave != -1);
							break;
							case 1://biblioteca
								gotoxy(32, 4); printf("-> Jogos do usuario: %s", user.login);
								library(arqRel, arqJogo, user.codigo);
								skip();
							break;
							case 2: // carteira
								if(strcmp(user.cartao.numero, "") != 0){
									gotoxy(32, 5); printf("-> Saldo atual: R$%.2f", user.carteira);
									gotoxy(35, 6); printf("Digite um valor menor que 0 para cancelar!!!");
									gotoxy(35, 7); printf("Digite o valor que deseja adicionar: ");
									gotoxy(35, 8); scanf("%f", &computSaldo);
									if(computSaldo > 0){
										user.carteira += computSaldo;
										gravaDadosUser(arqUser, user, posicao);
										gotoxy(35,9); printf("Valor adicionado com sucesso!");
										skip();						
									}else{
										gotoxy(35, 9); printf("Transacao cancelada!!!");
										skip();
									}
								}else{
									gotoxy(35, 5); printf("Cartao nao encontrado!");
									skip();
								}
							break;
							case 3: // adicionar cartão
								if(strcmp(user.cartao.numero, "") == 0){									
									gotoxy(32, 6); printf("-> Digite o numero do cartao: ");
									fflush(stdin);
									gotoxy(35, 7); gets(user.cartao.numero);
									if(validaNumCartao(user.cartao.numero) == 1){
										gotoxy(35, 8); printf("O cartao precisa ter 16 digitos!!!");
										strcpy(user.cartao.numero, "");
										skip();
									}else{
										gotoxy(35, 8); printf("Digite os 3 numeros (CVV) do cartao: ");
										gotoxy(35, 9); scanf("%d", &user.cartao.codigo);
										if(validaCodCartao(user.cartao.codigo) == 1){
											gotoxy(35, 10); printf("O CVV precisa ter 3 digitos!!!");
											skip();
										}else{
											gravaDadosUser(arqUser, user, posicao);
											gotoxy(35, 10); printf("Cartao cadastrado.");
										}
									}
								}else{
									gotoxy(32, 6); printf("-> Ja existe um cartao cadastrado!!");
									skip();
								}
							break;
							case 4: //remover catão
								if(strcmp(user.cartao.numero, "") == 0){
									gotoxy(32, 7); printf("-> Nao existe nenhum cartao cadastrado!!!");
									skip();
								}else{
									gotoxy(32, 7); printf("Remover cartao de final %s? (s - sim e n - nao)", user.cartao.numero + 12);
									chave2 = getche();
									if(chave2 == 's'){
										strcpy(user.cartao.numero, "");
										user.cartao.codigo = 0;
										gravaDadosUser(arqUser, user, posicao);
										gotoxy(35, 8); printf("Cartao removido com sucesso!!!");
										skip();
									}else
										gotoxy(35, 8); printf("Opcao cancelada.");
								}
						}
					}while(opc3 != 5);
				}else{
					printf("Usuario nao foi encontrado!");
					skip();
				}
				clrscr();
			break;
			case 2:
					
				if(cadastroLogin(user.login) == 1){
					printf("O login precisa ter no minimo 6 caracteres!!");
					skip();
				}else if(verifExistenciaLogin(arqUser, user.login) == 1){
						printf("Esse login ja esta em uso, tente outro!");
						skip();
					}else{
						if(cadastroSenha(user.senha) == 1)
							printf("A senha precisa ter no minimo 6 caracteres!!");
						else{ 
							user.codigo = contUser(arqUser);// O código e gerado automaticamente
							user.deletado = '-';
							user.carteira = 0;
							user.cartao.codigo = 0;
							strcpy(user.cartao.numero, "");
							gravaDadosUser(arqUser, user, -1);
							printf("Novo usuario cadastrado com sucesso!!!");
							skip();
						}
					}
		}
	}while(opc1 != 3);
	fclose(arqJogo);
	fclose(arqDev);
	fclose(arqUser);
	fclose(arqRel);	
	fclose(arqMod);	
	
	return 0;
}
// ********************************* FUNÇÕES PARA APRESENTAÇÕES ***********************************

// _____________________ Menu login. ______________________________________________________________

int menuLogin () {
	int opcao=0, i=0, flag=0;
	char direcionais[2];
	
	clrscr();
	_setcursortype(0);
	do{
		gotoxy(13, 1); textbackground(0); textcolor(3); printf("|");
		gotoxy(14, 1); textbackground(0); textcolor(15); printf("Login");
		gotoxy(19, 1); textbackground(0); textcolor(3); printf("|");
		textbackground(0); textcolor(3);
		for (i = 2; i < 31; i++) {//cima e baixo
	    	gotoxy(i,2);printf("%c", 196);
	    	gotoxy(i,7);printf("%c", 196);
	    }//quinas
	    gotoxy(1,2);printf("%c", 218);
	    gotoxy(31,2);printf("%c", 191);
	    gotoxy(1,7);printf("%c", 192);
	    gotoxy(31,7);printf("%c", 217);
	    //esquerda
	    gotoxy(1, 3); printf("%c", 179);
	    gotoxy(1, 4); printf("%c", 179);
	    gotoxy(1, 5); printf("%c", 179);
	    gotoxy(1, 6); printf("%c", 179);
	    //direita
	    gotoxy(31, 3); printf("%c", 179);
	    gotoxy(31, 4); printf("%c", 179);
	    gotoxy(31, 5); printf("%c", 179);
	    gotoxy(31, 6); printf("%c", 179);
	    
	    textbackground(0); textcolor(15);
	    
		gotoxy(3, 3); printf(" Entrar como administrador  ");
		gotoxy(3, 4); printf(" Entrar como usuario        ");
		gotoxy(3, 5); printf(" Criar um novo usuario      ");
		gotoxy(3, 6); printf(" Sair                       ");
		
		textbackground(1);
		textcolor(15);
		switch (opcao){
			case 0:
				gotoxy(1, 3); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 3); textbackground(1); textcolor(15); printf("Entrar como administrador  ");
				gotoxy(31, 3); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 1:
				gotoxy(1, 4); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 4); textbackground(1); textcolor(15); printf("Entrar como usuario        ");
				gotoxy(31, 4); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 2:
				gotoxy(1, 5); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 5); textbackground(1); textcolor(15); printf("Criar um novo usuario      ");
				gotoxy(31, 5); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 3:
				gotoxy(1, 6); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 6); textbackground(1); textcolor(15); printf("Sair                       ");
				gotoxy(31, 6); textbackground(0); textcolor(3); printf("%c", 179);
		}
		
		direcionais[0] = getch();
		if(direcionais[0] == -32){
			direcionais[1] = getch();
			
			if(direcionais[0] != -32 || direcionais[1] != 80)
				opcao--;
			else if(direcionais[0] != -32 || direcionais[1] != 72)
				opcao++;
				
			if(opcao < 0)
				opcao = 3;
			else if(opcao > 3)
				opcao = 0;
		}else if (direcionais[0] == 13)
			flag = 1;
	}while(flag != 1);
	textcolor(15);
	return opcao;
}

// _____________________ Menu adm. ________________________________________________________________

int menuAdm (){
	int opcao = 0, i, flag = 0;
	char direcionais[2];
	
	clrscr();
	_setcursortype(0);
	do{
		gotoxy(8, 1); textbackground(0); textcolor(5); printf("|");
		gotoxy(9, 1); textbackground(0); textcolor(15); printf("ADMINISTRADOR");
		gotoxy(22, 1); textbackground(0); textcolor(5); printf("|");
		textbackground(0); textcolor(5);
		for (i = 2; i < 31; i++) { //cima e baixo
	    	gotoxy(i,2);printf("%c", 196);
	    	gotoxy(i,17);printf("%c", 196);
	    }// quinas
	    gotoxy(1,2);printf("%c", 218);
	    gotoxy(31,2);printf("%c", 191);
	    gotoxy(1,17);printf("%c", 192);
	    gotoxy(31,17);printf("%c", 217);
	    //esquerda
	    gotoxy(1, 3); printf("%c", 179);
	    gotoxy(1, 4); printf("%c", 179);
	    gotoxy(1, 5); printf("%c", 179);
	    gotoxy(1, 6); printf("%c", 179);
	    gotoxy(1, 7); printf("%c", 179);
	    gotoxy(1, 8); printf("%c", 179);
	    gotoxy(1, 9); printf("%c", 179);
	    gotoxy(1, 10); printf("%c", 179);
	    gotoxy(1, 11); printf("%c", 179);
	    gotoxy(1, 12); printf("%c", 179);
	    gotoxy(1, 13); printf("%c", 179);
	    gotoxy(1, 14); printf("%c", 179);
	    gotoxy(1, 15); printf("%c", 179);
	    gotoxy(1, 16); printf("%c", 179);
	    //direita
	    gotoxy(31, 3); printf("%c", 179);
	    gotoxy(31, 4); printf("%c", 179);
	    gotoxy(31, 5); printf("%c", 179);
	    gotoxy(31, 6); printf("%c", 179);
	    gotoxy(31, 7); printf("%c", 179);
	    gotoxy(31, 8); printf("%c", 179);
	    gotoxy(31, 9); printf("%c", 179);
	    gotoxy(31, 10); printf("%c", 179);
	    gotoxy(31, 11); printf("%c", 179);
	    gotoxy(31, 12); printf("%c", 179);
	    gotoxy(31, 13); printf("%c", 179);
	    gotoxy(31, 14); printf("%c", 179);
	    gotoxy(31, 15); printf("%c", 179);
	    gotoxy(31, 16); printf("%c", 179);
	    
	    textbackground(0); textcolor(15);
		gotoxy(2, 3); printf(" Adicionar jogo             ");
		gotoxy(2, 4); printf(" Listar jogos               ");
		gotoxy(2, 5); printf(" Consultar Jogo             ");
		gotoxy(2, 6); printf(" Alterar jogo               ");
		gotoxy(2, 7); printf(" Remover jogo               ");
		gotoxy(2, 8); printf(" Adicionar Desenvolvedora   ");
		gotoxy(2, 9); printf(" Listar Desevolvedoras      ");
		gotoxy(2, 10); printf(" Consultar Desevolvedora    ");
		gotoxy(2, 11); printf(" Excluir Desevolvedora      ");
		gotoxy(2, 12); printf(" Listar Usuarios            ");
		gotoxy(2, 13); printf(" Excluir Usuarios           ");
		gotoxy(2, 14); printf(" Limpar lixeira             ");
		gotoxy(2, 15); printf(" Historico de moderacao     ");
		gotoxy(2, 16); printf(" Voltar                     ");
		
		textbackground(1);
		textcolor(15);
		switch (opcao){
			case 0:
				gotoxy(1, 3); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 3); textbackground(1); textcolor(15); printf("Adicionar jogo             ");
				gotoxy(31, 3); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 1:
				gotoxy(1, 4); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 4); textbackground(1); textcolor(15); printf("Listar jogos               ");
				gotoxy(31, 4); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 2:
				gotoxy(1, 5); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 5); textbackground(1); textcolor(15); printf("Consultar Jogo             ");
				gotoxy(31, 5); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 3:
				gotoxy(1, 6); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 6); textbackground(1); textcolor(15); printf("Alterar jogo               ");
				gotoxy(31, 6); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 4:
				gotoxy(1, 7); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 7); textbackground(1); textcolor(15); printf("Remover jogo               ");
				gotoxy(31, 7); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 5:
				gotoxy(1, 8); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 8); textbackground(1); textcolor(15); printf("Adicionar Desenvolvedora   ");
				gotoxy(31, 8); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 6:
				gotoxy(1, 9); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 9); textbackground(1); textcolor(15); printf("Listar Desenvolvedoras     ");
				gotoxy(31, 9); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 7:
				gotoxy(1, 10); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 10); textbackground(1); textcolor(15); printf("Consultar Desenvolvedora   ");
				gotoxy(31, 10); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 8:
				gotoxy(1, 11); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 11); textbackground(1); textcolor(15); printf("Excluir Desenvolvedora     ");
				gotoxy(31, 11); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 9:
				gotoxy(1, 12); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 12); textbackground(1); textcolor(15); printf("Listar Usuario             ");
				gotoxy(31, 12); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 10:
				gotoxy(1, 13); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 13); textbackground(1); textcolor(15); printf("Excluir usuario            ");
				gotoxy(31, 13); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 11:
				gotoxy(1, 14); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 14); textbackground(1); textcolor(15); printf("Limpar lixeira             ");
				gotoxy(31, 14); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 12:
				gotoxy(1, 15); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 15); textbackground(1); textcolor(15); printf("Historico de moracao       ");
				gotoxy(31, 15); textbackground(0); textcolor(5); printf("%c", 179);
			break;
			case 13:
				gotoxy(1, 16); textbackground(0); textcolor(5); printf("%c", 179);
				gotoxy(2, 16); textbackground(1); textcolor(15); printf("Voltar                     ");
				gotoxy(31, 16); textbackground(0); textcolor(5); printf("%c", 179);
		}
		direcionais[0] = getch();
		if(direcionais[0] == -32){
			direcionais[1] = getch();
			
			if(direcionais[0] != -32 || direcionais[1] != 80)
				opcao--;
			else if(direcionais[0] != -32 || direcionais[1] != 72)
				opcao++;
				
			if(opcao < 0)
				opcao = 13;
			else if(opcao > 13)
				opcao = 0;
		}else if (direcionais[0] == 13)
			flag = 1;
	}while(flag != 1);
	textcolor(15);
	return opcao;
}

// _____________________ Menu user. _______________________________________________________________

int menuUser (){
	int opcao=0, i=0, flag=0;
	char direcionais[2];
	
	clrscr();
	_setcursortype(0);
	do{
		gotoxy(12, 1); textbackground(0); textcolor(3); printf("|");
		gotoxy(13, 1); textbackground(0); textcolor(15); printf("Usuario");
		gotoxy(20, 1); textbackground(0); textcolor(3); printf("|");
		textbackground(0); textcolor(3);
		for (i = 2; i < 31; i++) {//cima e baixo
	    	gotoxy(i,2);printf("%c", 196);
	    	gotoxy(i,9);printf("%c", 196);
	    }//quinas
	    gotoxy(1,2);printf("%c", 218);
	    gotoxy(31,2);printf("%c", 191);
	    gotoxy(1,9);printf("%c", 192);
	    gotoxy(31,9);printf("%c", 217);
	    //esquerda
	    gotoxy(1, 3); printf("%c", 179);
	    gotoxy(1, 4); printf("%c", 179);
	    gotoxy(1, 5); printf("%c", 179);
	    gotoxy(1, 6); printf("%c", 179);
	    gotoxy(1, 7); printf("%c", 179);
		gotoxy(1, 8); printf("%c", 179);
	    //direita
	    gotoxy(31, 3); printf("%c", 179);
	    gotoxy(31, 4); printf("%c", 179);
	    gotoxy(31, 5); printf("%c", 179);
	    gotoxy(31, 6); printf("%c", 179);
	    gotoxy(31, 7); printf("%c", 179);
	    gotoxy(31, 8); printf("%c", 179);
	    
	    textbackground(0); textcolor(15);
	    
	    gotoxy(3, 3); printf(" Loja                       ");
		gotoxy(3, 4); printf(" Biblioteca                 ");
		gotoxy(3, 5); printf(" Carteira                   ");
		gotoxy(3, 6); printf(" Adicionar cartao           ");
		gotoxy(3, 7); printf(" Remover cartao             ");
		gotoxy(3, 8); printf(" Voltar                     ");
	    
		switch (opcao){
			case 0:
				gotoxy(1, 3); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 3); textbackground(1); textcolor(15); printf("Loja                       ");
				gotoxy(31, 3); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 1:
				gotoxy(1, 4); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 4); textbackground(1); textcolor(15); printf("Biblioteca                 ");
				gotoxy(31, 4); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 2:
				gotoxy(1, 5); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 5); textbackground(1); textcolor(15); printf("Carteira                   ");
				gotoxy(31, 5); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 3:
				gotoxy(1, 6); textbackground(0); textcolor(3); printf("%c", 179);
				gotoxy(3, 6); textbackground(1); textcolor(15); printf("Adicionar cartao           ");
				gotoxy(31, 6); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 4:
			gotoxy(1, 7); textbackground(0); textcolor(3); printf("%c", 179);
			gotoxy(3, 7); textbackground(1); textcolor(15); printf("Remover cartao             ");
			gotoxy(31, 7); textbackground(0); textcolor(3); printf("%c", 179);
			break;
			case 5:
			gotoxy(1, 8); textbackground(0); textcolor(3); printf("%c", 179);
			gotoxy(3, 8); textbackground(1); textcolor(15); printf("Voltar                     ");
			gotoxy(31, 8); textbackground(0); textcolor(3); printf("%c", 179);
		}
		direcionais[0] = getch();
		if(direcionais[0] == -32){
			direcionais[1] = getch();
			
			if(direcionais[0] != -32 || direcionais[1] != 80)
				opcao--;
			else if(direcionais[0] != -32 || direcionais[1] != 72)
				opcao++;
				
			if(opcao < 0)
				opcao = 5;
			else if(opcao > 5)
				opcao = 0;
		}else if (direcionais[0] == 13)
			flag = 1;
	}while(flag != 1);
	textcolor(15);
	return opcao;
}
// ********************************* FUNÇÕES UTILITÁRIAS: *****************************************

// ______ Abre ou cria arquivos. __________________________________________________________________

FILE *aberturaDeArquivo (char nome[]){
	FILE *arq;
	arq = fopen(nome, "rb+");
	if(arq == NULL)
		arq = fopen(nome, "wb+");
		
	return arq;
}

// ______ Pausa o código até que uma tecla seja apertada. _________________________________________

void skip (){
	char skip;
	
	gotoxy(25, 1); textcolor(2); printf("Aperte qualquer tecla para continuar...");
	skip = getche();
	system("cls");
	textcolor(15);
}

// ______ Faz o cadastro do login de usuário. _____________________________________________________

int cadastroLogin (char login[]){
	
	gotoxy(32, 5); printf("-> Novo login: ");
	fflush(stdin);
	gotoxy(35, 6); gets(login);
	if(strlen(login) < 6)
		return 1;
	return 0;
}

// ______ Faz o cadastro da senha do usuário. _____________________________________________________

int cadastroSenha (char senha[]){

	gotoxy(32, 7); printf("   Nova senha: ");
	fflush(stdin);
	gotoxy(35, 8); gets(senha);
	if(strlen(senha) < 6)
		return 1;
	return 0;

}

// ______ Faz validação entre usuário e senha para logar. _________________________________________

int validaEntrada (char login[], char senha[], FILE *arq){
	struct tAdministrador adm;
	fseek(arq, 0, SEEK_SET);
	fread(&adm, sizeof(adm), 1, arq);
	if(strcmp(adm.login, login) != 0)
		return 0;
	if(strcmp(adm.senha, senha) != 0)
		return 0;	
	return 1;
}

// ______ Verifica se já tem um usuário com determinado nome. _____________________________________

int validaEntradaUser (FILE *arq, char login[], char senha[]){
	struct tUser user;
	int cont = 0;	
	
	fseek(arq, 0, SEEK_SET);
	while(fread(&user, sizeof(user), 1, arq) != 0){
		if(strcmp(login, user.login) == 0 && strcmp(senha, user.senha) == 0)
			return cont;
		cont++;
	}
	
	return -1;
}

// ______ Valida o código do jogo. ________________________________________________________________

int validaCodigoJogo (int codigo, FILE *arq){
	struct tJogo jogo;
	fseek(arq, 0, SEEK_SET);
	while(fread(&jogo, sizeof(jogo), 1, arq) != 0){
		if(codigo == jogo.codigo){
			printf("Nao podem haver codigos repetidos!");
			return 0;
		}
	}
	return 1;
}

// ______ Válida o código dos desenvolvedores. ____________________________________________________

int validaCodigoDev (int codigo, FILE *arq){
	struct tDesenvolvedor dev;
	fseek(arq, 0, SEEK_SET);
	while(fread(&dev, sizeof(dev), 1, arq) != 0){
		if(codigo == dev.codigo){
			gotoxy(35, 10); printf("Nao podem haver codigos repetidos!");
			skip();
			return 0;
		}
	}
	return 1;
}

// ______ Valida número do cartão _________________________________________________________________

int validaNumCartao (char nCartao[]){
	if(strlen(nCartao) != 16)
		return 1;
	else
		return 0;
}

// ______ Valida código do cartão _________________________________________________________________

int validaCodCartao (int codCartao){
	if(codCartao <= 99 || codCartao >= 1000)
		return 1;
	else
		return 0;
}
// ********************************* FUNÇÕES DE LISTAGEM (PERFIL, JOGOS...): **********************

// ______ Lista o perfil do adm. __________________________________________________________________

void leituraAdm (FILE *arqAdm){
	struct tAdministrador adm;
	fseek(arqAdm, 0, SEEK_SET);
	while(fread(&adm, sizeof(adm), 1, arqAdm) != 0){
		printf("-> Codigo: %i", adm.codigo);
		printf("-> Login: %s", adm.login);
		printf("-> senha: %s", adm.senha);
	}
}

// ______ Lista os jogos. _________________________________________________________________________

void listarJogos (FILE *arq, FILE *arqD){
	struct tJogo jogo;
	struct tDesenvolvedor dev;
	int i=22;
	fseek(arq, 0, SEEK_SET);
	gotoxy(1, 20); printf("COD   NOME                VALOR    DESENVOLVEDORA");
	gotoxy(1, 21);printf("_____ ___________________  ________ _________________");
	while(fread(&jogo, sizeof(jogo), 1, arq) != 0){
		fseek(arqD, 0, SEEK_SET);
		while(fread(&dev, sizeof(dev), 1, arqD) != 0){
			
			if(jogo.desenvolvedora == dev.codigo){
				gotoxy(1, i); printf("%5d %19s %9.2f %17s\n", jogo.codigo, jogo.nome, jogo.valor, jogo.nomeDev);
				i++;
			}
		}
	}	
}

// ______ Lista os desenvolvedores. _______________________________________________________________

void listarDev (FILE *arq){
	struct tDesenvolvedor dev;
	int i=22;
	fseek(arq, 0, SEEK_SET);
	gotoxy(1, 20); printf("D COD   NOME                          ");
	gotoxy(1, 21); printf("_ _____ ______________________________");
	while(fread(&dev, sizeof(dev), 1, arq) != 0){
		gotoxy(1, i); printf("%c %d      %s\n", dev.deletado, dev.codigo, dev.nome);
		i++;
	}
	skip();
}

// ______ Lista os usuários. ______________________________________________________________________

void listarUsers (FILE *arq){
	int i= 22;
	struct tUser user;
	fseek(arq, 0, SEEK_SET);
	gotoxy(1, 20); printf("D COD   NOME                          ");
	gotoxy(1, 21); printf("_ _____ ______________________________");
	while(fread(&user, sizeof(user), 1, arq) != 0){
		gotoxy(1, i); printf("%c %d      %s                              %s\n", user.deletado, user.codigo, user.login, user.cartao.numero);
		i++;
	}
}

// ______ Listagem da moderação. __________________________________________________________________

void moderacao (FILE *arqMod){
	struct tModeracao mod;
	clrscr();
	fseek(arqMod, 0, SEEK_SET);
	while(fread(&mod, sizeof(mod), 1, arqMod) != 0){
		switch (mod.moderacao){
			case 11:
				textcolor(2);
				printf("|O administrador incluiu o jogo %10s                %s|\n", mod.entidade, mod.dataHora);
			break;
			case 12:
				textcolor(7);
				printf("|O administrador alterou o jogo %10s                %s|\n", mod.entidade, mod.dataHora);
			break;
			case 13:
				textcolor(4);
				printf("|O administrador excluiu o jogo %10s                %s|\n", mod.entidade, mod.dataHora);
			break;
			case 21:
				textcolor(2);
				printf("|O administrador incluiu a desenvolvedora %10s      %s|\n", mod.entidade, mod.dataHora);
			break;
			case 23:
				textcolor(4);
				printf("|O administrador excluiu a desenvolvedora %10s      %s|\n", mod.entidade, mod.dataHora);
			break;
			case 33:
				textcolor(4);
				printf("|O administrador excluiu o usuario %10s             %s|\n", mod.entidade, mod.dataHora);
		}
	}
	skip();
}
// ********************************* FUNÇÃO PARA GRAVAÇÃO DE DADOS: *******************************

// ______ Gravação de dados para adm's. ___________________________________________________________

void gravaDadosAdm (FILE *arqAdm, struct tAdministrador adm){
	fseek(arqAdm, 0, SEEK_END);
	fwrite(&adm, sizeof(adm), 1, arqAdm);
}

// ______ Gravação de dados para os desenvolvedores. ______________________________________________

void gravaDadosDev (FILE *arqDev, struct tDesenvolvedor dev, int posicao){
	if(posicao == -1){
		fseek(arqDev, 0, SEEK_END);
		fwrite(&dev, sizeof(dev), 1, arqDev);
	}else{
		fseek(arqDev, sizeof(dev)*posicao, SEEK_SET);
		fwrite(&dev, sizeof(dev), 1, arqDev);
	}
}
	
// ______ Gravação de dados para os jogos. ________________________________________________________

void gravaDadosJogo (FILE *arqJogo, struct tJogo jogo, int posicao){
	
	if(posicao == -1){
		fseek(arqJogo, 0, SEEK_END);
		fwrite(&jogo, sizeof(jogo), 1, arqJogo);
	}else{
		fseek(arqJogo, sizeof(jogo)*posicao, SEEK_SET);
		fwrite(&jogo, sizeof(jogo), 1, arqJogo);
	}
}

// ______ Gravação de dados para os Usuários. ________________________________________________________

void gravaDadosUser (FILE *arqUser, struct tUser user, int posicao){
	
	if(posicao == -1){
		fseek(arqUser, 0, SEEK_END);
		fwrite(&user, sizeof(user), 1, arqUser);
	}else{
		fseek(arqUser, sizeof(user)*posicao, SEEK_SET);
		fwrite(&user, sizeof(user), 1, arqUser);
	}
}

// ______ Grava os dados da relação entre a bibioteca do usuário e os jogos comprados. ____________

void gravaDadosRel (FILE *arqRel, struct tRelacoes rel, int posicao){
	
	if(posicao == -1){
		fseek(arqRel, 0, SEEK_END);
		fwrite(&rel, sizeof(rel), 1, arqRel);
	}else{
		fseek(arqRel, sizeof(rel)*posicao, SEEK_SET);
		fwrite(&rel, sizeof(rel), 1, arqRel);
	}
}

// ______ Grava os dados da Moderação. ____________________________________________________________

void gravaDadosModeracao (FILE *arqMod, struct tModeracao mod){
	
	fseek(arqMod, 0, SEEK_END);
	fwrite(&mod, sizeof(mod), 1, arqMod);
	
}
// ********************************* FUNÇÕES DEDICADAS PARA JOGOS: ********************************

// ______ Lê a posição de determinado jogo para fazer alterações ou consultas. ____________________

struct tJogo lerJogo (int posicao, FILE *arq){
	struct tJogo jogo;
	fseek(arq, sizeof(jogo)*posicao, SEEK_SET);
	fread(&jogo, sizeof(jogo), 1, arq);
	return jogo;
}

// ______ Faz a exclusão lógica dos jogos. ________________________________________________________

void excluirJogo (FILE *arq, int posicao){
	struct tJogo jogo;
	fseek(arq, sizeof(jogo)*posicao, SEEK_SET);
	jogo.deletado = '*';
	fwrite(&jogo, sizeof(jogo), 1, arq);
}

// ______ Retorna a posição do jogo para fazer alterações ou consultas. ___________________________

int retornaPosicao (FILE *arq, int chave){
	struct tJogo jogo;
	int i = 0;
	fseek(arq, 0, SEEK_SET);
	while(fread(&jogo, sizeof(jogo), 1, arq) != 0){
		if(chave == jogo.codigo)
			return i;
		i++;
	}
	return -1;
}
// ********************************* FUNÇÕES DEDICADAS PARA DEV: **********************************

// ______ lê a posição do desenvolvedor e retorna. ________________________________________________

struct tDesenvolvedor lerDev (int posicao, FILE *arq){
	struct tDesenvolvedor dev;
	fseek(arq, sizeof(dev)*posicao, SEEK_SET);
	fread(&dev, sizeof(dev), 1, arq);
	return dev;
}

// ______ Exclusão lógica dos dev. ________________________________________________________________

void excluirDev (FILE *arq, int posicao){
	struct tDesenvolvedor dev;
	fseek(arq, sizeof(dev)*posicao, SEEK_SET);
	dev.deletado = '*';
	dev.codigo = -1;
	fwrite(&dev, sizeof(dev), 1, arq);
}

// ______ Retorna a posição do dev. _______________________________________________________________

int retornaPosicaoD (FILE *arq, int chave){
	struct tDesenvolvedor dev;
	int i = 0;
	fseek(arq, 0, SEEK_SET);
	while(fread(&dev, sizeof(dev), 1, arq) != 0){
		if(chave == dev.codigo)
			return i;
		i++;
	}
	return -1;
}
// ********************************* FUNÇÕES DEDICADAS PARA USER: ********************************

// ______ Gera um código de acordo com a quantidade de usuarios salvos. ___________________________

int contUser (FILE *arq){
	struct tUser user;
	int i = sizeof(user);
	
	fseek(arq, -i, SEEK_END);
	
	fread(&user, sizeof(user), 1, arq);
	
	return user.codigo + 1;
}

// ______ Retorna o usuário após ler a linha de arquivo. __________________________________________

struct tUser lerUser (FILE *arq, int posicao){
	struct tUser user;
	fseek(arq, sizeof(user)*posicao, SEEK_SET);
	fread(&user, sizeof(user), 1, arq);
	
	return user;
}

// ______ Verifica a existência de login repetidos. _______________________________________________

int verifExistenciaLogin (FILE *arq, char login[]){
	struct tUser user;
	
	fseek(arq, 0, SEEK_SET);
	while(fread(&user, sizeof(user), 1, arq) != 0){
		if(strcmp(login, user.login) == 0)
			return 1;
	}
	
	return 0;
}

// ______ Retorna posição usuário através do código. ______________________________________________


int retornaPosicaoUser (FILE *arq, int chave){
	struct tUser user;
	int i = 0;
	fseek(arq, 0, SEEK_SET);
	while(fread(&user, sizeof(user), 1, arq) != 0){
		if(chave == user.codigo)
			return i;
		i++;
	}
	return -1;
}


// ______ Excluir o usuário. ______________________________________________________________________

void excluirUser (FILE *arq, int posicao){
	struct tUser user;
	fseek(arq, sizeof(user)*posicao, SEEK_SET);
	user.deletado = '*';
	fwrite(&user, sizeof(user), 1, arq);
}

// ______ Biblioteca do usuário. __________________________________________________________________

int library (FILE *arqRel, FILE *arqJogo, int codigo){
	struct tJogo jogo;
	struct tRelacoes rel;
	int i=12;
	fseek(arqRel, 0, SEEK_SET);
	gotoxy(1, 10); printf("NOME                        ");
	gotoxy(1, 11); printf("____________________________");
	while(fread(&rel, sizeof(rel), 1, arqRel) != 0){
		fseek(arqJogo, 0, SEEK_SET);
		while(fread(&jogo, sizeof(jogo), 1, arqJogo) != 0){
			if(codigo == rel.userCodigo && jogo.codigo == rel.jogoCodigo){
				gotoxy(1, i); printf("%s\n", jogo.nome);
				i++;
			}
		}
	}
}

int buscaRelJogo (FILE *arq, int codUser, int codJogo){
	struct tRelacoes rel;
	
	fseek(arq, 0, SEEK_SET);
	while(fread(&rel, sizeof(rel), 1, arq) != 0){
		if(codUser == rel.userCodigo && codJogo == rel.jogoCodigo)
			return 1;
	}	
	return 0;
}


// ********************************* Funções de exclusões fisica. *********************************

int exclusaoFisicaJogo (FILE *arqJogo, char nome[]){
	FILE *arqAux = fopen("jogoAux.dat", "wb");
	struct tJogo jogo;
	
	if(arqAux == NULL){
		printf("Erro de abertura!!!");
		return 0;
	}
	
	fseek(arqJogo, 0, SEEK_SET);
	while(fread(&jogo, sizeof(jogo), 1, arqJogo) != 0)
		if(jogo.deletado != '*')
			fwrite(&jogo, sizeof(jogo), 1, arqAux);
	
	fclose(arqJogo);
	fclose(arqAux);
	remove(nome);
	rename("jogoAux.dat", nome);
	
	arqJogo = aberturaDeArquivo(nome);
	if(arqJogo == NULL){
		printf("Perdeu arquivo 1");
		return 0;
	}
	
	return 1;
}

int exclusaoFisicaDev (FILE *arqDev, char nome[]){
	FILE *arqAux = fopen("devAux.dat", "wb");
	struct tDesenvolvedor dev;
	
	if(arqAux == NULL){
		printf("Erro de abertura!!!");
		return 0;
	}
	
	fseek(arqDev, 0, SEEK_SET);
	while(fread(&dev, sizeof(dev), 1, arqDev) != 0)
		if(dev.deletado != '*')
			fwrite(&dev, sizeof(dev), 1, arqAux);
	
	fclose(arqDev);
	fclose(arqAux);
	remove(nome);
	rename("devAux.dat", nome);
	
	arqDev = aberturaDeArquivo(nome);
	if(arqDev == NULL){
		printf("Perdeu arquivo 2");
		return 0;
	}
	
	return 1;
}

int exclusaoFisicaUser (FILE *arqUser, char nome[]){
	FILE *arqAux = fopen("userAux.dat", "wb");
	struct tUser user;
	
	if(arqAux == NULL){
		printf("Erro de abertura!!!");
		return 0;
	}
	
	fseek(arqUser, 0, SEEK_SET);
	while(fread(&user, sizeof(user), 1, arqUser) != 0)
		if(user.deletado != '*')
			fwrite(&user, sizeof(user), 1, arqAux);
	
	fclose(arqUser);
	fclose(arqAux);
	remove(nome);
	rename("userAux.dat", nome);
	
	arqUser = aberturaDeArquivo(nome);
	if(arqUser == NULL){
		printf("Perdeu arquivo 3");
		return 0;
	}
	
	return 1;
}


