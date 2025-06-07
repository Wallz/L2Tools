package l2cryptor

import (
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"golang.org/x/text/encoding/unicode"
)

type lineageFileInfo struct {
	os.FileInfo
	l2Version     int
	contentLength int
}

func (i *lineageFileInfo) blocksCount() int {
	if i.Size() == 0 {
		return 0
	}
	return i.contentLength / blockSize
}

func (i *lineageFileInfo) tailSize() int {
	// content tail. it migth contain CRC
	return i.contentLength % blockSize
}

// openLineageFile reads file, collect specific encrypted lineage file info
//  and return payload
func openLineageFile(filename string) (content []byte, info lineageFileInfo, err error) {
	var inf os.FileInfo
	inf, err = os.Stat(filename)
	if err != nil {
		return
	}
	info.FileInfo = inf

	sz := inf.Size()
	switch {
	case sz == 0:
		err = errInputFileIsEmpty
		return
	// can contain header
	case sz < 28:
		err = errInputFileTooSmallHeader
		return
	// can contain header at least one block
	case sz < fileHeaderSize+blockSize:
		err = errInputFileTooSmallContent
		return
	}

	f, err := os.Open(filename)
	if err != nil {
		return
	}
	defer f.Close()

	buf := make([]byte, sz) // allocate buffer for read content
	info.contentLength, err = f.Read(buf)
	if err != nil {
		return
	}

	// get bytes utf16le header...
	bytesHeader := buf[:28]
	// and convert into go string
	header16, _ := unicode.UTF16(unicode.LittleEndian, unicode.IgnoreBOM).
		NewDecoder().
		Bytes(bytesHeader)
	stringVer := strings.TrimPrefix(string(header16), "Lineage2Ver")

	info.l2Version, err = strconv.Atoi(stringVer)
	if err != nil {
		err = errInputFileFailDetectVersion
		return
	}
	content = buf[28:]
	return
}

// DecryptFile open file tnen decrypt and decompress file data
// `input` - encrypted Lineage2 file
// `key` - RSA key interface (must contain implementation for modulus N() and private exponent E())
// `output` - filename for save decrypted data. If empty, it will looks like 'dec.`input`'
func DecryptFile(input string, key RSAKey, output string) error {
	cipher, _, err := openLineageFile(input)
	if err != nil {
		return err
	}

	comp, err := decryptSequence(cipher, key)
	if err != nil {
		return err
	}

	data, err := decompress(comp)
	if err != nil {
		return err
	}

	if output == "" {
		output = fmt.Sprintf("%s/dec.%s", filepath.Dir(input), filepath.Base(input))
	}
	if err := os.WriteFile(output, data, 0755); err != nil {
		return err
	}

	return nil
}
