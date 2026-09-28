int bitAnd(int x, int y) {
    return ~(~x | ~y);
}

int bitXor(int x, int y) {
    return ~(~x & ~y) & ~(x & y);
}

int samesign(int x, int y) {
    if (!x) return !y;
    if (!y) return 0;
    return !((x ^ y) >> 31);
}

int logtwo(int v) {
    int r = 0;
    int s = (v > 0xFFFF) << 4;
    v = v >> s;
    r = r | s;
    s = (v > 0xFF) << 3;
    v = v >> s;
    r = r | s;
    s = (v > 0xF) << 2;
    v = v >> s;
    r = r | s;
    s = (v > 3) << 1;
    v = v >> s;
    r = r | s;
    return r | (v > 1);
}

int byteSwap(int x, int n, int m) {
    int ns = n << 3;
    int ms = m << 3;
    int a = (x >> ns) & 0xFF;
    int b = (x >> ms) & 0xFF;
    int delta = a ^ b;
    return x ^ (delta << ns) ^ (delta << ms);
}

unsigned reverse(unsigned v) {
    unsigned answer = 0;
    int i;
    for (i = 32; i; i = i - 1) {
        answer = (answer << 1) | (v & 1);
        v = v >> 1;
    }
    return answer;
}

int logicalShift(int x, int n) {
    return (x >> n) & ~(((1 << 31) >> n) << 1);
}

int leftBitCount(int x) {
    int n = 0;
    int b = !(~x >> 16) << 4;
    n = n + b;
    x = x << b;
    b = !(~x >> 24) << 3;
    n = n + b;
    x = x << b;
    b = !(~x >> 28) << 2;
    n = n + b;
    x = x << b;
    b = !(~x >> 30) << 1;
    n = n + b;
    x = x << b;
    b = !(~x >> 31);
    n = n + b;
    x = x << b;
    return n + (x >> 31 & 1);
}

unsigned float_i2f(int x) {
    unsigned sign = 0, mag, shifted, rem, halfway, exponent;
    int msb = 31, shift;
    if (!x) return 0;
    if (x < 0) sign = 0x80000000;
    mag = x;
    if (x < 0) mag = ~x + 1;
    while (!(mag >> msb)) msb = msb - 1;
    exponent = msb + 126;
    if (msb <= 23) return sign | ((exponent << 23) + (mag << (23 - msb)));
    shift = msb - 23;
    shifted = mag >> shift;
    rem = mag & ((1u << shift) - 1);
    halfway = 1u << (shift - 1);
    if (rem + (shifted & 1) > halfway) shifted = shifted + 1;
    return sign | ((exponent << 23) + shifted);
}

unsigned floatScale2(unsigned uf) {
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned sign = uf & 0x80000000;
    if (exp == 0xFF) return uf;
    if (!exp) return sign | ((uf & 0x7FFFFFFF) << 1);
    exp = exp + 1;
    if (exp == 0xFF) return sign | 0x7F800000;
    return sign | (exp << 23) | (uf & 0x7FFFFF);
}

int float64_f2i(unsigned uf1, unsigned uf2) {
    unsigned exp = (uf2 >> 20) & 0x7FF;
    unsigned high = (uf2 & 0xFFFFF) | 0x100000;
    unsigned value;
    int e = exp;
    e = e - 1023;
    if (exp > 0x7FE) return 0x80000000u;
    if (e > 31) return 0x80000000u;
    if (e < 0) return 0;
    if (e > 20) value = (high << (e - 20)) | (uf1 >> (52 - e));
    else value = high >> (20 - e);
    if (e > 30) return 0x80000000u;
    if (uf2 >> 31) {
        int signed_value = value;
        return -signed_value;
    }
    return value;
}

unsigned floatPower2(int x) {
    unsigned exponent;
    if (x < -149) return 0;
    if (x < -126) return 1u << (x + 149);
    if (x > 127) return 0x7F800000;
    exponent = x + 127;
    return exponent << 23;
}
