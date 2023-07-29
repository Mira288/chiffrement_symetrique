#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pgcd(int a, int b)
{
    int r = a % b;

    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }

    return b;
}

int inverse(int det)
{
    int a = det;
    int b = 26;
    int q, r, x1, x2, y1, y2, inv;

    x2 = 1;
    x1 = 0;
    y2 = 0;
    y1 = 1;

    while (b > 0)
    {
        q = a / b;
        r = a - q * b;

        inv = x2 - q * x1;
        x2 = x1;
        x1 = inv;

        inv = y2 - q * y1;
        y2 = y1;
        y1 = inv;

        a = b;
        b = r;
    }

    if (a == 1)
    {
        inv = x2;
        if (inv < 0)
        {
            inv += 26;
        }
        return inv;
    }
    else
    {
        printf("Le determinant n'a pas d'inverse modulo 26.\n");
        return 0;
    }
}

int letterToNumber(char letter)
{
    char tab[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    for (int i = 0; i < 26; i++)
    {
        if (letter == tab[i])
        {
            return i;
            break;
        }
    }
}

char numberToLetter(int number)
{
    char tab[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    return tab[number];
}

void encrypt(char *message, int key_matrix[2][2])
{
    int len = strlen(message);
    int i;

    if (len % 2 != 0)
    {
        message[len] = 'A';
        message[len + 1] = '\0';
        len++;
    }

    for (i = 0; i < len; i += 2)
    {
        int pair[2][1];
        pair[0][0] = letterToNumber(message[i]);
        pair[1][0] = letterToNumber(message[i + 1]);
        int result[2][1];
        result[0][0] = (key_matrix[0][0] * pair[0][0] + key_matrix[0][1] * pair[1][0]) % 26;
        result[1][0] = (key_matrix[1][0] * pair[0][0] + key_matrix[1][1] * pair[1][0]) % 26;
        message[i] = numberToLetter(result[0][0]);
        message[i + 1] = numberToLetter(result[1][0]);
    }
}

void decrypt(char *textchiffrer, int key_matrix[2][2])
{
    int len = strlen(textchiffrer);
    int i, var0, var1, var2, var3;

    for (i = 0; i < len; i += 2)
    {
        int pair[2][1];
        pair[0][0] = letterToNumber(textchiffrer[i]);
        pair[1][0] = letterToNumber(textchiffrer[i + 1]);

        int det = ((key_matrix[0][0] * key_matrix[1][1]) % 26 - (key_matrix[0][1] * key_matrix[1][0]) % 26 + 26) % 26;
        int inv_det = inverse(det);

        var0=inv_det * key_matrix[1][1];
        var1=(-inv_det * key_matrix[0][1]);
        var2=(-inv_det * key_matrix[1][0]);
        var3=inv_det * key_matrix[0][0];

        while (var0<0)
        {
            var0 +=26;
        }
        while (var1<0)
        {
            var1 +=26;
        }
        while (var2<0)
        {
            var2 +=26;
        }
        while (var3<0)
        {
            var2 +=26;
        }

        while (var0 > 26)
        {
            var0 -=26;
        }
         while (var1 > 26)
        {
            var1 -=26;
        }
         while (var2 > 26)
        {
            var2 -=26;
        }
         while (var3 > 26)
        {
            var3 -=26;
        }
        

        int result[2][1];

        result[0][0] = (var0 * pair[0][0] + var1 * pair[1][0]) % 26;
        result[1][0] = (var2 * pair[0][0] + var3 * pair[1][0]) % 26;

        textchiffrer[i] = numberToLetter(result[0][0]);
        textchiffrer[i + 1] = numberToLetter(result[1][0]);
    }

    if (textchiffrer[len - 1] == 'A')
    {
        textchiffrer[len - 1] = '\0';
    }

}

int main()
{

    int i = 0, det, c, b = 26;
    int tab[4];
    char message[1000];
    char chaine[1000];

    for (i = 0; i < 4; i++)
    {
        printf("Entrez l'element %d de la matrice \n\t>>:", i);
        scanf("%d", &tab[i]);
    }

    det = ((tab[0] * tab[3]) - (tab[1] * tab[2]));

    while (det < 0)
    {
        det = det + 26;
    }

    printf("det = %d \n ", det);

    while (det == 0)
    {
        printf("\nVotre matrice n'est pas inversible \n");
        break;
    }

    while (det > 26)
    {
        det = det - 26;
    }

    c = pgcd(b, det);

    if (c != 1)
    {
        printf("Vous ne pouvez plus continuer \n");
        return 0;
    }
    else
    {
        printf("Continuons\n");
    }

    printf("\nEntrez votre texte à chiffrer ");
    scanf("%s", chaine);

    int matrix[2][2] = {{tab[0], tab[1]}, {tab[2], tab[3]}};
    encrypt(chaine, matrix);

    printf("Texte chiffre: %s \n", chaine);

    printf("\nEntrez votre texte à déchiffrer ");
    scanf("%s", message);
    decrypt(message, matrix);
    printf("Texte dechiffre: %s \n", message);

    return 0;
}