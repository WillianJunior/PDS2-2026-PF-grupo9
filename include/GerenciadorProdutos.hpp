#ifndef GERENCIADOR_PRODUTOS_HPP
#define GERENCIADOR_PRODUTOS_HPP

#include <vector>
#include <string>
#include "Produto.hpp"

class GerenciadorProdutos {
private:
    std::vector<Produto> _produtos;
    const std::string NOME_ARQUIVO = "produtos.txt";

    void carregarProdutos();
    void salvarProdutoNoArquivo(const Produto& produto);
    void atualizarArquivoCompleto(); 

public:
    GerenciadorProdutos();
    
    void cadastrarProduto(const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria, const std::string& loginAnunciante, int idUsuarioLogado);
    
    // ID agora é string
    bool editarProduto(const std::string& id, const std::string& novoNome, double novoPreco, const std::string& novaCat, const std::string& novaSub);
   
    bool inativarProduto(const std::string& id);

    std::vector<Produto*> buscarProdutosPorUsuario(const std::string& loginUsuario);
    
    const std::vector<Produto>& get_produtos() const;
    Produto* buscarProdutoPorId(const std::string& id); // ID agora é string
};

#endif