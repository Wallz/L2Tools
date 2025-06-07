package l2cryptor

import (
	"errors"
	"fmt"
)

var (
	errInputFileIsEmpty           = errors.New("input file is empty")
	errInputFileTooSmallHeader    = fmt.Errorf("input file has to small length (a little than header size - %d)", fileHeaderSize)
	errInputFileTooSmallContent   = errors.New("input file content is too small")
	errInputFileFailDetectVersion = errors.New("could't detect lineage version")

	errDetectDecryptedBlock = func(n int) error {
		return fmt.Errorf("block  with key %d is't detected in decpypted blocks map", n)
	}

	errDecryptBlockTooShort = func(x int) error {
		return fmt.Errorf("decrypted block is too small %d", x)
	}

	errDecryptBlockTooLarge = errors.New("size in block header more than meaningful block part")

	errReadBlockMessageTooLong = func(spec, actual int) error {
		return fmt.Errorf("decrypt error: specified payload length - %d; actual - %d", spec, actual)
	}

	errCommonDecompressionWrongSize = errors.New("decompression error. wrong decompressed size")
	errCompressedDataBlockTooSmall  = errors.New("compressed data block is too small")
)
