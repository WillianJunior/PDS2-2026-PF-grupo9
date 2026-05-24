 - raiz do projeto cheia de lixo (arquivos html). pior: esses arquivos não são limpos com um "make clean". esses arquivos gerados devem obedecer o mesmo padrão dos arquivos .o, devem ter um diretório só deles.
 - arquivos gerados (.o, .gcno, .gcda, .html) estão sendo commitados. esses arquivos devem ser ignorados via .gitignore.
 - arquivos gerados em compilação (.o, .gcno, .gcda) devem ficar em build, não na raiz do projeto.
 - headers devem ser .hpp, não .h. 
 - código não compilou:
 will@nobara:~/git/pds2/PDS2-2026-PF-grupo9$ make
rm -f TesteUsuario TesteTerminalUI TesteGerenciadorUsuarios TesteComprador TesteItemVendido TestePedido TesteCarrinho TesteAnuncio TesteAnunciante
rm -f *.gcda *.gcno *.gcov
rm -f coverage.html
rm -f coverage.info
rm -rf coverage
rm -f src/*.gcda src/*.gcno
rm -f tests/*.gcda tests/*.gcno
g++ -std=c++17 -I include --coverage tests/TesteUsuario.cpp \
src/usuario.cpp \
-o TesteUsuario
tests/TesteUsuario.cpp:3:10: fatal error: ../include/Usuario.h: No such file or directory
    3 | #include "../include/Usuario.h"
      |          ^~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
src/usuario.cpp:1:10: fatal error: ../include/Usuario.h: No such file or directory
    1 | #include "../include/Usuario.h"
      |          ^~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
make: *** [Makefile:49: TesteUsuario] Error 1
