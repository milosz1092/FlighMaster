#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "fm.h"
#include "Bitmap.h"
#include <irrKlang.h>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

/* .:: DEKLARACJA ZMIENNYCH DO OBSLUGI PLIKOW ::. */
    fstream plikIn;
    fstream plikOut;
    fstream plikRes;

/* .:: SILNIK DZWIEKU ::. */
	// [URUCHOMIENIE SILNIKA]
		irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	// [DEFINICJA PLIKOW]
		irrklang::ISound* crash;
		irrklang::ISound* pullUp;
		irrklang::ISound* engineSound;
		irrklang::ISoundSource* backSound = engine->addSoundSourceFromFile("sounds/background.WAV");
		irrklang::ISoundSource* scoreSound = engine->addSoundSourceFromFile("sounds/score.WAV");
		irrklang::ISoundSource* starSound = engine->addSoundSourceFromFile("sounds/star.WAV");
		irrklang::ISoundSource* missSound = engine->addSoundSourceFromFile("sounds/miss.WAV");
		irrklang::ISoundSource* fuelSound = engine->addSoundSourceFromFile("sounds/fuel.WAV");
		irrklang::ISoundSource* nextbgSound = engine->addSoundSourceFromFile("sounds/next_bg.WAV");
		irrklang::ISoundSource* recordSound = engine->addSoundSourceFromFile("sounds/record.WAV");
		irrklang::ISoundSource* startGame = engine->addSoundSourceFromFile("sounds/countdown.WAV");

/* .:: DEFINICJA KOLOROW ::. */
	static float colorDim[] = {0.01f,0.01f,0.01f,1.f};
	static float colorWhite[] = {0.9f,0.9f,0.9f,1.0f};
	static float colorGreen[] = {0.01f,0.7f,0.01f,1.0f};
	static float colorGray[] = {0.4f,0.4f,0.4f,1.0f};
	static float colorDarkGray[] = {0.1f,0.1f,0.1f,1.0f};
	static float colorRed[] = {0.7f,0.01f,0.01f,1.0f};
	static float colorOrange[] = {1.000, 0.549, 0.000};
	static float colorOrangeRed[] = {1.000, 0.271, 0.000};
	static float colorYellow[] = {1.000, 1.000, 0.000};

/* .:: ZMIENNE POMOCNICZE ::. */
	float fuel_bar = (float)Game::AIRCRAFT_MAX_FUEL;
	long int frame = 0;
	long int torCounter = 0;
	float stop = 0.0001;
	float rot = 0.2;
	int rekord = 0;
	float z = -10;
	float los;
	int mult_skip_star = 1;
	int star_vPos = 0;
	int bg_switch = 1;
	int act_bg = 0;
	int rem_point = 0;
	float rem_star_zPos = 0;
	/* flagi */
	bool countdown_block = 0;
	bool fuelChance = 0;
	bool starChance = 0;
	bool pause_flag = 0;
	
/* .:: WSKAZNIKI DO OBIEKTOW Z PLIKU ::. */
	GLuint aircraft;
	GLuint canister;
	GLuint star;

/* .:: ZMIENNE PRZECHOWUJACE IDENTYFIKATORY TEKSTUR ::. */
	GLuint plane_text;
	GLuint star_text;
	GLuint canister_text;
	GLuint bg1;
	GLuint bg2;
	GLuint bg3;
	GLuint bg4;
	GLuint bg5;
	GLuint bg6;
	GLuint bg7;
	GLuint bg8;
	GLuint bg9;
	GLuint bg10;
	GLuint bg11;
	GLuint bg12;
	GLuint bg13;
	GLuint bg14;
	GLuint bg15;
	GLuint curr_back;

//  STYLE CZCIONEK
//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13,
//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10,
//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
//GLvoid *font_style = GLUT_BITMAP_8_BY_13;

void output(float x, float y, float z, int font, const char *string) { 
	int len, i;
	glLoadIdentity();
	glTranslatef(x, y, -5.0f);
	glRasterPos3f(x, y, z);

	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}


GLubyte losowy_kolor()
{
		return rand() % 3;
}

// ustawienie parametrow zrodla swiatla, wlaczenie oswietlenia
#define SET_VEC4(v,v0,v1,v2,v3) v[0]=v0; v[1]=v1; v[2]=v2; v[3]=v3;

// loader obiektow
struct SFace {
        int v[3];
        int n[3];
        int t[3];
};
 
GLuint LoadObj(char * file) {
    FILE * fp = fopen(file, "r");
 
    if (fp == NULL) {
            printf("ERROR: Cannot read model file \"%s\".\n", file);
            return -1;
    }
 
    std::vector<vec3> * v = new std::vector<vec3>();
    std::vector<vec3> * n = new std::vector<vec3>();
    std::vector<vec3> * t = new std::vector<vec3>();
    std::vector<SFace> * f = new std::vector<SFace>();
 
    char buf[128];
 
    while (fgets(buf, 128, fp) != NULL) {
            if (buf[0] == 'v' && buf[1] == ' ') {
                    vec3 * vertex = new vec3();
                    sscanf(buf, "v %f %f %f", &vertex->x, &vertex->y, &vertex->z);
                    v->push_back(*vertex);
            }
            if (buf[0] == 'v' && buf[1] == 't') {
                    vec3 * vertex = new vec3();
                    sscanf(buf, "vt %f %f", &vertex->x, &vertex->y);
                    t->push_back(*vertex);
            }
            if (buf[0] == 'v' && buf[1] == 'n') {
                    vec3 * vertex = new vec3();
                    sscanf(buf, "vn %f %f %f", &vertex->x, &vertex->y, &vertex->z);
                    n->push_back(*vertex);
            }
            if (buf[0] == 'f' && buf[1] == ' ') {
                    SFace * face = new SFace();
                    sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                            &face->v[0], &face->t[0], &face->n[0],
                            &face->v[1], &face->t[1], &face->n[1],
                            &face->v[2], &face->t[2], &face->n[2]
                    );
                    f->push_back(*face);
            }
    }
 
    //fclose(fp);
 
    GLuint dlId;
    dlId = glGenLists(1);
    glNewList(dlId, GL_COMPILE);
            glBegin(GL_TRIANGLES);
            for (int i = 0; i < f->size(); ++i) {
                    for (int j = 0; j < 3; ++j) {
                            vec3 * cv = &(*v)[((*f)[i].v[j] - 1)];
                            vec3 * ct = &(*t)[((*f)[i].t[j] - 1)];
                            vec3 * cn = &(*n)[((*f)[i].n[j] - 1)];
                            glTexCoord2f(ct->x, ct->y);
                            glNormal3f(cn->x, cn->y, cn->z);
                            glVertex3f(cv->x, cv->y, cv->z);
                    }
            }
            glEnd();
    glEndList();
 
    delete v;
    delete n;
    delete t;
    delete f;
 
    return dlId;
 
}

// Funkcja odczytuj¹ca bitmapê i tworz¹ca na jej podstawie teksturê z zadanym rodzajem filtracji
GLuint LoadTexture(char * file, int magFilter, int minFilter) {
	
	// Odczytanie bitmapy
	Bitmap *tex = new Bitmap();
	if (!tex->loadBMP(file)) {
		printf("ERROR: Cannot read texture file \"%s\".\n", file);
		return -1;
	}

	// Utworzenie nowego id wolnej tekstury
	GLuint texId;
	glGenTextures(1, &texId);

	// "Bindowanie" tekstury o nowoutworzonym id
	glBindTexture(GL_TEXTURE_2D, texId);

	// Okreœlenie parametrów filtracji dla tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); // Filtracja, gdy tekstura jest powiêkszana
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); // Filtracja, gdy tekstura jest pomniejszana

	// Wys³anie tekstury do pamiêci karty graficznej zale¿nie od tego, czy chcemy korzystaæ z mipmap czy nie
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST) {
		// Automatyczne zbudowanie mipmap i wys³anie tekstury do pamiêci karty graficznej
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	else {
		// Wys³anie tekstury do pamiêci karty graficznej 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	
	// Zwolnienie pamiêci, usuniêcie bitmapy z pamiêci - bitmapa jest ju¿ w pamiêci karty graficznej
	delete tex;
	
	// Zwrócenie id tekstury
	return texId;
}

/* .:: DEFINICJA SCIEZEK DO PLIKOW W FUNKCJI LADUJACEJ TEKSTURY */
void LoadTextures() {
	plane_text = LoadTexture("backgrounds/back1.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	star_text = LoadTexture("backgrounds/stars.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	canister_text = LoadTexture("backgrounds/canister.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg1 = LoadTexture("backgrounds/back1.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg2 = LoadTexture("backgrounds/back2.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg3 = LoadTexture("backgrounds/back3.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg4 = LoadTexture("backgrounds/back4.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg5 = LoadTexture("backgrounds/back5.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg6 = LoadTexture("backgrounds/back6.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg7 = LoadTexture("backgrounds/back7.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg8 = LoadTexture("backgrounds/back8.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg9 = LoadTexture("backgrounds/back9.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg10 = LoadTexture("backgrounds/back10.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg11 = LoadTexture("backgrounds/back11.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg12 = LoadTexture("backgrounds/back12.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg13 = LoadTexture("backgrounds/back13.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg14 = LoadTexture("backgrounds/back14.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bg15 = LoadTexture("backgrounds/back15.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
}

int main(int argc, char* argv[])
{
	/* .:: DEFINICJA ZMIENNYCH POCZATKOWYCH ::. */
		// [GRACZ]
			/* predkosc pionowa */
			player.speed = Game::PLAYER_DEFAULT_SPEED;
			/* definicja licznika gwiazdek */
			player.stars = Game::PLAYER_DEFAULT_STARS;
			/* pozycja startowa */
			player.pos.x = 0.0f;
			player.pos.y = 2.00f;
			player.pos.z = -10.0f;
			/* ustawienie kamery */
			player.dir.x = 0.0f;
			player.dir.y = 0.0f;
			player.dir.z = 1.0f;
			/* flagi */
			player.game = 1;
			player.get_record = 0;

		// [SAMOLOT]
			/* predkosc lotu */
			plane.maxSpeed = Game::AIRCRAFT_MAX_SPEED;
			plane.speed = Game::AIRCRAFT_DEFAULT_SPEED;
			/* zbiornik paliwa */
			plane.fuel = Game::AIRCRAFT_MAX_FUEL;
			/* ustawienia turbulencji */
			plane.turbSpeed = Game::AIRCRAFT_TURB_DEFAULT_SPEED;
			plane.turb_max = Game::AIRCRAFT_TURB_MAX_ANGLE;
			plane.turb_flag = 1;
			/* flagi */
			plane.flash_flag = 0;
			plane.crash = 0;

		// [OBRECZE]
			/* odleglosc miedzy obreczami */
			tor.skipZ = Game::TOR_SKIP_DEFAULT_POS;
			/* pozycja pierwszej obreczy */
			tor.posZ = Game::TOR_SKIP_DEFAULT_POS;
			tor.posY = 2.5;
			
	/* .:: USTAWIENIA POZIOMOW GLOSNOSCI ::. */
		backSound->setDefaultVolume(0.02f);
		scoreSound->setDefaultVolume(0.015f);
		missSound->setDefaultVolume(0.01f);
		starSound->setDefaultVolume(0.3f);
		fuelSound->setDefaultVolume(0.02f);
		nextbgSound->setDefaultVolume(0.1f);
		recordSound->setDefaultVolume(0.05f);
		startGame->setDefaultVolume(0.06f);

	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("GAME FlightMaster");

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutKeyboardUpFunc(OnKeyUp);

	glutTimerFunc(20, OnTimer, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	float vec[4];
	SET_VEC4( vec, 0.0f, player.pos.y+3, player.pos.z-4.0f, 1.0f );
	glLightfv(GL_LIGHT1, GL_POSITION, vec );
	SET_VEC4( vec, 0.0f, 0.0f, 4.0f, 1.0f );
	glLightfv(GL_LIGHT0, GL_POSITION, vec );
	
	SET_VEC4( vec, 1.0f, 1.0f, 1.0f, 1.0f );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, vec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, vec);
	glLightfv(GL_LIGHT0, GL_EMISSION, vec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, vec); 

	glLightfv(GL_LIGHT1, GL_DIFFUSE, vec);
	glLightfv(GL_LIGHT1, GL_AMBIENT, vec);
	glLightfv(GL_LIGHT1, GL_EMISSION, vec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, vec); 

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1); 
	glEnable(GL_LIGHTING);


	// wlaczenie cieniowania Gouraud'a (smooth shading)
	glShadeModel( GL_SMOOTH );

	// ustawienie koloru tla
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );

	/* .:: LADOWANIE TEKSTUR ::. */
	LoadTextures();

	/* .:: LADOWANIE OBIEKTOW Z PLIKU ::. */
		aircraft = LoadObj("plane.obj");
		canister = LoadObj("canister.obj");
		star = LoadObj("star.obj");

	glutMainLoop();
	engine->drop();
	return 0;
}

// Tablica przechowuj¹ca stan klawiszy w formie flag, indeksowana wg kodów ASCII.
bool keystate[256];

// Obs³uga zdarzenia, gdy zostanie wciœniêty klawisz - zdarzenie nieoodporne na repetycjê klawiszy.
void OnKeyPress(unsigned char key, int x, int y) {
	//printf("KeyPress: %c\n", key);
	if (!keystate[key]) {
		keystate[key] = true;
		OnKeyDown(key, x, y); // Emulacja zdarzenia zwi¹zanego z pojedynczym wciœniêciem klawisza
	}
}




// Obs³uga naszego w³asnego zdarzenia, gdy zostanie po raz pierwszy wciœniêty klawisz - zdarzenie odporne na repetycjê.
void OnKeyDown(unsigned char key, int x, int y) {
	//printf("KeyDown: %c\n", key);
	if (key == 27) { // ESC - wyjœcie
		glutLeaveMainLoop();
	}
}

// Obs³uga zdarzenia puszczenia klawisza.
void OnKeyUp(unsigned char key, int x, int y) {
	//printf("KeyUp: %c\n", key);
	keystate[key] = false;
}


// Aktualizacja stanu gry - wywo³ywana za poœrednictwem zdarzenia-timera.
void OnTimer(int id) {

	if (pause_flag == 0) {
		if (keystate['w'] || keystate['W']) {
			if(player.pos.y < 6.5) {
				player.pos.y += 1 * player.speed;
				//printf("Wysokosc: %f \n", player.pos.y);
			}
		}
		if (keystate['s'] || keystate['S']) {
			if(player.pos.y > 1.5) {
				player.pos.y -= 1 * player.speed;
				//printf("Wysokosc: %f \n", player.pos.y);
			}
		}
	}

	if(player.game == 1) {
		if (keystate['p'] || keystate['P']) {
			pause_flag = 1;
			engineSound->setVolume(0.00);
		}
		if (keystate['l'] || keystate['L']) {
			pause_flag = 0;
			engineSound->setVolume(0.04);
		}
	}

	glutPostRedisplay();
	// Chcemy, by ta funkcja wywolala sie ponownie za 20ms.
	glutTimerFunc(20, OnTimer, 0);
}

void OnRender() {
	frame++;
	if (frame == 1) {
		engine->play2D(backSound, true);
		pullUp = engine->play2D("sounds/pull_up.WAV", true, true, true);
		engineSound = engine->play2D("sounds/engines.WAV", true, false, true); 
		engineSound->setVolume(0.04f);
		pullUp->setVolume(0.02f);
		crash->setVolume(0.05f);
	}

	if (torCounter == 0) {
		plane.turb = 0.01;
		plane.turb_flag = 1;
	}

	//printf("Gdzie: %f \n", player.pos.z);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Ustawienie kamery na podstawie jej stanu przechowywanego w zmiennej player.
	if (pause_flag == 0)
		gluLookAt(
			player.pos.x, player.pos.y, player.pos.z+=plane.speed, // Pozycja kamery
			player.pos.x + player.dir.x, player.pos.y + player.dir.y, player.pos.z + player.dir.z, // Punkt na ktory patrzy kamera (pozycja + kierunek)
			0.0f, 1.0f, 0.0f // Wektor wyznaczajacy pion
		);
	else
		gluLookAt(
			player.pos.x, player.pos.y, player.pos.z, // Pozycja kamery
			player.pos.x + player.dir.x, player.pos.y + player.dir.y, player.pos.z + player.dir.z, // Punkt na ktory patrzy kamera (pozycja + kierunek)
			0.0f, 1.0f, 0.0f
		);

	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
 


	// NAPISY
	// pokazywanie konca gry
	if (player.game == 0) {
		string s = to_string(player.score);
		char const *pchar = s.c_str();

		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.73f, 4.13f, -20, 1, "KONIEC GRY");
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.7f, 4.1f, -20, 1, "KONIEC GRY");
		glPopMatrix();

		if (player.get_record == 1) {
			glPushMatrix();
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
				glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
				output(2.23f, 4.13f, -20, 1, "+ REKORD!");
			glPopMatrix();
			glPushMatrix();
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
				glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
				output(2.2f, 4.1f, -20, 1, "+ REKORD!");
			glPopMatrix();
		}

		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glColor3f(1,1,1);
			output(0.53f, 3.53f, -20, 1, pchar);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorGreen );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glColor3f(1,1,1);
			output(0.5f, 3.5f, -20, 1, pchar);
		glPopMatrix();


		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.73f, 3.53f, -20, 1, "Wynik:");
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.7f, 3.5f, -20, 1, "Wynik:");
		glPopMatrix();
	} 

	if (plane.crash == 0) {
		// pokazywanie poziomu paliwa
		glPushMatrix();
		switch(plane.fuel) {
			case 4:
				glMaterialfv( GL_FRONT, GL_DIFFUSE,  colorGreen);
			break;
			case 3:
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorYellow);
			break;
			case 2:
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorOrange);
			break;
			case 1:
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorOrangeRed);
			break;
			default:
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorRed);
		}
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glTranslatef(7.78, player.pos.y-5.5, player.pos.z + 10);
			glRotatef(-90, 1, 0, 0);
			glRotatef(-90, 0, 1, 0);
			if (fuel_bar > plane.fuel)
				fuel_bar = fuel_bar - 0.01;
			if (fuel_bar < plane.fuel)
				fuel_bar = fuel_bar + 0.01;
			glutSolidCylinder(0.1, fuel_bar*3, 30,1);
		glPopMatrix();
	}

	if (player.game == 1) {
		// punkty
		string s = to_string(player.score);
		char const *pchar = s.c_str(); 
		
		// pokazywanie licznika punktow
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-7.6f, -6.5f, -20, 1, pchar);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glColor3f(1,1,1);
			output(-8.8f, -6.5f, -20, 1, "Pkt: ");
		glPopMatrix();


		// Informacja o Esc
		/*glPushMatrix();
			glColor3f(1,1,1);
			output(2.0f, -6.5f, -20, 1, "Esc - koniec    P - wstrzymaj");
		glPopMatrix();*/
	}

	// pokazywanie czy jest rekord
	if (player.get_record == 1 && player.game == 1) {
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorRed );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.0f, 6.0f, -20, 1, "REKORD");
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-1.03f, 6.03f, -20, 1, "REKORD");
		glPopMatrix();

	}

	// zatrzymanie gry
	if (pause_flag == 1) {
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-2.53f, 4.13f, -20, 1, "GRA ZATRZYMANA");
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-2.5f, 4.1f, -20, 1, "GRA ZATRZYMANA");
		glPopMatrix();

		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorDarkGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-2.63f, 3.53f, -20, 1, "L - uruchom ponownie");
		glPopMatrix();
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			output(-2.6f, 3.5f, -20, 1, "L - uruchom ponownie");
		glPopMatrix();
	}

	// W³¹czamy teksturowanie
	glEnable(GL_TEXTURE_2D);

	// Ustawienie sposobu teksturowania - GL_MODULATE sprawia, ¿e œwiat³o ma wp³yw na teksturê; GL_DECAL i GL_REPLACE rysuj¹ teksturê tak jak jest
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Ustawienie materia³u
	float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };



	// Wybór tekstury krajobrazu korzystaj¹c z jej id
	if (player.score == 0 && curr_back != bg1) {
		curr_back = bg1;
		rem_point = player.score;
		bg_switch = 2;
		rem_point = player.score;
		act_bg = 1;
	}
	else if (torCounter % Game::BACKGROUND_CHANGES_INTERVAL == 0) {
		switch(bg_switch) {
			case 1:
				if(act_bg != 1 && rem_point != player.score) {
					curr_back = bg1;
					bg_switch = 2;
					act_bg = 1;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 2:
				if(act_bg != 2 && rem_point != player.score) {
					curr_back = bg2;
					bg_switch = 3;
					act_bg = 2;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 3:
				if(act_bg != 3 && rem_point != player.score) {
					curr_back = bg3;
					bg_switch = 4;
					act_bg = 3;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 4:
				if(act_bg != 4 && rem_point != player.score) {
					curr_back = bg4;
					bg_switch = 5;
					act_bg = 4;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 5:
				if(act_bg != 5 && rem_point != player.score) {
					curr_back = bg5;
					bg_switch = 6;
					act_bg = 5;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 6:
				if(act_bg != 6 && rem_point != player.score) {
					curr_back = bg6;
					bg_switch = 7;
					act_bg = 6;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 7:
				if(act_bg != 7 && rem_point != player.score) {
					curr_back = bg7;
					bg_switch = 8;
					act_bg = 7;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 8:
				if(act_bg != 8 && rem_point != player.score) {
					curr_back = bg8;
					bg_switch = 9;
					act_bg = 8;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 9:
				if(act_bg != 9 && rem_point != player.score) {
					curr_back = bg9;
					bg_switch = 10;
					act_bg = 9;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 10:
				if(act_bg != 10 && rem_point != player.score) {
					curr_back = bg10;
					bg_switch = 11;
					act_bg = 10;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 11:
				if(act_bg != 11 && rem_point != player.score) {
					curr_back = bg11;
					bg_switch = 12;
					act_bg = 11;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 12:
				if(act_bg != 12 && rem_point != player.score) {
					curr_back = bg12;
					bg_switch = 13;
					act_bg = 12;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 13:
				if(act_bg != 13 && rem_point != player.score) {
					curr_back = bg13;
					bg_switch = 14;
					act_bg = 13;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 14:
				if(act_bg != 14 && rem_point != player.score) {
					curr_back = bg14;
					bg_switch = 15;
					act_bg = 14;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
			case 15:
				if(act_bg != 15 && rem_point != player.score) {
					curr_back = bg15;
					bg_switch = 1;
					act_bg = 15;
					rem_point = player.score;
					engine->play2D(nextbgSound);
				}
			break;
		}
	}
	glBindTexture(GL_TEXTURE_2D, curr_back);

	glPushMatrix();
		// Ustawienie materia³u
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);
		glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );

		glTranslatef(player.pos.x, 3.8f, player.pos.z + 30 );
		glScalef( -24, 19.6f, 0.0001 );
				glBegin(GL_QUADS);
					glTexCoord2f( 0.0f,  0.0f);
					glNormal3f( 0.0f,  0.0f, -1.0f);
					glVertex3f(-1.0f, -1.0f,  0.0f);
			
					glTexCoord2f( 0.0f, 1.0f);
					glNormal3f( 0.0f,  0.0f, -1.0f);
					glVertex3f(-1.0f,  1.0f,  0.0f);
			
					glTexCoord2f(1.0f, 1.0f);
					glNormal3f( 0.0f,  0.0f, -1.0f);
					glVertex3f( 1.0f,  1.0f,  0.0f);
			
					glTexCoord2f(1.0f,  0.0f);
					glNormal3f( 0.0f,  0.0f, -1.0f);
					glVertex3f( 1.0f, -1.0f,  0.0f);
				glEnd();
	glPopMatrix();

	// Wybór tekstury korzystaj¹c z jej id
	glBindTexture(GL_TEXTURE_2D, plane_text);
	// RYSOWANIE SAMOLOTU

	glPushMatrix();
		glMaterialfv( GL_FRONT, GL_DIFFUSE, colorGray );
		glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );	

		if (player.score == 0 && (tor.posZ - player.pos.z > 28) && player.game == 1) { // MIGOTANIE NA POCZATKU GRY
			if (plane.flash_flag == 0) {
				glTranslatef(player.pos.x, -50, player.pos.z + 0.05 );
				plane.flash_flag = 1;
			}
			else if (plane.flash_flag == 1) {
				glTranslatef(player.pos.x, player.pos.y-0.001, player.pos.z + 0.05 );
				plane.flash_flag = 0;
			}
		}
		else {
				plane.flash_flag = 1;
				if(plane.fuel == 0) {
					glTranslatef(player.pos.x, (player.pos.y-0.001) - stop, (player.pos.z + 0.05) - stop );
					stop += 0.0001;
				}
				else
					glTranslatef(player.pos.x, player.pos.y-0.001, player.pos.z + 0.05 );
		}

		glRotatef(180, 0, 1, .0f);
		glRotatef(-90, 1, 0, .0f);

		// Turbulencje
		if (pause_flag == 0) {
			if (plane.turb > 0) {
				if (plane.turb < plane.turb_max) {
					if (plane.turb_flag == 1)
						plane.turb += plane.turbSpeed;
					else if (plane.turb_flag == 0)
						plane.turb -= plane.turbSpeed;
				}
				else if (plane.turb >= plane.turb_max) {
					if (plane.turb_flag == 0)
						plane.turb_flag = 1;
					else if (plane.turb_flag == 1)
						plane.turb_flag = 0;

					if (plane.turb_flag == 1)
						plane.turb += plane.turbSpeed;
					else if (plane.turb_flag == 0)
						plane.turb -= plane.turbSpeed;
				}
			}
			if (plane.turb <= 0) {
				if (plane.turb > -plane.turb_max) {
					if (plane.turb_flag == 1)
						plane.turb += plane.turbSpeed;
					else if (plane.turb_flag == 0)
						plane.turb -= plane.turbSpeed;
				}
				else if (plane.turb <= -plane.turb_max) {
					if (plane.turb_flag == 0)
						plane.turb_flag = 1;
					else if (plane.turb_flag == 1)
						plane.turb_flag = 0;

					if (plane.turb_flag == 1)
						plane.turb += plane.turbSpeed;
					else if (plane.turb_flag == 0)
						plane.turb -= plane.turbSpeed;
				}
			}
			glRotatef(plane.turb, 0, 1, 0);
		}

		if(plane.fuel == 0) {
			glRotatef(5+rot*1.5, -.9f, .0f, .0f);
			glRotatef(rot, 0, 1, 1);
			rot += 0.2;
		}
		glScalef( 0.002f, 0.001f, 0.002f );
		if (pause_flag == 0) {
			if ((keystate['s'] || keystate['S']) && player.pos.y > 1.5) {
				glRotatef(2, -.9f, .0f, .0f);
			}
			if ((keystate['w'] || keystate['W']) && player.pos.y < 6.5) {
				glRotatef(2, .9f, .0f, .0f);
			}
		}
		glCallList(aircraft);
	glPopMatrix();

	// RYSOWANIE KANISTRA
	if(player.score != 0 && torCounter % Game::FUEL_CHANCE_INTERVAL == 0 && plane.fuel < Game::AIRCRAFT_MAX_FUEL) {
		glBindTexture(GL_TEXTURE_2D, canister_text);
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorGray );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );	
			glTranslatef(0, tor.posY-0.2, tor.posZ);
			glRotatef(180, 0, 1, 0);
			glRotatef(frame, 0, 1, 0);
			glScalef(0.4, 0.4, 0.4);
			glCallList(canister);
		glPopMatrix();
		fuelChance = 1;
	}
	else {
		fuelChance = 0;
	}
	
	// WSKAZNIK GWIAZDEK
	if (player.stars > 0) {
		for (int i = 0; i < player.stars; i++) {
			glBindTexture(GL_TEXTURE_2D, star_text);
			glPushMatrix();
				glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
				glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
				glTranslatef((player.pos.x+0.355) - (i * 0.045), player.pos.y+0.25, player.pos.z+0.5);
				glRotatef(90, 0, 0, 1);
				glRotatef(90, 1, 0, 0);
				glRotatef(20, 1, 0, 0);
				glRotatef(plane.turb*2, 0, 1, 0);
				glScalef(0.0006, 0.0006, 0.0006);
				glCallList(star);
			glPopMatrix();
		}
	}

	// RYSOWANIE GWIAZDKI
	if (player.stars > 0)
			if (player.stars == 1)
				mult_skip_star = 5;
			else if (player.stars == 2)
				mult_skip_star = 10;
			else if (player.stars == 3)
				mult_skip_star = 15;
			else if (player.stars == 4)
				mult_skip_star = 25;
	else
		mult_skip_star = 1;

	//printf("skip: %d\n", Game::PLAYER_SKIP_STARS * mult_skip_star);
		if(player.score != 0 && (torCounter % (Game::PLAYER_SKIP_STARS * mult_skip_star)) == 0 && player.stars < Game::PLAYER_MAX_STARS) {
		//printf("rem_star_zPos: %
		if(rem_star_zPos != tor.posZ) {
			while(1) {
				los = rand() % (6 - 2 + 1) + 2;
				if (tor.posY != los) {
					star_vPos = los;
					rem_star_zPos = tor.posZ;
					break;
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, star_text);
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glTranslatef(0, star_vPos, tor.posZ);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			glRotatef(frame, 1, 0, 0);
			glScalef(0.015, 0.015, 0.015);
			glCallList(star);
		glPopMatrix();
		starChance = 1;
	}
	else {
		starChance = 0;
	}

	// Wy³¹czenie teksturowania - geometria renderowana od tego miejsca nie bêdzie ju¿ oteksturowana
	glDisable(GL_TEXTURE_2D);


	// Odliczanie rozpoczynajace gre
	if(player.score == 0 && (tor.posZ - player.pos.z <= 15) && countdown_block == 0) {
		engine->play2D(startGame);
		countdown_block = 1;
	}

	// RYSOWANIE KOLEJNYCH OBRECZY
	if (plane.fuel > 0) {
		glPushMatrix();
			glMaterialfv( GL_FRONT, GL_DIFFUSE, colorWhite );
			glMaterialfv( GL_FRONT, GL_AMBIENT, colorDim );
			glTranslatef(0, tor.posY,  tor.posZ);
			glutSolidTorus(.05, .4, 4, 20);
		glPopMatrix();

		if (player.pos.z >= tor.posZ) {
			// sprawdzamy kolizje - dodajemy punkty lub odejmujemy paliwo
			if (player.pos.y < tor.posY + 0.3 && player.pos.y > tor.posY - 0.3) {
				if (player.stars == 0)
					player.score++;
				else
					player.score = player.score + (1 * (player.stars + 1));

				if (fuelChance == 1) {
					plane.fuel++;
					engine->play2D(fuelSound);
				}
				torCounter++;
				engine->play2D(scoreSound);

				// ZAPISANIE JE¯ELI JEST REKORD
				plikIn.open("rekord.txt", ios::in);
				if(plikIn.good() == true) {
						string wiersz;

						while(!plikIn.eof()) {
								plikIn >> wiersz;
							// zamiana ze string na int
							istringstream iss(wiersz);
							iss >> rekord;
							//printf("REKORD: %d\n", rekord);
					 
						}

						plikIn.close();  
				}
				if (rekord < player.score) { // zapisanie nowego rekordu i akcja na ekranie
					// czyszczenie pliku
					plikRes.open("rekord.txt", ios::out | ios::trunc);
					plikRes.close();

					// zapisanie nowego rekordu
					plikOut.open("rekord.txt", ios::out | ios::app);
					if(plikOut.good() == true) {
						plikOut << player.score;
						plikOut.close();     
					}

					// akcja graficzna i dzwiekowa przy rekordzie
					if (player.get_record == 0) {
						player.get_record = 1;

						engine->play2D(recordSound);
					}
				}
			}
			else if (plane.fuel > 0) {
				// ZDOBYWANIE GWIAZDKI
				if (starChance == 1 && player.pos.y < star_vPos + 0.3 && player.pos.y > star_vPos - 0.3) {
					player.stars++;
					engine->play2D(starSound);
					//printf("gwiazdki: %d\n", player.stars);
				}
				plane.fuel--;
				torCounter++;
				engine->play2D(missSound);
			}

			//printf("SCORE: %d\n", player.score);
			//printf("FUEL: %d\n", plane.fuel);

			// zwiekszamy predkosc lotu i ruchu

			if (plane.speed < plane.maxSpeed) {
				//printf("speed: %f\n", plane.speed);

				if (plane.speed > 0.04 && plane.speed < 0.20) {
					plane.speed += .05;
					player.speed += .003;
				}
				else if (plane.speed > 0.19 && plane.speed < 0.30) {
					plane.speed += .006;
					player.speed += .002;
				}
				else if (plane.speed > 0.29 && plane.speed < 0.35) {
					plane.speed += .00035;
					player.speed += .00025;
				}
				else if (plane.speed > 0.34) {
					plane.speed += .00020;
					player.speed += .00010;
				}


			}

			// Pozycja Z nastepnej obreczy
			tor.posZ += tor.skipZ;

			// losujemy wysokosc dla nowej obreczy
			while(1) {
				los = rand() % (6 - 2 + 1) + 2;
				if (tor.posY != los) {
					tor.posY = los;
					break;
				}
			}
		}
	} else {
		// KONIEC GRY / zapisanie wyniku i nastepna gra
		
		// DZWIEKI PRZY UPADKU SAMOLOTU
		engineSound->setVolume(0.04-stop*2);
		player.game = 0;
		pullUp->setIsPaused(false);
		if (rot > 50) {
			pullUp->setIsPaused(true);
			crash = engine->play2D("sounds/crash.WAV", false, false, true);
			crash->setVolume(0.05f);
			plane.crash=1;

			// ROZPOCZECIE NASTEPNEJ GRY
			player.speed = Game::PLAYER_DEFAULT_SPEED;
			player.stars = Game::PLAYER_DEFAULT_STARS;
			rem_star_zPos = 0;
			mult_skip_star = 1;
			star_vPos = 0;
			player.score = 0;
			rem_point = 0;
			player.get_record = 0;

			plane.speed = Game::AIRCRAFT_DEFAULT_SPEED;
			plane.fuel = Game::AIRCRAFT_MAX_FUEL;
			fuel_bar = (float)Game::AIRCRAFT_MAX_FUEL;

			countdown_block = 0;
			startGame->setDefaultVolume(0.06f);
			engineSound->setVolume(0.04);

			player.game = 1;
			plane.crash = 0;
			bg_switch = 1;
			curr_back = bg2;
			act_bg = 0;
			
			tor.posY = 2.5;
			tor.posZ = player.pos.z + tor.skipZ + 10;
			torCounter = 0;
			fuelChance = 0;
			starChance = 0;

			stop = 0.0001;
			rot = 0.2;
		}
	}
	glutSwapBuffers();
	
	glFlush();
}

void OnReshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	gluPerspective(60.0f, (float) width / height, .01f, 100.0f);
}

