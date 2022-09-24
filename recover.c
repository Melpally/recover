#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //Ensure the correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }
    //open the card file
    char *infile = argv[1];
    FILE *inptr = fopen(argv[1], "r");

    if (infile == NULL)
    {
        printf("Could not open a file.");
        return 2;
    }
    //Initialise values of counter, block_size, buffer of bytes and the filename
    int counter = 0;
    int block_size = 512;
    typedef uint8_t BYTE;
    BYTE buffer[block_size];
    char filename[8];
    FILE *outptr;
    int jpeg_has_been_found = 0;

    //While statement to keep reading the file until the end is reached
    while (fread(&buffer, sizeof(BYTE)*block_size, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //IF the jpeg exists, keep going
            if (jpeg_has_been_found == 1)
            {
                fclose(outptr);
                jpeg_has_been_found++;
            }
            jpeg_has_been_found = 1;
            //creeate the file of the type ###.jpg and write the bytes from the buffer
            sprintf(filename, "%03i.jpg", counter);
            outptr = fopen(filename, "w");
            counter++;
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
        else if (jpeg_has_been_found == 1)
        {
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }

    }
    //close the files
    fclose(inptr);

    return 0;
}