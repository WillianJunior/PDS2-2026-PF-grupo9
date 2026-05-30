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
        string idStr, nome, login, senha;

        getline(ss, idStr, ','); // <-- Agora lê o ID primeiro
        getline(ss, nome, ',');
        getline(ss, login, ',');
        getline(ss, senha, ',');

        if (!idStr.empty()){
            int id = stoi(idStr);
            usuarios.emplace_back(id, nome, login, senha);
        }
    }
    arquivo.close();
}

void GerenciadorUsuarios::salvarUsuario(const Usuario& usuario) {
    ofstream arquivo(NOME_ARQUIVO, ios::app);
    if (arquivo.is_open()) {
        arquivo << usuario.getId() << ","
                << usuario.getNome() << ","
                << usuario.getLogin() << ","
                << usuario.getSenha() << "\n";
        arquivo.close();
    }
}

void GerenciadorUsuarios::registrarUsuario(const string& nome, const string& login, const string& senha) {
    // Gera o ID automaticamente: se tem 0 usuários, ele será o 1. Se tem 5, será o 6.
    int novoId = usuarios.size() + 1; 
    
    Usuario novoUsuario(novoId, nome, login, senha);
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