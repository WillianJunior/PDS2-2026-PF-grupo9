#include "../include/TerminalUI.h"
#include <iostream>
#include <cstdlib>
#include <vector>

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
                menuComprador(usuario);
                break;
            case 2:
                menuAnunciante(usuario);
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
        cout << "2 - Ver Minha Vitrine" << endl;
        cout << "3 - Editar um Produto" << endl;
        cout << "4 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            limparTela();
            int opcaoCat, opcaoSub;
            string nome, categoriaEscolhida, subcategoriaEscolhida;
            double preco;

            cout << "\n--- Cadastro de Produto ---" << endl;
            cout << "Digite o nome do produto: ";
            cin.ignore();
            getline(cin, nome);

            cout << "Digite o preco do produto: R$ ";
            cin >> preco;

            cout << "\nEscolha a Categoria:" << endl;
            cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\nOpcao: ";
            cin >> opcaoCat;

            if (opcaoCat == 1) categoriaEscolhida = "Veiculo";
            else if (opcaoCat == 2) categoriaEscolhida = "Eletrodomestico";
            else categoriaEscolhida = "Roupa";

            cout << "\nEscolha a Subcategoria:" << endl;
            if (categoriaEscolhida == "Veiculo") {
                cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Carro";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Moto";
                else subcategoriaEscolhida = "Caminhao";
            } 
            else if (categoriaEscolhida == "Eletrodomestico") {
                cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Cozinha";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Quarto";
                else if (opcaoSub == 3) subcategoriaEscolhida = "Escritorio";
                else subcategoriaEscolhida = "Gamer";
            }
            else if (categoriaEscolhida == "Roupa") {
                cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
                cin >> opcaoSub;
                if (opcaoSub == 1) subcategoriaEscolhida = "Camisa";
                else if (opcaoSub == 2) subcategoriaEscolhida = "Calca";
                else subcategoriaEscolhida = "Tenis";
            }

            gerenciadorProdutos.cadastrarProduto(nome, preco, categoriaEscolhida, subcategoriaEscolhida, usuario->getLogin(), usuario->getId());
            
            cout << "\nProduto cadastrado com sucesso! O ID foi gerado automaticamente." << endl;
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
            
        } else if (opcao == 2) {
            limparTela();
            cout << "\n--- Minha Vitrine ---" << endl;
            const auto& produtos = gerenciadorProdutos.get_produtos();

            bool temProdutoMeu = false;
            for (const auto& p : produtos) {
                if (p.get_login_anunciante() == usuario->getLogin()) {
                    cout << "ID: " << p.get_id() 
                         << " | Nome: " << p.get_nome() 
                         << "\n  Categoria: " << p.get_categoria() << " > " << p.get_subcategoria()
                         << "\n  Preco: R$ " << p.get_preco() << "\n" << endl;
                    temProdutoMeu = true;
                }
            }
            
            if (!temProdutoMeu) {
                cout << "Voce ainda nao tem nenhum produto cadastrado." << endl;
            }
            
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();

        } else if (opcao == 3) {
            limparTela();
            cout << "\n--- Editar Produto ---" << endl;
            
            const auto& produtos = gerenciadorProdutos.get_produtos();
            vector<const Produto*> meusProdutos;

            for (const auto& p : produtos) {
                if (p.get_login_anunciante() == usuario->getLogin()) {
                    meusProdutos.push_back(&p);
                }
            }

            if (meusProdutos.empty()) {
                cout << "Voce ainda nao tem nenhum produto cadastrado para editar." << endl;
                cout << "\nPressione Enter para continuar...";
                cin.ignore();
                cin.get();
            } else {
                cout << "Seus produtos disponiveis:\n" << endl;
                
                for (size_t i = 0; i < meusProdutos.size(); ++i) {
                    cout << "(" << i + 1 << ") " << meusProdutos[i]->get_nome() 
                         << " | R$ " << meusProdutos[i]->get_preco() 
                         << " | ID: " << meusProdutos[i]->get_id() << endl;
                }

                int escolha;
                cout << "\nDigite o numero do produto que deseja editar (ou 0 para cancelar): ";
                cin >> escolha;

                if (escolha > 0 && escolha <= static_cast<int>(meusProdutos.size())) {
                    
                    const Produto* prodAtual = meusProdutos[escolha - 1];
                    string idSelecionado = prodAtual->get_id();
                    
                    // Copia os dados atuais para variáveis temporárias
                    string nomeTemp = prodAtual->get_nome();
                    double precoTemp = prodAtual->get_preco();
                    string catTemp = prodAtual->get_categoria();
                    string subTemp = prodAtual->get_subcategoria();

                    int opcaoEdit;
                    do {
                        limparTela();
                        cout << "\n--- Editando: " << nomeTemp << " ---" << endl;
                        cout << "1 - Editar Nome (Atual: " << nomeTemp << ")" << endl;
                        cout << "2 - Editar Preco (Atual: R$ " << precoTemp << ")" << endl;
                        cout << "3 - Editar Categoria/Subcategoria (Atual: " << catTemp << " > " << subTemp << ")" << endl;
                        cout << "4 - Salvar alteracoes e Voltar" << endl;
                        cout << "5 - Cancelar sem salvar" << endl;
                        cout << "Escolha o que deseja alterar: ";
                        cin >> opcaoEdit;

                        if (opcaoEdit == 1) {
                            cout << "\nDigite o NOVO nome: ";
                            cin.ignore();
                            getline(cin, nomeTemp);
                        } 
                        else if (opcaoEdit == 2) {
                            cout << "\nDigite o NOVO preco: R$ ";
                            cin >> precoTemp;
                        } 
                        else if (opcaoEdit == 3) {
                            int opcaoCat, opcaoSub;
                            cout << "\nEscolha a NOVA Categoria:" << endl;
                            cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\nOpcao: ";
                            cin >> opcaoCat;

                            if (opcaoCat == 1) catTemp = "Veiculo";
                            else if (opcaoCat == 2) catTemp = "Eletrodomestico";
                            else catTemp = "Roupa";

                            cout << "\nEscolha a NOVA Subcategoria:" << endl;
                            if (catTemp == "Veiculo") {
                                cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
                                cin >> opcaoSub;
                                if (opcaoSub == 1) subTemp = "Carro";
                                else if (opcaoSub == 2) subTemp = "Moto";
                                else subTemp = "Caminhao";
                            } 
                            else if (catTemp == "Eletrodomestico") {
                                cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
                                cin >> opcaoSub;
                                if (opcaoSub == 1) subTemp = "Cozinha";
                                else if (opcaoSub == 2) subTemp = "Quarto";
                                else if (opcaoSub == 3) subTemp = "Escritorio";
                                else subTemp = "Gamer";
                            }
                            else if (catTemp == "Roupa") {
                                cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
                                cin >> opcaoSub;
                                if (opcaoSub == 1) subTemp = "Camisa";
                                else if (opcaoSub == 2) subTemp = "Calca";
                                else subTemp = "Tenis";
                            }
                        } 
                        else if (opcaoEdit == 4) {
                            // Salva as alterações enviando o pacote completo atualizado
                            gerenciadorProdutos.editarProduto(idSelecionado, nomeTemp, precoTemp, catTemp, subTemp);
                            cout << "\nProduto atualizado com sucesso!" << endl;
                            cout << "\nPressione Enter para continuar...";
                            cin.ignore();
                            cin.get();
                        } 
                        else if (opcaoEdit == 5) {
                            cout << "\nEdicao cancelada. Nenhuma alteracao foi salva." << endl;
                            cout << "\nPressione Enter para continuar...";
                            cin.ignore();
                            cin.get();
                        }

                    } while (opcaoEdit != 4 && opcaoEdit != 5);
                    
                } else if (escolha != 0) {
                    cout << "\nOpcao invalida!" << endl;
                    cout << "\nPressione Enter para continuar...";
                    cin.ignore();
                    cin.get();
                } else {
                    cout << "\nEdicao cancelada." << endl;
                    cout << "\nPressione Enter para continuar...";
                    cin.ignore();
                    cin.get();
                }
            }
        }

    } while (opcao != 4); 
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