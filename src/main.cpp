#include "../include/TerminalUI.hpp"

// main() fica deliberadamente vazio de lógica: toda a orquestração (Model +
// Fachada) já está encapsulada dentro de TerminalUI, que por sua vez só
// conhece SistemaEscambo. Isso é o ponto de entrada do MVC, não o lugar pra
// regra de negócio.
int main() {
    TerminalUI ui;
    ui.iniciar();
    return 0;
}