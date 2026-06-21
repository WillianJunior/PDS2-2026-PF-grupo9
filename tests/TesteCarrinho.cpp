#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/Carrinho.hpp"
#include "../include/ItemVendido.hpp"
#include <cstdio> // Necessário para a função std::remove (apagar ficheiros)
#include <string>

TEST_CASE("Teste Carrinho - Operacoes Basicas e Esvaziar") {
    Carrinho c;
    
    // Construtor de ItemVendido (Ajuste os parametros se a sua classe for diferente)
    ItemVendido i1("ID01", "Mouse", 50.0, 2);  // Subtotal = 100.0
    ItemVendido i2("ID02", "Teclado", 150.0, 1); // Subtotal = 150.0

    c.adicionarItem(i1);
    c.adicionarItem(i2);

    // Verifica se os itens foram adicionados e o total calculado
    CHECK(c.get_itens().size() == 2);
    CHECK(c.get_total() == doctest::Approx(250.0));

    // Testa o esvaziamento do carrinho
    c.esvaziar();
    CHECK(c.get_itens().size() == 0);
    CHECK(c.get_total() == doctest::Approx(0.0));
}

TEST_CASE("Teste Carrinho - Salvar e Carregar em Arquivo txt") {
    Carrinho carrinhoOriginal;
    ItemVendido i1("ID99", "Monitor UltraWide", 1500.0, 1);
    
    carrinhoOriginal.adicionarItem(i1);

    std::string loginTeste = "comprador_teste";
    
    // 1. Salva o carrinho num ficheiro "carrinho_comprador_teste.txt"
    carrinhoOriginal.salvarCarrinho(loginTeste);

    // 2. Cria um NOVO carrinho vazio e carrega os dados do ficheiro
    Carrinho carrinhoCarregado;
    carrinhoCarregado.carregarCarrinho(loginTeste);

    // 3. Verifica se os dados voltaram corretos
    REQUIRE(carrinhoCarregado.get_itens().size() == 1);
    
    // ATENÇÃO: Usando os mesmos nomes de funções que você mostrou no Carrinho.cpp!
    CHECK(carrinhoCarregado.get_itens()[0].get_idProduto() == "ID99");
    CHECK(carrinhoCarregado.get_itens()[0].get_nomeProduto() == "Monitor UltraWide");
    CHECK(carrinhoCarregado.get_itens()[0].get_precoNaVenda() == doctest::Approx(1500.0));
    CHECK(carrinhoCarregado.get_itens()[0].get_quantidade() == 1);

    // 4. Limpeza: Apaga o ficheiro de teste gerado para não deixar "lixo" no computador
    std::string nomeArquivo = "carrinho_" + loginTeste + ".txt";
    std::remove(nomeArquivo.c_str());
}

TEST_CASE("Teste Carrinho - Remover Item") {
    Carrinho c;
    ItemVendido i1("ID01", "Mouse", 50.0, 2);
    ItemVendido i2("ID02", "Teclado", 150.0, 1);

    c.adicionarItem(i1);
    c.adicionarItem(i2);

    // Remove o primeiro item (indice 0) e verifica que sobra apenas o Teclado
    c.removerItem(0);
    REQUIRE(c.get_itens().size() == 1);
    CHECK(c.get_itens()[0].get_idProduto() == "ID02");

    // Indice fora do intervalo nao deve crashar nem alterar o carrinho
    c.removerItem(99);
    CHECK(c.get_itens().size() == 1);
}

TEST_CASE("Teste Carrinho - Carregar Arquivo Inexistente") {
    Carrinho c;
    // Tenta carregar um carrinho de um utilizador que não tem ficheiro salvo
    c.carregarCarrinho("usuario_fantasma_que_nao_existe");
    
    // O sistema não deve falhar (crash), deve apenas manter o carrinho vazio
    CHECK(c.get_itens().size() == 0);
}