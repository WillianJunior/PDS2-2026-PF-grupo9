#include "../include/GerenciadorUsuarios.hpp"
#include <fstream>
#include <sstream>

using namespace std;

GerenciadorUsuarios::GerenciadorUsuarios() {
    carregarUsuarios();
}

void GerenciadorUsuarios::carregarUsuarios() {
    ifstream arquivo(NOME_ARQUIVO);
    if(!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string idStr, nome, login, senha, pix;

        getline(ss, idStr, ','); 
        getline(ss, nome, ',');
        getline(ss, login, ',');
        getline(ss, senha, ',');
        getline(ss, pix, ',');
        if (!idStr.empty()){
            int id = stoi(idStr);
            usuarios.emplace_back(id, nome, login, senha, pix);
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
                << usuario.getSenha() << ","
                << usuario.getChavePix() << "\n";
        arquivo.close();
    }
}

bool GerenciadorUsuarios::registrarUsuario(const string& nome, const string& login, const string& senha) {
    // 1. Verifica se o login (e-mail) já existe na lista de usuários carregados
    for (const auto& usuario : usuarios) {
        if (usuario.getLogin() == login) {
            // Encontrou um usuário com o mesmo e-mail! Interrompe o cadastro.
            return false; 
        }
    }

    // 2. Se o loop terminar e não achar ninguém, gera o ID e cadastra normalmente
    int novoId = usuarios.size() + 1; 
    
    Usuario novoUsuario(novoId, nome, login, senha);
    usuarios.push_back(novoUsuario);
    salvarUsuario(novoUsuario);

    // Retorna true para avisar à interface que o cadastro foi um sucesso
    return true; 
}

Usuario* GerenciadorUsuarios::autenticar (const string& loginIngressado, const string& senhaIngressado) {
    for (auto& usuario : usuarios) {
        if (usuario.validarLogin(loginIngressado, senhaIngressado)){
            return &usuario;
        }
    }
    return nullptr;
}

Usuario* GerenciadorUsuarios::buscarUsuarioPorLogin(const string& login) {
    for (auto& u : usuarios) {
        if (u.getLogin() == login) {
            return &u;
        }
    }
    return nullptr;
}