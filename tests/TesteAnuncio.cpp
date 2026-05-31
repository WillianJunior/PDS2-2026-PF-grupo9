#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Anuncio.hpp"
#include "../include/produto.hpp"
#include "../include/usuario.hpp"

TEST_CASE("Teste Anuncio - Construtor") {
    // Produto exige 6 campos (ID em string)
    Produto produto("1", "Mouse", 50.0, "Eletrodomestico", "Escritorio", "henrique@gmail.com");
    Usuario vendedor(3, "Henrique", "henrique@gmail.com", "123");

    // Anuncio exige ID em número inteiro (sem aspas!)
    Anuncio anuncio(1, &produto, &vendedor, 10);

    CHECK(anuncio.get_produto() == &produto);
    CHECK(anuncio.get_vendedor() == &vendedor);
    CHECK(anuncio.get_quantidade() == 10);
}

TEST_CASE("Teste Anuncio - Status") {
    Produto produto("1", "Mouse", 50.0, "Eletrodomestico", "Escritorio", "henrique@gmail.com");
    Usuario vendedor(3, "Henrique", "henrique@gmail.com", "123");

    Anuncio anuncio(2, &produto, &vendedor, 10);

    anuncio.set_status(true);
    CHECK(anuncio.get_status() == true);

    anuncio.set_status(false);
    CHECK(anuncio.get_status() == false);
}