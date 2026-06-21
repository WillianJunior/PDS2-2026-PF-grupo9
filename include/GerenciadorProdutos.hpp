#ifndef GERENCIADOR_PRODUTOS_HPP
#define GERENCIADOR_PRODUTOS_HPP

#include <deque>
#include <vector>
#include <string>
#include "Produto.hpp"

class GerenciadorProdutos {
private:
    // REVISÃO (bug real corrigido): mesmo motivo do GerenciadorUsuarios - este
    // container entrega Produto* (buscarProdutoPorId) que ficam guardados a
    // longo prazo dentro de Anuncio::_produto. std::vector reloca o buffer
    // inteiro quando cresce além da capacidade, invalidando esses ponteiros
    // (dangling pointer). std::deque nunca invalida ponteiros/referências pra
    // elementos existentes ao inserir no fim, então resolve o problema de
    // raiz sem precisar reescrever quem consome esses ponteiros.
    std::deque<Produto> _produtos;

    const std::string NOME_ARQUIVO = "produtos.txt";

    void carregarProdutos();
    void salvarProdutoNoArquivo(const Produto& produto);
    void atualizarArquivoCompleto();

public:
    GerenciadorProdutos();

    void cadastrarProduto(const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria, const std::string& loginAnunciante, int idUsuarioLogado, int quantidadeEstoque = 1);

    // ID agora é string
    bool editarProduto(const std::string& id, const std::string& novoNome, double novoPreco, const std::string& novaCat, const std::string& novaSub);

    bool inativarProduto(const std::string& id);

    // Subtrai 'quantidadeVendida' do estoque do produto (venda ou troca concluida).
    // Se o estoque chegar a 0, o produto e desativado automaticamente e some das vitrines.
    bool venderProduto(const std::string& id, int quantidadeVendida);

    std::vector<Produto*> buscarProdutosPorUsuario(const std::string& loginUsuario);

    // REVISÃO: get_produtos() agora devolve const std::deque<Produto>& (era
    // std::vector<Produto>&) - acompanha a troca do container acima. Todo
    // código que consome isso usa "const auto&", então essa troca não exige
    // nenhuma mudança em quem chama.
    const std::deque<Produto>& get_produtos() const;
    Produto* buscarProdutoPorId(const std::string& id); // ID agora é string
};

#endif
