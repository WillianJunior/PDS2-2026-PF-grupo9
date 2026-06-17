#ifndef GERENCIADOR_TRANSACOES_HPP
#define GERENCIADOR_TRANSACOES_HPP

#include <vector>
#include <string>
#include "Transacao.hpp"
#include "Compra.hpp"
#include "Troca.hpp"
#include "Usuario.hpp"

class GerenciadorTransacoes {
private:
    std::vector<Transacao*> _transacoes;
    const std::string NOME_ARQUIVO = "transacoes.txt"; 

public:
    GerenciadorTransacoes();
    ~GerenciadorTransacoes();

    void adicionarTransacao(Transacao* transacao);
    std::vector<Transacao*> buscarPropostasRecebidas(Usuario* receptor);
    std::vector<Transacao*> buscarPropostasEnviadas(Usuario* proponente);

    // === MÉTODOS DE PERSISTÊNCIA ===
    void salvarTransacaoNoArquivo(Transacao* t);
    void atualizarArquivoCompleto();
    // void carregarTransacoes(...); // Faremos este no próximo passo!
};

#endif