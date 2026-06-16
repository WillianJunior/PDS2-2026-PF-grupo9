#include "Anuncio.hpp"

// Implementação do construtor utilizando a lista de inicialização
Anuncio::Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade)
    : _idAnuncio(id),
      _produto(produto),
      _vendedor(vendedor),
      _quantidade(quantidade),
      _estaAtivo(true) {}

// Implementação dos métodos
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

bool Anuncio::validar_anuncio() const {
    return _quantidade > 0;
}