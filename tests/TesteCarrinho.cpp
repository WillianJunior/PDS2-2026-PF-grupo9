#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Carrinho.h"
#include "../include/produto.h"

TEST_CASE("Teste Carrinho - Adicionar Produtos") {

    Carrinho carrinho;

    Produto produto1(1, "Mouse", 20.0);
    Produto produto2(2, "Teclado", 100.0);

    carrinho.adicionarProduto(&produto1, 2);
    carrinho.adicionarProduto(&produto2, 1);

    CHECK(carrinho.get_itens().size() == 2);
}

TEST_CASE("Teste Carrinho - Calcular Total") {

    Carrinho carrinho;

    Produto produto1(1, "Mouse", 20.0);

    carrinho.adicionarProduto(&produto1, 2);

    CHECK(carrinho.calcularTotal() == doctest::Approx(40.0));
}

TEST_CASE("Teste Carrinho - Limpar Carrinho") {

    Carrinho carrinho;

    Produto produto1(1, "Mouse", 20.0);

    carrinho.adicionarProduto(&produto1, 1);

    carrinho.limparCarrinho();

    CHECK(carrinho.get_itens().empty());
}
