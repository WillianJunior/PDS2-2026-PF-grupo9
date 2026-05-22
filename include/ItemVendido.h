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
    ItemVendido(int id, string nome, double preco, int qtd){};
    
    int get_idProduto() const{return 0;};
    string get_nomeProduto() const{return "";};
    double get_precoNaVenda() const{return 0.0;};
    int get_quantidade() const{return 0;};
    double get_subtotal() const{return 0.0;}; 
};

#endif