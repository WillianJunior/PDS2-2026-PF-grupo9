#ifndef UI_OUTPUT_HPP
#define UI_OUTPUT_HPP

#include <string>
#include <vector>
#include "Produto.hpp"
#include "ItemVendido.hpp"
#include "Transacao.hpp"
#include "Troca.hpp"

class UIOutput {
public:
    void limparTela();
    void pausar();

    // Cabecalhos e menus principais
    void exibirMenuPrincipal();
    void exibirMenuPerfil(const std::string& nomeUsuario);
    void exibirMenuAnunciante();
    void exibirMenuComprador(const std::string& nomeUsuario);
    void exibirMenuCarrinho(const std::vector<ItemVendido>& itens, double total);
    void exibirMenuBusca();
    void exibirMenuEdicaoProduto(const std::string& nomeAtual, double precoAtual,
                                  const std::string& catAtual, const std::string& subAtual);

    // Listagens de produtos
    void exibirVitrine(const std::vector<const Produto*>& produtos);
    void exibirMeusProdutos(const std::vector<const Produto*>& produtos);
    void exibirProdutosParaOferta(const std::vector<Produto*>& produtos);

    void exibirPropostaTroca(const Troca* troca);

    void mensagem(const std::string& texto);
    void erro(const std::string& texto);
};

#endif