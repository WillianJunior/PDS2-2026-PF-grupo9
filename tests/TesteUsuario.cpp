#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Usuario.h"

TEST_CASE("Teste Usuario - Construtor e Getters") {

    Usuario usuario("Henrique", "henrique@gmail.com", "123");

    CHECK(usuario.getNome() == "Henrique");
    CHECK(usuario.getLogin() == "henrique@gmail.com");
    CHECK(usuario.getSenha() == "123");
}

TEST_CASE("Teste Usuario - Validacao de Login") {

    Usuario usuario("Henrique", "henrique@gmail.com", "123");

    CHECK(usuario.validarLogin("henrique@gmail.com", "123") == true);
    CHECK(usuario.validarLogin("errado", "123") == false);
    CHECK(usuario.validarLogin("henrique@gmail.com", "000") == false);
}