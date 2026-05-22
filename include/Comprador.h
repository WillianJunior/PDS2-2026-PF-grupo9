#ifndef COMPRADOR_H
#define COMPRADOR_H
#include <string>
#include "usuario.h"

using namespace std;

class Produto;
class Favoritos;
class Carrinho;
class Pedido;

class  Comprador : public Usuario{
    //private:
    //somente os atributos de usuário
    public:
    Comprador(string nome, string email, string senha)
    : Usuario(nome, email, senha) {}
    void avaliarProduto(const Produto& produto){};
    void verCarrinho(const Carrinho& carrinho){};
    void consultarPedidos(const Pedido& pedido){};
    void comprar(const Produto& produto){}; //Acho que vai precisar ser um vetor de produtos, ou o carrinho todo
};

#endif
