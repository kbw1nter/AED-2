#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // pra usar true ou false
#include <ctype.h> // pra usar o tolower

#define ALPHAET_SIZE 26

struct TrieNode{
    struct TrieNode *children[ALPHAET_SIZE];
    int sentimentScore; // 0 se não for o fim da palavra ou o score do sentimento
};

// retorna um novo no da trie
struct TrieNode *creatNode(void){

    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); //aloca memoria para o novo no

    if(pNode){
        pNode->sentimentScore = 0; //inicializa o score como 0

        for(int i = 0; i < ALPHAET_SIZE; i++){
            pNode->children[i] = NULL; //inicializa todos os filhos como NULL
        }
    }
    return pNode;
}

//inserir uma palavra na trie
void insert(struct TrieNode *root, const char *word, int score){
    struct TrieNode *pCurrent = root;

    for (int i = 0; i < strlen(word); i++) {
        //calcular o indice correspondente da letra 
        int index = tolower(word[i]) - 'a';
        //se o caminho não existir vai criar um novo no
        if (!pCurrent->children[index]){
           pCurrent->children[index] = creatNode();
        }
        //mover pro prox no 
        pCurrent = pCurrent->children[index];
    }
    //atribuir o score ao no do fim da palavra
    pCurrent->sentimentScore = score;
}

//busca uma palavra na trie e retorna score
int getSentiment(struct TrieNode *root, const char *word){
    struct TrieNode *pCurrent = root;

    for (int i = 0; i < strlen(word); i++) {
        int index = tolower(word[i]) - 'a';
        //se o caminho não existir a palavra não existe na trie
        if (!pCurrent->children[index]){
            return 0; //palavra não encontrada
        }
        pCurrent = pCurrent->children[index];
    }
    //retorna o score do no
    return pCurrent->sentimentScore;
}

//liberar a memoria alocada para a trie
void freeTrie(struct TrieNode* node){
    if (node == NULL){
        return;
    }
    //libera todos os filhos
    for (int i = 0; i < ALPHAET_SIZE; i++)
    {
        freeTrie(node->children[i]);
    }
    free(node); //libera o no atual
}

int main(){
    //criar a riz da trie
    struct TrieNode *root = creatNode();

    //inserir palavras na trie
    insert(root, "bom", 1);
    insert(root, "otimo", 2);
    insert(root, "incrivel", 2);
    insert(root, "ruim", -1);
    insert(root, "pessimo", -2);
    insert(root, "horrivel", -2);

    // teste
    char frase[] = "O aplicativo e otimo , mas tem alguns bugs que faz a experiencia ser ruim";
    int scoreTotal = 0;
    printf("Analisando frase: \"%s\"\n", frase);

    //strtok é usado pra dividir a frase em palavras
    char *palavra = strtok(frase, " ");

    while (palavra != NULL)
    {
        int scorePalavra = getSentiment(root, palavra);
        printf("Palavra: %s, Score: %d\n", palavra, scorePalavra);
        scoreTotal += scorePalavra;

        palavra = strtok(NULL, " ");
    }
    
    printf("Score final da frase: %d\n", scoreTotal); 
    if (scoreTotal > 0){
        printf("Sentimento positivo\n");
    } else if (scoreTotal < 0){
        printf("Sentimento negativo\n");
    } else {
        printf("Sentimento neutro\n");
    }
    
    freeTrie(root); //liberar a memoria alocada para a trie
    return 0;
}