#ifndef TERMINAL_UI_HPP
#define TERMINAL_UI_HPP

#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"
#include "Carrinho.hpp" 

using namespace std;

class TerminalUI {
private:
    GerenciadorUsuarios gerenciador;
    GerenciadorProdutos gerenciadorProdutos;
    Carrinho carrinhoCompras;

    void limparTela();
    
    void menuAnunciante(Usuario* usuario);
    void menuComprador(Usuario* usuario);

public:
    void iniciar();
    void menuLogin();
    void menuCadastro();
    void menuEscolhaPerfil(Usuario* usuario);
};

#endif