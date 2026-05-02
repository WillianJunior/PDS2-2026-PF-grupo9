#include "../include/Usuario.h"

using namespace std;

Usuario::Usuario(string nome, string login, string senha)
    : nome(nome), login(login), senha(senha) {}

bool Usuario::validarLogin(const string& loginIngressado, const string& senhaIngressado) const {
    return (this->login == loginIngressado && this->senha == senhaIngressado);
}

string Usuario::getNome() const {
    return nome;
}
string Usuario::getLogin() const {
    return login;
}
string Usuario::getSenha() const {
    return senha;
}
