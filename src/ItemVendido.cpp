#include "../include/ItemVendido.hpp"
#include <utility>

// REVISÃO: std::move nas strings do initializer list (mesmo padrão usado em
// Produto/Usuario) pra evitar copiar id e nome do produto de novo aqui dentro.
ItemVendido::ItemVendido(std::string id, std::string nome, double preco, int qtd)
    : _idProduto(std::move(id)), _nomeProduto(std::move(nome)), _precoNaVenda(preco), _quantidade(qtd) {}

const std::string& ItemVendido::get_idProduto() const { return _idProduto; }
const std::string& ItemVendido::get_nomeProduto() const { return _nomeProduto; }
double ItemVendido::get_precoNaVenda() const { return _precoNaVenda; }
int ItemVendido::get_quantidade() const { return _quantidade; }

double ItemVendido::get_subtotal() const {
    return _precoNaVenda * _quantidade;
}
