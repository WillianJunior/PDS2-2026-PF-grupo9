#include "../include/GerenciadorProdutos.h"
#include <fstream>
#include <sstream>

GerenciadorProdutos::GerenciadorProdutos() {
    carregarProdutos(); // Carrega os produtos assim que o sistema inicia
}

void GerenciadorProdutos::carregarProdutos() {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        return; // Se o arquivo não existir ainda, apenas ignora
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string idStr, nome, precoStr;

        // Lê separando por vírgula
        std::getline(ss, idStr, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, precoStr, ',');

        if (!idStr.empty() && !nome.empty() && !precoStr.empty()) {
            // Converte os textos para os tipos corretos (int e double)
            int id = std::stoi(idStr);
            double preco = std::stod(precoStr);
            
            _produtos.emplace_back(id, nome, preco);
        }
    }
    arquivo.close();
}

void GerenciadorProdutos::salvarProdutoNoArquivo(const Produto& produto) {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::app); // ios::app para adicionar no fim
    if (arquivo.is_open()) {
        arquivo << produto.get_id() << ","
                << produto.get_nome() << ","
                << produto.get_preco() << "\n";
        arquivo.close();
    }
}

void GerenciadorProdutos::cadastrarProduto(int id, const std::string& nome, double preco) {
    Produto novo(id, nome, preco);
    _produtos.push_back(novo);
    salvarProdutoNoArquivo(novo);
}

const std::vector<Produto>& GerenciadorProdutos::get_produtos() const {
    return _produtos;
}

Produto* GerenciadorProdutos::buscarProdutoPorId(int id) {
    for (auto& prod : _produtos) {
        if (prod.get_id() == id) {
            return &prod; // Retorna o ponteiro do produto encontrado
        }
    }
    return nullptr; // Se não achar, retorna ponteiro nulo
}