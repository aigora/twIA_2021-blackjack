#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>


#define DIMMANO 7
#define SI 1
#define NO 0
#define NUM_BARAJAS 1

//ESTRUCTURAS

typedef struct {
	int fichas; //Recoge el numero de fichas del usuario.
	int mano[DIMMANO]; //Recoge un numero del 0 al 51, el cual mas adelante esta asociado con un palo y un numero.
	int valmano[DIMMANO]; //Recoge el valor de la carta (por ejemplo, si es una J, recoge el valor 10).
	int tmano; //Recoge el tamaño de la mano del usuario.
	int sum; //Recoge la suma de los valores de las cartas.
	int f_ap; //Recoge las fichas apostadas en la partida. Solo usuarios
	int blackjack; // Asigna el valor 0 o 1 si hay Blackjack o no
	int charlie; // Asigna el valor 0 o 1 si hay Charlie o no

}INFO;


// Funciones prototipo
int menu_principal(void);

void configura(void);

int opc1();

void opc2();

void opc3();

int modo_normal();

int un_jugador(int);

void cont_cartas(int[], int[], int);

int reval_as(int, int, int[], int[], int*);

void palo_cartas(int[], int);

int Apuesta(INFO[], int);

int Reparto_fichas(int, int, int, int, int, int, int, int);

void reparto_cartas(int[], int, int[]);


int main(void)
{
	int opc;

	// Tareas de configuración y carga
	configura();

	printf("\t\t\t ---BIENVENIDO---\n\n\t\t");
	system("PAUSE");



	// Bucle principal de la aplicación
	do
	{
		opc = menu_principal();
		switch (opc)
		{
		case 1:

			opc = opc1();
			break;//FIN DE LA PRIMERA OPCION
		case 2:

			opc2();
			break;//FIN DE LA SEGUNDA OPCION
		case 3:

			opc3();
			break;//FIN DE LA TERCERA OPCION

		case 4:
			return 0;
			break;
		}
	} while (opc != 4);
	// CIERRE DEL PROGRAMA
}


//ARGUMENTO FUNCIONES

//se desplega el menu principal
int menu_principal(void)
{
	int opcion;
	do
	{
		printf("\n\t 1.JUGAR AL BLACKJACK\n\n");
		printf("\t 2.COMO JUGAR\n\n");
		printf("\t 3.REGLAS\n\n");
		printf("\t 4.CERRAR EL JUEGO\n\n");
		printf("Seleccione opción:");
		scanf_s("%d", &opcion);
		if (opcion < 1 || opcion>4)
			printf("\nOpción inexistente.\n\n");
	} while (opcion < 1 || opcion>4);
	return opcion;
}

void configura(void)
{
	setlocale(LC_ALL, "spanish");
}

//funcion que se inicia cuando se escoge la opcion 1 (jugar blackjack)
int opc1() {

	int mod = 0, opc;


	do {
		printf("\t\t\t 1.MODO NORMAL\n\n");

		printf("Introduzca el valor 1 para empezar a jugar: ");
		scanf_s("%d", &mod);

		if (mod == 1) {
			mod = modo_normal();
			if (mod == 0) {
				return 5;
			}
		}
	} while (mod != 1 && mod != 2);

}

//Funcion opcion 2 (como jugar) IGNACIO
void opc2() {
	char lectura;

	FILE* pf;
	errno_t err;


	printf("\n\t\t\t\t\t\t------------------------\n");
	printf("\n\t\t\t\t\t\tCOMO JUGAR AL BLACKJACK:\n");
	printf("\n\t\t\t\t\t\t------------------------\n");

	err = fopen_s(&pf, "como_jugar.txt", "r");

	//Comprobacion de la apertura del archivo
	if (err != 0)
		printf("El archivo NO esta abierto\n");

	//Lee el archivo como_jugar.txt
	while ((lectura = fgetc(pf)) != EOF)
		printf("%c", lectura);

	//Cierre del fichero como_jugar.txt
	fclose(pf);
}

//Funcion opcion 3 (reglas) IGNACIO
void opc3() {
	char lectura;

	FILE* pf;
	errno_t err;


	printf("\n\t\t\t\t\t\t------------------------\n");
	printf("\n\t\t\t\t\t\t\tREGLAS:\n");
	printf("\n\t\t\t\t\t\t------------------------\n");

	err = fopen_s(&pf, "reglas.txt", "r");

	//Comprobacion de la apertura del archivo
	if (err != 0)
		printf("El archivo NO esta abierto\n");

	//Lee el archivo reglas.txt
	while ((lectura = fgetc(pf)) != EOF)
		printf("%c", lectura);

	//Cierre del fichero como_jugar.txt
	fclose(pf);
}

//se inicia si se escoge el modo normal 
int modo_normal() {

	int jug, back, r;


	do {
		printf("\n\n\tINTRODUZCA LA CANTIDAD DE JUGADORES (dos jugadores maximo): ");
		scanf_s("%d", &jug);
		printf("\n\n");

		if (jug == 1 || jug == 2) {
			back = un_jugador(jug);

			if (back == 0) {
				printf("\n\nDesea volver al menu principal?\n");
				printf("\t1.Volver al menu principal");
				printf("\t2.Salir del juego");
				do {
					printf("\nInserte opcion: ");
					scanf_s("%d", &r);
				} while (r != 1 && r != 2);

				if (r == 1) {
					return 1;
				}
				else {
					printf("\n\n\t\t---GRACIAS POR JUGAR. ---");
					return 0;
				}
			}
		}

		else
			printf("\n\nOpcion no valida");

	} while (jug != 1 && jug != 2);

}


//codigo base del juego. ALBERTO
int un_jugador(int jug) {

	int i = 0;
	int c3 = 0; //Tercer condicional (caracter-valor)
	int c2 = 0; // Segundo condicional 1 (eleccion de cartas).(
	int af = 0; // (afirmacion de eleccion)
	int repeticiones[52]; //

	int* pvalmuser, * pvalmcrupier; //Punteros para falicitar la cuenta de los valores de las cartas.

	int onoff = 0; //permite volver a jugar tantas veces como desee.
	int info; //sirve para calificar en la funcion info_user si ha ganado, empatado o perdido.


	INFO u1[2], crupier;

	srand(time(NULL));

	for (i = 0; i < 2; i++) {
		u1[i].fichas = 100;
		u1[i].f_ap = 0;//Inicializo la variable para poder apostar.
		u1[i].sum = 0; //Inicializo la variable para poder calcular el puntaje.
		u1[i].tmano = 0;//Inicializo la variable para calcular el tamaño de la mano.
		u1[i].blackjack = 0;
		u1[i].charlie = 0;
	}
	crupier.sum = 0;
	crupier.tmano = 0;
	crupier.charlie = 0;
	crupier.blackjack = 0;

	for (i = 0; i < 52; i++) {
		repeticiones[i] = 0;
	} //Inicializacion del vector


	pvalmuser = (int*)malloc(u1[i].tmano * sizeof(int));//determinamos un puntero que contiene la informacion sobre los valores del usuario. MAL DEFINIDO

	pvalmcrupier = &crupier.valmano[0];

	do {
		for (i = 0; i < jug; i++) {
			printf("\n\t--JUGADOR %d--\n", (i + 1));

			u1[i].f_ap = Apuesta(u1, i);
		}

		//BUCLE REPARTO DE LAS PRIMERAS CARTAS DEL USUARIO
		for (i = 0; i < jug; i++) {

			printf("\n\nMANO DEL JUGADOR %d: \n", (i + 1));

			c3 = 0;
			pvalmuser = &u1[i].valmano[0]; //Ponemos el puntero apuntando al vector que recoga la info de las cartas del usuario.

			while (c3 < 2) {
				printf("\tCarta %d: ", (c3 + 1));

				reparto_cartas(u1[i].mano, c3, repeticiones);

				cont_cartas(u1[i].mano, u1[i].valmano, c3);

				palo_cartas(u1[i].mano, c3);

				u1[i].sum = u1[i].sum + *pvalmuser;

				if (u1[i].sum > 21) {
					u1[i].sum = reval_as(u1[i].sum, c3, u1[i].valmano, u1[i].mano, pvalmuser); //Revalorarizacion de la carta AS. De valer 11, pasa a valer 1.
					pvalmuser++;
					c3++;
					u1[i].tmano++;
				}
				else {
					pvalmuser++;
					c3++;
					u1[i].tmano++;
				}

				system("PAUSE");
			}		//FIN BUCLE REPARTO DE LAS PRIMERAS CARTAS DEL USUARIO

			printf("TU SUMA ES: %d. \n", u1[i].sum);
		}


		c3 = 0;

		printf("\n\nMANO DEL CRUPIER: \n");

		//BUCLE REPARTO DE LA PRIMERA CARTA DEL CRUPIER
		while (c3 < 1) {
			printf("\tCarta %d: ", (c3 + 1));

			reparto_cartas(crupier.mano, c3, repeticiones);

			cont_cartas(crupier.mano, crupier.valmano, c3);
			palo_cartas(crupier.mano, c3);

			crupier.sum = crupier.sum + *pvalmcrupier;

			pvalmcrupier++;
			c3++;
			crupier.tmano++;

			system("PAUSE");
		}
		//FIN DEL BUCLE REPARTO DE LA PRIMERA CARTA DEL CRUPIER

		printf("LA SUMA DEL CRUPIER ES: %d. \n", crupier.sum);


		for (i = 0; i < jug; i++) {
			pvalmuser = &u1[i].valmano[2]; //Ponemos nuestro vector apuntando a la posicion 2 de la mano.

			//SI EL USUARIO TIENE BLACKJACK
			if (u1[i].sum == 21) {
				printf("\n\n\t\t---ENHORABUENA JUGADOR %d. TIENES BLACKJACK---\n", (i + 1));
				u1[i].blackjack = 1;

			}

			//SI EL USUARIO NO TIENE BLACKJACK
			else {

				c3 = 2;//recolocamos la variable, la cual ha sido modificada para repartir la primera carta al crupier.

				//BUCLE DEL JUEGO PRINCIPAL DEL USUARIO.

				printf("\n\t\t\t--JUGADOR %d--\n", (i + 1));

				do {
					//si no tienes suficientes fichas para poder ejecutar la opcion 3
					if (u1[i].f_ap * 2 > u1[i].fichas) {


						printf("\n\t1. Plantarse (1).");
						printf("\t2. Pedir carta (2).");
						printf("\nElija una opcion: ");
						scanf_s("%d", &c2);
						switch (c2)
						{
						case 1: //Permite plantarse
							printf("TU SUMA ES %d. \n", u1[i].sum);
							af = 0;
							break;// FIN CASE 1

						case 2: //Permite pedir otra carta más

							do {
								printf("\tCarta %d: ", (c3 + 1));

								reparto_cartas(u1[i].mano, c3, repeticiones);

								cont_cartas(u1[i].mano, u1[i].valmano, c3);
								palo_cartas(u1[i].mano, c3);

								u1[i].sum = u1[i].sum + *pvalmuser;

								if (u1[i].sum > 21) {
									u1[i].sum = reval_as(u1[i].sum, c3, u1[i].valmano, u1[i].mano, pvalmuser); //revalorarizacion de la carta AS. de valer 11 pasa a valer 1.
									pvalmuser++;
									c3++;
									u1[i].tmano++;
								}
								else {
									pvalmuser++;
									c3++;
									u1[i].tmano++;
								}


								printf("TU SUMA ES: %d. \n", u1[i].sum);

								if (u1[i].sum < 21) {
									printf("Desea otra carta mas?\n");
									printf("\t0. No.");
									printf("\t1. Si.");
									do {
										printf("\nInserte opcion: ");
										scanf_s("%d", &af);
									} while (af != NO && af != SI);

									system("PAUSE");
								}


							} while (u1[i].tmano < 8 && u1[i].sum <= 21 && af == 1);

							printf("TU SUMA FINAL ES: %d. \n", u1[i].sum);
							if (u1[i].sum > 21)
								printf("SE PASA DE 21. HAS PERDIDO. \n");
							break;//FIN CASE 2

						default: //si no has introducido ningun valor de los de antes.
							printf("Valor no valido. Introduzca un valor entre el 1 o el 2.");
							break;


						}

						if (u1[i].tmano == 7 && u1[i].sum <= 21) {
							printf("\n\n\t\t---ENHORABUENA JUGADOR %d. TIENES BLACKJACK---\n", (i + 1));
							u1[i].charlie = 1;
						}

					}

					//si tienes suficientes fichas para poder ejecutar la opcion 3

					else {

						printf("\n\t1. Plantarse (1).");
						printf("\t2. Pedir carta (2).");
						printf("\t3. Doblar y pedir (3).");
						printf("\nElija una opcion: ");
						scanf_s("%d", &c2);
						switch (c2)
						{
						case 1: //Permite plantarse
							printf("TU SUMA ES %d. \n", u1[i].sum);
							af = 0;
							break;// FIN CASE 1

						case 2: //Permite pedir otra carta más

							do {
								printf("\tCarta %d: ", (c3 + 1));

								reparto_cartas(u1[i].mano, c3, repeticiones);

								cont_cartas(u1[i].mano, u1[i].valmano, c3);
								palo_cartas(u1[i].mano, c3);

								u1[i].sum = u1[i].sum + *pvalmuser;

								if (u1[i].sum > 21) {
									u1[i].sum = reval_as(u1[i].sum, c3, u1[i].valmano, u1[i].mano, pvalmuser); //revalorarizacion de la carta AS. de valer 11 pasa a valer 1.
									pvalmuser++;
									c3++;
									u1[i].tmano++;
								}
								else {
									pvalmuser++;
									c3++;
									u1[i].tmano++;
								}


								printf("TU SUMA ES: %d. \n", u1[i].sum);

								if (u1[i].sum < 21) {
									printf("Desea otra carta mas?\n");
									printf("\t0. No.");
									printf("\t1. Si.");
									do {
										printf("\nInserte opcion: ");
										scanf_s("%d", &af);
									} while (af != NO && af != SI);

									system("PAUSE");
								}


							} while (u1[i].tmano < 8 && u1[i].sum <= 21 && af == 1);


							printf("TU SUMA FINAL ES: %d. \n", u1[i].sum);
							if (u1[i].sum > 21)
								printf("SE PASA DE 21. HAS PERDIDO. \n");

							break;//FIN CASE 2

						case 3: //Permite doblar tu apuesta y pedir una única carta más

							u1[i].f_ap = u1[i].f_ap * 2;

							printf("Tu apuesta final es: %d fichas.\n\n", u1[i].f_ap);
							printf("\tCarta %d: ", (c3 + 1));

							reparto_cartas(u1[i].mano, c3, repeticiones);

							cont_cartas(u1[i].mano, u1[i].valmano, c3);
							palo_cartas(u1[i].mano, c3);

							u1[i].sum = u1[i].sum + *pvalmuser;

							if (u1[i].sum > 21) {
								u1[i].sum = reval_as(u1[i].sum, c3, u1[i].valmano, u1[i].mano, pvalmuser); //revalorarizacion de la carta AS. de valer 11 pasa a valer 1.
								pvalmuser++;
								c3++;
								u1[i].tmano++;
							}
							else {
								pvalmuser++;
								c3++;
								u1[i].tmano++;
							}

							printf("TU SUMA FINAL ES: %d. \n", u1[i].sum);
							if (u1[i].sum > 21)
								printf("SE PASA DE 21. HAS PERDIDO. \n");

							af = 0;

							system("PAUSE");
							break; //FIN DEL CASE 3

						default: //si no has introducido ningun valor de los de antes.
							printf("Valor no valido. Introduzca un valor entre el 1 o el 3.");
							break;

						}

					}

					if (u1[i].tmano == 7 && u1[i].sum <= 21) {

						printf("\n\n\t\t---ENHORABUENA JUGADOR %d. TIENES CHARLIE DE 7 CARTAS--\n", (i + 1));
						u1[i].charlie = 1;
					}
					u1[i].blackjack = 0;
				} while (af == 1 && u1[i].sum <= 21 && u1[i].tmano < 7);

			}
			//FIN DEL BUCLE DEL JUEGO PRINCIPAL DEL USUARIO.
		}



		if (u1[0].sum > 21 || u1[1].sum > 21) { //si te has pasado de 21, se le pone la suma a 0 para cumplir la condicion.
			for (i = 0; i < jug; i++) {
				if (u1[i].sum > 21)
					u1[i].sum = -1;
			}
		}
		//si no te pasas de 21, la partida continua.


		if (u1[0].sum < 21 || u1[1].sum < 21) {


			c3 = 1; //volvemos a revalorizar la variable del bucle a 1, ya que esta sacada la primera carta del crupier.
			pvalmcrupier = (int*)malloc(crupier.tmano * sizeof(int));//inicializo los vectores del crupier, aunque luego vuelven a ser modificados por el bucle de la mano final (por ese motivo, el tamaño se pone a 3.)

			pvalmcrupier = &crupier.valmano[1];

			printf("Introduce los caracteres en referencia a las cartas del crupier: \n");


			//BUCLE REPARTO DE LA SEGUNDA CARTA DEL CRUPIER
			while (c3 < 2) {
				printf("\tCarta %d: ", (c3 + 1));

				reparto_cartas(crupier.mano, c3, repeticiones);

				cont_cartas(crupier.mano, crupier.valmano, c3);
				palo_cartas(crupier.mano, c3);

				crupier.sum = crupier.sum + *pvalmcrupier;

				if (crupier.sum > 21) {
					crupier.sum = reval_as(crupier.sum, c3, crupier.valmano, crupier.mano, pvalmcrupier); //revalorarizacion de la carta AS. de valer 11 pasa a valer 1.
					pvalmcrupier++;
					c3++;
					crupier.tmano++;
				}
				else {
					pvalmcrupier++;
					c3++;
					crupier.tmano++;
				}

				system("PAUSE");
			}
			//FIN DEL BUCLE REPARTO DE LA SEGUNDA CARTA DEL CRUPIER

			printf("LA SUMA DEL CRUPIER ES: %d.\n", crupier.sum);

			system("PAUSE");

			if (crupier.sum == 21) {
				printf("\n\n\t\t---EL CRUPIER TIENE BLACKJACK. MALA SUERTE---\n");
				crupier.blackjack = 1;
			}

			//BUCLE DEL JUEGO PRINCIPAL DEL CRUPIER.
			else if (crupier.sum < 17) {

				do {
					printf("Introduzca la siguiente carta del crupier: \n");
					printf("\tCarta %d: ", (c3 + 1));

					reparto_cartas(crupier.mano, c3, repeticiones);

					cont_cartas(crupier.mano, crupier.valmano, c3);
					palo_cartas(crupier.mano, c3);

					crupier.sum = crupier.sum + *pvalmcrupier;

					if (crupier.sum > 21) {
						crupier.sum = reval_as(crupier.sum, c3, crupier.valmano, crupier.mano, pvalmcrupier); //revalorarizacion de la carta AS. de valer 11 pasa a valer 1.
						pvalmcrupier++;
						c3++;
						crupier.tmano++;
					}
					else {
						pvalmcrupier++;
						c3++;
						crupier.tmano++;
					}

					if (crupier.sum <= 21)
						printf("LA SUMA DEL CRUPIER ES: %d. \n", crupier.sum);
					else if (crupier.sum > 21) {
						printf("LA SUMA DEL CRUPIER ES: %d. SE PASA DE 21.\n", crupier.sum);
					}


					system("PAUSE");
					if (crupier.tmano == 7 && crupier.sum <= 21) {

						printf("\n\n\t\t---MALA SUERTE. EL CRUPIER TIENE CHARLIE---\n");
						crupier.charlie = 1;
					}

				} while (crupier.tmano < 7 && crupier.sum < 17);
			}
			//FIN DEL BUCLE DEL JUEGO PRINCIPAL DEL CRUPIER.

			if (crupier.sum > 21) //si te has pasado de 21, se le pone la suma a 0 para cumplir la condicion.
				crupier.sum = 0;

		}

		for (i = 0; i < jug; i++) {
			u1[i].fichas = Reparto_fichas(u1[i].blackjack, u1[i].sum, crupier.sum, crupier.blackjack, u1[i].f_ap, u1[i].fichas, u1[i].charlie, crupier.charlie);
			printf("\n\nFichas restantes: %d fichas", u1[i].fichas);
		}


		if (u1[i].fichas == 0) {
			printf("\n\n\t\t---TE HAS QUEDADO SIN FICHAS---");
			printf("\n\t\t\t FIN DE LA PARTIDA\n");
			system("PAUSE");
			exit;
		}

		else {
			printf("\n\nDesea volver a jugar?");
			printf("\n\t0. No.");
			printf("\t1. Si.");

			do {
				printf("\nInserte opcion: ");
				scanf_s("%d", &onoff);
			} while (af != NO && af != SI);
		}

		crupier.sum = 0;
		crupier.tmano = 0;

		pvalmcrupier = &crupier.valmano[0];
		pvalmuser = &u1[0].valmano[0];

		c3 = 0;

		u1[0].tmano = 0;
		u1[1].tmano = 0;

		u1[0].sum = 0;
		u1[1].sum = 0;

		c2 = 0;

		u1[0].f_ap = 0;
		u1[1].f_ap = 0;

		u1[0].blackjack = 0;
		u1[1].blackjack = 0;

		crupier.blackjack = 0;

		u1[0].charlie = 0;
		u1[1].charlie = 0;

		for (i = 0; i < 52; i++) {
			repeticiones[i] = 0;
		} //Borramos el registro

	} while (onoff == 1 || u1[0].fichas == 0 || u1[1].fichas == 0);

	return onoff;
}

//Asocia su valor aleatorio con la correspondiente numeracion de las cartas: As, 2, 3,..., J, Q K. IGNACIO
void cont_cartas(int mano[], int valmano[], int c3) {


	if (0 <= mano[c3] && mano[c3] <= 3) {
		valmano[c3] = 11;
		printf("As");
	}
	else if (4 <= mano[c3] && mano[c3] <= 7) {
		valmano[c3] = 2;
		printf("2");
	}
	else if (8 <= mano[c3] && mano[c3] <= 11) {
		valmano[c3] = 3;
		printf("3");
	}
	else if (12 <= mano[c3] && mano[c3] <= 15) {
		valmano[c3] = 4;
		printf("4");
	}
	else if (16 <= mano[c3] && mano[c3] <= 19) {
		valmano[c3] = 5;
		printf("5");
	}
	else if (20 <= mano[c3] && mano[c3] <= 23) {
		valmano[c3] = 6;
		printf("6");
	}
	else if (24 <= mano[c3] && mano[c3] <= 27) {
		valmano[c3] = 7;
		printf("7");
	}
	else if (28 <= mano[c3] && mano[c3] <= 31) {
		valmano[c3] = 8;
		printf("8");
	}
	else if (32 <= mano[c3] && mano[c3] <= 35) {
		valmano[c3] = 9;
		printf("9");
	}
	else if (36 <= mano[c3] && mano[c3] <= 51) {
		valmano[c3] = 10;
		if (36 <= mano[c3] && mano[c3] <= 39)
			printf("10");
		else if (40 <= mano[c3] && mano[c3] <= 43)
			printf("J");
		else if (44 <= mano[c3] && mano[c3] <= 47)
			printf("Q");
		else if (48 <= mano[c3] && mano[c3] <= 51)
			printf("K");
	}



}

//revaloriza el valor del As de 11 a 1. ALBERTO
int reval_as(int suma, int c3, int valmano[], int mano[], int* pvalmano) {

	do {
		if (0 <= mano[c3] && mano[c3] <= 3) {
			valmano[c3] = 1;
			c3--;
			suma = suma - 10;
		}
		else {
			c3--;
		}
	} while (suma > 21 && c3 == 0);

	return suma;
}

//asocia su valor aleatorio con el correspondiente palo de la carta: corazones, diamantes, treboles y picas. IGNACIO
void palo_cartas(int mano[], int c3) {

	int palo;

	palo = mano[c3] % 4;

	if (palo == 0)
		printf(" de \3 \n");
	else if (palo == 1)
		printf(" de \4 \n");
	else if (palo == 2)
		printf(" de \5 \n");
	else if (palo == 3)
		printf(" de \6 \n");

}

//Realiza la funcion de apostar las fichas. JESUS
int Apuesta(INFO u1[], int i) { 
	do {
		printf("Realice su apuesta (50 fichas max)(fichas actuales: %d): ", u1[i].fichas);
		scanf_s("%d", &u1[i].f_ap);
		if (u1[i].f_ap > 50)
			printf("\n\n\tCantidad de fichas excedida.\n\n");
		else if (u1[i].f_ap <= 0)
			printf("\n\n\tNo puedes apostar esas fichas, no seas tramposo.\n\n");
		else if (u1[i].f_ap > u1[i].fichas)
			printf("\n\n\tNo puedes apostar mas fichas de las que tienes.\n\n");
		else
			printf("\n\n\tFichas apostadas: %d", u1[i].f_ap);
	} while (u1[i].f_ap > 50 || u1[i].f_ap <= 0 || u1[i].f_ap > u1[i].fichas);

	return (u1[i].f_ap);
}
//JESUS
int Reparto_fichas(int u1blackjack, int u1sum, int crupiersum, int crupierblackjack, int f_ap, int fichas, int u1charlie, int crupiercharlie) {

	if (u1blackjack < crupierblackjack) {


		printf("\n\n\t\t---HA GANADO EL CRUPIER. MALA SUERTE---");
		printf("\n\t\t\t HAS PERDIDO %d FICHAS", f_ap);

		fichas = fichas - f_ap;
	}
	else if (u1blackjack == crupierblackjack) {
		if (u1blackjack == 1 && crupierblackjack == 1) {
			printf("\n\n\t\t---EMPATE. SE TE DEVUELVEN TUS FICHAS---");
		}
		else {
			if (u1charlie < crupiercharlie) {
				printf("\n\n\t\t---HA GANADO EL CRUPIER. MALA SUERTE---");
				printf("\n\t\t\t HAS PERDIDO %d FICHAS", f_ap);

				fichas = fichas - f_ap;
			}
			else if (u1charlie == crupiercharlie) {
				if (u1charlie == 1 && crupiercharlie == 1) {
					printf("\n\n\t\t---EMPATE. SE TE DEVUELVEN TUS FICHAS---");
				}
				else {
					if (u1sum < crupiersum) {
						printf("\n\n\t\t---HA GANADO EL CRUPIER. MALA SUERTE---");
						printf("\n\t\t\t HAS PERDIDO %d FICHAS", f_ap);

						fichas = fichas - f_ap;
					}
					else if (u1sum == crupiersum) {
						printf("\n\n\t\t---EMPATE. SE TE DEVUELVEN TUS FICHAS---");
					}
					else if (u1sum > crupiersum) {
						printf("\n\n\t\t---HAS GANADO. AHI TIENES TUS FICHAS---");
						printf("\n\t\t\t HAS GANADO %d FICHAS", f_ap);
						fichas = fichas + f_ap;
					}
				}
			}
			else if (u1charlie > crupiercharlie) {
				printf("\n\n\t\t---HAS GANADO. AHI TIENES TUS FICHAS---");
				printf("\n\t\t\t HAS GANADO %d FICHAS", f_ap);
				fichas = fichas + f_ap;
			}
		}
	}
	else if (u1blackjack > crupierblackjack) {
		f_ap = (float)(f_ap * 1.5);
		printf("\n\n\t\t---HAS GANADO. AHI TIENES TUS FICHAS---");
		printf("\n\t\t\t HAS GANADO %d FICHAS", f_ap);
		fichas = fichas + f_ap;
	}
	return fichas;
}
//JESUS
void reparto_cartas(int mano[], int c3, int repeticiones[]) {
	int i;

	mano[c3] = rand() % 52;
	for (i = 0; i < 52; i++) {
		if (i == mano[c3]) {
			repeticiones[i]++;
			if (repeticiones[i] > NUM_BARAJAS) {
				do {
					repeticiones[i]--;
					mano[c3] = rand() % 52;

				} while (repeticiones[i] > NUM_BARAJAS);
				i = -1;
			}
		}
	}
}