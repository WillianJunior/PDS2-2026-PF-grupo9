#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/GerenciadorUsuarios.hpp"
#include <cstdio>
#include <fstream>
#include <string>

// RAII: renomeia o arquivo de persistência antes do teste e restaura no fim.
// Garante que cada TEST_CASE começa com um estado limpo e que os dados reais
// não são corrompidos mesmo que o teste falhe no meio.
struct GuardaUsuarios {
    static constexpr const char* ORIG = "data/usuarios.txt";
    static constexpr const char* BAK  = "_testbak_usuarios.txt";
    bool tinha;

    GuardaUsuarios()  { tinha = (std::rename(ORIG, BAK) == 0); }
    ~GuardaUsuarios() {
        std::remove(ORIG);
        if (tinha) std::rename(BAK, ORIG);
    }
};

TEST_CASE("GerenciadorUsuarios - Registrar e autenticar") {
    GuardaUsuarios g;
    GerenciadorUsuarios gu;

    bool ok = gu.registrarUsuario("Ana Teste", "ana@gu.com", "senha1", "pix-ana");
    CHECK(ok == true);

    SUBCASE("login duplicado retorna false") {
        CHECK(gu.registrarUsuario("Outro", "ana@gu.com", "outra") == false);
    }

    SUBCASE("autenticar credenciais corretas") {
        Usuario* u = gu.autenticar("ana@gu.com", "senha1");
        REQUIRE(u != nullptr);
        CHECK(u->getNome() == "Ana Teste");
        CHECK(u->getChavePix() == "pix-ana");
    }

    SUBCASE("autenticar senha errada retorna nullptr") {
        CHECK(gu.autenticar("ana@gu.com", "errada") == nullptr);
    }

    SUBCASE("autenticar login inexistente retorna nullptr") {
        CHECK(gu.autenticar("ninguem@gu.com", "senha1") == nullptr);
    }
}

TEST_CASE("GerenciadorUsuarios - Buscar por login") {
    GuardaUsuarios g;
    GerenciadorUsuarios gu;
    gu.registrarUsuario("Bia Teste", "bia@gu.com", "pass");

    SUBCASE("encontra usuario existente") {
        auto* u = gu.buscarUsuarioPorLogin("bia@gu.com");
        REQUIRE(u != nullptr);
        CHECK(u->getNome() == "Bia Teste");
    }

    SUBCASE("retorna nullptr para login inexistente") {
        CHECK(gu.buscarUsuarioPorLogin("fantasma@gu.com") == nullptr);
    }
}

TEST_CASE("GerenciadorUsuarios - Progressao via notificacoes") {
    GuardaUsuarios g;
    GerenciadorUsuarios gu;
    gu.registrarUsuario("Carlos Teste", "carlos@gu.com", "abc");

    SUBCASE("notificarCompraConcluida desbloqueia TROCAR") {
        gu.notificarCompraConcluida("carlos@gu.com");
        auto* u = gu.buscarUsuarioPorLogin("carlos@gu.com");
        REQUIRE(u != nullptr);
        CHECK(u->getComprasConcluidas() == 1);
        CHECK(u->temCapacidade(Capacidade::TROCAR) == true);
    }

    SUBCASE("notificarVendaConcluida incrementa contador") {
        gu.notificarVendaConcluida("carlos@gu.com");
        auto* u = gu.buscarUsuarioPorLogin("carlos@gu.com");
        REQUIRE(u != nullptr);
        CHECK(u->getVendasTrocasConcluidas() == 1);
    }

    SUBCASE("notificar login inexistente nao crasha") {
        // Deve retornar graciosamente sem efeito colateral
        gu.notificarCompraConcluida("ninguem@gu.com");
        gu.notificarVendaConcluida("ninguem@gu.com");
        CHECK(true);
    }
}

TEST_CASE("GerenciadorUsuarios - Persistencia (roundtrip)") {
    GuardaUsuarios g;

    {
        GerenciadorUsuarios gu;
        gu.registrarUsuario("Diana Teste", "diana@gu.com", "pwd", "pix-d");
        gu.notificarCompraConcluida("diana@gu.com"); // persiste alteracao
    }

    // Novo gerenciador relê o arquivo
    GerenciadorUsuarios gu2;
    auto* u = gu2.buscarUsuarioPorLogin("diana@gu.com");
    REQUIRE(u != nullptr);
    CHECK(u->getNome() == "Diana Teste");
    CHECK(u->getChavePix() == "pix-d");
    CHECK(u->temCapacidade(Capacidade::COMPRAR)  == true);
    CHECK(u->temCapacidade(Capacidade::ANUNCIAR) == true);
    CHECK(u->temCapacidade(Capacidade::TROCAR)   == true);
    CHECK(u->getComprasConcluidas() == 1);
}
