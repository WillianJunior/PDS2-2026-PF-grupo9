#ifndef COMPRA_HPP
#define COMPRA_HPP

#include "Transacao.hpp"
#include "Anuncio.hpp"

// Implementação concreta de Transacao pra uma compra direta (via PIX).
// Integrada na ponta da UI via SistemaEscambo::finalizarCompra, que cria a
// Compra, valida contra o estoque real do Produto e registra o resultado
// (CONCLUIDA ou REJEITADA) em GerenciadorTransacoes.
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
