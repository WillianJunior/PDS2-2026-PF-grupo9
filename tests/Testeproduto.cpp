#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Produto.hpp"

TEST_CASE("Produto - Construtor e Getters") {
    Produto p("P01", "Cadeira Gamer", 1200.50, "Moveis", "Escritorio", "vendedor@gmail.com");

    CHECK(p.get_id() == "P01");
    CHECK(p.get_nome() == "Cadeira Gamer");
    CHECK(p.get_preco() == doctest::Approx(1200.50));
    CHECK(p.get_categoria() == "Moveis");
    CHECK(p.get_subcategoria() == "Escritorio");
    CHECK(p.get_login_anunciante() == "vendedor@gmail.com");
    CHECK(p.is_ativo() == true);
    CHECK(p.get_quantidade_estoque() == 1); // default
}

TEST_CASE("Produto - Setters") {
    Produto p("P02", "Mouse", 50.0, "Eletrodomestico", "Escritorio", "v@g.com");

    p.set_nome("Teclado");
    CHECK(p.get_nome() == "Teclado");

    p.set_preco(199.99);
    CHECK(p.get_preco() == doctest::Approx(199.99));

    p.set_categoria("Roupa");
    CHECK(p.get_categoria() == "Roupa");

    p.set_subcategoria("Camisa");
    CHECK(p.get_subcategoria() == "Camisa");

    p.set_ativo(false);
    CHECK(p.is_ativo() == false);
    p.set_ativo(true);
    CHECK(p.is_ativo() == true);

    p.set_quantidade_estoque(42);
    CHECK(p.get_quantidade_estoque() == 42);
}

TEST_CASE("Produto - baixarEstoque sucesso parcial") {
    Produto p("P03", "Notebook", 3000.0, "Eletrodomestico", "Escritorio", "v@g.com", true, 5);
    CHECK(p.baixarEstoque(3) == true);
    CHECK(p.get_quantidade_estoque() == 2);
    CHECK(p.is_ativo() == true);
}

TEST_CASE("Produto - baixarEstoque zera e desativa") {
    Produto p("P04", "Notebook", 3000.0, "Eletrodomestico", "Escritorio", "v@g.com", true, 3);
    CHECK(p.baixarEstoque(3) == true);
    CHECK(p.get_quantidade_estoque() == 0);
    CHECK(p.is_ativo() == false);
}

TEST_CASE("Produto - baixarEstoque rejeita quantidade invalida") {
    Produto p("P05", "Caneta", 2.0, "Outros", "Geral", "v@g.com", true, 5);

    CHECK(p.baixarEstoque(0)  == false);
    CHECK(p.get_quantidade_estoque() == 5);

    CHECK(p.baixarEstoque(-1) == false);
    CHECK(p.get_quantidade_estoque() == 5);

    CHECK(p.baixarEstoque(6)  == false);
    CHECK(p.get_quantidade_estoque() == 5);
}
