#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "GerenciadorUsuarios.h"
#include "GerenciadorProdutos.h"
#include "Carrinho.h" // <-- NOVO

using namespace std;

class TerminalUI {
private:
    GerenciadorUsuarios gerenciador;
    GerenciadorProdutos gerenciadorProdutos;
    Carrinho carrinhoCompras; // <-- O Carrinho da sessão!

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