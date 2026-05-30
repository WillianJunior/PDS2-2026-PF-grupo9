#include "../include/produto.h"

Produto::Produto(int id, std::string nome, double preco)
    : _id(id), _nome(nome), _preco(preco) {}

int Produto::get_id() const {
    return _id;
}

std::string Produto::get_nome() const {
    return _nome;
}

double Produto::get_preco() const {
    return _preco;
}