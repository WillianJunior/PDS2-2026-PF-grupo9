#include "../include/SistemaEscambo.hpp"
#include <random>
#include <memory>
#include <stdexcept>
#include <utility>

SistemaEscambo::SistemaEscambo() {
    // _usuarios e _produtos ja foram carregados dos seus arquivos pelos proprios
    // construtores (ordem de inicializacao dos membros). Agora reconstroi o histórico
    // de transacoes usando os ponteiros reais dessas instancias.
    _transacoes.carregarTransacoes(_usuarios, _produtos);
}

GerenciadorUsuarios& SistemaEscambo::getUsuarios() { return _usuarios; }
GerenciadorProdutos& SistemaEscambo::getProdutos() { return _produtos; }
GerenciadorTransacoes& SistemaEscambo::getTransacoes() { return _transacoes; }

// REVISÃO: este método tinha sua própria cópia do "achar ou criar Anuncio",
// idêntica à de GerenciadorTransacoes::obterOuCriarAnuncio, cada uma com seu
// próprio std::vector<unique_ptr<Anuncio>> por baixo. Consolidamos num único
// registro (o do GerenciadorTransacoes) - agora é só delegar pra lá. Detalhe
// que se manteve: o Anuncio* devolvido continua sendo um ponteiro estável
// durante toda a execução do programa, porque continua vivendo dentro de um
// unique_ptr guardado num vector - só quem "possui" esse vector mudou.
Anuncio* SistemaEscambo::obterAnuncio(Produto* produto) {
    return _transacoes.obterOuCriarAnuncio(produto, _usuarios);
}

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

    // REVISÃO (bug corrigido): a geração do ID usava rand() % 10000 sem
    // nenhum srand() em lugar nenhum do projeto. Sem seed, rand() sempre
    // produz a MESMA sequência em toda execução do programa - ou seja, a
    // primeira troca proposta em qualquer sessão sempre cairia no mesmo ID
    // da primeira troca de qualquer outra sessão, e como o histórico fica
    // persistido em transacoes.txt entre execuções, isso ia colidir cedo ou
    // tarde. Troca pelo motor do <random> (que o arquivo já importava, mas
    // não usava) com seed real via std::random_device, e
    // uniform_int_distribution em vez de "% 10000" (que além de não ser
    // aleatório de verdade, ainda introduz bias nos números mais baixos).
    // O motor é "static" pra ser inicializado (e ressemeado) só uma vez,
    // não a cada chamada desta função.
    static std::mt19937 geradorAleatorio(std::random_device{}());
    static std::uniform_int_distribution<int> distribuicaoId(0, 9999);

    std::string idTransacao = "TR" + std::to_string(distribuicaoId(geradorAleatorio));

    // Alocação dinâmica envelopada estritamente em um unique_ptr (RAII em ação!)
    auto novaTroca = std::make_unique<Troca>(std::move(idTransacao), proponente, receptor, alvo, ofertado);
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

        // dynamic_cast aqui é necessário: a Fachada só recebe um Transacao*
        // genérico (vindo da caixa de notificações), mas só sabemos como
        // "liquidar" o efeito colateral (baixar estoque dos dois lados) se
        // for de fato uma Troca - uma Compra, por exemplo, teria outro efeito.
        if (Troca* troca = dynamic_cast<Troca*>(transacao)) {
            if (troca->get_anuncio_alvo() && troca->get_anuncio_alvo()->get_produto() &&
                troca->get_anuncio_ofertado() && troca->get_anuncio_ofertado()->get_produto()) {

                const std::string& idAlvo = troca->get_anuncio_alvo()->get_produto()->get_id();
                const std::string& idOfertado = troca->get_anuncio_ofertado()->get_produto()->get_id();

                // Desconta do estoque a quantidade efetivamente trocada de cada lado.
                // Se for o ultimo do estoque, o produto e desativado e some das vitrines.
                _produtos.venderProduto(idAlvo, troca->get_quantidade_alvo());
                _produtos.venderProduto(idOfertado, troca->get_quantidade_ofertada());
            }
        }
    } else {
        transacao->set_status(StatusTransacao::REJEITADA);
    }
}
