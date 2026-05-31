#include "../include/TerminalUI.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream> 

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
    
    // CARREGA O CARRINHO SALVO DO USUÁRIO ASSIM QUE ELE FAZ LOGIN!
    carrinhoCompras.carregarCarrinho(usuario->getLogin());
    
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
                // SALVA O CARRINHO ANTES DE SAIR, PARA GARANTIR SEGURANÇA
                carrinhoCompras.salvarCarrinho(usuario->getLogin());
                carrinhoCompras.esvaziar();
                cout << "\nFazendo logout e salvando dados..." << endl;
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
    int opcao;
    do {
        limparTela();
        cout << "\n=== Area de Compras ===" << endl;
        cout << "Bem-vindo(a), " << usuario->getNome() << "!" << endl;
        cout << "1 - Ver Vitrine Global / Pesquisar" << endl;
        cout << "2 - Ver Meu Carrinho" << endl;
        cout << "3 - Salvar Carrinho" << endl; // <-- OPÇÃO ATUALIZADA
        cout << "4 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            limparTela();
            int opcaoFiltro;
            cout << "=== Menu de Busca de Produtos ===" << endl;
            cout << "1 - Ver TODOS os produtos disponiveis" << endl;
            cout << "2 - Pesquisar por Categoria e Subcategoria" << endl;
            cout << "3 - Pesquisar por Nome" << endl;
            cout << "Escolha uma opcao: ";
            cin >> opcaoFiltro;

            string categoriaFiltro = "";
            string subcategoriaFiltro = "";
            string nomeFiltro = ""; 

            if (opcaoFiltro == 2) {
                int opcaoCat, opcaoSub;
                limparTela();
                cout << "\n--- Selecione a Categoria desejada ---" << endl;
                cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\nOpcao: ";
                cin >> opcaoCat;

                if (opcaoCat == 1) categoriaFiltro = "Veiculo";
                else if (opcaoCat == 2) categoriaFiltro = "Eletrodomestico";
                else if (opcaoCat == 3) categoriaFiltro = "Roupa";

                int querSub;
                cout << "\nDeseja refinar a busca selecionando uma Subcategoria?\n1 - Sim\n2 - Nao\nOpcao: ";
                cin >> querSub;

                if (querSub == 1) {
                    cout << "\n--- Selecione a Subcategoria ---" << endl;
                    if (categoriaFiltro == "Veiculo") {
                        cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
                        cin >> opcaoSub;
                        if (opcaoSub == 1) subcategoriaFiltro = "Carro";
                        else if (opcaoSub == 2) subcategoriaFiltro = "Moto";
                        else subcategoriaFiltro = "Caminhao";
                    } 
                    else if (categoriaFiltro == "Eletrodomestico") {
                        cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
                        cin >> opcaoSub;
                        if (opcaoSub == 1) subcategoriaFiltro = "Cozinha";
                        else if (opcaoSub == 2) subcategoriaFiltro = "Quarto";
                        else if (opcaoSub == 3) subcategoriaFiltro = "Escritorio";
                        else subcategoriaFiltro = "Gamer";
                    }
                    else if (categoriaFiltro == "Roupa") {
                        cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
                        cin >> opcaoSub;
                        if (opcaoSub == 1) subcategoriaFiltro = "Camisa";
                        else if (opcaoSub == 2) subcategoriaFiltro = "Calca";
                        else subcategoriaFiltro = "Tenis";
                    }
                }
            } 
            else if (opcaoFiltro == 3) {
                limparTela();
                cout << "\n--- Pesquisa por Nome ---" << endl;
                cout << "Digite o nome ou palavra-chave: ";
                cin.ignore();
                getline(cin, nomeFiltro);
            }

            limparTela();
            cout << "\n--- Resultados da Busca ---" << endl;
            if (!categoriaFiltro.empty()) {
                cout << "Filtro ativo: " << categoriaFiltro;
                if (!subcategoriaFiltro.empty()) cout << " > " << subcategoriaFiltro;
                cout << "\n" << endl;
            } else if (!nomeFiltro.empty()) {
                cout << "Buscando por: \"" << nomeFiltro << "\"\n" << endl;
            }

            const auto& produtos = gerenciadorProdutos.get_produtos();
            vector<const Produto*> produtosExibidos; 

            for (const auto& p : produtos) {
                if (p.get_login_anunciante() != usuario->getLogin()) {
                    
                    if (!categoriaFiltro.empty() && p.get_categoria() != categoriaFiltro) continue;
                    if (!subcategoriaFiltro.empty() && p.get_subcategoria() != subcategoriaFiltro) continue;

                    if (!nomeFiltro.empty()) {
                        string nomeDoBanco = p.get_nome();
                        string nomeDoBancoLow = nomeDoBanco;
                        string nomeBuscaLow = nomeFiltro;

                        transform(nomeDoBancoLow.begin(), nomeDoBancoLow.end(), nomeDoBancoLow.begin(), ::tolower);
                        transform(nomeBuscaLow.begin(), nomeBuscaLow.end(), nomeBuscaLow.begin(), ::tolower);

                        if (nomeDoBancoLow.find(nomeBuscaLow) == string::npos) {
                            continue; 
                        }
                    }

                    produtosExibidos.push_back(&p);
                    cout << "(" << produtosExibidos.size() << ") ID: " << p.get_id() 
                         << " | Nome: " << p.get_nome() 
                         << "\n    Categoria: " << p.get_categoria() << " > " << p.get_subcategoria()
                         << "\n    Preco: R$ " << p.get_preco() 
                         << "\n    Vendedor: " << p.get_login_anunciante() << "\n" << endl;
                }
            }
            
            if (produtosExibidos.empty()) {
                cout << "Nenhum produto correspondente foi encontrado." << endl;
                cout << "\nPressione Enter para continuar...";
                cin.ignore();
                cin.get();
            } else {
                cout << "---------------------------------" << endl;
                int querComprar;
                cout << "Deseja adicionar algum produto ao carrinho?\n1 - Sim\n2 - Nao\nOpcao: ";
                cin >> querComprar;

                if (querComprar == 1) {
                    int escolhaProduto;
                    cout << "\nDigite o NUMERO do produto na lista: ";
                    cin >> escolhaProduto;

                    if (escolhaProduto > 0 && escolhaProduto <= static_cast<int>(produtosExibidos.size())) {
                        
                        const Produto* p = produtosExibidos[escolhaProduto - 1];
                        int qtd;
                        
                        cout << "Produto selecionado: " << p->get_nome() << " | R$ " << p->get_preco() << endl;
                        cout << "Digite a quantidade que deseja comprar: ";
                        cin >> qtd;

                        if (qtd > 0) {
                            ItemVendido novoItem(p->get_id(), p->get_nome(), p->get_preco(), qtd);
                            carrinhoCompras.adicionarItem(novoItem);
                            
                            // Salva automaticamente após adicionar ao carrinho
                            carrinhoCompras.salvarCarrinho(usuario->getLogin());
                            
                            cout << "\nSucesso! " << qtd << "x " << p->get_nome() << " adicionado(s) ao carrinho e salvo." << endl;
                        } else {
                            cout << "\nQuantidade invalida." << endl;
                        }
                    } else {
                        cout << "\nErro: Numero invalido." << endl;
                    }
                }
                
                cout << "\nPressione Enter para continuar...";
                cin.ignore();
                cin.get();
            }

        } else if (opcao == 2) {
            limparTela();
            cout << "\n--- Meu Carrinho ---" << endl;
            
            const auto& itens = carrinhoCompras.get_itens();
            
            if (itens.empty()) {
                cout << "Seu carrinho esta completamente vazio no momento." << endl;
            } else {
                for (const auto& item : itens) {
                    cout << "ID: " << item.get_idProduto() 
                         << " | " << item.get_nomeProduto() 
                         << " | Qtd: " << item.get_quantidade() 
                         << " | Subtotal: R$ " << item.get_subtotal() << endl;
                }
                cout << "---------------------------------" << endl;
                cout << "TOTAL DO PEDIDO: R$ " << carrinhoCompras.get_total() << endl;
            }
            
            cout << "\nPressione Enter para voltar...";
            cin.ignore();
            cin.get();

        } else if (opcao == 3) { // <-- OPÇÃO PARA SALVAR O CARRINHO
            limparTela();
            cout << "\n--- Salvar Carrinho ---" << endl;
            
            if (carrinhoCompras.get_itens().empty()) {
                cout << "O carrinho esta vazio, mas foi atualizado no sistema." << endl;
            } else {
                carrinhoCompras.salvarCarrinho(usuario->getLogin());
                cout << "O seu carrinho foi salvo com sucesso em nossos servidores!" << endl;
                cout << "Os itens ficarao aguardando o futuro modulo de pagamento." << endl;
            }

            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (opcao != 4); 
}