/*Oncioiu Anamaria Raluca
**Grupa: 323CA
**Tema1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

using namespace std;

#define SIZE_DICT 8000
#define SIZE 65
#define MAX_SIZE 1000

//functie care calculeaza distantele de editare intre prefixele a doua cuvinte
void levenshtein(char *sir, char *definition, int copy[SIZE][SIZE])
{
  int i, j, len1, len2;
  
  len1 = strlen(sir);
  len2 = strlen(definition);  
  
  int d[SIZE][SIZE];
	
  //distanta de la orice prefix din primul sir la al doilea sir
  for(i = 0; i <= len1; i++)
    d[i][0] = copy[i][0] = i;
		
  //distanta de la orice prefix din al doilea sir la primul sir	
  for(i = 0; i <= len2; i++)
    d[0][i] = copy[0][i] = i;
		
  for(i = 1; i <= len1; i++)
  {
    for(j = 1; j <= len2; j++)
    {
      //nu exista vreo diferenta
      if(sir[i - 1] == definition[j - 1])
      {
        d[i][j] = copy[i][j] = d[i - 1][j - 1];
      }
      else
      {	
        //se ia costul minim dintre o stergere, o inserare si o inlocuire
        d[i][j] = copy[i][j] = 
          min(d[i - 1][j - 1] + 1, min(d[i - 1][j] + 1, d[i][j - 1] + 1));
      }
    }
  }
}

//functie care intoarce numarul de spatii dintr-un sir
int spaces(char *word)
{
  char *p_word = strchr(word, ' ');
  int k = 0;
	
  while(p_word != NULL)
  {
    k++;
    p_word = strchr(p_word + 1, ' ');
  }	
	
  return k;
}

int main()
{	
  typedef struct {char word[MAX_SIZE]; unsigned int freq;} cuvant;
  typedef struct {cuvant spelling; unsigned int distance;} definition;
	
  cuvant dict[SIZE_DICT];
  definition def[SIZE][SIZE];
	
  int i, j, k, a, b, len, len_word;
  char sir[SIZE], corectat[SIZE], sir2[MAX_SIZE], *p;
	
  FILE *f = fopen("dict.txt", "r");
	
  if(f == NULL) 
  {
    printf("Nu s-a putut deschide fisierul\n");
    return 0;
  }
	
  //se citeste sirul de corectat
  strcpy(sir, fgets(sir, SIZE, stdin));
	
  //se scot spatiile din sirul de corectat
  memset(corectat, 0, sizeof(corectat));
	
  p = strtok(sir, " \n");
	
  while(p != NULL)
  {
    strcat(corectat, p);
    p = strtok(NULL, " \n");	
  }
	
  //se memoreaza datele din dictionar
  for(i = 0; i < SIZE_DICT; i++)
    int bytes = fscanf(f, "%s %u\n", dict[i].word, &dict[i].freq);

  len = strlen(corectat);

  //se creeaza toate cuvintele posibile, daca am fi pus spatii
  for(i = 0; i < SIZE; i++) 
  { 
    for(j = i; j < SIZE; j++)
    {
      def[i][j].distance = MAX_SIZE;
      def[i][j].spelling.freq = 0;
			
      strncpy(def[i][j].spelling.word, corectat + i, j - i + 1);
      strcat(def[i][j].spelling.word, "\0");
    }
  }
  
  int copy[SIZE][SIZE];
  
  //se retin cele mai bune solutii din matricea de cuvinte 
  for(i = 0; i < len; i++)
  {
    //vom retine distantele de editare intr-o matrice 
    memset(copy, 0, sizeof(copy));
		
    for(j = 0; j < SIZE_DICT; j++)
    {
      len_word = strlen(dict[j].word);
			
      //se calculeaza distanta de editare intre cuvantul din dictionar
      //si cuvantul curent
      levenshtein(dict[j].word, corectat + i, copy);
			
      for(k = i; k < len; k++)
      {
        //se retine cuvantul cu distanta de editare minima
        if(def[i][k].distance > copy[len_word][k - i + 1])
        {
          def[i][k].distance = copy[len_word][k - i + 1]; 
          def[i][k].spelling = dict[j];
        }
        else
        {
          //daca distanta de editare este aceeasi
          if(def[i][k].distance == copy[len_word][k - i + 1])
          {
            //se retine cuvantul cu frecventa maxima
            if(def[i][k].spelling.freq < dict[j].freq)
            {
              def[i][k].spelling = dict[j];
            }
            else
            {
              //la frecvente egale, se ia cuvantul care este minim lexical
              if(def[i][k].spelling.freq == dict[j].freq)  
              {
                if(strcmp(def[i][k].spelling.word, dict[j].word) > 0)
                {
                  strcpy(def[i][k].spelling.word, dict[j].word);
                  strcat(def[i][k].spelling.word, "\0");
                }
              }
            }			
          }	
        }
      }
    }
  }
 
  //se face corectia sirului initial
  for(i = 0; i < len; i++)
  {
    for(j = i + 1; j < len; j++)
    {
      for(k = i; k <= j - 1; k++)
      {
        //se retine fraza cu cea mai mica distanta de editare
        if(def[i][j].distance > 
          (def[i][k].distance + def[k + 1][j].distance))
        {
          def[i][j].distance = def[i][k].distance + def[k + 1][j].distance;
          def[i][j].spelling.freq = 
            def[i][k].spelling.freq + def[k + 1][j].spelling.freq; 
					
          strcpy(def[i][j].spelling.word, def[i][k].spelling.word);
          strcat(def[i][j].spelling.word, " "); 
          strcat(def[i][j].spelling.word, def[k + 1][j].spelling.word);
          strcat(def[i][j].spelling.word, "\0");
        }
        else
        {
          //daca frazele au aceeasi distanta de editare
          if(def[i][j].distance == (def[i][k].distance + def[k + 1][j].distance))
          {
            a = spaces(def[i][j].spelling.word);
					
            b = spaces(def[i][k].spelling.word); 
            b += spaces(def[k + 1][j].spelling.word);
            b++;
	
            //se retine fraza cu cele mai putine cuvinte		
            if(a > b)
            {
              def[i][j].spelling.freq = 
                def[i][k].spelling.freq + def[k + 1][j].spelling.freq;
								
              strcpy(def[i][j].spelling.word, def[i][k].spelling.word);
              strcat(def[i][j].spelling.word, " "); 
              strcat(def[i][j].spelling.word, def[k + 1][j].spelling.word);
              strcat(def[i][j].spelling.word, "\0");
            }
            else
            {
              //daca frazele au acelasi numar de cuvinte
              if(a == b)
              {
                //se retine fraza cu frecventa totala maxima
                if(def[i][j].spelling.freq <
                  (def[i][k].spelling.freq + def[k + 1][j].spelling.freq))
                {
                  def[i][j].spelling.freq = 
                  def[i][k].spelling.freq + def[k + 1][j].spelling.freq;
							
                  strcpy(def[i][j].spelling.word, def[i][k].spelling.word);
                  strcat(def[i][j].spelling.word, " "); 
                  strcat(def[i][j].spelling.word, def[k + 1][j].spelling.word);
                  strcat(def[i][j].spelling.word, "\0");
                }		
                else
                {
                  //daca frazele au aceeasi frecventa
                  if(def[i][j].spelling.freq == 
                    (def[i][k].spelling.freq + def[k + 1][j].spelling.freq))
                  {
                    strcpy(sir2, def[i][k].spelling.word);
                    strcat(sir2, " "); 
                    strcat(sir2, def[k + 1][j].spelling.word);
                    strcat(sir2, "\0");
						
                    //se retine fraza minima lexicografic
                    if(strcmp(def[i][j].spelling.word, sir2) > 0) 
                    {		    	
                      strcpy(def[i][j].spelling.word, sir2);
                    }
                  }
                }
              }	
            }
          }
        }		
      }
    } 
  }
	
  //se afiseaza corectia sirului de la intrare
  printf("%s\n", def[0][len - 1].spelling.word);
	
  fflush(stdin);
  fclose(f);
	
  return 0;
}
