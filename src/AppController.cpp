#include "../include/AppController.hpp"
#include "../include/Anuncio.hpp"
#include "../include/Troca.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

using namespace std;

// ============================================================
// Ponto de entrada
// ============================================================

void AppController::executar() {
    int opcao = 0;
    do {
        _view.limparTela();
        _view.exibirMenuPrincipal();
        opcao = _view.lerOpcaoInteira();

        switch (opcao) {
            case 1: fluxoLogin();    break;
            case 2: fluxoCadastro(); break;
            case 3:
                _view.limparTela();
                _view.mensagem("\nSaindo do sistema. Ate logo!");
                break;
            default:
                _view.mensagem("\nOpcao invalida! Tente novamente.");
                break;
        }
    } while (opcao != 3);
}

// ============================================================
// Autenticação e cadastro
// ============================================================

void AppController::fluxoCadastro() {
    _view.limparTela();
    _view.mensagem("\n--- Cadastro de Novo Usuario ---");

    auto dados = _view.solicitarDadosCadastro();

    if (_sistema.getUsuarios().registrarUsuario(dados.nome, dados.login, dados.senha, dados.chavePix)) {
        _view.mensagem("Cadastro realizado com sucesso!");
    } else {
        _view.mensagem("Erro: Este e-mail ja esta cadastrado no sistema!");
    }

    _view.pausar();
}

void AppController::fluxoLogin() {
    _view.limparTela();
    _view.mensagem("\n--- Acesso ao Sistema ---");

    auto [login, senha] = _view.solicitarCredenciaisLogin();

    Usuario* usuario = _sistema.getUsuarios().autenticar(login, senha);

    if (usuario != nullptr) {
        _view.mensagem("\nLogin bem-sucedido!");
        fluxoMenuPerfil(usuario);
    } else {
        _view.mensagem("\nFalha na autenticacao. Verifique seu e-mail e senha.");
        _view.pausar();
    }
}

// ============================================================
// Menu de perfil (escolha comprador / anunciante)
// ============================================================

void AppController::fluxoMenuPerfil(Usuario* usuario) {
    _carrinho.carregarCarrinho(usuario->getLogin());

    int opcao = 0;
    do {
        _view.limparTela();
        _view.exibirMenuPerfil(usuario->getNome());
        opcao = _view.lerOpcaoInteira();

        switch (opcao) {
            case 1: fluxoComprador(usuario);  break;
            case 2: fluxoAnunciante(usuario); break;
            case 3:
                _carrinho.salvarCarrinho(usuario->getLogin());
                _carrinho.esvaziar();
                _view.mensagem("\nFazendo logout e salvando dados...");
                break;
            default:
                _view.mensagem("\nOpcao invalida! Tente novamente.");
                break;
        }
    } while (opcao != 3);
}

// ============================================================
// Módulo do Anunciante
// ============================================================

void AppController::fluxoAnunciante(Usuario* usuario) {
    int opcao = 0;
    do {
        _view.limparTela();
        bool podeAnunciar = usuario->temCapacidade(Capacidade::ANUNCIAR)
                            && usuario->podeCriarAnuncio();
        _view.exibirMenuAnunciante(podeAnunciar,
                                   usuario->getAnunciosAtivos(),
                                   usuario->getLimiteAnuncios());
        opcao = _view.lerOpcaoInteira();

        switch (opcao) {
            case 1: fluxoCadastrarProduto(usuario); break;
            case 2: {
                _view.limparTela();
                const auto& todos = _sistema.getProdutos().get_produtos();
                vector<const Produto*> meus;
                for (const auto& p : todos) {
                    if (p.get_login_anunciante() == usuario->getLogin() && p.is_ativo())
                        meus.push_back(&p);
                }
                _view.exibirMeusProdutos(meus);
                _view.pausar();
                break;
            }
            case 3: fluxoEditarProduto(usuario);   break;
            case 4: fluxoNotificacoes(usuario);    break;
            case 5: break; // Voltar
            default:
                _view.mensagem("\nOpcao invalida!");
                break;
        }
    } while (opcao != 5);
}

void AppController::fluxoCadastrarProduto(Usuario* usuario) {
    if (!usuario->temCapacidade(Capacidade::ANUNCIAR)) {
        _view.mensagem("\nVoce nao tem permissao para anunciar produtos.");
        _view.pausar();
        return;
    }
    if (!usuario->podeCriarAnuncio()) {
        _view.mensagem("\nLimite de anuncios atingido! Voce ja possui "
                       + to_string(usuario->getAnunciosAtivos())
                       + " de " + to_string(usuario->getLimiteAnuncios())
                       + " anuncios ativos permitidos para o seu nivel.");
        _view.pausar();
        return;
    }

    _view.limparTela();
    _view.mensagem("\n--- Cadastro de Produto ---");

    auto d = _view.solicitarDadosNovoProduto();

    _sistema.getProdutos().cadastrarProduto(d.nome, d.preco, d.categoria,
        d.subcategoria, usuario->getLogin(), usuario->getId(), d.quantidade);

    usuario->incrementarAnunciosAtivos();

    _view.mensagem("\nProduto cadastrado com sucesso! O ID foi gerado automaticamente.");
    _view.pausar();
}

void AppController::fluxoEditarProduto(Usuario* usuario) {
    _view.limparTela();
    _view.mensagem("\n--- Editar Produto ---");

    const auto& todos = _sistema.getProdutos().get_produtos();
    vector<const Produto*> meusProdutos;
    for (const auto& p : todos) {
        if (p.get_login_anunciante() == usuario->getLogin())
            meusProdutos.push_back(&p);
    }

    if (meusProdutos.empty()) {
        _view.mensagem("Voce ainda nao tem nenhum produto cadastrado para editar.");
        _view.pausar();
        return;
    }

    int escolha = _view.solicitarEscolhaProdutoParaEditar(meusProdutos);

    if (escolha == 0) {
        _view.mensagem("\nEdicao cancelada.");
        _view.pausar();
        return;
    }

    if (escolha < 1 || escolha > static_cast<int>(meusProdutos.size())) {
        _view.mensagem("\nOpcao invalida!");
        _view.pausar();
        return;
    }

    const Produto* prodAtual = meusProdutos[escolha - 1];
    string idSelecionado = prodAtual->get_id();
    string nomeTemp  = prodAtual->get_nome();
    double precoTemp = prodAtual->get_preco();
    string catTemp   = prodAtual->get_categoria();
    string subTemp   = prodAtual->get_subcategoria();

    int opcaoEdit = 0;
    do {
        _view.limparTela();
        _view.exibirMenuEdicaoProduto(nomeTemp, precoTemp, catTemp, subTemp);
        opcaoEdit = _view.lerOpcaoInteira();

        if (opcaoEdit == 1) {
            nomeTemp = _view.solicitarNovoNome();
        } else if (opcaoEdit == 2) {
            precoTemp = _view.solicitarNovoPreco();
        } else if (opcaoEdit == 3) {
            auto [novaCat, novaSub] = _view.solicitarNovaCategoriaSubcategoria();
            catTemp = novaCat;
            subTemp = novaSub;
        } else if (opcaoEdit == 4) {
            _sistema.getProdutos().editarProduto(idSelecionado, nomeTemp, precoTemp, catTemp, subTemp);
            _view.mensagem("\nProduto atualizado com sucesso!");
            _view.pausar();
        } else if (opcaoEdit == 5) {
            _view.mensagem("\nEdicao cancelada. Nenhuma alteracao foi salva.");
            _view.pausar();
        }
    } while (opcaoEdit != 4 && opcaoEdit != 5);
}

void AppController::fluxoNotificacoes(Usuario* usuario) {
    _view.limparTela();
    _view.mensagem("\n=== CAIXA DE ENTRADA (NOTIFICACOES) ===");

    vector<Transacao*> propostas = _sistema.getTransacoes().buscarPropostasRecebidas(usuario);

    if (propostas.empty()) {
        _view.mensagem("Nao tens propostas pendentes no momento.");
    } else {
        for (Transacao* t : propostas) {
            if (Troca* troca = dynamic_cast<Troca*>(t)) {
                _view.exibirPropostaTroca(troca);
                int resp = _view.lerOpcaoInteira();

                if (resp == 1) {
                    _sistema.processarRespostaTroca(t, true);
                    _view.mensagem("\n[SUCESSO] Malou na catira! Os produtos foram removidos do mercado.");
                } else if (resp == 2) {
                    _sistema.processarRespostaTroca(t, false);
                    _view.mensagem("\n[AVISO] Proposta recusada.");
                }
            }
        }
    }

    _view.pausar();
}

// ============================================================
// Módulo do Comprador
// ============================================================

void AppController::fluxoComprador(Usuario* usuario) {
    bool podeTrocar  = usuario->temCapacidade(Capacidade::TROCAR);
    int  opcaoVoltar = podeTrocar ? 4 : 3;
    int  opcao       = 0;

    do {
        _view.limparTela();
        _view.exibirMenuComprador(usuario->getNome(), podeTrocar);
        opcao = _view.lerOpcaoInteira();

        if (opcao == 1) {
            fluxoBuscarEAdicionarCarrinho(usuario);
        } else if (opcao == 2) {
            fluxoCarrinho(usuario);
        } else if (podeTrocar && opcao == 3) {
            _view.mensagem("\nFuncionalidade de troca em desenvolvimento.");
            _view.pausar();
        }
    } while (opcao != opcaoVoltar);
}

void AppController::fluxoBuscarEAdicionarCarrinho(Usuario* usuario) {
    _view.limparTela();
    _view.exibirMenuBusca();
    int opcaoFiltro = _view.lerOpcaoInteira();

    string categoriaFiltro, subcategoriaFiltro, nomeFiltro;

    if (opcaoFiltro == 2) {
        _view.limparTela();
        auto [cat, sub] = _view.solicitarFiltroCategoria();
        categoriaFiltro    = cat;
        subcategoriaFiltro = sub;
    } else if (opcaoFiltro == 3) {
        _view.limparTela();
        nomeFiltro = _view.solicitarFiltroNome();
    }

    _view.limparTela();
    if (!categoriaFiltro.empty()) {
        cout << "Filtro ativo: " << categoriaFiltro;
        if (!subcategoriaFiltro.empty()) cout << " > " << subcategoriaFiltro;
        cout << "\n" << endl;
    } else if (!nomeFiltro.empty()) {
        cout << "Buscando por: \"" << nomeFiltro << "\"\n" << endl;
    }

    const auto& todos = _sistema.getProdutos().get_produtos();
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

    _view.exibirVitrine(produtosExibidos);

    if (produtosExibidos.empty()) {
        _view.mensagem("Nenhum produto correspondente foi encontrado.");
        _view.pausar();
        return;
    }

    int escolhaProduto = _view.solicitarEscolhaProdutoCarrinho(static_cast<int>(produtosExibidos.size()));

    if (escolhaProduto > 0 && escolhaProduto <= static_cast<int>(produtosExibidos.size())) {
        const Produto* p = produtosExibidos[escolhaProduto - 1];
        cout << "Produto selecionado: " << p->get_nome()
             << " | R$ " << p->get_preco()
             << " | Estoque: " << p->get_quantidade_estoque() << endl;

        int qtd = _view.solicitarQuantidade();

        if (qtd <= 0) {
            _view.mensagem("\nQuantidade invalida.");
        } else if (qtd > p->get_quantidade_estoque()) {
            cout << "\nEstoque insuficiente! Disponivel: " << p->get_quantidade_estoque() << endl;
        } else {
            ItemVendido novoItem(p->get_id(), p->get_nome(), p->get_preco(), qtd);
            _carrinho.adicionarItem(novoItem);
            _carrinho.salvarCarrinho(usuario->getLogin());
            cout << "\nSucesso! " << qtd << "x " << p->get_nome()
                 << " adicionado(s) ao carrinho e salvo." << endl;
        }
    } else if (escolhaProduto != 0) {
        _view.mensagem("\nErro: Numero invalido.");
    }

    _view.pausar();
}

// ============================================================
// Módulo do Carrinho
// ============================================================

void AppController::fluxoCarrinho(Usuario* usuario) {
    int opcao = 0;
    do {
        _view.limparTela();
        _view.exibirMenuCarrinho(_carrinho.get_itens(), _carrinho.get_total());
        opcao = _view.lerOpcaoInteira();

        if (opcao == 1) {
            fluxoFinalizarCompra(usuario);
        } else if (opcao == 2) {
            fluxoProporEscambo(usuario);
        } else if (opcao == 3) {
            _view.limparTela();
            _carrinho.esvaziar();
            _carrinho.salvarCarrinho(usuario->getLogin());
            _view.mensagem("\nSeu carrinho foi esvaziado.");
            _view.pausar();
        }
    } while (opcao != 4);
}

void AppController::fluxoFinalizarCompra(Usuario* usuario) {
    _view.limparTela();
    _view.mensagem("\n--- Finalizar Compra ---");

    if (_carrinho.get_itens().empty()) {
        _view.mensagem("Seu carrinho esta vazio. Nao ha o que finalizar.");
        _view.pausar();
        return;
    }

    for (const auto& item : _carrinho.get_itens()) {
        Produto* produtoOriginal = _sistema.getProdutos().buscarProdutoPorId(item.get_idProduto());
        if (produtoOriginal) {
            Usuario* vendedor = _sistema.getUsuarios().buscarUsuarioPorLogin(
                produtoOriginal->get_login_anunciante());
            if (vendedor) {
                _view.exibirItemFinalizacao(item.get_nomeProduto(), item.get_quantidade(),
                                            item.get_subtotal(), vendedor->getNome(),
                                            vendedor->getChavePix());
            } else {
                cout << "\nProduto: " << item.get_nomeProduto()
                     << " | Qtd: " << item.get_quantidade()
                     << " | Subtotal: R$ " << item.get_subtotal() << endl;
                _view.mensagem("Aviso: nao foi possivel localizar os dados de PIX do vendedor.");
            }

            if (!_sistema.finalizarCompra(usuario, produtoOriginal, item.get_quantidade())) {
                _view.mensagem("Aviso: estoque insuficiente no momento da finalizacao.");
            }
        } else {
            _view.mensagem("Aviso: este produto nao esta mais disponivel no sistema.");
        }
    }

    _carrinho.esvaziar();
    _carrinho.salvarCarrinho(usuario->getLogin());
    _view.mensagem("\nCompra finalizada com sucesso! O carrinho foi esvaziado.");
    _view.pausar();
}

void AppController::fluxoProporEscambo(Usuario* usuario) {
    _view.limparTela();
    _view.mensagem("\n--- Propor E-scambo a partir do Carrinho ---");

    const auto& itensTroca = _carrinho.get_itens();

    if (itensTroca.empty()) {
        _view.mensagem("Seu carrinho esta vazio. Adicione um produto antes de propor troca.");
        _view.pausar();
        return;
    }

    int escolhaAlvo = _view.solicitarItemAlvoTroca(itensTroca);

    if (escolhaAlvo <= 0 || escolhaAlvo > static_cast<int>(itensTroca.size())) {
        if (escolhaAlvo != 0) _view.mensagem("\nErro: Numero invalido.");
        _view.pausar();
        return;
    }

    const string& idAlvo = itensTroca[escolhaAlvo - 1].get_idProduto();
    Produto* produtoAlvo = _sistema.getProdutos().buscarProdutoPorId(idAlvo);

    if (!produtoAlvo) {
        _view.mensagem("\nAviso: este produto nao esta mais disponivel no sistema.");
        _view.pausar();
        return;
    }

    vector<Produto*> meusProdutos = _sistema.buscarProdutosDoUsuario(usuario->getLogin());

    if (meusProdutos.empty()) {
        _view.mensagem("\nVoce nao tem produtos ativos para oferecer em troca.");
        _view.pausar();
        return;
    }

    int escolhaOferta = _view.solicitarProdutoOfertaTroca(meusProdutos);

    if (escolhaOferta <= 0 || escolhaOferta > static_cast<int>(meusProdutos.size())) {
        if (escolhaOferta != 0) _view.mensagem("\nErro: Numero invalido.");
        _view.pausar();
        return;
    }

    Produto* produtoOferta = meusProdutos[escolhaOferta - 1];
    string mensagem = _view.solicitarMensagemTroca();

    Usuario* receptor      = _sistema.getUsuarios().buscarUsuarioPorLogin(
        produtoAlvo->get_login_anunciante());
    Anuncio* anuncioAlvo   = _sistema.obterAnuncio(produtoAlvo);
    Anuncio* anuncioOferta = _sistema.obterAnuncio(produtoOferta);

    if (receptor && _sistema.enviarPropostaTroca(
            usuario, receptor, anuncioAlvo, anuncioOferta, mensagem)) {
        _view.mensagem("\nProposta de E-scambo enviada com sucesso!");
        _carrinho.removerItem(static_cast<size_t>(escolhaAlvo - 1));
        _carrinho.salvarCarrinho(usuario->getLogin());
    } else {
        _view.mensagem("\nNao foi possivel enviar a proposta (verifique se os precos sao compativeis).");
    }

    _view.pausar();
}
