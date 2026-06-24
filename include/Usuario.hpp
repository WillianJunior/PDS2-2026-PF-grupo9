#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <limits>
#include <set>
#include <string>

using namespace std;

// Capacidade = permissão binária (ou o usuário tem, ou não tem - não existe
// "meio caminho"). COMPRAR e ANUNCIAR já nascem com o usuário; TROCAR é
// concedida automaticamente na primeira compra concluída como comprador
// (ver Usuario::registrarCompraConcluida).
enum class Capacidade {
    COMPRAR,
    ANUNCIAR,
    TROCAR
};

// NivelUsuario é derivado (nunca persistido como campo próprio): calculado a
// partir de _vendasTrocasConcluidas só na hora de responder getNivel(). Ele
// não desbloqueia nada por si só, só afeta o limite de anúncios simultâneos
// (getLimiteAnuncios) - diferente de Capacidade, que é o que de fato controla
// "pode fazer ou não pode fazer X" no sistema.
enum class NivelUsuario {
    INICIANTE,
    VETERANO
};

// Conversões Capacidade <-> string, usadas tanto pra serializar quanto pra
// reconstruir a partir do usuarios.txt. Ficam como funções livres (e não
// métodos de Usuario) porque convertem um enum, não dependem de nenhum
// estado de instância.
std::string capacidadeParaString(Capacidade cap);
Capacidade stringParaCapacidade(const std::string& texto);

// Classe de Model "pura": representa um usuário do sistema (tanto faz se ele
// está a atuar como comprador ou anunciante naquele momento - esse papel é
// escolhido na View/TerminalUI, não aqui dentro). Não tem nenhuma lógica de
// I/O (isso é do GerenciadorUsuarios), mas agora concentra a regra de negócio
// de capacidades/progressão que antes não existia - sem isso, qualquer
// usuário recém-criado conseguiria anunciar/trocar sem limite, o que não é a
// regra desejada.
class Usuario {
private:
    int _id;
    string _nome;
    string _login;
    string _senha;
    string _chavePix;

    // Conjunto (não duplica) das capacidades concedidas até agora.
    std::set<Capacidade> _capacidades;

    // Como comprador: dispara o desbloqueio de TROCAR ao chegar em 1.
    int _comprasConcluidas;
    // Como vendedor (vendas OU trocas concluídas): determina o NivelUsuario.
    int _vendasTrocasConcluidas;
    // Contagem corrente de anúncios ativos do usuário (sobe/desce conforme
    // ele cria/desativa anúncios - controlado de fora, por quem gerencia
    // anúncios, não por Usuario).
    int _anunciosAtivos;

public:
    // Limite de anúncios simultâneos enquanto o usuário ainda é INICIANTE.
    static constexpr int LIMITE_ANUNCIOS_INICIANTE = 3;
    // Sentinela "sem limite", devolvido por getLimiteAnuncios() pra um
    // VETERANO. Usamos o próprio int::max em vez de um valor mágico tipo -1
    // pra que comparações diretas (ex: "anunciosAtivos < limite") continuem
    // funcionando sem precisar de um caso especial em quem chama.
    static constexpr int ANUNCIOS_ILIMITADO = std::numeric_limits<int>::max();
    // A partir de quantas vendas/trocas concluídas como vendedor o usuário
    // vira VETERANO.
    static constexpr int VENDAS_PARA_VETERANO = 3;

    // Construtor de usuário NOVO: todo usuário criado por aqui já nasce com
    // as capacidades padrão (COMPRAR + ANUNCIAR) e todos os contadores
    // zerados - é o ponto de entrada usado por GerenciadorUsuarios::registrarUsuario.
    Usuario(int id, string nome, string login, string senha, string chavePix = "");

    // Construtor de RECONSTRUÇÃO: usado só por
    // GerenciadorUsuarios::carregarUsuarios pra recriar um usuário já
    // existente a partir do que foi lido em usuarios.txt, com capacidades e
    // contadores específicos (em vez dos valores padrão de usuário novo).
    Usuario(int id, string nome, string login, string senha, string chavePix,
            std::set<Capacidade> capacidades, int comprasConcluidas,
            int vendasTrocasConcluidas, int anunciosAtivos);

    // Destrutor virtual: mesmo a classe não tendo subclasses hoje, é boa prática
    // deixar virtual sempre que existe a possibilidade de polimorfismo/ponteiros
    // a esta classe (ex: GerenciadorTransacoes guarda Usuario* vindos de fora).
    virtual ~Usuario() = default;

    int getId() const;

    // Validação de login fica encapsulada aqui dentro (e não no Gerenciador) porque
    // é uma regra que pertence ao próprio Usuario: só ele sabe comparar a senha dele.
    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;

    // REVISÃO: getters de string passaram a devolver const string& em vez de
    // string por valor. Cada chamada antiga gerava uma cópia inteira da
    // string só pra ler o valor (ex: imprimir o nome num cout) - com
    // referência, evitamos essa cópia toda vez que alguém só quer ler o
    // dado, sem abrir mão da imutabilidade.
    const string& getNome() const;
    const string& getLogin() const;
    const string& getSenha() const;
    const string& getChavePix() const;
    void setChavePix(const string& pix);

    // --- Capacidades ---

    bool temCapacidade(Capacidade cap) const;
    // Idempotente: conceder uma capacidade que o usuário já tem não duplica
    // nada (o std::set já garante isso por construção).
    void concederCapacidade(Capacidade cap);

    // --- Progressão (chamado pelo GerenciadorUsuarios/GerenciadorTransacoes
    // quando uma transação é concluída) ---

    // Incrementa _comprasConcluidas; ao chegar em 1, concede TROCAR.
    void registrarCompraConcluida();
    // Incrementa _vendasTrocasConcluidas (usado tanto pra vendas quanto pra
    // trocas concluídas como vendedor).
    void registrarVendaConcluida();

    // --- Nível e limites (derivados de _vendasTrocasConcluidas, não
    // guardados como campo próprio) ---

    NivelUsuario getNivel() const;
    int getLimiteAnuncios() const;
    bool podeCriarAnuncio() const;

    // --- Controle de anúncios ativos (chamado por quem gerencia anúncios -
    // GerenciadorProdutos/Anuncio - ao criar/desativar um anúncio) ---

    void incrementarAnunciosAtivos();
    // Nunca deixa _anunciosAtivos ficar negativo.
    void decrementarAnunciosAtivos();
    int getAnunciosAtivos() const;

    int getComprasConcluidas() const;
    int getVendasTrocasConcluidas() const;

    // Serializa as capacidades atuais como "COMPRAR;ANUNCIAR;TROCAR", pra
    // persistência em usuarios.txt (a vírgula já é o separador principal de
    // campo, por isso a sublista usa ';').
    std::string serializarCapacidades() const;
};

#endif
