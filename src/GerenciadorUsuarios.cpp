#include "../include/GerenciadorUsuarios.h"
#include <fstream>
#include <sstream>

using namespace std;

GerenciadorUsuarios::GerenciadorUsuarios() {
    carregarUsuarios();
}

void GerenciadorUsuarios::carregarUsuarios() {
    ifstream arquivo(NOME_ARQUIVO);
    if(!arquivo.is_open()){
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string nome, login, senha;

        getline(ss, nome, ',');
        getline(ss, login, ',');
        getline(ss, senha, ',');

        if (!nome.empty()){
            usuarios.emplace_back(nome, login, senha);
        }
    }
    arquivo.close();
}

void GerenciadorUsuarios::salvarUsuario(const Usuario& usuario) {
    ofstream arquivo(NOME_ARQUIVO, ios::app);
    if (arquivo.is_open()) {
        arquivo << usuario.getNome() << ","
                << usuario.getLogin() << ","
                << usuario.getSenha() << "\n";
        arquivo.close();
    }
}

void GerenciadorUsuarios::registrarUsuario(const string& nome, const string& login, const string& senha) {
    Usuario novoUsuario(nome, login, senha);
    usuarios.push_back(novoUsuario);
    salvarUsuario(novoUsuario);
}

Usuario* GerenciadorUsuarios::autenticar (const string& loginIngressado, const string& senhaIngressado) {
    for (auto& usuario : usuarios) {
        if (usuario.validarLogin(loginIngressado, senhaIngressado)){
            return &usuario;
        }
    }
    return nullptr;
}






