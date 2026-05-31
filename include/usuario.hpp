#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

using namespace std;

class Usuario {
private:
    int _id;
    string nome;
    string login;
    string senha;
public:
    Usuario(int id, string nome, string email, string senha);

    virtual ~Usuario() = default;

    int getId() const;
    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;
    string getNome() const;
    string getLogin() const;
    string getSenha() const;
};

#endif