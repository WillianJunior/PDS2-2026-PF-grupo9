#include "../include/GerenciadorUsuarios.hpp"
#include <fstream>
#include <sstream>
#include <utility>

using namespace std;

GerenciadorUsuarios::GerenciadorUsuarios() {
    carregarUsuarios();
}

void GerenciadorUsuarios::carregarUsuarios() {
    ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string idStr, nome, login, senha, pix;

        getline(ss, idStr, ',');
        getline(ss, nome, ',');
        getline(ss, login, ',');
        getline(ss, senha, ',');
        getline(ss, pix, ',');
        if (!idStr.empty()) {
            int id = stoi(idStr);
            _usuarios.emplace_back(id, nome, login, senha, pix);
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

bool GerenciadorUsuarios::registrarUsuario(const string& nome, const string& login, const string& senha, const string& chavePix) {
    // 1. Verifica se o login (e-mail) já existe na lista de usuários carregados
    for (const auto& usuario : _usuarios) {
        if (usuario.getLogin() == login) {
            // Encontrou um usuário com o mesmo e-mail! Interrompe o cadastro.
            return false;
        }
    }

    // 2. Se o loop terminar e não achar ninguém, gera o ID e cadastra normalmente
    int novoId = _usuarios.size() + 1;

    // REVISÃO: antes fazíamos push_back(novoUsuario) (cópia) e só depois
    // chamávamos salvarUsuario(novoUsuario) usando a variável local original.
    // Agora movemos novoUsuario pro deque (sem copiar de novo) e persistimos
    // lendo direto de _usuarios.back() - é a cópia que de fato vai ficar viva
    // na memória, então não tem motivo pra manter o objeto local depois do
    // move. Isso também evita o erro clássico de "usar a variável depois do
    // std::move", que aconteceria se só tivéssemos trocado o push_back sem
    // reordenar a gravação no arquivo.
    Usuario novoUsuario(novoId, nome, login, senha, chavePix);
    _usuarios.push_back(std::move(novoUsuario));
    salvarUsuario(_usuarios.back());

    // Retorna true para avisar à interface que o cadastro foi um sucesso
    return true;
}

Usuario* GerenciadorUsuarios::autenticar(const string& loginIngressado, const string& senhaIngressado) {
    // Busca linear: com a escala de usuários de um projeto acadêmico (algumas
    // dezenas de contas), isso é instantâneo. Trocar por um std::unordered_map
    // só valeria a pena com uma base de usuários muito maior - otimização
    // prematura aqui só adicionaria complexidade sem ganho perceptível.
    for (auto& usuario : _usuarios) {
        if (usuario.validarLogin(loginIngressado, senhaIngressado)) {
            return &usuario;
        }
    }
    return nullptr;
}

Usuario* GerenciadorUsuarios::buscarUsuarioPorLogin(const string& login) {
    for (auto& u : _usuarios) {
        if (u.getLogin() == login) {
            return &u;
        }
    }
    return nullptr;
}
