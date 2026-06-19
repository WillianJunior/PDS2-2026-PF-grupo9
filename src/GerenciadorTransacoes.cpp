#include "../include/GerenciadorTransacoes.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

GerenciadorTransacoes::GerenciadorTransacoes() {
    // Inicialização futura se necessário
}

// O destrutor manual com 'delete' foi REMOVIDO. 
// O std::vector<std::unique_ptr<Transacao>> gerencia a memória sozinho.
GerenciadorTransacoes::~GerenciadorTransacoes() = default;

void GerenciadorTransacoes::adicionarTransacao(std::unique_ptr<Transacao> transacao) {
    if (!transacao) {
        throw std::invalid_argument("Erro: Tentativa de adicionar transacao nula.");
    }
    
    // Armazena e transfere a posse do ponteiro único
    _transacoes.push_back(std::move(transacao));
    salvarTransacaoNoArquivo(_transacoes.back().get());
}

void GerenciadorTransacoes::salvarTransacaoNoArquivo(Transacao* t) {
    if (!t) return;
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::app);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro crítico: Nao foi possivel abrir o arquivo de transacoes para escrita.");
    }

    arquivo << t->get_id_transacao() << ","
            << t->get_usuario_proponente()->getLogin() << ","
            << t->get_usuario_receptor()->getLogin() << ","
            << static_cast<int>(t->get_status()) << ","
            << t->get_mensagem() << ",";

    if (Troca* troca = dynamic_cast<Troca*>(t)) {
        arquivo << "T," 
                << troca->get_anuncio_alvo()->get_produto()->get_id() << ","
                << troca->get_anuncio_ofertado()->get_produto()->get_id() << "\n";
    } 
    else if (Compra* compra = dynamic_cast<Compra*>(t)) {
        arquivo << "C," 
                << compra->get_anuncio()->get_produto()->get_id() << ","
                << compra->get_quantidade() << "\n";
    }
}

void GerenciadorTransacoes::atualizarArquivoCompleto() {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::trunc);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro crítico: Nao foi possivel atualizar o arquivo de transacoes.");
    }

    for (const auto& t : _transacoes) {
        arquivo << t->get_id_transacao() << ","
                << t->get_usuario_proponente()->getLogin() << ","
                << t->get_usuario_receptor()->getLogin() << ","
                << static_cast<int>(t->get_status()) << ","
                << t->get_mensagem() << ",";

        if (Troca* troca = dynamic_cast<Troca*>(t.get())) {
            arquivo << "T," 
                    << troca->get_anuncio_alvo()->get_produto()->get_id() << ","
                    << troca->get_anuncio_ofertado()->get_produto()->get_id() << "\n";
        } 
        else if (Compra* compra = dynamic_cast<Compra*>(t.get())) {
            arquivo << "C," 
                    << compra->get_anuncio()->get_produto()->get_id() << ","
                    << compra->get_quantidade() << "\n";
        }
    }
}

std::vector<Transacao*> GerenciadorTransacoes::buscarPropostasRecebidas(Usuario* receptor) {
    if (!receptor) return {};
    std::vector<Transacao*> propostas;
    for (const auto& t : _transacoes) {
        if (t->get_usuario_receptor()->getLogin() == receptor->getLogin() && 
            t->get_status() == StatusTransacao::PENDENTE) {
            propostas.push_back(t.get());
        }
    }
    return propostas;
}

Anuncio* GerenciadorTransacoes::obterOuCriarAnuncio(Produto* produto, GerenciadorUsuarios& gerUsers) {
    if (!produto) return nullptr;

    for (auto& anuncio : _anuncios) {
        if (anuncio->get_produto() == produto) {
            return anuncio.get();
        }
    }

    Usuario* vendedor = gerUsers.buscarUsuarioPorLogin(produto->get_login_anunciante());
    auto novoAnuncio = std::make_unique<Anuncio>(static_cast<int>(_anuncios.size()) + 1, produto, vendedor, 1);
    _anuncios.push_back(std::move(novoAnuncio));
    return _anuncios.back().get();
}

void GerenciadorTransacoes::carregarTransacoes(GerenciadorUsuarios& gerUsers, GerenciadorProdutos& gerProds) {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) return;

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string idTransacao, loginProponente, loginReceptor, statusStr, mensagem, tipo;

        std::getline(ss, idTransacao, ',');
        std::getline(ss, loginProponente, ',');
        std::getline(ss, loginReceptor, ',');
        std::getline(ss, statusStr, ',');
        std::getline(ss, mensagem, ',');
        std::getline(ss, tipo, ',');

        if (idTransacao.empty() || tipo.empty()) continue;

        Usuario* proponente = gerUsers.buscarUsuarioPorLogin(loginProponente);
        Usuario* receptor = gerUsers.buscarUsuarioPorLogin(loginReceptor);
        if (!proponente || !receptor) continue;

        std::unique_ptr<Transacao> transacao;

        if (tipo == "T") {
            std::string idAlvo, idOfertado;
            std::getline(ss, idAlvo, ',');
            std::getline(ss, idOfertado, ',');

            Produto* produtoAlvo = gerProds.buscarProdutoPorId(idAlvo);
            Produto* produtoOfertado = gerProds.buscarProdutoPorId(idOfertado);
            if (!produtoAlvo || !produtoOfertado) continue;

            Anuncio* anuncioAlvo = obterOuCriarAnuncio(produtoAlvo, gerUsers);
            Anuncio* anuncioOfertado = obterOuCriarAnuncio(produtoOfertado, gerUsers);

            transacao = std::make_unique<Troca>(idTransacao, proponente, receptor, anuncioAlvo, anuncioOfertado);
        } else if (tipo == "C") {
            std::string idProduto, qtdStr;
            std::getline(ss, idProduto, ',');
            std::getline(ss, qtdStr, ',');

            Produto* produto = gerProds.buscarProdutoPorId(idProduto);
            if (!produto) continue;
            int quantidade = qtdStr.empty() ? 1 : std::stoi(qtdStr);

            Anuncio* anuncio = obterOuCriarAnuncio(produto, gerUsers);

            transacao = std::make_unique<Compra>(idTransacao, proponente, receptor, anuncio, quantidade);
        } else {
            continue;
        }

        transacao->set_status(static_cast<StatusTransacao>(std::stoi(statusStr)));
        transacao->set_mensagem(mensagem);
        _transacoes.push_back(std::move(transacao));
    }
    arquivo.close();
}

std::vector<Transacao*> GerenciadorTransacoes::buscarPropostasEnviadas(Usuario* proponente) {
    if (!proponente) return {};
    std::vector<Transacao*> propostas;
    for (const auto& t : _transacoes) {
        if (t->get_usuario_proponente()->getLogin() == proponente->getLogin()) {
            propostas.push_back(t.get());
        }
    }
    return propostas;
}