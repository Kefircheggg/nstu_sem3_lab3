package structures

import (
	"encoding/json"
	"os"
	"strconv"
)

// Array - динамический массив
type Array struct {
	data []string
}

// NewArray создает новый массив
func NewArray() *Array {
	return &Array{
		data: make([]string, 0),
	}
}

// Push добавляет элемент в конец массива
func (a *Array) Push(value string) {
	a.data = append(a.data, value)
}

// Get возвращает элемент по индексу
func (a *Array) Get(index int) (string, bool) {
	if index < 0 || index >= len(a.data) {
		return "", false
	}
	return a.data[index], true
}

// Delete удаляет элемент по индексу
func (a *Array) Delete(index int) bool {
	if index < 0 || index >= len(a.data) {
		return false
	}
	a.data = append(a.data[:index], a.data[index+1:]...)
	return true
}

// Len возвращает длину массива
func (a *Array) Len() int {
	return len(a.data)
}

// Values возвращает все элементы массива
func (a *Array) Values() []string {
	return a.data
}

// Print выводит массив
func (a *Array) Print(name string) string {
	result := "Array [" + name + "] (size: " + strconv.Itoa(len(a.data)) + "):\n"
	for i, val := range a.data {
		result += "  [" + strconv.Itoa(i) + "] = " + val + "\n"
	}
	if len(a.data) == 0 {
		result += "  (empty)\n"
	}
	return result
}

// Serialize сохраняет массив в JSON файл
func (a *Array) Serialize(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	return encoder.Encode(a.data)
}

// Deserialize загружает массив из JSON файла
func (a *Array) Deserialize(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	decoder := json.NewDecoder(file)
	var data []string
	if err := decoder.Decode(&data); err != nil {
		return err
	}
	a.data = data
	return nil
}
