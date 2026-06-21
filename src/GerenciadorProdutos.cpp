#include "../include/GerenciadorProdutos.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>

GerenciadorProdutos::GerenciadorProdutos() {
    carregarProdutos();
}

void GerenciadorProdutos::carregarProdutos() {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string idStr, nome, precoStr, categoria, subcategoria, loginDono, ativoStr, estoqueStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, nome, ',');
        std::getline(ss, precoStr, ',');
        std::getline(ss, categoria, ',');
        std::getline(ss, subcategoria, ',');
        std::getline(ss, loginDono, ',');
        std::getline(ss, ativoStr, ','); // coluna opcional (arquivos antigos nao tem)
        std::getline(ss, estoqueStr, ','); // coluna opcional (arquivos antigos nao tem)

        if (!idStr.empty() && !nome.empty() && !precoStr.empty()) {
            double preco = std::stod(precoStr);
            // Compatibilidade com produtos.txt no formato antigo (sem essas
            // duas colunas): se a coluna não existe, assume o valor padrão
            // de sempre - produto ativo, 1 unidade em estoque.
            bool ativo = ativoStr.empty() ? true : (ativoStr != "0");
            int estoque = estoqueStr.empty() ? 1 : std::stoi(estoqueStr);
            // O ID já vem como string do arquivo
            _produtos.emplace_back(idStr, nome, preco, categoria, subcategoria, loginDono, ativo, estoque);
        }
    }
    arquivo.close();
}

void GerenciadorProdutos::salvarProdutoNoArquivo(const Produto& produto) {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::app);
    if (arquivo.is_open()) {
        arquivo << produto.get_id() << ","
                << produto.get_nome() << ","
                << produto.get_preco() << ","
                << produto.get_categoria() << ","
                << produto.get_subcategoria() << ","
                << produto.get_login_anunciante() << ","
                << produto.is_ativo() << ","
                << produto.get_quantidade_estoque() << "\n";
        arquivo.close();
    }
}

// Reescreve o arquivo inteiro a partir do estado atual em memória. Pra escala
// de um projeto acadêmico (algumas centenas de produtos no máximo) isso é
// instantâneo; num sistema real com muito mais dados, faria mais sentido usar
// um banco de dados de verdade (ou atualizar só a linha que mudou num
// formato com tamanho fixo) em vez de reescrever tudo a cada edição/venda.
void GerenciadorProdutos::atualizarArquivoCompleto() {
    std::ofstream arquivo(NOME_ARQUIVO, std::ios::trunc);
    if (arquivo.is_open()) {
        for (const auto& produto : _produtos) {
            arquivo << produto.get_id() << ","
                    << produto.get_nome() << ","
                    << produto.get_preco() << ","
                    << produto.get_categoria() << ","
                    << produto.get_subcategoria() << ","
                    << produto.get_login_anunciante() << ","
                    << produto.is_ativo() << ","
                    << produto.get_quantidade_estoque() << "\n";
        }
        arquivo.close();
    }
}

void GerenciadorProdutos::cadastrarProduto(const std::string& nome, double preco, const std::string& categoria, const std::string& subcategoria, const std::string& loginAnunciante, int idUsuarioLogado, int quantidadeEstoque) {

    // Esquema de ID: 5 dígitos do usuário + 5 dígitos sequenciais do produto
    // (ex: usuário 1, terceiro produto cadastrado -> "0000100003"). Como
    // produto nunca é removido de fato (só inativado/soft delete), o tamanho
    // do deque cresce de forma monotônica e o ID nunca se repete.
    int idProdutoSequencial = _produtos.size() + 1;

    std::ostringstream geradorId;
    geradorId << std::setfill('0') << std::setw(5) << idUsuarioLogado
              << std::setw(5) << idProdutoSequencial;

    std::string idFinalGerado = geradorId.str(); // Ex: "0000100001"

    // REVISÃO: construímos o Produto numa variável local, mas agora movemos
    // ele pro deque (push_back(std::move(novo))) em vez de copiar, e
    // gravamos no arquivo lendo de volta _produtos.back() - mesma lógica
    // aplicada em GerenciadorUsuarios::registrarUsuario, evitando uma cópia
    // inteira do Produto (5 strings + o resto) só pra inserir na coleção.
    Produto novo(idFinalGerado, nome, preco, categoria, subcategoria, loginAnunciante, true, quantidadeEstoque);
    _produtos.push_back(std::move(novo));
    salvarProdutoNoArquivo(_produtos.back());
}

// REVISÃO: editarProduto, inativarProduto e venderProduto reimplementavam,
// cada um, o mesmo loop linear "for (auto& p : _produtos) if (p.get_id() ==
// id)" que o buscarProdutoPorId público já faz. Centralizamos a busca nele -
// elimina a redundância (DRY) e, se um dia a forma de buscar por ID mudar
// (ex: indexar por hash), só precisa mudar buscarProdutoPorId.
bool GerenciadorProdutos::editarProduto(const std::string& id, const std::string& novoNome, double novoPreco, const std::string& novaCat, const std::string& novaSub) {
    Produto* p = buscarProdutoPorId(id);
    if (!p) return false;

    p->set_nome(novoNome);
    p->set_preco(novoPreco);
    p->set_categoria(novaCat);
    p->set_subcategoria(novaSub);

    atualizarArquivoCompleto();
    return true;
}

bool GerenciadorProdutos::inativarProduto(const std::string& id) {
    Produto* p = buscarProdutoPorId(id);
    if (!p) return false;

    p->set_ativo(false); // Soft delete: nunca removemos do deque de verdade
    atualizarArquivoCompleto();
    return true;
}

bool GerenciadorProdutos::venderProduto(const std::string& id, int quantidadeVendida) {
    Produto* p = buscarProdutoPorId(id);
    if (!p) return false;

    // baixarEstoque já cuida de validar a quantidade e desativar o produto
    // sozinho se zerar o estoque - aqui só repassamos o resultado.
    if (!p->baixarEstoque(quantidadeVendida)) return false;

    atualizarArquivoCompleto();
    return true;
}

const std::deque<Produto>& GerenciadorProdutos::get_produtos() const {
    return _produtos;
}

Produto* GerenciadorProdutos::buscarProdutoPorId(const std::string& id) {
    for (auto& prod : _produtos) {
        if (prod.get_id() == id) {
            return &prod;
        }
    }
    return nullptr;
}

std::vector<Produto*> GerenciadorProdutos::buscarProdutosPorUsuario(const std::string& loginUsuario) {
    std::vector<Produto*> produtosDoUsuario;

    // Varre todos os produtos cadastrados e devolve os que pertencem ao utilizador
    for (auto& prod : _produtos) {
        if (prod.get_login_anunciante() == loginUsuario) {
            produtosDoUsuario.push_back(&prod);
        }
    }

    return produtosDoUsuario;
}
