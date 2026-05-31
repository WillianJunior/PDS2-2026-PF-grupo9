#ifndef ITEM_VENDIDO_H
#define ITEM_VENDIDO_H

#include <string>

class ItemVendido {
private:
    std::string _idProduto; // <-- ID agora é string!
    std::string _nomeProduto;
    double _precoNaVenda;
    int _quantidade;

public:
    ItemVendido(std::string id, std::string nome, double preco, int qtd);
    
    std::string get_idProduto() const;
    std::string get_nomeProduto() const;
    double get_precoNaVenda() const;
    int get_quantidade() const;
    double get_subtotal() const; 
};

#endif