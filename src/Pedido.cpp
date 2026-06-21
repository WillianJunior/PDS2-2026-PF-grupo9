#include "../include/Pedido.hpp"
#include <utility>

Pedido::Pedido(int idPedido, std::string loginComprador)
    : _idPedido(idPedido), _loginComprador(std::move(loginComprador)), _status("Aguardando Pagamento") {}

// Guarda uma cópia do ItemVendido (que já é, ele próprio, uma "foto" do
// produto - ver nota em ItemVendido.hpp) no pedido, e não uma referência.
void Pedido::adicionarItem(const ItemVendido& item) {
    _itens.push_back(item);
}

void Pedido::set_status(const std::string& novoStatus) { _status = novoStatus; }

int Pedido::get_idPedido() const { return _idPedido; }
const std::string& Pedido::get_loginComprador() const { return _loginComprador; }
const std::string& Pedido::get_status() const { return _status; }

const std::vector<ItemVendido>& Pedido::get_itens() const { return _itens; }

double Pedido::calcularTotalPedido() const {
    double total = 0.0;
    for (const auto& item : _itens) {
        total += item.get_subtotal();
    }
    return total;
}
