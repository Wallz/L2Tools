package l2cryptor

import (
	"os"
	"path/filepath"
	"testing"
)

func checkExt(ext string, filter []string) bool {
	for _, f := range filter {
		if f == ext {
			return true
		}
	}
	return false
}

func testOriginalFiles(t *testing.T, filter []string) {
	key := &key413

	os.Chdir("original_files")
	defer os.Chdir("..")

	entries, _ := os.ReadDir(".")

	for _, entry := range entries {
		if entry.IsDir() || !checkExt(filepath.Ext(entry.Name()), filter) {
			continue
		}

		if err := DecryptFile(entry.Name(), key, ""); err != nil {
			t.Log(err)
		}

		out := "dec." + entry.Name()
		if _, err := os.Stat(out); os.IsNotExist(err) {
			t.Error(entry.Name(), "NOT FOUND! FAIL!")
		} else {
			t.Log(entry.Name(), "PASS!")
			os.Remove(out)
		}
	}
}

func TestDecryptOriginalDatFiles(t *testing.T) {
	testOriginalFiles(t, []string{".dat"})
}

func TestDecryptOriginalIniFiles(t *testing.T) {
	testOriginalFiles(t, []string{".ini"})
}
