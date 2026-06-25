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
        string capacidadesStr, comprasStr, vendasStr, anunciosStr;

        getline(ss, idStr, ',');
        getline(ss, nome, ',');
        getline(ss, login, ',');
        getline(ss, senha, ',');
        getline(ss, pix, ',');
        // Campos novos: podem estar ausentes em linhas no formato legado
        // (arquivo escrito antes desta refatoração). Cada getline que falhar
        // (não há mais campo na linha) deixa a string correspondente vazia,
        // e tratamos isso como "use o valor padrão" abaixo.
        getline(ss, capacidadesStr, ',');
        getline(ss, comprasStr, ',');
        getline(ss, vendasStr, ',');
        getline(ss, anunciosStr, ',');

        if (idStr.empty()) continue;
        int id = stoi(idStr);

        // Padrão de usuário novo (COMPRAR + ANUNCIAR), usado quando o campo
        // de capacidades está ausente ou não contém nenhum token válido.
        std::set<Capacidade> capacidades{Capacidade::COMPRAR, Capacidade::ANUNCIAR};
        if (!capacidadesStr.empty()) {
            std::set<Capacidade> lidas;
            stringstream capSs(capacidadesStr);
            string token;
            while (getline(capSs, token, ';')) {
                try {
                    lidas.insert(stringParaCapacidade(token));
                } catch (const std::invalid_argument&) {
                    // Token corrompido/desconhecido: ignora e segue lendo os
                    // outros, em vez de derrubar o carregamento inteiro.
                }
            }
            if (!lidas.empty()) capacidades = std::move(lidas);
        }

        int comprasConcluidas = 0;
        int vendasTrocasConcluidas = 0;
        int anunciosAtivos = 0;
        try { if (!comprasStr.empty()) comprasConcluidas = stoi(comprasStr); } catch (...) {}
        try { if (!vendasStr.empty()) vendasTrocasConcluidas = stoi(vendasStr); } catch (...) {}
        try { if (!anunciosStr.empty()) anunciosAtivos = stoi(anunciosStr); } catch (...) {}

        _usuarios.emplace_back(id, nome, login, senha, pix, capacidades,
                                comprasConcluidas, vendasTrocasConcluidas, anunciosAtivos);
    }
    arquivo.close();
}

void GerenciadorUsuarios::escreverLinhaUsuario(std::ofstream& arquivo, const Usuario& usuario) const {
    arquivo << usuario.getId() << ","
            << usuario.getNome() << ","
            << usuario.getLogin() << ","
            << usuario.getSenha() << ","
            << usuario.getChavePix() << ","
            << usuario.serializarCapacidades() << ","
            << usuario.getComprasConcluidas() << ","
            << usuario.getVendasTrocasConcluidas() << ","
            << usuario.getAnunciosAtivos() << "\n";
}

void GerenciadorUsuarios::salvarUsuario(const Usuario& usuario) {
    ofstream arquivo(NOME_ARQUIVO, ios::app);
    if (arquivo.is_open()) {
        escreverLinhaUsuario(arquivo, usuario);
        arquivo.close();
    }
}

void GerenciadorUsuarios::regravarArquivoCompleto() {
    ofstream arquivo(NOME_ARQUIVO, ios::trunc);
    if (!arquivo.is_open()) return;
    for (const auto& usuario : _usuarios) {
        escreverLinhaUsuario(arquivo, usuario);
    }
    arquivo.close();
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

void GerenciadorUsuarios::notificarCompraConcluida(const string& loginUsuario) {
    Usuario* usuario = buscarUsuarioPorLogin(loginUsuario);
    if (usuario == nullptr) return;
    usuario->registrarCompraConcluida();
    regravarArquivoCompleto();
}

void GerenciadorUsuarios::notificarVendaConcluida(const string& loginUsuario) {
    Usuario* usuario = buscarUsuarioPorLogin(loginUsuario);
    if (usuario == nullptr) return;
    usuario->registrarVendaConcluida();
    regravarArquivoCompleto();
}
