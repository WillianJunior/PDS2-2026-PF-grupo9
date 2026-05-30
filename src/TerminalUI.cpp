#include "../include/TerminalUI.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void TerminalUI::limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void TerminalUI::iniciar() {
    int opcao;
    do {
        limparTela();
        cout << "\n=== E-commerce Terminal ===" << endl;
        cout << "1 - Fazer Login" << endl;
        cout << "2 - Cadastrar Novo Usuario" << endl;
        cout << "3 - Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                menuLogin();
                break;
            case 2:
                menuCadastro();
                break;
            case 3:
                limparTela();
                cout << "\nSaindo do sistema. Ate logo!" << endl;
                break;
            default:
                cout << "\nOpcao invalida! Tente novamente." << endl;
                break;
        }
    } while (opcao != 3);
}

void TerminalUI::menuCadastro() {
    limparTela();
    string nome, login, senha;
    
    cout << "\n--- Cadastro de Novo Usuario ---" << endl;
    cout << "Digite seu Nome: ";
    cin.ignore();
    getline(cin, nome);
    cout << "Digite seu E-mail (login): ";
    cin >> login;
    cout << "Digite sua Senha: ";
    cin >> senha;

    gerenciador.registrarUsuario(nome, login, senha);
    cout << "\nUsuario cadastrado com sucesso!" << endl;
    
    // Pequena pausa para que você consiga ler a mensagem antes de limpar a tela novamente
    cout << "\nPressione Enter para voltar ao menu principal...";
    cin.ignore();
    cin.get();
}

void TerminalUI::menuLogin() {
    limparTela();
    string login, senha;
    
    cout << "\n--- Acesso ao Sistema ---" << endl;
    cout << "Digite seu e-mail (login): ";
    cin >> login;
    cout << "Digite sua senha: ";
    cin >> senha;

    Usuario* usuarioAutenticado = gerenciador.autenticar(login, senha);

    if (usuarioAutenticado != nullptr) {
        cout << "\nLogin bem-sucedido!" << endl;
        menuEscolhaPerfil(usuarioAutenticado);
    } else {
        cout << "\nFalha na autenticacao. Verifique seu e-mail e senha." << endl;
        cout << "\nPressione Enter para continuar...";
        cin.ignore();
        cin.get();
    }
}

void TerminalUI::menuEscolhaPerfil(Usuario* usuario) {
    int opcao;
    
    do {
        limparTela();
        cout << "\nBem-vindo(a), " << usuario->getNome() << "!" << endl;
        cout << "Selecione o seu perfil de acesso:" << endl;
        cout << "1 - Comprador" << endl;
        cout << "2 - Anunciante" << endl;
        cout << "3 - Sair da Conta (Logout)" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                menuComprador(usuario); // Pula para a tela de compras
                break;
            case 2:
                menuAnunciante(usuario); // Pula para a tela de vendas
                break;
            case 3:
                cout << "\nFazendo logout..." << endl;
                break;
            default:
                cout << "\nOpcao invalida! Tente novamente." << endl;
                break;
        }
    } while (opcao != 3); 
}

// ==========================================
// MÓDULO DO ANUNCIANTE
// ==========================================
void TerminalUI::menuAnunciante(Usuario* usuario) {
    int opcao;
    do {
        limparTela();
        cout << "\n=== Painel do Anunciante ===" << endl;
        cout << "1 - Cadastrar Novo Produto" << endl;
        cout << "2 - Ver Minha Vitrine (Produtos Cadastrados)" << endl;
        cout << "3 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            limparTela();
            int id, opcaoCat, opcaoSub;
            string nome, categoriaEscolhida, subcategoriaEscolhida;
            double preco;

            cout << "\n--- Cadastro de Produto ---" << endl;
            cout << "Digite o ID do produto: ";
            cin >> id;

            cout << "Digite o nome do produto: ";
            cin.ignore();
            getline(cin, nome);

            cout << "Digite o preco do produto: R$ ";
            cin >> preco;

            // 1. MENU TRAVADO DE CATEGORIA
            cout << "\nEscolha a Categoria:" << endl;
            cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa" << endl;
            cout << "Opcao: ";
            cin >> opcaoCat;

            if (opcaoCat == 1) categoriaEscolhida = "Veiculo";
            else if (opcaoCat == 2) categoriaEscolhida = "Eletrodomestico";
            else categoriaEscolhida = "Roupa";

            // 2. MENU TRAVADO DE SUBCATEGORIA
            cout << "\nEscolha a Subcategoria:" << endl;
            if (categoriaEscolhida == "Veiculo") {
                cout << "1 - Carro\n2 - Moto\n3 - Caminhao" << endl;
                cout << "Opcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Carro";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Moto";
                else subcategoriaEscolhida = "Caminhao";
            } 
            else if (categoriaEscolhida == "Eletrodomestico") {
                cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer" << endl;
                cout << "Opcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Cozinha";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Quarto";
                else if (opcaoSub == 3) subcategoriaEscolhida = "Escritorio";
                else subcategoriaEscolhida = "Gamer";
            }
            else if (categoriaEscolhida == "Roupa") {
                cout << "1 - Camisa\n2 - Calca\n3 - Tenis" << endl;
                cout << "Opcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Camisa";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Calca";
                else subcategoriaEscolhida = "Tenis";
            }

            // Agora enviamos as strings prontas para o gerenciador
            gerenciadorProdutos.cadastrarProduto(id, nome, preco, categoriaEscolhida, subcategoriaEscolhida);

            cout << "\nProduto cadastrado com sucesso!" << endl;
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
            
        } else if (opcao == 2) {
            limparTela();
            cout << "\n--- Vitrine de Produtos ---" << endl;
            
            // Puxa a lista de produtos carregada do arquivo
            const auto& produtos = gerenciadorProdutos.get_produtos();

            if (produtos.empty()) {
                cout << "Nenhum produto cadastrado ainda." << endl;
            } else {
                for (const auto& p : produtos) {
                    cout << "ID: " << p.get_id() 
                         << " | Nome: " << p.get_nome() 
                         << "\n  Categoria: " << p.get_categoria() << " > " << p.get_subcategoria()
                         << "\n  Preco: R$ " << p.get_preco() << "\n" << endl;
                }
            }
            
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    } while (opcao != 3);
}

// ==========================================
// MÓDULO DO COMPRADOR
// ==========================================
void TerminalUI::menuComprador(Usuario* usuario) {
    limparTela();
    cout << "\n=== Area de Compras ===" << endl;
    cout << "Bem-vindo a loja, " << usuario->getNome() << "!" << endl;
    cout << "(A funcionalidade de Carrinho de Compras estara disponivel em breve!)" << endl;
    
    cout << "\nPressione Enter para voltar...";
    cin.ignore();
    cin.get();
}