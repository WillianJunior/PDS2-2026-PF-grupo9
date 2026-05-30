#include "../include/ItemVendido.h"

// Construtor: Inicializa a "fotografia" do produto no momento da venda
ItemVendido::ItemVendido(int id, std::string nome, double preco, int qtd)
    : _idProduto(id), _nomeProduto(nome), _precoNaVenda(preco), _quantidade(qtd) {}

int ItemVendido::get_idProduto() const {
    return _idProduto;
}

std::string ItemVendido::get_nomeProduto() const {
    return _nomeProduto;
}

double ItemVendido::get_precoNaVenda() const {
    return _precoNaVenda;
}

int ItemVendido::get_quantidade() const {
    return _quantidade;
}

// Calcula o valor total deste item específico (Preço fixado x Quantidade)
double ItemVendido::get_subtotal() const {
    return _precoNaVenda * _quantidade;
}