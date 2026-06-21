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

// Classe base abstrata pra Compra e Troca. A ideia de usar polimorfismo aqui
// é clássica de Strategy: GerenciadorTransacoes e a Fachada só conhecem o
// tipo Transacao* (validar_transacao/executar_transacao são puramente
// virtuais), então não precisam de um if/else pra cada tipo concreto - cada
// subclasse sabe validar e executar a própria regra de negócio. Isso deixa
// o sistema aberto pra novos tipos de transação sem tocar no código que já
// existe (Open/Closed Principle).
class Transacao {
protected:
    std::string _idTransacao;
    Usuario* _usuarioProponente;
    Usuario* _usuarioReceptor;
    StatusTransacao _status;
    std::string _dataCriacao;
    std::string _mensagem;

public:
    Transacao(std::string id, Usuario* proponente, Usuario* receptor);

    // Destrutor virtual obrigatório: sem isso, destruir um Transacao* que na
    // verdade é uma Troca/Compra (caso comum aqui, já que GerenciadorTransacoes
    // guarda só Transacao* dentro do unique_ptr) não chamaria o destrutor
    // certo da classe derivada -> vazamento/comportamento indefinido.
    virtual ~Transacao() = default;

    // REVISÃO: getters de string (id, data, mensagem) agora devolvem
    // const std::string& em vez de copiar a string a cada chamada.
    const std::string& get_id_transacao() const;
    Usuario* get_usuario_proponente() const;
    Usuario* get_usuario_receptor() const;
    StatusTransacao get_status() const;
    void set_status(StatusTransacao status);

    const std::string& get_data_criacao() const;
    void set_data_criacao(const std::string& data);

    const std::string& get_mensagem() const;
    void set_mensagem(const std::string& msg);

    // Cada subclasse decide o que significa "válido" pra ela (estoque, preços
    // compatíveis, etc.) e o que fazer ao "executar". A Fachada só chama essas
    // duas funções sem saber se está lidando com uma Compra ou uma Troca.
    virtual bool validar_transacao() const = 0;
    virtual void executar_transacao() = 0;
};

#endif
