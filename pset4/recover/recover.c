#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check input
    if (argc != 2)
    {
        printf("Please enter one image name.\n");
        return 1;
    }

    //check if file exist
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Couldn't open the file.\n");
        return 1;
    }

    //prepare buffer, image number, new image file and name string
    BYTE buffer[512];
    int count = 0;
    FILE *image = NULL;
    char name[8];
    
    //read till the end of card
    while (fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        //check if it's a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            //finish last one
            if (image != NULL)
            {
                fclose(image);
            }
            
            //create new image file;
            sprintf(name, "%03i.jpg", count);
            count++;
            image = fopen(name, "w");
        }
        
        //write the file
        if (image != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, image);
        }
    }
    
    fclose(image);
    fclose(file);
    return 0;

}
