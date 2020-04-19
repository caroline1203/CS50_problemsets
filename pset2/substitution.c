#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

bool valid(string s);
string cipher(string input, string vode);

int main(int argc, string argv[])
{
    //if the input is not one argument
    if (argc != 2)
    {
        printf("Please input one argument.\n");
        return 1;
    }
    
    string code = argv[1];
    if (valid(code))
    {
        string input = get_string("plaintext: ");
        printf("ciphertext: %s\n", cipher(input, code));
    }
    else
    {
        printf("Invalid input.\n");
        return 1;
    }
    
}

//check if the input is valid with all and exactly 26 letters, regardless of case
bool valid(string s)
{
    if (strlen(s) != 26)
    {
        return false;
    }
    //use hashtable to check if all the letters are inside
    int count = 0;
    for (int i = 0; i < 26; i++)
    {
        count += (int) pow(2, tolower(s[i]) - 'a');
    }
    return ((int)(pow(2, 26) - 1) ^ count) == 0;
}

//cipher the input based on the code
string cipher(string input, string code)
{
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        if (islower(input[i]))
        {
            input[i] = tolower(code[input[i] - 'a']);
        }
        else if (isupper(input[i]))
        {
            input[i] = toupper(code[input[i] - 'A']);
        }
    }
    return input;
}