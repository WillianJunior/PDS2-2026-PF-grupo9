#ifndef ITEM_VENDIDO_H
#define ITEM_VENDIDO_H

#include <string>

using namespace std;

class ItemVendido {
private:
    int _idProduto;
    string _nomeProduto;
    double _precoNaVenda;
    int _quantidade;

public:
    ItemVendido(int id, string nome, double preco, int qtd);
    
    int get_idProduto() const;
    string get_nomeProduto() const;
    double get_precoNaVenda() const;
    int get_quantidade() const;
    double get_subtotal() const; 
};

#endif