#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Produto.hpp"

TEST_CASE("Teste Produto - Retorno de Dados") {
    // Testa o construtor completo
    Produto p("P01", "Cadeira Gamer", 1200.50, "Moveis", "Escritorio", "vendedor@gmail.com");

    // Usando os nomes corretos das funções da classe Produto
    CHECK(p.get_id() == "P01");
    CHECK(p.get_nome() == "Cadeira Gamer");
    CHECK(p.get_preco() == doctest::Approx(1200.50));
    CHECK(p.get_categoria() == "Moveis");
    CHECK(p.get_subcategoria() == "Escritorio");
    CHECK(p.get_login_anunciante() == "vendedor@gmail.com");
}