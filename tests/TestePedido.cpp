#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Pedido.h"
#include "../include/ItemVendido.h"

TEST_CASE("Teste Pedido - Estado Inicial") {

    Pedido pedido(1, "henrique");

    CHECK(pedido.get_idPedido() == 1);
    CHECK(pedido.get_loginComprador() == "henrique");
    CHECK(pedido.get_itens().empty());
}

TEST_CASE("Teste Pedido - Adicionar Itens") {

    Pedido pedido(1, "henrique");

    ItemVendido item1(1, "Mouse", 50.0, 2);
    ItemVendido item2(2, "Teclado", 100.0, 1);

    pedido.adicionarItem(item1);
    pedido.adicionarItem(item2);

    CHECK(pedido.get_itens().size() == 2);
}

TEST_CASE("Teste Pedido - Status") {

    Pedido pedido(1, "henrique");

    pedido.set_status("ENVIADO");

    CHECK(pedido.get_status() == "ENVIADO");
}

