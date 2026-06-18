#ifndef GERENCIADOR_TRANSACOES_HPP
#define GERENCIADOR_TRANSACOES_HPP

#include <vector>
#include <string>
#include <memory>
#include "Transacao.hpp"
#include "Usuario.hpp"
#include "Troca.hpp"
#include "Compra.hpp"

class GerenciadorTransacoes {
private:
    std::vector<std::unique_ptr<Transacao>> _transacoes;
    const std::string NOME_ARQUIVO = "transacoes.txt";

    void salvarTransacaoNoArquivo(Transacao* t);
    void atualizarArquivoCompleto();

public:
    GerenciadorTransacoes();
    ~GerenciadorTransacoes(); 

    void adicionarTransacao(std::unique_ptr<Transacao> transacao);

    std::vector<Transacao*> buscarPropostasRecebidas(Usuario* receptor);
    std::vector<Transacao*> buscarPropostasEnviadas(Usuario* proponente);
};

#endif