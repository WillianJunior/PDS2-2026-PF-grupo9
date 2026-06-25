#ifndef APP_CONTROLLER_HPP
#define APP_CONTROLLER_HPP

/*
 * Mapeamento TerminalUI → AppController + TerminalView:
 *
 *   TerminalUI::iniciar()            → AppController::executar()
 *   TerminalUI::menuLogin()          → AppController::fluxoLogin()
 *   TerminalUI::menuCadastro()       → AppController::fluxoCadastro()
 *   TerminalUI::menuEscolhaPerfil()  → AppController::fluxoMenuPerfil()
 *   TerminalUI::menuAnunciante() (1) → AppController::fluxoCadastrarProduto()
 *   TerminalUI::menuAnunciante() (3) → AppController::fluxoEditarProduto()
 *   TerminalUI::menuAnunciante() (4) → AppController::fluxoNotificacoes()
 *   TerminalUI::menuAnunciante()     → AppController::fluxoAnunciante()
 *   TerminalUI::menuComprador() (1)  → AppController::fluxoBuscarEAdicionarCarrinho()
 *   TerminalUI::menuComprador()      → AppController::fluxoComprador()
 *   TerminalUI::menuCarrinho() (1)   → AppController::fluxoFinalizarCompra()
 *   TerminalUI::menuCarrinho() (2)   → AppController::fluxoProporEscambo()
 *   TerminalUI::menuCarrinho()       → AppController::fluxoCarrinho()
 *   TerminalUI::escolherSubcategoria()→ TerminalView::solicitarSubcategoria()
 *   TerminalUI::limparTela()         → TerminalView::limparTela()
 *   TerminalUI::pausar()             → TerminalView::pausar()
 *   TerminalUI::lerOpcaoInteira()    → TerminalView::lerOpcaoInteira()
 *   TerminalUI::lerValorDouble()     → TerminalView::lerValorDouble()
 *   TerminalUI::lerLinha()           → TerminalView::lerLinha()
 */

#include "Carrinho.hpp"
#include "SistemaEscambo.hpp"
#include "TerminalView.hpp"
#include "Usuario.hpp"

class AppController {
private:
    TerminalView  _view;
    SistemaEscambo _sistema;
    Carrinho       _carrinho;

    void fluxoLogin();
    void fluxoCadastro();
    void fluxoMenuPerfil(Usuario* usuario);

    void fluxoAnunciante(Usuario* usuario);
    void fluxoCadastrarProduto(Usuario* usuario);
    void fluxoEditarProduto(Usuario* usuario);
    void fluxoNotificacoes(Usuario* usuario);

    void fluxoComprador(Usuario* usuario);
    void fluxoBuscarEAdicionarCarrinho(Usuario* usuario);

    void fluxoCarrinho(Usuario* usuario);
    void fluxoFinalizarCompra(Usuario* usuario);
    void fluxoProporEscambo(Usuario* usuario);

public:
    void executar(); // ponto de entrada; substitui TerminalUI::iniciar()
};

#endif
