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