#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/ItemVendido.hpp"

TEST_CASE("Teste ItemVendido - Getters") {

    ItemVendido item("1", "Mouse", 50.0, 2);

    CHECK(item.get_idProduto() == "1");
    CHECK(item.get_nomeProduto() == "Mouse");
    CHECK(item.get_precoNaVenda() == doctest::Approx(50.0));
    CHECK(item.get_quantidade() == 2);
}

TEST_CASE("Teste itemVendido - Subtotal") {

    ItemVendido item("10", "Teclado", 100.0, 3);

    CHECK(item.get_subtotal() == doctest::Approx(300.0));
}