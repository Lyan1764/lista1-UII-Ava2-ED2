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
    enum Cor cor;
    struct Revendedor *esq, *dir, *pai;
};

// NULL conta como no preto, entao da para olhar a cor de qualquer ponteiro.
enum Cor corDe(struct Revendedor *no) {
    return no == NULL ? PRETO : no->cor;
}

void rotacaoEsquerda(struct Revendedor **raiz, struct Revendedor *x) {
    struct Revendedor *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL)
        y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
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
    if (y->dir != NULL)
        y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->dir)
        x->pai->dir = y;
    else
        x->pai->esq = y;
    y->dir = x;
    x->pai = y;
}

// Busca o CNPJ descendo pela arvore. Retorna NULL se nao achar.
struct Revendedor *buscarRevendedor(struct Revendedor *raiz, char *cnpj) {
    if (raiz == NULL)
        return NULL;
    int cmp = strcmp(cnpj, raiz->cnpj);
    if (cmp == 0)
        return raiz;
    if (cmp < 0)
        return buscarRevendedor(raiz->esq, cnpj);
    return buscarRevendedor(raiz->dir, cnpj);
}

// Conserta as cores e rotaciona depois de inserir o no z.
void corrigirInsercao(struct Revendedor **raiz, struct Revendedor *z) {
    while (z->pai != NULL && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            struct Revendedor *tio = z->pai->pai->dir;
            if (corDe(tio) == VERMELHO) {
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
            if (corDe(tio) == VERMELHO) {
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
    if (buscarRevendedor(*raiz, cnpj) != NULL)
        return 0;

    struct Revendedor *z = (struct Revendedor *)malloc(sizeof(struct Revendedor));
    strcpy(z->cnpj, cnpj);
    strcpy(z->nome, nome);
    strcpy(z->endereco, endereco);
    strcpy(z->contato, contato);
    z->historico = NULL;
    z->cor = VERMELHO;
    z->esq = z->dir = NULL;

    struct Revendedor *y = NULL;
    struct Revendedor *x = *raiz;
    while (x != NULL) {
        y = x;
        if (strcmp(cnpj, x->cnpj) < 0)
            x = x->esq;
        else
            x = x->dir;
    }

    z->pai = y;
    if (y == NULL)
        *raiz = z;
    else if (strcmp(cnpj, y->cnpj) < 0)
        y->esq = z;
    else
        y->dir = z;

    corrigirInsercao(raiz, z);
    return 1;
}

// Coloca a subarvore v (que pode ser NULL) no lugar da subarvore u.
void transplantar(struct Revendedor **raiz, struct Revendedor *u, struct Revendedor *v) {
    if (u->pai == NULL)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    if (v != NULL)
        v->pai = u->pai;
}

struct Revendedor *minimo(struct Revendedor *x) {
    while (x->esq != NULL)
        x = x->esq;
    return x;
}

// Resolve o "preto extra" em x quando um no preto e removido. Como x pode ser NULL, o pai vem de fora.
void corrigirRemocao(struct Revendedor **raiz, struct Revendedor *x, struct Revendedor *pai) {
    struct Revendedor *w;
    while (x != *raiz && corDe(x) == PRETO) {
        if (x == pai->esq) {
            w = pai->dir;
            if (corDe(w) == VERMELHO) {
                w->cor = PRETO;
                pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, pai);
                w = pai->dir;
            }
            if (corDe(w->esq) == PRETO && corDe(w->dir) == PRETO) {
                w->cor = VERMELHO;
                x = pai;
                pai = x->pai;
            } else {
                if (corDe(w->dir) == PRETO) {
                    w->esq->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = pai->dir;
                }
                w->cor = pai->cor;
                pai->cor = PRETO;
                w->dir->cor = PRETO;
                rotacaoEsquerda(raiz, pai);
                x = *raiz;
            }
        } else {
            w = pai->esq;
            if (corDe(w) == VERMELHO) {
                w->cor = PRETO;
                pai->cor = VERMELHO;
                rotacaoDireita(raiz, pai);
                w = pai->esq;
            }
            if (corDe(w->dir) == PRETO && corDe(w->esq) == PRETO) {
                w->cor = VERMELHO;
                x = pai;
                pai = x->pai;
            } else {
                if (corDe(w->esq) == PRETO) {
                    w->dir->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = pai->esq;
                }
                w->cor = pai->cor;
                pai->cor = PRETO;
                w->esq->cor = PRETO;
                rotacaoDireita(raiz, pai);
                x = *raiz;
            }
        }
    }
    if (x != NULL)
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
    if (z == NULL)
        return 0;

    struct Revendedor *x;
    struct Revendedor *paiX;
    enum Cor corOriginal = z->cor;

    if (z->esq == NULL) {
        x = z->dir;
        paiX = z->pai;
        transplantar(raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        paiX = z->pai;
        transplantar(raiz, z, z->esq);
    } else {
        struct Revendedor *y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            paiX = y;
        } else {
            paiX = y->pai;
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
        corrigirRemocao(raiz, x, paiX);

    liberarCompras(z->historico);
    free(z);
    return 1;
}

void liberarArvore(struct Revendedor *r) {
    if (r == NULL)
        return;
    liberarArvore(r->esq);
    liberarArvore(r->dir);
    liberarCompras(r->historico);
    free(r);
}

/* ---------------- Item d: experimento com 100 CNPJs ---------------- */

#define TOTAL 100
#define BUSCAS 10

// Gerador proprio com semente fixa: os resultados sao os mesmos em qualquer maquina.
unsigned long long estado = 2026;

unsigned int sorteio(void) {
    estado = estado * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned int)(estado >> 33);
}

// CNPJ de 14 digitos guardado como numero (long long).
long long sortearCnpj(void) {
    long long alto = sorteio();
    long long baixo = sorteio();
    return (alto * 2147483648LL + baixo) % 100000000000000LL;
}

int contem(long long v[], int n, long long x) {
    for (int i = 0; i < n; i++)
        if (v[i] == x)
            return 1;
    return 0;
}

void gerarCnpjs(long long v[]) {
    for (int i = 0; i < TOTAL; i++) {
        long long novo;
        do {
            novo = sortearCnpj();
        } while (contem(v, i, novo));
        v[i] = novo;
    }
}

// Bubble sort crescente.
void ordenar(long long v[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (v[j] > v[j + 1]) {
                long long aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
}

// Embaralhamento de Fisher-Yates.
void embaralhar(long long v[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = sorteio() % (i + 1);
        long long aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
}

// Mesma busca da letra c, mas contando os nos visitados ate achar o CNPJ (ou chegar em NULL).
struct Revendedor *buscarPassos(struct Revendedor *raiz, char *cnpj, int *passos) {
    if (raiz == NULL)
        return NULL;
    (*passos)++;
    int cmp = strcmp(cnpj, raiz->cnpj);
    if (cmp == 0)
        return raiz;
    if (cmp < 0)
        return buscarPassos(raiz->esq, cnpj, passos);
    return buscarPassos(raiz->dir, cnpj, passos);
}

// Altura em niveis (numero de nos do caminho mais longo).
int altura(struct Revendedor *r) {
    if (r == NULL)
        return 0;
    int e = altura(r->esq);
    int d = altura(r->dir);
    return 1 + (e > d ? e : d);
}

// Insere os 100 CNPJs na ordem recebida e depois faz as buscas, contando os passos.
void executarCaso(char *titulo, long long ordem[], long long buscas[], long long inexistente) {
    struct Revendedor *raiz = NULL;
    char cnpj[20];
    int passos, soma = 0;

    for (int i = 0; i < TOTAL; i++) {
        sprintf(cnpj, "%014lld", ordem[i]);
        inserirRevendedor(&raiz, cnpj, "Revendedor", "Picos-PI", "contato");
    }

    printf("\n%s (altura da arvore: %d niveis)\n", titulo, altura(raiz));
    for (int i = 0; i < BUSCAS; i++) {
        sprintf(cnpj, "%014lld", buscas[i]);
        passos = 0;
        buscarPassos(raiz, cnpj, &passos);
        soma += passos;
        printf("  %s: %d passos\n", cnpj, passos);
    }
    printf("  media dos %d CNPJs: %.1f passos\n", BUSCAS, soma / (double)BUSCAS);

    sprintf(cnpj, "%014lld", inexistente);
    passos = 0;
    buscarPassos(raiz, cnpj, &passos);
    printf("  CNPJ nao cadastrado %s: %d passos\n", cnpj, passos);

    liberarArvore(raiz);
}

void experimento(void) {
    long long cnpjs[TOTAL], ordenados[TOTAL], ordem[TOTAL], buscas[BUSCAS], inexistente;

    gerarCnpjs(cnpjs);
    memcpy(ordenados, cnpjs, sizeof(cnpjs));
    ordenar(ordenados, TOTAL);

    // Os 10 CNPJs buscados saem de um embaralhamento e valem para os quatro casos.
    memcpy(ordem, cnpjs, sizeof(cnpjs));
    embaralhar(ordem, TOTAL);
    memcpy(buscas, ordem, sizeof(buscas));

    do {
        inexistente = sortearCnpj();
    } while (contem(cnpjs, TOTAL, inexistente));

    executarCaso("Caso 1 - crescente", ordenados, buscas, inexistente);

    for (int i = 0; i < TOTAL; i++)
        ordem[i] = ordenados[TOTAL - 1 - i];
    executarCaso("Caso 2 - decrescente", ordem, buscas, inexistente);

    // Aleatorio, mas com o CNPJ do meio (da lista ordenada) inserido primeiro.
    memcpy(ordem, cnpjs, sizeof(cnpjs));
    embaralhar(ordem, TOTAL);
    for (int i = 0; i < TOTAL; i++)
        if (ordem[i] == ordenados[TOTAL / 2]) {
            ordem[i] = ordem[0];
            ordem[0] = ordenados[TOTAL / 2];
            break;
        }
    executarCaso("Caso 3 - meio primeiro, resto aleatorio", ordem, buscas, inexistente);

    memcpy(ordem, cnpjs, sizeof(cnpjs));
    embaralhar(ordem, TOTAL);
    executarCaso("Caso 4 - totalmente aleatorio", ordem, buscas, inexistente);
}

int main(void) {
    printf("==== Experimento (item d) ====\n");
    experimento();
    return 0;
}