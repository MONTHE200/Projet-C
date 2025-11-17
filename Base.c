#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BASE 2

typedef struct {
    int *Tdigits;  // Tdigits[0] = MSB, Tdigits[Taille-1] = LSB
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 positif, -1 négatif, 0 nul
} BigBinary;

// ==================== FONCTIONS DE BASE ====================

// Crée un BigBinary vide (initialisé à 0)
BigBinary createBigBinary(int size) {
    BigBinary bb;
    bb.Tdigits = (int*)malloc(sizeof(int) * size);
    bb.Taille = size;
    bb.Signe = 0;
    for (int i = 0; i < size; i++) bb.Tdigits[i] = 0;
    return bb;
}

// Initialise manuellement un BigBinary avec taille et signe (tout à zéro)
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = (int*) malloc(sizeof(int) * taille);
    for (int i = 0; i < taille; ++i) nb.Tdigits[i] = 0;
    return nb;
}

// Crée un BigBinary à partir d'une chaîne binaire (ex: "1010011")
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Compter les bits valides
    for (int i = 0; i < n; i++) {
        if (chaine[i] == '0' || chaine[i] == '1') nb.Taille++;
    }

    if (nb.Taille == 0) { // chaîne vide ou sans bits -> zéro
        return initBigBinary(1, 0);
    }

    nb.Tdigits = (int*)malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;

    int index = 0;
    int tousZeros = 1;
    for (int i = 0; i < n; i++) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Tdigits[index] = chaine[i] - '0';
            if (nb.Tdigits[index] == 1) tousZeros = 0;
            index++;
        }
    }

    if (tousZeros) nb.Signe = 0;
    return nb;
}

// Affiche un BigBinary
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; i++) printf("%d", nb.Tdigits[i]);
    printf("\n");
}

// Libère la mémoire
void libereBigBinary(BigBinary *nb) {
    if (nb->Tdigits != NULL) {
        free(nb->Tdigits);
        nb->Tdigits = NULL;
    }
    nb->Taille = 0;
    nb->Signe = 0;
}

// ==================== FONCTIONS DE COMPARAISON ====================

bool Egal(BigBinary A, BigBinary B) {
    if (A.Signe != B.Signe) return false;
    if (A.Taille != B.Taille) return false;
    for (int i = 0; i < A.Taille; i++) if (A.Tdigits[i] != B.Tdigits[i]) return false;
    return true;
}

bool Inferieur(BigBinary A, BigBinary B) {
    if (A.Signe < B.Signe) return true;
    if (A.Signe > B.Signe) return false;
    if (A.Signe == 0 && B.Signe == 0) return false;

    if (A.Taille < B.Taille) return (A.Signe == 1);
    if (A.Taille > B.Taille) return (A.Signe == -1);

    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] < B.Tdigits[i]) return (A.Signe == 1);
        if (A.Tdigits[i] > B.Tdigits[i]) return (A.Signe == -1);
    }
    return false;
}

// ==================== OPÉRATIONS ARITHMÉTIQUES ====================

// Addition de deux BigBinary positifs
BigBinary Addition(BigBinary A, BigBinary B) {
    int maxTaille = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary resultat = createBigBinary(maxTaille + 1); // +1 pour la retenue

    int retenue = 0;
    int idxA = A.Taille - 1;
    int idxB = B.Taille - 1;
    int idxRes = resultat.Taille - 1;

    while (idxRes >= 0) {
        int bitA = (idxA >= 0) ? A.Tdigits[idxA] : 0;
        int bitB = (idxB >= 0) ? B.Tdigits[idxB] : 0;
        int somme = bitA + bitB + retenue;
        resultat.Tdigits[idxRes] = somme % 2;
        retenue = somme / 2;
        idxA--; idxB--; idxRes--;
    }

    resultat.Signe = 1;

    // Supprimer les zéros en tête
    int debut = 0;
    while (debut < resultat.Taille - 1 && resultat.Tdigits[debut] == 0) debut++;

    if (debut > 0) {
        int nouvelleTaille = resultat.Taille - debut;
        int *nouveauxDigits = (int*)malloc(sizeof(int) * nouvelleTaille);
        for (int i = 0; i < nouvelleTaille; i++) nouveauxDigits[i] = resultat.Tdigits[debut + i];
        free(resultat.Tdigits);
        resultat.Tdigits = nouveauxDigits;
        resultat.Taille = nouvelleTaille;
    }

    return resultat;
}

// Soustraction (A - B) avec A >= B et A,B >= 0
BigBinary Soustraction(BigBinary A, BigBinary B) {
    BigBinary resultat = createBigBinary(A.Taille);
    resultat.Signe = 1;

    int emprunt = 0;
    int idxA = A.Taille - 1;
    int idxB = B.Taille - 1;
    int idxRes = resultat.Taille - 1;

    while (idxRes >= 0) {
        int bitA = (idxA >= 0) ? A.Tdigits[idxA] : 0;
        int bitB = (idxB >= 0) ? B.Tdigits[idxB] : 0;
        int diff = bitA - bitB - emprunt;
        if (diff < 0) { diff += 2; emprunt = 1; } else { emprunt = 0; }
        resultat.Tdigits[idxRes] = diff;
        idxA--; idxB--; idxRes--;
    }

    // Supprimer les zéros en tête
    int debut = 0;
    while (debut < resultat.Taille - 1 && resultat.Tdigits[debut] == 0) debut++;

    if (debut > 0) {
        int nouvelleTaille = resultat.Taille - debut;
        int *nouveauxDigits = (int*)malloc(sizeof(int) * nouvelleTaille);
        for (int i = 0; i < nouvelleTaille; i++) nouveauxDigits[i] = resultat.Tdigits[debut + i];
        free(resultat.Tdigits);
        resultat.Tdigits = nouveauxDigits;
        resultat.Taille = nouvelleTaille;
    }

    if (resultat.Taille == 1 && resultat.Tdigits[0] == 0) resultat.Signe = 0;
    return resultat;
}

// ==================== DIVISION PAR 2 ====================

// Divise nb par 2 (arithmétique pour valeurs non signées ici).
// Comportement : décale vers la droite d'un bit (perte du LSB).
// Met à jour Taille et Signe correctement (si résultat = 0).
void divisePar2(BigBinary *nb) {
    if (nb == NULL || nb->Taille == 0 || nb->Signe == 0) {
        // déjà nul ou invalide ; rien à faire
        nb->Signe = 0;
        if (nb->Taille == 0) {
            nb->Taille = 1;
            nb->Tdigits = (int*)malloc(sizeof(int));
            nb->Tdigits[0] = 0;
        }
        return;
    }

    // Si taille = 1, le résultat est soit 0 (si bit 0) soit 0 aussi après division
    if (nb->Taille == 1) {
        nb->Tdigits[0] = 0;
        nb->Signe = 0;
        return;
    }

    // Décaler tous les bits vers la droite : MSB <- next MSB, ... LSB supprimé
    for (int i = nb->Taille - 1; i >= 1; --i) {
        nb->Tdigits[i] = nb->Tdigits[i-1];
    }
    // Le MSB devient 0 (nouveau MSB)
    nb->Tdigits[0] = 0;

    // Supprimer les zéros en tête (MSB) pour ajuster la taille
    int debut = 0;
    while (debut < nb->Taille - 1 && nb->Tdigits[debut] == 0) debut++;

    if (debut > 0) {
        int nouvelleTaille = nb->Taille - debut;
        int *nouveauxDigits = (int*)malloc(sizeof(int) * nouvelleTaille);
        for (int i = 0; i < nouvelleTaille; i++) nouveauxDigits[i] = nb->Tdigits[debut + i];
        free(nb->Tdigits);
        nb->Tdigits = nouveauxDigits;
        nb->Taille = nouvelleTaille;
    }

    // Si tout est zéro après le décalage, ajuster le signe
    int tousZeros = 1;
    for (int i = 0; i < nb->Taille; i++) if (nb->Tdigits[i] == 1) { tousZeros = 0; break; }
    if (tousZeros) nb->Signe = 0;
}

// ==================== EXEMPLE D'UTILISATION (main) ====================

int main(void) {
  // Ajoutez les tests du modulo à la fin du main
    printf("\nTest Modulo:\n");
    BigBinary num1 = creerBigBinaryDepuisChaine("1101"); 
    BigBinary num2 = creerBigBinaryDepuisChaine("101");  
    BigBinary mod;                
    printf("1101 mod 101 = ");
    afficheBigBinary(mod);
    
    libereBigBinary(&num1);
    libereBigBinary(&num2);
    libereBigBinary(&mod);

    return 0;
}


