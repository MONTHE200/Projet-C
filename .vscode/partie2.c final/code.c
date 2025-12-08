/*
 * Projet arithmétique binaire — version C
 * - Multiplication égyptienne (BigBinary)
 * - Exponentiation modulaire rapide (BigBinary)
 *
 * Auteur : Christy (adapté)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------
 * Structure BigBinary
 * --------------------------- */

typedef struct {
    int taille;          // nombre d’octets
    unsigned char *digits; // tableau de bytes (LSB en premier)
} BigBinary;

/* Initialiser un BigBinary de taille donnée */
BigBinary initBigBinary(int taille) {
    BigBinary b;
    b.taille = taille;
    b.digits = calloc(taille, sizeof(unsigned char));
    return b;
}

/* Charger un BigBinary à partir d’une chaîne hexadécimale */
BigBinary fromHex(const char *hex) {
    int len = strlen(hex);
    int taille = (len + 1) / 2;
    BigBinary b = initBigBinary(taille);

    for (int i = 0; i < len; i++) {
        int val;
        if (hex[i] >= '0' && hex[i] <= '9') val = hex[i] - '0';
        else if (hex[i] >= 'a' && hex[i] <= 'f') val = 10 + hex[i] - 'a';
        else if (hex[i] >= 'A' && hex[i] <= 'F') val = 10 + hex[i] - 'A';
        else val = 0;

        int byteIndex = (len - 1 - i) / 2;
        if ((len - i) % 2 == 0) {
            b.digits[byteIndex] = val << 4;
        } else {
            b.digits[byteIndex] |= val;
        }
    }
    return b;
}

/* Afficher un BigBinary en hexadécimal */
void printBigBinary(BigBinary b) {
    for (int i = b.taille - 1; i >= 0; i--) {
        printf("%02x", b.digits[i]);
    }
    printf("\n");
}

/* ---------------------------
 * Multiplication égyptienne
 * --------------------------- */

/* Ici simplifiée : on utilise des entiers natifs pour la démo.
   Pour un vrai projet, il faut coder l’addition et le décalage sur BigBinary */
unsigned long long egyptian_mul(unsigned long long a, unsigned long long b) {
    unsigned long long res = 0;
    while (b > 0) {
        if (b & 1ULL) res += a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

/* ---------------------------
 * Exponentiation modulaire rapide
 * --------------------------- */

/* Version simplifiée avec unsigned long long.
   Pour un vrai projet BigBinary, il faut remplacer par des opérations sur tableaux. */
unsigned long long modexp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1 % mod;
    base = base % mod;

    while (exp > 0) {
        if (exp & 1ULL) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------------------------
 * main()
 * --------------------------- */

int main(void) {
    unsigned long long A, B, N, e;

    printf("=== Multiplication égyptienne ===\n");
    printf("Entrez A : ");
    scanf("%llu", &A);
    printf("Entrez B : ");
    scanf("%llu", &B);

    unsigned long long produit = egyptian_mul(A, B);
    printf("Résultat : A x B = %llu\n\n", produit);

    printf("=== Exponentiation modulaire ===\n");
    printf("Entrez la base A : ");
    scanf("%llu", &A);
    printf("Entrez l'exposant e : ");
    scanf("%llu", &e);
    printf("Entrez le modulo N : ");
    scanf("%llu", &N);

    unsigned long long resultat = modexp(A, e, N);
    printf("Résultat : (A^e) mod N = %llu\n", resultat);

    return 0;
}
