#ifndef USUARIO_H
#define USUARIO_H

#include <string>

using namespace std;

class Usuario {
private:
    string nome;
    string login;
    string senha;
public:
    Usuario(string nome, string email, string senha);

    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;
    string getNome() const;
    string getLogin() const;
    string getSenha() const;
};

#endif