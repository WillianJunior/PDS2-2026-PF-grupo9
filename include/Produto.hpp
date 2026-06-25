#ifndef PRODUTO_HPP
#define PRODUTO_HPP

#include <string>

// Entidade do catálogo: um Produto não sabe nada sobre carrinho, transação
// ou tela - só guarda o estado dele e as regras que dizem respeito só a ele
// (baixar estoque, ficar inativo). Quem decide QUANDO vender/trocar é o
// SistemaEscambo (Model); o Produto só executa a consequência no próprio
// estado quando mandado.
class Produto {
private:
    std::string _id;
    std::string _nome;
    double _preco;
    std::string _categoria;
    std::string _subcategoria;
    std::string _loginAnunciante;
    bool _ativo;
    int _quantidadeEstoque;

public:
    // O '= true' faz com que não precisemos mudar o código onde o Produto é criado
    Produto(std::string id, std::string nome, double preco, std::string categoria, std::string subcategoria, std::string loginAnunciante, bool ativo = true, int quantidadeEstoque = 1);

    // REVISÃO: getters de string (id, nome, categoria, subcategoria, login do
    // anunciante) agora devolvem const std::string& em vez de std::string por
    // valor. Esses getters são chamados toda hora pra montar listagem da
    // vitrine (um loop por produto, várias vezes); evitar uma cópia de string
    // por chamada é uma otimização real e segura, já que quem chama nunca
    // guarda a referência além do tempo de vida do Produto.
    const std::string& get_id() const;
    const std::string& get_nome() const;
    double get_preco() const;
    const std::string& get_categoria() const;
    const std::string& get_subcategoria() const;
    const std::string& get_login_anunciante() const;
    bool is_ativo() const;
    int get_quantidade_estoque() const;

    void set_nome(const std::string& nome);
    void set_preco(double preco);
    void set_categoria(const std::string& categoria);
    void set_subcategoria(const std::string& subcategoria);
    void set_ativo(bool ativo);
    void set_quantidade_estoque(int quantidade);

    // Subtrai 'qtd' do estoque. Se o estoque chegar a 0, o produto e desativado
    // automaticamente (deixa de aparecer nas vitrines). Retorna false se nao
    // havia estoque suficiente (nao desconta nada nesse caso).
    bool baixarEstoque(int qtd);
};

#endif
