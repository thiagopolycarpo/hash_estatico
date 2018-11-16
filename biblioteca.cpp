#include<stdio.h>
#include<conio.h>
#include<string.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct livro {
        char isbn[2];
        char titulo[50];
        char autor[50];
        char ano[5];
    } vet[5] = {{"3", "TiT-3", "Autor-A-1.3", "3333"},
                {"2", "TiT-2", "Autor-B", "2222"},                
                {"1", "TiT-1", "Autor-A-1.3", "1111"},                
                {"6", "Titulo-6", "Autor-A-1.3", "6666"},                
                {"5", "T-5", "Autor-B", "5555"},
                
                };
       
    fd = fopen("biblioteca.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct buscar {
        char isbn[2];
    } vet_b[3] = {{"3"},
                  {"2"},
                  {"7"}};
       
    fd = fopen("buscar.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct remover {
        char isbn[2];
    } vet_r[3] = {{"6"},
                  {"3"},
                  {"7"}};
       
    fd = fopen("remover.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}
