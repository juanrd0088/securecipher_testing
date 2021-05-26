#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 256   // 2^8

void swap(unsigned char* a, unsigned char* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//KSA: Key scheduling algorithm
int ksa(char* key, unsigned char* S) {
    int i = 0;
    int keylen = strlen(key);
    //Primero se rellena el array de 256 bytes
    for (i = 0; i < N; i++) {
        S[i] = i;
    }
    //A continuacion se intercambian sus valores mezclandolos con la clave
    int j = 0;
    for (i = 0; i < N; i++) {
        //La key funciona como semilla que marca los intercambios en S
        j = (j + S[i] + key[i % keylen]) % N; 
        swap(&S[i], &S[j]);
    }
    return 0;
}

//PRGA: Pseudo Random generation algorithm
int prga(unsigned char* S, char* orig, unsigned char* ciphered ) {
    /*int i = 0, j = 0, r = 0;
    i = (i + 1) % N;
    j = (j + S[i]) % N;
    swap(&S[i], &S[j]);
    r = (S[i] + S[j]) % 256;
    return r;*/

    //From robin verton: https://gist.github.com/rverton/a44fc8ca67ab9ec32089
    int i = 0, j = 0, n = 0, rnd = 0;

    for (n = 0; n < strlen(orig); n++) {
        i = (i + 1) % N;
        j = (j + S[i]) % N;

        swap(&S[i], &S[j]);
        rnd = S[(S[i] + S[j]) % N];

        //El elemento cifrado, es el elemento elegido de S xor Texto plano
        ciphered[n] = rnd ^ orig[n]; 

    }
    return 0;
}

//RC4
int rc4(char* key, char* orig, unsigned char* ciphered) {
    int i = 0;
    unsigned char S[N];
    
    ksa(key, S);
    prga(S, orig, ciphered);
    
    return 0;
}

int testMode() {
    char* test_vectors[3][2] =
    {
        {"Key", "Plaintext"},
        {"Wiki", "pedia"},
        {"Secret", "Attack at dawn"}
    };

    for (int i = 0; i < 3; i++) {
        printf("Texto plano: %s, Clave: %s, Texto cifrado: ", test_vectors[i][1], test_vectors[i][0]);
        unsigned char* ciphered = malloc(sizeof(int) * strlen(test_vectors[i][1]));
        rc4(test_vectors[i][0], test_vectors[i][1], ciphered);
        for (int i = 0; i < strlen(test_vectors[0][1]); i++) {
            printf("%02X", ciphered[i]);
        }
        printf("\n");

    }

    /*unsigned char* ciphered = malloc(sizeof(int) * strlen(test_vectors[0][1]));

    rc4(test_vectors[0][0], test_vectors[0][1], ciphered);

    for (int i = 0; i < strlen(test_vectors[0][1]); i++) {
        printf("%02X", ciphered[i]);
    }
    printf("\n");*/

    return 0;
}

int main() {
    testMode();
}