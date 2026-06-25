CXX = g++
# Sem -fprofile-dir: o .gcno/.gcda ja nascem em build/ porque -o aponta pra la.
# (-fprofile-dir=build forcava o runtime a mangled o path absoluto no nome do .gcda,
# o que quebrava o pareamento com o .gcno e fazia o gcov falhar.)
CFLAGS = -std=gnu++17 -I include --coverage -DDOCTEST_CONFIG_NO_MULTITHREADING

# Flags do binario de uso normal: sem --coverage/doctest, que so servem pros testes.
APP_CFLAGS = -std=gnu++17 -I include

BUILD_DIR = build
COVERAGE_DIR = $(BUILD_DIR)/coverage

APP_BIN = app
APP_SRCS = \
	src/main.cpp \
	src/TerminalUI.cpp \
	src/GerenciadorUsuarios.cpp \
	src/GerenciadorProdutos.cpp \
	src/Carrinho.cpp \
	src/ItemVendido.cpp \
	src/Produto.cpp \
	src/Usuario.cpp \
	src/SistemaEscambo.cpp \
	src/GerenciadorTransacoes.cpp \
	src/Transacao.cpp \
	src/Troca.cpp \
	src/Compra.cpp \
	src/Anuncio.cpp

TESTES = \
	$(BUILD_DIR)/TesteUsuario \
	$(BUILD_DIR)/TesteTerminalUI \
	$(BUILD_DIR)/TesteGerenciadorUsuarios \
	$(BUILD_DIR)/TesteItemVendido \
	$(BUILD_DIR)/TestePedido \
	$(BUILD_DIR)/TesteCarrinho \
	$(BUILD_DIR)/TesteAnuncio \
	$(BUILD_DIR)/TesteTransacao

.PHONY: all dirs test clean run

# Default: so compila e executa o app, sem rodar a suite de testes/cobertura.
# Para isso, use "make test" explicitamente.
all: run

$(APP_BIN): $(APP_SRCS)
	$(CXX) $(APP_CFLAGS) $(APP_SRCS) -o $(APP_BIN)

run: $(APP_BIN)
	./$(APP_BIN)

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COVERAGE_DIR)

DADOS_PERSISTENCIA = usuarios.txt produtos.txt transacoes.txt

test: clean dirs $(TESTES)
	@echo ""
	@echo "===== PROTEGENDO ARQUIVOS DE PERSISTENCIA ====="
	@for f in $(DADOS_PERSISTENCIA); do \
		[ -f $$f ] && cp $$f $(BUILD_DIR)/$$f.bak; \
	done

	-./$(BUILD_DIR)/TesteUsuario
	-./$(BUILD_DIR)/TesteTerminalUI
	-./$(BUILD_DIR)/TesteGerenciadorUsuarios
	-./$(BUILD_DIR)/TesteItemVendido
	-./$(BUILD_DIR)/TestePedido
	-./$(BUILD_DIR)/TesteCarrinho
	-./$(BUILD_DIR)/TesteAnuncio
	-./$(BUILD_DIR)/TesteTransacao

	@echo ""
	@echo "===== RESTAURANDO ARQUIVOS DE PERSISTENCIA ====="
	@for f in $(DADOS_PERSISTENCIA); do \
		[ -f $(BUILD_DIR)/$$f.bak ] && mv $(BUILD_DIR)/$$f.bak $$f; \
	done

	@echo ""
	@echo "===== RELATORIO DE COBERTURA ====="
	-gcovr -r . --object-directory $(BUILD_DIR) --exclude 'tests/.*'

	@echo ""
	@echo "===== RELATORIO HTML ====="
	-gcovr -r . --object-directory $(BUILD_DIR) --html --html-details -o $(COVERAGE_DIR)/coverage.html

	@echo ""
	@if [ -f $(COVERAGE_DIR)/coverage.html ]; then \
		echo "Arquivo gerado: $(COVERAGE_DIR)/coverage.html"; \
	else \
		echo "AVISO: gcovr falhou, relatorio de cobertura nao foi gerado."; \
	fi

	@echo ""
	@echo "===== LIMPANDO RESIDUOS DE COBERTURA ====="
	@find $(BUILD_DIR) -maxdepth 1 -type f \( -name "*.gcno" -o -name "*.gcda" -o -name "*.gcov" \) -delete
	@for t in $(TESTES); do rm -f $$t; done

# Regras de compilação
$(BUILD_DIR)/TesteUsuario:
	$(CXX) $(CFLAGS) tests/TesteUsuario.cpp src/Usuario.cpp -o $@

$(BUILD_DIR)/TesteTerminalUI:
	$(CXX) $(CFLAGS) tests/TesteTerminalUI.cpp src/TerminalUI.cpp src/GerenciadorUsuarios.cpp src/GerenciadorProdutos.cpp src/Carrinho.cpp src/ItemVendido.cpp src/Produto.cpp src/Usuario.cpp src/SistemaEscambo.cpp src/GerenciadorTransacoes.cpp src/Transacao.cpp src/Troca.cpp src/Compra.cpp src/Anuncio.cpp -o $@

$(BUILD_DIR)/TesteGerenciadorUsuarios:
	$(CXX) $(CFLAGS) tests/TesteGerenciadorUsuarios.cpp src/GerenciadorUsuarios.cpp src/Usuario.cpp -o $@

$(BUILD_DIR)/TesteItemVendido:
	$(CXX) $(CFLAGS) tests/TesteItemVendido.cpp src/ItemVendido.cpp -o $@

$(BUILD_DIR)/TestePedido:
	$(CXX) $(CFLAGS) tests/TestePedido.cpp src/Pedido.cpp src/ItemVendido.cpp -o $@

$(BUILD_DIR)/TesteCarrinho:
	$(CXX) $(CFLAGS) tests/TesteCarrinho.cpp src/Carrinho.cpp src/ItemVendido.cpp src/Produto.cpp -o $@

$(BUILD_DIR)/TesteAnuncio:
	$(CXX) $(CFLAGS) tests/TesteAnuncio.cpp src/Anuncio.cpp src/Produto.cpp src/Usuario.cpp -o $@

# --- NOVA REGRA PARA TRANSACOES ---
$(BUILD_DIR)/TesteTransacao:
	$(CXX) $(CFLAGS) tests/TesteTransacao.cpp src/Transacao.cpp src/Compra.cpp src/Troca.cpp src/Anuncio.cpp src/Produto.cpp src/Usuario.cpp -o $@

clean:
	@echo "Limpando o ambiente..."
	rm -rf $(BUILD_DIR)/*
	rm -f *.gcda *.gcno *.gcov
	rm -f src/*.gcda src/*.gcno
	rm -f tests/*.gcda tests/*.gcno
	rm -f $(APP_BIN)