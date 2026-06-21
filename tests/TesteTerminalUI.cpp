#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"
#include "../include/TerminalUI.hpp"
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("Teste TerminalUI - Simulacao Multi-Usuario (Vendedor e Comprador via Carrinho)") {
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
        << "Vendedor RefatoracaoUX\n" // Nome
        << "vendedor.refatoracaoux@teste.com\n" // Login
        << "123\n"                  // Senha
        << "pix-vendedor-refatoracaoux\n" // Chave PIX
        << "\n"                     // Enter para continuar

        << "1\n"                    // Menu: Fazer Login
        << "vendedor.refatoracaoux@teste.com\n" // Login
        << "123\n"                  // Senha

        << "2\n"                    // Perfil: Entrar como Anunciante
        << "2\n" << "\n"            // Ver Minha Vitrine -> Enter (sempre 1 pausa, com ou sem itens)

        << "1\n"                    // Cadastrar Produto
        << "CamisetaRefatoracaoUX\n" // Nome
        << "50.0\n"                 // Preço
        << "3\n"                    // Categoria: Roupa
        << "1\n"                    // Subcategoria: Camisa
        << "5\n"                    // Quantidade em estoque
        << "\n"                     // Enter para continuar

        << "3\n"                    // Editar Produto
        << "1\n"                    // Escolher o produto 1
        << "2\n"                    // Editar Preço
        << "45.0\n"                 // Novo preço
        << "4\n"                    // Salvar alterações e Voltar
        << "\n"                     // Enter para continuar

        << "5\n"                    // Voltar para Menu Perfil (opcao 5 no menu do Anunciante)
        << "3\n"                    // Sair da Conta (Logout)

        // --- 2. CRIAR COMPRADOR E EXPLORAR A VITRINE / CARRINHO ---
        << "2\n"                    // Menu: Cadastrar
        << "Comprador RefatoracaoUX\n" // Nome
        << "comprador.refatoracaoux@teste.com\n" // Login
        << "123\n"                  // Senha
        << "\n"                     // Chave PIX (em branco)
        << "\n"                     // Enter para continuar

        << "1\n"                    // Menu: Fazer Login
        << "comprador.refatoracaoux@teste.com\n" // Login
        << "123\n"                  // Senha

        << "9\n"                    // Testar opção de perfil inválida
        << "1\n"                    // Perfil: Entrar como Comprador

        // Testar blindagem anti-crash: letra invalida antes de escolher opcao valida
        << "abc\n"                  // Entrada invalida (letras) no menu do Comprador
        << "2\n"                    // Meu Carrinho (vazio)

        // Dentro do carrinho vazio: esvaziar (no-op) e voltar
        << "3\n" << "\n"            // Esvaziar Carrinho (vazio) -> Enter
        << "4\n"                    // Voltar para Area de Compras

        // Testar erro de quantidade (0) ao adicionar ao carrinho
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "1\n"                    // Ver TODOS
        << "\n"                     // Enter vazio invalido ao escolher produto
        << "1\n"                    // Escolher produto 1
        << "0\n"                    // Quantidade: 0 (Inválida!)
        << "\n"                     // Enter para continuar

        // Testar pesquisa por NOME e adicionar com sucesso ao carrinho
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "3\n"                    // Pesquisar por Nome
        << "refatoracaoux\n"        // Texto da busca (Testa o ::tolower)
        << "1\n"                    // Escolher produto 1
        // Quantidade: 1 (nao usar >1 aqui - execucoes repetidas deste teste no mesmo
        // ambiente acumulam duplicatas com estoques baixos; 1 funciona com qualquer uma)
        << "1\n"
        << "\n"                     // Enter para continuar

        // Testar pesquisa por CATEGORIA com subcategoria e cancelar (0) sem adicionar
        << "1\n"                    // Ver Vitrine / Pesquisar
        << "2\n"                    // Pesquisar por Categoria
        << "3\n"                    // Categoria: Roupa
        << "1\n"                    // Refinar com subcategoria? 1 - Sim
        << "1\n"                    // Subcategoria: Camisa
        << "0\n"                    // Cancelar (não adicionar)
        << "\n"                     // Enter para continuar

        // --- 3. CHECKOUT NO CARRINHO ---
        << "2\n"                    // Meu Carrinho (agora com 1 item)

        // Propor E-scambo usando o item do carrinho como alvo (comprador ainda nao tem produtos)
        << "2\n"                    // Carrinho: Propor E-scambo
        << "1\n"                    // Item alvo
        << "\n"                     // Enter para continuar

        // Finalizar Compra (PIX) com o item restante no carrinho
        << "1\n"                    // Carrinho: Finalizar Compra (PIX)
        << "\n"                     // Enter para continuar

        << "4\n"                    // Voltar para Area de Compras (carrinho ja vazio)
        << "3\n"                    // Voltar para Menu Perfil
        << "3\n"                    // Sair da Conta (Logout)

        // --- 4. SAIR DO SISTEMA ---
        << "3\n";                   // Menu: Sair

    // Dispara a simulação!
    terminal.iniciar();

    // Devolve o controle ao terminal real
    std::cout.rdbuf(cout_original);
    std::cin.rdbuf(cin_original);

    // Avaliações de sanidade do teste
    std::string saida = ecra_falso.str();
    CHECK(saida.find("Produto atualizado com sucesso!") != std::string::npos);
    CHECK(saida.find("[Erro] Entrada invalida. Digite um numero:") != std::string::npos);
    CHECK(saida.find("Quantidade invalida.") != std::string::npos);
    CHECK(saida.find("adicionado(s) ao carrinho e salvo.") != std::string::npos);
    CHECK(saida.find("Voce nao tem produtos ativos para oferecer em troca.") != std::string::npos);
    CHECK(saida.find("Chave PIX:") != std::string::npos);
    CHECK(saida.find("Compra finalizada com sucesso!") != std::string::npos);
    CHECK(saida.find("Seu carrinho foi esvaziado.") != std::string::npos);
    CHECK(saida.find("Saindo do sistema. Ate logo!") != std::string::npos);
}

TEST_CASE("Teste TerminalUI - Blindagem anti-crash contra letras, simbolos e Enter vazio") {
    TerminalUI terminal;

    std::stringstream ecra_falso;
    std::stringstream teclado_falso;

    std::streambuf* cout_original = std::cout.rdbuf();
    std::streambuf* cin_original = std::cin.rdbuf();

    std::cout.rdbuf(ecra_falso.rdbuf());
    std::cin.rdbuf(teclado_falso.rdbuf());

    // No menu principal, ataca com letra, simbolo e Enter vazio antes de sair corretamente.
    // Se a blindagem falhar, o programa entraria em loop infinito ou crasharia aqui.
    teclado_falso
        << "letra\n"   // letras
        << "@#$\n"     // simbolos
        << "\n"        // Enter vazio
        << "3\n";      // Sair (opcao valida)

    terminal.iniciar();

    std::cout.rdbuf(cout_original);
    std::cin.rdbuf(cin_original);

    std::string saida = ecra_falso.str();
    size_t ocorrencias = 0;
    size_t pos = 0;
    while ((pos = saida.find("[Erro] Entrada invalida. Digite um numero:", pos)) != std::string::npos) {
        ++ocorrencias;
        pos += 1;
    }

    CHECK(ocorrencias == 3);
    CHECK(saida.find("Saindo do sistema. Ate logo!") != std::string::npos);
}

TEST_CASE("Teste TerminalUI - Estoque e rejeitado quando insuficiente e produto some da vitrine ao esgotar") {
    TerminalUI terminal;

    std::stringstream ecra_falso;
    std::stringstream teclado_falso;

    std::streambuf* cout_original = std::cout.rdbuf();
    std::streambuf* cin_original = std::cin.rdbuf();

    std::cout.rdbuf(ecra_falso.rdbuf());
    std::cin.rdbuf(teclado_falso.rdbuf());

    teclado_falso
        // --- Vendedor cadastra um produto com estoque = 1 (item unico) ---
        << "2\n" << "Vendedor EstoqueUX\n" << "vendedor.estoqueux@teste.com\n" << "123\n" << "\n" << "\n"
        << "1\n" << "vendedor.estoqueux@teste.com\n" << "123\n"
        << "2\n"                    // Perfil: Anunciante
        << "1\n"                    // Cadastrar Produto
        << "ProdutoUnicoEstoqueUX\n" // Nome
        << "100.0\n"                // Preco
        << "4\n"                    // Categoria: Outros (pula subcategoria)
        << "1\n"                    // Quantidade em estoque
        << "\n"                     // Enter para continuar
        << "5\n"                    // Voltar para Menu Perfil
        << "3\n"                    // Sair da Conta (Logout)

        // --- Comprador tenta levar mais unidades do que ha em estoque ---
        << "2\n" << "Comprador EstoqueUX\n" << "comprador.estoqueux@teste.com\n" << "123\n" << "\n" << "\n"
        << "1\n" << "comprador.estoqueux@teste.com\n" << "123\n"
        << "1\n"                    // Perfil: Comprador

        << "1\n"                    // Vitrine / Pesquisar
        << "3\n"                    // Pesquisar por Nome
        << "estoqueux\n"            // Busca
        << "1\n"                    // Escolher produto 1
        << "2\n"                    // Pede 2 unidades (so ha 1 em estoque)
        << "\n"                     // Enter para continuar

        // --- Compra exatamente a 1 unidade disponivel ---
        << "1\n"                    // Vitrine / Pesquisar
        << "3\n"                    // Pesquisar por Nome
        << "estoqueux\n"            // Busca (produto ainda ativo, nada foi vendido ainda)
        << "1\n"                    // Escolher produto 1
        << "1\n"                    // Pede 1 unidade (sucesso)
        << "\n"                     // Enter para continuar

        << "2\n"                    // Meu Carrinho
        << "1\n"                    // Finalizar Compra (PIX) -> esgota o estoque
        << "\n"                     // Enter para continuar
        << "4\n"                    // Voltar para Area de Compras

        // --- Produto esgotado nao deve mais aparecer na vitrine global ---
        << "1\n"                    // Vitrine / Pesquisar
        << "3\n"                    // Pesquisar por Nome
        << "estoqueux\n"            // Busca pelo mesmo produto
        << "\n"                     // Enter para continuar (nenhum resultado)

        << "3\n"                    // Voltar para Menu Perfil
        << "3\n"                    // Sair da Conta (Logout)
        << "3\n";                   // Sair do sistema

    terminal.iniciar();

    std::cout.rdbuf(cout_original);
    std::cin.rdbuf(cin_original);

    std::string saida = ecra_falso.str();
    CHECK(saida.find("Estoque insuficiente! Disponivel: 1") != std::string::npos);
    CHECK(saida.find("ProdutoUnicoEstoqueUX adicionado(s) ao carrinho e salvo.") != std::string::npos);
    CHECK(saida.find("Compra finalizada com sucesso!") != std::string::npos);
    CHECK(saida.find("Nenhum produto correspondente foi encontrado.") != std::string::npos);
    CHECK(saida.find("Saindo do sistema. Ate logo!") != std::string::npos);
}
