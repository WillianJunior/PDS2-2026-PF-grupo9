#ifndef TROCA_HPP
#define TROCA_HPP

#include "Transacao.hpp"
#include "Anuncio.hpp"
#include <cmath>
#include <algorithm>

// Implementação concreta de Transacao pro E-scambo (troca de produtos sem
// dinheiro envolvido). Esta é a única das duas subclasses que está de fato
// integrada na ponta da UI (ver SistemaEscambo::enviarPropostaTroca /
// processarRespostaTroca) - diferente da Compra, que ainda roda em paralelo
// por fora dessa hierarquia (ver nota em Compra.hpp).
class Troca : public Transacao {
private:
    Anuncio* _anuncioAlvo;
    Anuncio* _anuncioOfertado;
    int _quantidadeAlvo;
    int _quantidadeOfertada;
    double _margemTolerancia;

public:
    Troca(std::string id, Usuario* proponente, Usuario* receptor,
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
