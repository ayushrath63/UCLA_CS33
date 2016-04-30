# Floating Point

##Fractional Binary Numbers

Examples:

23/4 -> 0b101.11

## IEEE Floating Point

Numerical form: _(-1)<sup>s</sup> M 2<sup>E</sup>_

- Sign bit _s_ determines if negative or positive
- Significand _M_ encodes fractional value in range [1.0.2.0)
- Exponent E weights value by power of 2

Encoding:

s|exp|frac
-|-|-
MSB is sign bit _s_ |Encodes _E_ (not equal to E)| Encodes _M_ (not equal to M)

#### "Normalized" Values

- exp is not all 0 or all 1

Exponent coded as biased value
 - E = exp - bias
 - exp = unsigned value of exp field
 - bias = _2<sup>k-1</sup> - 1_, _k_ is the number of exponent bits
  - 127 for single precision, 1023 for double precision

Significand encoded with implied leading 1: M= 1.xxxx..., where xxxx... is stored in frac
