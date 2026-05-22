CXX = g++
CFLAGS = -Wall -Wextra -std=c++17 -I include --coverage

TESTES = \
	TesteUsuario \
	TesteTerminalUI \
	TesteGerenciadorUsuarios \
	TesteComprador \
	TesteItemVendido \
	TestePedido \
	TesteCarrinho \
	TesteAnuncio \
	TesteAnunciante

all: test

test: clean $(TESTES)
	-./TesteUsuario
	-./TesteTerminalUI
	-./TesteGerenciadorUsuarios
	-./TesteComprador
	-./TesteItemVendido
	-./TestePedido
	-./TesteCarrinho
	-./TesteAnuncio
	-./TesteAnunciante

	@echo ""
	@echo "===== RELATORIO DE COBERTURA ====="

	gcovr \
	-r . \
	--exclude 'tests/' \
	--exclude 'include/' \
	--exclude 'src/main.cpp'

	@echo ""
	@echo "===== RELATORIO HTML ====="

	gcovr \
	-r . \
	--exclude 'tests/' \
	--exclude 'include/' \
	--exclude 'src/main.cpp' \
	--html \
	--html-details \
	-o coverage.html

	@echo ""
	@echo "Arquivo gerado: coverage.html"

TesteUsuario:
	$(CXX) $(CFLAGS) tests/TesteUsuario.cpp \
	src/usuario.cpp \
	-o TesteUsuario

TesteTerminalUI:
	$(CXX) $(CFLAGS) tests/TesteTerminalUI.cpp \
	src/TerminalUI.cpp \
	src/GerenciadorUsuarios.cpp \
	src/usuario.cpp \
	-o TesteTerminalUI

TesteGerenciadorUsuarios:
	$(CXX) $(CFLAGS) tests/TesteGerenciadorUsuarios.cpp \
	src/GerenciadorUsuarios.cpp \
	src/usuario.cpp \
	-o TesteGerenciadorUsuarios

TesteComprador:
	$(CXX) $(CFLAGS) tests/TesteComprador.cpp \
	src/Comprador.cpp \
	src/usuario.cpp \
	-o TesteComprador

TesteItemVendido:
	$(CXX) $(CFLAGS) tests/TesteItemVendido.cpp \
	src/ItemVendido.cpp \
	-o TesteItemVendido

TestePedido:
	$(CXX) $(CFLAGS) tests/TestePedido.cpp \
	src/Pedido.cpp \
	src/ItemVendido.cpp \
	-o TestePedido

TesteCarrinho:
	$(CXX) $(CFLAGS) tests/TesteCarrinho.cpp \
	src/Carrinho.cpp \
	src/produto.cpp \
	-o TesteCarrinho

TesteAnuncio:
	$(CXX) $(CFLAGS) tests/TesteAnuncio.cpp \
	src/produto.cpp \
	src/usuario.cpp \
	-o TesteAnuncio

TesteAnunciante:
	$(CXX) $(CFLAGS) tests/TesteAnunciante.cpp \
	src/usuario.cpp \
	src/produto.cpp \
	-o TesteAnunciante

clean:
	rm -f $(TESTES)
	rm -f *.gcda *.gcno *.gcov
	rm -f coverage.html
	rm -f coverage.info
	rm -rf coverage
	rm -f src/*.gcda src/*.gcno
	rm -f tests/*.gcda tests/*.gcno