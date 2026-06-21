#ifndef TERMINAL_UI_HPP
#define TERMINAL_UI_HPP

#include "GerenciadorUsuarios.hpp"
#include "GerenciadorProdutos.hpp"
#include "Carrinho.hpp"
#include "SistemaEscambo.hpp"
#include "Transacao.hpp"      
#include "Troca.hpp"         

using namespace std;

class TerminalUI {
private:
    Carrinho carrinhoCompras;
    SistemaEscambo sistema;

    void limparTela();

    // Leitura segura de entradas do usuario: nunca deixam o programa crashar
    // ou entrar em loop infinito por causa de letras, simbolos ou Enter vazio.
    int lerOpcaoInteira();
    double lerValorDouble();
    string lerLinha();

    // REVISÃO: extraído porque "Pressione Enter para continuar..." + lerLinha()
    // se repetia colado em mais de uma dezena de lugares do arquivo - clássico
    // copy-paste que um helper de uma linha resolve de vez.
    void pausar();

    // REVISÃO: extraído porque o sub-menu de Subcategoria (Veiculo->Carro/Moto/
    // Caminhao, Eletrodomestico->Cozinha/Quarto/..., Roupa->Camisa/Calca/Tenis)
    // estava copiado e colado em TRES lugares (Cadastro de Produto, Edicao de
    // Produto e Filtro da Vitrine). Centralizar aqui significa que, se um dia
    // mudar a arvore de subcategorias, só precisa mudar num lugar.
    string escolherSubcategoria(const string& categoria);

    void menuAnunciante(Usuario* usuario);
    void menuComprador(Usuario* usuario);
    void menuCarrinho(Usuario* usuario);

public:
    void iniciar();
    void menuLogin();
    void menuCadastro();
    void menuEscolhaPerfil(Usuario* usuario);
};

#endif