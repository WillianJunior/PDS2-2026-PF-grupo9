#ifndef PEDIDO_H
#define PEDIDO_H

#include <vector>
#include <string>
#include "ItemVendido.h" // Inclui a estrutura de histórico de itens

using namespace std;

class Pedido {
private:
    int _idPedido;
    string _loginComprador;
    vector<ItemVendido> _itens;
    string _status;          

public:
    Pedido(int idPedido, string loginComprador);

    void adicionarItem(const ItemVendido& item);
    void set_status(const string& novoStatus);
    
    int get_idPedido() const;
    string get_loginComprador() const;
    string get_status() const;
    const vector<ItemVendido>& get_itens() const;
    
    double calcularTotalPedido() const;
};

#endif