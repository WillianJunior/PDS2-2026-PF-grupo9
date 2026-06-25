#include "../include/UIOutput.hpp"
#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

void UIOutput::limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Exibe o prompt de pausa. A TerminalUI chama input.lerLinha() logo apos.
void UIOutput::pausar() {
    cout << "\nPressione Enter para continuar...";
}

// ==========================================
// MENUS
// ==========================================

void UIOutput::exibirMenuPrincipal() {
    cout << "\n=== E-scambo Terminal ===" << endl;
    cout << "1 - Fazer Login" << endl;
    cout << "2 - Cadastrar Novo Usuario" << endl;
    cout << "3 - Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuPerfil(const string& nomeUsuario) {
    cout << "\nBem-vindo(a), " << nomeUsuario << "!" << endl;
    cout << "Selecione o seu perfil de acesso:" << endl;
    cout << "1 - Comprador" << endl;
    cout << "2 - Anunciante" << endl;
    cout << "3 - Sair da Conta (Logout)" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuAnunciante() {
    cout << "\n=== Painel do Anunciante ===" << endl;
    cout << "1 - Cadastrar Novo Produto" << endl;
    cout << "2 - Ver Minha Vitrine" << endl;
    cout << "3 - Editar um Produto" << endl;
    cout << "4 - Notificacoes" << endl;
    cout << "5 - Voltar" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuComprador(const string& nomeUsuario) {
    cout << "\n=== Area de Compras ===" << endl;
    cout << "Bem-vindo(a), " << nomeUsuario << "!" << endl;
    cout << "1 - Ver Vitrine Global / Pesquisar" << endl;
    cout << "2 - Meu Carrinho" << endl;
    cout << "3 - Voltar" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuCarrinho(const vector<ItemVendido>& itens, double total) {
    cout << "\n=== Meu Carrinho ===" << endl;

    if (itens.empty()) {
        cout << "Seu carrinho esta vazio no momento." << endl;
    } else {
        for (size_t i = 0; i < itens.size(); ++i) {
            cout << "(" << i + 1 << ") " << itens[i].get_nomeProduto()
                 << " | Qtd: " << itens[i].get_quantidade()
                 << " | Subtotal: R$ " << itens[i].get_subtotal() << endl;
        }
        cout << "---------------------------------" << endl;
        cout << "TOTAL DO PEDIDO: R$ " << total << endl;
    }

    cout << "\n1 - Finalizar Compra (PIX)" << endl;
    cout << "2 - Propor E-scambo" << endl;
    cout << "3 - Esvaziar Carrinho" << endl;
    cout << "4 - Voltar" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuBusca() {
    cout << "=== Menu de Busca de Produtos ===" << endl;
    cout << "1 - Ver TODOS os produtos disponiveis" << endl;
    cout << "2 - Pesquisar por Categoria e Subcategoria" << endl;
    cout << "3 - Pesquisar por Nome" << endl;
    cout << "Escolha uma opcao: ";
}

void UIOutput::exibirMenuEdicaoProduto(const string& nomeAtual, double precoAtual,
                                        const string& catAtual, const string& subAtual) {
    cout << "\n--- Editando: " << nomeAtual << " ---" << endl;
    cout << "1 - Editar Nome (Atual: " << nomeAtual << ")" << endl;
    cout << "2 - Editar Preco (Atual: R$ " << precoAtual << ")" << endl;
    cout << "3 - Editar Categoria/Subcategoria (Atual: " << catAtual << " > " << subAtual << ")" << endl;
    cout << "4 - Salvar alteracoes e Voltar" << endl;
    cout << "5 - Cancelar sem salvar" << endl;
    cout << "Escolha o que deseja alterar: ";
}

// ==========================================
// LISTAGENS
// ==========================================

void UIOutput::exibirVitrine(const vector<const Produto*>& produtos) {
    cout << "\n--- Resultados da Busca ---" << endl;
    for (size_t i = 0; i < produtos.size(); ++i) {
        const Produto* p = produtos[i];
        cout << "(" << i + 1 << ") ID: " << p->get_id()
             << " | Nome: " << p->get_nome()
             << "\n    Categoria: " << p->get_categoria() << " > " << p->get_subcategoria()
             << "\n    Preco: R$ " << p->get_preco()
             << "\n    Estoque: " << p->get_quantidade_estoque()
             << "\n    Vendedor: " << p->get_login_anunciante() << "\n" << endl;
    }
}

void UIOutput::exibirMeusProdutos(const vector<const Produto*>& produtos) {
    cout << "\n--- Minha Vitrine ---" << endl;
    for (const auto* p : produtos) {
        cout << "ID: " << p->get_id()
             << " | Nome: " << p->get_nome()
             << "\n  Categoria: " << p->get_categoria() << " > " << p->get_subcategoria()
             << "\n  Preco: R$ " << p->get_preco()
             << "\n  Estoque: " << p->get_quantidade_estoque() << "\n" << endl;
    }
    if (produtos.empty()) {
        cout << "Voce nao tem nenhum produto ativo na vitrine no momento." << endl;
    }
}

void UIOutput::exibirProdutosParaOferta(const vector<Produto*>& produtos) {
    cout << "\nSeus produtos disponiveis para oferecer:\n" << endl;
    for (size_t i = 0; i < produtos.size(); ++i) {
        cout << "(" << i + 1 << ") " << produtos[i]->get_nome()
             << " | R$ " << produtos[i]->get_preco()
             << " | ID: " << produtos[i]->get_id() << endl;
    }
}

// ==========================================
// NOTIFICACOES
// ==========================================

void UIOutput::exibirPropostaTroca(const Troca* troca) {
    cout << "\n[NOVA PROPOSTA DE E-SCAMBO]" << endl;
    cout << "De: " << troca->get_usuario_proponente()->getNome() << endl;
    cout << "Quer : " << troca->get_anuncio_alvo()->get_produto()->get_nome() << endl;
    cout << "Oferece: " << troca->get_anuncio_ofertado()->get_produto()->get_nome() << endl;
    cout << "Mensagem: \"" << troca->get_mensagem() << "\"" << endl;
    cout << "\n1 - Aceitar\n2 - Recusar\n3 - Decidir Depois\nOpcao: ";
}

// ==========================================
// FEEDBACK
// ==========================================

void UIOutput::mensagem(const string& texto) {
    cout << texto << endl;
}

void UIOutput::erro(const string& texto) {
    cout << "[Erro] " << texto << endl;
}