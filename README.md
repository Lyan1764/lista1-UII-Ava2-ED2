# Lista 1 - UII - Ava2 - ED2: Árvore Vermelho-Preta

Repositório destinado à resolução da Lista 2 (Unidade II) da disciplina. O projeto consiste na modelagem e implementação de um sistema de gestão para uma fábrica de produtos capilares, com foco na otimização de buscas utilizando Árvores Vermelho-Pretas.

## 🏭 Contexto do Problema

O sistema visa gerir o catálogo, o stock e as vendas de uma fábrica que produz cosméticos capilares. As características do catálogo incluem:
* **Tipos de Produto:** Shampoo, condicionador, finalizador, máscara hidratante, óleo reparador de pontas.
* **Tipos de Cabelo:** Liso, ondulado, crespo.
* **Situação do Cabelo:** Oleoso, normal, seco, pós-química.
* **Tamanhos:** 300ml, 500ml, 1L.

O sistema controla o **stock** e os **preços** (variáveis conforme o volume da compra). Os clientes são **revendedores** (identificados por CNPJ, nome, endereço e contacto). As compras são atreladas ao CNPJ do revendedor, organizadas por data, e contêm: código da compra, produtos, quantidades e valor pago.

---

## 🎯 Tarefas e Requisitos

- [ ] **(a) Modelação das Estruturas de Dados:**
  - Criar as `structs` adequadas (vetores, listas encadeadas, etc.).
  - **Requisito Obrigatório:** Utilizar a estrutura de **Árvore Vermelho-Preta** para indexar os revendedores.
  - Produzir o desenho das estruturas, evidenciando os ponteiros e ligações entre elas.
  - Escrever a justificativa técnica para as escolhas tomadas.

- [ ] **(b) Fluxograma:**
  - Desenhar o fluxograma detalhando o processo de registo de um novo produto no sistema, respeitando a arquitetura das estruturas definidas.

- [ ] **(c) Implementação da Árvore Vermelho-Preta:**
  - Desenvolver as operações fundamentais em C: `inserir`, `buscar` e `remover`.
  - Construir a função `main()` que chame e teste todas as operações.

- [ ] **(d) Bateria de Testes e Análise de Desempenho:**
  - Gerar 100 CNPJs e armazená-los num vetor.
  - Inserir estes 100 clientes na árvore sob 4 cenários distintos:
    1. Ordem Crescente.
    2. Ordem Decrescente.
    3. Aleatório, forçando o CNPJ central a ser o primeiro da lista (raiz).
    4. Totalmente aleatório.
  - **Métricas de Sucesso:** Buscar os mesmos 10 CNPJs em cada um dos 4 cenários e registar a quantidade de nós percorridos (passos).
  - **Métricas de Falha:** Buscar 1 CNPJ inexistente em cada cenário e registar a quantidade de passos até o algoritmo concluir que não existe.
  - Redigir uma análise detalhada a justificar a diferença de desempenho entre os cenários.

---

## 🛠️ Especificações Técnicas

* **Linguagem de Programação:** C
* **Estrutura Central:** Árvore Vermelho-Preta (Red-Black Tree)

## 📦 Regras de Entrega

* **Formato:** O desenvolvimento do código pode ser realizado individualmente ou em dupla (identificar os autores no cabeçalho do código). O **Relatório é obrigatoriamente individual**.
* **Ficheiros a Submeter:** 
  1. Ficheiros de Código Fonte (`.c` / `.h`).
  2. Relatório Técnico em formato `PDF` (respeitando o modelo fornecido).
* **Plataforma:** Submissão via SIGAA. Em caso de falha no sistema, remeter para o e-mail: `julianaoc@ufpi.edu.br`.
* **Prazo:** *Verificar data agendada no SIGAA.*