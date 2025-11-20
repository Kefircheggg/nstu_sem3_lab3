package structures

import (
	"encoding/json"
	"os"
	"strconv"
)

// SNode - узел односвязного списка
type SNode struct {
	data string
	next *SNode
}

// SinglyList - односвязный список
type SinglyList struct {
	head *SNode
}

// NewSinglyList создает новый односвязный список
func NewSinglyList() *SinglyList {
	return &SinglyList{head: nil}
}

// PushFront добавляет элемент в начало
func (l *SinglyList) PushFront(value string) {
	newNode := &SNode{data: value, next: l.head}
	l.head = newNode
}

// PushBack добавляет элемент в конец
func (l *SinglyList) PushBack(value string) {
	newNode := &SNode{data: value, next: nil}
	if l.head == nil {
		l.head = newNode
		return
	}
	current := l.head
	for current.next != nil {
		current = current.next
	}
	current.next = newNode
}

// Get возвращает элемент по индексу
func (l *SinglyList) Get(index int) (string, bool) {
	current := l.head
	for i := 0; current != nil; i++ {
		if i == index {
			return current.data, true
		}
		current = current.next
	}
	return "", false
}

// Delete удаляет элемент по значению
func (l *SinglyList) Delete(value string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == value {
		l.head = l.head.next
		return true
	}
	current := l.head
	for current.next != nil {
		if current.next.data == value {
			current.next = current.next.next
			return true
		}
		current = current.next
	}
	return false
}

// Values возвращает все элементы списка
func (l *SinglyList) Values() []string {
	var values []string
	current := l.head
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return values
}

// Print выводит список
func (l *SinglyList) Print(name string) string {
	result := "Singly Linked List [" + name + "]:\n"
	current := l.head
	index := 0
	for current != nil {
		result += "  [" + strconv.Itoa(index) + "] -> " + current.data + "\n"
		current = current.next
		index++
	}
	if index == 0 {
		result += "  (empty)\n"
	}
	return result
}

// Serialize сохраняет список в JSON файл
func (l *SinglyList) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	return encoder.Encode(l.Values())
}

// Deserialize загружает список из JSON файла
func (l *SinglyList) Deserialize(filename string) error {
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
	l.head = nil
	for _, v := range values {
		l.PushBack(v)
	}
	return nil
}

// DNode - узел двусвязного списка
type DNode struct {
	data string
	next *DNode
	prev *DNode
}

// DoublyList - двусвязный список
type DoublyList struct {
	head *DNode
	tail *DNode
}

// NewDoublyList создает новый двусвязный список
func NewDoublyList() *DoublyList {
	return &DoublyList{head: nil, tail: nil}
}

// PushFront добавляет элемент в начало
func (l *DoublyList) PushFront(value string) {
	newNode := &DNode{data: value, next: l.head, prev: nil}
	if l.head != nil {
		l.head.prev = newNode
	} else {
		l.tail = newNode
	}
	l.head = newNode
}

// PushBack добавляет элемент в конец
func (l *DoublyList) PushBack(value string) {
	newNode := &DNode{data: value, next: nil, prev: l.tail}
	if l.tail != nil {
		l.tail.next = newNode
	} else {
		l.head = newNode
	}
	l.tail = newNode
}

// Get возвращает элемент по индексу
func (l *DoublyList) Get(index int) (string, bool) {
	current := l.head
	for i := 0; current != nil; i++ {
		if i == index {
			return current.data, true
		}
		current = current.next
	}
	return "", false
}

// Delete удаляет элемент по значению
func (l *DoublyList) Delete(value string) bool {
	current := l.head
	for current != nil {
		if current.data == value {
			if current.prev != nil {
				current.prev.next = current.next
			} else {
				l.head = current.next
			}
			if current.next != nil {
				current.next.prev = current.prev
			} else {
				l.tail = current.prev
			}
			return true
		}
		current = current.next
	}
	return false
}

// Values возвращает все элементы списка
func (l *DoublyList) Values() []string {
	var values []string
	current := l.head
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return values
}

// Print выводит список
func (l *DoublyList) Print(name string) string {
	result := "Doubly Linked List [" + name + "]:\n"
	current := l.head
	index := 0
	for current != nil {
		result += "  [" + strconv.Itoa(index) + "] <-> " + current.data + "\n"
		current = current.next
		index++
	}
	if index == 0 {
		result += "  (empty)\n"
	}
	return result
}

// Serialize сохраняет список в JSON файл
func (l *DoublyList) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	return encoder.Encode(l.Values())
}

// Deserialize загружает список из JSON файла
func (l *DoublyList) Deserialize(filename string) error {
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
	l.head = nil
	l.tail = nil
	for _, v := range values {
		l.PushBack(v)
	}
	return nil
}
