#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/GerenciadorProdutos.hpp"
#include <cstdio>
#include <string>

struct GuardaProdutos {
    static constexpr const char* ORIG = "data/produtos.txt";
    static constexpr const char* BAK  = "_testbak_produtos.txt";
    bool tinha;

    GuardaProdutos()  { tinha = (std::rename(ORIG, BAK) == 0); }
    ~GuardaProdutos() {
        std::remove(ORIG);
        if (tinha) std::rename(BAK, ORIG);
    }
};

// ID gerado: setw(5) userID + setw(5) posicao1 → "0000500001" p/ user=5
TEST_CASE("GerenciadorProdutos - Cadastrar e Buscar") {
    GuardaProdutos g;
    GerenciadorProdutos gp;
    gp.cadastrarProduto("Mouse Gamer", 250.0, "Eletrodomestico", "Escritorio", "v@gp.com", 5, 3);

    SUBCASE("buscarProdutoPorId encontra produto") {
        auto* p = gp.buscarProdutoPorId("0000500001");
        REQUIRE(p != nullptr);
        CHECK(p->get_nome() == "Mouse Gamer");
        CHECK(p->get_preco() == doctest::Approx(250.0));
        CHECK(p->get_categoria() == "Eletrodomestico");
        CHECK(p->get_subcategoria() == "Escritorio");
        CHECK(p->is_ativo() == true);
        CHECK(p->get_quantidade_estoque() == 3);
    }

    SUBCASE("buscarProdutoPorId ID inexistente retorna nullptr") {
        CHECK(gp.buscarProdutoPorId("NAOEXISTE") == nullptr);
    }

    SUBCASE("get_produtos reflete o cadastro") {
        CHECK(gp.get_produtos().size() == 1);
    }

    SUBCASE("reload do arquivo restaura o produto") {
        GerenciadorProdutos gp2; // lê do arquivo salvo pelo cadastro acima
        auto* p = gp2.buscarProdutoPorId("0000500001");
        REQUIRE(p != nullptr);
        CHECK(p->get_nome() == "Mouse Gamer");
    }
}

TEST_CASE("GerenciadorProdutos - Editar") {
    GuardaProdutos g;
    GerenciadorProdutos gp;
    gp.cadastrarProduto("Produto Original", 100.0, "Roupa", "Camisa", "e@gp.com", 6, 2);

    SUBCASE("editar produto existente altera dados") {
        CHECK(gp.editarProduto("0000600001", "Produto Editado", 150.0, "Veiculo", "Moto") == true);
        auto* p = gp.buscarProdutoPorId("0000600001");
        REQUIRE(p != nullptr);
        CHECK(p->get_nome() == "Produto Editado");
        CHECK(p->get_preco() == doctest::Approx(150.0));
        CHECK(p->get_categoria() == "Veiculo");
        CHECK(p->get_subcategoria() == "Moto");
    }

    SUBCASE("editar ID inexistente retorna false") {
        CHECK(gp.editarProduto("FAKE_ID", "X", 1.0, "Y", "Z") == false);
    }
}

TEST_CASE("GerenciadorProdutos - Inativar") {
    GuardaProdutos g;
    GerenciadorProdutos gp;
    gp.cadastrarProduto("Produto Inativar", 50.0, "Outros", "Geral", "i@gp.com", 7, 1);

    SUBCASE("inativar produto existente") {
        CHECK(gp.inativarProduto("0000700001") == true);
        auto* p = gp.buscarProdutoPorId("0000700001");
        REQUIRE(p != nullptr);
        CHECK(p->is_ativo() == false);
    }

    SUBCASE("inativar ID inexistente retorna false") {
        CHECK(gp.inativarProduto("FAKE") == false);
    }
}

TEST_CASE("GerenciadorProdutos - Vender") {
    GuardaProdutos g;
    GerenciadorProdutos gp;
    gp.cadastrarProduto("Produto Vender", 25.0, "Outros", "Geral", "vd@gp.com", 8, 5);

    SUBCASE("vender parcialmente reduz estoque") {
        CHECK(gp.venderProduto("0000800001", 3) == true);
        auto* p = gp.buscarProdutoPorId("0000800001");
        CHECK(p->get_quantidade_estoque() == 2);
        CHECK(p->is_ativo() == true);
    }

    SUBCASE("vender tudo desativa o produto") {
        CHECK(gp.venderProduto("0000800001", 5) == true);
        auto* p = gp.buscarProdutoPorId("0000800001");
        CHECK(p->get_quantidade_estoque() == 0);
        CHECK(p->is_ativo() == false);
    }

    SUBCASE("vender mais que o estoque retorna false sem alterar") {
        CHECK(gp.venderProduto("0000800001", 99) == false);
        CHECK(gp.buscarProdutoPorId("0000800001")->get_quantidade_estoque() == 5);
    }

    SUBCASE("vender ID inexistente retorna false") {
        CHECK(gp.venderProduto("FAKE", 1) == false);
    }
}

TEST_CASE("GerenciadorProdutos - BuscarPorUsuario") {
    GuardaProdutos g;
    GerenciadorProdutos gp;
    gp.cadastrarProduto("P1", 10.0, "Outros", "Geral", "ua@gp.com", 9,  1);
    gp.cadastrarProduto("P2", 20.0, "Outros", "Geral", "ua@gp.com", 9,  1);
    gp.cadastrarProduto("P3", 30.0, "Outros", "Geral", "ub@gp.com", 10, 1);

    CHECK(gp.buscarProdutosPorUsuario("ua@gp.com").size()     == 2);
    CHECK(gp.buscarProdutosPorUsuario("ub@gp.com").size()     == 1);
    CHECK(gp.buscarProdutosPorUsuario("ninguem@gp.com").size() == 0);
}
