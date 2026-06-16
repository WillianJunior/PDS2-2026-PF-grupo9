#ifndef TROCA_HPP
#define TROCA_HPP

#include "Transacao.hpp"
#include "Anuncio.hpp"
#include <cmath>
#include <algorithm>

class Troca : public Transacao {
private:
    Anuncio* _anuncioAlvo;       
    Anuncio* _anuncioOfertado;   
    int _quantidadeAlvo;
    int _quantidadeOfertada;
    double _margemTolerancia;    

public:
    Troca(const std::string& id, Usuario* proponente, Usuario* receptor, 
          Anuncio* alvo, Anuncio* ofertado, int qtdAlvo = 1, int qtdOfertada = 1, double margem = 0.20);

    Anuncio* get_anuncio_alvo() const;
    Anuncio* get_anuncio_ofertado() const;
    int get_quantidade_alvo() const;
    int get_quantidade_ofertada() const;

    bool verificar_precos_similares() const;
    bool validar_transacao() const override;
    void executar_transacao() override;
};

#endif