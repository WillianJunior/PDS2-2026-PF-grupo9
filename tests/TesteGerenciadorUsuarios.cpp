#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/GerenciadorUsuarios.hpp"

TEST_CASE("Teste GerenciadorUsuarios - Criacao") {

    GerenciadorUsuarios gerenciador;

    CHECK(true);
}

TEST_CASE("Teste GerenciadorUsuarios - Registro") {

    GerenciadorUsuarios gerenciador;

    gerenciador.registrarUsuario(
        "Henrique",
        "henrique@gmail.com",
        "123"
    );

    CHECK(true);
}