#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct heap {
	FILE *arq;
	int qant_registros ;
	char nome_arquivo[100] ;
} Heap;

typedef struct aluno{
	char nome[50];
	double nota;
	char cpf[11];
}Aluno;

Aluno *cria_aluno(char *nome , char *cpf , double nota) ; 
void inicializa_heap(Heap *h, char *nome_arquivo) ; 
int pai(int i) ; 
int esq(int i) ; 
int dir(int i) ; 
Aluno *ler_arquivo(Heap *h, int pos  ) ; 
void escrever_arquivo(Heap *h , int pos , Aluno *a );
void subir(Heap *h , int i) ; 
void insere_heap(Heap *h, char *nome , char *cpf , double nota  ) ;
void imprimir_heap(Heap *h) ; 
void descer(Heap *h, int i) ; 
Aluno* remover_max(Heap *h) ; 

int main(){
    Heap *h = (Heap *)malloc(sizeof(Heap));
    inicializa_heap(h, "outro.bin");

    
    Aluno *a = (Aluno *)malloc(sizeof(Aluno));
 
	int condition = 1 ;
	while(condition){
		
		printf("Digitar operacao: 1 - inserir , 2 - imprimir , 3 - Retirar Maior Prioridade, sair - 0 : ") ; 
		scanf("%d" , &condition) ; 
		
		switch (condition) {
		
		    case 1: {
		        char nome[50];
		        char cpf[11];
		        double nota;
		
		        printf("Digite nome (sem espaco) : ");
		        scanf("%s", nome);
		        printf("Digite cpf : ");
		        scanf("%s", cpf);
		        printf("Digite nota : ");
		        scanf("%lf", &nota);
		
		        insere_heap(h, nome, cpf, nota);
		        break;
		    }
		
		    case 2:
		        imprimir_heap(h);
		        break;
		
		    case 3: {
		        Aluno *removido = remover_max(h);
		        if (removido) {
		            printf("\n\nAluno Removido com sucesso\n");
		            printf("Propriedades do aluno removido: %s / %s / %.2lf\n\n", removido->nome, removido->cpf, removido->nota);
		            free(removido);
		        } else {
		            printf("\nNenhum aluno na Heap\n\n");
		        }
		        break;
		    }
		    
		    
		
		    case 0:
		        break;
		
		    default:
		        printf("Opção inválida.\n");
		}
		
		
		
	}	
	

	fclose(h->arq);
	free(h); 
 
 
    return 0;
}

Aluno *cria_aluno(char *nome , char *cpf , double nota){
	
	Aluno *new = (Aluno*) malloc(sizeof(Aluno)) ; 
	strcpy( new->nome , nome  ) ; 
	strcpy( new->cpf , cpf  ) ; 
	new->nota = nota;
	return new ; 
	
}

void inicializa_heap(Heap *h, char *nome_arquivo) {
	FILE *arq = fopen(nome_arquivo, "w+b");
	h->arq = arq;
	
	h->qant_registros = 1;
	strcpy(h->nome_arquivo, nome_arquivo);
	
	
	int num = -1 ;
	
	Aluno *inicio = cria_aluno("-", "-" , -1) ; 
	
	fwrite(inicio , sizeof(Aluno) , 1 , h->arq ) ; 
	rewind(h->arq);
}




Aluno *ler_arquivo(Heap *h, int pos  ) {
	
	Aluno *aluno = (Aluno *)malloc(sizeof(Aluno));
	fseek(h->arq , pos*sizeof(Aluno) , SEEK_SET ) ; 
	fread(aluno , sizeof(Aluno) , 1 , h->arq) ; 
	rewind(h->arq) ; 
	return aluno ; 
}




void escrever_arquivo(Heap *h , int pos , Aluno *a ) {
	fseek(h->arq , pos * sizeof(Aluno) , SEEK_SET ) ; 
	fwrite(a , sizeof(Aluno) , 1 , h->arq ) ; 
	rewind(h->arq);
}


void subir(Heap *h , int i){
	
	int j = pai(i) ; 
	
	if( j >=1) {
			
		Aluno *filho =  ler_arquivo(h , i ) ;  
		Aluno *pai_no =  ler_arquivo(h , j ) ;  
		
		
		if(  filho->nota > pai_no->nota  ){
			escrever_arquivo(h, i , pai_no   ) ; 
			escrever_arquivo(h , j , filho  ) ; 
			subir(h , j  ) ; 
		}
		
		
		
	}
	
}




void insere_heap(Heap *h, char *nome , char *cpf , double nota  ) {
    
    Aluno *aluno = cria_aluno(nome , cpf , nota) ; 
    
	escrever_arquivo(h , h->qant_registros   , aluno );
    subir(h , h->qant_registros  ) ; 
	h->qant_registros++;
    
}

Aluno* remover_max(Heap *h) {
    if (h->qant_registros <= 1) {
        return NULL; // heap vazia (posição 0 é dummy)
    }

    Aluno *max = ler_arquivo(h, 1); // raiz
    Aluno *ultimo = ler_arquivo(h, h->qant_registros - 1);

    // Substitui a raiz pelo último elemento
    escrever_arquivo(h, 1, ultimo);

    h->qant_registros--; // reduz o tamanho da heap

    descer(h, 1); // reorganiza a heap

    free(ultimo); // liberamos o que foi copiado
    return max;
}

void descer(Heap *h, int i) {
    int e = esq(i);
    int d = dir(i);
    int maior = i;

    Aluno *a_i = ler_arquivo(h, i);
    Aluno *a_e = NULL;
    Aluno *a_d = NULL;

    if (e < h->qant_registros) {
        a_e = ler_arquivo(h, e);
        if (a_e->nota > a_i->nota) {
            maior = e;
        }
    }

    if (d < h->qant_registros) {
        a_d = ler_arquivo(h, d);
        Aluno *a_maior = (maior == i) ? a_i : a_e;
        if (a_d->nota > a_maior->nota) {
            maior = d;
        }
        free(a_d);
    }

    if (maior != i) {
        Aluno *a_maior = ler_arquivo(h, maior);
        escrever_arquivo(h, i, a_maior);
        escrever_arquivo(h, maior, a_i);
        free(a_maior);
        free(a_i);
        if (a_e) free(a_e);
        descer(h, maior);
    } else {
        free(a_i);
        if (a_e) free(a_e);
    }
}



void imprimir_heap(Heap *h) {
    int i;
	int numero;
    Aluno *a = (Aluno *)malloc(sizeof(Aluno));
	rewind(h->arq); 
	printf("\n") ; 
	for(i = 0 ; i <  h->qant_registros ; i++ ){
        if(i == 0 ) continue ; 
		fseek(h->arq, i * sizeof(Aluno), SEEK_SET);
		fread(a, sizeof(Aluno), 1, h->arq) ; 
		printf("Aluno : %s / cpf : %s / nota : %.2lf \n", a->nome , a->cpf , a->nota );
    }
	printf("\n") ; 
	free(a) ; 
}




int pai(int i){
return (i/2);
}

int esq(int i){
return (i*2);
}

int dir(int i){
return (i*2+1);
}




