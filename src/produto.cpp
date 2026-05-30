#include "../include/produto.h"

Produto::Produto(int id, std::string nome, double preco, std::string categoria, std::string subcategoria)
    : _id(id), _nome(nome), _preco(preco), _categoria(categoria), _subcategoria(subcategoria) {}

int Produto::get_id() const { return _id; }
std::string Produto::get_nome() const { return _nome; }
double Produto::get_preco() const { return _preco; }
std::string Produto::get_categoria() const { return _categoria; }
std::string Produto::get_subcategoria() const { return _subcategoria; }