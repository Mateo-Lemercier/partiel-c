#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 6

int AskInt( const char* sMessage )
{
	while( 1 )
	{
		printf( "%s\n", sMessage);

		int iValue;
		int iSuccess = scanf_s( "%d", &iValue );
		while( getchar() != '\n' );

		if( iSuccess == 1 )
			return iValue;

		printf( "Entrée non valide" );
	}
}

// ===========
// #TODO 1 pts
// ===========
typedef struct Grid
{
	//Mettre les attributs utiles pour representer, calculer et afficher la grille de jeu 
	char** cell;

} Grid;

// =============
// #TODO 0.5 pts
// =============
void Init( Grid* pGrid )
{
	//Initialise les attributs de la structure Grid
	pGrid->cell = (char**)malloc(sizeof(char*) * SIZE);
	for (int i = 0; i < SIZE; i++)
	{
		pGrid->cell[i] = (char*)malloc(sizeof(char) * SIZE);
		for (int j = 0; j < SIZE; j++)
		{
			pGrid->cell[i][j] = 0;
		}
	}
}

// ===========
// #TODO 1 pts
// ===========
void Print( const Grid* pGrid )
{
	system("cls"); //Nettoie l'écran

	for (int iRow = SIZE - 1; iRow >= 0; iRow--)
	{
		for (int iColumn = 0; iColumn < SIZE; iColumn++)
		{
			if (pGrid->cell[iRow][iColumn] != 0)
				printf("[%c]",pGrid->cell[iRow][iColumn]);
			else
				printf("[ ]");
		}
		printf("\n");
	}

	//Affiche la grille de jeu au format:
	
	/*
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][O][X][ ][ ]
		[O][O][X][X][O][ ]
	*/
	
	//Attention doit s'adapter si on change la valeur de SIZE (ligne 5)
}

// =============
// #TODO 0.5 pts
// =============
int AskValidColumn( const Grid* pGrid )
{
	//Demande à l'utilisateur de rentrer une colonne, 
	//si celle-ci est pleine il faudra re demander à l'utilisateur une colonne valide.
	int iColumn;
	char isFull;
	while (1)
	{
		iColumn = AskInt("Dans quelle colonne voulez-vous jouer ?") - 1;

		if (!(0 <= iColumn && iColumn < SIZE))
		{
			printf("Cette colonne n'existe pas\n");
			continue;
		}
		
		isFull = 1;
		for (int iRow = 0; iRow < SIZE; iRow++)
		{
			if (pGrid->cell[iRow][iColumn] == 0)
				isFull = 0;
		}
		if (isFull == 1)
		{
			printf("Cette colonne est déjà remplie\n");
			continue;
		}
		
		return iColumn + 1;
	}
	
	//renvoie l'index de la colonne
}

// ===========
// #TODO 1 pts
// ===========
int FillColumn( Grid* pGrid, int iColumn, char cSymbol )
{
	//Rempli une case à partir d'une colonne en simulant la gravité:

	//Premier tour: la colonne 1 et le symbole 'O':
	/*
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[O][ ][ ][ ][ ][ ]
	*/

	//Deuxième tour: la colonne 1 et le symbol 'X':
	/*
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[ ][ ][ ][ ][ ][ ]
		[X][ ][ ][ ][ ][ ]
		[O][ ][ ][ ][ ][ ]
	*/

	//cSymbol represente le symbole à utiliser, 'X' ou 'O'

	iColumn--; // Column 1 = Index 0 so you have to iColumn-- to get the Index

	for (int iRow = 0; iRow < SIZE; iRow++)
	{
		if (pGrid->cell[iRow][iColumn] == 0)
		{
			pGrid->cell[iRow][iColumn] = cSymbol;
			return iRow + 1;
		}
	}

	//renvoie l'index de la ligne sur laquelle le symbole a été mis
}

// ===========
// #TODO 4 pts
// ===========
bool IsWin( const Grid* pGrid, int iLine, int iColumn )
{
	//Detecte si le dernier coup joué est un coup gagnant
	//4 symboles à la suite sur une ligne, une colonne ou une diagonale (gauche ou droite)
	
	iLine--; // The comments weren't clear about the iLine being the same as the Index or 1 above so I'm refering to the FillColumn
	iColumn--; // The comments weren't clear about the iColumn being the same as the Index or 1 above so I'm refering to the FillColumn

	
	
	int iLineStart = iLine - 3;
	if (iLineStart < 0) iLineStart = 0;
	int iLineEnd = iLine + 3;
	if (iLineEnd >= SIZE) iLineEnd = SIZE - 1;
	
	int iCount = 1;
	char cLastSymbol = ' ';
	for (int i = iLineStart; i <= iLineEnd; i++)
	{
		if (pGrid->cell[i][iColumn] != cLastSymbol || pGrid->cell[i][iColumn] == 0)
		{
			iCount = 1;
			cLastSymbol = pGrid->cell[i][iColumn];
			continue;
		}
		
		iCount++;
		if (iCount >= 4)
			return true;
	}

	
	
	int iColumnStart = iColumn - 3;
	if (iColumnStart < 0) iColumnStart = 0;
	int iColumnEnd = iColumn + 3;
	if (iColumnEnd >= SIZE) iColumnEnd = SIZE - 1;

	iCount = 1;
	cLastSymbol = ' ';
	for (int i = iColumnStart; i <= iColumnEnd; i++)
	{
		if (pGrid->cell[iLine][i] != cLastSymbol || pGrid->cell[iLine][i] == 0)
		{
			iCount = 1;
			cLastSymbol = pGrid->cell[iLine][i];
			continue;
		}

		iCount++;
		if (iCount >= 4)
			return true;
	}



	iCount = 1;
	cLastSymbol = ' ';
	int distance;
	if (iLineEnd - iLine < iColumn - iColumnStart) distance = iLineEnd - iLine;
	else distance = iColumn - iColumnStart;
	int i = iLine + distance;
	int j = iColumn - distance;
	while (i >= iLineStart && j <= iColumnEnd)
	{
		if (pGrid->cell[i][j] != cLastSymbol || pGrid->cell[i][j] == 0)
		{
			iCount = 1;
			cLastSymbol = pGrid->cell[i][j];
			i--; j++;
			continue;
		}

		iCount++;
		if (iCount >= 4)
			return true;
		
		i--; j++;
	}



	iCount = 1;
	cLastSymbol = ' ';
	distance;
	if (iLine - iLineStart < iColumn - iColumnStart) distance = iLine - iLineStart;
	else distance = iColumn - iColumnStart;
	i = iLine - distance;
	j = iColumn - distance;
	while (i <= iLineEnd && j <= iColumnEnd)
	{
		if (pGrid->cell[i][j] != cLastSymbol || pGrid->cell[i][j] == 0)
		{
			iCount = 1;
			cLastSymbol = pGrid->cell[i][j];
			i++; j++;
			continue;
		}

		iCount++;
		if (iCount >= 4)
			return true;
		
		i++; j++;
	}
	
	
	
	return false;

	//renvoie true si le dernier coup est gagnant, false sinon
}

// ===========
// #TODO 1 pts
// ===========
bool IsFull( const Grid* pGrid )
{
	//renvoie true si la grille est pleine, false sinon
	for (int iRow = 0; iRow < SIZE; iRow++)
	{
		for (int iColumn = 0; iColumn < SIZE; iColumn++)
		{
			if (pGrid->cell[iRow][iColumn] == 0)
				return false;
		}
	}
	
	return true;
}

// ===========
// #TODO 3 pts
// ===========
int GetValidRandomColumn( const Grid* pGrid )
{
	//Choisi une colonne aléatoire, celle-ci ne doit pas être pleine
	
	//Attention: 
	//l'aléatoire doit être uniforme et ne doit pas intervenir dans le temps d'execution de l'algorithme


	int* validColumns = (int*)malloc(sizeof(int) * SIZE);
	for (int iColumn = 0; iColumn < SIZE; iColumn++)
	{
		validColumns[iColumn] = iColumn;
	}

	char isFull;
	int temp;
	int iCount = 0;
	for (int iColumn = 0; iColumn < SIZE; iColumn++)
	{
		isFull = 1;
		for (int iRow = 0; iRow < SIZE; iRow++)
		{
			if (pGrid->cell[iRow][validColumns[iColumn - iCount]] == 0)
				isFull = 0;
		}
		if (isFull == 1)
		{
			temp = validColumns[iColumn - iCount];
			validColumns[iColumn - iCount] = validColumns[SIZE - 1 - iCount];
			validColumns[SIZE - 1 - iCount] = temp;
			iCount++;
		}
	}

	int random = rand() % (SIZE - iCount);
	return validColumns[random] + 1;

	
	//renvoie l'index de la colonne
}

// ===========
// #TODO 3 pts
// ===========
int main()
{
	// =====================
	// Initialisation du jeu
	// =====================

	srand( time( NULL ) );

	Grid oGrid;
	Init( &oGrid );

	/*
		Ajouter toutes les variables necessaires au fonctionnement du jeu
	*/
	int iColumn;
	int iLine;
	char cSymbol = 'X';

	while( 1 )
	{	
		// =============
		// Boucle de jeu
		// =============

		/*
			-Affiche la grille
			
			-Alterne 1 tour sur 2: 
				-Demande à l'utilisateur de choisir une colonne
				-Choisi une colonne aléatoirement (pour simuler le coup adversaire)
			
			-Met à jour la grille avec le symbole correspondant ('O' pour le joueur 'X' pour l'adversaire)
			
			-Vérifie l'état de la grille:
				-Si le coup est gagnant: affiche "Vous avez gagné" ou "L'adversaire a gagné" et quitte la boucle
				-Si la grille est pleine: affiche "Match nul" et quitte la boucle
		*/

		Print(&oGrid);

		if (cSymbol == 'X')
			iColumn = AskValidColumn(&oGrid);
		else
			iColumn = GetValidRandomColumn(&oGrid);

		iLine = FillColumn(&oGrid, iColumn, cSymbol);

		if (IsWin(&oGrid, iLine, iColumn))
		{
			if (cSymbol == 'X') printf("Vous avez gagné");
			else printf("L'adversaire a gagné");
			return 0;
		}
		
		else if (IsFull(&oGrid))
		{
			printf("Match nul");
			return 0;
		}

		if (cSymbol == 'X') cSymbol = 'O';
		else cSymbol = 'X';
	}

	return 0;
}
