#ifndef CARRINHO_H
#define CARRINHO_H

#include <vector>
#include "produto.h"

using namespace std;

struct ItemCarrinho {
    Produto* produto;
    int quantidade;
};

class Carrinho {
private:
    vector<ItemCarrinho> _itens;

public:
    void adicionarProduto(Produto* produto, int quantidade);
    void removerProduto(int idProduto);
    void limparCarrinho();
    
    double calcularTotal() const;
    const vector<ItemCarrinho>& get_itens() const; 
};

#endif