#ifndef SISTEMA_ESCAMBO_HPP
#define SISTEMA_ESCAMBO_HPP

#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"
#include "GerenciadorTransacoes.hpp"
#include "Anuncio.hpp"
#include "Troca.hpp"
#include <string>
#include <vector>

class SistemaEscambo {
private:
    GerenciadorUsuarios _usuarios;
    GerenciadorProdutos _produtos;
    GerenciadorTransacoes _transacoes;

public:
    SistemaEscambo();

    // Acesso rápido aos gestores (caso a UI precise de listas simples)
    GerenciadorUsuarios& getUsuarios();
    GerenciadorProdutos& getProdutos();
    GerenciadorTransacoes& getTransacoes();

    // === Lógica de Negócio Centralizada ===
    
    // Devolve os produtos ativos de um utilizador específico
    std::vector<Produto*> buscarProdutosDoUsuario(const std::string& login);

    // Valida as regras de negócio e cria a proposta de troca
    bool enviarPropostaTroca(Usuario* proponente, Usuario* receptor, Anuncio* alvo, Anuncio* ofertado, const std::string& mensagem);

    // Processa a resposta a uma notificação (Aceitar ou Recusar) e aplica o Soft Delete
    void processarRespostaTroca(Transacao* transacao, bool aceitar);
};

#endif