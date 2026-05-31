#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Carrinho.hpp"
#include "../include/itemVendido.hpp" 

TEST_CASE("Teste Carrinho - Adicionar e Contar Itens") {
    Carrinho carrinho;
    
    ItemVendido item1("1", "Mouse", 20.0, 2);
    ItemVendido item2("2", "Teclado", 100.0, 1);

    carrinho.adicionarItem(item1);
    carrinho.adicionarItem(item2);

    CHECK(carrinho.get_itens().size() == 2);
}

TEST_CASE("Teste Carrinho - Calcular Total") {
    Carrinho carrinho;
    
    ItemVendido item1("1", "Mouse", 20.0, 2); 
    carrinho.adicionarItem(item1);

    CHECK(carrinho.get_total() == doctest::Approx(40.0));
}

TEST_CASE("Teste Carrinho - Esvaziar") {
    Carrinho carrinho;
    ItemVendido item1("1", "Mouse", 20.0, 1);
    
    carrinho.adicionarItem(item1);
    carrinho.esvaziar();

    CHECK(carrinho.get_itens().empty() == true);
}