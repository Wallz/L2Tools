package l2cryptor

import "math/big"

func encrypt(data []byte, key RSAKey) []byte {
	return new(big.Int).Exp(
		new(big.Int).SetBytes(data), key.E(), key.N()).Bytes()
}

func ecryptSeqence(data []byte, key RSAKey) ([]byte, error) {
	return nil, nil
}
