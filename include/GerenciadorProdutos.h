#ifndef GERENCIADOR_PRODUTOS_H
#define GERENCIADOR_PRODUTOS_H

#include <vector>
#include <string>
#include "produto.h"

class GerenciadorProdutos {
private:
    std::vector<Produto> _produtos;
    const std::string NOME_ARQUIVO = "produtos.txt";

    void carregarProdutos();
    void salvarProdutoNoArquivo(const Produto& produto);

public:
    GerenciadorProdutos();
    
    // Cadastra um produto e já grava no arquivo
    void cadastrarProduto(int id, const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria);
    
    // Retorna a lista de produtos para exibir na vitrine
    const std::vector<Produto>& get_produtos() const;
    
    // Busca um produto específico pelo ID (útil para o carrinho)
    Produto* buscarProdutoPorId(int id);
};

#endif