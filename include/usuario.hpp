#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

using namespace std;

class Usuario {
private:
    int _id;
    string _nome;
    string _login;
    string _senha;
    string _chavePix;
public:
    Usuario(int id, string nome, string login, string senha, string chavePix = "");

    virtual ~Usuario() = default;

    int getId() const;
    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;
    string getNome() const;
    string getLogin() const;
    string getSenha() const;
    string getChavePix() const;
    void setChavePix(const string& pix);
};

#endif