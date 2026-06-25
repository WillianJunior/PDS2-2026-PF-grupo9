#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Compra.hpp"
#include "../include/Troca.hpp"
#include "../include/Transacao.hpp"
#include "../include/Usuario.hpp"
#include "../include/Produto.hpp"
#include "../include/Anuncio.hpp"

TEST_CASE("Transacao - Getters e setters da base") {
    Usuario u1(1, "A", "a@t.com", "1");
    Usuario u2(2, "B", "b@t.com", "2");
    Produto p("P1", "X", 10.0, "Outros", "Geral", "a@t.com");
    Anuncio a(1, &p, &u1, 1);
    Compra c("ID001", &u1, &u2, &a, 1);

    CHECK(c.get_id_transacao() == "ID001");
    CHECK(c.get_usuario_proponente() == &u1);
    CHECK(c.get_usuario_receptor()   == &u2);
    CHECK(c.get_status() == StatusTransacao::PENDENTE);

    c.set_status(StatusTransacao::ACEITA);
    CHECK(c.get_status() == StatusTransacao::ACEITA);

    c.set_mensagem("ola");
    CHECK(c.get_mensagem() == "ola");

    c.set_data_criacao("2026-01-01");
    CHECK(c.get_data_criacao() == "2026-01-01");
}

TEST_CASE("Compra - valida com estoque suficiente") {
    Usuario comprador(1, "Comp", "c@t.com", "p");
    Usuario vendedor(2, "Vend", "v@t.com", "p");
    Produto p1("PROD1", "Notebook", 3000.0, "Eletronicos", "Informatica", "v@t.com");
    Anuncio anuncio(1, &p1, &vendedor, 5);

    SUBCASE("valor total calculado") {
        Compra c("C001", &comprador, &vendedor, &anuncio, 2);
        CHECK(c.get_valor_total() == doctest::Approx(6000.0));
        CHECK(c.get_quantidade() == 2);
        CHECK(c.get_anuncio() == &anuncio);
    }

    SUBCASE("validacao e execucao OK") {
        Compra c("C002", &comprador, &vendedor, &anuncio, 2);
        CHECK(c.validar_transacao() == true);
        c.executar_transacao();
        CHECK(c.get_status() == StatusTransacao::CONCLUIDA);
    }

    SUBCASE("estoque insuficiente invalida") {
        Compra c("C003", &comprador, &vendedor, &anuncio, 10);
        CHECK(c.validar_transacao() == false);
        c.executar_transacao();
        CHECK(c.get_status() == StatusTransacao::REJEITADA);
    }

    SUBCASE("anuncio inativo invalida") {
        anuncio.set_status(false);
        Compra c("C004", &comprador, &vendedor, &anuncio, 1);
        CHECK(c.validar_transacao() == false);
    }
}

TEST_CASE("Troca - margem de preco") {
    Usuario user1(3, "U1", "u1@t.com", "1");
    Usuario user2(4, "U2", "u2@t.com", "1");

    Produto pAlvo   ("PA", "PS5",           4000.0, "Games", "Console", "u2@t.com");
    Produto pOferta ("PO", "Xbox Series X", 3800.0, "Games", "Console", "u1@t.com");
    Produto pBarato ("PB", "Jogo Tabuleiro",  200.0, "Jogos", "Fisico",  "u1@t.com");

    Anuncio anAlvo   (1, &pAlvo,   &user2, 1);
    Anuncio anOferta (2, &pOferta, &user1, 1);
    Anuncio anBarato (3, &pBarato, &user1, 1);

    SUBCASE("troca valida dentro de 20%") {
        Troca t("T001", &user1, &user2, &anAlvo, &anOferta);
        CHECK(t.get_anuncio_alvo()     == &anAlvo);
        CHECK(t.get_anuncio_ofertado() == &anOferta);
        CHECK(t.get_quantidade_alvo()     == 1);
        CHECK(t.get_quantidade_ofertada() == 1);
        CHECK(t.verificar_precos_similares() == true);
        CHECK(t.validar_transacao() == true);
        t.executar_transacao();
        CHECK(t.get_status() == StatusTransacao::CONCLUIDA);
    }

    SUBCASE("troca invalida por discrepancia de preco") {
        Troca t("T002", &user1, &user2, &anAlvo, &anBarato);
        CHECK(t.verificar_precos_similares() == false);
        CHECK(t.validar_transacao() == false);
        t.executar_transacao();
        CHECK(t.get_status() == StatusTransacao::REJEITADA);
    }

    SUBCASE("anuncio inativo invalida a troca") {
        anAlvo.set_status(false);
        Troca t("T003", &user1, &user2, &anAlvo, &anOferta);
        CHECK(t.validar_transacao() == false);
    }
}

TEST_CASE("Compra - valor total zero quando anuncio nulo") {
    Usuario u1(1, "A", "a@t.com", "1");
    Usuario u2(2, "B", "b@t.com", "2");
    Compra c("C999", &u1, &u2, nullptr, 2);
    CHECK(c.get_valor_total() == doctest::Approx(0.0));
    CHECK(c.validar_transacao() == false);
}
