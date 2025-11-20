package structures

import (
	"testing"
)

// --- Array Tests ---
func TestArray(t *testing.T) {
	arr := NewArray()
	if arr.Len() != 0 {
		t.Errorf("Expected empty array, got len %d", arr.Len())
	}

	arr.Push("A")
	arr.Push("B")
	if arr.Len() != 2 {
		t.Errorf("Expected len 2, got %d", arr.Len())
	}

	val, ok := arr.Get(0)
	if !ok || val != "A" {
		t.Errorf("Expected A at 0, got %s", val)
	}

	val, ok = arr.Get(1)
	if !ok || val != "B" {
		t.Errorf("Expected B at 1, got %s", val)
	}

	val, ok = arr.Get(2)
	if ok {
		t.Error("Expected error for index 2")
	}

	if !arr.Delete(0) {
		t.Error("Failed to delete at 0")
	}
	if arr.Len() != 1 {
		t.Errorf("Expected len 1 after delete, got %d", arr.Len())
	}
	val, _ = arr.Get(0)
	if val != "B" {
		t.Errorf("Expected B at 0 after delete, got %s", val)
	}

	if arr.Delete(10) {
		t.Error("Expected false for delete out of bounds")
	}

	arr.Print("test")
	arr.Values()
}

// --- SinglyList Tests ---
func TestSinglyList(t *testing.T) {
	list := NewSinglyList()
	list.PushBack("A")
	list.PushFront("B") // B -> A

	val, ok := list.Get(0)
	if !ok || val != "B" {
		t.Errorf("Expected B at 0, got %s", val)
	}
	val, ok = list.Get(1)
	if !ok || val != "A" {
		t.Errorf("Expected A at 1, got %s", val)
	}

	if !list.Delete("B") {
		t.Error("Failed to delete B")
	}
	val, ok = list.Get(0)
	if !ok || val != "A" {
		t.Errorf("Expected A at 0 after delete, got %s", val)
	}

	if list.Delete("Z") {
		t.Error("Expected false for delete non-existent")
	}

	list.Print("test")
	list.Values()
}

// --- DoublyList Tests ---
func TestDoublyList(t *testing.T) {
	list := NewDoublyList()
	list.PushBack("A")
	list.PushFront("B") // B <-> A

	val, ok := list.Get(0)
	if !ok || val != "B" {
		t.Errorf("Expected B at 0, got %s", val)
	}
	val, ok = list.Get(1)
	if !ok || val != "A" {
		t.Errorf("Expected A at 1, got %s", val)
	}

	if !list.Delete("B") {
		t.Error("Failed to delete B")
	}
	val, ok = list.Get(0)
	if !ok || val != "A" {
		t.Errorf("Expected A at 0 after delete, got %s", val)
	}

	// Test tail update
	list.PushBack("C") // A <-> C
	if !list.Delete("C") {
		t.Error("Failed to delete C (tail)")
	}

	list.Print("test")
	list.Values()
}

// --- Stack Tests ---
func TestStack(t *testing.T) {
	stack := NewStack()
	if !stack.IsEmpty() {
		t.Error("Expected empty stack")
	}

	stack.Push("A")
	stack.Push("B")

	if stack.IsEmpty() {
		t.Error("Expected non-empty stack")
	}

	val, ok := stack.Pop()
	if !ok || val != "B" {
		t.Errorf("Expected B, got %s", val)
	}

	val, ok = stack.Pop()
	if !ok || val != "A" {
		t.Errorf("Expected A, got %s", val)
	}

	val, ok = stack.Pop()
	if ok {
		t.Error("Expected empty stack pop to fail")
	}

	stack.Print("test")
	stack.Values()
}

// --- Queue Tests ---
func TestQueue(t *testing.T) {
	q := NewQueue()
	if !q.IsEmpty() {
		t.Error("Expected empty queue")
	}

	q.Push("A")
	q.Push("B")

	val, ok := q.Pop()
	if !ok || val != "A" {
		t.Errorf("Expected A, got %s", val)
	}

	val, ok = q.Pop()
	if !ok || val != "B" {
		t.Errorf("Expected B, got %s", val)
	}

	val, ok = q.Pop()
	if ok {
		t.Error("Expected empty queue pop to fail")
	}

	q.Print("test")
	q.Values()
}

// --- Benchmarks ---

func BenchmarkArrayPush(b *testing.B) {
	arr := NewArray()
	for i := 0; i < b.N; i++ {
		arr.Push("test")
	}
}

func BenchmarkSinglyListPushBack(b *testing.B) {
	list := NewSinglyList()
	for i := 0; i < b.N; i++ {
		list.PushBack("test")
	}
}
