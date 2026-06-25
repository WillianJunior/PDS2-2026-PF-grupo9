#include "../include/TerminalView.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
// Primitivas
// ============================================================

void TerminalView::limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void TerminalView::pausar() {
    cout << "\nPressione Enter para continuar...";
    lerLinha();
}

void TerminalView::mensagem(const string& texto) {
    cout << texto << endl;
}

void TerminalView::erro(const string& texto) {
    cout << "[Erro] " << texto << endl;
}

// Lê uma linha inteira e tenta converter para int.
// Imprime mensagem de erro e repete para qualquer entrada não-numérica ou vazia.
int TerminalView::lerOpcaoInteira() {
    string linha;
    while (true) {
        if (!getline(cin, linha)) return 0; // EOF — encerra graciosamente
        if (!linha.empty()) {
            try {
                size_t pos;
                int valor = stoi(linha, &pos);
                if (pos == linha.size()) return valor;
            } catch (...) {}
        }
        cout << "[Erro] Entrada invalida. Digite um numero:" << endl;
    }
}

double TerminalView::lerValorDouble() {
    string linha;
    while (true) {
        if (!getline(cin, linha)) return 0.0;
        if (!linha.empty()) {
            try {
                size_t pos;
                double valor = stod(linha, &pos);
                if (pos == linha.size()) return valor;
            } catch (...) {}
        }
        cout << "[Erro] Entrada invalida. Digite um numero:" << endl;
    }
}

string TerminalView::lerLinha() {
    string linha;
    getline(cin, linha);
    return linha;
}

// ============================================================
// Menus (só display — sem leitura)
// ============================================================

void TerminalView::exibirMenuPrincipal() {
    cout << "\n=== E-scambo Terminal ===" << endl;
    cout << "1 - Fazer Login" << endl;
    cout << "2 - Cadastrar Novo Usuario" << endl;
    cout << "3 - Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void TerminalView::exibirMenuPerfil(const string& nomeUsuario) {
    cout << "\nBem-vindo(a), " << nomeUsuario << "!" << endl;
    cout << "Selecione o seu perfil de acesso:" << endl;
    cout << "1 - Comprador" << endl;
    cout << "2 - Anunciante" << endl;
    cout << "3 - Sair da Conta (Logout)" << endl;
    cout << "Escolha uma opcao: ";
}

void TerminalView::exibirMenuAnunciante(bool podeAnunciar, int anunciosAtivos, int limiteAnuncios) {
    cout << "\n=== Painel do Anunciante ===" << endl;
    if (!podeAnunciar) {
        cout << "1 - Cadastrar Novo Produto (LIMITE ATINGIDO: "
             << anunciosAtivos << "/" << limiteAnuncios << ")" << endl;
    } else if (limiteAnuncios < numeric_limits<int>::max()) {
        cout << "1 - Cadastrar Novo Produto (" << anunciosAtivos << "/" << limiteAnuncios << " ativos)" << endl;
    } else {
        cout << "1 - Cadastrar Novo Produto" << endl;
    }
    cout << "2 - Ver Minha Vitrine" << endl;
    cout << "3 - Editar um Produto" << endl;
    cout << "4 - Notificacoes" << endl;
    cout << "5 - Voltar" << endl;
    cout << "Escolha uma opcao: ";
}

void TerminalView::exibirMenuComprador(const string& nomeUsuario, bool podeTrocar) {
    cout << "\n=== Area de Compras ===" << endl;
    cout << "Bem-vindo(a), " << nomeUsuario << "!" << endl;
    cout << "1 - Ver Vitrine Global / Pesquisar" << endl;
    cout << "2 - Meu Carrinho" << endl;
    if (podeTrocar) {
        cout << "3 - Propor Troca" << endl;
        cout << "4 - Voltar" << endl;
    } else {
        cout << "3 - Voltar" << endl;
    }
    cout << "Escolha uma opcao: ";
}

void TerminalView::exibirMenuCarrinho(const vector<ItemVendido>& itens, double total) {
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

void TerminalView::exibirMenuBusca() {
    cout << "=== Menu de Busca de Produtos ===" << endl;
    cout << "1 - Ver TODOS os produtos disponiveis" << endl;
    cout << "2 - Pesquisar por Categoria e Subcategoria" << endl;
    cout << "3 - Pesquisar por Nome" << endl;
    cout << "Escolha uma opcao: ";
}

void TerminalView::exibirMenuEdicaoProduto(const string& nomeAtual, double precoAtual,
                                            const string& catAtual, const string& subAtual) {
    cout << "\n--- Editando: " << nomeAtual << " ---" << endl;
    cout << "1 - Editar Nome (Atual: " << nomeAtual << ")" << endl;
    cout << "2 - Editar Preco (Atual: R$ " << precoAtual << ")" << endl;
    cout << "3 - Editar Categoria/Subcategoria (Atual: " << catAtual << " > " << subAtual << ")" << endl;
    cout << "4 - Salvar alteracoes e Voltar" << endl;
    cout << "5 - Cancelar sem salvar" << endl;
    cout << "Escolha o que deseja alterar: ";
}

// ============================================================
// Listagens
// ============================================================

void TerminalView::exibirVitrine(const vector<const Produto*>& produtos) {
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

void TerminalView::exibirMeusProdutos(const vector<const Produto*>& produtos) {
    cout << "\n--- Minha Vitrine ---" << endl;
    if (produtos.empty()) {
        cout << "Voce nao tem nenhum produto ativo na vitrine no momento." << endl;
    } else {
        for (const auto* p : produtos) {
            cout << "ID: " << p->get_id()
                 << " | Nome: " << p->get_nome()
                 << "\n  Categoria: " << p->get_categoria() << " > " << p->get_subcategoria()
                 << "\n  Preco: R$ " << p->get_preco()
                 << "\n  Estoque: " << p->get_quantidade_estoque() << "\n" << endl;
        }
    }
}

void TerminalView::exibirProdutosParaOferta(const vector<Produto*>& produtos) {
    cout << "\nSeus produtos disponiveis para oferecer:\n" << endl;
    for (size_t i = 0; i < produtos.size(); ++i) {
        cout << "(" << i + 1 << ") " << produtos[i]->get_nome()
             << " | R$ " << produtos[i]->get_preco()
             << " | ID: " << produtos[i]->get_id() << endl;
    }
}

void TerminalView::exibirPropostaTroca(const Troca* troca) {
    cout << "\n[NOVA PROPOSTA DE E-SCAMBO]" << endl;
    cout << "De: " << troca->get_usuario_proponente()->getNome() << endl;
    cout << "Quer : " << troca->get_anuncio_alvo()->get_produto()->get_nome() << endl;
    cout << "Oferece: " << troca->get_anuncio_ofertado()->get_produto()->get_nome() << endl;
    cout << "Mensagem: \"" << troca->get_mensagem() << "\"" << endl;
    cout << "\n1 - Aceitar\n2 - Recusar\n3 - Decidir Depois\nOpcao: ";
}

// ============================================================
// Formulários combinados
// ============================================================

TerminalView::DadosCadastro TerminalView::solicitarDadosCadastro() {
    DadosCadastro d;
    cout << "Digite seu Nome: ";
    d.nome = lerLinha();
    cout << "Digite seu E-mail (login): ";
    d.login = lerLinha();
    cout << "Digite sua Senha: ";
    d.senha = lerLinha();
    cout << "Digite sua Chave PIX (Enter para deixar em branco): ";
    d.chavePix = lerLinha();
    return d;
}

pair<string, string> TerminalView::solicitarCredenciaisLogin() {
    cout << "Digite seu e-mail (login): ";
    string login = lerLinha();
    cout << "Digite sua senha: ";
    string senha = lerLinha();
    return {login, senha};
}

// Árvore de subcategorias — só formata o que mostrar, sem decisão de fluxo.
string TerminalView::solicitarSubcategoria(const string& categoria) {
    if (categoria == "Veiculo") {
        cout << "1 - Carro\n2 - Moto\n3 - Caminhao\nOpcao: ";
        int op = lerOpcaoInteira();
        if (op == 1) return "Carro";
        if (op == 2) return "Moto";
        return "Caminhao";
    }
    if (categoria == "Eletrodomestico") {
        cout << "1 - Cozinha\n2 - Quarto\n3 - Escritorio\n4 - Gamer\nOpcao: ";
        int op = lerOpcaoInteira();
        if (op == 1) return "Cozinha";
        if (op == 2) return "Quarto";
        if (op == 3) return "Escritorio";
        return "Gamer";
    }
    if (categoria == "Roupa") {
        cout << "1 - Camisa\n2 - Calca\n3 - Tenis\nOpcao: ";
        int op = lerOpcaoInteira();
        if (op == 1) return "Camisa";
        if (op == 2) return "Calca";
        return "Tenis";
    }
    return "Geral"; // "Outros"
}

TerminalView::DadosNovoProduto TerminalView::solicitarDadosNovoProduto() {
    DadosNovoProduto d;

    cout << "Digite o nome do produto: ";
    d.nome = lerLinha();

    cout << "Digite o preco do produto: R$ ";
    d.preco = lerValorDouble();

    cout << "\nEscolha a Categoria:\n1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
    int opcaoCat = lerOpcaoInteira();
    if (opcaoCat == 1)      d.categoria = "Veiculo";
    else if (opcaoCat == 2) d.categoria = "Eletrodomestico";
    else if (opcaoCat == 3) d.categoria = "Roupa";
    else                    d.categoria = "Outros";

    if (d.categoria == "Outros") {
        d.subcategoria = "Geral";
    } else {
        cout << "\nEscolha a Subcategoria:\n";
        d.subcategoria = solicitarSubcategoria(d.categoria);
    }

    cout << "\nDigite a quantidade em estoque: ";
    d.quantidade = lerOpcaoInteira();
    if (d.quantidade <= 0) d.quantidade = 1;

    return d;
}

pair<string, string> TerminalView::solicitarNovaCategoriaSubcategoria() {
    cout << "\nEscolha a NOVA Categoria:\n1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
    int opcaoCat = lerOpcaoInteira();
    string cat;
    if (opcaoCat == 1)      cat = "Veiculo";
    else if (opcaoCat == 2) cat = "Eletrodomestico";
    else if (opcaoCat == 3) cat = "Roupa";
    else                    cat = "Outros";

    string sub;
    if (cat == "Outros") {
        sub = "Geral";
    } else {
        cout << "\nEscolha a NOVA Subcategoria:\n";
        sub = solicitarSubcategoria(cat);
    }
    return {cat, sub};
}

int TerminalView::solicitarEscolhaProdutoParaEditar(const vector<const Produto*>& produtos) {
    cout << "\nSeus produtos disponiveis:\n" << endl;
    for (size_t i = 0; i < produtos.size(); ++i) {
        cout << "(" << i + 1 << ") " << produtos[i]->get_nome()
             << " | R$ " << produtos[i]->get_preco()
             << " | ID: " << produtos[i]->get_id() << endl;
    }
    cout << "\nDigite o numero do produto que deseja editar (ou 0 para cancelar): ";
    return lerOpcaoInteira();
}

string TerminalView::solicitarNovoNome() {
    cout << "\nDigite o NOVO nome: ";
    return lerLinha();
}

double TerminalView::solicitarNovoPreco() {
    cout << "\nDigite o NOVO preco: R$ ";
    return lerValorDouble();
}

pair<string, string> TerminalView::solicitarFiltroCategoria() {
    cout << "\n--- Selecione a Categoria desejada ---" << endl;
    cout << "1 - Veiculo\n2 - Eletrodomestico\n3 - Roupa\n4 - Outros\nOpcao: ";
    int opcaoCat = lerOpcaoInteira();

    string cat;
    if (opcaoCat == 1)      cat = "Veiculo";
    else if (opcaoCat == 2) cat = "Eletrodomestico";
    else if (opcaoCat == 3) cat = "Roupa";
    else                    cat = "Outros";

    string sub;
    cout << "\nDeseja refinar a busca selecionando uma Subcategoria?\n1 - Sim\n2 - Nao\nOpcao: ";
    int querSub = lerOpcaoInteira();
    if (querSub == 1) {
        cout << "\n--- Selecione a Subcategoria ---" << endl;
        sub = solicitarSubcategoria(cat);
    }
    return {cat, sub};
}

string TerminalView::solicitarFiltroNome() {
    cout << "\n--- Pesquisa por Nome ---" << endl;
    cout << "Digite o nome ou palavra-chave: ";
    return lerLinha();
}

int TerminalView::solicitarEscolhaProdutoCarrinho(int totalProdutos) {
    cout << "---------------------------------" << endl;
    cout << "Digite o NUMERO do produto que deseja adicionar ao carrinho (ou 0 para voltar): ";
    (void)totalProdutos; // só para validação por quem chama
    return lerOpcaoInteira();
}

int TerminalView::solicitarQuantidade() {
    cout << "Digite a quantidade que deseja adicionar ao carrinho: ";
    return lerOpcaoInteira();
}

void TerminalView::exibirItemFinalizacao(const string& nomeProduto, int quantidade, double subtotal,
                                          const string& nomeVendedor, const string& chavePix) {
    cout << "\nProduto: " << nomeProduto
         << " | Qtd: " << quantidade
         << " | Subtotal: R$ " << subtotal << endl;
    cout << "Realize o PIX para " << nomeVendedor
         << " | Chave PIX: " << chavePix << endl;
}

int TerminalView::solicitarItemAlvoTroca(const vector<ItemVendido>& itens) {
    for (size_t i = 0; i < itens.size(); ++i) {
        cout << "(" << i + 1 << ") " << itens[i].get_nomeProduto()
             << " | Qtd: " << itens[i].get_quantidade() << endl;
    }
    cout << "\nDigite o NUMERO do item do carrinho a usar como ALVO da troca (ou 0 para cancelar): ";
    return lerOpcaoInteira();
}

int TerminalView::solicitarProdutoOfertaTroca(const vector<Produto*>& produtos) {
    exibirProdutosParaOferta(produtos);
    cout << "\nDigite o NUMERO do produto que deseja oferecer (ou 0 para cancelar): ";
    return lerOpcaoInteira();
}

string TerminalView::solicitarMensagemTroca() {
    cout << "\nDigite uma mensagem para o vendedor: ";
    return lerLinha();
}
