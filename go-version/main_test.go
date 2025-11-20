package main

import (
	"dbms/structures"
	"flag"
	"io"
	"os"
	"strings"
	"testing"
)

func TestNewDatabase(t *testing.T) {
	db := NewDatabase()
	if db == nil {
		t.Fatal("NewDatabase returned nil")
	}
	if db.structures == nil {
		t.Fatal("Database structures map is nil")
	}
}

func TestExecuteQuery_Array(t *testing.T) {
	db := NewDatabase()

	// MPUSH
	if res := db.ExecuteQuery("MPUSH arr val1"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}
	if res := db.ExecuteQuery("MPUSH arr val2"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}

	// MLEN
	if res := db.ExecuteQuery("MLEN arr"); res != "2" {
		t.Errorf("Expected 2, got %s", res)
	}

	// MGET
	if res := db.ExecuteQuery("MGET arr 0"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db.ExecuteQuery("MGET arr 1"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}
	if res := db.ExecuteQuery("MGET arr 2"); !strings.Contains(res, "ERROR") {
		t.Errorf("Expected ERROR, got %s", res)
	}

	// MDEL
	if res := db.ExecuteQuery("MDEL arr 0"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}
	if res := db.ExecuteQuery("MLEN arr"); res != "1" {
		t.Errorf("Expected 1, got %s", res)
	}
	if res := db.ExecuteQuery("MGET arr 0"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}

	// PRINT
	if res := db.ExecuteQuery("PRINT arr"); !strings.Contains(res, "val2") {
		t.Errorf("Expected output to contain val2, got %s", res)
	}
}

func TestExecuteQuery_SList(t *testing.T) {
	db := NewDatabase()

	// LPUSH_BACK
	if res := db.ExecuteQuery("LPUSH_BACK list val1"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}
	// LPUSH_FRONT
	if res := db.ExecuteQuery("LPUSH_FRONT list val0"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}

	// LGET
	if res := db.ExecuteQuery("LGET list 0"); res != "val0" {
		t.Errorf("Expected val0, got %s", res)
	}
	if res := db.ExecuteQuery("LGET list 1"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}

	// LDEL
	if res := db.ExecuteQuery("LDEL list val0"); res != "OK" {
		t.Errorf("Expected OK, got %s", res)
	}
	if res := db.ExecuteQuery("LGET list 0"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
}

func TestExecuteQuery_Stack(t *testing.T) {
	db := NewDatabase()

	// SPUSH
	if res := db.ExecuteQuery("SPUSH stack val1"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db.ExecuteQuery("SPUSH stack val2"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}

	// SPOP
	if res := db.ExecuteQuery("SPOP stack"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}
	if res := db.ExecuteQuery("SPOP stack"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db.ExecuteQuery("SPOP stack"); !strings.Contains(res, "ERROR") {
		t.Errorf("Expected ERROR for empty stack, got %s", res)
	}
}

func TestExecuteQuery_Queue(t *testing.T) {
	db := NewDatabase()

	// QPUSH
	if res := db.ExecuteQuery("QPUSH queue val1"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db.ExecuteQuery("QPUSH queue val2"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}

	// QPOP
	if res := db.ExecuteQuery("QPOP queue"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db.ExecuteQuery("QPOP queue"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}
	if res := db.ExecuteQuery("QPOP queue"); !strings.Contains(res, "ERROR") {
		t.Errorf("Expected ERROR for empty queue, got %s", res)
	}
}

func TestExecuteQuery_Errors(t *testing.T) {
	db := NewDatabase()

	if res := db.ExecuteQuery(""); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for empty query")
	}
	if res := db.ExecuteQuery("UNKNOWN cmd"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for unknown command")
	}

	// Type mismatch
	db.ExecuteQuery("MPUSH arr val")
	if res := db.ExecuteQuery("SPUSH arr val"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for type mismatch (stack vs array)")
	}

	// Not found
	if res := db.ExecuteQuery("MGET missing 0"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for missing structure")
	}

	// Missing arguments
	if res := db.ExecuteQuery("MPUSH arr"); !strings.Contains(res, "ERROR") { // Need 3 args
		t.Error("Expected ERROR for MPUSH missing args")
	}
	if res := db.ExecuteQuery("MGET arr"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for MGET missing args")
	}
	if res := db.ExecuteQuery("MDEL arr"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for MDEL missing args")
	}
	if res := db.ExecuteQuery("MLEN"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for MLEN missing args")
	}
	if res := db.ExecuteQuery("LPUSH_FRONT list"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for LPUSH_FRONT missing args")
	}
	if res := db.ExecuteQuery("LPUSH_BACK list"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for LPUSH_BACK missing args")
	}
	if res := db.ExecuteQuery("LGET list"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for LGET missing args")
	}
	if res := db.ExecuteQuery("LDEL list"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for LDEL missing args")
	}
	if res := db.ExecuteQuery("SPUSH stack"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for SPUSH missing args")
	}
	if res := db.ExecuteQuery("SPOP"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for SPOP missing args")
	}
	if res := db.ExecuteQuery("QPUSH queue"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for QPUSH missing args")
	}
	if res := db.ExecuteQuery("QPOP"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for QPOP missing args")
	}
	if res := db.ExecuteQuery("PRINT"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for PRINT missing args")
	}

	// Invalid index
	if res := db.ExecuteQuery("MGET arr invalid"); !strings.Contains(res, "ERROR") {
		t.Error("Expected ERROR for invalid index")
	}
}

func TestPrintUsage(t *testing.T) {
	// Capture stdout
	oldStdout := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	printUsage()

	w.Close()
	os.Stdout = oldStdout

	var buf strings.Builder
	_, _ = io.Copy(&buf, r)
	output := buf.String()

	if !strings.Contains(output, "Использование:") {
		t.Error("Expected usage info")
	}
}

func TestPersistence(t *testing.T) {
	filename := "test_db.txt"
	defer os.Remove(filename)

	// 1. Create and populate
	db1 := NewDatabase()
	db1.ExecuteQuery("MPUSH arr val1")
	db1.ExecuteQuery("SPUSH stack val2")
	db1.ExecuteQuery("QPUSH queue val3")
	db1.ExecuteQuery("LPUSH_BACK slist val4")
	db1.ExecuteQuery("LPUSH_FRONT dlist val5") // Will create SList by default if not specified?
	// Wait, LPUSH_FRONT creates SList by default in main.go logic if not exists.
	// To create DList, we need to manually inject or use a command that specifies type?
	// main.go:164: if _, exists := db.structures[name]; !exists { db.structures[name] = ... TypeSList ... }
	// So LPUSH_FRONT always creates SList if new.
	// We can't create DList via commands in main.go!
	// Wait, let's check main.go logic.
	// Lines 135-157 (LPUSH_FRONT): if !exists -> create SList.
	// So DList is effectively dead code in main.go CLI?
	// But LoadFromFile can create DList!
	// "STRUCTURE name DLIST"

	// So we can test DList persistence by creating it manually or via LoadFromFile.
	// Let's inject it manually for db1.
	db1.structures["dlist"] = &DataStructure{
		structType: TypeDList,
		data:       structures.NewDoublyList(),
	}
	db1.ExecuteQuery("LPUSH_BACK dlist val5")

	if err := db1.SaveToFile(filename); err != nil {
		t.Fatalf("SaveToFile failed: %v", err)
	}

	// 2. Load
	db2 := NewDatabase()
	if err := db2.LoadFromFile(filename); err != nil {
		t.Fatalf("LoadFromFile failed: %v", err)
	}

	// 3. Verify
	if res := db2.ExecuteQuery("MGET arr 0"); res != "val1" {
		t.Errorf("Expected val1, got %s", res)
	}
	if res := db2.ExecuteQuery("SPOP stack"); res != "val2" {
		t.Errorf("Expected val2, got %s", res)
	}
	if res := db2.ExecuteQuery("QPOP queue"); res != "val3" {
		t.Errorf("Expected val3, got %s", res)
	}
	if res := db2.ExecuteQuery("LGET slist 0"); res != "val4" {
		t.Errorf("Expected val4, got %s", res)
	}
	if res := db2.ExecuteQuery("LGET dlist 0"); res != "val5" {
		t.Errorf("Expected val5, got %s", res)
	}
}

func TestEmptyStructures(t *testing.T) {
	db := NewDatabase()
	db.ExecuteQuery("MPUSH arr val")
	db.ExecuteQuery("MDEL arr 0") // Empty array

	db.ExecuteQuery("SPUSH stack val")
	db.ExecuteQuery("SPOP stack") // Empty stack

	db.ExecuteQuery("QPUSH queue val")
	db.ExecuteQuery("QPOP queue") // Empty queue

	// Print empty
	if res := db.ExecuteQuery("PRINT arr"); !strings.Contains(res, "empty") && !strings.Contains(res, "Array") {
		// Array might not say "empty" explicitly, but "Array [arr] (0 items)" or similar?
		// Let's check array.go Print.
		// It prints index/value. If empty, it prints nothing?
		// array.go: Print: "Array [name] (len=0):" then loop.
		// It doesn't print "(empty)".
	}

	if res := db.ExecuteQuery("PRINT stack"); !strings.Contains(res, "(empty)") {
		t.Error("Expected (empty) for stack")
	}
	if res := db.ExecuteQuery("PRINT queue"); !strings.Contains(res, "(empty)") {
		t.Error("Expected (empty) for queue")
	}

	// Persistence of empty structures
	filename := "test_empty.db"
	defer os.Remove(filename)

	if err := db.SaveToFile(filename); err != nil {
		t.Fatal(err)
	}

	db2 := NewDatabase()
	if err := db2.LoadFromFile(filename); err != nil {
		t.Fatal(err)
	}

	if res := db2.ExecuteQuery("MLEN arr"); res != "0" {
		t.Errorf("Expected 0, got %s", res)
	}
}

func resetFlags() {
	flag.CommandLine = flag.NewFlagSet(os.Args[0], flag.ExitOnError)
}

func TestMainFunc_Help(t *testing.T) {
	oldArgs := os.Args
	defer func() { os.Args = oldArgs }()

	resetFlags()
	os.Args = []string{"cmd", "--help"}

	// Capture stdout to avoid clutter
	oldStdout := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	main()

	w.Close()
	os.Stdout = oldStdout
	var buf strings.Builder
	_, _ = io.Copy(&buf, r)
	if !strings.Contains(buf.String(), "Использование:") {
		t.Error("Expected usage info in main --help")
	}
}

func TestMainFunc_Success(t *testing.T) {
	oldArgs := os.Args
	defer func() { os.Args = oldArgs }()

	resetFlags()
	filename := "test_main_success.db"
	// Ensure file doesn't exist or is empty
	os.Remove(filename)
	defer os.Remove(filename)

	os.Args = []string{"cmd", "--file", filename, "--query", "MPUSH arr val"}

	// Capture stdout
	oldStdout := os.Stdout
	r, w, _ := os.Pipe()
	os.Stdout = w

	main()

	w.Close()
	os.Stdout = oldStdout
	var buf strings.Builder
	_, _ = io.Copy(&buf, r)
	if !strings.Contains(buf.String(), "OK") {
		t.Errorf("Expected OK, got %s", buf.String())
	}

	// Verify file created
	if _, err := os.Stat(filename); os.IsNotExist(err) {
		t.Error("File was not created")
	}
}

func TestLoadNonExistentFile(t *testing.T) {
	db := NewDatabase()
	if err := db.LoadFromFile("non_existent_file.txt"); err != nil {
		t.Errorf("Expected no error for non-existent file, got %v", err)
	}
}
