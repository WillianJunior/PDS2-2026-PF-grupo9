#include "../include/Carrinho.hpp"
#include <fstream>
#include <sstream>
#include <utility>
#include <filesystem>

void Carrinho::adicionarItem(ItemVendido item) {
    _itens.push_back(std::move(item));
}

const std::vector<ItemVendido>& Carrinho::get_itens() const {
    return _itens;
}

double Carrinho::get_total() const {
    double total = 0.0;
    for (const auto& item : _itens) {
        total += item.get_subtotal();
    }
    return total;
}

void Carrinho::removerItem(size_t indice) {
    // Guarda contra índice inválido: como o índice vem de uma escolha do
    // usuário no menu (TerminalUI), é melhor ignorar silenciosamente um
    // índice fora do intervalo do que arriscar um acesso indevido ao vetor.
    if (indice < _itens.size()) {
        _itens.erase(_itens.begin() + indice);
    }
}

void Carrinho::esvaziar() {
    _itens.clear();
}

void Carrinho::salvarCarrinho(const std::string& loginUsuario) const {
    std::filesystem::create_directories("data");
    std::string nomeArquivo = "data/carrinho_" + loginUsuario + ".txt";
    std::ofstream arquivo(nomeArquivo, std::ios::trunc); // Sobrescreve o arquivo com os itens atuais

    if (arquivo.is_open()) {
        for (const auto& item : _itens) {
            arquivo << item.get_idProduto() << ","
                    << item.get_nomeProduto() << ","
                    << item.get_precoNaVenda() << ","
                    << item.get_quantidade() << "\n";
        }
        arquivo.close();
    }
}

void Carrinho::carregarCarrinho(const std::string& loginUsuario) {
    esvaziar(); // Limpa a memória para não duplicar itens ao carregar

    // Se o arquivo não existir (usuário nunca salvou um carrinho antes), o
    // ifstream simplesmente não abre e o laço abaixo nem executa - o carrinho
    // fica vazio, sem precisar de nenhum tratamento de erro especial aqui.

    std::string nomeArquivo = "data/carrinho_" + loginUsuario + ".txt";
    std::ifstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        std::string linha;
        while (std::getline(arquivo, linha)) {
            std::stringstream ss(linha);
            std::string id, nome, precoStr, qtdStr;

            std::getline(ss, id, ',');
            std::getline(ss, nome, ',');
            std::getline(ss, precoStr, ',');
            std::getline(ss, qtdStr, ',');

            if (!id.empty() && !nome.empty() && !precoStr.empty() && !qtdStr.empty()) {
                double preco = std::stod(precoStr);
                int qtd = std::stoi(qtdStr);
                adicionarItem(ItemVendido(id, nome, preco, qtd));
            }
        }
        arquivo.close();
    }
}