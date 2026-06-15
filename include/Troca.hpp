#ifndef TROCA_HPP
#define TROCA_HPP

#include "transacao.hpp"
#include "anuncio.hpp"
#include <cmath>
#include <algorithm>

class Troca : public Transacao {
private:
    Anuncio* _anuncioAlvo;       // O produto do receptor que o proponente deseja obter
    Anuncio* _anuncioOfertado;   // O produto do proponente oferecido como moeda de troca
    int _quantidadeAlvo;
    int _quantidadeOfertada;
    double _margemTolerancia;    // Exemplo: 0.20 significa tolerar até 20% de diferença de preço

public:
    Troca(const std::string& id, Usuario* proponente, Usuario* receptor, 
          Anuncio* alvo, Anuncio* ofertado, int qtdAlvo = 1, int qtdOfertada = 1, double margem = 0.20)
        : Transacao(id, proponente, receptor), 
          _anuncioAlvo(alvo), 
          _anuncioOfertado(ofertado),
          _quantidadeAlvo(qtdAlvo), 
          _quantidadeOfertada(qtdOfertada), 
          _margemTolerancia(margem) {}

    Anuncio* get_anuncio_alvo() const { return _anuncioAlvo; }
    Anuncio* get_anuncio_ofertado() const { return _anuncioOfertado; }
    int get_quantidade_alvo() const { return _quantidadeAlvo; }
    int get_quantidade_ofertada() const { return _quantidadeOfertada; }

    // Validação matemática do critério de preço similar para o escambo
    bool verificar_precos_similares() const {
        if (!_anuncioAlvo || !_anuncioOfertado) return false;
        if (!_anuncioAlvo->get_produto() || !_anuncioOfertado->get_produto()) return false;

        double valorTotalAlvo = _anuncioAlvo->get_produto()->get_preco() * _quantidadeAlvo;
        double valorTotalOfertado = _anuncioOfertado->get_produto()->get_preco() * _quantidadeOfertada;

        if (valorTotalAlvo == 0.0 || valorTotalOfertado == 0.0) return false;

        double diferencaAbsoluta = std::abs(valorTotalAlvo - valorTotalOfertado);
        double maiorValor = std::max(valorTotalAlvo, valorTotalOfertado);
        
        // Verifica se a proporção da diferença está dentro do limite aceitável
        return (diferencaAbsoluta / maiorValor) <= _margemTolerancia;
    }

    // Validação completa incluindo status e limites físicos de estoque
    bool validar_transacao() const override {
        if (!_anuncioAlvo || !_anuncioOfertado) return false;
        if (!_anuncioAlvo->get_status() || !_anuncioOfertado->get_status()) return false;
        
        if (_anuncioAlvo->get_quantidade() < _quantidadeAlvo) return false;
        if (_anuncioOfertado->get_quantidade() < _quantidadeOfertada) return false;
        
        return verificar_precos_similares();
    }

    void executar_transacao() override {
        if (validar_transacao()) {
            _status = StatusTransacao::CONCLUIDA;
        } else {
            _status = StatusTransacao::REJEITADA;
        }
    }
};

#endif