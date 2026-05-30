#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>

class Produto {
private:
    std::string _id; // <-- AGORA É STRING! Ex: "0000100001"
    std::string _nome;
    double _preco;
    std::string _categoria;
    std::string _subcategoria;
    std::string _loginAnunciante;

public:
    Produto(std::string id, std::string nome, double preco, std::string categoria, std::string subcategoria, std::string loginAnunciante);

    std::string get_id() const; // <-- AGORA RETORNA STRING
    std::string get_nome() const;
    double get_preco() const;
    std::string get_categoria() const;
    std::string get_subcategoria() const;
    std::string get_login_anunciante() const;

    void set_nome(const std::string& nome);
    void set_preco(double preco);
    void set_categoria(const std::string& categoria);
    void set_subcategoria(const std::string& subcategoria);
};

#endif