#ifndef ANUNCIO_H
#define ANUNCIO_H

#include <string>
#include "produto.h"
#include "usuario.h"

class Anuncio {
private:
    int _idAnuncio;
    Produto* _produto;
    Usuario* _vendedor;
    int _quantidade;
    bool _estaAtivo;

public:

    Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade)
        : _idAnuncio(id),
          _produto(produto),
          _vendedor(vendedor),
          _quantidade(quantidade),
          _estaAtivo(true) {}

    void set_status(bool ativo){
        _estaAtivo = ativo;
    }

    bool get_status() const{
        return _estaAtivo;
    }

    Produto* get_produto() const{
        return _produto;
    }

    Usuario* get_vendedor() const{
        return _vendedor;
    }

    int get_quantidade() const{
        return _quantidade;
    }

    bool validar_anuncio() const{
        return _quantidade > 0;
    }
};

#endif