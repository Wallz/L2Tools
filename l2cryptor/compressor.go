package l2cryptor

import (
	"bytes"
	"compress/zlib"
	"encoding/binary"
	"io"
)

func decompress(comp []byte) ([]byte, error) {
	if len(comp) < 4 {
		return nil, errCompressedDataBlockTooSmall
	}

	// TODO: выяснить почему не всегда совпадает указанный размер с действительным
	// first 4 bytes is size of decompressed data in binary 32-bit little endian
	// sz := binary.LittleEndian.Uint32(comp[:4])

	z, err := zlib.NewReader(bytes.NewBuffer(comp[4:]))
	if err != nil {
		return nil, err
	}
	defer z.Close()

	o := bytes.NewBuffer([]byte{})
	io.Copy(o, z)

	// TODO: выяснить почему не всегда совпадает указанный размер с действительным
	// if len(o.Bytes()) != int(sz) {
	// 	return nil, errCommonDecompressionWrongSize
	// }

	return o.Bytes(), nil
}

// unused now
func compress(data []byte) ([]byte, error) {
	comp := bytes.Buffer{}
	// write header with data size
	if err := binary.Write(&comp, binary.LittleEndian, int32(len(data))); err != nil {
		return nil, err
	}

	z := zlib.NewWriter(&comp)
	if _, err := z.Write(data); err != nil {
		return nil, err
	}
	defer z.Close()

	return comp.Bytes(), nil
}
