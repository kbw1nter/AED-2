#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_DIGITS 10

// estrutura para a lista de palavras dentro de um nó
typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

// estrutura do nó da trie.
typedef struct TrieNode {
    struct TrieNode *children[NUM_DIGITS];
    WordNode *wordsHere;
} TrieNode;

// converte um caractere para o dígito correspondente.
int charToDigit(char c) {
    c = tolower(c);
    switch (c) {
        case 'a': case 'b': case 'c': return 2;
        case 'd': case 'e': case 'f': return 3;
        case 'g': case 'h': case 'i': return 4;
        case 'j': case 'k': case 'l': return 5;
        case 'm': case 'n': case 'o': return 6;
        case 'p': case 'q': case 'r': case 's': return 7;
        case 't': case 'u': case 'v': return 8;
        case 'w': case 'x': case 'y': case 'z': return 9;
        default: return -1;
    }
}

// criar e inicializar um novo nó da trie.
TrieNode* createNode() {
    TrieNode *newNode = (TrieNode*)malloc(sizeof(TrieNode));
    if (!newNode) {
        perror("Falha ao alocar memória para o nó da Trie");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUM_DIGITS; i++) {
        newNode->children[i] = NULL;
    }
    newNode->wordsHere = NULL;
    return newNode;
}

// insere uma palavra na trie
void insertWord(TrieNode *root, const char *word) {
    TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int digit = charToDigit(word[i]);
        if (digit == -1) {
            return; // ignora palavras com caracteres não mapeáveis
        }

        if (currentNode->children[digit] == NULL) {
            currentNode->children[digit] = createNode();
        }
        currentNode = currentNode->children[digit];
    }

    // adicionar a palavra na lista do nó final
    WordNode *newWordNode = (WordNode*)malloc(sizeof(WordNode));
    newWordNode->word = strdup(word);
    newWordNode->next = currentNode->wordsHere;
    currentNode->wordsHere = newWordNode;
}

// imprimir recursivamente todas as palavras a partir de um nó
int printWordsFromNode(TrieNode *node) {
    if (node == NULL) {
        return 0;
    }
    
    int wordsFound = 0;

    // imprimir as palavras que terminam exatamente neste nó
    WordNode *currentWord = node->wordsHere;
    while (currentWord != NULL) {
        printf("- %s\n", currentWord->word);
        wordsFound++;
        currentWord = currentWord->next;
    }

    // cntinuar a busca recursivamente para todos os nós filhos
    for (int i = 0; i < NUM_DIGITS; i++) {
        wordsFound += printWordsFromNode(node->children[i]);
    }
    
    return wordsFound;
}

// buscar por palavras que correspondem a uma sequência de dígitos
void searchWords(TrieNode *root, const char *digits) {
    TrieNode *currentNode = root;

    // navega na trie até o nó correspondente da sequência
    for (int i = 0; digits[i] != '\0'; i++) {
        int digitIndex = digits[i] - '0';
        
        if (digitIndex < 0 || digitIndex >= NUM_DIGITS || currentNode->children[digitIndex] == NULL) {
            printf("Nenhuma palavra encontrada.\n");
            return;
        }
        currentNode = currentNode->children[digitIndex];
    }

    // a partir do nó encontrado, imprime todas as palavras possíveis
    printf("Palavras correspondentes a '%s':\n", digits);
    int totalFound = printWordsFromNode(currentNode);

    if (totalFound == 0) {
        printf("Nenhuma palavra encontrada.\n");
    }
}

// libera recursivamente toda memória alocada para a trie
void freeTrie(TrieNode *node) {
    if (node == NULL) {
        return;
    }

    // libra todos os filhos
    for (int i = 0; i < NUM_DIGITS; i++) {
        freeTrie(node->children[i]);
    }

    // libera a lista de palavras deste nó
    WordNode *currentWord = node->wordsHere;
    while (currentWord != NULL) {
        WordNode *temp = currentWord;
        free(temp->word);
        currentWord = currentWord->next;
        free(temp);
    }
    
    // libera o próprio nó
    free(node);
}

int main() {
    TrieNode *root = createNode();
    const char* fileName = "palavras.txt";
    
    // carregar o dicionário a partir do arquivo
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'.\n", fileName);
        return 1;
    }

    char wordBuffer[100];
    printf("Carregando dicionário... Aguarde.\n");
    while (fgets(wordBuffer, sizeof(wordBuffer), file)) {
        // remover o caractere de nova linha
        wordBuffer[strcspn(wordBuffer, "\n")] = 0;
        if (strlen(wordBuffer) > 0) {
             insertWord(root, wordBuffer);
        }
    }
    fclose(file);
    printf("Dicionário carregado. Sistema pronto.\n\n");

    // loop principal pra interface
    char inputBuffer[50];
    while (1) {
        printf("Digite uma sequência de dígitos (ou '0' para sair): ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        inputBuffer[strcspn(inputBuffer, "\n")] = 0;

        if (strcmp(inputBuffer, "0") == 0) {
            break;
        }
        
        if (strlen(inputBuffer) > 0) {
            searchWords(root, inputBuffer);
        }
        printf("\n");
    }

    // limpar a memória
    printf("Encerrando o programa e liberando memória...\n");
    freeTrie(root);
    printf("Feito.\n");

    return 0;
}