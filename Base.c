/*#include <stdio.h>

#include <stdbool.h>

#define BASE 2
 
void bigBinary(int n) {

    if (n == 0) {

        printf("0");

        return;

    }

    int bits[32];

    int i = 0;

    while (n > 0) {

        bits[i++] = n % 2;

        n /= 2;

    }

    for (int j = i - 1; j >= 0; j--) {

        printf("%d", bits[j]);

    }

}

int main() {

    int a, b;

    printf("Entrer deux entiers :");

    scanf("%d %d", &a, &b);
 
    printf("%d en binaire : ", a);

    bigBinary(a);

    printf("\n");
 
    printf("%d en binaire : ", b);

    bigBinary(b);

    printf("\n");

return 0;
 
}

*/

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <string.h>
 
#define BASE 2
 
typedef struct {

    int *Tdigits;

    int Taille;

    int Signe;

} BigBinary;
 
// Conversion d'un entier en BigBinary

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
 
void libereBigBinary(BigBinary *nb) {

    free(nb->Tdigits);

    nb->Tdigits = NULL;

    nb->Taille = 0;

    nb->Signe = 0;

}
 
BigBinary createBigBinary(int size) {

    BigBinary bb;

    bb.Tdigits = calloc(size, sizeof(int));

    bb.Taille = size;

    bb.Signe = 0;

    return bb;

}
 
void afficheBigBinary(BigBinary nb) {

    // Affiche le nombre converti

    for (int i = 0; i < nb.Taille; i++)

        printf("%d", nb.Tdigits[i]);

    printf("\n");

    // Affiche le nombre de bits

    printf("nombre de bits : %d\n", nb.Taille);

    // Affiche le signe

    printf("signe : %d\n", nb.Signe);
 
}
 
// Addition

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
 
// soustraction

BigBinary Soustraction(BigBinary A, BigBinary B) {

    bool negatif = false;

    if (A.Taille < B.Taille || (A.Taille == B.Taille && memcmp(A.Tdigits, B.Tdigits, A.Taille * sizeof(int)) < 0)) {

        BigBinary tmp = A;

        A = B;

        B = tmp;

        negatif = true;

    }

    BigBinary R = createBigBinary(A.Taille);

    int borrow = 0;

    for (int i = 0; i < A.Taille; i++) {

        int a = A.Tdigits[A.Taille - 1 - i];

        int b = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;

        int diff = a - b - borrow;

        if (diff < 0) {

            diff += 2;

            borrow = 1;

        } else borrow = 0;

        R.Tdigits[A.Taille - 1 - i] = diff;

    }

    R.Signe = negatif ? -1 : +1;

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

int main() {

    int a, b;

    printf("Entrez le premier nombre decimal : ");

    while (scanf("%d", &a) != 1) {

        printf("Erreur : veuillez entrer un nombre decimal valide.\n");

        // Vider le buffer d'entrée

        while (getchar() != '\n');

        printf("Entrez le premier nombre decimal : ");

    }

    printf("Entrez le second nombre decimal : ");

    while (scanf("%d", &b) != 1) {

        printf("Erreur : veuillez entrer un nombre decimal valide.\n");

        while (getchar() != '\n');

        printf("Entrez le second nombre décimal : ");

    }
 
    BigBinary A = creerBigBinaryDepuisEntier(a);

    BigBinary B = creerBigBinaryDepuisEntier(b);
 
    printf("A = "); afficheBigBinary(A);

    printf("B = "); afficheBigBinary(B);
 
    BigBinary S = Addition(A, B);

    printf("A + B = "); afficheBigBinary(S);
 
    BigBinary D = Soustraction(A, B);

    printf("A - B = "); afficheBigBinary(D);
 
    int cmp = comparerBigBinary(A, B);

    if (cmp < 0)

        printf("A < B\n");

    else if (cmp > 0)

        printf("A > B\n");

    else

        printf("A == B\n");
 
    libereBigBinary(&A);

    libereBigBinary(&B);

    libereBigBinary(&S);

    libereBigBinary(&D);

    return 0;

}
 
