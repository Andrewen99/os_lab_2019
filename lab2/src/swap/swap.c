#include "swap.h"

void Swap(char *left, char *right)
{
	// мой код здесь
	char middle = *left;
  	*left = *right;
  	*right = middle;
}
