#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

float get_grade(string input);

int main(void)
{
    string input = get_string("Text: ");
    double grade = get_grade(input);
    
    //output according to grade
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int)roundf(grade));
    }
}

//helper function to get Coleman-Liau index of an input string
float get_grade(string input)
{
    int len = strlen(input);
    // printf("%i",len);
    float letters = 0;
    float words = 1;
    float sentences = 0;
    for (int i = 0; i < len; i++)
    {
        if (i > 0 && input[i] == ' ' && input[i - 1] != ' ')
        {
            words++; 
        }
        else if (input[i] == '.' || input[i] == '!' || input[i] == '?')
        {
            sentences++;
        }
        else if (isalpha(input[i]))
        {
            letters++;
        }
    }
    // printf("%f, %f, %f", letters, words, sentences);
    float grade = 0.0588 * (letters * 100 / words) - 0.296 * (sentences * 100 / words) - 15.8;
    return grade;
}