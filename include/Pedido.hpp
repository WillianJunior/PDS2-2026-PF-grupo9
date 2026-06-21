#ifndef PEDIDO_HPP
#define PEDIDO_HPP

#include <vector>
#include <string>
#include "ItemVendido.hpp"

// Registro de um pedido fechado (snapshot dos itens comprados + status de
// pagamento). Assim como a Compra (ver nota em Compra.hpp), esta classe está
// pronta e testada (TestePedido.cpp) mas ainda não é instanciada em nenhum
// lugar do fluxo real do TerminalUI - hoje o "Finalizar Compra" não cria um
// Pedido, só esvazia o Carrinho direto. Documentando aqui pra não passar a
// impressão de que o pedido fica "perdido" por bug: ele nunca chegou a ser
// criado porque essa integração ainda não foi feita.
class Pedido {
private:
    int _idPedido;
    std::string _loginComprador;
    std::vector<ItemVendido> _itens;
    std::string _status;

public:
    // REVISÃO: corpo dos métodos movido pra Pedido.cpp. Antes a classe inteira
    // vinha implementada dentro do .hpp (única assim no projeto) - padronizamos
    // pra declaração no .hpp / implementação no .cpp, igual ao resto das classes.
    Pedido(int idPedido, std::string loginComprador);

    void adicionarItem(const ItemVendido& item);

    void set_status(const std::string& novoStatus);

    int get_idPedido() const;
    const std::string& get_loginComprador() const;
    const std::string& get_status() const;

    const std::vector<ItemVendido>& get_itens() const;

    // Calcula o total somando os subtotais de cada ItemVendido
    double calcularTotalPedido() const;
};

#endif
