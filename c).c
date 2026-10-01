#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 50

enum Cor { PRETO, VERMELHO };

struct Faixa {
    int qtd_min;
    float preco;
};

struct Produto {
    int id;
    char tipo[50], cabelo[50], situacao[50];
    int tamanho_ml;
    struct Faixa faixas[3];
    int estoque;
};

struct Item {
    int id_produto;
    int qtd;
    float preco_unit;
    float preco_total;
};

struct Compra {
    int id_compra;
    char data[11];
    struct Item carrinho[MAX_ITENS];
    int total_itens;
    struct Compra *proxima;
};

struct Revendedor {
    char cnpj[20];
    char nome[100], endereco[200], contato[50];
    struct Compra *historico;
    int cor;
    struct Revendedor *esq, *dir, *pai;
};

struct Revendedor sentinela;
struct Revendedor *NIL = &sentinela;

// Deixa a arvore vazia. NIL e o no preto que faz o papel de NULL.
void iniciarArvore(struct Revendedor **raiz) {
    sentinela.cor = PRETO;
    sentinela.esq = sentinela.dir = sentinela.pai = NIL;
    *raiz = NIL;
}

void rotacaoEsquerda(struct Revendedor **raiz, struct Revendedor *x) {
    struct Revendedor *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NIL)
        y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL)
        *raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rotacaoDireita(struct Revendedor **raiz, struct Revendedor *x) {
    struct Revendedor *y = x->esq;
    x->esq = y->dir;
    if (y->dir != NIL)
        y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL)
        *raiz = y;
    else if (x == x->pai->dir)
        x->pai->dir = y;
    else
        x->pai->esq = y;
    y->dir = x;
    x->pai = y;
}

// Busca o CNPJ descendo pela arvore. Retorna NIL se nao achar.
struct Revendedor *buscarRevendedor(struct Revendedor *raiz, char *cnpj) {
    if (raiz == NIL)
        return NIL;
    int cmp = strcmp(cnpj, raiz->cnpj);
    if (cmp == 0)
        return raiz;
    if (cmp < 0)
        return buscarRevendedor(raiz->esq, cnpj);
    return buscarRevendedor(raiz->dir, cnpj);
}

// Conserta as cores e rotaciona depois de inserir o no z.
void corrigirInsercao(struct Revendedor **raiz, struct Revendedor *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            struct Revendedor *tio = z->pai->pai->dir;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            struct Revendedor *tio = z->pai->pai->esq;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

// Insere como numa ABB (no novo vermelho) e depois corrige. Retorna 0 se o CNPJ ja existe.
int inserirRevendedor(struct Revendedor **raiz, char *cnpj, char *nome, char *endereco, char *contato) {
    if (buscarRevendedor(*raiz, cnpj) != NIL)
        return 0;

    struct Revendedor *z = (struct Revendedor *)malloc(sizeof(struct Revendedor));
    strcpy(z->cnpj, cnpj);
    strcpy(z->nome, nome);
    strcpy(z->endereco, endereco);
    strcpy(z->contato, contato);
    z->historico = NULL;
    z->cor = VERMELHO;
    z->esq = z->dir = NIL;

    struct Revendedor *y = NIL;
    struct Revendedor *x = *raiz;
    while (x != NIL) {
        y = x;
        if (strcmp(cnpj, x->cnpj) < 0)
            x = x->esq;
        else
            x = x->dir;
    }

    z->pai = y;
    if (y == NIL)
        *raiz = z;
    else if (strcmp(cnpj, y->cnpj) < 0)
        y->esq = z;
    else
        y->dir = z;

    corrigirInsercao(raiz, z);
    return 1;
}

// Coloca a subarvore v no lugar da subarvore u.
void transplantar(struct Revendedor **raiz, struct Revendedor *u, struct Revendedor *v) {
    if (u->pai == NIL)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    v->pai = u->pai;
}

struct Revendedor *minimo(struct Revendedor *x) {
    while (x->esq != NIL)
        x = x->esq;
    return x;
}

// Resolve o "preto extra" que fica em x quando um no preto e removido.
void corrigirRemocao(struct Revendedor **raiz, struct Revendedor *x) {
    struct Revendedor *w;
    while (x != *raiz && x->cor == PRETO) {
        if (x == x->pai->esq) {
            w = x->pai->dir;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == PRETO && w->dir->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->dir->cor == PRETO) {
                    w->esq->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->dir->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        } else {
            w = x->pai->esq;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == PRETO && w->esq->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esq->cor == PRETO) {
                    w->dir->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esq->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    x->cor = PRETO;
}

void liberarCompras(struct Compra *c) {
    while (c != NULL) {
        struct Compra *prox = c->proxima;
        free(c);
        c = prox;
    }
}

// Remove o revendedor (e o historico dele). Retorna 0 se o CNPJ nao existe.
int removerRevendedor(struct Revendedor **raiz, char *cnpj) {
    struct Revendedor *z = buscarRevendedor(*raiz, cnpj);
    if (z == NIL)
        return 0;

    struct Revendedor *y = z;
    struct Revendedor *x;
    int corOriginal = y->cor;

    if (z->esq == NIL) {
        x = z->dir;
        transplantar(raiz, z, z->dir);
    } else if (z->dir == NIL) {
        x = z->esq;
        transplantar(raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplantar(raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    if (corOriginal == PRETO)
        corrigirRemocao(raiz, x);

    liberarCompras(z->historico);
    free(z);
    return 1;
}

// Mostra os CNPJs em ordem crescente, com a cor de cada no (P ou V).
void emOrdem(struct Revendedor *r) {
    if (r == NIL)
        return;
    emOrdem(r->esq);
    printf("%s(%c) ", r->cnpj, r->cor == PRETO ? 'P' : 'V');
    emOrdem(r->dir);
}

int main(void) {
    struct Revendedor *raiz;
    char *cnpjs[] = {"11111111000111", "22222222000122", "33333333000133", "44444444000144",
                     "55555555000155", "66666666000166", "77777777000177"};

    iniciarArvore(&raiz);

    for (int i = 0; i < 7; i++)
        inserirRevendedor(&raiz, cnpjs[i], "Revendedor", "Picos-PI", "contato");
    printf("Inserir 33333333000133 de novo: %d\n", inserirRevendedor(&raiz, "33333333000133", "Revendedor", "Picos-PI", "contato"));
    printf("Arvore: ");
    emOrdem(raiz);
    printf("\nRaiz: %s\n\n", raiz->cnpj);

    printf("Buscar 55555555000155: %s\n", buscarRevendedor(raiz, "55555555000155") != NIL ? "encontrado" : "nao encontrado");
    printf("Buscar 99999999000199: %s\n\n", buscarRevendedor(raiz, "99999999000199") != NIL ? "encontrado" : "nao encontrado");

    printf("Remover 22222222000122: %d\n", removerRevendedor(&raiz, "22222222000122"));
    printf("Remover 44444444000144: %d\n", removerRevendedor(&raiz, "44444444000144"));
    printf("Remover 99999999000199: %d\n", removerRevendedor(&raiz, "99999999000199"));
    printf("Arvore: ");
    emOrdem(raiz);
    printf("\nRaiz: %s\n", raiz->cnpj);

    return 0;
}