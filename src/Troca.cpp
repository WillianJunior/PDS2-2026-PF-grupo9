#include "../include/Troca.hpp"
#include <cmath>
#include <algorithm>

Troca::Troca(const std::string& id, Usuario* proponente, Usuario* receptor, 
             Anuncio* alvo, Anuncio* ofertado, int qtdAlvo, int qtdOfertada, double margem)
    : Transacao(id, proponente, receptor), 
      _anuncioAlvo(alvo), 
      _anuncioOfertado(ofertado),
      _quantidadeAlvo(qtdAlvo), 
      _quantidadeOfertada(qtdOfertada), 
      _margemTolerancia(margem) {}

Anuncio* Troca::get_anuncio_alvo() const { return _anuncioAlvo; }

Anuncio* Troca::get_anuncio_ofertado() const { return _anuncioOfertado; }

int Troca::get_quantidade_alvo() const { return _quantidadeAlvo; }

int Troca::get_quantidade_ofertada() const { return _quantidadeOfertada; }

bool Troca::verificar_precos_similares() const {
    if (!_anuncioAlvo || !_anuncioOfertado) return false;
    if (!_anuncioAlvo->get_produto() || !_anuncioOfertado->get_produto()) return false;

    double valorTotalAlvo = _anuncioAlvo->get_produto()->get_preco() * _quantidadeAlvo;
    double valorTotalOfertado = _anuncioOfertado->get_produto()->get_preco() * _quantidadeOfertada;

    if (valorTotalAlvo == 0.0 || valorTotalOfertado == 0.0) return false;

    double diferencaAbsoluta = std::abs(valorTotalAlvo - valorTotalOfertado);
    double maiorValor = std::max(valorTotalAlvo, valorTotalOfertado);
    
    // Verifica se a proporção da diferença está dentro do limite aceitável (ex: 20%)
    return (diferencaAbsoluta / maiorValor) <= _margemTolerancia;
}

bool Troca::validar_transacao() const {
    // 1. Verifica se os anúncios existem e estão ativos
    if (!_anuncioAlvo || !_anuncioOfertado) return false;
    if (!_anuncioAlvo->get_status() || !_anuncioOfertado->get_status()) return false;
    
    // 2. Verifica se há estoque suficiente para ambas as partes
    if (_anuncioAlvo->get_quantidade() < _quantidadeAlvo) return false;
    if (_anuncioOfertado->get_quantidade() < _quantidadeOfertada) return false;
    
    // 3. Verifica a regra de negócio do e-scambo (preços similares)
    return verificar_precos_similares();
}

void Troca::executar_transacao() {
    if (validar_transacao()) {
        _status = StatusTransacao::CONCLUIDA;
        // A lógica de transferir a posse dos produtos e fechar os anúncios
        // deve ser tratada pelo GerenciadorProdutos/GerenciadorUsuarios posteriormente.
    } else {
        _status = StatusTransacao::REJEITADA;
    }
}