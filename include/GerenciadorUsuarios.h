#ifndef GERENCIADOR_USUARIOS_HPP
#define GERENCIADOR_USUARIOS_HPP

#include <vector>
#include <string>
#include "usuario.hpp"

using namespace std;

class GerenciadorUsuarios {
private:
    vector<Usuario> usuarios;
    const string NOME_ARQUIVO = "usuarios.txt";

    void carregarUsuarios();
    void salvarUsuario(const Usuario& usuario);

public:
    GerenciadorUsuarios();
    void registrarUsuario(const string& nome, const string& login, const string& senha);
    Usuario* autenticar(const string& loginIngressado, const string&senhaIngressado);
};

#endif
