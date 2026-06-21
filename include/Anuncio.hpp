#ifndef ANUNCIO_HPP
#define ANUNCIO_HPP

#include <string>
#include "Produto.hpp"
#include "Usuario.hpp"

// Anuncio é uma camada fina entre o Produto "puro" e as Transacoes (Compra/
// Troca): ele é quem efetivamente participa de uma negociação, guardando a
// quantidade envolvida e se está ativo pra negociar. _produto e _vendedor são
// ponteiros NÃO donos (non-owning) - quem possui essas instâncias de verdade
// são os vetores dentro de GerenciadorProdutos/GerenciadorUsuarios. O Anuncio
// só aponta pra lá, então nunca deve dar delete nesses ponteiros.
class Anuncio {
private:
    int _idAnuncio;
    Produto* _produto;
    Usuario* _vendedor;
    int _quantidade;
    bool _estaAtivo;

public:
    // Apenas as declarações terminando em ponto e vírgula!
    Anuncio(int id, Produto* produto, Usuario* vendedor, int quantidade);

    void set_status(bool ativo);
    bool get_status() const;
    Produto* get_produto() const;
    Usuario* get_vendedor() const;
    int get_quantidade() const;
    bool validar_anuncio() const;
};

#endif
