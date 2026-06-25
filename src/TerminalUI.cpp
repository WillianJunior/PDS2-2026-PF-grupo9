#include "../include/TerminalUI.hpp"
#include "../include/Anuncio.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// ==========================================
// HELPERS PRIVADOS
// ==========================================

// Une output.pausar() + input.lerLinha() num unico passo para o fluxo.
// Substitui as ~15 repeticoes de "cout << ... ; lerLinha();" espalhadas
// pelo codigo original.
void TerminalUI::pausar() {
    output.pausar();
    input.lerLinha();
}

// Centraliza a arvore de Subcategoria que antes estava copiada em tres pontos
// (Cadastro, Edicao e Filtro da Vitrine). Se a categoria nao tem subcategoria
// propria retorna string vazia - quem chama decide o que fazer.
string TerminalUI::escolherSubcategoria(const string& categoria) {
    if (categoria == "Veiculo") {
        cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
        int opcaoSub = input.lerOpcaoInteira();
        if (opcaoSub == 1) return "Carro";
        if (opcaoSub == 2) return "Moto";
        return "Caminhao";
    }
    if (categoria == "Eletrodomestico") {
        cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
        int opcaoSub = input.lerOpcaoInteira();
        if (opcaoSub == 1) return "Cozinha";
        if (opcaoSub == 2) return "Quarto";
        if (opcaoSub == 3) return "Escritorio";
        return "Gamer";
    }
    if (categoria == "Roupa") {
        cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
        int opcaoSub = input.lerOpcaoInteira();
        if (opcaoSub == 1) return "Camisa";
        if (opcaoSub == 2) return "Calca";
        return "Tenis";
    }
    return string();
}

// ==========================================
// MENU PRINCIPAL
// ==========================================

void TerminalUI::iniciar() {
    int opcao = 0;
    do {
        output.limparTela();
        output.exibirMenuPrincipal();
        opcao = input.lerOpcaoInteira();

        switch (opcao) {
            case 1: menuLogin();    break;
            case 2: menuCadastro(); break;
            case 3:
                output.limparTela();
                output.mensagem("\nSaindo do sistema. Ate logo!");
                break;
            default:
                output.mensagem("\nOpcao invalida! Tente novamente.");
                break;
        }
    } while (opcao != 3);
}

void TerminalUI::menuCadastro() {
    output.limparTela();
    output.mensagem("\n--- Cadastro de Novo Usuario ---");

    cout << "Digite seu Nome: ";
    string nome = input.lerLinha();
    cout << "Digite seu E-mail (login): ";
    string login = input.lerLinha();
    cout << "Digite sua Senha: ";
    string senha = input.lerLinha();
    cout << "Digite sua Chave PIX (Enter para deixar em branco): ";
    string chavePix = input.lerLinha();

    if (sistema.getUsuarios().registrarUsuario(nome, login, senha, chavePix)) {
        output.mensagem("Cadastro realizado com sucesso!");
    } else {
        output.mensagem("Erro: Este e-mail ja esta cadastrado no sistema!");
    }

    pausar();
}

void TerminalUI::menuLogin() {
    output.limparTela();
    output.mensagem("\n--- Acesso ao Sistema ---");

    cout << "Digite seu e-mail (login): ";
    string login = input.lerLinha();
    cout << "Digite sua senha: ";
    string senha = input.lerLinha();

    Usuario* usuarioAutenticado = sistema.getUsuarios().autenticar(login, senha);

    if (usuarioAutenticado != nullptr) {
        output.mensagem("\nLogin bem-sucedido!");
        menuEscolhaPerfil(usuarioAutenticado);
    } else {
        output.mensagem("\nFalha na autenticacao. Verifique seu e-mail e senha.");
        pausar();
    }
}

void TerminalUI::menuEscolhaPerfil(Usuario* usuario) {
    int opcao = 0;

    // Carrega o carrinho salvo do usuario assim que ele faz login.
    carrinhoCompras.carregarCarrinho(usuario->getLogin());

    do {
        output.limparTela();
        output.exibirMenuPerfil(usuario->getNome());
        opcao = input.lerOpcaoInteira();

        switch (opcao) {
            case 1: menuComprador(usuario);  break;
            case 2: menuAnunciante(usuario); break;
            case 3:
                // Salva o carrinho antes de sair.
                carrinhoCompras.salvarCarrinho(usuario->getLogin());
                carrinhoCompras.esvaziar();
                output.mensagem("\nFazendo logout e salvando dados...");
                break;
            default:
                output.mensagem("\nOpcao invalida! Tente novamente.");
                break;
        }
    } while (opcao != 3);
}

// ==========================================
// MODULO DO ANUNCIANTE
// ==========================================

void TerminalUI::menuAnunciante(Usuario* usuario) {
    int opcao = 0;
    do {
        output.limparTela();
        output.exibirMenuAnunciante();
        opcao = input.lerOpcaoInteira();

        if (opcao == 1) {
            // --- Cadastrar Produto ---
            output.limparTela();
            output.mensagem("\n--- Cadastro de Produto ---");

            cout << "Digite o nome do produto: ";
            string nome = input.lerLinha();

            cout << "Digite o preco do produto: R$ ";
            double preco = input.lerValorDouble();

            cout << "\nEscolha a Categoria:\n1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
            int opcaoCat = input.lerOpcaoInteira();

            string categoriaEscolhida;
            if (opcaoCat == 1)      categoriaEscolhida = "Veiculo";
            else if (opcaoCat == 2) categoriaEscolhida = "Eletrodomestico";
            else if (opcaoCat == 3) categoriaEscolhida = "Roupa";
            else                    categoriaEscolhida = "Outros";

            string subcategoriaEscolhida;
            if (categoriaEscolhida == "Outros") {
                subcategoriaEscolhida = "Geral";
            } else {
                cout << "\nEscolha a Subcategoria:\n";
                subcategoriaEscolhida = escolherSubcategoria(categoriaEscolhida);
            }

            cout << "\nDigite a quantidade em estoque: ";
            int quantidadeEstoque = input.lerOpcaoInteira();
            if (quantidadeEstoque <= 0) quantidadeEstoque = 1;

            sistema.getProdutos().cadastrarProduto(nome, preco, categoriaEscolhida,
                subcategoriaEscolhida, usuario->getLogin(), usuario->getId(), quantidadeEstoque);

            output.mensagem("\nProduto cadastrado com sucesso! O ID foi gerado automaticamente.");
            pausar();

        } else if (opcao == 2) {
            // --- Ver Vitrine ---
            output.limparTela();
            const auto& todos = sistema.getProdutos().get_produtos();
            vector<const Produto*> meus;
            for (const auto& p : todos) {
                if (p.get_login_anunciante() == usuario->getLogin() && p.is_ativo()) {
                    meus.push_back(&p);
                }
            }
            output.exibirMeusProdutos(meus);
            pausar();

        } else if (opcao == 3) {
            // --- Editar Produto ---
            output.limparTela();
            output.mensagem("\n--- Editar Produto ---");

            const auto& todos = sistema.getProdutos().get_produtos();
            vector<const Produto*> meusProdutos;
            for (const auto& p : todos) {
                if (p.get_login_anunciante() == usuario->getLogin()) {
                    meusProdutos.push_back(&p);
                }
            }

            if (meusProdutos.empty()) {
                output.mensagem("Voce ainda nao tem nenhum produto cadastrado para editar.");
                pausar();
            } else {
                output.mensagem("Seus produtos disponiveis:\n");
                for (size_t i = 0; i < meusProdutos.size(); ++i) {
                    cout << "(" << i + 1 << ") " << meusProdutos[i]->get_nome()
                         << " | R$ " << meusProdutos[i]->get_preco()
                         << " | ID: " << meusProdutos[i]->get_id() << endl;
                }

                cout << "\nDigite o numero do produto que deseja editar (ou 0 para cancelar): ";
                int escolha = input.lerOpcaoInteira();

                if (escolha > 0 && escolha <= static_cast<int>(meusProdutos.size())) {
                    const Produto* prodAtual = meusProdutos[escolha - 1];
                    string idSelecionado = prodAtual->get_id();
                    string nomeTemp  = prodAtual->get_nome();
                    double precoTemp = prodAtual->get_preco();
                    string catTemp   = prodAtual->get_categoria();
                    string subTemp   = prodAtual->get_subcategoria();

                    int opcaoEdit = 0;
                    do {
                        output.limparTela();
                        output.exibirMenuEdicaoProduto(nomeTemp, precoTemp, catTemp, subTemp);
                        opcaoEdit = input.lerOpcaoInteira();

                        if (opcaoEdit == 1) {
                            cout << "\nDigite o NOVO nome: ";
                            nomeTemp = input.lerLinha();
                        } else if (opcaoEdit == 2) {
                            cout << "\nDigite o NOVO preco: R$ ";
                            precoTemp = input.lerValorDouble();
                        } else if (opcaoEdit == 3) {
                            cout << "\nEscolha a NOVA Categoria:\n1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
                            int opcaoCat = input.lerOpcaoInteira();
                            if (opcaoCat == 1)      catTemp = "Veiculo";
                            else if (opcaoCat == 2) catTemp = "Eletrodomestico";
                            else if (opcaoCat == 3) catTemp = "Roupa";
                            else                    catTemp = "Outros";

                            if (catTemp == "Outros") {
                                subTemp = "Geral";
                            } else {
                                cout << "\nEscolha a NOVA Subcategoria:\n";
                                subTemp = escolherSubcategoria(catTemp);
                            }
                        } else if (opcaoEdit == 4) {
                            sistema.getProdutos().editarProduto(idSelecionado, nomeTemp, precoTemp, catTemp, subTemp);
                            output.mensagem("\nProduto atualizado com sucesso!");
                            pausar();
                        } else if (opcaoEdit == 5) {
                            output.mensagem("\nEdicao cancelada. Nenhuma alteracao foi salva.");
                            pausar();
                        }
                    } while (opcaoEdit != 4 && opcaoEdit != 5);

                } else if (escolha != 0) {
                    output.mensagem("\nOpcao invalida!");
                    pausar();
                } else {
                    output.mensagem("\nEdicao cancelada.");
                    pausar();
                }
            }

        } else if (opcao == 4) {
            // --- Notificacoes ---
            output.limparTela();
            output.mensagem("\n=== CAIXA DE ENTRADA (NOTIFICACOES) ===");

            vector<Transacao*> propostas = sistema.getTransacoes().buscarPropostasRecebidas(usuario);

            if (propostas.empty()) {
                output.mensagem("Nao tens propostas pendentes no momento.");
            } else {
                for (Transacao* t : propostas) {
                    // dynamic_cast: so sabemos montar a tela de aceitar/recusar
                    // se for de fato uma Troca (unico tipo que chega aqui na pratica).
                    if (Troca* troca = dynamic_cast<Troca*>(t)) {
                        output.exibirPropostaTroca(troca);
                        int resp = input.lerOpcaoInteira();

                        if (resp == 1) {
                            sistema.processarRespostaTroca(t, true);
                            output.mensagem("\n[SUCESSO] Malou na catira! Os produtos foram removidos do mercado.");
                        } else if (resp == 2) {
                            sistema.processarRespostaTroca(t, false);
                            output.mensagem("\n[AVISO] Proposta recusada.");
                        }
                    }
                }
            }

            pausar();
        }

    } while (opcao != 5);
}

// ==========================================
// MODULO DO COMPRADOR
// ==========================================

void TerminalUI::menuComprador(Usuario* usuario) {
    int opcao = 0;
    do {
        output.limparTela();
        output.exibirMenuComprador(usuario->getNome());
        opcao = input.lerOpcaoInteira();

        if (opcao == 1) {
            output.limparTela();
            output.exibirMenuBusca();
            int opcaoFiltro = input.lerOpcaoInteira();

            string categoriaFiltro, subcategoriaFiltro, nomeFiltro;

            if (opcaoFiltro == 2) {
                output.limparTela();
                output.mensagem("\n--- Selecione a Categoria desejada ---");
                cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
                int opcaoCat = input.lerOpcaoInteira();

                if (opcaoCat == 1)      categoriaFiltro = "Veiculo";
                else if (opcaoCat == 2) categoriaFiltro = "Eletrodomestico";
                else if (opcaoCat == 3) categoriaFiltro = "Roupa";
                else if (opcaoCat == 4) categoriaFiltro = "Outros";

                cout << "\nDeseja refinar a busca selecionando uma Subcategoria?\n1 - Sim\n2 - Nao\nOpcao: ";
                int querSub = input.lerOpcaoInteira();
                if (querSub == 1) {
                    output.mensagem("\n--- Selecione a Subcategoria ---");
                    subcategoriaFiltro = escolherSubcategoria(categoriaFiltro);
                }
            } else if (opcaoFiltro == 3) {
                output.limparTela();
                output.mensagem("\n--- Pesquisa por Nome ---");
                cout << "Digite o nome ou palavra-chave: ";
                nomeFiltro = input.lerLinha();
            }

            output.limparTela();
            if (!categoriaFiltro.empty()) {
                cout << "Filtro ativo: " << categoriaFiltro;
                if (!subcategoriaFiltro.empty()) cout << " > " << subcategoriaFiltro;
                cout << "\n" << endl;
            } else if (!nomeFiltro.empty()) {
                cout << "Buscando por: \"" << nomeFiltro << "\"\n" << endl;
            }

            const auto& todos = sistema.getProdutos().get_produtos();
            vector<const Produto*> produtosExibidos;

            for (const auto& p : todos) {
                if (p.get_login_anunciante() == usuario->getLogin() || !p.is_ativo()) continue;
                if (!categoriaFiltro.empty() && p.get_categoria() != categoriaFiltro) continue;
                if (!subcategoriaFiltro.empty() && p.get_subcategoria() != subcategoriaFiltro) continue;
                if (!nomeFiltro.empty()) {
                    string nLow = p.get_nome();
                    string bLow = nomeFiltro;
                    transform(nLow.begin(), nLow.end(), nLow.begin(), ::tolower);
                    transform(bLow.begin(), bLow.end(), bLow.begin(), ::tolower);
                    if (nLow.find(bLow) == string::npos) continue;
                }
                produtosExibidos.push_back(&p);
            }

            output.exibirVitrine(produtosExibidos);

            if (produtosExibidos.empty()) {
                output.mensagem("Nenhum produto correspondente foi encontrado.");
                pausar();
            } else {
                cout << "---------------------------------" << endl;
                cout << "Digite o NUMERO do produto que deseja adicionar ao carrinho (ou 0 para voltar): ";
                int escolhaProduto = input.lerOpcaoInteira();

                if (escolhaProduto > 0 && escolhaProduto <= static_cast<int>(produtosExibidos.size())) {
                    const Produto* p = produtosExibidos[escolhaProduto - 1];
                    cout << "Produto selecionado: " << p->get_nome()
                         << " | R$ " << p->get_preco()
                         << " | Estoque: " << p->get_quantidade_estoque() << endl;
                    cout << "Digite a quantidade que deseja adicionar ao carrinho: ";
                    int qtd = input.lerOpcaoInteira();

                    if (qtd <= 0) {
                        output.mensagem("\nQuantidade invalida.");
                    } else if (qtd > p->get_quantidade_estoque()) {
                        cout << "\nEstoque insuficiente! Disponivel: " << p->get_quantidade_estoque() << endl;
                    } else {
                        ItemVendido novoItem(p->get_id(), p->get_nome(), p->get_preco(), qtd);
                        carrinhoCompras.adicionarItem(novoItem);
                        carrinhoCompras.salvarCarrinho(usuario->getLogin());
                        cout << "\nSucesso! " << qtd << "x " << p->get_nome() << " adicionado(s) ao carrinho e salvo." << endl;
                    }
                } else if (escolhaProduto != 0) {
                    output.mensagem("\nErro: Numero invalido.");
                }

                pausar();
            }

        } else if (opcao == 2) {
            menuCarrinho(usuario);
        }

    } while (opcao != 3);
}

// ==========================================
// MODULO DO CARRINHO (Checkout: PIX, E-scambo e gestao)
// ==========================================

void TerminalUI::menuCarrinho(Usuario* usuario) {
    int opcao = 0;
    do {
        output.limparTela();
        output.exibirMenuCarrinho(carrinhoCompras.get_itens(), carrinhoCompras.get_total());
        opcao = input.lerOpcaoInteira();

        if (opcao == 1) {
            // --- Finalizar Compra (PIX) ---
            output.limparTela();
            output.mensagem("\n--- Finalizar Compra ---");

            if (carrinhoCompras.get_itens().empty()) {
                output.mensagem("Seu carrinho esta vazio. Nao ha o que finalizar.");
            } else {
                for (const auto& item : carrinhoCompras.get_itens()) {
                    cout << "\nProduto: " << item.get_nomeProduto()
                         << " | Qtd: " << item.get_quantidade()
                         << " | Subtotal: R$ " << item.get_subtotal() << endl;

                    Produto* produtoOriginal = sistema.getProdutos().buscarProdutoPorId(item.get_idProduto());
                    if (produtoOriginal) {
                        Usuario* vendedor = sistema.getUsuarios().buscarUsuarioPorLogin(
                            produtoOriginal->get_login_anunciante());
                        if (vendedor) {
                            cout << "Realize o PIX para " << vendedor->getNome()
                                 << " | Chave PIX: " << vendedor->getChavePix() << endl;
                        } else {
                            output.mensagem("Aviso: nao foi possivel localizar os dados de PIX do vendedor.");
                        }

                        if (!sistema.finalizarCompra(usuario, produtoOriginal, item.get_quantidade())) {
                            output.mensagem("Aviso: estoque insuficiente no momento da finalizacao.");
                        }
                    } else {
                        output.mensagem("Aviso: este produto nao esta mais disponivel no sistema.");
                    }
                }

                carrinhoCompras.esvaziar();
                carrinhoCompras.salvarCarrinho(usuario->getLogin());
                output.mensagem("\nCompra finalizada com sucesso! O carrinho foi esvaziado.");
            }

            pausar();

        } else if (opcao == 2) {
            // --- Propor E-scambo ---
            output.limparTela();
            output.mensagem("\n--- Propor E-scambo a partir do Carrinho ---");

            const auto& itensTroca = carrinhoCompras.get_itens();

            if (itensTroca.empty()) {
                output.mensagem("Seu carrinho esta vazio. Adicione um produto antes de propor troca.");
                pausar();
            } else {
                for (size_t i = 0; i < itensTroca.size(); ++i) {
                    cout << "(" << i + 1 << ") " << itensTroca[i].get_nomeProduto()
                         << " | Qtd: " << itensTroca[i].get_quantidade() << endl;
                }

                cout << "\nDigite o NUMERO do item do carrinho a usar como ALVO da troca (ou 0 para cancelar): ";
                int escolhaAlvo = input.lerOpcaoInteira();

                if (escolhaAlvo > 0 && escolhaAlvo <= static_cast<int>(itensTroca.size())) {
                    const string& idAlvo = itensTroca[escolhaAlvo - 1].get_idProduto();
                    Produto* produtoAlvo = sistema.getProdutos().buscarProdutoPorId(idAlvo);

                    if (!produtoAlvo) {
                        output.mensagem("\nAviso: este produto nao esta mais disponivel no sistema.");
                    } else {
                        vector<Produto*> meusProdutos = sistema.buscarProdutosDoUsuario(usuario->getLogin());

                        if (meusProdutos.empty()) {
                            output.mensagem("\nVoce nao tem produtos ativos para oferecer em troca.");
                        } else {
                            output.exibirProdutosParaOferta(meusProdutos);

                            cout << "\nDigite o NUMERO do produto que deseja oferecer (ou 0 para cancelar): ";
                            int escolhaOferta = input.lerOpcaoInteira();

                            if (escolhaOferta > 0 && escolhaOferta <= static_cast<int>(meusProdutos.size())) {
                                Produto* produtoOferta = meusProdutos[escolhaOferta - 1];

                                cout << "\nDigite uma mensagem para o vendedor: ";
                                string mensagem = input.lerLinha();

                                Usuario* receptor = sistema.getUsuarios().buscarUsuarioPorLogin(
                                    produtoAlvo->get_login_anunciante());
                                Anuncio* anuncioAlvo    = sistema.obterAnuncio(produtoAlvo);
                                Anuncio* anuncioOferta  = sistema.obterAnuncio(produtoOferta);

                                if (receptor && sistema.enviarPropostaTroca(
                                        usuario, receptor, anuncioAlvo, anuncioOferta, mensagem)) {
                                    output.mensagem("\nProposta de E-scambo enviada com sucesso!");
                                    carrinhoCompras.removerItem(static_cast<size_t>(escolhaAlvo - 1));
                                    carrinhoCompras.salvarCarrinho(usuario->getLogin());
                                } else {
                                    output.mensagem("\nNao foi possivel enviar a proposta (verifique se os precos sao compativeis).");
                                }
                            } else if (escolhaOferta != 0) {
                                output.mensagem("\nErro: Numero invalido.");
                            }
                        }
                    }
                } else if (escolhaAlvo != 0) {
                    output.mensagem("\nErro: Numero invalido.");
                }

                pausar();
            }

        } else if (opcao == 3) {
            output.limparTela();
            carrinhoCompras.esvaziar();
            carrinhoCompras.salvarCarrinho(usuario->getLogin());
            output.mensagem("\nSeu carrinho foi esvaziado.");
            pausar();
        }

    } while (opcao != 4);
}
CPPEOF
