#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 100

// estrutura do nó da Trie
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

// --- Exercício 1 ---

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

void insert(TrieNode* root, const char* key) {
    TrieNode* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = key[i] - 'A';
        if (!pCurrent->children[index]) {
            pCurrent->children[index] = createNode();
        }
        pCurrent = pCurrent->children[index];
    }
    pCurrent->isEndOfWord = true;
}

// --- Exercício 2 ---

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

// funcão auxiliar recursiva para buscar por prefixo
void keysWithPrefixRecursive(TrieNode* node, char* currentPrefix, int level) {
    if (node->isEndOfWord) {
        currentPrefix[level] = '\0';
        printf("  - %s\n", currentPrefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            currentPrefix[level] = i + 'A';
            keysWithPrefixRecursive(node->children[i], currentPrefix, level + 1);
        }
    }
}

void keysWithPrefix(TrieNode* root, const char* prefix) {
    TrieNode* pCurrent = root;
    
    // navega até o nó do prefixo
    for (int i = 0; i < strlen(prefix); i++) {
        int index = prefix[i] - 'A';
        if (!pCurrent->children[index]) {
            printf("Nenhuma palavra encontrada com o prefixo '%s'.\n", prefix);
            return;
        }
        pCurrent = pCurrent->children[index];
    }

    // apartir do nó do prefixo busca todas as palavras
    char currentWord[MAX_WORD_LENGTH];
    strcpy(currentWord, prefix);
    printf("Palavras com o prefixo '%s':\n", prefix);
    keysWithPrefixRecursive(pCurrent, currentWord, strlen(prefix));
}


// --- Exercício 3 ---

// retorna true se um nó não tem filhos
bool isEmpty(TrieNode* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            return false;
        }
    }
    return true;
}

// funcão recursiva para remover uma chave da Trie
TrieNode* removeKey(TrieNode* root, const char* key, int depth) {
    if (!root) {
        return NULL;
    }

    // se chegamos ao final da chave
    if (depth == strlen(key)) {
        if (root->isEndOfWord) {
            root->isEndOfWord = false; // desmarca como fim de palavra
        }

        // se o nó não tem filhos pode ser removido
        if (isEmpty(root)) {
            free(root);
            root = NULL;
        }
        return root;
    }

    // se não for o final da chave, continua a recursão
    int index = key[depth] - 'A';
    root->children[index] = removeKey(root->children[index], key, depth + 1);

    // se o nó atual não é fim de outra palavra e não tem filhos após a remoção remove ele
    if (isEmpty(root) && !root->isEndOfWord) {
        free(root);
        root = NULL;
    }

    return root;
}


int main() {
    const char* words[] = {"RATO", "RITA", "AMIGO", "AMORA", "ELA"};
    int n = sizeof(words) / sizeof(words[0]);

    TrieNode* root = createNode();

    // --- Exercício 1: Inserção ---
    printf("--- Exercício 1: Inserindo palavras ---\n");
    for (int i = 0; i < n; i++) {
        insert(root, words[i]);
    }
    printf("Trie construída.\n\n");

    // --- Exercício 2: Busca ---
    printf("--- Exercício 2: Operações de Busca ---\n");
    printf("Busca por palavra completa:\n");
    printf("search(\"RITA\"): %s\n", search(root, "RITA") ? "true" : "false");
    printf("search(\"RATO\"): %s\n", search(root, "RATO") ? "true" : "false");
    printf("search(\"AMOR\"): %s\n", search(root, "AMOR") ? "true" : "false");
    printf("search(\"ELA\"): %s\n", search(root, "ELA") ? "true" : "false");
    printf("search(\"RIT\"): %s\n", search(root, "RIT") ? "true" : "false");
    
    printf("\nBusca por prefixo:\n");
    keysWithPrefix(root, "AM");
    printf("\n");

    // --- Exercício 3: Remoção ---
    printf("--- Exercício 3: Operações de Remoção ---\n");
    printf("Removendo 'AMIGO'...\n");
    root = removeKey(root, "AMIGO", 0);
    printf("Removendo 'RITA'...\n");
    root = removeKey(root, "RITA", 0);
    printf("Remoções concluídas.\n\n");
    
    // verificando o estado final da árvore
    printf("Verificando a árvore após a remoção:\n");
    printf("search(\"AMIGO\"): %s (removida)\n", search(root, "AMIGO") ? "true" : "false");
    printf("search(\"AMORA\"): %s (deve existir)\n", search(root, "AMORA") ? "true" : "false");
    printf("search(\"RITA\"): %s (removida)\n", search(root, "RITA") ? "true" : "false");
    printf("search(\"RATO\"): %s (deve existir)\n", search(root, "RATO") ? "true" : "false");
    
    return 0;
}