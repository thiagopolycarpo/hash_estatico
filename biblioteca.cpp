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
    } vet[5] = {{"03", "TiT-3", "Autor-A-1.3", "3333"},
                {"02", "TiT-2", "Autor-B", "2222"},                
                {"10", "TiT-1", "Autor-A-1.3", "1111"},                
                {"60", "Titulo-6", "Autor-A-1.3", "6666"},                
                {"55", "T-5", "Autor-B", "5555"},
                
                };
       
    fd = fopen("biblioteca.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct buscar {
        char isbn[3];
    } vet_b[3] = {{"33"},
                  {"22"},
                  {"73"}};
       
    fd = fopen("buscar.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct remover {
        char isbn[3];
    } vet_r[3] = {{"36"},
                  {"43"},
                  {"67"}};
       
    fd = fopen("remover.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}
