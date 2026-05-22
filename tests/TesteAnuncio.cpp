#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../include/Anuncio.h"
#include "../include/produto.h"
#include "../include/Usuario.h"

TEST_CASE("Teste Anuncio - Construtor") {

    Produto produto(1, "Mouse", 50.0);
    Usuario vendedor("Henrique", "henrique@gmail.com", "123");

    Anuncio anuncio(1, &produto, &vendedor, 10);

    CHECK(anuncio.get_produto() == &produto);
    CHECK(anuncio.get_vendedor() == &vendedor);
    CHECK(anuncio.get_quantidade() == 10);
}

TEST_CASE("Teste Anuncio - Status") {

    Produto produto(1, "Mouse", 50.0);
    Usuario vendedor("Henrique", "henrique@gmail.com", "123");

    Anuncio anuncio(1, &produto, &vendedor, 10);

    anuncio.set_status(true);
    CHECK(anuncio.get_status() == true);

    anuncio.set_status(false);
    CHECK(anuncio.get_status() == false);
}

TEST_CASE("Teste Anuncio - Validacao") {

    Produto produto(1, "Mouse", 50.0);
    Usuario vendedor("Henrique", "henrique@gmail.com", "123");

    Anuncio anuncio(1, &produto, &vendedor, 10);

    CHECK(anuncio.validar_anuncio() == true);
}