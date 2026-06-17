#ifndef PRODUTO_HPP
#define PRODUTO_HPP

#include <string>

class Produto {
private:
    std::string _id;
    std::string _nome;
    double _preco;
    std::string _categoria;
    std::string _subcategoria;
    std::string _loginAnunciante;
    bool _ativo; 

public:
    // O '= true' faz com que não precisemos mudar o código onde o Produto é criado
    Produto(std::string id, std::string nome, double preco, std::string categoria, std::string subcategoria, std::string loginAnunciante, bool ativo = true);

    std::string get_id() const;
    std::string get_nome() const;
    double get_preco() const;
    std::string get_categoria() const;
    std::string get_subcategoria() const;
    std::string get_login_anunciante() const;
    bool is_ativo() const; // <--- NOVO GETTER

    void set_nome(const std::string& nome);
    void set_preco(double preco);
    void set_categoria(const std::string& categoria);
    void set_subcategoria(const std::string& subcategoria);
    void set_ativo(bool ativo); // <--- NOVO SETTER
};

#endif