#include "../include/Compra.hpp"
#include <utility>

// REVISÃO: id por valor + std::move, igual no resto da hierarquia (Transacao/Troca).
Compra::Compra(std::string id, Usuario* comprador, Usuario* vendedor, Anuncio* anuncio, int quantidade)
    : Transacao(std::move(id), comprador, vendedor),
      _anuncio(anuncio),
      _quantidade(quantidade) {

    // Valor total calculado uma única vez na criação (e não a cada get), já
    // que preço e quantidade não mudam depois que a Compra existe.
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
    // Garante que não estamos vendendo mais do que o anúncio diz ter disponível.
    return _anuncio->get_quantidade() >= _quantidade;
}

void Compra::executar_transacao() {
    if (validar_transacao()) {
        _status = StatusTransacao::CONCLUIDA;
        // De propósito esta classe só marca o status como concluído - ela não
        // sabe (e não deveria saber) como persistir em arquivo nem como
        // decrementar o estoque do Produto de verdade. Essa responsabilidade
        // é do Gerenciador/Fachada que efetivamente disparar a compra, pra
        // não misturar "validar a regra de negócio" com "efeito colateral
        // no resto do sistema" dentro da própria Transacao.
    } else {
        _status = StatusTransacao::REJEITADA;
    }
}
