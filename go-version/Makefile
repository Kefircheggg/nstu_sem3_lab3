# Makefile для Go версии

.PHONY: all build run clean test help

# Имя исполняемого файла
BINARY=dbms

# По умолчанию - компиляция
all: build

# Линтер и форматирование
lint:
	go fmt ./...
	go vet ./...
	@echo "Код отформатирован и проверен"

# Инициализация Go модуля
init:
	go mod init dbms
	@echo "Go модуль инициализирован"

# Компиляция
build:
	go build -o $(BINARY) .
	@echo "Скомпилировано: $(BINARY)"

# Запуск по умолчанию
run: build
	./$(BINARY) --help

# Тестирование
test: build
	@echo "=== Тест массива ==="
	./$(BINARY) --file test.db --query 'MPUSH myarray apple'
	./$(BINARY) --file test.db --query 'MPUSH myarray banana'
	./$(BINARY) --file test.db --query 'MPUSH myarray cherry'
	./$(BINARY) --file test.db --query 'PRINT myarray'
	@echo ""
	@echo "=== Тест стека (LIFO) ==="
	./$(BINARY) --file test.db --query 'SPUSH mystack first'
	./$(BINARY) --file test.db --query 'SPUSH mystack second'
	./$(BINARY) --file test.db --query 'SPUSH mystack third'
	./$(BINARY) --file test.db --query 'PRINT mystack'
	@echo "Извлекаем из стека:"
	./$(BINARY) --file test.db --query 'SPOP mystack'
	@echo ""
	@echo "=== Тест очереди (FIFO) ==="
	./$(BINARY) --file test.db --query 'QPUSH myqueue A'
	./$(BINARY) --file test.db --query 'QPUSH myqueue B'
	./$(BINARY) --file test.db --query 'QPUSH myqueue C'
	./$(BINARY) --file test.db --query 'PRINT myqueue'
	@echo "Извлекаем из очереди:"
	./$(BINARY) --file test.db --query 'QPOP myqueue'

# Очистка
clean:
	rm -f $(BINARY)
	rm -f test.db
	@echo "Очистка завершена"

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make init    - Инициализировать Go модуль (один раз)"
	@echo "  make build   - Скомпилировать программу"
	@echo "  make run     - Запустить программу (справка)"
	@echo "  make test    - Запустить тесты"
	@echo "  make clean   - Удалить скомпилированные файлы"
	@echo "  make help    - Показать эту справку"
