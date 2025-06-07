package l2cryptor

import (
	"os"
	"testing"
)

func TestFileInfo(t *testing.T) {
	os.Chdir("original_files")
	defer os.Chdir("..")

	validVersions := []int{111, 112, 411, 412, 413, 414}

	isValidVersion := func(ver int, valids []int) bool {
		for _, v := range valids {
			if ver == v {
				return true
			}
		}
		return false
	}

	entries, err := os.ReadDir(".")
	if err != nil {
		t.Error(err)
	}

	for _, entry := range entries {
		if entry.IsDir() {
			continue
		}

		_, inf, err := openLineageFile(entry.Name())
		if err != nil {
			t.Error(err)
		}
		t.Logf("file: %-25s | ver: %d | len: %-10d | blocks: %-5d |\n", inf.Name(), inf.l2Version, inf.contentLength, inf.blocksCount())
		if !isValidVersion(inf.l2Version, validVersions) {
			t.Errorf("File has wrong version: %d\n", inf.l2Version)
		}
	}
}
