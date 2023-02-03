#include <iostream>
/* Incrementa en 2 una variable */
int cuenta (int y) {
	int tmp;
	tmp = y + 2;
	return tmp;
}
/* Calcula la multiplicación de dos números */
int multiplica (int x, int y) {
	int final1;
	int i;
	final1 = 0;
	for (i = 0; i < x; i ++) {
		final1 = final1 + y;
	}
	return final1;
}
int main (void) {
	int final1;
	int final2;
	int i;
	final1 = multiplica(3, 2);
	
	for (i = 0; i < 100; i ++)
		final2 = cuenta(i);
	std::cout << final1 << "\n";
	return 0;
}
