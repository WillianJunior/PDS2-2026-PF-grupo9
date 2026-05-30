#ifndef ITEM_VENDIDO_H
#define ITEM_VENDIDO_H

#include <string>

class ItemVendido {
private:
    int _idProduto;
    std::string _nomeProduto;
    double _precoNaVenda;
    int _quantidade;

public:
    ItemVendido(int id, std::string nome, double preco, int qtd);
    
    int get_idProduto() const;
    std::string get_nomeProduto() const;
    double get_precoNaVenda() const;
    int get_quantidade() const;
    double get_subtotal() const; 
};

#endif