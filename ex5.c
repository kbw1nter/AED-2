#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // tolower()

//sensível ao caso

#define ALPHABET_SIZE_CS 52 // 26 minúsculas + 26 maiúsculas

typedef struct TrieNode_CS {
    struct TrieNode_CS *children[ALPHABET_SIZE_CS];
    bool isEndOfWord;
} TrieNode_CS;

// 'a'->0, 'b'->1 ... 'A'->26, 'B'->27,...
int char_to_index_cs(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26;
    }
    return -1; // caractere inválido
}

TrieNode_CS* createNode_CS() {
    TrieNode_CS* pNode = (TrieNode_CS*)malloc(sizeof(TrieNode_CS));
    pNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE_CS; i++) pNode->children[i] = NULL;
    return pNode;
}

void insert_cs(TrieNode_CS* root, const char* key) {
    TrieNode_CS* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = char_to_index_cs(key[i]);
        if (index == -1) continue; // Ignora caracteres não alfabéticos
        if (!pCurrent->children[index]) {
            pCurrent->children[index] = createNode_CS();
        }
        pCurrent = pCurrent->children[index];
    }
    pCurrent->isEndOfWord = true;
}

bool search_cs(TrieNode_CS* root, const char* key) {
    TrieNode_CS* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = char_to_index_cs(key[i]);
        if (index == -1 || !pCurrent->children[index]) {
            return false;
        }
        pCurrent = pCurrent->children[index];
    }
    return (pCurrent != NULL && pCurrent->isEndOfWord);
}

//insensível ao caso

#define ALPHABET_SIZE_CI 26 // apenas 26 pq tudo será minúsculo

typedef struct TrieNode_CI {
    struct TrieNode_CI *children[ALPHABET_SIZE_CI];
    bool isEndOfWord;
} TrieNode_CI;

TrieNode_CI* createNode_CI() {
    TrieNode_CI* pNode = (TrieNode_CI*)malloc(sizeof(TrieNode_CI));
    pNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE_CI; i++) pNode->children[i] = NULL;
    return pNode;
}

void insert_ci(TrieNode_CI* root, const char* key) {
    TrieNode_CI* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        // normaliza o caractere para minúsculo antes de obter o índice
        int index = tolower(key[i]) - 'a';
        if (index < 0 || index >= ALPHABET_SIZE_CI) continue;
        
        if (!pCurrent->children[index]) {
            pCurrent->children[index] = createNode_CI();
        }
        pCurrent = pCurrent->children[index];
    }
    pCurrent->isEndOfWord = true;
}

bool search_ci(TrieNode_CI* root, const char* key) {
    TrieNode_CI* pCurrent = root;
    for (int i = 0; i < strlen(key); i++) {
        // tbm normaliza na busca para garantir consistência
        int index = tolower(key[i]) - 'a';
        if (index < 0 || index >= ALPHABET_SIZE_CI || !pCurrent->children[index]) {
            return false;
        }
        pCurrent = pCurrent->children[index];
    }
    return (pCurrent != NULL && pCurrent->isEndOfWord);
}


int main() {
    const char* words[] = {"Gato", "gato", "Gata"};
    int n = sizeof(words) / sizeof(words[0]);

    printf("--- Exercício 5: Sensibilidade ao Caso ---\n\n");

    printf("--- 1. Construindo Trie SENSÍVEL ao caso ---\n");
    TrieNode_CS* root_cs = createNode_CS();
    for (int i = 0; i < n; i++) {
        insert_cs(root_cs, words[i]);
    }
    printf("Inserções concluídas.\n");
    printf("Verificando buscas:\n");
    printf("  search_cs(\"Gato\"): %s\n", search_cs(root_cs, "Gato") ? "Encontrada" : "Não encontrada");
    printf("  search_cs(\"gato\"): %s\n", search_cs(root_cs, "gato") ? "Encontrada" : "Não encontrada");
    printf("  search_cs(\"Gata\"): %s\n", search_cs(root_cs, "Gata") ? "Encontrada" : "Não encontrada");
    printf("  search_cs(\"GATO\"): %s (letras diferentes)\n", search_cs(root_cs, "GATO") ? "Encontrada" : "Não encontrada");
    
    printf("\n");

    printf("--- 2. Construindo Trie INSENSÍVEL ao caso ---\n");
    TrieNode_CI* root_ci = createNode_CI();
    for (int i = 0; i < n; i++) {
        insert_ci(root_ci, words[i]);
    }
    printf("Inserções concluídas (palavras foram convertidas para minúsculas).\n");
    printf("Verificando buscas:\n");
    printf("  search_ci(\"gato\"): %s\n", search_ci(root_ci, "gato") ? "Encontrada" : "Não encontrada");
    printf("  search_ci(\"Gato\"): %s\n", search_ci(root_ci, "Gato") ? "Encontrada" : "Não encontrada");
    printf("  search_ci(\"GATA\"): %s\n", search_ci(root_ci, "GATA") ? "Encontrada" : "Não encontrada");
    printf("  search_ci(\"GaTo\"): %s (qualquer combinação de caso funciona)\n", search_ci(root_ci, "GaTo") ? "Encontrada" : "Não encontrada");
    
    return 0;
}