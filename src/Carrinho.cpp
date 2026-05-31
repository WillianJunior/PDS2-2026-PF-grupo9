#include "../include/Carrinho.h"
#include <fstream>
#include <sstream>

void Carrinho::adicionarItem(const ItemVendido& item) {
    _itens.push_back(item);
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

void Carrinho::esvaziar() {
    _itens.clear();
}

void Carrinho::salvarCarrinho(const std::string& loginUsuario) const {
    // Cria um arquivo único para o usuário (ex: carrinho_igor@email.txt)
    std::string nomeArquivo = "carrinho_" + loginUsuario + ".txt";
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
    
    std::string nomeArquivo = "carrinho_" + loginUsuario + ".txt";
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