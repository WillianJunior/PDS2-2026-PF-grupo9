#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Compra.hpp"
#include "../include/Troca.hpp"
#include "../include/Usuario.hpp"
#include "../include/Produto.hpp"
#include "../include/Anuncio.hpp"

TEST_CASE("Testando a classe Compra") {
    // Usuario: id(int), nome, email, senha
    Usuario comprador(1, "Comprador", "comprador@email.com", "senha123");
    Usuario vendedor(2, "Vendedor", "vendedor@email.com", "senha123");
    
    // Produto: id(string), nome, preco, categoria, subcategoria, loginAnunciante
    Produto p1("PROD1", "Notebook", 3000.0, "Eletronicos", "Informatica", "vendedor@email.com");
    Anuncio anuncio(1, &p1, &vendedor, 5); // ID 1, 5 unidades em estoque

    SUBCASE("Compra valida com estoque suficiente") {
        Compra c("C001", &comprador, &vendedor, &anuncio, 2); // Comprando 2 unidades
        
        CHECK(c.get_valor_total() == 6000.0);
        CHECK(c.validar_transacao() == true);
        
        c.executar_transacao();
        CHECK(c.get_status() == StatusTransacao::CONCLUIDA);
    }

    SUBCASE("Compra invalida por falta de estoque") {
        Compra c("C002", &comprador, &vendedor, &anuncio, 10); // Tentando comprar 10 (só tem 5)
        
        CHECK(c.validar_transacao() == false);
        
        c.executar_transacao();
        CHECK(c.get_status() == StatusTransacao::REJEITADA);
    }
}

TEST_CASE("Testando a classe Troca (e-scambo)") {
    Usuario user1(3, "User1", "u1@email.com", "123");
    Usuario user2(4, "User2", "u2@email.com", "123");

    Produto pAlvo("P_ALVO", "PS5", 4000.0, "Games", "Console", "u2@email.com");
    Produto pOferta("P_OFERTA", "Xbox Series X", 3800.0, "Games", "Console", "u1@email.com");
    Produto pInvalida("P_INV", "Jogo de Tabuleiro", 200.0, "Jogos", "Fisico", "u1@email.com");

    Anuncio anuncioAlvo(1, &pAlvo, &user2, 1);
    Anuncio anuncioOferta(2, &pOferta, &user1, 1);
    Anuncio anuncioInvalido(3, &pInvalida, &user1, 1);

    SUBCASE("Troca valida dentro da margem de 20% de tolerancia") {
        Troca t("T001", &user1, &user2, &anuncioAlvo, &anuncioOferta);
        
        CHECK(t.verificar_precos_similares() == true);
        CHECK(t.validar_transacao() == true);
        
        t.executar_transacao();
        CHECK(t.get_status() == StatusTransacao::CONCLUIDA);
    }

    SUBCASE("Troca invalida por discrepancia de precos") {
        Troca t("T002", &user1, &user2, &anuncioAlvo, &anuncioInvalido);
        
        CHECK(t.verificar_precos_similares() == false);
        CHECK(t.validar_transacao() == false);
        
        t.executar_transacao();
        CHECK(t.get_status() == StatusTransacao::REJEITADA);
    }
}