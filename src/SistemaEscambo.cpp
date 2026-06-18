#include "../include/SistemaEscambo.hpp"
#include <random>
#include <memory>
#include <stdexcept>

SistemaEscambo::SistemaEscambo() = default;

GerenciadorUsuarios& SistemaEscambo::getUsuarios() { return _usuarios; }
GerenciadorProdutos& SistemaEscambo::getProdutos() { return _produtos; }
GerenciadorTransacoes& SistemaEscambo::getTransacoes() { return _transacoes; }

std::vector<Produto*> SistemaEscambo::buscarProdutosDoUsuario(const std::string& login) {
    std::vector<Produto*> todos = _produtos.buscarProdutosPorUsuario(login); 
    std::vector<Produto*> ativos;
    
    for (Produto* p : todos) {
        if (p && p->is_ativo()) {
            ativos.push_back(p);
        }
    }
    return ativos;
}

bool SistemaEscambo::enviarPropostaTroca(Usuario* proponente, Usuario* receptor, Anuncio* alvo, Anuncio* ofertado, const std::string& mensagem) {
    if (!proponente || !receptor || !alvo || !ofertado) {
        return false; // Programação defensiva contra ponteiros nulos externos
    }

    std::string idTransacao = "TR" + std::to_string(rand() % 10000);
    
    // Alocação dinâmica envelopada estritamente em um unique_ptr (RAII em ação!)
    auto novaTroca = std::make_unique<Troca>(idTransacao, proponente, receptor, alvo, ofertado);
    novaTroca->set_mensagem(mensagem);

    if (novaTroca->verificar_precos_similares()) {
        // Transfere o controle do unique_ptr para o gerenciador sem vazamento de memória
        _transacoes.adicionarTransacao(std::move(novaTroca));
        return true;
    } 
    // Se falhar na regra de negócio, o unique_ptr sai de escopo e limpa a memória sozinho.
    // Sem necessidade do 'delete novaTroca;' problemático.
    return false;
}

void SistemaEscambo::processarRespostaTroca(Transacao* transacao, bool aceitar) {
    if (!transacao) return;

    if (aceitar) {
        transacao->set_status(StatusTransacao::ACEITA);
        transacao->executar_transacao();
        
        if (Troca* troca = dynamic_cast<Troca*>(transacao)) {
            if (troca->get_anuncio_alvo() && troca->get_anuncio_alvo()->get_produto() &&
                troca->get_anuncio_ofertado() && troca->get_anuncio_ofertado()->get_produto()) {
                
                std::string idAlvo = troca->get_anuncio_alvo()->get_produto()->get_id();
                std::string idOfertado = troca->get_anuncio_ofertado()->get_produto()->get_id();
                
                _produtos.inativarProduto(idAlvo);
                _produtos.inativarProduto(idOfertado);
            }
        }
    } else {
        transacao->set_status(StatusTransacao::REJEITADA);
    }
}