#include "../include/produto.h"

Produto::Produto(std::string id, std::string nome, double preco, std::string categoria, std::string subcategoria, std::string loginAnunciante)
    : _id(id), _nome(nome), _preco(preco), _categoria(categoria), _subcategoria(subcategoria), _loginAnunciante(loginAnunciante) {}

std::string Produto::get_id() const { return _id; }
std::string Produto::get_nome() const { return _nome; }
double Produto::get_preco() const { return _preco; }
std::string Produto::get_categoria() const { return _categoria; }
std::string Produto::get_subcategoria() const { return _subcategoria; }
std::string Produto::get_login_anunciante() const { return _loginAnunciante; }

void Produto::set_nome(const std::string& nome) { _nome = nome; }
void Produto::set_preco(double preco) { _preco = preco; }
void Produto::set_categoria(const std::string& categoria) { _categoria = categoria; }
void Produto::set_subcategoria(const std::string& subcategoria) { _subcategoria = subcategoria; }