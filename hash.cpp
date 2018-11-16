/* Desenvolvido por Bruno Domene e Thiago Polycarpo*/

/* bibliotecas */
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

/* Defines */

#define TAM_STRUCT 100
#define TAM_HASH 31 

/* variaveis globais */

FILE *arq_hash; /* aponta pro arquivo hash.bin */
FILE *arq;		/* aponta pro arquivo livros.bin*/
int cont_inserir=0, cont_remover=0, cont_buscar=0;
char atualizar[] = "ab", leitura[] = "rb+", escrever[] = "wb";

/* Structs */
struct hash{
	char isbn[2];
	int rrn;
};

struct livro {
        char isbn[2];
        char titulo[50];
        char autor[50];
        char ano[5];
}arq_livros[TAM_STRUCT];

#define TAM_REGISTRO sizeof(struct livro) 

struct buscar {
        char isbn[2];
}arq_buscar[TAM_STRUCT];

struct remover {
        char isbn[2];
}arq_remover[TAM_STRUCT]; 

/* prototipos */
int inserir_arq_principal();
int criar_arquivo(char nome_arq[]);
int abrir_arquivo(char nome_arq[], char tipo_abertura[]);
int divisao_inteira(char chave[]);
void carregar_arquivo();
void inicializar_hash();
void fechar_arquivo(FILE **p_arq);
void inserir_indice(int posicao_hash, int offset, char chave[]);

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
	  			//buscar
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
	char nome_arq[]="livros.bin", chave[2];
	int cont, posicao_hash, offset_livros;
	
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
		fseek(arq,8,0);
		fwrite(&arq_livros[cont],TAM_REGISTRO,1,arq);
		fseek(arq,-sizeof(struct livro),1);
		offset_livros = ftell(arq) + sizeof(struct livro);
		fclose(arq);
		inserir_indice(posicao_hash, offset_livros, chave);
		printf("\n Chave %s inserida com sucesso  ",chave);
		
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
	int soma;
	soma = (chave[0] % TAM_HASH) + (chave[1] % TAM_HASH); // usar espaco tbm na soma???
	printf("\n Endereco %d para chave %s",soma,chave);
	getch();
	return soma;
}

void inserir_indice(int posicao_hash, int offset, char chave[]){
	char nome_arq[]= "hash.bin";
	int pos;
	
	if(!abrir_arquivo(nome_arq,leitura)){
		arq_hash = fopen("hash.bin","wb");
		inicializar_hash();
	}
	pos = posicao_hash * sizeof(struct hash);
	fseek(arq_hash,pos,0);
	fwrite(chave,sizeof(chave),1,arq_hash);
	fwrite(&offset,sizeof(int),1,arq_hash);
	fechar_arquivo(&arq_hash);
	printf("\n Inserirdo no arquivo hash");
}

void inicializar_hash(){
	hash *hashpage;
	strcpy(hashpage->isbn,"@");
	hashpage->rrn=-1;
	fwrite(&hashpage, sizeof(struct hash), 31,arq_hash);
	printf("\nArquivo de Hash Inicializado!");
}


