
#include <stdio.h>
#include <stdlib.h>

#define MAX 400
#define MAX2 160000
#define FNMAX 200


int DecimalToBinaryInt(int decimal)
{
    /* get a decimal int value as a binary int value */

    if (decimal == 0)
    {
        return 0;
    }

    if (decimal == 1) 
    {
        return 1;                     
    }

    return (decimal % 2) + 10 * DecimalToBinaryInt(decimal / 2);
}

void DecimalToBinaryArray(int decimal, int asciibinaryarray[7]) 
{
    /* turn a "decimal int" into a "binary int", slice the "binary int" into a "8bitish binary array", returning it */
    /* example: decimal int 103 converted to a binary int 1100111 and then converted into a 8bitish binary array [0, 1, 1, 0, 0, 1, 1, 1] */

    int binaryint;
    int i;

    binaryint = DecimalToBinaryInt(decimal);

    for (i = 0; i < 8; i++)
    {
        asciibinaryarray[i] = 0;
    }

    for (i = 7; i >= 0; i--)
    {
        asciibinaryarray[i] = binaryint % 10;
        binaryint /= 10;
    }
}

int Pow(int base, int exponent)
{
    /* get the power of a number */

    int power=1;

    while(exponent>0)
    {
    power = power * base;
    exponent = exponent - 1;
    }

    return power;
}


int LeDesenho(char nomearq[FNMAX], int M[MAX][MAX], int *pm, int *pn, int *pmax)
{
    FILE* fp;
    char firstchar[MAX];
    int i;
    int j;
    int holder;


    scanf(" %[^\n]", nomearq);
    

    if (!(fp = fopen(nomearq, "r"))) 
    {
    printf("Erro ao abrir o arquivo %s\n", nomearq);
    return 1;
    }

    fscanf(fp, "%s", firstchar);
    fscanf(fp, "%d", pn);
    fscanf(fp, "%d", pm);
    fscanf(fp, "%d", pmax);

    printf("m: %d, n: %d\n", *pm, *pn);

    for (i = 0; i < *pm; i++)
    {
        for (j = 0; j < *pn; j++)
        {
            fscanf(fp, "%d", &holder);   
            M[i][j] = holder;
        }
    }

    fclose(fp);

    if (MAX < *pm || MAX < *pn)
    {
        return 1;
    } 

    return 0;
}

int LeTexto(char nomearq[FNMAX], char T[MAX2], int *pk)
{
    FILE* fp;
    char characterholder;

    *pk = 0;

    scanf(" %[^\n]", nomearq);

    if (!(fp = fopen(nomearq, "r"))) 
    {
    printf("Erro ao abrir o arquivo %s\n", nomearq);
    return 1;
    }

    while((characterholder = fgetc(fp)) != EOF)
    {
        T[*pk] = characterholder;
        *pk = *pk + 1;
    }

    fclose(fp);

    if (MAX2 < *pk)
    {
        return 1;
    }

    return 0;
}

int BeDe(int k, int m, int n, int *pb, int *pd)
{
    int flag;

    if ((m*n - 1) >= (8*k))
    {
        *pb = 1;
    }
    else if ((m*n -1)*2 >= (8*k))
    {
        *pb = 2;
    }
    else if ((m*n -1)*4 >= (8*k))
    {
        *pb = 4;
    }
    else if ((m*n -1)*8 >= (8*k))
    {
        *pb = 8;
    }
    else
    {
        return 1;
    }

    if (m>n)
    {
        *pd = n;
    }
    else
    {
        *pd = m;
    }

    flag = 0;
    while (flag == 0)
    {
        if (((m/(*pd))*(n/(*pd)*(*pb))) >= (8*k+(*pb)))
        {
        break;
        }
        *pd = *pd - 1;
        if ((*pd) <= 1)
        {
            return 1;
        }
        
    }

    return 0;
}

int ProximosBBits(char T[MAX2], int b, int *pik, int *pib)
{
    int asciidecimal;
    int asciibinaryarray[8];
    int i;
    int proximos;

    asciidecimal = (int)T[*pik];
    proximos = 0;

    DecimalToBinaryArray(asciidecimal, asciibinaryarray);

    for (i = 1; i <= b; i++)
    {
        proximos += asciibinaryarray[8-(*pib)-i]*(Pow(2, i-1));
    }

    *pib += b;

    if (*pib >= 8)
    {
        *pik += 1;
        *pib -= 8;
    }


    return proximos;
}

void Codifica(int D[MAX][MAX], int m, int n, char T[MAX2], int k, int Dl[MAX][MAX], int b, int d, int modo)
{
    int i;
    int j;
    int B;
    int flag;
    int bits;
    int ik;
    int ib;
    int kcounter;
    int secondflag;

    int *pik;
    int *pib;
    
    pik = &ik;
    pib = &ib;

    *pik = 0;
    *pib = 0;

    B = Pow(2, b);

    for (i = 0; i <= m; i++)
    {
        for (j = 0; j <= n; j++)
        {
            Dl[i][j] = D[i][j];
        }
    } 
    
    Dl[d-1][d-1] = (((D[(d)-1][(d)-1] / B) * B) + ((D[(d)-1][(d)-1] + b + 256) % B));
    
    if(modo == 1)
    {
    printf("(%d, %d) bits %d original %02x codificado %02x\n", d-1, d-1, b, D[d-1][d-1], Dl[d-1][d-1]);
    }

    kcounter = 0;
    secondflag = 0;
    flag = 0;



    for (i = 1; i <= m/d; i++)
    {
        for (j = 1; j <= n/d; j++)
        {
            if(flag == 1)
            {
                bits = ProximosBBits(T, b, pik, pib);   
                Dl[(i*d)-1][(j*d)-1] = (((D[(i*d)-1][(j*d)-1] / B) * B) + ((D[(i*d)-1][(j*d)-1] + bits + 256) % B));
             if(modo == 1)
                {    
                    if (secondflag == 0)
                    {
                        printf("(%d, %d) bits %d original %d codificado %d\n", ((i*d)-1), ((j*d)-1), bits, D[(i*d)-1][(j*d)-1], Dl[(i*d)-1][(j*d)-1]);
                        kcounter += 1;
                        if(kcounter == k*(8/b))
                        {
                            secondflag = 1;
                        }
                    }
                }   
            }
            flag = 1;
        }

    }
}

int Maximo(int D[MAX][MAX], int m, int n)
{
    int i;
    int j;
    int matrixmaxval;

    matrixmaxval = 0;

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (D[i][j] > matrixmaxval)
            {
                matrixmaxval = D[i][j];
            }
        }
    }

    return matrixmaxval;
}

int EscreveDesenho(char nomearq[FNMAX], int M[MAX][MAX], int m, int n, int max)
{
    FILE* fp;
    int i;
    int j;

    scanf(" %[^\n]", nomearq);

    if (!(fp = fopen(nomearq, "w"))) 
    {
    printf("Erro ao escrever no arquivo %s\n", nomearq);
    return 1;
    }

    fprintf(fp, "P2\n");
    fprintf(fp, "%d ", n);
    fprintf(fp, "%d\n", m);
    fprintf(fp, "%d\n", max);

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            fprintf(fp, "%03d ", M[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    
    return 0;
}

int DeBeDe(int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int *pb, int *pd)
{
    int i;
    int flag;

    i = 0;
    flag = 0;

    while(flag == 0)
    {
        if((Dl[i][i] - D[i][i]) != 0)
        {
            *pd = i+1;
            flag = 1;
            break;
        }
        i = i + 1;
    }

    if(1 == ((Dl[i][i]-D[i][i]+256)%Pow(2, 1)))
    {
        *pb = 1;
    }
    else if(2 == ((Dl[i][i]-D[i][i]+256)%Pow(2, 2)))
    {
        *pb = 2;
    }
    else if(4 == ((Dl[i][i]-D[i][i]+256)%Pow(2, 4)))
    {
        *pb = 4;
    }
    else if(8 == ((Dl[i][i]-D[i][i]+256)%Pow(2, 8)))
    {
        *pb = 8;
    }
    else
    {
        return 1;
    }
    
    return 0;
}

int DeCodifica(int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int b, int d, char T[MAX2], int modo)
{
    int ik;
    int holder;
    int B;
    int im;
    int in;
    int mybits;
    int k;
    int binaryarray[8];
    int flag;
    int secondflag;
    int kcounter;

    int *pb;
    int *pd;

    pb = &b;
    pd = &d;

    k = 0;
    ik = 0;
    holder = 0;

    DeBeDe(D, Dl, m, n, pb, pd);

    printf("b = %d d = %d", b, d);

    B = Pow(2, b);

    for (im = 1; im <= m/d; im++)
    {
        for (in = 1; in <= n/d; in++)
        {       
                mybits = ((Dl[(im*d)-1][(in*d)-1])-(D[(im*d)-1][(in*d)-1]) + 256)%B;
                T[ik] = mybits;
                ik += 1;
        }
  
    }

    while(b == 1)
    {
        holder = 0;
        
        DecimalToBinaryArray(T[1+(8*k)], binaryarray);
        holder += binaryarray[7]*1;    

        DecimalToBinaryArray(T[2+(8*k)], binaryarray);
        holder += binaryarray[7]*2;

        DecimalToBinaryArray(T[3+(8*k)], binaryarray);
        holder += binaryarray[7]*4;

        DecimalToBinaryArray(T[4+(8*k)], binaryarray);
        holder += binaryarray[7]*8;

        DecimalToBinaryArray(T[5+(8*k)], binaryarray);
        holder += binaryarray[7]*16;    

        DecimalToBinaryArray(T[6+(8*k)], binaryarray);
        holder += binaryarray[7]*32;

        DecimalToBinaryArray(T[7+(8*k)], binaryarray);
        holder += binaryarray[7]*64;

        DecimalToBinaryArray(T[8+(8*k)], binaryarray);
        holder += binaryarray[7]*128;

        if(holder == 0)
        {
            break;
        }

        T[k] = holder;
        k += 1;

    }

    while(b == 2)
    {
        holder = 0;
        
        DecimalToBinaryArray(T[1+(4*k)], binaryarray);
        holder += binaryarray[6]*2 + binaryarray[7];    

        DecimalToBinaryArray(T[2+(4*k)], binaryarray);
        holder += binaryarray[6]*8 + binaryarray[7]*4;

        DecimalToBinaryArray(T[3+(4*k)], binaryarray);
        holder += binaryarray[6]*32 + binaryarray[7]*16;

        DecimalToBinaryArray(T[4+(4*k)], binaryarray);
        holder += binaryarray[6]*128 + binaryarray[7]*64;

        if(holder == 0)
        {
            break;
        }

        T[k] = holder;
        k += 1;

    }

    while(b == 4)
    {
        holder = 0;
        
        DecimalToBinaryArray(T[1+(2*k)], binaryarray);
        holder += binaryarray[4]*8 + binaryarray[5]*4 + binaryarray[6]*2 + binaryarray[7];    

        DecimalToBinaryArray(T[2+(2*k)], binaryarray);
        holder += binaryarray[4]*128 + binaryarray[5]*64 + binaryarray[6]*32 + binaryarray[7]*16;

        if(holder == 0)
        {
            break;
        }
        T[k] = holder;
        k += 1;

    }

    while(b == 8)
    {
        holder = 0;
        
        holder += T[1+k];

        if(holder == 0)
        {
            break;
        }

        T[k] = holder;
        k += 1;

    }

    printf("\n");
    flag = 0;

    flag = 0;
    secondflag = 0;
    kcounter = 0;

    if(modo == 1)
    {
        for (im = 1; im <= m/d; im++)
        {
            for (in = 1; in <= n/d; in++)
            {       
                if(flag == 1)
                {
                    if (secondflag == 0)
                    {
                        mybits = ((Dl[(im*d)-1][(in*d)-1])-(D[(im*d)-1][(in*d)-1]) + 256)%B;
                        printf("(%d, %d) bits %d original %02x codificado %02x\n", ((im*d)-1), ((in*d)-1), mybits, D[(im*d)-1][(in*d)-1], Dl[(im*d)-1][(in*d)-1]);
                        kcounter += 1;
                        if(kcounter == k*(8/b))
                        {
                            secondflag = 1;
                        }
                    }
                }
                flag = 1;
            }
    
        }
    }

    printf("k = %d\n", k);
    
    return k;
}

int EscreveTexto(char nomearq[FNMAX], char T[MAX2], int k)
{
    FILE* fp;
    int i;

    scanf(" %[^\n]", nomearq);

    if (!(fp = fopen(nomearq, "w"))) 
    {
    printf("Erro ao abrir o arquivo %s\n", nomearq);
    return 1;
    }

    for (i = 0; i < k; i++)
    {
    fprintf(fp, "%c", T[i]);
    }

    fclose(fp);

    return 0; 
}


int main()
{
    char nomearq[FNMAX];
    char T[MAX2];
    int D[MAX][MAX];
    int Dl[MAX][MAX];

    int m;
    int n;
    int max;
    int k;
    int b;
    int d;
    int modo;
    int flag;
    int scanned;
    int originalmax;
    int encodedmax;
    int i;
    int secondflag;

    int *pm;
    int *pn;
    int *pmax;
    int *pk;
    int *pb;
    int *pd;

    pm = &m;
    pn = &n;
    pmax = &max;
    pk = &k;
    pb = &b;
    pd = &d;
    
    secondflag = 0;
    flag = 0;
    modo = 0;

    while (secondflag == 0)
    {
        while(flag == 0)
        {
            printf("\nDigite a operacao desejada:\n");
            printf("    1. Codificar\n");
            printf("    2. Decodificar\n");
            printf("    3. Mostrar mensagem\n");
            if(modo == 0)
            {
                printf("    4. Ativar modo verborragico\n");
            }
            if(modo == 1)
            {
                printf("    4. Desativar modo verborragico\n");
            }
            printf("    0. Finalizar\n");

            printf("\nOPCAO: ");
            scanf("%d", &scanned);

            if(scanned == 4)
            {
                if(modo == 1)
                {
                    printf("Modo verborragico desativado\n");
                    modo = 0;
                }
                else
                {
                    printf("Modo verborragico ativado\n");
                    modo = 1;
                }

            }

            if(scanned == 0)
            {
                secondflag = 1;
                flag = 1;
                break;
            }

            if(scanned == 1)
            {
                printf("Arquivo que contem o desenho orginal: ");

                if(LeDesenho(nomearq, D, pm, pn, pmax) == 1)
                {
                    break;
                } 
    

                printf("Arquivo que contem a mensagem a ser ocultada: ");
                
                if(LeTexto(nomearq, T, pk) == 1)
                {
                    break;
                }
                
                
                printf("k = %d\n", k);

                BeDe(k, m, n, pb, pd);
                printf("b = %d, d = %d\n", b, d);

                Codifica(D, m, n, T, k, Dl, b, d, modo);

                printf("Gravar desenho esteganografado em: ");

                originalmax = Maximo(D, m, n);
                encodedmax = Maximo(Dl, m, n);
                if(originalmax > encodedmax)
                {
                    max = originalmax;
                }
                else
                {
                    max = encodedmax;
                }


                if(EscreveDesenho(nomearq, Dl, m, n, max) == 1)
                {
                    break;
                }

            }

            if(scanned == 2)
            {
                printf("Arquivo que contem o desenho orginal: ");
                
                if(LeDesenho(nomearq, D, pm, pn, pmax) == 1)
                {
                    break;
                }

                printf("Arquivo que contem o desenho esteganografado: ");
                
                if(LeDesenho(nomearq, Dl, pm, pn, pmax) == 1)
                {
                    break;
                }

                k = DeCodifica(D, Dl, m, n, b, d, T, modo);

                printf("Gravar mensagem decodificada em: ");
                if(EscreveTexto(nomearq, T, k) == 1)
                {
                    break;
                }
            }

            if(scanned == 3)
            {
                printf("Arquivo que contem a mensagem: ");
                
                if(LeTexto(nomearq, T, pk) ==  1)
                {
                    break;
                }

                printf("k = %d\n", k);
                printf("MENSAGEM contida no arquivo msg1.txt (tudo entre ---> e <---)\n");
                printf("--->");

                for (i = 0; i < k; i++)
                {
                printf("%c", T[i]);
                }

                printf("<---");

            }


        }
    }
    return 0;
}   
