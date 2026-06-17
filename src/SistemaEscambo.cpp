#include "../include/SistemaEscambo.hpp"
#include <random>

SistemaEscambo::SistemaEscambo() {
    // Os gerenciadores já são construídos automaticamente aqui
}

GerenciadorUsuarios& SistemaEscambo::getUsuarios() { return _usuarios; }
GerenciadorProdutos& SistemaEscambo::getProdutos() { return _produtos; }
GerenciadorTransacoes& SistemaEscambo::getTransacoes() { return _transacoes; }

std::vector<Produto*> SistemaEscambo::buscarProdutosDoUsuario(const std::string& login) {

    std::vector<Produto*> todos = _produtos.buscarProdutosPorUsuario(login); 
    
    std::vector<Produto*> ativos;
    
    // Agora sim o 'todos' existe para ser filtrado
    for (Produto* p : todos) {
        if (p->is_ativo()) {
            ativos.push_back(p);
        }
    }
    return ativos;
}

bool SistemaEscambo::enviarPropostaTroca(Usuario* proponente, Usuario* receptor, Anuncio* alvo, Anuncio* ofertado, const std::string& mensagem) {
    // Gera um ID único para a transação
    std::string idTransacao = "TR" + std::to_string(rand() % 10000);
    
    Troca* novaTroca = new Troca(idTransacao, proponente, receptor, alvo, ofertado);
    novaTroca->set_mensagem(mensagem);

    // A lógica de negócio fica toda escondida aqui!
    if (novaTroca->verificar_precos_similares()) {
        _transacoes.adicionarTransacao(novaTroca);
        return true;
    } else {
        delete novaTroca; // Liberta a memória se a margem de 20% for ultrapassada
        return false;
    }
}

void SistemaEscambo::processarRespostaTroca(Transacao* transacao, bool aceitar) {
    if (aceitar) {
        transacao->set_status(StatusTransacao::ACEITA);
        transacao->executar_transacao();
        
        // soft delete: se for aceito, inativamos os dois produtos no ficheiro
        Troca* troca = dynamic_cast<Troca*>(transacao);
        if (troca) {
            std::string idAlvo = troca->get_anuncio_alvo()->get_produto()->get_id();
            std::string idOfertado = troca->get_anuncio_ofertado()->get_produto()->get_id();
            
            _produtos.inativarProduto(idAlvo);
            _produtos.inativarProduto(idOfertado);
        }
    } else {
        transacao->set_status(StatusTransacao::REJEITADA);
    }
}