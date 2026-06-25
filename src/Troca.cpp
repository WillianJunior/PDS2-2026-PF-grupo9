#include "../include/Troca.hpp"
#include <cmath>
#include <algorithm>
#include <utility>

// REVISÃO: id por valor + std::move, igual no resto da hierarquia.
Troca::Troca(std::string id, Usuario* proponente, Usuario* receptor,
             Anuncio* alvo, Anuncio* ofertado, int qtdAlvo, int qtdOfertada, double margem)
    : Transacao(std::move(id), proponente, receptor),
      _anuncioAlvo(alvo),
      _anuncioOfertado(ofertado),
      _quantidadeAlvo(qtdAlvo),
      _quantidadeOfertada(qtdOfertada),
      _margemTolerancia(margem) {}

Anuncio* Troca::get_anuncio_alvo() const { return _anuncioAlvo; }

Anuncio* Troca::get_anuncio_ofertado() const { return _anuncioOfertado; }

int Troca::get_quantidade_alvo() const { return _quantidadeAlvo; }

int Troca::get_quantidade_ofertada() const { return _quantidadeOfertada; }

// Regra de negócio central do E-scambo: dois produtos só podem ser trocados
// se os valores totais envolvidos forem "parecidos" (dentro de uma margem de
// tolerância, 20% por padrão). Sem isso, dava pra trocar um carro por uma
// caneta só porque os dois estão "disponíveis pra troca".
bool Troca::verificar_precos_similares() const {
    if (!_anuncioAlvo || !_anuncioOfertado) return false;
    if (!_anuncioAlvo->get_produto() || !_anuncioOfertado->get_produto()) return false;

    double valorTotalAlvo = _anuncioAlvo->get_produto()->get_preco() * _quantidadeAlvo;
    double valorTotalOfertado = _anuncioOfertado->get_produto()->get_preco() * _quantidadeOfertada;

    // Preço zero indicaria produto mal cadastrado/anúncio inválido - melhor
    // recusar a troca do que dividir por zero na conta da margem abaixo.
    if (valorTotalAlvo == 0.0 || valorTotalOfertado == 0.0) return false;

    double diferencaAbsoluta = std::abs(valorTotalAlvo - valorTotalOfertado);
    double maiorValor = std::max(valorTotalAlvo, valorTotalOfertado);

    // Compara a diferença como PROPORÇÃO do maior valor (e não um valor fixo
    // em R$), assim a mesma margem de 20% funciona tanto pra trocas de R$50
    // quanto de R$50.000.
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
        // Assim como na Compra, quem efetivamente baixa o estoque/inativa os
        // produtos é o Model (SistemaEscambo::processarRespostaTroca) depois
        // que esta função confirma que a transação é válida - a Troca não
        // mexe direto no GerenciadorProdutos.
    } else {
        _status = StatusTransacao::REJEITADA;
    }
}
