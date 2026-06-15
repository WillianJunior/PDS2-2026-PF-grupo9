#ifndef TRANSACAO_HPP
#define TRANSACAO_HPP

#include <string>
#include "usuario.hpp"

enum class StatusTransacao {
    PENDENTE,
    ACEITA,
    REJEITADA,
    CONCLUIDA
};

class Transacao {
protected:
    std::string _idTransacao;
    Usuario* _usuarioProponente; // Quem iniciou a transação (comprador ou ofertante da troca)
    Usuario* _usuarioReceptor;    // Quem recebe a proposta (anunciante/vendedor)
    StatusTransacao _status;
    std::string _dataCriacao;

public:
    Transacao(const std::string& id, Usuario* proponente, Usuario* receptor)
        : _idTransacao(id), 
          _usuarioProponente(proponente), 
          _usuarioReceptor(receptor), 
          _status(StatusTransacao::PENDENTE), 
          _dataCriacao("") {}

    virtual ~Transacao() = default;

    // Getters e Setters
    std::string get_id_transacao() const { return _idTransacao; }
    Usuario* get_usuario_proponente() const { return _usuarioProponente; }
    Usuario* get_usuario_receptor() const { return _usuarioReceptor; }
    StatusTransacao get_status() const { return _status; }
    void set_status(StatusTransacao status) { _status = status; }
    
    std::string get_data_criacao() const { return _dataCriacao; }
    void set_data_criacao(const std::string& data) { _dataCriacao = data; }

    // Métodos Polimórficos Puros
    virtual bool validar_transacao() const = 0;
    virtual void executar_transacao() = 0;
};

#endif