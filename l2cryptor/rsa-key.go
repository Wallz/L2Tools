package l2cryptor

import "math/big"

type RSAKey interface {
	N() *big.Int // Modulus
	E() *big.Int // Public exponent
	D() *big.Int // Private exponent
}
