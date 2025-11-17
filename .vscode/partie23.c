#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define BASE 2

typedef struct {
    int *Tdigits;   
    int Taille;     
    int Signe;      
} BigBinary;

/* --- Fonctions utilitaires --- */

BigBinary createBigBinary(int size) {
    BigBinary bb;
    if (size <= 0) size = 1;
    bb.Tdigits = calloc((size_t)size, sizeof(int));
    bb.Taille = size;
    bb.Signe = 0;
    return bb;
}

void libereBigBinary(BigBinary *nb) {
    if (!nb) return;
    if (nb->Tdigits) free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

BigBinary creerBigBinaryDepuisEntier(int n) {
    BigBinary nb;
    if (n == 0) {
        nb.Taille = 1;
        nb.Tdigits = malloc(sizeof(int));
        nb.Tdigits[0] = 0;
        nb.Signe = 0;
        return nb;
    }
    nb.Signe = (n < 0) ? -1 : +1;
    unsigned int val = (n < 0) ? -n : n;
    int bits = 0;
    unsigned int tmp = val;
    while (tmp > 0) {
        bits++;
        tmp >>= 1;
    }
    nb.Taille = bits;
    nb.Tdigits = malloc(sizeof(int) * bits);
    for (int i = bits - 1; i >= 0; i--) {
        nb.Tdigits[i] = val % 2;
        val /= 2;
    }
    return nb;
}

bool Egal(BigBinary A, BigBinary B) {
    if (A.Signe != B.Signe) return false;
    if (A.Taille != B.Taille) return false;
    for (int i = 0; i < A.Taille; ++i)
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    return true;
}

bool Inferieur(BigBinary A, BigBinary B) {
    if (A.Taille != B.Taille) return A.Taille < B.Taille;
    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }
    return false;
}

BigBinary Soustraction(BigBinary A, BigBinary B) {
    bool negatif = false;
    if (Inferieur(A, B)) {
        BigBinary tmp = A;
        A = B;
        B = tmp;
        negatif = true;
    }
    BigBinary R = createBigBinary(A.Taille);
    int borrow = 0;
    for (int i = 0; i < A.Taille; ++i) {
        int ai = A.Tdigits[A.Taille - 1 - i];
        int bi = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        int diff = ai - bi - borrow;
        if (diff < 0) { diff += 2; borrow = 1; } else borrow = 0;
        R.Tdigits[A.Taille - 1 - i] = diff;
    }
    R.Signe = negatif ? -1 : +1;
    return R;
}

BigBinary cloneBigBinary(const BigBinary *src) {
    BigBinary dst = createBigBinary(src->Taille);
    for (int i = 0; i < src->Taille; ++i) dst.Tdigits[i] = src->Tdigits[i];
    dst.Signe = src->Signe;
    return dst;
}

BigBinary multiplierPar2PuissanceK(const BigBinary b, int k) {
    if (k <= 0) return cloneBigBinary(&b);
    BigBinary resultat = createBigBinary(b.Taille + k);
    for (int i = 0; i < b.Taille; ++i) resultat.Tdigits[i] = b.Tdigits[i];
    resultat.Signe = b.Signe;
    return resultat;
}

BigBinary Modulo(const BigBinary a, const BigBinary b) {
    if (a.Signe <= 0 || b.Signe <= 0) {
        BigBinary z = createBigBinary(1);
        z.Tdigits[0] = 0; z.Signe = 0;
        return z;
    }
    if (Inferieur(a, b)) return cloneBigBinary(&a);
    if (Egal(a, b)) {
        BigBinary z = createBigBinary(1);
        z.Tdigits[0] = 0; z.Signe = 0;
        return z;
    }

    BigBinary reste = cloneBigBinary(&a);
    while (!Inferieur(reste, b)) {
        int k = reste.Taille - b.Taille;
        if (k < 0) k = 0;
        BigBinary produit = multiplierPar2PuissanceK(b, k);
        while (k > 0 && Inferieur(reste, produit)) {
            libereBigBinary(&produit);
            --k;
            produit = multiplierPar2PuissanceK(b, k);
        }
        BigBinary nouveauReste = Soustraction(reste, produit);
        libereBigBinary(&produit);
        libereBigBinary(&reste);
        reste = nouveauReste;
    }
    return reste;
}

BigBinary Addition(BigBinary A, BigBinary B) {
    int n = (A.Taille > B.Taille ? A.Taille : B.Taille) + 1;
    BigBinary R = createBigBinary(n);
    int carry = 0;
    for (int i = 0; i < n; i++) {
        int a = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        int b = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        int s = a + b + carry;
        R.Tdigits[n - 1 - i] = s % 2;
        carry = s / 2;
    }
    R.Signe = +1;
    return R;
}

int comparerBigBinary(BigBinary A, BigBinary B) {
    if (A.Signe < B.Signe) return -1;
    if (A.Signe > B.Signe) return 1;
    if (A.Taille < B.Taille) return -1 * A.Signe;
    if (A.Taille > B.Taille) return 1 * A.Signe;
    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] < B.Tdigits[i]) return -1 * A.Signe;
        if (A.Tdigits[i] > B.Tdigits[i]) return 1 * A.Signe;
    }
    return 0;
}

void afficheBigBinary(BigBinary nb) {
    for (int i = 0; i < nb.Taille; i++)
        printf("%d", nb.Tdigits[i]);
    printf("\n");
    printf("nombre de bits : %d\n", nb.Taille);
    printf("signe : %d\n", nb.Signe);
}

/* --- RSA Helpers --- */  

static unsigned long long mul_mod_ull(unsigned long long a, unsigned long long b, unsigned long long mod) {
    __uint128_t prod = (__uint128_t)a * b;
    return (unsigned long long)(prod % mod);
}

unsigned long long mod_pow_ull(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    if (mod == 0ULL) return 0ULL;
    unsigned long long result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod_ull(result, base, mod);
        base = mul_mod_ull(base, base, mod);
        exp >>= 1;
    }
    return result;
}


/* --- Menu Interactif --- */

void afficherMenu() {
    printf("\n");
    printf("======== RSA Simplifié - Menu Principal ========\n");
    printf("1. Addition BigBinary\n");
    printf("2. Soustraction BigBinary\n");
    printf("3. Multiplication\n");
    printf("4. Modulo BigBinary\n");
    printf("5. Exponentiation Modulo (M^e mod N)\n");
    printf("6. RSA Chiffrement (C = M^e mod N)\n");
    printf("7. RSA Déchiffrement (M = C^d mod N)\n");
    printf("8. Tests Automatiques\n");
    printf("0. Quitter\n");
    printf("===============================================\n");
    printf("Choisissez une option : ");
}

unsigned long long saisirNombreULL(const char *prompt) {
    char buffer[100];
    printf("%s", prompt);
    if (scanf("%99s", buffer) != 1) {
        printf("Erreur de saisie.\n");
        return 0;
    }
    return strtoull(buffer, NULL, 10);
}

int saisirNombreInt(const char *prompt) {
    char buffer[100];
    printf("%s", prompt);
    if (scanf("%99s", buffer) != 1) {
        printf("Erreur de saisie.\n");
        return 0;
    }
    return (int)strtol(buffer, NULL, 10);
}

void operationAddition() {
    printf("\n--- Addition BigBinary ---\n");
    unsigned long long a = saisirNombreULL("Entrez le premier nombre : ");
    unsigned long long b = saisirNombreULL("Entrez le second nombre : ");
    
    BigBinary A = creerBigBinaryDepuisEntier((int)a);
    BigBinary B = creerBigBinaryDepuisEntier((int)b);
    BigBinary S = Addition(A, B);
    
    printf("Résultat : ");
    afficheBigBinary(S);
    
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&S);
}

void operationSoustraction() {
    printf("\n--- Soustraction BigBinary ---\n");
    unsigned long long a = saisirNombreULL("Entrez le premier nombre : ");
    unsigned long long b = saisirNombreULL("Entrez le second nombre : ");
    
    BigBinary A = creerBigBinaryDepuisEntier((int)a);
    BigBinary B = creerBigBinaryDepuisEntier((int)b);
    BigBinary D = Soustraction(A, B);
    
    printf("Résultat : ");
    afficheBigBinary(D);
    
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&D);
}

void operationMultiplication() {
    printf("\n--- Multiplication ---\n");
    unsigned long long a = saisirNombreULL("Entrez le premier nombre : ");
    unsigned long long b = saisirNombreULL("Entrez le second nombre : ");
    unsigned long long prod = a * b;
    
    printf("Résultat : %llu\n", prod);
}

void operationModulo() {
    printf("\n--- Modulo BigBinary ---\n");
    unsigned long long a = saisirNombreULL("Entrez le nombre (dividende) : ");
    unsigned long long b = saisirNombreULL("Entrez le diviseur : ");
    
    if (b == 0) {
        printf("Erreur : diviseur ne peut pas être 0.\n");
        return;
    }
    
    BigBinary A = creerBigBinaryDepuisEntier((int)a);
    BigBinary B = creerBigBinaryDepuisEntier((int)b);
    BigBinary M = Modulo(A, B);
    
    printf("Résultat : ");
    afficheBigBinary(M);
    
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&M);
}

void operationExponentiationModulo() {
    printf("\n--- Exponentiation Modulo (M^e mod N) ---\n");
    unsigned long long m = saisirNombreULL("Entrez M (base) : ");
    unsigned long long e = saisirNombreULL("Entrez e (exposant) : ");
    unsigned long long n = saisirNombreULL("Entrez N (modulo) : ");
    
    if (n == 0) {
        printf("Erreur : modulo ne peut pas être 0.\n");
        return;
    }
    
    unsigned long long res = mod_pow_ull(m, e, n);
    printf("Résultat : %llu\n", res);
}

void operationRSAChiffrement() {
    printf("\n--- RSA Chiffrement (C = M^e mod N) ---\n");
    unsigned long long m = saisirNombreULL("Entrez le message M : ");
    unsigned long long e = saisirNombreULL("Entrez l'exposant public e : ");
    unsigned long long n = saisirNombreULL("Entrez le modulo public N : ");
    
    if (n == 0) {
        printf("Erreur : N ne peut pas être 0.\n");
        return;
    }
    
    unsigned long long c = mod_pow_ull(m, e, n);
    printf("Message original : %llu\n", m);
    printf("Ciphertext : %llu\n", c);
}

void operationRSADechiffrement() {
    printf("\n--- RSA Déchiffrement (M = C^d mod N) ---\n");
    unsigned long long c = saisirNombreULL("Entrez le ciphertext C : ");
    unsigned long long d = saisirNombreULL("Entrez l'exposant privé d : ");
    unsigned long long n = saisirNombreULL("Entrez le modulo N : ");
    
    if (n == 0) {
        printf("Erreur : N ne peut pas être 0.\n");
        return;
    }
    
    unsigned long long m = mod_pow_ull(c, d, n);
    printf("Ciphertext : %llu\n", c);
    printf("Message décrypté : %llu\n", m);
}

void testsAutomatiques() {
    printf("\n--- Tests Automatiques ---\n");
    
    printf("\nTest 1 : Addition (5 + 3)\n");
    BigBinary a1 = creerBigBinaryDepuisEntier(5);
    BigBinary b1 = creerBigBinaryDepuisEntier(3);
    BigBinary s1 = Addition(a1, b1);
    printf("Résultat : ");
    afficheBigBinary(s1);
    libereBigBinary(&a1);
    libereBigBinary(&b1);
    libereBigBinary(&s1);
    
    printf("\nTest 2 : Soustraction (10 - 4)\n");
    BigBinary a2 = creerBigBinaryDepuisEntier(10);
    BigBinary b2 = creerBigBinaryDepuisEntier(4);
    BigBinary d2 = Soustraction(a2, b2);
    printf("Résultat : ");
    afficheBigBinary(d2);
    libereBigBinary(&a2);
    libereBigBinary(&b2);
    libereBigBinary(&d2);
    
    printf("\nTest 3 : Modulo (17 mod 5)\n");
    BigBinary a3 = creerBigBinaryDepuisEntier(17);
    BigBinary b3 = creerBigBinaryDepuisEntier(5);
    BigBinary m3 = Modulo(a3, b3);
    printf("Résultat : ");
    afficheBigBinary(m3);
    libereBigBinary(&a3);
    libereBigBinary(&b3);
    libereBigBinary(&m3);
    
    printf("\nTest 4 : RSA simplifié (p=5, q=11, n=55, e=3, d=27)\n");
    unsigned long long msg = 17;
    unsigned long long cipher = mod_pow_ull(msg, 3, 55);
    unsigned long long decrypted = mod_pow_ull(cipher, 27, 55);
    printf("Message original : %llu\n", msg);
    printf("Chiffré : %llu\n", cipher);
    printf("Déchiffré : %llu\n", decrypted);
}

/* --- Main --- */
int main() {
    int choix = -1;
    
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  Bienvenue dans RSA Simplifié v1.0    ║\n");
    printf("║        Arithmétique BigBinary         ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    while (choix != 0) {
        afficherMenu();
        choix = saisirNombreInt("");
        
        switch (choix) {
            case 1:
                operationAddition();
                break;
            case 2:
                operationSoustraction();
                break;
            case 3:
                operationMultiplication();
                break;
            case 4:
                operationModulo();
                break;
            case 5:
                operationExponentiationModulo();
                break;
            case 6:
                operationRSAChiffrement();
                break;
            case 7:
                operationRSADechiffrement();
                break;
            case 8:
                testsAutomatiques();
                break;
            case 0:
                printf("\nAu revoir ! 👋\n");
                break;
            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    }
    
    return 0;
}