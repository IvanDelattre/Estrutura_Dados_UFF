#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANHO_TABELA 100019

typedef struct hash {
	FILE *arq;
	int qant_registros ;
	char nome_arquivo[100] ;
} Hash;


typedef struct aluno{
	char nome[50];
	double nota;
	char cpf[12];
	int ocupado ; // definindo [0 = nunca usado/  1 = ocupado/ 2 = deletado ]
}Aluno;


int hash(char *cpf, int k  );
int hash_rand(char *cpf); 
Aluno *buscar_hash(Hash *h, char *cpf) ; 
void inserir_hash(Hash *h, char *cpf , char*nome , double nota ) ; 
void imprimir_hash(Hash *h) ;
Aluno *ler_arquivo(Hash *h, int pos  ) ;
void escrever_arquivo(Hash *h , int pos , Aluno *a ) ; 
void povoar_tabela(Hash *h, char *arquivo) ;
int remover_hash(Hash *h, char *cpf) ; 
void inicializa_hash(Hash *h, char *arquivo  ) ; 

int main(){
	Hash *h = (Hash*) malloc(sizeof(Hash)) ; 
	inicializa_hash(h , "hash.dat") ; 
	
	
	int condition = 1 ; 
	while(condition){
		
		printf("\n====== MENU HASH ======\n");
		printf("  1 - Inserir registro\n");
		printf("  2 - Buscar registro\n");
		printf("  3 - Remover registro\n");
		printf("  4 - Imprimir tabela\n");
		printf("  0 - Sair\n");
		printf("========================\n");
		printf("Escolha uma opcao: ");
 		
		scanf("%d" , &condition) ; 
		
		switch (condition) {
			case 1: {
		        char nome[50];
		        char cpf[12];
		        double nota;
		
		        printf("Digite nome (sem espaco) : ");
		        scanf("%s", nome);
		        printf("Digite cpf : ");
		        scanf("%s", cpf);
		        printf("Digite nota : ");
		        scanf("%lf", &nota);
		
		        inserir_hash(h, cpf, nome, nota);
		        printf("\nInsercao bem sucedida\n") ; 
		        
		        break;
			}
			
			case 2 :{
				char cpf[12];
				printf("Digite cpf : ");
				scanf("%s", cpf);
		        Aluno *aluno = buscar_hash(h , cpf) ; 
		        if(aluno) printf("\nAtributos do aluno:  %s / %s / %.2f\n\n" , aluno->nome , aluno->cpf , aluno->nota) ; 
				if(!aluno) printf("Registro nao encontrado\n\n") ; 
				
				break;
			}
			
			case 4 :{
				imprimir_hash(h) ; 
				break;
			}
			
			case 3 : {
				char cpf[12] ; 
				printf("\nDigite cpf : ") ; 
				scanf("%s", cpf ) ; 
				if( remover_hash(h , cpf) ){
					printf("\nAluno removido com sucesso!\n") ;	
				}else{
					printf("\n\nAluno nao encontrado\n\n") ; 
				}  
				
				
				break;
			}
			
			case 0 : {
				
				break;
			}
			
			
			default:
		        printf("Op��o inv�lida.\n");
				
		}		
			
			
	}	
	
	
	return 0;
}



int hash(char *cpf, int k  ) {
	
	return  (hash_rand(cpf) + k ) % TAMANHO_TABELA ; 
	
}





int hash_rand(char *cpf) {
    char cpf_seed_str[10]; 
    strncpy(cpf_seed_str, cpf, 9);
    cpf_seed_str[9] = '\0';  

    int seed = atoi(cpf_seed_str); 
    srand(seed);
	
	
    return ( (rand()*3) + 10000) % TAMANHO_TABELA ;
}




Aluno *buscar_hash(Hash *h, char *cpf) {
    int k = 0;
    int pos;
    Aluno *aluno;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, k);  
        aluno = ler_arquivo(h, pos);

        if (aluno->ocupado == 0) {
            
            free(aluno);
            return NULL;
        }

        if (aluno->ocupado == 1 && strncmp(aluno->cpf, cpf, 11) == 0) {
            
            return aluno;
        }

        free(aluno);  
        k++;  
    }

    
    return NULL;
}


void inserir_hash(Hash *h, char *cpf , char*nome , double nota ) {
    int k = 0;
    int pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, k);
        Aluno *a = ler_arquivo(h, pos);

        if (a->ocupado == 1 && strncmp(a->cpf, cpf, 11) == 0) {
            
            free(a);
            return;
        }

        if (a->ocupado == 0 || a->ocupado == 2) {
            
            Aluno novo;
            strncpy(novo.nome, nome, 50);
            strncpy(novo.cpf, cpf, 12);
            novo.nota = nota;
            novo.ocupado = 1;

            escrever_arquivo(h, pos, &novo);
            
            
            free(a);
            return;
        }

        free(a);
        k++;
    }

    printf("Hash cheia. Insercao falhou.\n");
}



int remover_hash(Hash *h, char *cpf) {
    int k = 0;
    int pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, k);
        Aluno *a = ler_arquivo(h, pos);

        if (a->ocupado == 0) {
            free(a);
            return 0; // Falha ao remover
        }

        if (a->ocupado == 1 && strncmp(a->cpf, cpf, 11) == 0) {
            a->ocupado = 2;  // Marca como deletado
            escrever_arquivo(h, pos, a);
            free(a);
            return 1; // Remo��o bem-sucedida
        }

        free(a);
        k++;
    }

    return 0; // N�o encontrado
}








void imprimir_hash(Hash *h) {
    int i;
	int numero;
    Aluno *a = (Aluno *)malloc(sizeof(Aluno));
	rewind(h->arq); 
	printf("\n") ; 
	for(i = 0 ; i <  TAMANHO_TABELA ; i++ ){
        
		fseek(h->arq, i * sizeof(Aluno), SEEK_SET);
		fread(a, sizeof(Aluno), 1, h->arq) ; 
		printf("Aluno %d : %s / cpf : %s / nota : %.2f / oupdado : %d \n", i ,a->nome , a->cpf , a->nota, a->ocupado );
    }
	printf("\n") ; 
	free(a) ; 
}



Aluno *ler_arquivo(Hash *h, int pos  ) {
	Aluno *aluno = (Aluno *)malloc(sizeof(Aluno));
	fseek(h->arq , pos*sizeof(Aluno) , SEEK_SET ) ; 
	fread(aluno , sizeof(Aluno) , 1 , h->arq) ; 
	rewind(h->arq) ; 
	return aluno ; 
}



void escrever_arquivo(Hash *h , int pos , Aluno *a ) {
	fseek(h->arq , pos * sizeof(Aluno) , SEEK_SET ) ; 
	fwrite(a , sizeof(Aluno) , 1 , h->arq ) ; 
	rewind(h->arq);
}



void povoar_tabela(Hash *h, char *arquivo){
	
	FILE *arq = fopen(arquivo, "rb");
	int i ; 
	Aluno *a = (Aluno *)malloc(sizeof(Aluno));
	for ( i = 0; i < TAMANHO_TABELA ; i++) {
		fseek(h->arq, i * sizeof(Aluno), SEEK_SET);
		fread(a, sizeof(Aluno), 1, arq) ; 
		//printf("Aluno %d : %s / cpf : %s / nota : %.2f \n", i ,a->nome , a->cpf , a->nota );
		inserir_hash(h , a->cpf , a->nome , a->nota  ) ; 
				
		
	}
	
	
}



void inicializa_hash(Hash *h, char *arquivo  ) {
	
	FILE *arq = fopen(arquivo, "r+b");  
	
	if (arq == NULL) {
        
        arq = fopen(arquivo, "w+b");
        h->arq = arq ; 
        int i ; 
		for ( i = 0; i < TAMANHO_TABELA ; i++) {
			Aluno aluno;
			strcpy(aluno.nome, " ");
			strcpy(aluno.cpf, " ");
			aluno.nota = 0.0;
			aluno.ocupado = 0;
			fwrite(&aluno, sizeof(Aluno), 1, arq);
		}
		povoar_tabela(h , "alunos.dat") ;
		strcpy(h->nome_arquivo, arquivo);
    	fseek(arq, 0, SEEK_END);
    	h->qant_registros = ftell(arq) / sizeof(Aluno);
    	rewind(arq);
    	return ; 
    }

	h->arq = arq ; 
	strcpy(h->nome_arquivo, arquivo);
    fseek(arq, 0, SEEK_END);
    h->qant_registros = ftell(arq) / sizeof(Aluno);
    rewind(arq);	
	
}


