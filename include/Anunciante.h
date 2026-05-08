#ifndef ANUNCIANTE_H
#define ANUNCIANTE_H
#include <string>
#include "usuario.h"

using namespace std;

class Produto;
class Estoque;
class Comprador;

class Anunciante : public Usuario{
    //private:
    //somente os atributos de usuário
    public:
    Anunciante(string nome, string email, string senha);
    Anunciante(const Usuario& usuario);
    void anunciarProduto(const Produto& produto);
    void venderProduto(const Produto& produto);
    void gerenciarEstoque(Estoque& estoque);

};

#endif
