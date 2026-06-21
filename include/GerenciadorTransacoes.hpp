#ifndef GERENCIADOR_TRANSACOES_HPP
#define GERENCIADOR_TRANSACOES_HPP

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "Transacao.hpp"
#include "Usuario.hpp"
#include "Troca.hpp"
#include "Compra.hpp"
#include "Anuncio.hpp"
#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"

// Guarda o histórico de Trocas e Compras (polimorficamente, via Transacao*)
// e os Anuncios criados sob demanda pra cada Produto negociado. Diferente de
// GerenciadorUsuarios/GerenciadorProdutos, aqui std::vector é seguro mesmo
// guardando ponteiros usados depois: o vetor guarda unique_ptr<Transacao>,
// então o que pode "mover na memória" ao realocar é o ponteiro em si, nunca
// o objeto Transacao/Anuncio apontado (esse vive solto no heap). Por isso
// não precisou da mesma troca pra deque que fizemos nos outros Gerenciadores.
class GerenciadorTransacoes {
private:
    std::vector<std::unique_ptr<Transacao>> _transacoes;
    std::vector<std::unique_ptr<Anuncio>> _anuncios;
    const std::string NOME_ARQUIVO = "transacoes.txt";

    // REVISÃO: extraída pra eliminar a duplicação que existia entre
    // salvarTransacaoNoArquivo e atualizarArquivoCompleto (as duas escreviam,
    // cada uma com seu próprio dynamic_cast, exatamente a mesma linha de
    // formato pra uma Transacao). Agora as duas só chamam este helper.
    void escreverLinhaTransacao(std::ofstream& arquivo, Transacao* t) const;

    void salvarTransacaoNoArquivo(Transacao* t);
    void atualizarArquivoCompleto();

public:
    GerenciadorTransacoes();
    ~GerenciadorTransacoes();

    void adicionarTransacao(std::unique_ptr<Transacao> transacao);
    void carregarTransacoes(GerenciadorUsuarios& gerUsers, GerenciadorProdutos& gerProds);

    // REVISÃO: promovido de private pra public. SistemaEscambo tinha o seu
    // próprio std::vector<unique_ptr<Anuncio>> com uma cópia praticamente
    // idêntica desta função (mesma lógica de "achar ou criar"). Isso criava
    // DOIS registros de Anuncio paralelos pro mesmo Produto: um quando a
    // proposta de troca era feita pela primeira vez (SistemaEscambo), outro
    // quando transacoes.txt era relido no início do programa (aqui). Agora
    // existe um único registro de Anuncios (o deste GerenciadorTransacoes) e
    // SistemaEscambo::obterAnuncio só delega pra aqui - eliminamos a
    // duplicação E o risco de inconsistência entre os dois registros.
    Anuncio* obterOuCriarAnuncio(Produto* produto, GerenciadorUsuarios& gerUsers);

    std::vector<Transacao*> buscarPropostasRecebidas(Usuario* receptor);
    std::vector<Transacao*> buscarPropostasEnviadas(Usuario* proponente);
};

#endif
