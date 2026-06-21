#include "../include/Produto.hpp"
#include <utility>

// REVISÃO: idem ao Usuario - parâmetros de string por valor + std::move no
// initializer list, em vez de copiar cada std::string de novo pro atributo.
Produto::Produto(std::string id, std::string nome, double preco, std::string categoria, std::string subcategoria, std::string loginAnunciante, bool ativo, int quantidadeEstoque)
    : _id(std::move(id)), _nome(std::move(nome)), _preco(preco), _categoria(std::move(categoria)),
      _subcategoria(std::move(subcategoria)), _loginAnunciante(std::move(loginAnunciante)),
      _ativo(ativo), _quantidadeEstoque(quantidadeEstoque) {}

const std::string& Produto::get_id() const { return _id; }
const std::string& Produto::get_nome() const { return _nome; }
double Produto::get_preco() const { return _preco; }
const std::string& Produto::get_categoria() const { return _categoria; }
const std::string& Produto::get_subcategoria() const { return _subcategoria; }
const std::string& Produto::get_login_anunciante() const { return _loginAnunciante; }
bool Produto::is_ativo() const { return _ativo; }
int Produto::get_quantidade_estoque() const { return _quantidadeEstoque; }

void Produto::set_nome(const std::string& nome) { _nome = nome; }
void Produto::set_preco(double preco) { _preco = preco; }
void Produto::set_categoria(const std::string& categoria) { _categoria = categoria; }
void Produto::set_subcategoria(const std::string& subcategoria) { _subcategoria = subcategoria; }
void Produto::set_ativo(bool ativo) { _ativo = ativo; }
void Produto::set_quantidade_estoque(int quantidade) { _quantidadeEstoque = quantidade; }

// Soft delete por estoque: em vez de remover o Produto da lista (o que invalidaria
// qualquer ponteiro/Anuncio que já apontava pra ele), só desligamos a flag _ativo.
// Assim o histórico de transações antigas continua com um Produto* válido pra
// consultar, e a vitrine simplesmente filtra quem está ativo.
bool Produto::baixarEstoque(int qtd) {
    // Defesa contra quantidade inválida ou venda maior que o estoque disponível -
    // sem isso o estoque podia ficar negativo, o que não faz sentido nenhum.
    if (qtd <= 0 || qtd > _quantidadeEstoque) return false;

    _quantidadeEstoque -= qtd;
    if (_quantidadeEstoque <= 0) {
        _quantidadeEstoque = 0;
        _ativo = false; // último do estoque: produto sai de circulação automaticamente
    }
    return true;
}
