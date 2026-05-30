#ifndef PEDIDO_H
#define PEDIDO_H

#include <vector>
#include <string>
#include "ItemVendido.h" 

class Pedido {
private:
    int _idPedido;
    std::string _loginComprador;
    std::vector<ItemVendido> _itens;
    std::string _status;          

public:
    // Construtor
    Pedido(int idPedido, std::string loginComprador)
        : _idPedido(idPedido), _loginComprador(loginComprador), _status("Aguardando Pagamento") {}

    // Adiciona a cópia do item vendido na lista do pedido
    void adicionarItem(const ItemVendido& item) {
        _itens.push_back(item);
    }

    void set_status(const std::string& novoStatus) { _status = novoStatus; }
    
    int get_idPedido() const { return _idPedido; }
    std::string get_loginComprador() const { return _loginComprador; }
    std::string get_status() const { return _status; }
    
    const std::vector<ItemVendido>& get_itens() const { return _itens; }
    
    // Calcula o total somando os subtotais de cada ItemVendido
    double calcularTotalPedido() const { 
        double total = 0.0;
        for (const auto& item : _itens) {
            total += item.get_subtotal();
        }
        return total;
    }
};

#endif