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
        cout << "3 - Voltar ao Menu Anterior" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                limparTela();
                cout << "\nVoce entrou como Comprador." << endl;
                // Pausa antes de retornar ao menu de perfil
                cout << "\nPressione Enter para voltar...";
                cin.ignore();
                cin.get();
                break;
            case 2:
                limparTela();
                cout << "\nVoce entrou como Anunciante." << endl;
                // Pausa antes de retornar ao menu de perfil
                cout << "\nPressione Enter para voltar...";
                cin.ignore();
                cin.get();
                break;
            case 3:
                cout << "\nVoltando ao menu anterior." << endl;
                break;
            default:
                cout << "\nOpcao invalida! Tente novamente." << endl;
                break;
        }
    } while (opcao != 3); 
}