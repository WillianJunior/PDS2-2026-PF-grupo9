#ifndef CARRINHO_H
#define CARRINHO_H

#include <vector>
#include <string>
#include "ItemVendido.h"

class Carrinho {
private:
    std::vector<ItemVendido> _itens;

public:
    void adicionarItem(const ItemVendido& item);
    const std::vector<ItemVendido>& get_itens() const;
    double get_total() const;
    void esvaziar();
    
    // Novas funções para persistência no banco de dados
    void salvarCarrinho(const std::string& loginUsuario) const;
    void carregarCarrinho(const std::string& loginUsuario);
};

#endif