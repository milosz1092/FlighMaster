// [USTAWIENIA GRY]
class Game {
public:
	static float PLAYER_DEFAULT_SPEED;
	static int PLAYER_DEFAULT_STARS;
	static int PLAYER_MAX_STARS;
	static int PLAYER_SKIP_STARS;
	static float AIRCRAFT_DEFAULT_SPEED;
	static float AIRCRAFT_MAX_SPEED;
	static int AIRCRAFT_MAX_FUEL;
	static float AIRCRAFT_TURB_DEFAULT_SPEED;
	static float AIRCRAFT_TURB_MAX_ANGLE;
	static float TOR_SKIP_DEFAULT_POS;
	static int BACKGROUND_CHANGES_INTERVAL;
	static int FUEL_CHANCE_INTERVAL;
};
float	Game::PLAYER_DEFAULT_SPEED			= .05;
int		Game::PLAYER_DEFAULT_STARS			= 0;
int		Game::PLAYER_MAX_STARS				= 5;
int		Game::PLAYER_SKIP_STARS				= 20;
float	Game::AIRCRAFT_DEFAULT_SPEED		= .05;
float	Game::AIRCRAFT_MAX_SPEED			= .415;
int		Game::AIRCRAFT_MAX_FUEL				= 4;
float	Game::AIRCRAFT_TURB_DEFAULT_SPEED	= .05;
float	Game::AIRCRAFT_TURB_MAX_ANGLE		= 3.7;
float	Game::TOR_SKIP_DEFAULT_POS			= 20;
int		Game::BACKGROUND_CHANGES_INTERVAL	= 25;
int		Game::FUEL_CHANCE_INTERVAL			= 30;

// Deklaracje funkcji u¿ywanych jako obs³uga zdarzeñ GLUT-a.
void OnRender();
void OnReshape(int, int);
void OnKeyPress(unsigned char, int, int);
void OnKeyDown(unsigned char, int, int);
void OnKeyUp(unsigned char, int, int);
void OnTimer(int);

// Uniwersalna struktura reprezentuj¹ca trójwymiarowy wektor.
// Mo¿e s³u¿yæ do przechowywania pozycji, wektora, itp. Wskazane jest rozszerzenie
// tej struktury o metody i operatory pozwalaj¹ce na wykonywanie operacji na 
// wektorach - to na pewno siê przyda.
struct vec3 {
	float x, y, z;
};

struct vec4 {
	float x, y, z, c;
};

// Struktura pozwalaj¹ca na przechowanie aktualnego stanu kamery.
struct PlayerStruct {
	vec3 pos; // pozycja kamery
	vec3 dir; // kierunek patrzenia (wektor jednostkowy)
	float speed; // mno¿nik zmian pozycji - "szybkoœæ" ruchów kamery
	int score;
	bool game;
	bool get_record;
	int stars;
};

struct PlaneStruct {
	float speed; // mno¿nik zmian pozycji - "szybkoœæ" ruchów kamery
	int fuel;
	float turb;
	float turb_max;
	float maxSpeed;
	float turbSpeed;
	bool turb_flag;
	bool flash_flag;
	bool crash;
};

struct TorusStruct {
	float posZ;
	float posY;
	float skipZ;
};

// Zmianna przechowuj¹ca aktualny stan kamery.
PlayerStruct player;
PlaneStruct plane;
TorusStruct tor;
