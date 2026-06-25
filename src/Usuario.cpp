#include "../include/Usuario.hpp"
#include <stdexcept>
#include <utility>

using namespace std;

std::string capacidadeParaString(Capacidade cap) {
    switch (cap) {
        case Capacidade::COMPRAR: return "COMPRAR";
        case Capacidade::ANUNCIAR: return "ANUNCIAR";
        case Capacidade::TROCAR: return "TROCAR";
    }
    throw std::invalid_argument("Capacidade desconhecida");
}

Capacidade stringParaCapacidade(const std::string& texto) {
    if (texto == "COMPRAR") return Capacidade::COMPRAR;
    if (texto == "ANUNCIAR") return Capacidade::ANUNCIAR;
    if (texto == "TROCAR") return Capacidade::TROCAR;
    throw std::invalid_argument("Texto de capacidade desconhecido: " + texto);
}

// REVISÃO: os parâmetros de string continuam recebidos por valor (idiom "pass
// by value, then move"), mas agora usamos std::move pra inicializar os
// atributos. Antes, cada string passada (nome, login, senha, chavePix) era
// copiada de novo aqui dentro; com move, "roubamos" o buffer que já tinha sido
// alocado pro parâmetro em vez de duplicar a alocação - zero cópias extra.
Usuario::Usuario(int id, string nome, string login, string senha, string chavePix)
    : _id(id), _nome(std::move(nome)), _login(std::move(login)), _senha(std::move(senha)),
      _chavePix(std::move(chavePix)), _capacidades{Capacidade::COMPRAR, Capacidade::ANUNCIAR},
      _comprasConcluidas(0), _vendasTrocasConcluidas(0), _anunciosAtivos(0) {}

Usuario::Usuario(int id, string nome, string login, string senha, string chavePix,
                  std::set<Capacidade> capacidades, int comprasConcluidas,
                  int vendasTrocasConcluidas, int anunciosAtivos)
    : _id(id), _nome(std::move(nome)), _login(std::move(login)), _senha(std::move(senha)),
      _chavePix(std::move(chavePix)), _capacidades(std::move(capacidades)),
      _comprasConcluidas(comprasConcluidas), _vendasTrocasConcluidas(vendasTrocasConcluidas),
      _anunciosAtivos(anunciosAtivos) {}

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

bool Usuario::temCapacidade(Capacidade cap) const {
    return _capacidades.find(cap) != _capacidades.end();
}

void Usuario::concederCapacidade(Capacidade cap) {
    // std::set::insert não duplica se a chave já existe
    _capacidades.insert(cap);
}

void Usuario::registrarCompraConcluida() {
    _comprasConcluidas++;
    if (_comprasConcluidas >= 1) {
        concederCapacidade(Capacidade::TROCAR);
    }
}

void Usuario::registrarVendaConcluida() {
    _vendasTrocasConcluidas++;
}

NivelUsuario Usuario::getNivel() const {
    return _vendasTrocasConcluidas >= VENDAS_PARA_VETERANO ? NivelUsuario::VETERANO : NivelUsuario::INICIANTE;
}

int Usuario::getLimiteAnuncios() const {
    return getNivel() == NivelUsuario::VETERANO ? ANUNCIOS_ILIMITADO : LIMITE_ANUNCIOS_INICIANTE;
}

bool Usuario::podeCriarAnuncio() const {
    int limite = getLimiteAnuncios();
    return limite == ANUNCIOS_ILIMITADO || _anunciosAtivos < limite;
}

void Usuario::incrementarAnunciosAtivos() {
    _anunciosAtivos++;
}

void Usuario::decrementarAnunciosAtivos() {
    if (_anunciosAtivos > 0) {
        _anunciosAtivos--;
    }
}

int Usuario::getAnunciosAtivos() const { return _anunciosAtivos; }
int Usuario::getComprasConcluidas() const { return _comprasConcluidas; }
int Usuario::getVendasTrocasConcluidas() const { return _vendasTrocasConcluidas; }

std::string Usuario::serializarCapacidades() const {
    std::string resultado;
    bool primeiro = true;
    for (Capacidade cap : _capacidades) {
        if (!primeiro) resultado += ";";
        resultado += capacidadeParaString(cap);
        primeiro = false;
    }
    return resultado;
}
