#include "../include/Transacao.hpp"
#include <utility>

// REVISÃO: id por valor + std::move no initializer list (mesmo padrão do
// Model), em vez de copiar a string do id mais uma vez aqui na base.
Transacao::Transacao(std::string id, Usuario* proponente, Usuario* receptor)
    : _idTransacao(std::move(id)),
      _usuarioProponente(proponente),
      _usuarioReceptor(receptor),
      _status(StatusTransacao::PENDENTE),
      _dataCriacao(""),
      _mensagem("") {}

const std::string& Transacao::get_id_transacao() const { return _idTransacao; }
Usuario* Transacao::get_usuario_proponente() const { return _usuarioProponente; }
Usuario* Transacao::get_usuario_receptor() const { return _usuarioReceptor; }
StatusTransacao Transacao::get_status() const { return _status; }
void Transacao::set_status(StatusTransacao status) { _status = status; }
const std::string& Transacao::get_data_criacao() const { return _dataCriacao; }
void Transacao::set_data_criacao(const std::string& data) { _dataCriacao = data; }

const std::string& Transacao::get_mensagem() const { return _mensagem; }
void Transacao::set_mensagem(const std::string& msg) { _mensagem = msg; }
