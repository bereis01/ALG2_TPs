# Trabalho Prático 01
DCC207 - Algoritmos 2 | Universidade Federal de Minas Gerais | 2023/01

Bernardo Reis de Almeida

# Introdução
  O presente trabalho prático tem por objetivo a manifestação e o desenvolvimento das habilidades teóricas trabalhadas sobre o tema algoritmos para manipulação de sequências, o que será feito em uma aplicação prática, seja ela a compressão de arquivos de texto.
  
  A fim de atingir essa meta, será implementado um programa que realiza a compressão desses arquivos pelo método LZ78, tomando como base uma árvore Trie compacta como estrutura de dados auxiliar. Desenvolvido em 1978 por Abraham Lempel e Jacob Ziv, tal algoritmo é baseado na ideia de se codificar sequências de caracteres que aparecem no texto e as armazenar em uma estrutura de dados que permita recuperar esses códigos e os substituir pelas aparições sucessivas da mesma sequência, de modo a reduzir a quantidade de bits utilizada para representar toda a informação. Usualmente, utiliza-se um dicionário para o mapeamento de códigos a sequências, mas, na presente implementação, isso será feito por meio de uma árvore Trie compacta, cujos nós armazenam prefixos e, possivelmente, seus códigos.
  
  A implementação foi feita por meio da linguagem C++ em um ambiente Linux (Debian GNU/Linux 11 (bullseye)). Um Makefile é disponibilizado para a geração do executável. Detalhes sobre o funcionamento do algoritmo e sobre as estruturas de dados utilizadas podem ser encontrados na seção [Implementação], enquanto as instruções para compilação e para execução podem ser encontradas na seção [Considerações Finais].
