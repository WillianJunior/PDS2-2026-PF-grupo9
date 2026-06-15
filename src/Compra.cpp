#include "../include/Compra.hpp"

Compra::Compra(const std::string& id, Usuario* comprador, Usuario* vendedor, Anuncio* anuncio, int quantidade)
    : Transacao(id, comprador, vendedor), 
      _anuncio(anuncio), 
      _quantidade(quantidade) {
    
    if (_anuncio && _anuncio->get_produto()) {
        _valorTotal = _anuncio->get_produto()->get_preco() * _quantidade;
    } else {
        _valorTotal = 0.0;
    }
}

Anuncio* Compra::get_anuncio() const { return _anuncio; }

int Compra::get_quantidade() const { return _quantidade; }

double Compra::get_valor_total() const { return _valorTotal; }

bool Compra::validar_transacao() const {
    if (!_anuncio || !_anuncio->get_status()) {
        return false;
    }
    return _anuncio->get_quantidade() >= _quantidade;
}

void Compra::executar_transacao() {
    if (validar_transacao()) {
        _status = StatusTransacao::CONCLUIDA;
        // O decremento físico de estoque e a criação do Pedido
        // devem ser chamados pelo Gerenciador correspondente após esta etapa.
    } else {
        _status = StatusTransacao::REJEITADA;
    }
}