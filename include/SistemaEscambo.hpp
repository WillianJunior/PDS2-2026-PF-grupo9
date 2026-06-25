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

// Camada de Model do sistema: agrega os três Gerenciadores (que funcionam
// como Repositories de Usuario/Produto/Transacao) e concentra as regras de
// negócio que cruzam mais de um deles - finalizar compra, propor troca,
// processar resposta de troca. O AppController (Controller) conversa com
// esta classe; a TerminalView (View) nunca a vê. Alguns métodos públicos
// (finalizarCompra, processarRespostaTroca) atuam como fachada por
// esconderem a orquestração entre gerenciadores, mas a classe não é uma
// Facade no sentido estrito do GoF - ela é dona do estado e expõe os
// repositórios via getters para consultas pontuais do Controller.
class SistemaEscambo {
private:
    // Ordem de declaração importa: _usuarios e _produtos são inicializados
    // antes de _transacoes (que precisa deles prontos pra recarregar o
    // histórico no construtor) - C++ inicializa membros na ordem em que
    // aparecem aqui, não na ordem da lista de inicialização.
    GerenciadorUsuarios _usuarios;
    GerenciadorProdutos _produtos;
    GerenciadorTransacoes _transacoes;

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

    // Cria e registra uma Compra de verdade (ver Compra.hpp) pro checkout PIX,
    // debitando o estoque real do Produto e gravando a transação no histórico.
    // Retorna false sem registrar nada se faltar comprador/produto/vendedor.
    bool finalizarCompra(Usuario* comprador, Produto* produto, int quantidade);
};

#endif
