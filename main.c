#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipo de dado da Árvore
typedef char Tipo[100];

// No da Árvore
struct No
{
    Tipo info;
    struct No *esq;
    struct No *dir;
    int altura;
};


int max(int a, int b) //retorna o maior de dois inteiros
{
    if(a > b)
        return a;
    else
        return b;
}

int altura(struct No *no) //retorna a altura da arvore
{
    if (no == NULL)
        return -1;

    return no->altura;
}



struct No* novoNo(Tipo elem)//aloca um novo no com a chave dada e com ponteiros esquerdo e direito nulos
{
    struct No* no = (struct No*)
                        malloc(sizeof(struct No));
    strcpy(no->info, elem); //no->info   = elem;
    no->esq    = NULL;
    no->dir    = NULL;
    no->altura = 0;  //novo no inicialmente eh uma folha

    return(no);
}


struct No *rotacaoDireita(struct No *y) //realiza rotacao a direita
{
    struct No *x  = y->esq;
    struct No *T2 = x->dir;

    // rotacao
    x->dir = y;
    y->esq = T2;

    // atualizar alturas
    y->altura = max(altura(y->esq), altura(y->dir));
    x->altura = max(altura(x->esq), altura(x->dir));

    // retorna nova raiz
    return x;
}

struct No *rotacaoEsquerda(struct No *x) //realiza rotacao a esquerda
{
    struct No *y  = x->dir;
    struct No *T2 = y->esq;

    // rotacao
    y->esq = x;
    x->dir = T2;

    //  atualiza alturas
    x->altura = max(altura(x->esq), altura(x->dir));
    y->altura = max(altura(y->esq), altura(y->dir));

    // retorna nova raiz
    return y;
}


int retornaBalanco(struct No *N) //retorna o fator de balanceamento do no N
{
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}


struct No* insere(struct No* no, Tipo chave) //funcao recursiva para inserir uma chave na subarvore de raiz no e retorna a nova raiz da subarvore
{
    //Passo 1: Insercao normal de arvore binaria de busca
    if (no == NULL)
        return(novoNo(chave));

    if (strcmp(chave, no->info) < 0)      //(chave < no->info)
        no->esq = insere(no->esq, chave);
    else if (strcmp(chave, no->info) > 0) //(chave > no->info)
        no->dir = insere(no->dir, chave);
    else //chaves iguais: nao permitido
        return no;

    //Passo 2: Atualizar a altura do no pai
    no->altura = max(altura(no->esq),
                           altura(no->dir));

    ///Passo 3: Obter o fator de balanceamento do no pai e confirmar se esse no esta desbalanceado
    int balanco = retornaBalanco(no);

    //esquerda esquerda
    if (balanco > 1 && (strcmp(chave, no->esq->info) < 0))  //(balanco > 1 && chave < no->esq->info)
        return rotacaoDireita(no);

    //direita direita
    if (balanco < -1 && (strcmp(chave, no->dir->info) > 0)) //(balanco < -1 && chave > no->dir->info)
        return rotacaoEsquerda(no);

    //esquerda direita
    if (balanco > 1 && (strcmp(chave, no->esq->info) > 0))  //(balanco > 1 && chave > no->esq->info)
    {
        no->esq =  rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

     //direita esquerda
    if (balanco < -1 && (strcmp(chave, no->dir->info) < 0)) //(balanco < -1 && chave < no->dir->info)
    {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    //retorna o ponteiro do no (nao modificado)
    return no;
}

//void imprimeAVL (struct No *raiz)
//{
//    if(raiz != NULL)
//    {
//        printf("%d ", raiz->chave);
//        imprimeAVL(raiz->esq);
//        imprimeAVL(raiz->dir);
//    }
//}

void inOrdem(struct No *raiz)
{
    if(raiz != NULL)
    {
        inOrdem(raiz->esq);
        printf(" %s ", raiz->info);
        inOrdem(raiz->dir);
    }
}

void preOrdem(struct No *raiz)
{
    if(raiz != NULL)
    {
        printf(" %s ", raiz->info);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

// No programa principal, ler um arquivo texto e fazer a inserção de cada palavra na árvore AVL.
struct No* LerArquivo(Tipo nomeArq, struct No *raiz)
{
    char palavra;
    Tipo aux;
    FILE *arquivo;
    int i = 0;
    arquivo = fopen(nomeArq, "r");
    if(arquivo == 0)
    {
        printf("ERRO AO LER ARQUIVO.\n");
        fclose(arquivo);
    }
    else
    {
        i = 0;
        do
        {
            palavra = getc(arquivo);
            if(palavra != '\n' && palavra != EOF)
            {
                aux[i] = palavra;
                i++;
            }
            else
            {
                aux[i] = '\0';
                if(aux[0] != '\0')
                {
                   raiz = insere(raiz, aux);
                   printf("%s \n", aux); // DEBUG
                }
                i = 0;
            }
        }while(palavra != EOF);
        fclose(arquivo);
    }
    return raiz;
}

int main()
{
    /* Criar raiz da arvore */
    struct No *raiz = NULL;

    printf("ARQUIVO:\n");
    raiz = LerArquivo("entrada.txt", raiz);
    printf("===================================\n");
    printf("\nPRE ORDEM:\n"); preOrdem(raiz);
    printf("\n");
    printf("\nEM ORDEM:\n"); inOrdem(raiz);


    return 0;
}
