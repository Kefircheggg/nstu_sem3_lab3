package main

import (
	"bufio"
	"dbms/structures"
	"flag"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// StructureType - тип структуры данных
type StructureType int

const (
	TypeArray StructureType = iota
	TypeSList
	TypeDList
	TypeStack
	TypeQueue
)

// DataStructure - обертка для любой структуры данных
type DataStructure struct {
	structType StructureType
	data       interface{}
}

// Database - база данных со структурами
type Database struct {
	structures map[string]*DataStructure
}

// NewDatabase создает новую базу данных
func NewDatabase() *Database {
	return &Database{
		structures: make(map[string]*DataStructure),
	}
}

// ExecuteQuery выполняет SQL-подобный запрос
func (db *Database) ExecuteQuery(query string) string {
	tokens := strings.Fields(query)
	if len(tokens) == 0 {
		return "ERROR: Empty query"
	}

	command := tokens[0]

	// MPUSH - добавить элемент в массив
	if command == "MPUSH" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		if _, exists := db.structures[name]; !exists {
			db.structures[name] = &DataStructure{
				structType: TypeArray,
				data:       structures.NewArray(),
			}
		}

		if db.structures[name].structType != TypeArray {
			return "ERROR: " + name + " is not an array"
		}

		arr := db.structures[name].data.(*structures.Array)
		arr.Push(value)
		return "OK"
	}

	// MGET - получить элемент массива
	if command == "MGET" && len(tokens) >= 3 {
		name := tokens[1]
		index, err := strconv.Atoi(tokens[2])
		if err != nil {
			return "ERROR: Invalid index"
		}

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Array " + name + " not found"
		}
		if ds.structType != TypeArray {
			return "ERROR: " + name + " is not an array"
		}

		arr := ds.data.(*structures.Array)
		if val, ok := arr.Get(index); ok {
			return val
		}
		return "ERROR: Index out of bounds"
	}

	// MDEL - удалить элемент массива
	if command == "MDEL" && len(tokens) >= 3 {
		name := tokens[1]
		index, err := strconv.Atoi(tokens[2])
		if err != nil {
			return "ERROR: Invalid index"
		}

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Array " + name + " not found"
		}
		if ds.structType != TypeArray {
			return "ERROR: " + name + " is not an array"
		}

		arr := ds.data.(*structures.Array)
		if arr.Delete(index) {
			return "OK"
		}
		return "ERROR: Index out of bounds"
	}

	// MLEN - длина массива
	if command == "MLEN" && len(tokens) >= 2 {
		name := tokens[1]

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Array " + name + " not found"
		}
		if ds.structType != TypeArray {
			return "ERROR: " + name + " is not an array"
		}

		arr := ds.data.(*structures.Array)
		return strconv.Itoa(arr.Len())
	}

	// LPUSH_FRONT - добавить в начало списка
	if command == "LPUSH_FRONT" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		if _, exists := db.structures[name]; !exists {
			db.structures[name] = &DataStructure{
				structType: TypeSList,
				data:       structures.NewSinglyList(),
			}
		}

		ds := db.structures[name]
		if ds.structType == TypeSList {
			list := ds.data.(*structures.SinglyList)
			list.PushFront(value)
			return "OK"
		} else if ds.structType == TypeDList {
			list := ds.data.(*structures.DoublyList)
			list.PushFront(value)
			return "OK"
		}
		return "ERROR: " + name + " is not a list"
	}

	// LPUSH_BACK - добавить в конец списка
	if command == "LPUSH_BACK" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		if _, exists := db.structures[name]; !exists {
			db.structures[name] = &DataStructure{
				structType: TypeSList,
				data:       structures.NewSinglyList(),
			}
		}

		ds := db.structures[name]
		if ds.structType == TypeSList {
			list := ds.data.(*structures.SinglyList)
			list.PushBack(value)
			return "OK"
		} else if ds.structType == TypeDList {
			list := ds.data.(*structures.DoublyList)
			list.PushBack(value)
			return "OK"
		}
		return "ERROR: " + name + " is not a list"
	}

	// LGET - получить элемент списка
	if command == "LGET" && len(tokens) >= 3 {
		name := tokens[1]
		index, err := strconv.Atoi(tokens[2])
		if err != nil {
			return "ERROR: Invalid index"
		}

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: List " + name + " not found"
		}

		if ds.structType == TypeSList {
			list := ds.data.(*structures.SinglyList)
			if val, ok := list.Get(index); ok {
				return val
			}
		} else if ds.structType == TypeDList {
			list := ds.data.(*structures.DoublyList)
			if val, ok := list.Get(index); ok {
				return val
			}
		} else {
			return "ERROR: " + name + " is not a list"
		}
		return "ERROR: Index out of bounds"
	}

	// LDEL - удалить элемент списка
	if command == "LDEL" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: List " + name + " not found"
		}

		if ds.structType == TypeSList {
			list := ds.data.(*structures.SinglyList)
			if list.Delete(value) {
				return "OK"
			}
		} else if ds.structType == TypeDList {
			list := ds.data.(*structures.DoublyList)
			if list.Delete(value) {
				return "OK"
			}
		} else {
			return "ERROR: " + name + " is not a list"
		}
		return "ERROR: Value not found"
	}

	// SPUSH - добавить в стек
	if command == "SPUSH" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		if _, exists := db.structures[name]; !exists {
			db.structures[name] = &DataStructure{
				structType: TypeStack,
				data:       structures.NewStack(),
			}
		}

		ds := db.structures[name]
		if ds.structType != TypeStack {
			return "ERROR: " + name + " is not a stack"
		}

		stack := ds.data.(*structures.Stack)
		return stack.Push(value)
	}

	// SPOP - извлечь из стека
	if command == "SPOP" && len(tokens) >= 2 {
		name := tokens[1]

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Stack " + name + " not found"
		}
		if ds.structType != TypeStack {
			return "ERROR: " + name + " is not a stack"
		}

		stack := ds.data.(*structures.Stack)
		if val, ok := stack.Pop(); ok {
			return val
		}
		return "ERROR: Stack is empty"
	}

	// QPUSH - добавить в очередь
	if command == "QPUSH" && len(tokens) >= 3 {
		name := tokens[1]
		value := tokens[2]

		if _, exists := db.structures[name]; !exists {
			db.structures[name] = &DataStructure{
				structType: TypeQueue,
				data:       structures.NewQueue(),
			}
		}

		ds := db.structures[name]
		if ds.structType != TypeQueue {
			return "ERROR: " + name + " is not a queue"
		}

		queue := ds.data.(*structures.Queue)
		return queue.Push(value)
	}

	// QPOP - извлечь из очереди
	if command == "QPOP" && len(tokens) >= 2 {
		name := tokens[1]

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Queue " + name + " not found"
		}
		if ds.structType != TypeQueue {
			return "ERROR: " + name + " is not a queue"
		}

		queue := ds.data.(*structures.Queue)
		if val, ok := queue.Pop(); ok {
			return val
		}
		return "ERROR: Queue is empty"
	}

	// PRINT - вывести структуру на экран
	if command == "PRINT" && len(tokens) >= 2 {
		name := tokens[1]

		ds, exists := db.structures[name]
		if !exists {
			return "ERROR: Structure " + name + " not found"
		}

		switch ds.structType {
		case TypeArray:
			return ds.data.(*structures.Array).Print(name)
		case TypeSList:
			return ds.data.(*structures.SinglyList).Print(name)
		case TypeDList:
			return ds.data.(*structures.DoublyList).Print(name)
		case TypeStack:
			return ds.data.(*structures.Stack).Print(name)
		case TypeQueue:
			return ds.data.(*structures.Queue).Print(name)
		}
	}

	return "ERROR: Unknown command or invalid syntax"
}

// SaveToFile сохраняет базу данных в файл
func (db *Database) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)

	for name, ds := range db.structures {
		// Записываем тип структуры
		structTypeName := ""
		switch ds.structType {
		case TypeArray:
			structTypeName = "ARRAY"
		case TypeSList:
			structTypeName = "SLIST"
		case TypeDList:
			structTypeName = "DLIST"
		case TypeStack:
			structTypeName = "STACK"
		case TypeQueue:
			structTypeName = "QUEUE"
		}

		fmt.Fprintf(writer, "STRUCTURE %s %s\n", name, structTypeName)

		// Записываем данные
		switch ds.structType {
		case TypeArray:
			arr := ds.data.(*structures.Array)
			for _, val := range arr.Values() {
				fmt.Fprintf(writer, "MPUSH %s %s\n", name, val)
			}
		case TypeSList:
			list := ds.data.(*structures.SinglyList)
			for _, val := range list.Values() {
				fmt.Fprintf(writer, "LPUSH_BACK %s %s\n", name, val)
			}
		case TypeDList:
			list := ds.data.(*structures.DoublyList)
			for _, val := range list.Values() {
				fmt.Fprintf(writer, "LPUSH_BACK %s %s\n", name, val)
			}
		case TypeStack:
			// Для стека нужно сохранить в обратном порядке
			stack := ds.data.(*structures.Stack)
			items := stack.Values()
			// Записываем в обратном порядке
			for i := len(items) - 1; i >= 0; i-- {
				fmt.Fprintf(writer, "SPUSH %s %s\n", name, items[i])
			}
		case TypeQueue:
			queue := ds.data.(*structures.Queue)
			for _, val := range queue.Values() {
				fmt.Fprintf(writer, "QPUSH %s %s\n", name, val)
			}
		}
	}

	return writer.Flush()
}

// LoadFromFile загружает базу данных из файла
func (db *Database) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		if os.IsNotExist(err) {
			return nil // Файл не существует - это нормально
		}
		return err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}

		tokens := strings.Fields(line)
		if len(tokens) < 3 {
			continue
		}

		if tokens[0] == "STRUCTURE" {
			name := tokens[1]
			structType := tokens[2]

			switch structType {
			case "ARRAY":
				db.structures[name] = &DataStructure{
					structType: TypeArray,
					data:       structures.NewArray(),
				}
			case "SLIST":
				db.structures[name] = &DataStructure{
					structType: TypeSList,
					data:       structures.NewSinglyList(),
				}
			case "DLIST":
				db.structures[name] = &DataStructure{
					structType: TypeDList,
					data:       structures.NewDoublyList(),
				}
			case "STACK":
				db.structures[name] = &DataStructure{
					structType: TypeStack,
					data:       structures.NewStack(),
				}
			case "QUEUE":
				db.structures[name] = &DataStructure{
					structType: TypeQueue,
					data:       structures.NewQueue(),
				}
			}
		} else {
			// Выполняем команду
			db.ExecuteQuery(line)
		}
	}

	return scanner.Err()
}

func printUsage() {
	fmt.Println("Использование:")
	fmt.Println("  ./dbms --file <filename> --query '<command>'")
	fmt.Println()
	fmt.Println("Команды:")
	fmt.Println()
	fmt.Println("  Массив (Array):")
	fmt.Println("    MPUSH myarray value      - добавить элемент в массив")
	fmt.Println("    MGET myarray 0           - получить элемент по индексу")
	fmt.Println("    MDEL myarray 0           - удалить элемент по индексу")
	fmt.Println("    MLEN myarray             - получить длину массива")
	fmt.Println("    PRINT myarray            - вывести массив")
	fmt.Println()
	fmt.Println("  Односвязный/Двусвязный список (Linked List):")
	fmt.Println("    LPUSH_FRONT mylist value - добавить элемент в начало")
	fmt.Println("    LPUSH_BACK mylist value  - добавить элемент в конец")
	fmt.Println("    LGET mylist 0            - получить элемент по индексу")
	fmt.Println("    LDEL mylist value        - удалить элемент по значению")
	fmt.Println("    PRINT mylist             - вывести список")
	fmt.Println()
	fmt.Println("  Стек (Stack - LIFO):")
	fmt.Println("    SPUSH mystack item       - добавить элемент в стек")
	fmt.Println("    SPOP mystack             - извлечь элемент из стека")
	fmt.Println("    PRINT mystack            - вывести стек")
	fmt.Println()
	fmt.Println("  Очередь (Queue - FIFO):")
	fmt.Println("    QPUSH myqueue element    - добавить элемент в очередь")
	fmt.Println("    QPOP myqueue             - извлечь элемент из очереди")
	fmt.Println("    PRINT myqueue            - вывести очередь")
	fmt.Println()
	fmt.Println("Примеры использования:")
	fmt.Println("  # Массив")
	fmt.Println("  ./dbms --file data.db --query 'MPUSH arr apple'")
	fmt.Println("  ./dbms --file data.db --query 'MGET arr 0'")
	fmt.Println("  ./dbms --file data.db --query 'MLEN arr'")
	fmt.Println()
	fmt.Println("  # Список")
	fmt.Println("  ./dbms --file data.db --query 'LPUSH_BACK list item1'")
	fmt.Println("  ./dbms --file data.db --query 'LPUSH_FRONT list item0'")
	fmt.Println("  ./dbms --file data.db --query 'LGET list 0'")
	fmt.Println()
	fmt.Println("  # Стек (LIFO)")
	fmt.Println("  ./dbms --file data.db --query 'SPUSH stack A'")
	fmt.Println("  ./dbms --file data.db --query 'SPUSH stack B'")
	fmt.Println("  ./dbms --file data.db --query 'SPOP stack'  # Вернёт B")
	fmt.Println()
	fmt.Println("  # Очередь (FIFO)")
	fmt.Println("  ./dbms --file data.db --query 'QPUSH queue X'")
	fmt.Println("  ./dbms --file data.db --query 'QPUSH queue Y'")
	fmt.Println("  ./dbms --file data.db --query 'QPOP queue'  # Вернёт X")
	fmt.Println()
	fmt.Println("  # Вывод любой структуры")
	fmt.Println("  ./dbms --file data.db --query 'PRINT arr'")
	fmt.Println("  ./dbms --file data.db --query 'PRINT stack'")
}

func main() {
	filename := flag.String("file", "", "Файл для хранения данных")
	query := flag.String("query", "", "SQL-подобная команда для выполнения")
	help := flag.Bool("help", false, "Показать справку")

	flag.Parse()

	if *help {
		printUsage()
		return
	}

	if *filename == "" || *query == "" {
		fmt.Println("ERROR: Missing required parameters")
		fmt.Println()
		printUsage()
		os.Exit(1)
	}

	// Создание БД
	db := NewDatabase()

	// Загрузка данных из файла
	if err := db.LoadFromFile(*filename); err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: Cannot load file: %v\n", err)
		os.Exit(1)
	}

	// Выполнение запроса
	result := db.ExecuteQuery(*query)

	// Сохранение данных в файл
	if err := db.SaveToFile(*filename); err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: Cannot save file: %v\n", err)
		os.Exit(1)
	}

	// Вывод результата
	fmt.Print(result)
	// Добавляем перевод строки только если результат не заканчивается на него
	if len(result) > 0 && result[len(result)-1] != '\n' {
		fmt.Println()
	}
}
