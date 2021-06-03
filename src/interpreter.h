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
    unsigned short STACK[16];
    unsigned char video[ROWS][COLUMNS];
    unsigned char KB[16];
    unsigned char KBinterrupt;
    unsigned char KBrequest;
    unsigned char DT;
    unsigned char ST;
} VM;

VM initState();
void halt(unsigned short instruction);
void cicle(VM *state);
