#include "../include/Anuncio.hpp"

// Lista de inicialização: _estaAtivo sempre começa true, já que um Anuncio só
// é criado quando alguém efetivamente quer negociar aquele Produto agora.
Anuncio::Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade)
    : _idAnuncio(id),
      _produto(produto),
      _vendedor(vendedor),
      _quantidade(quantidade),
      _estaAtivo(true) {}

void Anuncio::set_status(bool ativo) {
    _estaAtivo = ativo;
}

bool Anuncio::get_status() const {
    return _estaAtivo;
}

Produto* Anuncio::get_produto() const {
    return _produto;
}

Usuario* Anuncio::get_vendedor() const {
    return _vendedor;
}

int Anuncio::get_quantidade() const {
    return _quantidade;
}

// Validação simples: um Anuncio sem quantidade positiva não tem como ser
// negociado. É uma guarda barata que a Troca/Compra checam antes de validar
// o resto (preços, estoque), pra falhar rápido em caso óbvio.
bool Anuncio::validar_anuncio() const {
    return _quantidade > 0;
}
