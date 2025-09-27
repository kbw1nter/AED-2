# Análise de Complexidade e Justificativa
A complexidade das operações é determinada pelo comprimento `L` da palavra, e não pelo número total de palavras `N` no léxico.

## 1. Inserção (`insert`)

**Complexidade:** `O(L)`

**Justificativa:** O algoritmo de inserção percorre a Trie a partir da raiz. Para cada um dos `L` caracteres da palavra a ser inserida, ele realiza um conjunto de operações de tempo constante:

1. Converte o caractere para minúsculo.
2. Calcula um índice para o array de `filhos`.
3. Verifica se o ponteiro na posição do índice é nulo.
4. Caso seja nulo, aloca memória para um novo nó.
5. Avança para o próximo nó.

Como essas operações são repetidas uma vez para cada caractere da palavra, o tempo total de execução é diretamente proporcional ao comprimento da palavra, `L`. Portanto, a complexidade de tempo é `O(L)`.

## 2. Busca (`search`)

**Complexidade:** `O(L)`

**Justificativa:** O algoritmo itera pelos `L` caracteres da palavra-alvo, e a cada passo, desce um nível na árvore. Cada passo envolve apenas o cálculo de um índice e uma verificação de ponteiro, que são operações de tempo constante (`O(1)`).

## 3. Edição de Polaridade (`editPolarity`)

**Complexidade:** `O(L)`

**Justificativa:** A operação de edição é, na verdade, uma composição de duas sub-operações: uma busca e uma atribuição.

1. **Busca:** Primeiro, o algoritmo deve localizar o nó terminal correspondente a palavra que se deseja editar. Conforme justificado acima, esta etapa de busca tem uma complexidade de tempo de `O(L)`.
2. **Atribuição:** Uma vez que o nó é encontrado, a alteração da sua polaridade consiste em uma única operação (`pCurrent->polarity = newPolarity`), esta é uma operação de tempo constante `O(1)`.