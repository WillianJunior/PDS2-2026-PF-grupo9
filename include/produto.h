#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>

class Produto {
private:
    int _id;
    std::string _nome;
    double _preco;
    std::string _categoria;     // Guardará textos como "Veiculo"
    std::string _subcategoria;  // Guardará textos como "Carro"

public:
    Produto(int id, std::string nome, double preco, std::string categoria, std::string subcategoria);

    int get_id() const;
    std::string get_nome() const;
    double get_preco() const;
    std::string get_categoria() const;
    std::string get_subcategoria() const;
};

#endif