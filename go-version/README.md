# Go Version - Структуры данных

## Описание

Версия программы на языке Go с той же логикой, что и C++ версия.

Реализованы следующие структуры данных:
- **Массив** (Array) - динамический массив
- **Односвязный список** (Singly Linked List)
- **Двусвязный список** (Doubly Linked List)
- **Стек** (Stack - LIFO)
- **Очередь** (Queue - FIFO)

## Компиляция и запуск

### Инициализация модуля (один раз)
```bash
go mod init dbms
```

### Компиляция
```bash
go build -o dbms .
```

### Или запуск без компиляции
```bash
go run . --file file.data --query 'MPUSH myarray value'
```

## Примеры использования

### Массив
```bash
./dbms --file data.db --query 'MPUSH myarray apple'
./dbms --file data.db --query 'MPUSH myarray banana'
./dbms --file data.db --query 'MGET myarray 0'
# Результат: apple
./dbms --file data.db --query 'MLEN myarray'
# Результат: 2
./dbms --file data.db --query 'PRINT myarray'
```

### Односвязный список
```bash
./dbms --file data.db --query 'LPUSH_FRONT mylist first'
./dbms --file data.db --query 'LPUSH_BACK mylist last'
./dbms --file data.db --query 'LGET mylist 0'
# Результат: first
./dbms --file data.db --query 'PRINT mylist'
```

### Стек (LIFO)
```bash
./dbms --file data.db --query 'SPUSH mystack item1'
./dbms --file data.db --query 'SPUSH mystack item2'
./dbms --file data.db --query 'SPUSH mystack item3'
./dbms --file data.db --query 'SPOP mystack'
# Результат: item3 (последний добавленный)
./dbms --file data.db --query 'PRINT mystack'
```

### Очередь (FIFO)
```bash
./dbms --file data.db --query 'QPUSH myqueue job1'
./dbms --file data.db --query 'QPUSH myqueue job2'
./dbms --file data.db --query 'QPUSH myqueue job3'
./dbms --file data.db --query 'QPOP myqueue'
# Результат: job1 (первый добавленный)
./dbms --file data.db --query 'PRINT myqueue'
```

### Справка
```bash
./dbms --help
```

## Структура файлов

```
go-version/
├── main.go         - Основная логика, база данных, CLI
├── array.go        - Реализация массива
├── list.go         - Односвязный и двусвязный списки
├── stack.go        - Реализация стека
├── queue.go        - Реализация очереди
├── go.mod          - Модуль Go
└── README.md       - Эта документация
```

## Полный список команд

| Команда | Описание |
|---------|----------|
| `MPUSH name value` | Добавить элемент в массив |
| `MGET name index` | Получить элемент массива по индексу |
| `MDEL name index` | Удалить элемент массива по индексу |
| `MLEN name` | Получить длину массива |
| `LPUSH_FRONT name value` | Добавить элемент в начало списка |
| `LPUSH_BACK name value` | Добавить элемент в конец списка |
| `LGET name index` | Получить элемент списка по индексу |
| `LDEL name value` | Удалить элемент списка по значению |
| `SPUSH name value` | Добавить элемент в стек |
| `SPOP name` | Извлечь элемент из стека |
| `QPUSH name value` | Добавить элемент в очередь |
| `QPOP name` | Извлечь элемент из очереди |
| `PRINT name` | Вывести структуру на экран |

## Особенности реализации

- Использованы встроенные возможности Go (slices, maps)
- Автоматическое управление памятью (garbage collector)
- Типобезопасность через интерфейсы
- Простая сериализация в текстовый формат
- Совместимый формат файлов с C++ версией

## Требования

- Go 1.16 или новее

## Тестирование

```bash
# Создать тестовые данные
./dbms --file test.db --query 'MPUSH colors red'
./dbms --file test.db --query 'MPUSH colors green'
./dbms --file test.db --query 'MPUSH colors blue'

# Вывести
./dbms --file test.db --query 'PRINT colors'

# Проверить стек LIFO
./dbms --file test.db --query 'SPUSH stack A'
./dbms --file test.db --query 'SPUSH stack B'
./dbms --file test.db --query 'SPUSH stack C'
./dbms --file test.db --query 'SPOP stack'
# Ожидается: C

# Проверить очередь FIFO
./dbms --file test.db --query 'QPUSH queue X'
./dbms --file test.db --query 'QPUSH queue Y'
./dbms --file test.db --query 'QPUSH queue Z'
./dbms --file test.db --query 'QPOP queue'
# Ожидается: X
```
