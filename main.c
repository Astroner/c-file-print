#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 256
#define SIZE (WIDTH * WIDTH)

size_t statistic[SIZE];
unsigned char image[SIZE];
char PALETTE[] = " .:-=+*#%@@";

/**
 * Amplification function:
 * f(x) = -(1 / (x / TUNER) + 1/255) + 255
 * 
 * Modified hyperbolic function to take any number and produce number less than or equal to 255
*/

#define TUNER 1000

unsigned char amplify(size_t x) {
    return 255 - ((255 * TUNER) / (255 * x + TUNER));
}


struct Args { 
    char* inputFile; 
    char* outputFile;
    int printIntoConsole;
    int printWidth;
};

int parseArgs(int argc, char** argv, struct Args* args) {
    if(argc < 2) {
        printf("No input file provided\n");

        return 1;
    }

    args->inputFile = NULL;
    args->outputFile = "mark.png";
    args->printIntoConsole = 0;
    args->printWidth = 16;

    for(int i = 1; i < argc; i++) {
        char* current = argv[i];

        if(current[0] != '-') {
            if(args->inputFile) {
                printf("Unexpected argument '%s'\n", current);
                return 1;
            }

            args->inputFile = current;

            continue;
        }

        if(current[1] == '-') {
            // Rejecting big flags for now;
            printf("Unknown argument '%s'\n", current);
            return 1;
        }

        if(current[2] != '\0') {
            // Rejecting flags with length > 1
            printf("Unknown argument '%s'\n", current);
            return 1;
        }

        switch(current[1]) {
            case 'o': // Specify output file
                if(i + 1 >= argc) {
                    printf("No output file after -o");
                    return 1;
                }
                args->outputFile = argv[i + 1];
                i++;
                break;

            case 'P': // Print into console and dont write file
                #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
                args->outputFile = NULL;
            case 'p': // Print result into console
                args->printIntoConsole = 1;

                if(i + 1 >= argc) break;
                char* nextArg = argv[i + 1];
                if(nextArg[1] != '\0') break;

                int size = 0;
                switch(nextArg[0]) {
                    case 's':
                        size = 16;
                        break;
                    
                    case 'm':
                        size = 32;
                        break;

                    case 'b':
                        size = 64;
                        break;

                    default:
                        printf("Invalid value for print size '%c', valid ones: s, m, b\n", nextArg[0]);

                        return 1;
                        break;
                }
                i++;

                args->printWidth = size;

                break;

            case 'h':
                printf(
                    "\n"
                    "This tool generates file binary visualization in form of Digraph Dot Plot View.\n"
                    "fprint [-p | -P [size]][-o output.file] input-file-name\n"
                    "\n"
                    "Flags:\n"
                    "    -p, -P [size]  Print the plot in ascii art format into the stdout.\n"
                    "                   -P prevents file generation, only stdout print\n"
                    "                   Optional [size] after the flag changes the print resolution.\n"
                    "                   Available sizes: s, m, b\n"
                    "                   Default: s\n"
                    "\n"
                    "              -o   Specifies output file. Provided name should be with .png extension.\n"
                    "                   Default: mark.png\n"
                    "\n"
                );
                return 1;

            default:
                printf("Unknown argument '%s'\n", current);
                return 1;
        }
    }

    if(args->inputFile == NULL) {
        printf("No input file provided\n");
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {

    struct Args args;
    if(parseArgs(argc, argv, &args) != 0) {
        return 1;
    }

    printf("Creating a mark of '%s'\n", args.inputFile);

    FILE* f = fopen(args.inputFile, "rb");

    if(!f) {
        printf("Failed to open %s\n", args.inputFile);
        return 1;
    }


    int x = -1;
    int y = -1;

    int byte;
    while((byte = getc(f)) != EOF) {
        if(x < 0) {
            x = byte;
            continue;
        }

        y = byte;

        if(statistic[y * WIDTH + x] < (SIZE_MAX - 1)) {
            statistic[y * WIDTH + x] += 1;
        }

        x = -1;
    }
    
    fclose(f);

    for(int i = 0; i < SIZE; i++) {
        image[i] = 255 - amplify(statistic[i]);
    }

    if(args.outputFile) {
        printf("Outputting into %s\n", args.outputFile);
        stbi_write_png(args.outputFile, WIDTH, WIDTH, 1, image, 0);
    }


    if(args.printIntoConsole) {
        int pixelPerChar = WIDTH / args.printWidth;
        printf("Mark:\n");
        for(int row = 0; row < args.printWidth; row++) {
            printf("|");
            for(int col = 0; col < args.printWidth; col++) {
                unsigned char max = 0;
                for(int py = 0; py < pixelPerChar; py++) {
                    for(int px = 0; px < pixelPerChar; px++) {
                        unsigned char pixelValue = 255 - image[
                            row * WIDTH * pixelPerChar 
                            + col * pixelPerChar 
                            + py * WIDTH 
                            + px
                        ];
                        if(pixelValue > max) max = pixelValue;
                    }
                }
                char symbol = PALETTE[max * (sizeof(PALETTE) - 1) / 256];
                printf("%c%c", symbol, symbol);
            }
            printf("|\n");
        }
    }

    return 0;
}
