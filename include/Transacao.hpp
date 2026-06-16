#ifndef TRANSACAO_HPP
#define TRANSACAO_HPP

#include <string>
#include "Usuario.hpp"

enum class StatusTransacao {
    PENDENTE,
    ACEITA,
    REJEITADA,
    CONCLUIDA
};

class Transacao {
protected:
    std::string _idTransacao;
    Usuario* _usuarioProponente; 
    Usuario* _usuarioReceptor;    
    StatusTransacao _status;
    std::string _dataCriacao;

public:
    Transacao(const std::string& id, Usuario* proponente, Usuario* receptor);
    virtual ~Transacao() = default;

    std::string get_id_transacao() const;
    Usuario* get_usuario_proponente() const;
    Usuario* get_usuario_receptor() const;
    StatusTransacao get_status() const;
    void set_status(StatusTransacao status);
    
    std::string get_data_criacao() const;
    void set_data_criacao(const std::string& data);

    virtual bool validar_transacao() const = 0;
    virtual void executar_transacao() = 0;
};

#endif