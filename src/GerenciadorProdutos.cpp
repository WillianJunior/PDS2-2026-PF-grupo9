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
        std::string idStr, nome, precoStr, categoria, subcategoria;

        // Lê separando por vírgula
        std::getline(ss, idStr, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, precoStr, ',');
        std::getline(ss, categoria, ',');   
        std::getline(ss, subcategoria, ','); 

        if (!idStr.empty() && !nome.empty() && !precoStr.empty()) {
            int id = std::stoi(idStr);
            double preco = std::stod(precoStr);
            
            _produtos.emplace_back(id, nome, preco, categoria, subcategoria);
        }
    }
    arquivo.close();
}

void GerenciadorProdutos::salvarProdutoNoArquivo(const Produto& produto) {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::app);
    if (arquivo.is_open()) {
        arquivo << produto.get_id() << ","
                << produto.get_nome() << ","
                << produto.get_preco() << ","
                << produto.get_categoria() << ","     // <-- Salva a categoria com vírgula
                << produto.get_subcategoria() << "\n"; // <-- Salva a subcategoria com quebra de linha
        arquivo.close();
    }
}

void GerenciadorProdutos::cadastrarProduto(int id, const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria) {
    Produto novo(id, nome, preco, categoria, subcategoria);
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