#ifndef CARRINHO_HPP
#define CARRINHO_HPP

#include <vector>
#include <string>
#include "ItemVendido.hpp"

// Carrinho não é dono de nenhum Produto - ele guarda "fotos" (ItemVendido)
// que o comprador acumulou antes de fechar a compra. Por isso o checkout
// (Carrinho::esvaziar) é seguro: não tem ponteiro nenhum pra invalidar aqui.
class Carrinho {
private:
    std::vector<ItemVendido> _itens;

public:
    // REVISÃO: parâmetro por valor (em vez de const&) + std::move dentro da
    // função. Quem já tem o ItemVendido pronto (ex: carregarCarrinho, que cria
    // um temporário) ganha um "move" em vez de cópia; quem passa uma variável
    // já existente (TerminalUI) paga o mesmo custo de antes - não há lado
    // pior, só um possível ganho.
    void adicionarItem(ItemVendido item);
    void removerItem(size_t indice);
    const std::vector<ItemVendido>& get_itens() const;
    double get_total() const;
    void esvaziar();
    
    // Novas funções para persistência no banco de dados
    void salvarCarrinho(const std::string& loginUsuario) const;
    void carregarCarrinho(const std::string& loginUsuario);
};

#endif