#ifndef USUARIO_H
#define USUARIO_H

#include <string>

using namespace std;

class Usuario {
private:
    int _id; // <-- NOVO: O identificador único do usuário
    string nome;
    string login;
    string senha;
public:
    Usuario(int id, string nome, string email, string senha);

    virtual ~Usuario() = default;

    int getId() const; // <-- NOVO
    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;
    string getNome() const;
    string getLogin() const;
    string getSenha() const;
};

#endif