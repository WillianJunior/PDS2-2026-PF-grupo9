#ifndef GERENCIADOR_USUARIOS_HPP
#define GERENCIADOR_USUARIOS_HPP

#include <deque>
#include <string>
#include "Usuario.hpp"

using namespace std;

// Gerenciador = camada de "Controller/Repository" entre o Model (Usuario) e o
// armazenamento em arquivo. Quem cadastra/autentica passa por aqui; ninguém
// de fora (TerminalUI, SistemaEscambo) lê ou escreve usuarios.txt direto.
class GerenciadorUsuarios {
private:
    // REVISÃO (bug real corrigido): trocamos std::vector<Usuario> por
    // std::deque<Usuario>. O motivo é sério: autenticar()/buscarUsuarioPorLogin()
    // devolvem Usuario* apontando PRA DENTRO do container, e esses ponteiros
    // ficam guardados em Anuncio/Transacao durante toda a sessão (até o fim do
    // programa, já que vários usuários se cadastram/logam no mesmo processo).
    // Com vector, um push_back que force realocação invalida TODOS os ponteiros
    // já entregues - dangling pointer, comportamento indefinido na próxima
    // leitura. deque garante que inserir no fim NUNCA invalida referências/
    // ponteiros pros elementos já existentes (só iteradores, que não usamos a
    // longo prazo aqui), resolvendo o problema sem mudar nenhuma outra lógica.
    deque<Usuario> _usuarios;

    // Por convenção, nomes em CAIXA_ALTA aqui marcam uma constante "tipo macro"
    // (o caminho do arquivo nunca muda em runtime) - por isso não segue o
    // padrão de atributo privado normal com _ (esse seria pra estado mutável).
    const string NOME_ARQUIVO = "usuarios.txt";

    void carregarUsuarios();
    void salvarUsuario(const Usuario& usuario);

public:
    GerenciadorUsuarios();
    bool registrarUsuario(const string& nome, const string& login, const string& senha, const string& chavePix = "");
    Usuario* autenticar(const string& loginIngressado, const string& senhaIngressado);
    Usuario* buscarUsuarioPorLogin(const std::string& login);
};

#endif
