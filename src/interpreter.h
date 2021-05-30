#define OFFSET 512
#define ROWS 32
#define COLUMNS 64

typedef struct
{
    unsigned char RAM[4096];
    unsigned char SP;
    unsigned short PC;
    unsigned short I;
    unsigned char V[16];
    char video[ROWS][COLUMNS];
} VM;

VM initState();
void halt(unsigned char instruction);
void cicle(VM *state);
