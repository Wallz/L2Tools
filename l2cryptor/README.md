# l2cryptor
L2Cryptor - go package for working with Lineage 2 client files

Package clients have to implement RSAKey interface
```golang
type RSAKey interface {
	N() *big.Int // Modulus
	E() *big.Int // Public exponent
	D() *big.Int // Private exponent
}
```

### Example:
```golang
// Decrypting file
input := "castlename-e.dat"
output := ""

err := DecryptFile(input, key, output)
if err != nil {
  log.Fatal(err)
}

// Input file will saved in same dir with name `dec-castlename-e.dat`
```
