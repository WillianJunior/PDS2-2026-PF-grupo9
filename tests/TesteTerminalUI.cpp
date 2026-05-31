#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/TerminalUI.hpp"
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("Teste TerminalUI - Simulacao Multi-Usuario (Vendedor e Comprador)") {
    TerminalUI terminal;

    std::stringstream ecra_falso;
    std::stringstream teclado_falso;

    std::streambuf* cout_original = std::cout.rdbuf();
    std::streambuf* cin_original = std::cin.rdbuf();

    std::cout.rdbuf(ecra_falso.rdbuf());
    std::cin.rdbuf(teclado_falso.rdbuf());

    // =========================================================================
    // O ROBO DIGITANDO: Navegando por todas as ramificacoes de IFs e Menus
    // =========================================================================
    teclado_falso 
        // --- 1. CRIAR VENDEDOR E CADASTRAR PRODUTO ---
        << "2\n"                    // Menu: Cadastrar
        << "Vendedor Silva\n"       // Nome
        << "v@m.com\n"              // Login
        << "123\n"                  // Senha
        << "\n"                     // Enter para continuar
        
        << "1\n"                    // Menu: Fazer Login
        << "v@m.com\n"              // Login
        << "123\n"                  // Senha
        
        << "2\n"                    // Perfil: Entrar como Anunciante
        << "2\n" << "\n"            // Ver Vitrine (está vazia) -> Enter
        << "3\n" << "\n"            // Editar Produto (está vazio) -> Enter
        
        << "1\n"                    // Cadastrar Produto
        << "Camiseta\n"             // Nome
        << "50.0\n"                 // Preço
        << "3\n"                    // Categoria: Roupa
        << "1\n"                    // Subcategoria: Camisa
        << "\n"                     // Enter para continuar
        
        << "3\n"                    // Editar Produto
        << "1\n"                    // Escolher o produto 1
        << "2\n"                    // Editar Preço
        << "45.0\n"                 // Novo preço
        << "4\n"                    // Salvar alterações e Voltar
        << "\n"                     // Enter para continuar
        
        << "4\n"                    // Voltar para Menu Perfil
        << "3\n"                    // Sair da Conta (Logout)

        // --- 2. CRIAR COMPRADOR E EXPLORAR A VITRINE ---
        << "2\n"                    // Menu: Cadastrar
        << "Comprador Souza\n"      // Nome
        << "c@m.com\n"              // Login
        << "123\n"                  // Senha
        << "\n"                     // Enter para continuar
        
        << "1\n"                    // Menu: Fazer Login
        << "c@m.com\n"              // Login
        << "123\n"                  // Senha
        
        << "9\n"                    // Testar opção de perfil inválida
        << "1\n"                    // Perfil: Entrar como Comprador
        
        << "2\n" << "\n"            // Ver Carrinho (Vazio) -> Enter
        << "3\n" << "\n"            // Salvar Carrinho (Vazio) -> Enter
        
        // Testar erro de quantidade (0)
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "1\n"                    // Ver TODOS
        << "1\n"                    // Deseja comprar? 1 - Sim
        << "1\n"                    // Escolher produto 1 (Camiseta)
        << "0\n"                    // Quantidade: 0 (Inválida!)
        << "\n"                     // Enter para continuar
        
        // Testar pesquisa por NOME e comprar com sucesso
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "3\n"                    // Pesquisar por Nome
        << "camise\n"               // Texto da busca (Testa o ::tolower)
        << "1\n"                    // Deseja comprar? 1 - Sim
        << "1\n"                    // Escolher produto 1
        << "2\n"                    // Quantidade: 2
        << "\n"                     // Enter para continuar
        
        // Testar pesquisa por CATEGORIA sem subcategoria
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "2\n"                    // Pesquisar por Categoria
        << "2\n"                    // Categoria: Eletrodoméstico
        << "2\n"                    // Refinar com subcategoria? 2 - Não
        << "2\n"                    // Deseja comprar? 2 - Não
        << "\n"                     // Enter para continuar
        
        << "2\n" << "\n"            // Ver Carrinho (Agora cheio!) -> Enter
        << "3\n" << "\n"            // Salvar Carrinho (Cheio) -> Enter
        
        << "4\n"                    // Voltar para Menu Perfil
        << "3\n"                    // Sair da Conta (Logout)
        
        // --- 3. SAIR DO SISTEMA ---
        << "3\n";                   // Menu: Sair
        
    // Dispara a simulação!
    terminal.iniciar(); 

    // Devolve o controle ao terminal real
    std::cout.rdbuf(cout_original);
    std::cin.rdbuf(cin_original);

    // Avaliações de sanidade do teste
    std::string saida = ecra_falso.str();
    CHECK(saida.find("Produto atualizado com sucesso!") != std::string::npos);
    CHECK(saida.find("Quantidade invalida.") != std::string::npos);
    CHECK(saida.find("adicionado(s) ao carrinho e salvo.") != std::string::npos);
    CHECK(saida.find("Saindo do sistema. Ate logo!") != std::string::npos);
}