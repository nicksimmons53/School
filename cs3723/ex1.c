 #include <stdio.h>

int iX = 100;
int iY = 200;
int iZ = 300;
void funcB();
void funcA()
{
    int iZ = 400;
    iX += 10;
    iY += 10;
    printf("funcA: %d %d %d\n", iX, iY, iZ); 
    funcB();
}
void funcB()
{
    iX += 5;
    iY += 5;
    iZ += 5;
    printf("funcB: %d %d %d\n", iX, iY, iZ); 
}
int main( )
{
	funcA( );
	return 0;
}

