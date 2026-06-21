#ifndef ITEM_VENDIDO_HPP
#define ITEM_VENDIDO_HPP

#include <string>

// Representa uma "foto" de um produto no momento em que ele entrou no
// carrinho: guardamos o preço (_precoNaVenda) separado do Produto original
// de propósito. Se o vendedor mudar o preço depois, o item que já está no
// carrinho do comprador não pode ser afetado retroativamente - daí a
// duplicação dos dados em vez de só guardar o id e ir buscar o preço atual.
class ItemVendido {
private:
    std::string _idProduto;
    std::string _nomeProduto;
    double _precoNaVenda;
    int _quantidade;

public:
    ItemVendido(std::string id, std::string nome, double preco, int qtd);

    // REVISÃO: getters de string devolvendo const& (mesmo motivo do Produto/Usuario).
    const std::string& get_idProduto() const;
    const std::string& get_nomeProduto() const;
    double get_precoNaVenda() const;
    int get_quantidade() const;
    double get_subtotal() const;
};

#endif
