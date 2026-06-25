#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Anuncio.hpp"
#include "../include/Produto.hpp"
#include "../include/Usuario.hpp"

TEST_CASE("Anuncio - Construtor e Getters") {
    Produto produto("1", "Mouse", 50.0, "Eletrodomestico", "Escritorio", "henrique@gmail.com");
    Usuario vendedor(3, "Henrique", "henrique@gmail.com", "123");
    Anuncio anuncio(1, &produto, &vendedor, 10);

    CHECK(anuncio.get_produto() == &produto);
    CHECK(anuncio.get_vendedor() == &vendedor);
    CHECK(anuncio.get_quantidade() == 10);
}

TEST_CASE("Anuncio - Status") {
    Produto produto("1", "Mouse", 50.0, "Eletrodomestico", "Escritorio", "henrique@gmail.com");
    Usuario vendedor(3, "Henrique", "henrique@gmail.com", "123");
    Anuncio anuncio(2, &produto, &vendedor, 10);

    anuncio.set_status(true);
    CHECK(anuncio.get_status() == true);

    anuncio.set_status(false);
    CHECK(anuncio.get_status() == false);
}

TEST_CASE("Anuncio - validar_anuncio") {
    Produto produto("P1", "Teclado", 100.0, "Eletrodomestico", "Escritorio", "v@g.com");
    Usuario vendedor(1, "Vendedor", "v@g.com", "123");

    SUBCASE("quantidade positiva e valido") {
        Anuncio a(3, &produto, &vendedor, 5);
        CHECK(a.validar_anuncio() == true);
    }

    SUBCASE("quantidade zero e invalido") {
        Anuncio a(4, &produto, &vendedor, 0);
        CHECK(a.validar_anuncio() == false);
    }

    SUBCASE("quantidade negativa e invalido") {
        Anuncio a(5, &produto, &vendedor, -1);
        CHECK(a.validar_anuncio() == false);
    }
}
