#include "../include/Usuario.hpp"

using namespace std;

Usuario::Usuario(int id, string nome, string login, string senha, string chavePix)
    : _id(id), _nome(nome), _login(login), _senha(senha), _chavePix(chavePix) {}

int Usuario::getId() const {
    return _id;
}

bool Usuario::validarLogin(const string& loginIngressado, const string& senhaIngressado) const {
    return (this->_login == loginIngressado && this->_senha == senhaIngressado);
}

string Usuario::getNome() const { return _nome; }
string Usuario::getLogin() const { return _login; }
string Usuario::getSenha() const { return _senha; }
string Usuario::getChavePix() const { return _chavePix; }

void Usuario::setChavePix(const std::string& pix) { _chavePix = pix; }