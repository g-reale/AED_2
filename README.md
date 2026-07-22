<p align="center">
  <strong>Algorithms and Data Structures II</strong>
</p>

This repository gathers implementations developed throughout the **Algorithms and Data Structures II** course at the Federal University of São Paulo. Each structure is organized in its own folder, with code in C++, and covers from balanced search trees to graphs and hash tables, focusing on algorithm understanding and complexity analysis.

## Objective

- Implement and study advanced data structures in C++.
- Understand insertion, removal, and search operations in each structure.
- Analyze the performance and use cases of each approach.
- Develop skills for writing efficient and well-structured code.

## Technologies Used

- **Language:** C++
- **Compiler:** g++ (C++11 standard or higher)
- **Tools:** Make (when available), command line

## Project Structure

```bash
AED_2/
  └── programas/
      ├── ABB/                  # Binary Search Tree (BST)
      ├── AVL/                  # AVL Tree (self-balancing)
      ├── Arvore B/             # B-Tree (multi-way)
      ├── Arvore Vermelha e Preta/  # Red-Black Tree
      ├── Graph/                # Graphs and associated algorithms
      └── Hash/                 # Hash Tables
```

Each folder contains the implementation files (`.cpp`, `.h`) for the corresponding structure.

## Implemented Structures

### Binary Search Tree (BST)

- Base tree search structure.
- Operations: insertion, removal, and search in O(h), where h is the tree height.

### AVL Tree

- Binary search tree with automatic balancing via rotations.
- Guarantees O(log n) height in all cases.
- Operations: insertion, removal, and rebalancing.

### B-Tree

- Multi-way search tree optimized for disk-access systems.
- Generalization of the binary tree with multiple keys per node.

### Red-Black Tree

- Binary search tree with balancing via node coloring.
- Efficient alternative to AVL with fewer rotations during insertions.

### Graphs

- Graph representation and algorithms.
- May include Breadth-First Search (BFS), Depth-First Search (DFS), and shortest path algorithms.

### Hash Table

- Hash table implementation with collision handling.
- Allows search, insertion, and removal operations in average O(1).

## How to Run the Project

```bash
git clone https://github.com/g-reale/AED_2.git
cd AED_2/AED_2/programas
```

To compile and run a specific program, access the folder of the desired structure:

```bash
cd AVL
g++ -o avl main.cpp
./avl
```

> The exact compilation commands may vary depending on the internal organization of each folder.

## Environment Setup

- **C++11** or higher
- `g++` compiler installed
- Operating System: Linux, macOS, or Windows (with MinGW)

```bash
# Verify g++ version
g++ --version
```

## Workflow

1. The code for each structure defines the data types and available operations.
2. The main program (`main.cpp` or equivalent) demonstrates the use of the structure with test cases.
3. Operations are executed and results are displayed in the terminal.

---

<p align="center">
  <strong>Algoritmos e Estruturas de Dados II</strong>
</p>

Este repositório reúne implementações desenvolvidas ao longo da disciplina de **Algoritmos e Estruturas de Dados II** realizada na Universidade Federal de São Paulo. Cada estrutura está organizada em sua própria pasta, com código em C++, e abrange desde árvores de busca balanceadas até grafos e tabelas hash, com foco em compreensão dos algoritmos e análise de complexidade.

## Objetivo

- Implementar e estudar estruturas de dados avançadas em C++.
- Compreender as operações de inserção, remoção e busca em cada estrutura.
- Analisar o desempenho e os casos de uso de cada abordagem.
- Desenvolver habilidade de escrita de código eficiente e bem estruturado.

## Tecnologias Utilizadas

- **Linguagem:** C++
- **Compilador:** g++ (padrão C++11 ou superior)
- **Ferramentas:** Make (quando disponível), linha de comando

## Estrutura do Projeto

```bash
AED_2/
  └── programas/
      ├── ABB/                  # Árvore Binária de Busca
      ├── AVL/                  # Árvore AVL (auto-balanceada)
      ├── Arvore B/             # Árvore B (multi-caminho)
      ├── Arvore Vermelha e Preta/  # Árvore Rubro-Negra
      ├── Graph/                # Grafos e algoritmos associados
      └── Hash/                 # Tabelas Hash
```

Cada pasta contém os arquivos de implementação (`.cpp`, `.h`) referentes à estrutura correspondente.

## Estruturas Implementadas

### Árvore Binária de Busca (ABB)

- Estrutura base de busca em árvore.
- Operações: inserção, remoção e busca em O(h), onde h é a altura da árvore.

### Árvore AVL

- Árvore de busca binária com balanceamento automático por rotações.
- Garante altura O(log n) em todos os casos.
- Operações: inserção, remoção e rebalanceamento.

### Árvore B

- Árvore de múltiplos caminhos otimizada para sistemas com acesso a disco.
- Generalização da árvore binária com múltiplas chaves por nó.

### Árvore Rubro-Negra (Red-Black Tree)

- Árvore de busca binária com balanceamento por coloração de nós.
- Alternativa eficiente à AVL com menos rotações nas inserções.

### Grafos

- Representação e algoritmos sobre grafos.
- Pode incluir busca em largura (BFS), busca em profundidade (DFS) e algoritmos de caminho mínimo.

### Tabela Hash

- Implementação de tabela de dispersão com tratamento de colisões.
- Permite operações de busca, inserção e remoção em O(1) médio.

## Como Executar o Projeto

```bash
git clone https://github.com/g-reale/AED_2.git
cd AED_2/AED_2/programas
```

Para compilar e executar um programa específico, acesse a pasta da estrutura desejada:

```bash
cd AVL
g++ -o avl main.cpp
./avl
```

> Os comandos exatos de compilação podem variar conforme a organização interna de cada pasta.

## Configuração do Ambiente

- **C++11** ou superior
- Compilador `g++` instalado
- Sistema operacional: Linux, macOS ou Windows (com MinGW)

```bash
# Verificar versão do g++
g++ --version
```

## Fluxo de Funcionamento

1. O código de cada estrutura define os tipos de dados e as operações disponíveis.
2. O programa principal (`main.cpp` ou equivalente) demonstra o uso da estrutura com casos de teste.
3. Operações são executadas e os resultados são exibidos no terminal.