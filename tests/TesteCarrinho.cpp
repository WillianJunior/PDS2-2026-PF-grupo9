#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/Carrinho.h"

TEST_CASE("Teste de adição de produto ao carrinho") {
    Carrinho carrinho;
    Produto produto1("Produto 1", 10.0);
    Produto produto2("Produto 2", 20.0);

    carrinho.adicionarProduto(produto1);
    carrinho.adicionarProduto(produto2);

    CHECK(carrinho.getProdutos().size() == 2);
    CHECK(carrinho.getProdutos()[0].getNome() == "Produto 1");
    CHECK(carrinho.getProdutos()[1].getNome() == "Produto 2");
}
