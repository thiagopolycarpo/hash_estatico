#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct livro {
        char isbn[3];
        char titulo[50];
        char autor[50];
        char ano[5];
    } vet[12] = {{"03", "TiT-3", "Autor-A-1.3", "3333"},
                {"02", "TiT-2", "Autor-B", "2222"},                
                {"10", "TiT-1", "Autor-A-1.3", "1111"},                
                {"41", "Titulo-6", "Autor-A-1.3", "6666"},                
                {"55", "T-5", "Autor-B", "5555"},
                {"89", "T-8", "Autor-B", "5555"},
                {"11", "T-9", "Autor-B", "5555"},
                {"13", "T-10", "Autor-B", "5555"},
                {"51", "T-11", "Autor-B", "5555"},
                {"15", "T-12", "Autor-B", "5555"},
                {"06", "T-13", "Autor-B", "5555"},
                {"32", "T-4", "Autor-B", "5555"},
                
                };
       
    fd = fopen("biblioteca.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct buscar {
        char isbn[3];
    } vet_b[6] = {{"33"},
                  {"41"},
                  {"73"},
                  {"06"},
                  {"51"},
				  {"22"}};
       
    fd = fopen("buscar.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct remover {
        char isbn[3];
    } vet_r[6] = {{"02"},
                  {"43"},
                  {"41"},
                  {"15"},
                  {"06"},
				  {"36"}};
       
    fd = fopen("remover.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}
