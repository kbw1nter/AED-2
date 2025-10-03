#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

// funcão para criar um novo nó da Trie
TrieNode* createNode() {
    TrieNode* pNode = (TrieNode*)malloc(sizeof(TrieNode));
    if (pNode) {
        pNode->isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

// funcão para inserir uma palavra na Trie
void insert(TrieNode* root, const char* key) {
    TrieNode* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        // assumindo que as palavras são todas em maiúsculas
        int index = key[i] - 'A';
        if (!pCurrent->children[index]) {
            pCurrent->children[index] = createNode();
        }
        pCurrent = pCurrent->children[index];
    }
    // marca o ultimo nó como fim de palavra
    pCurrent->isEndOfWord = true;
}

// funcão para buscar uma palavra na Trie
bool search(TrieNode* root, const char* key) {
    TrieNode* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = key[i] - 'A';
        if (!pCurrent->children[index]) {
            return false;
        }
        pCurrent = pCurrent->children[index];
    }
    return (pCurrent != NULL && pCurrent->isEndOfWord);
}

int main() {
    TrieNode* root = createNode();

    printf("--- Exercício 4: Inserindo palavras com prefixos existentes ---\n\n");

    printf("1. Inserindo 'TESTER'...\n");
    insert(root, "TESTER");

    printf("2. Inserindo 'TESTE'...\n");
    insert(root, "TESTE");

    printf("3. Inserindo 'TESTA'...\n");
    insert(root, "TESTA");

    printf("\nInserções concluídas. A trie final foi construída.\n");

    // checando se todas as palavras inseridas são encontradas
    printf("\n--- Verificando o conteúdo da Trie ---\n");
    printf("A palavra 'TESTER' existe? %s\n", search(root, "TESTER") ? "Sim" : "Não");
    printf("A palavra 'TESTE' existe? %s\n", search(root, "TESTE") ? "Sim" : "Não");
    printf("A palavra 'TESTA' existe? %s\n", search(root, "TESTA") ? "Sim" : "Não");
    
    // verificando um prefixo que não foi inserido como palavra completa
    printf("A palavra 'TEST' existe? %s (é apenas um prefixo)\n", search(root, "TEST") ? "Sim" : "Não");
    
    return 0;
}