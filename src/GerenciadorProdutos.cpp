#include "../include/GerenciadorProdutos.hpp"
#include <fstream>
#include <sstream>
#include <iomanip> // <-- Biblioteca necessária para preencher os zeros!

GerenciadorProdutos::GerenciadorProdutos() {
    carregarProdutos();
}

void GerenciadorProdutos::carregarProdutos() {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string idStr, nome, precoStr, categoria, subcategoria, loginDono;

        std::getline(ss, idStr, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, precoStr, ',');
        std::getline(ss, categoria, ',');
        std::getline(ss, subcategoria, ',');
        std::getline(ss, loginDono, ',');

        if (!idStr.empty() && !nome.empty() && !precoStr.empty()) {
            double preco = std::stod(precoStr);
            // O ID já vem como string do arquivo
            _produtos.emplace_back(idStr, nome, preco, categoria, subcategoria, loginDono);
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
                << produto.get_categoria() << ","
                << produto.get_subcategoria() << ","
                << produto.get_login_anunciante() << "\n";
        arquivo.close();
    }
}

void GerenciadorProdutos::atualizarArquivoCompleto() {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::trunc); 
    if (arquivo.is_open()) {
        for (const auto& produto : _produtos) {
            arquivo << produto.get_id() << ","
                    << produto.get_nome() << ","
                    << produto.get_preco() << ","
                    << produto.get_categoria() << ","
                    << produto.get_subcategoria() << ","
                    << produto.get_login_anunciante() << "\n";
        }
        arquivo.close();
    }
}

void GerenciadorProdutos::cadastrarProduto(const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria, const std::string& loginAnunciante, int idUsuarioLogado) {
    
    // A MÁGICA ACONTECE AQUI:
    int idProdutoSequencial = _produtos.size() + 1;

    std::ostringstream geradorId;
    // Preenche com zeros à esquerda até dar 5 casas
    geradorId << std::setfill('0') << std::setw(5) << idUsuarioLogado 
              << std::setw(5) << idProdutoSequencial;
    
    std::string idFinalGerado = geradorId.str(); // Ex: "0000100001"

    Produto novo(idFinalGerado, nome, preco, categoria, subcategoria, loginAnunciante);
    _produtos.push_back(novo);
    salvarProdutoNoArquivo(novo);
}

bool GerenciadorProdutos::editarProduto(const std::string& id, const std::string& novoNome, double novoPreco, const std::string& novaCat, const std::string& novaSub) {
    for (auto& p : _produtos) {
        if (p.get_id() == id) {
            p.set_nome(novoNome);
            p.set_preco(novoPreco);
            p.set_categoria(novaCat);
            p.set_subcategoria(novaSub);
            
            atualizarArquivoCompleto(); 
            return true; 
        }
    }
    return false; 
}

const std::vector<Produto>& GerenciadorProdutos::get_produtos() const {
    return _produtos;
}

Produto* GerenciadorProdutos::buscarProdutoPorId(const std::string& id) {
    for (auto& prod : _produtos) {
        if (prod.get_id() == id) {
            return &prod; 
        }
    }
    return nullptr; 
}