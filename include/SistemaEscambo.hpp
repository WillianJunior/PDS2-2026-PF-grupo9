#ifndef SISTEMA_ESCAMBO_HPP
#define SISTEMA_ESCAMBO_HPP

#include <vector>
#include <string>
#include <memory>
#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"
#include "GerenciadorTransacoes.hpp"
#include "Produto.hpp"
#include "Anuncio.hpp"

class SistemaEscambo {
private:
    GerenciadorUsuarios _usuarios;
    GerenciadorProdutos _produtos;
    GerenciadorTransacoes _transacoes;
    std::vector<std::unique_ptr<Anuncio>> _anuncios;

public:
    SistemaEscambo();

    GerenciadorUsuarios& getUsuarios();
    GerenciadorProdutos& getProdutos();
    GerenciadorTransacoes& getTransacoes();

    std::vector<Produto*> buscarProdutosDoUsuario(const std::string& login);

    // Garante um Anuncio estavel (ponteiro valido durante toda a execucao) para um Produto,
    // criando-o sob demanda. Necessario porque Troca/Compra trabalham com Anuncio*, mas a
    // vitrine so conhece Produto*.
    Anuncio* obterAnuncio(Produto* produto);

    bool enviarPropostaTroca(Usuario* proponente, Usuario* receptor, Anuncio* alvo, Anuncio* ofertado, const std::string& mensagem);

    void processarRespostaTroca(Transacao* transacao, bool aceitar);
};

#endif