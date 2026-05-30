#include "../include/usuario.h"

using namespace std;

Usuario::Usuario(int id, string nome, string login, string senha)
    : _id(id), nome(nome), login(login), senha(senha) {}

int Usuario::getId() const {
    return _id;
}

bool Usuario::validarLogin(const string& loginIngressado, const string& senhaIngressado) const {
    return (this->login == loginIngressado && this->senha == senhaIngressado);
}

string Usuario::getNome() const { return nome; }
string Usuario::getLogin() const { return login; }
string Usuario::getSenha() const { return senha; }