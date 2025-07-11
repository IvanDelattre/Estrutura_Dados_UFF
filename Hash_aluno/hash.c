#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANHO_TABELA 20011

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
 

int main(){
	
	FILE *arq = fopen("hash.dat", "w+b");
	int i;
    for ( i = 0; i < TAMANHO_TABELA ; i++) {
		Aluno aluno;
		strcpy(aluno.nome, " ");
		strcpy(aluno.cpf, " ");
		aluno.nota = 0.0;
		aluno.ocupado = 0;
		fwrite(&aluno, sizeof(Aluno), 1, arq);
	}
	
	
	Hash *h = (Hash*) malloc(sizeof(Hash)) ; 
	h->arq = arq; 
	povoar_tabela(h , "alunos.dat") ;
	
	
	imprimir_hash(h) ; 
	
	
	int condition = 1 ; 
	while(condition){
		
		printf("Digitar operacao: 1 - inserir , 2 - buscar , 3 - remover  , 4 - imprimir , sair - 0: ") ; 
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
		        if(aluno) printf("\nAtributos do aluno:  %s / %s / %f\n\n" , aluno->nome , aluno->cpf , aluno->nota) ; 
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
				if( remover_hash(h , cpf) ) printf("\nAluno removido com sucesso!\n") ; 
				
				
				break;
			}
			
			default:
		        printf("Opção inválida.\n");
				
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

    return rand() % TAMANHO_TABELA ;
}


Aluno *buscar_hash(Hash *h, char *cpf) {
    int k = 0;
    int pos;
    Aluno *aluno;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, k);  // posição tentativa
        aluno = ler_arquivo(h, pos);

        if (aluno->ocupado == 0) {
            // Nunca houve registro aqui: CPF não está na tabela
            free(aluno);
            return NULL;
        }

        if (aluno->ocupado == 1 && strncmp(aluno->cpf, cpf, 11) == 0) {
            // Encontrou o aluno com CPF correspondente
            return aluno;
        }

        free(aluno);  // não é o que buscamos
        k++;  // próxima tentativa
    }

    // Tabela cheia e CPF não encontrado
    return NULL;
}


void inserir_hash(Hash *h, char *cpf , char*nome , double nota ) {
    int k = 0;
    int pos;

    while (k < TAMANHO_TABELA) {
        pos = hash(cpf, k);
        Aluno *a = ler_arquivo(h, pos);

        if (a->ocupado == 1 && strncmp(a->cpf, cpf, 11) == 0) {
            //printf("CPF já existente. Inserção cancelada.\n");
            free(a);
            return;
        }

        if (a->ocupado == 0 || a->ocupado == 2) {
            // Inserir aqui diretamente
            Aluno novo;
            strncpy(novo.nome, nome, 50);
            strncpy(novo.cpf, cpf, 12);
            novo.nota = nota;
            novo.ocupado = 1;

            escrever_arquivo(h, pos, &novo);
            
            //printf("Aluno inserido na posicao %d\n", pos);
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
            // Posição nunca ocupada -> CPF não está na tabela
            free(a);
            return 0; // Falha ao remover
        }

        if (a->ocupado == 1 && strncmp(a->cpf, cpf, 11) == 0) {
            a->ocupado = 2;  // Marca como deletado
            escrever_arquivo(h, pos, a);
            free(a);
            return 1; // Remoção bem-sucedida
        }

        free(a);
        k++;
    }

    return 0; // Não encontrado
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


