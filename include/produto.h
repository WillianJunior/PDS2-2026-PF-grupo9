#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>

class Produto {
private:
    int _id;
    std::string _nome;
    double _preco;

public:
    Produto(int id, std::string nome, double preco);

    int get_id() const; // <-- Adicionado: essencial para o arquivo!
    std::string get_nome() const;
    double get_preco() const;
};

#endif