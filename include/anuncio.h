#ifndef ANUNCIO_H
#define ANUNCIO_H

#include <string>
#include "produto.h" 
#include "usuario.h" 

class Anuncio {
private:
    int _idAnuncio;
    Produto* _produto;      // Produto associado 
    Usuario* _vendedor;     // O Anunciante
    int _quantidade;        // Quantidade no estoque 
    bool _estaAtivo;        // Status de visibilidade do anúncio 

public:
    Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade);

    void set_status(bool ativo);
    bool get_status() const;
    
    // Getters 
    Produto* get_produto() const;
    Usuario* get_vendedor() const;
    int get_quantidade() const;

    bool validar_anuncio() const; // Válido se preço/estoque > 0  
};

#endif