#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Usuario.hpp"

TEST_CASE("Teste Usuario - Construtor e Getters") {

    Usuario usuario(3, "Henrique", "henrique@gmail.com", "123");

    CHECK(usuario.getNome() == "Henrique");
    CHECK(usuario.getLogin() == "henrique@gmail.com");
    CHECK(usuario.getSenha() == "123");
}

TEST_CASE("Teste Usuario - Validacao de Login") {

    Usuario usuario(3, "Henrique", "henrique@gmail.com", "123");

    CHECK(usuario.validarLogin("henrique@gmail.com", "123") == true);
    CHECK(usuario.validarLogin("errado", "123") == false);
    CHECK(usuario.validarLogin("henrique@gmail.com", "000") == false);
}

// Demonstração do sistema de capacidades/progressão descrito na refatoração
// da classe Usuario: usuário novo só tem COMPRAR+ANUNCIAR, ganha TROCAR na
// primeira compra concluída, é limitado a 3 anúncios ativos como INICIANTE e
// passa a VETERANO (sem limite de anúncios) após 3 vendas/trocas concluídas
// como vendedor.
TEST_CASE("Teste Usuario - Capacidades e progressao") {
    Usuario usuario(1, "Henrique", "henrique@gmail.com", "123");

    CHECK(usuario.temCapacidade(Capacidade::COMPRAR) == true);
    CHECK(usuario.temCapacidade(Capacidade::ANUNCIAR) == true);
    CHECK(usuario.temCapacidade(Capacidade::TROCAR) == false);

    usuario.registrarCompraConcluida();
    CHECK(usuario.temCapacidade(Capacidade::TROCAR) == true);
    CHECK(usuario.getComprasConcluidas() == 1);

    CHECK(usuario.getNivel() == NivelUsuario::INICIANTE);
    CHECK(usuario.getLimiteAnuncios() == Usuario::LIMITE_ANUNCIOS_INICIANTE);

    usuario.incrementarAnunciosAtivos();
    usuario.incrementarAnunciosAtivos();
    usuario.incrementarAnunciosAtivos();
    CHECK(usuario.getAnunciosAtivos() == 3);
    CHECK(usuario.podeCriarAnuncio() == false);

    usuario.registrarVendaConcluida();
    usuario.registrarVendaConcluida();
    usuario.registrarVendaConcluida();
    CHECK(usuario.getVendasTrocasConcluidas() == 3);
    CHECK(usuario.getNivel() == NivelUsuario::VETERANO);
    CHECK(usuario.getLimiteAnuncios() == Usuario::ANUNCIOS_ILIMITADO);
    CHECK(usuario.podeCriarAnuncio() == true);
}

TEST_CASE("Teste Usuario - Serializacao de capacidades") {
    Usuario usuario(2, "Igor", "igor@email.com", "abc");

    CHECK(usuario.serializarCapacidades() == "COMPRAR;ANUNCIAR");

    usuario.registrarCompraConcluida();
    CHECK(usuario.serializarCapacidades() == "COMPRAR;ANUNCIAR;TROCAR");

    CHECK(capacidadeParaString(Capacidade::TROCAR) == "TROCAR");
    CHECK(stringParaCapacidade("ANUNCIAR") == Capacidade::ANUNCIAR);
}

TEST_CASE("Teste Usuario - Decrementar anuncios nao fica negativo") {
    Usuario usuario(4, "Kaio", "kaio@email.com", "k9");

    usuario.decrementarAnunciosAtivos();
    CHECK(usuario.getAnunciosAtivos() == 0);

    usuario.incrementarAnunciosAtivos();
    usuario.decrementarAnunciosAtivos();
    usuario.decrementarAnunciosAtivos();
    CHECK(usuario.getAnunciosAtivos() == 0);
}