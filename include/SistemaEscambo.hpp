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

// Fachada (Facade) do sistema: é a ÚNICA classe que o TerminalUI (View)
// conhece pra fazer qualquer coisa que envolva regra de negócio. Por trás
// dela ficam escondidos os três Gerenciadores e toda a lógica de orquestrar
// Compra/Troca - a View nunca manipula GerenciadorProdutos ou
// GerenciadorTransacoes direto, sempre passa por aqui. Isso é o que mantém
// o "C" (Controller) da nossa adaptação de MVC isolado do "V" (View).
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
};

#endif
