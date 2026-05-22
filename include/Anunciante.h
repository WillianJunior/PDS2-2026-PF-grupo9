#ifndef ANUNCIANTE_H
#define ANUNCIANTE_H

#include <string>
#include "usuario.h"

using namespace std;

class Produto;
class Estoque;
class Comprador;

class Anunciante : public Usuario {

public:

    Anunciante(string nome, string email, string senha)
        : Usuario(nome, email, senha) {}

    Anunciante(const Usuario& usuario)
        : Usuario(usuario) {}

    void anunciarProduto(const Produto& produto){
        (void)produto;
    }

    void venderProduto(const Produto& produto){
        (void)produto;
    }

    void gerenciarEstoque(Estoque& estoque){
        (void)estoque;
    }
};

#endif