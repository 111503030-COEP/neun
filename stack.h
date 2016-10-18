#define MAX 128
typedef struct stack{
	int a[MAX];
	int i;
}stack;
void push(stack *s, int num);
int pop(stack *s);
int empty(stack *s);
int full(stack *s);
void init(stack *s);
