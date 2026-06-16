#ifndef ANUNCIO_HPP
#define ANUNCIO_HPP

#include <string>
#include "Produto.hpp"
#include "Usuario.hpp"

class Anuncio {
private:
    int _idAnuncio;
    Produto* _produto;
    Usuario* _vendedor;
    int _quantidade;
    bool _estaAtivo;

public:
    // Apenas as declarações terminando em ponto e vírgula!
    Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade);

    void set_status(bool ativo);
    bool get_status() const;
    Produto* get_produto() const;
    Usuario* get_vendedor() const;
    int get_quantidade() const;
    bool validar_anuncio() const;
};

#endif