#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "GerenciadorUsuarios.h"
#include "GerenciadorProdutos.h" 

using namespace std;

class TerminalUI {
private:
    GerenciadorUsuarios gerenciador;
    GerenciadorProdutos gerenciadorProdutos; 
    void limparTela();
    
    // Nossos novos menus específicos!
    void menuAnunciante(Usuario* usuario);
    void menuComprador(Usuario* usuario);

public:
    void iniciar();
    void menuLogin();
    void menuCadastro();
    void menuEscolhaPerfil(Usuario* usuario);
};

#endif