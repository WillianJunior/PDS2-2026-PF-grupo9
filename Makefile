CXX = g++
# Flag -fprofile-dir=build garante que o lixo .gcda e .gcno vai direto para a pasta build!
CFLAGS = -std=gnu++17 -I include --coverage -DDOCTEST_CONFIG_NO_MULTITHREADING -fprofile-dir=build

BUILD_DIR = build
COVERAGE_DIR = $(BUILD_DIR)/coverage

TESTES = \
	$(BUILD_DIR)/TesteUsuario \
	$(BUILD_DIR)/TesteTerminalUI \
	$(BUILD_DIR)/TesteGerenciadorUsuarios \
	$(BUILD_DIR)/TesteItemVendido \
	$(BUILD_DIR)/TestePedido \
	$(BUILD_DIR)/TesteCarrinho \
	$(BUILD_DIR)/TesteAnuncio \
	$(BUILD_DIR)/TesteTransacao

all: test

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COVERAGE_DIR)

test: clean dirs $(TESTES)
	-./$(BUILD_DIR)/TesteUsuario
	-./$(BUILD_DIR)/TesteTerminalUI
	-./$(BUILD_DIR)/TesteGerenciadorUsuarios
	-./$(BUILD_DIR)/TesteItemVendido
	-./$(BUILD_DIR)/TestePedido
	-./$(BUILD_DIR)/TesteCarrinho
	-./$(BUILD_DIR)/TesteAnuncio
	-./$(BUILD_DIR)/TesteTransacao

	@echo ""
	@echo "===== ORGANIZANDO ARQUIVOS GERADOS ====="
	@find . -type f \( -name "*.gcno" -o -name "*.gcda" \) ! -path "./$(BUILD_DIR)/*" -exec mv {} $(BUILD_DIR)/ \; || true

	@echo ""
	@echo "===== RELATORIO DE COBERTURA ====="
	gcovr -r . --object-directory $(BUILD_DIR) --exclude 'tests/.*'

	@echo ""
	@echo "===== RELATORIO HTML ====="
	gcovr -r . --object-directory $(BUILD_DIR) --html --html-details -o $(COVERAGE_DIR)/coverage.html

	@echo ""
	@echo "Arquivo gerado: $(COVERAGE_DIR)/coverage.html"

# Regras de compilação
$(BUILD_DIR)/TesteUsuario:
	$(CXX) $(CFLAGS) tests/TesteUsuario.cpp src/usuario.cpp -o $@

$(BUILD_DIR)/TesteTerminalUI:
	$(CXX) $(CFLAGS) tests/TesteTerminalUI.cpp src/TerminalUI.cpp src/GerenciadorUsuarios.cpp src/GerenciadorProdutos.cpp src/Carrinho.cpp src/itemVendido.cpp src/produto.cpp src/usuario.cpp src/SistemaEscambo.cpp src/GerenciadorTransacoes.cpp src/Transacao.cpp src/Troca.cpp src/Compra.cpp src/Anuncio.cpp -o $@

$(BUILD_DIR)/TesteGerenciadorUsuarios:
	$(CXX) $(CFLAGS) tests/TesteGerenciadorUsuarios.cpp src/GerenciadorUsuarios.cpp src/usuario.cpp -o $@

$(BUILD_DIR)/TesteItemVendido:
	$(CXX) $(CFLAGS) tests/TesteItemVendido.cpp src/itemVendido.cpp -o $@

$(BUILD_DIR)/TestePedido:
	$(CXX) $(CFLAGS) tests/TestePedido.cpp src/Pedido.cpp src/itemVendido.cpp -o $@

$(BUILD_DIR)/TesteCarrinho:
	$(CXX) $(CFLAGS) tests/TesteCarrinho.cpp src/Carrinho.cpp src/itemVendido.cpp src/produto.cpp -o $@

$(BUILD_DIR)/TesteAnuncio:
	$(CXX) $(CFLAGS) tests/TesteAnuncio.cpp src/Anuncio.cpp src/produto.cpp src/usuario.cpp -o $@

# --- NOVA REGRA PARA TRANSACOES ---
$(BUILD_DIR)/TesteTransacao:
	$(CXX) $(CFLAGS) tests/TesteTransacao.cpp src/Transacao.cpp src/Compra.cpp src/Troca.cpp src/Anuncio.cpp src/produto.cpp src/usuario.cpp -o $@

clean:
	@echo "Limpando o ambiente..."
	rm -rf $(BUILD_DIR)/*
	rm -f *.gcda *.gcno *.gcov
	rm -f src/*.gcda src/*.gcno
	rm -f tests/*.gcda tests/*.gcno