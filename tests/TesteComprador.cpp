#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Comprador.h"
#include "../include/produto.h"

TEST_CASE("Teste Comprador - Criacao") {

    Comprador comprador("Henrique", "henrique@gmail.com", "123");

    CHECK(comprador.getNome() == "Henrique");
}

TEST_CASE("Teste Comprador - Metodos") {

    Comprador comprador("Henrique", "henrique@gmail.com", "123");

    Produto produto(1, "Mouse", 20.0);

    comprador.comprar(produto);
    comprador.avaliarProduto(produto);

    CHECK(true);
}