#ifndef TERMINAL_VIEW_HPP
#define TERMINAL_VIEW_HPP

#include <string>
#include <utility>
#include <vector>
#include "ItemVendido.hpp"
#include "Produto.hpp"
#include "Troca.hpp"

// Responsabilidade única: todo cout e cin do sistema passa por aqui.
// Nenhum if de decisão de fluxo — condicionais aqui só escolhem O QUE mostrar,
// nunca o que fazer depois (isso é do AppController).
class TerminalView {
public:
    // ---- Primitivas ----
    void limparTela();
    void pausar();          // imprime "Pressione Enter..." e lê Enter
    void mensagem(const std::string& texto);
    void erro(const std::string& texto);

    int         lerOpcaoInteira();   // loop até entrada válida; imprime [Erro] nos inválidos
    double      lerValorDouble();
    std::string lerLinha();

    // ---- Menus (só display) ----
    void exibirMenuPrincipal();
    void exibirMenuPerfil(const std::string& nomeUsuario);
    void exibirMenuAnunciante(bool podeAnunciar, int anunciosAtivos, int limiteAnuncios);
    void exibirMenuComprador(const std::string& nomeUsuario, bool podeTrocar);
    void exibirMenuCarrinho(const std::vector<ItemVendido>& itens, double total);
    void exibirMenuBusca();
    void exibirMenuEdicaoProduto(const std::string& nomeAtual, double precoAtual,
                                  const std::string& catAtual, const std::string& subAtual);

    // ---- Listagens ----
    void exibirVitrine(const std::vector<const Produto*>& produtos);
    void exibirMeusProdutos(const std::vector<const Produto*>& produtos);
    void exibirProdutosParaOferta(const std::vector<Produto*>& produtos);
    void exibirPropostaTroca(const Troca* troca);

    // ---- Formulários combinados (prompt + leitura) ----
    struct DadosCadastro {
        std::string nome;
        std::string login;
        std::string senha;
        std::string chavePix;
    };
    DadosCadastro solicitarDadosCadastro();

    std::pair<std::string, std::string> solicitarCredenciaisLogin();

    struct DadosNovoProduto {
        std::string nome;
        double preco;
        std::string categoria;
        std::string subcategoria;
        int quantidade;
    };
    DadosNovoProduto solicitarDadosNovoProduto();

    // Mostra sub-opções de categoria e lê a escolha; retorna "Geral" para "Outros".
    std::string solicitarSubcategoria(const std::string& categoria);
    // Solicita categoria + subcategoria (usado na edição de produto).
    std::pair<std::string, std::string> solicitarNovaCategoriaSubcategoria();

    // Mostra lista numerada de produtos e lê a escolha (retorna 1-based ou 0 para cancelar).
    int solicitarEscolhaProdutoParaEditar(const std::vector<const Produto*>& produtos);
    std::string solicitarNovoNome();
    double      solicitarNovoPreco();

    // Busca na vitrine global
    std::pair<std::string, std::string> solicitarFiltroCategoria(); // retorna (cat, sub)
    std::string solicitarFiltroNome();
    int solicitarEscolhaProdutoCarrinho(int totalProdutos); // 1-based ou 0
    int solicitarQuantidade();

    // Finalizar compra
    void exibirItemFinalizacao(const std::string& nomeProduto, int quantidade, double subtotal,
                                const std::string& nomeVendedor, const std::string& chavePix);

    // E-scambo
    int solicitarItemAlvoTroca(const std::vector<ItemVendido>& itens);
    int solicitarProdutoOfertaTroca(const std::vector<Produto*>& produtos);
    std::string solicitarMensagemTroca();
};

#endif
