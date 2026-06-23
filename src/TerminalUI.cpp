#include "../include/TerminalUI.hpp"
#include "../include/Anuncio.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <limits>

using namespace std;

void TerminalUI::limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ==========================================
// BLINDAGEM ANTI-CRASH (leitura segura de inputs)
// ==========================================
// Em vez de usar "cin >> valor" puro (que entra em loop infinito se o stream
// cair em fail state com uma letra digitada), lemos sempre a linha inteira
// como string e validamos com stringstream. É mais código, mas é o jeito que
// garante que NENHUMA entrada do usuário (letra, simbolo, Enter vazio) derruba
// o programa.
int TerminalUI::lerOpcaoInteira() {
    string linha;
    int valor;

    while (true) {
        if (!getline(cin, linha)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        stringstream ss(linha);
        char sobra;

        // "!(ss >> sobra)" garante que não sobrou NADA depois do número (ex:
        // rejeita "5abc"), senão "5abc" seria aceito como 5 silenciosamente.
        if ((ss >> valor) && !(ss >> sobra)) {
            return valor;
        }

        cout << "[Erro] Entrada invalida. Digite um numero: ";
    }
}

double TerminalUI::lerValorDouble() {
    string linha;
    double valor;

    while (true) {
        if (!getline(cin, linha)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        stringstream ss(linha);
        char sobra;

        if ((ss >> valor) && !(ss >> sobra)) {
            return valor;
        }

        cout << "[Erro] Entrada invalida. Digite um valor numerico: ";
    }
}

string TerminalUI::lerLinha() {
    string linha;
    if (!getline(cin, linha)) {
        cin.clear();
        linha.clear();
    }
    return linha;
}

// REVISÃO: helper novo - substitui as ~15 repetições idênticas de
// `cout << "\nPressione Enter para continuar..."; lerLinha();` espalhadas
// pelo arquivo inteiro.
void TerminalUI::pausar() {
    cout << "\nPressione Enter para continuar...";
    lerLinha();
}

// REVISÃO: helper novo - centraliza a árvore de Subcategoria que antes estava
// copiada e colada em três pontos diferentes (Cadastro, Edicao e Filtro da
// Vitrine). Categorias sem subcategoria própria (ex: "Outros") caem no
// "return string()" final - quem chama decide o que fazer (Cadastro e Edicao
// usam "Geral" direto, sem nem chamar isto).
string TerminalUI::escolherSubcategoria(const string& categoria) {
    if (categoria == "Veiculo") {
        cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
        int opcaoSub = lerOpcaoInteira();
        if (opcaoSub == 1) return "Carro";
        if (opcaoSub == 2) return "Moto";
        return "Caminhao";
    }
    if (categoria == "Eletrodomestico") {
        cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
        int opcaoSub = lerOpcaoInteira();
        if (opcaoSub == 1) return "Cozinha";
        if (opcaoSub == 2) return "Quarto";
        if (opcaoSub == 3) return "Escritorio";
        return "Gamer";
    }
    if (categoria == "Roupa") {
        cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
        int opcaoSub = lerOpcaoInteira();
        if (opcaoSub == 1) return "Camisa";
        if (opcaoSub == 2) return "Calca";
        return "Tenis";
    }
    return string();
}

void TerminalUI::iniciar() {
    // REVISÃO: variáveis de controle de laço (aqui e nos outros menus abaixo)
    // passaram a ser inicializadas com "= 0". Não mudava o comportamento (o
    // corpo do do-while sempre roda antes da condição ser checada), mas
    // deixar uma variável sem valor inicial é um hábito perigoso - um dia
    // alguém reordena o código e isso vira leitura de lixo de memória.
    int opcao = 0;
    do {
        limparTela();
        cout << "\n=== E-scambo Terminal ===" << endl;
        cout << "1 - Fazer Login" << endl;
        cout << "2 - Cadastrar Novo Usuario" << endl;
        cout << "3 - Sair" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcaoInteira();

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
    cout << "\n--- Cadastro de Novo Usuario ---" << endl;
    cout << "Digite seu Nome: ";
    string nome = lerLinha();
    cout << "Digite seu E-mail (login): ";
    string login = lerLinha();
    cout << "Digite sua Senha: ";
    string senha = lerLinha();
    cout << "Digite sua Chave PIX (Enter para deixar em branco): ";
    string chavePix = lerLinha();

    if (sistema.getUsuarios().registrarUsuario(nome, login, senha, chavePix)) {
        cout << "Cadastro realizado com sucesso!" << endl;
    } else {
        cout << "Erro: Este e-mail ja esta cadastrado no sistema!" << endl;
    }

    pausar();
}

void TerminalUI::menuLogin() {
    limparTela();
    cout << "\n--- Acesso ao Sistema ---" << endl;
    cout << "Digite seu e-mail (login): ";
    string login = lerLinha();
    cout << "Digite sua senha: ";
    string senha = lerLinha();

    Usuario* usuarioAutenticado = sistema.getUsuarios().autenticar(login, senha);

    if (usuarioAutenticado != nullptr) {
        cout << "\nLogin bem-sucedido!" << endl;
        menuEscolhaPerfil(usuarioAutenticado);
    } else {
        cout << "\nFalha na autenticacao. Verifique seu e-mail e senha." << endl;
        pausar();
    }
}

void TerminalUI::menuEscolhaPerfil(Usuario* usuario) {
    int opcao = 0;

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
        opcao = lerOpcaoInteira();

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
    int opcao = 0;
    do {
        limparTela();
        cout << "\n=== Painel do Anunciante ===" << endl;
        cout << "1 - Cadastrar Novo Produto" << endl;
        cout << "2 - Ver Minha Vitrine" << endl;
        cout << "3 - Editar um Produto" << endl;
        cout << "4 - Notificacoes" << endl;
        cout << "5 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcaoInteira();

        if (opcao == 1) {
            limparTela();
            cout << "\n--- Cadastro de Produto ---" << endl;
            cout << "Digite o nome do produto: ";
            string nome = lerLinha();

            cout << "Digite o preco do produto: R$ ";
            double preco = lerValorDouble();

            cout << "\nEscolha a Categoria:" << endl;
            cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
            int opcaoCat = lerOpcaoInteira();

            string categoriaEscolhida;
            if (opcaoCat == 1) categoriaEscolhida = "Veiculo";
            else if (opcaoCat == 2) categoriaEscolhida = "Eletrodomestico";
            else if (opcaoCat == 3) categoriaEscolhida = "Roupa";
            else categoriaEscolhida = "Outros";

            // Se for "Outros", define a subcategoria automaticamente e pula o menu
            string subcategoriaEscolhida;
            if (categoriaEscolhida == "Outros") {
                subcategoriaEscolhida = "Geral";
            }
            else {
                // Só imprime e pede a subcategoria se a categoria principal NÃO for "Outros"
                cout << "\nEscolha a Subcategoria:" << endl;
                subcategoriaEscolhida = escolherSubcategoria(categoriaEscolhida);
            }

            cout << "\nDigite a quantidade em estoque: ";
            int quantidadeEstoque = lerOpcaoInteira();
            if (quantidadeEstoque <= 0) quantidadeEstoque = 1;

            sistema.getProdutos().cadastrarProduto(nome, preco, categoriaEscolhida, subcategoriaEscolhida, usuario->getLogin(), usuario->getId(), quantidadeEstoque);

            cout << "\nProduto cadastrado com sucesso! O ID foi gerado automaticamente." << endl;
            pausar();

        } else if (opcao == 2) {
            limparTela();
            cout << "\n--- Minha Vitrine ---" << endl;
            const auto& produtos = sistema.getProdutos().get_produtos();

            bool temProdutoMeu = false;
            for (const auto& p : produtos) {
                if (p.get_login_anunciante() == usuario->getLogin() && p.is_ativo()) {
                    cout << "ID: " << p.get_id()
                         << " | Nome: " << p.get_nome()
                         << "\n  Categoria: " << p.get_categoria() << " > " << p.get_subcategoria()
                         << "\n  Preco: R$ " << p.get_preco()
                         << "\n  Estoque: " << p.get_quantidade_estoque() << "\n" << endl;
                    temProdutoMeu = true;
                }
            }

            if (!temProdutoMeu) {
                cout << "Voce nao tem nenhum produto ativo na vitrine no momento." << endl;
            }

            pausar();

        } else if (opcao == 3) {
            limparTela();
            cout << "\n--- Editar Produto ---" << endl;

            const auto& produtos = sistema.getProdutos().get_produtos();
            vector<const Produto*> meusProdutos;

            for (const auto& p : produtos) {
                if (p.get_login_anunciante() == usuario->getLogin()) {
                    meusProdutos.push_back(&p);
                }
            }

            if (meusProdutos.empty()) {
                cout << "Voce ainda nao tem nenhum produto cadastrado para editar." << endl;
                pausar();
            } else {
                cout << "Seus produtos disponiveis:\n" << endl;

                for (size_t i = 0; i < meusProdutos.size(); ++i) {
                    cout << "(" << i + 1 << ") " << meusProdutos[i]->get_nome()
                         << " | R$ " << meusProdutos[i]->get_preco()
                         << " | ID: " << meusProdutos[i]->get_id() << endl;
                }

                cout << "\nDigite o numero do produto que deseja editar (ou 0 para cancelar): ";
                int escolha = lerOpcaoInteira();

                if (escolha > 0 && escolha <= static_cast<int>(meusProdutos.size())) {

                    const Produto* prodAtual = meusProdutos[escolha - 1];
                    string idSelecionado = prodAtual->get_id();

                    string nomeTemp = prodAtual->get_nome();
                    double precoTemp = prodAtual->get_preco();
                    string catTemp = prodAtual->get_categoria();
                    string subTemp = prodAtual->get_subcategoria();

                    int opcaoEdit = 0;
                    do {
                        limparTela();
                        cout << "\n--- Editando: " << nomeTemp << " ---" << endl;
                        cout << "1 - Editar Nome (Atual: " << nomeTemp << ")" << endl;
                        cout << "2 - Editar Preco (Atual: R$ " << precoTemp << ")" << endl;
                        cout << "3 - Editar Categoria/Subcategoria (Atual: " << catTemp << " > " << subTemp << ")" << endl;
                        cout << "4 - Salvar alteracoes e Voltar" << endl;
                        cout << "5 - Cancelar sem salvar" << endl;
                        cout << "Escolha o que deseja alterar: ";
                        opcaoEdit = lerOpcaoInteira();

                        if (opcaoEdit == 1) {
                            cout << "\nDigite o NOVO nome: ";
                            nomeTemp = lerLinha();
                        }
                        else if (opcaoEdit == 2) {
                            cout << "\nDigite o NOVO preco: R$ ";
                            precoTemp = lerValorDouble();
                        }
                        else if (opcaoEdit == 3) {
                            cout << "\nEscolha a NOVA Categoria:" << endl;
                            cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
                            int opcaoCat = lerOpcaoInteira();

                            if (opcaoCat == 1) catTemp = "Veiculo";
                            else if (opcaoCat == 2) catTemp = "Eletrodomestico";
                            else if (opcaoCat == 3) catTemp = "Roupa";
                            else catTemp = "Outros";

                            // REVISÃO (bug corrigido): faltava a opcao "Outros" aqui -
                            // dava pra CADASTRAR um produto como "Outros", mas nunca
                            // dava pra EDITAR um produto existente pra essa categoria
                            // (ou tirar ele de "Outros"). Agora espelha o mesmo
                            // tratamento do Cadastro: "Outros" pula direto pra "Geral".
                            if (catTemp == "Outros") {
                                subTemp = "Geral";
                            } else {
                                cout << "\nEscolha a NOVA Subcategoria:" << endl;
                                subTemp = escolherSubcategoria(catTemp);
                            }
                        }
                        else if (opcaoEdit == 4) {
                            sistema.getProdutos().editarProduto(idSelecionado, nomeTemp, precoTemp, catTemp, subTemp);
                            cout << "\nProduto atualizado com sucesso!" << endl;
                            pausar();
                        }
                        else if (opcaoEdit == 5) {
                            cout << "\nEdicao cancelada. Nenhuma alteracao foi salva." << endl;
                            pausar();
                        }

                    } while (opcaoEdit != 4 && opcaoEdit != 5);

                } else if (escolha != 0) {
                    cout << "\nOpcao invalida!" << endl;
                    pausar();
                } else {
                    cout << "\nEdicao cancelada." << endl;
                    pausar();
                }
            }
        }

        else if (opcao == 4) {
            limparTela();
            cout << "\n=== CAIXA DE ENTRADA (NOTIFICACOES) ===" << endl;
            

            // Não há um sistema de notificações "push" ou assíncrono — é só essa tela que,
            // quando aberta pelo vendedor, consulta as transações pendentes armazenadas e as lista como caixa de entrada
            
            vector<Transacao*> propostas = sistema.getTransacoes().buscarPropostasRecebidas(usuario);   

            if (propostas.empty()) {
                cout << "Nao tens propostas pendentes no momento." << endl;
            } else {
                for (Transacao* t : propostas) {
                    // dynamic_cast aqui: a caixa de entrada lista qualquer Transacao
                    // pendente, mas só sabemos montar a tela de "aceitar/recusar
                    // troca" se for de fato uma Troca - hoje é o único tipo que
                    // chega aqui na prática (ver nota de arquitetura em Compra.hpp).
                    if (Troca* troca = dynamic_cast<Troca*>(t)) {
                        cout << "\n[NOVA PROPOSTA DE E-SCAMBO]" << endl;
                        cout << "De: " << troca->get_usuario_proponente()->getNome() << endl;
                        cout << "Quer : " << troca->get_anuncio_alvo()->get_produto()->get_nome() << endl;
                        cout << "Oferece: " << troca->get_anuncio_ofertado()->get_produto()->get_nome() << endl;
                        cout << "Mensagem: \"" << troca->get_mensagem() << "\"" << endl;

                        cout << "\n1 - Aceitar\n2 - Recusar\n3 - Decidir Depois\nOpcao: ";
                        int resp = lerOpcaoInteira();

                        if (resp == 1) {
                            sistema.processarRespostaTroca(t, true);
                            cout << "\n[SUCESSO] Malou na catira! Os produtos foram removidos do mercado." << endl;
                        } else if (resp == 2) {
                            sistema.processarRespostaTroca(t, false);
                            cout << "\n[AVISO] Proposta recusada." << endl;
                        }
                    }
                }
            }

            pausar();
        }

    } while (opcao != 5);
}

// ==========================================
// MÓDULO DO COMPRADOR
// ==========================================
void TerminalUI::menuComprador(Usuario* usuario) {
    int opcao = 0;
    do {
        limparTela();
        cout << "\n=== Area de Compras ===" << endl;
        cout << "Bem-vindo(a), " << usuario->getNome() << "!" << endl;
        cout << "1 - Ver Vitrine Global / Pesquisar" << endl;
        cout << "2 - Meu Carrinho" << endl;
        cout << "3 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcaoInteira();

        if (opcao == 1) {
            limparTela();
            cout << "=== Menu de Busca de Produtos ===" << endl;
            cout << "1 - Ver TODOS os produtos disponiveis" << endl;
            cout << "2 - Pesquisar por Categoria e Subcategoria" << endl;
            cout << "3 - Pesquisar por Nome" << endl;
            cout << "Escolha uma opcao: ";
            int opcaoFiltro = lerOpcaoInteira();

            string categoriaFiltro = "";
            string subcategoriaFiltro = "";
            string nomeFiltro = "";

            if (opcaoFiltro == 2) {
                limparTela();
                cout << "\n--- Selecione a Categoria desejada ---" << endl;
                cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
                int opcaoCat = lerOpcaoInteira();

                if (opcaoCat == 1) categoriaFiltro = "Veiculo";
                else if (opcaoCat == 2) categoriaFiltro = "Eletrodomestico";
                else if (opcaoCat == 3) categoriaFiltro = "Roupa";
                // REVISÃO (bug corrigido): faltava a opcao "Outros" no filtro -
                // um produto cadastrado como "Outros" nunca podia ser encontrado
                // por essa busca, só pela busca por Nome. Agora o filtro cobre
                // as mesmas categorias que existem no Cadastro.
                else if (opcaoCat == 4) categoriaFiltro = "Outros";

                cout << "\nDeseja refinar a busca selecionando uma Subcategoria?\n1 - Sim\n2 - Nao\nOpcao: ";
                int querSub = lerOpcaoInteira();

                if (querSub == 1) {
                    cout << "\n--- Selecione a Subcategoria ---" << endl;
                    subcategoriaFiltro = escolherSubcategoria(categoriaFiltro);
                }
            }
            else if (opcaoFiltro == 3) {
                limparTela();
                cout << "\n--- Pesquisa por Nome ---" << endl;
                cout << "Digite o nome ou palavra-chave: ";
                nomeFiltro = lerLinha();
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

            const auto& produtos = sistema.getProdutos().get_produtos();
            vector<const Produto*> produtosExibidos;

            for (const auto& p : produtos) {
                if (p.get_login_anunciante() != usuario->getLogin() && p.is_ativo()) {

                    if (!categoriaFiltro.empty() && p.get_categoria() != categoriaFiltro) continue;
                    if (!subcategoriaFiltro.empty() && p.get_subcategoria() != subcategoriaFiltro) continue;

                    if (!nomeFiltro.empty()) {
                        string nomeDoBancoLow = p.get_nome();
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
                         << "\n    Estoque: " << p.get_quantidade_estoque()
                         << "\n    Vendedor: " << p.get_login_anunciante() << "\n" << endl;
                }
            }

            if (produtosExibidos.empty()) {
                cout << "Nenhum produto correspondente foi encontrado." << endl;
                pausar();
            } else {
                cout << "---------------------------------" << endl;
                cout << "Digite o NUMERO do produto que deseja adicionar ao carrinho (ou 0 para voltar): ";
                int escolhaProduto = lerOpcaoInteira();

                if (escolhaProduto > 0 && escolhaProduto <= static_cast<int>(produtosExibidos.size())) {

                    const Produto* p = produtosExibidos[escolhaProduto - 1];

                    cout << "Produto selecionado: " << p->get_nome() << " | R$ " << p->get_preco()
                         << " | Estoque: " << p->get_quantidade_estoque() << endl;
                    cout << "Digite a quantidade que deseja adicionar ao carrinho: ";
                    int qtd = lerOpcaoInteira();

                    if (qtd <= 0) {
                        cout << "\nQuantidade invalida." << endl;
                    } else if (qtd > p->get_quantidade_estoque()) {
                        cout << "\nEstoque insuficiente! Disponivel: " << p->get_quantidade_estoque() << endl;
                    } else {
                        ItemVendido novoItem(p->get_id(), p->get_nome(), p->get_preco(), qtd);
                        carrinhoCompras.adicionarItem(novoItem);

                        // Salva automaticamente após adicionar ao carrinho
                        carrinhoCompras.salvarCarrinho(usuario->getLogin());

                        cout << "\nSucesso! " << qtd << "x " << p->get_nome() << " adicionado(s) ao carrinho e salvo." << endl;
                    }
                } else if (escolhaProduto != 0) {
                    cout << "\nErro: Numero invalido." << endl;
                }

                pausar();
            }

        } else if (opcao == 2) {
            menuCarrinho(usuario);
        }

    } while (opcao != 3);
}

// ==========================================
// MÓDULO DO CARRINHO (Checkout: PIX, E-scambo e gestao)
// ==========================================
void TerminalUI::menuCarrinho(Usuario* usuario) {
    int opcao = 0;
    do {
        limparTela();
        cout << "\n=== Meu Carrinho ===" << endl;

        const auto& itens = carrinhoCompras.get_itens();

        if (itens.empty()) {
            cout << "Seu carrinho esta vazio no momento." << endl;
        } else {
            for (size_t i = 0; i < itens.size(); ++i) {
                cout << "(" << i + 1 << ") " << itens[i].get_nomeProduto()
                     << " | Qtd: " << itens[i].get_quantidade()
                     << " | Subtotal: R$ " << itens[i].get_subtotal() << endl;
            }
            cout << "---------------------------------" << endl;
            cout << "TOTAL DO PEDIDO: R$ " << carrinhoCompras.get_total() << endl;
        }

        cout << "\n1 - Finalizar Compra (PIX)" << endl;
        cout << "2 - Propor E-scambo" << endl;
        cout << "3 - Esvaziar Carrinho" << endl;
        cout << "4 - Voltar" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcaoInteira();

        if (opcao == 1) {
            limparTela();
            cout << "\n--- Finalizar Compra ---" << endl;

            if (carrinhoCompras.get_itens().empty()) {
                cout << "Seu carrinho esta vazio. Nao ha o que finalizar." << endl;
            } else {
                for (const auto& item : carrinhoCompras.get_itens()) {
                    cout << "\nProduto: " << item.get_nomeProduto()
                         << " | Qtd: " << item.get_quantidade()
                         << " | Subtotal: R$ " << item.get_subtotal() << endl;

                    Produto* produtoOriginal = sistema.getProdutos().buscarProdutoPorId(item.get_idProduto());
                    if (produtoOriginal) {
                        Usuario* vendedor = sistema.getUsuarios().buscarUsuarioPorLogin(produtoOriginal->get_login_anunciante());
                        if (vendedor) {
                            cout << "Realize o PIX para " << vendedor->getNome()
                                 << " | Chave PIX: " << vendedor->getChavePix() << endl;
                        } else {
                            cout << "Aviso: nao foi possivel localizar os dados de PIX do vendedor." << endl;
                        }

                        if (!sistema.finalizarCompra(usuario, produtoOriginal, item.get_quantidade())) {
                            cout << "Aviso: estoque insuficiente no momento da finalizacao." << endl;
                        }
                    } else {
                        cout << "Aviso: este produto nao esta mais disponivel no sistema." << endl;
                    }
                }

                carrinhoCompras.esvaziar();
                carrinhoCompras.salvarCarrinho(usuario->getLogin());
                cout << "\nCompra finalizada com sucesso! O carrinho foi esvaziado." << endl;
            }

            pausar();

        } else if (opcao == 2) {
            limparTela();
            cout << "\n--- Propor E-scambo a partir do Carrinho ---" << endl;

            const auto& itensTroca = carrinhoCompras.get_itens();

            if (itensTroca.empty()) {
                cout << "Seu carrinho esta vazio. Adicione um produto antes de propor troca." << endl;
                pausar();
            } else {
                for (size_t i = 0; i < itensTroca.size(); ++i) {
                    cout << "(" << i + 1 << ") " << itensTroca[i].get_nomeProduto()
                         << " | Qtd: " << itensTroca[i].get_quantidade() << endl;
                }

                cout << "\nDigite o NUMERO do item do carrinho a usar como ALVO da troca (ou 0 para cancelar): ";
                int escolhaAlvo = lerOpcaoInteira();

                if (escolhaAlvo > 0 && escolhaAlvo <= static_cast<int>(itensTroca.size())) {
                    // REVISÃO: const& em vez de copiar a string (get_idProduto()
                    // já devolve const string&). O ItemVendido referenciado segue
                    // vivo dentro de itensTroca durante todo este escopo, então a
                    // referência não corre risco de virar dangling aqui.
                    const string& idAlvo = itensTroca[escolhaAlvo - 1].get_idProduto();
                    Produto* produtoAlvo = sistema.getProdutos().buscarProdutoPorId(idAlvo);

                    if (!produtoAlvo) {
                        cout << "\nAviso: este produto nao esta mais disponivel no sistema." << endl;
                    } else {
                        vector<Produto*> meusProdutos = sistema.buscarProdutosDoUsuario(usuario->getLogin());

                        if (meusProdutos.empty()) {
                            cout << "\nVoce nao tem produtos ativos para oferecer em troca." << endl;
                        } else {
                            cout << "\nSeus produtos disponiveis para oferecer:\n" << endl;
                            for (size_t i = 0; i < meusProdutos.size(); ++i) {
                                cout << "(" << i + 1 << ") " << meusProdutos[i]->get_nome()
                                     << " | R$ " << meusProdutos[i]->get_preco()
                                     << " | ID: " << meusProdutos[i]->get_id() << endl;
                            }

                            cout << "\nDigite o NUMERO do produto que deseja oferecer (ou 0 para cancelar): ";
                            int escolhaOferta = lerOpcaoInteira();

                            if (escolhaOferta > 0 && escolhaOferta <= static_cast<int>(meusProdutos.size())) {
                                Produto* produtoOferta = meusProdutos[escolhaOferta - 1];

                                cout << "\nDigite uma mensagem para o vendedor: ";
                                string mensagem = lerLinha();

                                Usuario* receptor = sistema.getUsuarios().buscarUsuarioPorLogin(produtoAlvo->get_login_anunciante());
                                Anuncio* anuncioAlvo = sistema.obterAnuncio(produtoAlvo);
                                Anuncio* anuncioOferta = sistema.obterAnuncio(produtoOferta);

                                if (receptor && sistema.enviarPropostaTroca(usuario, receptor, anuncioAlvo, anuncioOferta, mensagem)) {
                                    cout << "\nProposta de E-scambo enviada com sucesso!" << endl;
                                    carrinhoCompras.removerItem(static_cast<size_t>(escolhaAlvo - 1));
                                    carrinhoCompras.salvarCarrinho(usuario->getLogin());
                                } else {
                                    cout << "\nNao foi possivel enviar a proposta (verifique se os precos sao compativeis)." << endl;
                                }
                            } else if (escolhaOferta != 0) {
                                cout << "\nErro: Numero invalido." << endl;
                            }
                        }
                    }
                } else if (escolhaAlvo != 0) {
                    cout << "\nErro: Numero invalido." << endl;
                }

                pausar();
            }

        } else if (opcao == 3) {
            limparTela();
            carrinhoCompras.esvaziar();
            carrinhoCompras.salvarCarrinho(usuario->getLogin());
            cout << "\nSeu carrinho foi esvaziado." << endl;
            pausar();
        }

    } while (opcao != 4);
}
