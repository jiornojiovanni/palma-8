#define OFFSET 512
#define ROWS 64
#define COLUMNS 32

typedef struct
{
    unsigned char RAM[4096];
    unsigned char SP;
    unsigned short PC;
    unsigned short I;
    unsigned char V[16];
    char video[64][32];
} VM;

VM initState();
void halt(unsigned char instruction);
void cicle(VM *state);
