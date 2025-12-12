#include <stdio.h>

// Algorithme Binaire d’Euclide pour le PGCD
unsigned int pgcd_binaire(unsigned long long  a, unsigned long long  b) {
    if (a == 0) return b;
    if (b == 0) return a;

    int shift;
    for (shift = 0; ((a | b) & 1) == 0; ++shift) {
        a >>= 1;
        b >>= 1;
    }

    while ((a & 1) == 0)
        a >>= 1;

    do {
        while ((b & 1) == 0)
            b >>= 1;

        if (a > b) {
            unsigned int temp = a;
            a = b;
            b = temp;
        }

        b = b - a;
    } while (b != 0);

    return a << shift;
}

// Modulo de A par N
unsigned long long  modulo(unsigned long long  a, unsigned long long  n) {
    return a % n;
}

// Multiplication Égyptienne
unsigned long long  multiplication_egyptienne(unsigned long long  a, unsigned long long  b) {
    unsigned int result = 0;
    while (b > 0) {
        if (b & 1)
            result += a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}

// Exponentiation rapide modulaire
unsigned long long exp_modulaire(unsigned long long base, unsigned long long  exp, unsigned long long mod) {
    unsigned long long  result = 1;
    while (exp > 0) {
        if (exp & 1)
            for (int i = 0; i < exp; i++) {
                result = (result * base) % mod;
            }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Exemple d’utilisation
int main() {
    int a, b;
    printf("Entrez votre premier chiffre: ");
    scanf("%u",&a);
    printf("Entrez votre second chiffre: ");
    scanf("%u",&b);
    printf("PGCD binaire de %u et %u = %u\n", &a, &b, pgcd_binaire(a, b));
    printf("Modulo de %u par %u = %u\n", &a, &b, modulo(a, b));
    printf("Multiplication Egyptienne de %u et %u = %u\n", &a, &b, multiplication_egyptienne(a, b));
    printf("Exponentiation rapide modulaire: (%u^%d) mod %u = %u\n", &a, 17, &b, exp_modulaire(a, 17, b));
    return 0;
}
