#include "../include/Transacao.hpp" 

Transacao::Transacao(const std::string& id, Usuario* proponente, Usuario* receptor)
    : _idTransacao(id), 
      _usuarioProponente(proponente), 
      _usuarioReceptor(receptor), 
      _status(StatusTransacao::PENDENTE), 
      _dataCriacao("") {}

// Como é uma classe abstrata e os métodos validar e executar são puramente virtuais (= 0 no .hpp),
// não precisamos implementá-los aqui.

std::string Transacao::get_id_transacao() const { return _idTransacao; }

Usuario* Transacao::get_usuario_proponente() const { return _usuarioProponente; }

Usuario* Transacao::get_usuario_receptor() const { return _usuarioReceptor; }

StatusTransacao Transacao::get_status() const { return _status; }

void Transacao::set_status(StatusTransacao status) { _status = status; }

std::string Transacao::get_data_criacao() const { return _dataCriacao; }

void Transacao::set_data_criacao(const std::string& data) { _dataCriacao = data; }