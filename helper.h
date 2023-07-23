struct node {
    struct node *prev;
    struct node *next;
    int dPage, page, usedTime;
};

struct node *newNodes(int page) {

    struct node *newNode = calloc(1, sizeof(struct node));
    (*newNode).page = page;
    (*newNode).dPage = 0;
    return newNode;
}

char *algName;
int algorithm = 0;  
int max = 0, count = 0, totalPageR = 0;
int pageMiss = 0, pageMissTime = 0, writeBackTime = 0, totalWBT = 0;  
char operation, *line = NULL;
struct node *head = NULL;
struct node *tail = NULL;
size_t linelen = 20;