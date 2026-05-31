CXX = g++
CFLAGS = -std=gnu++17 -I include --coverage -DDOCTEST_CONFIG_NO_MULTITHREADING

BUILD_DIR = build
COVERAGE_DIR = coverage

TESTES = \
	$(BUILD_DIR)/Testeusuario \
	$(BUILD_DIR)/TesteTerminalUI \
	$(BUILD_DIR)/TesteGerenciadorUsuarios \
	$(BUILD_DIR)/TesteitemVendido \
	$(BUILD_DIR)/TestePedido \
	$(BUILD_DIR)/TesteCarrinho \
	$(BUILD_DIR)/TesteAnuncio

all: test

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COVERAGE_DIR)

test: clean dirs $(TESTES)
	-./$(BUILD_DIR)/Testeusuario
	-./$(BUILD_DIR)/TesteTerminalUI
	-./$(BUILD_DIR)/TesteGerenciadorUsuarios
	-./$(BUILD_DIR)/TesteitemVendido
	-./$(BUILD_DIR)/TestePedido
	-./$(BUILD_DIR)/TesteCarrinho
	-./$(BUILD_DIR)/TesteAnuncio


	@echo ""
	@echo "===== RELATORIO DE COBERTURA ====="

	gcovr \
	-r . \
	--exclude 'tests/.*'

	@echo ""
	@echo "===== RELATORIO HTML ====="

	gcovr \
	-r . \
	--html \
	--html-details \
	-o $(COVERAGE_DIR)/coverage.html

	@echo ""
	@echo "Arquivo gerado: $(COVERAGE_DIR)/coverage.html"

$(BUILD_DIR)/Testeusuario:
	$(CXX) $(CFLAGS) tests/TesteUsuario.cpp src/usuario.cpp -o $@

$(BUILD_DIR)/TesteTerminalUI:
	$(CXX) $(CFLAGS) tests/TesteTerminalUI.cpp src/TerminalUI.cpp src/GerenciadorUsuarios.cpp src/GerenciadorProdutos.cpp src/Carrinho.cpp src/itemVendido.cpp src/produto.cpp src/usuario.cpp -o $@

$(BUILD_DIR)/TesteGerenciadorUsuarios:
	$(CXX) $(CFLAGS) tests/TesteGerenciadorUsuarios.cpp src/GerenciadorUsuarios.cpp src/usuario.cpp -o $@

$(BUILD_DIR)/TesteitemVendido:
	$(CXX) $(CFLAGS) tests/TesteItemVendido.cpp src/itemVendido.cpp -o $@

$(BUILD_DIR)/TestePedido:
	$(CXX) $(CFLAGS) tests/TestePedido.cpp src/Pedido.cpp src/itemVendido.cpp -o $@

$(BUILD_DIR)/TesteCarrinho:
	$(CXX) $(CFLAGS) tests/TesteCarrinho.cpp src/Carrinho.cpp src/itemVendido.cpp src/produto.cpp -o $@

$(BUILD_DIR)/TesteAnuncio:
	$(CXX) $(CFLAGS) tests/TesteAnuncio.cpp src/Anuncio.cpp src/produto.cpp src/usuario.cpp -o $@


clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(COVERAGE_DIR)
	rm -f *.gcda *.gcno *.gcov
	rm -f src/*.gcda src/*.gcno
	rm -f tests/*.gcda tests/*.gcno