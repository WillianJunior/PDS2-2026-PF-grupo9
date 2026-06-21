#include "../include/Usuario.hpp"
#include <utility>

using namespace std;

// REVISÃO: os parâmetros de string continuam recebidos por valor (idiom "pass
// by value, then move"), mas agora usamos std::move pra inicializar os
// atributos. Antes, cada string passada (nome, login, senha, chavePix) era
// copiada de novo aqui dentro; com move, "roubamos" o buffer que já tinha sido
// alocado pro parâmetro em vez de duplicar a alocação - zero cópias extra.
Usuario::Usuario(int id, string nome, string login, string senha, string chavePix)
    : _id(id), _nome(std::move(nome)), _login(std::move(login)), _senha(std::move(senha)), _chavePix(std::move(chavePix)) {}

int Usuario::getId() const {
    return _id;
}

bool Usuario::validarLogin(const string& loginIngressado, const string& senhaIngressado) const {
    // Nota de segurança: a senha está guardada e comparada em texto puro, o que
    // nunca seria aceitável num sistema real (faltaria hash + salt, ex: bcrypt).
    // Pro escopo deste projeto acadêmico mantivemos assim pra não quebrar o
    // formato já persistido em usuarios.txt, mas é uma limitação conhecida.
    return _login == loginIngressado && _senha == senhaIngressado;
}

const string& Usuario::getNome() const { return _nome; }
const string& Usuario::getLogin() const { return _login; }
const string& Usuario::getSenha() const { return _senha; }
const string& Usuario::getChavePix() const { return _chavePix; }

void Usuario::setChavePix(const string& pix) { _chavePix = pix; }
