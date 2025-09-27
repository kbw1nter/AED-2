#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// estrutura do nó da Trie
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int polarity; // -1se for negativo, 0 se for neutro, 1 se for positive
    int isEndOfWord;
} TrieNode;

// criar um novo nó da trie
TrieNode *createNode(void) {
    TrieNode *pNode = (TrieNode *)malloc(sizeof(TrieNode));
    if (pNode) {
        pNode->isEndOfWord = 0;
        pNode->polarity = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

// inserir uma palavra na trie
void insert(TrieNode *root, const char *word, int polarity) {
    TrieNode *pCurrent = root;
    int len = strlen(word);

    for (int level = 0; level < len; level++) {
        // apenas caracteres alfabéticos
        if (isalpha((unsigned char)word[level])) {
            int index = tolower((unsigned char)word[level]) - 'a';
            if (!pCurrent->children[index]) {
                pCurrent->children[index] = createNode();
            }
            pCurrent = pCurrent->children[index];
        }
    }

    // marrcar o último nó como fim de palavra e armazenar a polaridade
    pCurrent->isEndOfWord = 1;
    pCurrent->polarity = polarity;
}

// buscar a polaridade de uma palavra na trie
int search(TrieNode *root, const char *word) {
    TrieNode *pCurrent = root;
    int len = strlen(word);

    for (int level = 0; level < len; level++) {
        if (isalpha((unsigned char)word[level])) {
            int index = tolower((unsigned char)word[level]) - 'a';
            if (!pCurrent->children[index]) {
                return 2; // sinaliza que a palavra não foi encontrada
            }
            pCurrent= pCurrent->children[index];
        }
    }

    if (pCurrent != NULL && pCurrent->isEndOfWord) {
        return pCurrent->polarity;
    }

    return 2; // sinaliza que a palavra não foi encontrada
}

// rditar a polaridade de uma palavra existente
void editPolarity(TrieNode *root, const char *word, int newPolarity) {
    TrieNode *pCurrent = root;
    int len = strlen(word);

    for (int level = 0; level < len; level++) {
         if (isalpha((unsigned char)word[level])) {
            int index = tolower((unsigned char)word[level]) - 'a';
            if (!pCurrent->children[index]) {
                printf("Palavra nao encontrada para edicao.\n");
                return;
            }
            pCurrent = pCurrent->children[index];
        }
    }

    if (pCurrent != NULL && pCurrent->isEndOfWord) {
        pCurrent->polarity = newPolarity;
        printf("Polaridade da palavra '%s' atualizada para %d.\n", word, newPolarity);
    } else {
        printf("Palavra nao encontrada para edicao.\n");
    }
}

// funcão auxiliar recursiva para salvar a trie
void saveTrieRecursive(TrieNode *node, char *prefix, int length, FILE *fp) {
    if (node->isEndOfWord) {
        prefix[length] = '\0';
        // salva no formato original do .txt
        fprintf(fp, "%s,adj,%d,A\n", prefix, node->polarity);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[length] = 'a' + i;
            saveTrieRecursive(node->children[i], prefix, length + 1, fp);
        }
    }
}

// salvar a trie em um arquivo
void saveTrieToFile(TrieNode *root, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    char prefix[100]; // assumindo um tamanho maximo de palavra
    saveTrieRecursive(root, prefix, 0, fp);

    fclose(fp);
    printf("Lexico salvo com sucesso em '%s'.\n", filename);
}

// liberar a memória alocada para a trie
void freeTrie(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}

int main() {
    TrieNode *root = createNode();

    FILE *fp = fopen("lexico_v3.0.txt", "r");
    if (fp == NULL) {
        perror("Nao foi possivel abrir o arquivo lexico_v3.0.txt");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, ",");
        strtok(NULL, ","); // ignora a classe gramatical
        char *polarity_str = strtok(NULL, ","); // pega a polaridade (terceira coluna)
        // A quarta coluna e o \n são ignorados automaticamente

        if (word && polarity_str) {
            insert(root, word, atoi(polarity_str));
        }
    }
    fclose(fp);

    // interface de linha de comando
    int choice;
    char word[100];
    int polarity;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Buscar polaridade de uma palavra\n");
        printf("2. Editar polaridade de uma palavra\n");
        printf("3. Salvar lexico em arquivo\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Digite a palavra para buscar: ");
                scanf("%s", word);
                polarity = search(root, word);
                if (polarity != 2) {
                    printf("A polaridade de '%s' e: %d\n", word, polarity);
                } else {
                    printf("Palavra '%s' nao encontrada no lexico.\n", word);
                }
                break;
            case 2:
                printf("Digite a palavra para editar: ");
                scanf("%s", word);
                printf("Digite a nova polaridade (-1, 0 ou 1): ");
                scanf("%d", &polarity);
                if(polarity >= -1 && polarity <= 1) {
                    editPolarity(root, word, polarity);
                } else {
                    printf("Polaridade invalida. Use -1, 0 ou 1.\n");
                }
                break;
            case 3:
                printf("Digite o nome do arquivo para salvar: ");
                scanf("%s", word); // reutilizando a variável word para o nome do arquivo
                saveTrieToFile(root, word);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 4);

    freeTrie(root);

    return 0;
}