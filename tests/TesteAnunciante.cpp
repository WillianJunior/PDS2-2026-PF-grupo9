#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Anunciante.h"

TEST_CASE("Teste Anunciante - Caso Base") {

    Anunciante anunciante("Henrique", "henrique@gmail.com", "123");

    CHECK(true);
}