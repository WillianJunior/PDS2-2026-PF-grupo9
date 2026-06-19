#ifndef GERENCIADOR_TRANSACOES_HPP
#define GERENCIADOR_TRANSACOES_HPP

#include <vector>
#include <string>
#include <memory>
#include "Transacao.hpp"
#include "Usuario.hpp"
#include "Troca.hpp"
#include "Compra.hpp"
#include "Anuncio.hpp"
#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"

class GerenciadorTransacoes {
private:
    std::vector<std::unique_ptr<Transacao>> _transacoes;
    std::vector<std::unique_ptr<Anuncio>> _anuncios;
    const std::string NOME_ARQUIVO = "transacoes.txt";

    void salvarTransacaoNoArquivo(Transacao* t);
    void atualizarArquivoCompleto();
    Anuncio* obterOuCriarAnuncio(Produto* produto, GerenciadorUsuarios& gerUsers);

public:
    GerenciadorTransacoes();
    ~GerenciadorTransacoes();

    void adicionarTransacao(std::unique_ptr<Transacao> transacao);
    void carregarTransacoes(GerenciadorUsuarios& gerUsers, GerenciadorProdutos& gerProds);

    std::vector<Transacao*> buscarPropostasRecebidas(Usuario* receptor);
    std::vector<Transacao*> buscarPropostasEnviadas(Usuario* proponente);
};

#endif