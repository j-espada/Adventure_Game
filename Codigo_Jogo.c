/*
* Jogo de Aventuras
*
*  Actualizado a: 28/11/2015
*  Autores: Joaquim Espada e João Orvalho
*/

#include "stdafx.h" // ficheiro com definições do Windows etc
#include <stdio.h> // para o printf etc
#include <string.h>
#include <stdlib.h> // para o system("pause") , cores etc
#include <locale.h> // Para acentuacao etc
#include <time.h>
#include <iostream>  // Para Cores
#include <windows.h>   // WinApi header
/*Para Cores*/
using namespace std;    // std::cout, std::cin
HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
/* Fim Cores*/


#define VERSAO "V1.0"
#define MOVE_INFO 100


//Player options
#define MAX_NAME 100
#define MAX_ENGERGY 10
#define GOD_MODE_ENERGY 100
#define PLAYER_INIT_CELL 0
#define NO_TREASURE  0
#define TREASURE  1
#define NO_COOKIE 0
#define COOKIE 1
#define COOKIE_ENERGY 40



//Monster options
#define MONSTER_NAME "Resident"
#define MONSTER_MAX_ENGERGY 10
#define MONSTER_INIT_CELL 7

//Map options
#define MAP_CELLS 100
#define MAX_CELL_DESCRIPTION 256

//Cell description
#define CELL_0_DESCRIPTION "You are in a dark and putrid forest . Oh look a Mansion !\nDo you have the guts to enter ? \n"
#define CELL_1_DESCRIPTION "You are at the main hall you can hear ticking ... Sounds like a clock \n!"
#define CELL_2_DESCRIPTION "You are at the dinning room. I found the clock! There is something strange about this ... \nA secret passage !! \n"
#define CELL_3_DESCRIPTION "You are at a small and dark corridor! Look a door, isn´t an ordinary door , has an armor craved in it ... \nI'm going to enter ! \n"
#define CELL_4_DESCRIPTION "You are at large room . There is a piano with a partiture . I'm going to play it. Another secret passage \n! "
#define CELL_5_DESCRIPTION "You are at a studio there are some strange notes about a Treasure hidden in the courtyard .There is only one way out of this room! \n"
#define CELL_6_DESCRIPTION "A small deposit room , there is a typewritter machine with a story about the Mansion! Horrible things happened here! \n"
#define CELL_7_DESCRIPTION "You are at the courtyard there is something next to that crypt! \n"
#define CELL_8_DESCRIPTION "You are in a small closet! \n"
/* Estruturas*/

#define MAX_LIN 1000
#define MAX_L 200


struct Cell {
	int north;
	int south;
	int east;
	int west;
	char description[MAX_CELL_DESCRIPTION];
	int treasure;
	int cookie;
};

struct Map {

	struct Cell cells[MAP_CELLS];
	int nCells;
};

struct Monster
{
	char name[MAX_NAME];
	int cell;
	int energy;

};

struct Player {
	char name[MAX_NAME];
	int energy;
	int cell;
	int treasure;
};

/* Fim Estruturas*/

/* Prototipo de funções*/
void printPlayer(struct Player *pPlayer);
void PrintMap(struct Map *pMap);
void InitializeGameItems(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster);
void GameEngine(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster);
void Movement(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster);
void InitializeCombat(struct Player *pPlayer, struct Monster *pMonster);
void LockedDoors();
void EndGame();
void WinGame();
void Menu();
void NewGame();
void LoadGame();
void SaveGame(struct Map *pMap, struct Player *pPlayer, struct Monster *pMonster);
void SaveMapBin(struct Map *pMap);
void InitializeMapFBin(struct Map *pMap);
int file_exist(char *filename);
void GetExtension(char *filename);
/* Fim prototipo de funções */


// Global variables
char MapToUse[50];
char Extension_Map[50];
int su = 0; // para saber em que modo se joga (super user,com mais energia, ou normal, energia normal)





int main(int argc, char* argv[])
{
	srand(time(NULL));

	if ((argc >= 2) && (argc <= 3)) {


		// system("color 0A");
		setlocale(LC_ALL, "Portuguese");
		SetConsoleTextAttribute(hConsole, 78);
		printf("Version: %s\n", VERSAO);
		SetConsoleTextAttribute(hConsole, 15); // White

		if (((strcmp(argv[1], "su") == 0)) | ((strcmp(argv[1], "SU") == 0))) { // Detecta caso a inicialização do jogo seja feita por um "super user"
			su = 1;

			if (argc == 2) {
				printf("Error! - Invalid Parameters \n");
				printf("> Start the game like this: game.exe mapa.txt (example) \n");
				return 1;
			}

			strcpy(MapToUse, argv[2]);
		}
		else {
			strcpy(MapToUse, argv[1]);
		}


		if (!file_exist(MapToUse))
		{
			printf("Error ! - The map in parameter does not exist!\n");
			printf("> Start the game like this: game.exe mapa.txt (example) \n");
			return 1;
		}



		GetExtension(MapToUse); // Detecta extensao de Mapa
		


		Menu();



	}
	else {
		printf("Error! - Start the game like this: game.exe mapa.txt (example) \n");
		return 1;
	}


	return 0;
}



void GetExtension(char *filename) {
	int a = 0;
	int i = 0;
	while ((filename[i] != '.') && (filename[i] != '\0'))
		i++;
	while (filename[i] != '\0') {
		Extension_Map[a] = toupper(filename[i]); // toupper - converter para maiusculas
		a++;
		i++;
	}
	Extension_Map[a] = '\0';	


}



int file_exist(char *filename)
{
	FILE* fptr = fopen(filename, "r");
	if (fptr != NULL)
	{
		fclose(fptr);
		return 1;
	}
	printf("File doesnt exist\n");
	return 0;
}



void Menu() {

	int option = 0;
	int continuar = 1;
	char str[MAX_LIN];

	printf("\n################### Menu ###################\n");
	printf("#                                          #\n");
	printf("#   1. New Game                            #\n");
	printf("#   2. Load Game                           #\n");
	printf("#   3. Quit                                #\n");
	printf("#                                          #\n");
	printf("############################################\n\n");

	while (true) {

		printf("What is your choice? [1-3]:");
		scanf("%s", str);
		option = atoi(str); // evita bugs caso o utilizador insira uma string en vez de um numero


		switch (option) {
		case 0: // se o utilizador nao inserir um numero o programa retorna automaticamente 0
			SetConsoleTextAttribute(hConsole, 12); // Red
			printf("Invalid Option ! Try again...\n\n");
			SetConsoleTextAttribute(hConsole, 15); // White
			break;
		case 1:
			NewGame();
			break;
		case 2:
			LoadGame();
			break;
		case 3:
			EndGame();
			break;
		default:
			SetConsoleTextAttribute(hConsole, 12); // Red
			printf("Invalid Option ! Try again...\n\n");
			SetConsoleTextAttribute(hConsole, 15); // White
			break;
		}
	}


}

void NewGame() {


	// Inicializa as Estruturas
	struct Player player;
	struct Map map;
	struct Monster monster;

	// Fim da inicialização
	InitializeGameItems(&player, &map, &monster); // Iniciliza os Itens do jogo:  mapa , jogador, monstro
	GameEngine(&player, &map, &monster); // Inicializa motor de jogo


}

void InitializePlayer(struct Player *pPlayer) {

	printf("Traveler what is your name ? ");

	scanf(" %[^\n]s", pPlayer->name);


	if (su == 1) {
		pPlayer->energy = GOD_MODE_ENERGY;
	}
	else if (su == 0)
	{
		pPlayer->energy = MAX_ENGERGY;
	}

	pPlayer->cell = PLAYER_INIT_CELL;
	pPlayer->treasure = NO_TREASURE;
}

void InitializeMonster(struct Monster *pMonster) {

	strcpy(pMonster->name, MONSTER_NAME);
	pMonster->energy = MONSTER_MAX_ENGERGY;
	pMonster->cell = MONSTER_INIT_CELL;

}

void printMonster(struct Monster *pMonster) {
	SetConsoleTextAttribute(hConsole, 11); // Light Blue
	printf("*********************Monster Info********************* \n");
	SetConsoleTextAttribute(hConsole, 15); //  White
	printf("*Monster 's name  -> %s\n", pMonster->name);
	printf("*Monster's energy -> %d\n", pMonster->energy);
	printf("*Monster's cell -> %d\n", pMonster->cell);
}

void printPlayer(struct Player *pPlayer) {
	SetConsoleTextAttribute(hConsole, 11); // Light Blue
	printf("*********************Traveler Info********************* \n");
	SetConsoleTextAttribute(hConsole, 15); //  White
	printf("*Traveler's name  -> %s\n", pPlayer->name);
	printf("*Traveler's energy -> %d\n", pPlayer->energy);
	printf("*Traveler's cell -> %d\n", pPlayer->cell);
	if (pPlayer->treasure == 0) {
		printf("*Traveler has treasure ? -> No \n");
	}

	else {

		printf("*Traveler has treasure ? -> Yes \n");
	}

}

void GameEngine(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster) {

	while (true)
	{

		Movement(pPlayer, pMap, pMonster);

		if (pPlayer->cell == 0 && pPlayer->treasure == 1) {
			WinGame();
			break;
		}


	}
}


void InitializeCombat(struct Player *pPlayer, struct Monster *pMonster) {

	if ((pPlayer->cell == pMonster->cell) && (pMonster->energy > 0) && (pPlayer->energy > 0)) {

		SetConsoleTextAttribute(hConsole, 160);
		printf("The %s apperead Let the Combat begin !!!\n", pMonster->name);
		SetConsoleTextAttribute(hConsole, 15); // White

		while ((pMonster->energy > 0) && (pPlayer->energy > 0)) {


			int monsterDamage = rand() % 4;
			int playerDamage = rand() % 3;


			if ((rand() % 2) == 1) { // Ataca o Jogador

				pMonster->energy = pMonster->energy - playerDamage;


				printf("%s attack -> %d  \n%s energy -> %d ", pPlayer->name, playerDamage, pMonster->name, pMonster->energy);

				if (pMonster->energy <= 0) {
					SetConsoleTextAttribute(hConsole, 161);
					printf("\nYou won the batlle! \n");
					SetConsoleTextAttribute(hConsole, 15); // White
				}
			}
			else { // Ataca Monstro

				pPlayer->energy = pPlayer->energy - monsterDamage;
				printf("%s attack ->  %d  \n%s energy -> %d  ", pMonster->name, monsterDamage, pPlayer->name, pPlayer->energy);


				if (pPlayer->energy <= 0) {
					SetConsoleTextAttribute(hConsole, 206);
					printf("You died !\n");
					SetConsoleTextAttribute(hConsole, 15); // White
					printf("Press Any Key to Continue...");
					getchar(); getchar(); // Tem que ficar as 2 funcoes
					Menu();
				}
			}

			Sleep(1000);
		}
	}

}



void MonsterMov(struct Monster *pMonster, struct Map *pMap) {

	int stay = rand() % 2;

	if (stay == 0 && pMonster->energy > 0) {



		int w, s, n, e, i = 0; // i= numero de portas na celula
		int doors[4];

		if (pMap->cells[pMonster->cell].west != -1) {
			doors[i] = pMap->cells[pMonster->cell].west; // adiciona porta ao array
			i++;
		}


		if (pMap->cells[pMonster->cell].south != -1) {
			doors[i] = pMap->cells[pMonster->cell].south; // adiciona porta ao array
			i++;
		}


		if (pMap->cells[pMonster->cell].north != -1) {
			doors[i] = pMap->cells[pMonster->cell].north; // adiciona porta ao array
			i++;

		}


		if (pMap->cells[pMonster->cell].east != -1) {
			doors[i] = pMap->cells[pMonster->cell].east; // adiciona porta ao array
			i++;
		}

		int moveX = rand() % i;
		int newPos = doors[moveX];

		/*
		Para Testar:

		for (int a = 0; a <= i - 1; a++)
		{
		printf("[%d] - Opcao Possivel: %d \n", a, doors[a]);
		}

		printf("Random: Inteligencia Artificial: %d \n", newPos);

		*/



		pMonster->cell = newPos;

	}


}


void Movement(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster) {

	char moveData[MOVE_INFO];


	while (true) {

		MonsterMov(pMonster, pMap); // Movimenta o Monstro

		if (pPlayer->cell == pMonster->cell) {
			InitializeCombat(pPlayer, pMonster); // Se os 2 estiverem na mesma célula
		}

		printf("%s -> %d\n", pPlayer->name, pPlayer->cell); // Descreve a localizacao do utilizador
		if (su == 1) {
			printf("%s tresure -> %d\n", pPlayer->name, pPlayer->treasure);
			printf("%s -> %d\n", pMonster->name, pMonster->cell);
		}

		do
		{
			SetConsoleTextAttribute(hConsole, 14); // Yellow
			printf("Type w , e , s , n to move or k to save game \n");
			SetConsoleTextAttribute(hConsole, 15); // White
			scanf("%s", moveData);

			if (moveData[1] != '\0') { // Segundo caracter inserido tem alguma coisa?
				SetConsoleTextAttribute(hConsole, 12); // Red
				printf("Enter only one character! \n");
				SetConsoleTextAttribute(hConsole, 15); // White
			}


		} while (moveData[1] != '\0'); // Para ter acerteza que o utilizador inseriu apenas um caracter




		switch (moveData[0]) {


		case 'w':
			if (pMap->cells[pPlayer->cell].west != -1) {

				SetConsoleTextAttribute(hConsole, 10); // Green
				pPlayer->cell = pMap->cells[pPlayer->cell].west;
				if (pPlayer->treasure == 0)
					pPlayer->treasure = pMap->cells[pPlayer->cell].treasure;
				printf("%s \n", pMap->cells[pPlayer->cell].description);
				SetConsoleTextAttribute(hConsole, 15); // White

			}
			else
				LockedDoors();
			break;


		case 's':
			if (pMap->cells[pPlayer->cell].south != -1) {

				SetConsoleTextAttribute(hConsole, 10); // Green
				pPlayer->cell = pMap->cells[pPlayer->cell].south;
				if (pPlayer->treasure == 0)
					pPlayer->treasure = pMap->cells[pPlayer->cell].treasure;
				printf("%s \n", pMap->cells[pPlayer->cell].description);
				SetConsoleTextAttribute(hConsole, 15); // White

			}
			else
				LockedDoors();
			break;

		case 'n':
			if (pMap->cells[pPlayer->cell].north != -1) {
				SetConsoleTextAttribute(hConsole, 10); // Green
				pPlayer->cell = pMap->cells[pPlayer->cell].north;
				if (pPlayer->treasure == 0)
					pPlayer->treasure = pMap->cells[pPlayer->cell].treasure;
				printf("%s \n", pMap->cells[pPlayer->cell].description);
				SetConsoleTextAttribute(hConsole, 15); // White
			}
			else
				LockedDoors();
			break;

		case 'e':
			if (pMap->cells[pPlayer->cell].east != -1) {
				SetConsoleTextAttribute(hConsole, 10); // Green
				pPlayer->cell = pMap->cells[pPlayer->cell].east;
				if (pPlayer->treasure == 0)
					pPlayer->treasure = pMap->cells[pPlayer->cell].treasure;
				printf("%s \n", pMap->cells[pPlayer->cell].description);
				SetConsoleTextAttribute(hConsole, 15); // White
			}
			else
				LockedDoors();
			break;
		case 'k':
			SaveGame(pMap, pPlayer, pMonster);
			break;
		default:
			SetConsoleTextAttribute(hConsole, 12); // Red
			printf("Invalid Movement ! \n");
			SetConsoleTextAttribute(hConsole, 15); // White
			break;
		}

		if (pPlayer->cell == pMonster->cell) {
			InitializeCombat(pPlayer, pMonster); // Se os 2 estiverem na mesma célula
		}


		if (pMap->cells[pPlayer->cell].treasure == 1)
		{
			printf("You found the treasure !!! \nYou need to return to the point of the start \n");
			pMap->cells[pPlayer->cell].treasure = 0; // O jogador já encontrou o tesouro!
			pPlayer->treasure = 1;
		}

		if ((pMap->cells[pPlayer->cell].cookie == 1)) // Armario com uma bolacha de energia
		{
			pPlayer->energy = pPlayer->energy + COOKIE_ENERGY;
			pMap->cells[pPlayer->cell].cookie = 0;
			printf("%s energy -> %d\n", pPlayer->name, pPlayer->energy);
		}


		if (pPlayer->treasure == 1 && pPlayer->energy > 0 && pPlayer->cell == 0)
			break;



	}



}

void LockedDoors() {

	SetConsoleTextAttribute(hConsole, 228);
	int randInt = rand() % 3; // 0 - 2
	switch (randInt)
	{
	case 0:
		printf("That door is locked !\n");
		break;
	case 1:
		printf("Cannot go that way!\n");
		break;
	case 2:
		printf("I sense a dark force that way ... Not going ...\n");
		break;
	default:
		SetConsoleTextAttribute(hConsole, 12); //  Red
		printf("Invalid path\n");
		break;
	}
	SetConsoleTextAttribute(hConsole, 15); //  White
}

void InitializeMapFTXT(struct Map *pMap) { // Caracteristicas dos mapas


	char l[MAX_LIN]; // Vai guardando o que lê do ficheiro txt para o array

	FILE * f;


	f = fopen(MapToUse, "r"); // read file

	fscanf(f, "%s", l);
	int nCells = atoi(l);
	pMap->nCells = nCells;


	for (int i = 0; i <= nCells; i++) {
		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].north = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].south = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].west = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].east = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].treasure = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		pMap->cells[i].cookie = atoi(l);
		// Necessita de fazer duas vezes fgets
		fgets(l, MAX_LIN, f);
		fgets(l, MAX_LIN, f);
		//printf("%s \n", l);
		strcpy(pMap->cells[i].description, l);

	}

	SaveMapBin(pMap);
}


void LoadGame() { // Caracteristicas dos mapas


				  // Inicializa as Estruturas
	struct Player player;
	struct Map map;
	struct Monster monster;

	char l[MAX_LIN]; // Vai guardando o que lê do ficheiro txt para o array

	FILE * f;


	if (!file_exist("SavedGame.txt"))
	{
		printf("Error ! - You don't have recorded games!\n");
		EndGame();
	}

	f = fopen("SavedGame.txt", "r"); // read file

	fscanf(f, "%s", l);
	int nCells = atoi(l);
	//printf("Numero de celulas: %d \n", nCells);
	map.nCells = nCells;



	for (int i = 0; i <= nCells; i++) {
		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].north = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].south = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].west = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].east = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].treasure = atoi(l);

		fscanf(f, "%s", l);
		//printf("%d \n", atoi(l));
		map.cells[i].cookie = atoi(l);
		// Necessita de fazer duas vezes fgets
		fgets(l, MAX_LIN, f);
		fgets(l, MAX_LIN, f);
		//printf("DESCRICAO: %s \n", l);
		strcpy(map.cells[i].description, l);

	}



	fgets(l, MAX_LIN, f);
	fgets(l, MAX_LIN, f);
	int i = 0;
	char result[20];
	while (l[i] != '\0' && l[i] != '\n')
	{
		result[i] = l[i];
		i++;
	}
	result[i] = '\0';
	strcpy(player.name, result);
	fscanf(f, "%s", l);
	player.energy = atoi(l);
	fscanf(f, "%s", l);
	player.cell = atoi(l);
	fscanf(f, "%s", l);
	player.treasure = atoi(l);



	fgets(l, MAX_LIN, f);
	fgets(l, MAX_LIN, f);
	i = 0;
	char resultM[20];
	while (l[i] != '\0' && l[i] != '\n')
	{
		resultM[i] = l[i];
		i++;
	}
	resultM[i] = '\0';
	strcpy(monster.name, resultM);
	fscanf(f, "%s", l);
	monster.energy = atoi(l);
	fscanf(f, "%s", l);
	monster.cell = atoi(l);



	fclose(f);


	GameEngine(&player, &map, &monster); // Inicializa motor de jogo


}


void InitializeMap(struct Map *pMap) { // Caracteristicas dos mapas
	pMap->nCells = 8;

	//Cell 0
	pMap->cells[0].north = -1;
	pMap->cells[0].south = -1;
	pMap->cells[0].west = -1;
	pMap->cells[0].east = 1;
	pMap->cells[0].treasure = NO_TREASURE;
	pMap->cells[0].cookie = NO_COOKIE;
	strcpy(pMap->cells[0].description, CELL_0_DESCRIPTION);

	//Cell 1

	pMap->cells[1].north = -1;
	pMap->cells[1].south = 2;
	pMap->cells[1].west = 0;
	pMap->cells[1].east = -1;
	pMap->cells[1].treasure = NO_TREASURE;
	pMap->cells[1].cookie = NO_COOKIE;
	strcpy(pMap->cells[1].description, CELL_1_DESCRIPTION);

	//Cell 2

	pMap->cells[2].north = 1;
	pMap->cells[2].south = 8;
	pMap->cells[2].west = -1;
	pMap->cells[2].east = 3;
	pMap->cells[2].treasure = NO_TREASURE;
	pMap->cells[2].cookie = NO_COOKIE;
	strcpy(pMap->cells[2].description, CELL_2_DESCRIPTION);

	//Cell 3

	pMap->cells[3].north = 4;
	pMap->cells[3].south = -1;
	pMap->cells[3].west = 2;
	pMap->cells[3].east = -1;
	pMap->cells[3].treasure = NO_TREASURE;
	pMap->cells[3].cookie = NO_COOKIE;
	strcpy(pMap->cells[3].description, CELL_3_DESCRIPTION);

	//Cell 4

	pMap->cells[4].north = -1;
	pMap->cells[4].south = 3;
	pMap->cells[4].west = -1;
	pMap->cells[4].east = 5;
	pMap->cells[4].treasure = NO_TREASURE;
	pMap->cells[4].cookie = NO_COOKIE;
	strcpy(pMap->cells[4].description, CELL_4_DESCRIPTION);

	//Cell 5

	pMap->cells[5].north = -1;
	pMap->cells[5].south = 6;
	pMap->cells[5].west = 4;
	pMap->cells[5].east = -1;
	pMap->cells[5].treasure = NO_TREASURE;
	pMap->cells[5].cookie = NO_COOKIE;
	strcpy(pMap->cells[5].description, CELL_5_DESCRIPTION);

	//Cell 6

	pMap->cells[6].north = 5;
	pMap->cells[6].south = -1;
	pMap->cells[6].west = -1;
	pMap->cells[6].east = 7;
	pMap->cells[6].treasure = NO_TREASURE;
	pMap->cells[6].cookie = NO_COOKIE;
	strcpy(pMap->cells[6].description, CELL_6_DESCRIPTION);

	//Cell 7

	pMap->cells[7].north = -1;
	pMap->cells[7].south = -1;
	pMap->cells[7].west = 6;
	pMap->cells[7].east = -1;
	pMap->cells[7].treasure = TREASURE;
	pMap->cells[7].cookie = NO_COOKIE;
	strcpy(pMap->cells[7].description, CELL_7_DESCRIPTION);

	// Cell 8

	pMap->cells[8].north = 2;
	pMap->cells[8].south = -1;
	pMap->cells[8].west = -1;
	pMap->cells[8].east = -1;
	pMap->cells[8].treasure = NO_TREASURE;
	pMap->cells[8].cookie = COOKIE;
	strcpy(pMap->cells[8].description, CELL_8_DESCRIPTION);


}


void PrintMap(struct Map *pMap) {
	SetConsoleTextAttribute(hConsole, 11); // Light Blue
	printf("*********************Map Info********************* \n");
	SetConsoleTextAttribute(hConsole, 15); //  White
	for (int i = 0; i < pMap->nCells; i++)
	{
		SetConsoleTextAttribute(hConsole, 10); // Green
		printf("Cell %d -> Treasure  %d -> %s \n", i, pMap->cells[i].treasure, pMap->cells[i].description);
		SetConsoleTextAttribute(hConsole, 15); // White
	}
}

void InitializeGameItems(struct Player *pPlayer, struct Map *pMap, struct Monster *pMonster) {
	InitializePlayer(pPlayer); // Inicializa o Jogador
	InitializeMonster(pMonster); // Inicializa o monstro

	if (strcmp(Extension_Map, ".TXT") == 0) {
		InitializeMapFTXT(pMap); // Inicializa o Mapa do jogo
	}
	else if (strcmp(Extension_Map, ".BIN") == 0) {
		InitializeMapFBin(pMap);
	}
	else {
		printf("Error! - Invalid Map \n");
		EndGame();
	}
	
							 //InitializeMapFBin(pMap); // Inicializa o Mapa Binario do jogo 
	printPlayer(pPlayer); // Mostra detalhes sobre o Jogador
	printMonster(pMonster); // Mostra detalhes sobre o Monstro
							//PrintMap(pMap);//Mostra detalhes do mapa
	printf("******************************\n");
	SetConsoleTextAttribute(hConsole, 10); // Green
	cout << ("%s \n%s \n", "The game is about to begin", pMap->cells[pPlayer->cell].description) << endl;
	SetConsoleTextAttribute(hConsole, 15); // White
	printf("******************************\n");
}


void WinGame() {
	printf("\n---You Win!---\n");
	printf("Press Any Key to Continue...");
	getchar();      getchar(); // Tem que ficar as 2 funcoes

	Menu();

}

void EndGame() {

	printf("Press Any Key to Continue...");
	getchar(); getchar(); // Tem que ficar as 2 funcoes
	exit(0); // sair e tudo correu bem
}

void SaveGame(struct Map *pMap, struct Player *pPlayer, struct Monster *pMonster) {
	char buff[MAX_L];
	FILE *fp;

	fp = fopen("SavedGame.txt", "w");
	if (fp == NULL)
		exit(-1);


	fprintf(fp, strcat(itoa(pMap->nCells, buff, 10), "\n\n"));

	for (int i = 0; i <= pMap->nCells; i++)
	{
		//strcat = concatenação de Strings | itoa = converte inteiro para string
		fprintf(fp, strcat(itoa(pMap->cells[i].north, buff, 10), "\n"));
		fprintf(fp, strcat(itoa(pMap->cells[i].south, buff, 10), "\n"));
		fprintf(fp, strcat(itoa(pMap->cells[i].west, buff, 10), "\n"));
		fprintf(fp, strcat(itoa(pMap->cells[i].east, buff, 10), "\n"));
		fprintf(fp, strcat(itoa(pMap->cells[i].treasure, buff, 10), "\n"));
		fprintf(fp, strcat(itoa(pMap->cells[i].cookie, buff, 10), "\n"));
		fprintf(fp, pMap->cells[i].description);

		if (i != (pMap->nCells)) { // para nao deixar espacos no final do ficheiro e fazer quebras de linha
			fprintf(fp, "\n");
		}

	}

	fprintf(fp, "\n");
	fprintf(fp, pPlayer->name); //Nome do Jogador
	fprintf(fp, "\n");
	fprintf(fp, strcat(itoa(pPlayer->energy, buff, 10), "\n")); // Enegia do Player
	fprintf(fp, strcat(itoa(pPlayer->cell, buff, 10), "\n")); // Celula do Player
	fprintf(fp, strcat(itoa(pPlayer->treasure, buff, 10), "\n")); // Tesouro Player
	fprintf(fp, pMonster->name); // nome do Monstro
	fprintf(fp, "\n");
	fprintf(fp, strcat(itoa(pMonster->energy, buff, 10), "\n")); // Energia do Monster
	fprintf(fp, strcat(itoa(pMonster->cell, buff, 10), "\n"));  // Celula do Monstro
	fclose(fp);
	SetConsoleTextAttribute(hConsole, 10); // Green
	printf("Success! - The game has been saved\n\n");
	SetConsoleTextAttribute(hConsole, 15); // White



}



void SaveMapBin(struct Map *pMap) {
	char buff[MAX_L];
	char temp[MAX_L];

	FILE *file = fopen("MapBin.bin", "wb");

	if (file == NULL)
		exit(-1);

	strcpy(temp, strcat(itoa(pMap->nCells, buff, 10), "\n"));
	fwrite(temp, 1, sizeof(temp), file);



	for (int i = 0; i <= pMap->nCells; i++)
	{

		strcpy(temp, strcat(itoa(pMap->cells[i].north, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, strcat(itoa(pMap->cells[i].south, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, strcat(itoa(pMap->cells[i].west, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, strcat(itoa(pMap->cells[i].east, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, strcat(itoa(pMap->cells[i].treasure, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, strcat(itoa(pMap->cells[i].cookie, buff, 10), "\n"));
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, pMap->cells[i].description);
		fwrite(temp, 1, sizeof(temp), file);

		strcpy(temp, "\n");
		fwrite(temp, 1, sizeof(temp), file);


	}



	fclose(file);
	SetConsoleTextAttribute(hConsole, 10); // Green
	printf("Success! - The Map has been saved in  binary mode\n\n");
	SetConsoleTextAttribute(hConsole, 15); // White


	
}

void InitializeMapFBin(struct Map *pMap) {
	

	char temp[MAX_L];
	char buffer[MAX_L];


	FILE *file2 = fopen("MapBin.bin", "rb");

	fread(buffer, 1, sizeof(temp), file2);
	int nCells = atoi(buffer);
	pMap->nCells = nCells;


	for (int i = 0; i <= nCells; i++) {


		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].north = atoi(buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].south = atoi(buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].west = atoi(buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].east = atoi(buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].treasure = atoi(buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		pMap->cells[i].cookie = atoi(buffer);
		//printf("%s", buffer);


		fread(buffer, 1, sizeof(temp), file2);
		strcpy(pMap->cells[i].description, buffer);
		//printf("%s", buffer);

		fread(buffer, 1, sizeof(temp), file2);
		//printf("%s", buffer);

	}

}