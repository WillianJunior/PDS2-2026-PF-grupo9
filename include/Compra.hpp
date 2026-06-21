#ifndef COMPRA_HPP
#define COMPRA_HPP

#include "Transacao.hpp"
#include "Anuncio.hpp"

// Implementação concreta de Transacao pra uma compra direta (via PIX).
//
// NOTA DE ARQUITETURA (pra quem for revisar/apresentar): esta classe está
// completa e testada isoladamente (ver TesteTransacao.cpp), mas o fluxo de
// "Finalizar Compra" do TerminalUI hoje NÃO passa por aqui - ele chama
// GerenciadorProdutos::venderProduto() direto, sem criar um objeto Compra
// nem registrar a transação em GerenciadorTransacoes. Ou seja, toda a parte
// de validação polimórfica e histórico de compras que esta classe permitiria
// ainda não está integrada na ponta da UI. Ficou assim de propósito nesta
// entrega pra não arriscar o fluxo de checkout às vésperas do prazo - é o
// próximo passo natural de evolução do projeto.
class Compra : public Transacao {
private:
    Anuncio* _anuncio;
    int _quantidade;
    double _valorTotal;

public:
    Compra(std::string id, Usuario* comprador, Usuario* vendedor, Anuncio* anuncio, int quantidade);

    Anuncio* get_anuncio() const;
    int get_quantidade() const;
    double get_valor_total() const;

    bool validar_transacao() const override;
    void executar_transacao() override;
};

#endif
