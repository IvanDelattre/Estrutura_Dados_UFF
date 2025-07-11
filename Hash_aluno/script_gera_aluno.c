#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct aluno{
	char nome[50];
	double nota;
	char cpf[12];
	int ocupado ; 
}Aluno;

void gerar_alunos( const char *arquivo_saida) ; 


int main(){
	
	
	gerar_alunos("alunos.dat") ; 
	
	return 0 ; 
}


void gerar_alunos(const char *arquivo_saida) {
    const char *nomes_base[6] = {"Halbert", "Joseph", "Bart", "William", "Cesar", "Sol"};
    FILE *out = fopen(arquivo_saida, "wb");
    
	int i , j , k , l , m , n ; 
    int total = 0;
    for (i = 0; i < 6 && total < 10000; i++) {
        for (j = 0; j < 6 && total < 10000; j++) {
            for (k = 0; k < 6 && total < 10000; k++) {
                for (l = 0; l < 6 && total < 10000; l++) {
                    for (m = 0; m < 6 && total < 10000; m++) {
                        for (n = 0; n < 6 && total < 10000; n++) {

                            char nome[100];
                            snprintf(nome, sizeof(nome), "%s %s %s %s %s %s",
                                     nomes_base[i], nomes_base[j], nomes_base[k],
                                     nomes_base[l], nomes_base[m], nomes_base[n]);

                            char cpf[12];
                            int c ; 
							for (c = 0; c < 11; c++) {
                                cpf[c] = '0' + (rand() % 10);
                            }
                            cpf[11] = '\0';

                            double nota = ((double)(rand() % 10000)) / 1000.0;

                            // Cria e escreve no arquivo de saída
                            Aluno aluno;
                            strcpy(aluno.nome, nome);
                            strcpy(aluno.cpf, cpf);
                            aluno.nota = nota;
							aluno.ocupado = 1;
                            fwrite(&aluno, sizeof(Aluno), 1, out);


                            total++;
                        }
                    }
                }
            }
        }
    }

    fclose(out);
    printf("%d alunos gerados com sucesso.\n", total);
}

