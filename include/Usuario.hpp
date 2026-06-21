#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

using namespace std;

// Classe de Model "pura": representa um usuário do sistema (tanto faz se ele
// está a atuar como comprador ou anunciante naquele momento - esse papel é
// escolhido na View/TerminalUI, não aqui dentro). Não tem nenhuma lógica de
// negócio nem de I/O, só guarda estado e valida as próprias credenciais.
class Usuario {
private:
    int _id;
    string _nome;
    string _login;
    string _senha;
    string _chavePix;
public:
    Usuario(int id, string nome, string login, string senha, string chavePix = "");

    // Destrutor virtual: mesmo a classe não tendo subclasses hoje, é boa prática
    // deixar virtual sempre que existe a possibilidade de polimorfismo/ponteiros
    // a esta classe (ex: GerenciadorTransacoes guarda Usuario* vindos de fora).
    virtual ~Usuario() = default;

    int getId() const;

    // Validação de login fica encapsulada aqui dentro (e não no Gerenciador) porque
    // é uma regra que pertence ao próprio Usuario: só ele sabe comparar a senha dele.
    bool validarLogin(const string& loginIngressado, const string& senhaIngressado) const;

    // REVISÃO: getters de string passaram a devolver const string& em vez de string
    // por valor. Cada chamada antiga gerava uma cópia inteira da string só pra
    // ler o valor (ex: imprimir o nome num cout) - com referência, evitamos essa
    // cópia toda vez que alguém só quer ler o dado, sem abrir mão da imutabilidade.
    const string& getNome() const;
    const string& getLogin() const;
    const string& getSenha() const;
    const string& getChavePix() const;
    void setChavePix(const string& pix);
};

#endif
