/**
 * @file Produto.h
 * @brief Definição da classe Produto para o sistema de e-commerce.
 */

#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>

/**
 * @class Produto
 * @brief Classe que representa um item anunciado no sistema.
 */
class Produto {
private:
    int _id;
    std::string _nome;
    double _preco;

public:
    /**
     * @brief Construtor da classe Produto.
     * @param id Identificador único.
     * @param nome Nome do item.
     * @param preco Valor de venda.
     */
    Produto(int id, std::string nome, double preco);

    std::string get_nome() const;
    double get_preco() const;
};

#endif