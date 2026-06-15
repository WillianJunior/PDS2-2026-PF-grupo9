#ifndef COMPRA_HPP
#define COMPRA_HPP

#include "transacao.hpp"
#include "anuncio.hpp"

class Compra : public Transacao {
private:
    Anuncio* _anuncio;
    int _quantidade;
    double _valorTotal;

public:
    Compra(const std::string& id, Usuario* comprador, Usuario* vendedor, Anuncio* anuncio, int quantidade)
        : Transacao(id, comprador, vendedor), 
          _anuncio(anuncio), 
          _quantidade(quantidade) {
            if (_anuncio && _anuncio->get_produto()) {
                _valorTotal = _anuncio->get_produto()->get_preco() * _quantidade;
            } else {
                _valorTotal = 0.0;
            }
        }

    Anuncio* get_anuncio() const { return _anuncio; }
    int get_quantidade() const { return _quantidade; }
    double get_valor_total() const { return _valorTotal; }

    // Implementação das regras de negócio específicas de uma compra
    bool validar_transacao() const override {
        if (!_anuncio || !_anuncio->get_status()) {
            return false;
        }
        return _anuncio->get_quantidade() >= _quantidade;
    }

    void executar_transacao() override {
        if (validar_transacao()) {
            _status = StatusTransacao::CONCLUIDA;
            // A lógica de decremento de estoque deve ser gerenciada pelo Gerenciador correspondente
        } else {
            _status = StatusTransacao::REJEITADA;
        }
    }
};

#endif