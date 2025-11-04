#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha

// Definicao da estrutura Peca
typedef struct {
    char nome;  // Tipo da peca: 'I', 'O', 'T', 'L'
    int id;     // Identificador unico da peca
} Peca;

// Definicao da estrutura Fila Circular
typedef struct {
    Peca elementos[5];  // Array com capacidade para 5 pecas
    int frente;         // Indice da frente da fila
    int tras;           // Indice do final da fila
    int tamanho;        // Quantidade atual de elementos
} FilaCircular;

// Definicao da estrutura Pilha Linear
typedef struct {
    Peca elementos[3];  // Array com capacidade para 3 pecas
    int topo;           // Indice do topo da pilha (-1 indica pilha vazia)
} PilhaLinear;

// Variavel global para controlar o ID sequencial das pecas
int proximoId = 0;

// Funcao para gerar uma peca automaticamente
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// Funcoes da Fila Circular

// Inicializa a fila circular
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Verifica se a fila esta vazia
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

// Verifica se a fila esta cheia
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == 5;
}

// Insere uma peca no final da fila
int enqueue(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0;  // Fila cheia, operacao falhou
    }
    fila->tras = (fila->tras + 1) % 5;
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
    return 1;  // Operacao bem-sucedida
}

// Remove e retorna a peca da frente da fila
Peca dequeue(FilaCircular *fila) {
    Peca pecaRemovida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % 5;
    fila->tamanho--;
    return pecaRemovida;
}

// Exibe o estado atual da fila
void mostrarFila(FilaCircular *fila) {
    printf("Fila de pecas: ");
    if (filaVazia(fila)) {
        printf("[vazia]");
    } else {
        int i;
        int indice = fila->frente;
        for (i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->elementos[indice].nome, fila->elementos[indice].id);
            indice = (indice + 1) % 5;
        }
    }
    printf("\n");
}

// Funcoes da Pilha Linear

// Inicializa a pilha linear
void inicializarPilha(PilhaLinear *pilha) {
    pilha->topo = -1;  // Pilha vazia
}

// Verifica se a pilha esta vazia
int pilhaVazia(PilhaLinear *pilha) {
    return pilha->topo == -1;
}

// Verifica se a pilha esta cheia
int pilhaCheia(PilhaLinear *pilha) {
    return pilha->topo == 2;
}

// Insere uma peca no topo da pilha
int push(PilhaLinear *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0;  // Pilha cheia, operacao falhou
    }
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
    return 1;  // Operacao bem-sucedida
}

// Remove e retorna a peca do topo da pilha
Peca pop(PilhaLinear *pilha) {
    Peca pecaRemovida = pilha->elementos[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

// Exibe o estado atual da pilha
void mostrarPilha(PilhaLinear *pilha) {
    printf("Pilha de reserva (Topo -> base): ");
    if (pilhaVazia(pilha)) {
        printf("[vazia]");
    } else {
        int i;
        for (i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
        }
    }
    printf("\n");
}

// Funcoes de interacao entre Fila e Pilha

// Troca a peca da frente da fila com o topo da pilha
void trocarFrentePilha(FilaCircular *fila, PilhaLinear *pilha) {
    if (filaVazia(fila)) {
        printf("Erro: fila vazia. Nao e possivel realizar a troca.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("Erro: pilha vazia. Nao e possivel realizar a troca.\n");
        return;
    }

    // Realiza a troca
    Peca temp = fila->elementos[fila->frente];
    fila->elementos[fila->frente] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;

    printf("Troca realizada: peca da frente da fila com o topo da pilha.\n");
}

// Troca os 3 primeiros elementos da fila com as 3 pecas da pilha
void trocarTresElementos(FilaCircular *fila, PilhaLinear *pilha) {
    if (fila->tamanho < 3) {
        printf("Erro: fila tem menos de 3 pecas. Nao e possivel realizar a troca.\n");
        return;
    }
    if (pilha->topo != 2) {
        printf("Erro: pilha nao tem exatamente 3 pecas. Nao e possivel realizar a troca.\n");
        return;
    }

    // Armazena temporariamente os 3 primeiros da fila
    Peca tempFila[3];
    int indice = fila->frente;
    int i;
    for (i = 0; i < 3; i++) {
        tempFila[i] = fila->elementos[indice];
        indice = (indice + 1) % 5;
    }

    // Copia as 3 pecas da pilha para os 3 primeiros da fila
    indice = fila->frente;
    for (i = 2; i >= 0; i--) {
        fila->elementos[indice] = pilha->elementos[i];
        indice = (indice + 1) % 5;
    }

    // Copia as 3 pecas da fila para a pilha (ordem invertida)
    for (i = 0; i < 3; i++) {
        pilha->elementos[i] = tempFila[2 - i];
    }

    printf("Troca realizada: os 3 primeiros da fila com as 3 pecas da pilha.\n");
}

// Funcao para exibir o menu
void exibirMenu() {
    printf("\n===========================================\n");
    printf("Opcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("===========================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));

    // Cria e inicializa as estruturas
    FilaCircular fila;
    PilhaLinear pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 pecas
    int i;
    for (i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }

    // Loop principal do programa
    int opcao;
    int continuar = 1;

    while (continuar) {
        // Exibe o estado atual
        printf("\n");
        printf("Estado atual:\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        // Exibe o menu e le a opcao
        exibirMenu();
        scanf("%d", &opcao);

        // Processa a opcao escolhida
        switch (opcao) {
            case 1:
                // Jogar peca da frente da fila
                if (filaVazia(&fila)) {
                    printf("Erro: fila vazia. Nao ha pecas para jogar.\n");
                } else {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
                    // Repoe uma nova peca na fila
                    if (!filaCheia(&fila)) {
                        enqueue(&fila, gerarPeca());
                        printf("Nova peca adicionada a fila.\n");
                    }
                }
                break;

            case 2:
                // Enviar peca da fila para a pilha
                if (filaVazia(&fila)) {
                    printf("Erro: fila vazia. Nao ha pecas para enviar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Erro: pilha cheia. Nao e possivel adicionar mais pecas.\n");
                } else {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    printf("Peca [%c %d] enviada para a pilha de reserva.\n",
                           pecaReservada.nome, pecaReservada.id);
                    // Repoe uma nova peca na fila
                    if (!filaCheia(&fila)) {
                        enqueue(&fila, gerarPeca());
                        printf("Nova peca adicionada a fila.\n");
                    }
                }
                break;

            case 3:
                // Usar peca da pilha
                if (pilhaVazia(&pilha)) {
                    printf("Erro: pilha vazia. Nao ha pecas reservadas.\n");
                } else {
                    Peca pecaUsada = pop(&pilha);
                    printf("Peca reservada usada: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
                }
                break;

            case 4:
                // Trocar peca da frente com topo da pilha
                trocarFrentePilha(&fila, &pilha);
                break;

            case 5:
                // Trocar 3 primeiros da fila com os 3 da pilha
                trocarTresElementos(&fila, &pilha);
                break;

            case 0:
                // Sair do programa
                printf("Encerrando o programa...\n");
                continuar = 0;
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
