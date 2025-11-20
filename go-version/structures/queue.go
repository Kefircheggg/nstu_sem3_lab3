package structures

import (
	"encoding/json"
	"os"
	"strconv"
)

// QueueNode - узел очереди
type QueueNode struct {
	data string
	next *QueueNode
}

// Queue - очередь (FIFO)
type Queue struct {
	front *QueueNode
	rear  *QueueNode
}

// NewQueue создает новую очередь
func NewQueue() *Queue {
	return &Queue{front: nil, rear: nil}
}

// Push добавляет элемент в очередь
func (q *Queue) Push(value string) string {
	newNode := &QueueNode{data: value, next: nil}
	if q.rear == nil {
		q.front = newNode
		q.rear = newNode
	} else {
		q.rear.next = newNode
		q.rear = newNode
	}
	return value
}

// Pop удаляет и возвращает первый элемент
func (q *Queue) Pop() (string, bool) {
	if q.front == nil {
		return "", false
	}
	value := q.front.data
	q.front = q.front.next
	if q.front == nil {
		q.rear = nil
	}
	return value, true
}

// IsEmpty проверяет, пуста ли очередь
func (q *Queue) IsEmpty() bool {
	return q.front == nil
}

// Values возвращает все элементы очереди (от начала к концу)
func (q *Queue) Values() []string {
	var values []string
	current := q.front
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return values
}

// Print выводит очередь
func (q *Queue) Print(name string) string {
	result := "Queue [" + name + "] (front to rear):\n"
	values := q.Values()
	for i, val := range values {
		result += "  [" + strconv.Itoa(i) + "] -> " + val + "\n"
	}
	if len(values) == 0 {
		result += "  (empty)\n"
	}
	return result
}

// Serialize сохраняет очередь в JSON файл
func (q *Queue) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	return encoder.Encode(q.Values())
}

// Deserialize загружает очередь из JSON файла
func (q *Queue) Deserialize(filename string) error {
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
	q.front = nil
	q.rear = nil
	for _, v := range values {
		q.Push(v)
	}
	return nil
}
