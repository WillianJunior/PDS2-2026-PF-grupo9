#include "../include/GerenciadorTransacoes.hpp"
#include <fstream>
#include <sstream>

GerenciadorTransacoes::GerenciadorTransacoes() {
    
    // No futuro chamaremos carregarTransacoes() aqui
}

GerenciadorTransacoes::~GerenciadorTransacoes() {
    // Como estamos guardando ponteiros, precisamos de liberar a memória ao fechar o programa
    for (Transacao* t : _transacoes) {
        delete t;
    }
    _transacoes.clear();
}

void GerenciadorTransacoes::adicionarTransacao(Transacao* transacao) {
    _transacoes.push_back(transacao);
    salvarTransacaoNoArquivo(transacao);
}

void GerenciadorTransacoes::salvarTransacaoNoArquivo(Transacao* t) {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::app);
    if (!arquivo.is_open()) return;

    // salva os dados comuns a qualquer transação
    arquivo << t->get_id_transacao() << ","
            << t->get_usuario_proponente()->getLogin() << ","
            << t->get_usuario_receptor()->getLogin() << ","
            << static_cast<int>(t->get_status()) << ","
            << t->get_mensagem() << ",";

    // dynamic_cast diferencia se é troca ou compra na hora de salvar
    if (Troca* troca = dynamic_cast<Troca*>(t)) {
        arquivo << "T," // Identificador de Troca
                << troca->get_anuncio_alvo()->get_produto()->get_id() << ","
                << troca->get_anuncio_ofertado()->get_produto()->get_id() << "\n";
    } 
    else if (Compra* compra = dynamic_cast<Compra*>(t)) {
        arquivo << "C," // Identificador de Compra
                << compra->get_anuncio()->get_produto()->get_id() << ","
                << compra->get_quantidade() << "\n";
    }
    arquivo.close();
}

void GerenciadorTransacoes::atualizarArquivoCompleto() {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::trunc); // Apaga e reescreve tudo
    if (!arquivo.is_open()) return;

    for (Transacao* t : _transacoes) {
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
    arquivo.close();
}

std::vector<Transacao*> GerenciadorTransacoes::buscarPropostasRecebidas(Usuario* receptor) {
    std::vector<Transacao*> propostas;
    for (Transacao* t : _transacoes) {
        // Se o login do receptor bater certo e a transação estiver pendente
        if (t->get_usuario_receptor()->getLogin() == receptor->getLogin() && 
            t->get_status() == StatusTransacao::PENDENTE) {
            propostas.push_back(t);
        }
    }
    return propostas;
}

std::vector<Transacao*> GerenciadorTransacoes::buscarPropostasEnviadas(Usuario* proponente) {
    std::vector<Transacao*> propostas;
    for (Transacao* t : _transacoes) {
        if (t->get_usuario_proponente()->getLogin() == proponente->getLogin()) {
            propostas.push_back(t);
        }
    }
    return propostas;
}