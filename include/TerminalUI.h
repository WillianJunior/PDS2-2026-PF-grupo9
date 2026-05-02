#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "GerenciadorUsuarios.h"

using namespace std;

class TerminalUI {
private:
    GerenciadorUsuarios gerenciador;
    void limparTela();

public:
    void iniciar();
    void menuLogin();
    void menuCadastro();
    void menuEscolhaPerfil(Usuario* usuario);
};

#endif