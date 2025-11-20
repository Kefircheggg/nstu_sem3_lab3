package structures

import (
	"encoding/json"
	"os"
	"strconv"
)

// StackNode - узел стека
type StackNode struct {
	data string
	next *StackNode
}

// Stack - стек (LIFO)
type Stack struct {
	top *StackNode
}

// NewStack создает новый стек
func NewStack() *Stack {
	return &Stack{top: nil}
}

// Push добавляет элемент в стек
func (s *Stack) Push(value string) string {
	newNode := &StackNode{data: value, next: s.top}
	s.top = newNode
	return value
}

// Pop удаляет и возвращает верхний элемент
func (s *Stack) Pop() (string, bool) {
	if s.top == nil {
		return "", false
	}
	value := s.top.data
	s.top = s.top.next
	return value, true
}

// IsEmpty проверяет, пуст ли стек
func (s *Stack) IsEmpty() bool {
	return s.top == nil
}

// Values возвращает все элементы стека (от вершины к дну)
func (s *Stack) Values() []string {
	var values []string
	current := s.top
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return values
}

// Print выводит стек
func (s *Stack) Print(name string) string {
	result := "Stack [" + name + "] (top to bottom):\n"
	current := s.top
	index := 0
	for current != nil {
		result += "  [" + strconv.Itoa(index) + "] | " + current.data + "\n"
		current = current.next
		index++
	}
	if index == 0 {
		result += "  (empty)\n"
	}
	return result
}

// Serialize сохраняет стек в JSON файл
func (s *Stack) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	return encoder.Encode(s.Values())
}

// Deserialize загружает стек из JSON файла
func (s *Stack) Deserialize(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	decoder := json.NewDecoder(file)
	var values []string
	if err := decoder.Decode(&values); err != nil {
		return err
	}
	s.top = nil
	// Iterate in reverse to restore stack order
	for i := len(values) - 1; i >= 0; i-- {
		s.Push(values[i])
	}
	return nil
}
