/* Desenvolvido por Bruno Domene e Thiago Polycarpo*/

/* bibliotecas */
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

/* Defines */

#define TAM_STRUCT 100
#define TAM_HASH 31
#define VAZIO "@@"
#define NULO -1 

/* variaveis globais */

FILE *arq_hash; /* aponta pro arquivo hash.bin */
FILE *arq;		/* aponta pro arquivo livros.bin*/
int cont_inserir=0, cont_remover=0, cont_buscar=0;
char atualizar[] = "ab", leitura[] = "rb+", escrever[] = "wb";

/* Structs */
struct hash{
	char isbn[3];
	int rrn;
}tabela_hash[TAM_HASH];

struct livro {
        char isbn[3];
        char titulo[50];
        char autor[50];
        char ano[5];
}arq_livros[TAM_STRUCT];

#define TAM_REGISTRO sizeof(struct livro) 

struct buscar {
        char isbn[3];
}arq_buscar[TAM_STRUCT];

struct remover {
        char isbn[3];
}arq_remover[TAM_STRUCT]; 

/* prototipos */
int inserir_arq_principal();
int criar_arquivo(char nome_arq[]);
int abrir_arquivo(char nome_arq[], char tipo_abertura[]);
int divisao_inteira(char chave[]);
int colisao(int posicao_hash, int *nova_posicao);
void carregar_arquivo();
int buscar();
void inicializar_hash();
void fechar_arquivo(FILE **p_arq);
void inserir_indice(int posicao_hash, int rrn, char chave[]);

/* funcoes */
int main(){
	int resp, sair = 0;
	do{ 
		system("cls");
	 	printf("\n        Menu");
	  	printf("\n1 - Inserir");
	  	printf("\n2 - Remover");
	  	printf("\n3 - Buscar");
	  	printf("\n4 - Carregar Arquivos");
	  	printf("\n5 - Sair ");
	  	printf("\nOpcao: ");
	  	scanf("%d",&resp);
	
		switch(resp){
			case 1:{
				inserir_arq_principal();
				break;
			}
		  	case 2:{
		  		//remover
				break;
			}
		  	case 3: {
	  			buscar();
	  			break;
	  		}
	  		case 4:{
		  		carregar_arquivo(); 
			    printf("\nArquivos Carregados\n");
			    system("pause");
				break;
			}
	  		case 5:{
		    	sair = 1;
				break;
			}
		  	default:{
			    printf("\nOpcao invalida!");
				break;
			} 
		}
	}while(sair != 1);
}

int abrir_arquivo(char nome_arq[], char tipo_abertura[]) { 
	if(strcmp(nome_arq, "hash.bin") == 0){
		if((arq_hash = fopen(nome_arq, tipo_abertura)) == NULL)
			return 0;
	    return 1;
	}
	if((arq = fopen(nome_arq, tipo_abertura)) == NULL)
			return 0;
	return 1;	 
}  

void fechar_arquivo(FILE **p_arq){ 
	fclose(*p_arq); 
}

int criar_arquivo(char nome_arq[]){
	int cont = 0;
	if((arq = fopen(nome_arq,escrever)) == NULL){
	    printf("\nImpossivel criar o arquivo");
	    system("pause");
	    return 0;
  	}
	fwrite(&cont, sizeof(int), 1, arq);  //contador de inserção 0 
	fwrite(&cont, sizeof(int), 1, arq);  //contador de remocao  4 
	fwrite(&cont, sizeof(int), 1, arq);  //contador de bucas    8
	fclose(arq);
	printf("\nArquivo Criado");
}

void carregar_arquivo(){
	char cadastro_arq[]="biblioteca.bin", busca_arq[]="buscar.bin", remove_arq[]= "remover.bin";
  	int i, todos=0;
  
  	system("cls");
  	/* abre arquivo biblioteca, carrega em vetor de struct */
  	if(abrir_arquivo(cadastro_arq, leitura)){
	  		i=0;
	    	while(fread(&arq_livros[i], sizeof(struct livro), 1, arq)){
	    		printf("\nIsbn: %s - Titulo: %s - Autor: %s - Ano: %s", arq_livros[i].isbn, arq_livros[i].titulo, 
						arq_livros[i].autor, arq_livros[i].ano);
	      		i++;  
	    	}
	    fechar_arquivo(&arq);
		cont_inserir = i;  
	}
  	
  	/* abre arquivo buscar.bin, carrega em vetor de struct */
  	printf("\n\ndados da busca:");
  	if(abrir_arquivo(busca_arq, leitura)){
		    i=0;
			while(fread(&arq_buscar[i], sizeof(struct buscar), 1, arq)){
				printf("\nIsbn buscar: %s", arq_buscar[i].isbn);
				i++;
			}
			cont_buscar= i;
			fechar_arquivo(&arq);			
	} 
  	/* abre arquivo remover.bin, carrega em vetor de struct */
  	printf("\n\nDados remover");
  	if(abrir_arquivo(remove_arq, leitura)){
	    	i=0;
			while(fread(&arq_remover[i], sizeof(struct remover), 1, arq)){
			  printf("\nIsbn remover: %s", arq_remover[i].isbn);
			  i++;
			}
			cont_remover = i;
			fechar_arquivo(&arq);
	}
} 

int inserir_arq_principal(){
	char nome_arq[]="livros.bin", chave[3], hash_arq[]="hash.bin";
	int cont, posicao_hash, rrn_livros, nova_posicao=-1, tentativa =0, posicao_inicial, nao_achou;
	
	system("cls");
	
	if(!abrir_arquivo(nome_arq,leitura)){
		criar_arquivo(nome_arq);
	}
	abrir_arquivo(nome_arq,leitura);
	fread(&cont, sizeof(int), 1, arq);
	printf("\nContador insercao: %d", cont);
	fclose(arq);
	
	if(cont < cont_inserir){
		if(!abrir_arquivo(nome_arq, atualizar)){
			printf("\nErro, arquivo nao pode ser aberto");
			getch();
			return 0;
		}
		strcpy(chave, arq_livros[cont].isbn);
		posicao_hash = divisao_inteira(chave);
		
		if(cont > 0){
			abrir_arquivo(hash_arq,leitura);
			posicao_inicial = posicao_hash;
			nao_achou = colisao(posicao_hash, &nova_posicao);  
			fclose(arq_hash);
		}else{
		 	nao_achou =1;
		}
		if(nao_achou){
			fseek(arq,12,0);
			fwrite(&arq_livros[cont],TAM_REGISTRO,1,arq);
			fseek(arq,-sizeof(struct livro),1);
			rrn_livros = ftell(arq);
			fclose(arq);
			inserir_indice(nova_posicao, rrn_livros, chave);
			printf("\n Chave %s inserida com sucesso  ",chave);
		}else{
			printf("\nNão tem mais espaço para insercao na tabela hash");
		}
		
	
		
		//atualizando cont insercao
		abrir_arquivo(nome_arq,"rt+");
		fseek(arq, 0, 0);
		cont++;
		fwrite(&cont, sizeof(int), 1, arq);
		fclose(arq);
	}else{
		printf("\nNao ha mais livros a serem inseridos\n");
	}	
	getch();
}

int divisao_inteira(char chave[]){
   int i, soma = chave[0];
   for (i = 1; chave[i] != '\0'; i++){
      soma = (soma * 31 + chave[i]) % TAM_HASH;
   }
   printf("\n Endereco %d",soma);
   return soma;
}

void inserir_indice(int posicao_hash, int rrn, char chave[]){
	char nome_arq[]= "hash.bin";
	int pos;
	
	if(!abrir_arquivo(nome_arq,leitura)){
		arq_hash = fopen("hash.bin","wb");
		inicializar_hash();
	}
	strcpy(tabela_hash[0].isbn,chave);
	tabela_hash[0].rrn;
	pos = posicao_hash * sizeof(struct hash);
	fseek(arq_hash,pos,0);
	fwrite(&tabela_hash[0],sizeof(struct hash),1,arq_hash);
	fechar_arquivo(&arq_hash);
	printf("\n Inserirdo no arquivo hash");
}

void inicializar_hash(){
	int i;
	for(i=0;i<TAM_HASH;i++){
		strcpy(tabela_hash[i].isbn,VAZIO);
		tabela_hash[i].rrn=NULO;
		fwrite(&tabela_hash[i],1,sizeof(struct hash),arq_hash);
		printf("\n registro %d -> %s - %d",i,tabela_hash[i].isbn,tabela_hash[i].rrn);
	}	
	printf("\nArquivo de Hash Inicializado!");
		
}

int colisao(int posicao_hash, int *nova_posicao){
	int i=0, tentativa =0, posicao_inicial;
	char chave_teste[3];
	strcpy(chave_teste,VAZIO);
	posicao_inicial = posicao_hash - 1;
	
	for(i=0;i<31;i++){
		fseek(arq_hash,posicao_hash*sizeof(struct hash),0);
		fread(&tabela_hash[0],sizeof(struct hash), 1,arq_hash);
		if(strcmp(tabela_hash[0].isbn,chave_teste) == 0){
			*nova_posicao = posicao_hash;
			return 1; /* não tem colisão */
		}
		tentativa++;
		printf("\nColisao\nTentativa %d",tentativa);
		*nova_posicao = posicao_hash + 1;
		printf("\nNovaPosicao %d",*nova_posicao);
		posicao_hash = *nova_posicao;
	}
	return 0; /* não tem mais espaço na tabela hash */	
}

int buscar(){
	char nome_arq[]="livros.bin", chave[3], hash_arq[]="hash.bin";
	int cont, posicao_hash, acesso=1,i;
	
	system("cls");
	
	if(!abrir_arquivo(nome_arq,leitura)){
		printf("\nImpossivel abrir arquivo");
		return 0;
	}
	fseek(arq,8,0);
	fread(&cont, sizeof(int), 1, arq);
	printf("\nContador busca: %d", cont);
	
	if(cont < cont_buscar){
		strcpy(chave,arq_buscar[cont].isbn);
		posicao_hash = divisao_inteira(chave);
		
		abrir_arquivo(hash_arq,leitura);
		for(i=0;i<31;i++){
			fseek(arq_hash,posicao_hash*sizeof(struct hash),0);
			fread(&tabela_hash[0],sizeof(struct hash), 1,arq_hash);
			if(strcmp(tabela_hash[0].isbn,chave) == 0){
				fseek(arq,tabela_hash[0].rrn*sizeof(struct livro),0);
				fread(&arq_livros[0],sizeof(struct livro),1,arq);
				printf("\nChave %s encontrada, endereco %d, %d acesso(s) ",chave, posicao_hash,acesso);
				printf("\nISBN: %s \nTitulo: %s \nAutor: %s \nAno: %s",arq_livros[0].isbn,
					   arq_livros[0].titulo,arq_livros[0].autor,arq_livros[0].titulo);
				fclose(arq);
				fclose(arq_hash);
				break; /* arq achado em uma unica posicao */
			}
			posicao_hash = posicao_hash + 1;
        	acesso = acesso +1;
		}
		if(i == 31){
			printf("\nChave %s nao encontrada",chave);
		}
		fclose(arq_hash);
		//atualizando cont insercao
		abrir_arquivo(nome_arq,"rt+");
		fseek(arq, 8, 0);
		cont++;
		fwrite(&cont, sizeof(int), 1, arq);
		fclose(arq);
	}else{
		printf("\nNao ha mais livros a serem buscados\n");
	}	
	getch();
	return 1;
}


