#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CAP_IMPLEMENTATION
#include "cap.h"

#define WIDTH 256
#define SIZE (WIDTH * WIDTH)

size_t statistic[SIZE];
unsigned char image[SIZE];
char PALETTE[] = " .:-=+*#%@@";



/**
 * Just a linear function of file size
*/
#define DEFAULT_TUNER 1000
size_t getTuner(fpos_t fileSize) {
    return fileSize / 700 + DEFAULT_TUNER;
}

/**
 * Amplification function:
 * f(x) = -(1 / (x / TUNER) + 1/255) + 255
 * 
 * Modified hyperbolic function to take any number and produce number less than or equal to 255
 * 
 * The bigger TUNER, the slower the function grows.
*/
unsigned char amplify(size_t x, size_t tuner) {
    return 255 - ((255 * tuner) / (255 * x + tuner));
}

struct Args { 
    char* inputFile; 
    char* outputFile;
    int printIntoConsole;
    int printWidth;
};

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("No input file provided\n");

        return 1;
    }

    struct Args args = {
        .inputFile = NULL,
        .outputFile = "file-print.png",
        .printIntoConsole = 0,
        .printWidth = 16,
    };

    CAP_PARSE_SWITCH(argc - 1, argv + 1) {
        CAP_ARGS(inputFile, {
            if(args.inputFile) {
                printf("Unexpected argument '%s'\n", inputFile);
                return 1;
            }

            args.inputFile = inputFile;
        })
        CAP_FLAGS(
            CAP_MATCH_FLAG('o', {
                char* value = Cap_getFlagValue();
                if(!value){
                    printf("No output file after -o");
                    return 1;
                }

                args.outputFile = value;
            })
            CAP_ADD_FLAG_MATCH('P')
            CAP_MATCH_FLAG('p', {
                if(Cap_getCurrentFlag() == 'P') {
                    args.outputFile = NULL;
                }
                args.printIntoConsole = 1;

                CAP_CHECK_NEXT(optionalValue) {
                    if(optionalValue.type == CAP_ARG && optionalValue.value.arg[1] == '\0') {
                        switch(optionalValue.value.arg[0]) {
                            case 's':
                                args.printWidth = 16;
                                break;
                            
                            case 'm':
                                args.printWidth = 32;
                                break;

                            case 'b':
                                args.printWidth = 64;
                                break;

                            default:
                                printf("Invalid value for print size '%c', valid ones: s, m, b\n", optionalValue.value.arg[0]);

                                return 1;
                        }
                        CAP_CHECK_CONFIRM();
                    }
                }
            })
            CAP_MATCH_FLAG('h', {
                printf(
                    "\n"
                    "This tool generates file binary visualization in form of Digraph Dot Plot View.\n"
                    "fprint [-p | -P [size]][-o output.png] input-file-name\n"
                    "\n"
                    "Flags:\n"
                    "    -p, -P [size]  Print the plot in ascii art format into the stdout.\n"
                    "                   -P prevents file generation, only stdout print\n"
                    "                   Optional [size] after the flag changes the print resolution.\n"
                    "                   Available sizes: s, m, b\n"
                    "                   Default: s\n"
                    "\n"
                    "              -o   Specifies output file. Provided name should be with .png extension.\n"
                    "                   Default: file-print.png\n"
                    "\n"
                );

                return 0;
            })
            CAP_UNMATCHED_FLAGS(ch, {
                printf("Unknown flag -%c\n", ch);
                return 1;
            })
        )
        CAP_LONG_FLAGS(
            CAP_UNMATCHED_LFLAGS(name, printf("Unknown option --%s\n", name->str);)
        )
    }

    if(args.inputFile == NULL) {
        printf("No input file provided\n");
        return 1;
    }

    printf("Creating a print of '%s'\n", args.inputFile);

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
    
    fpos_t fileSize;
    fgetpos(f, &fileSize);
    size_t tuner = getTuner(fileSize);

    fclose(f);

    for(int i = 0; i < SIZE; i++) {
        image[i] = 255 - amplify(statistic[i], tuner);
    }

    if(args.outputFile) {
        printf("Outputting into %s\n", args.outputFile);
        stbi_write_png(args.outputFile, WIDTH, WIDTH, 1, image, 0);
    }


    if(args.printIntoConsole) {
        int pixelPerChar = WIDTH / args.printWidth;
        printf("ASCII Print:\n");
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
