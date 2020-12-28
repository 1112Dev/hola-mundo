#include <iostream>
#include <string>
#include <cstring>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <Windows.h>
#include <sstream>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

#define width 1920
#define height 1280

int load();
int jugar();
int menu();
int ancho = 900;
int alto = 506;

ALLEGRO_DISPLAY* ventana;
ALLEGRO_FONT* arial;
ALLEGRO_TIMER* segundoTimer;
ALLEGRO_TIMER* fps;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_KEYBOARD_STATE teclado;

int main() {


	if (!al_init()) {
		al_show_native_message_box(NULL, "ERROR CRITICO", "ERROR: 404", "No se pudo cargar correctamente la libreria alelgro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	al_set_new_display_flags(ALLEGRO_NOFRAME);

	ventana = al_create_display(ancho, alto);
	arial = al_load_font("arial.ttf", 30, 0);

	int ancho_W = GetSystemMetrics(SM_CXSCREEN);
	int alto_W = GetSystemMetrics(SM_CYSCREEN);

	al_set_window_title(ventana, "Mi Juego");
	al_set_window_position(ventana, ancho_W / 2 - ancho / 2, alto_W / 2 - alto / 2);

	//al_show_native_message_box(NULL, "ERROR CRITICO", "ERROR: 404", "No se pudo cargar correctamente la libreria alelgro", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

	//eventos timer
	segundoTimer = al_create_timer(1.0);
	fps = al_create_timer(1.0 / 10);
	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_register_event_source(event_queue, al_get_timer_event_source(segundoTimer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(ventana));

	menu();

	al_destroy_display(ventana);
	al_destroy_timer(segundoTimer);
	al_destroy_event_queue(event_queue);
	return 0;
}



// --------------------------------------------------------JUGAR-----------------------------------------------------------------------


int jugar()
{
	al_reserve_samples(2);

	ALLEGRO_SAMPLE* pasos = NULL;
	ALLEGRO_SAMPLE* musica2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE* musicajuego = NULL;
	pasos = al_load_sample("click.wav");
	musica2 = al_load_sample("melody.wav");
	musicajuego = al_create_sample_instance(musica2);

	al_set_sample_instance_playmode(musicajuego, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(musicajuego, al_get_default_mixer());

	enum direction { W = 9, A = 3, S = 0, D = 6 };

	ALLEGRO_BITMAP* player[12];

	int y = 200;
	int x = 100;
	int movimiento = 10;

	bool bb = false;
	bool dibujo = true;
	bool activo = false;

	int state = NULL;
	int dir = S;
	int indice = 0;
	int previadir;

	al_play_sample_instance(musicajuego);

	al_start_timer(fps);
	al_start_timer(segundoTimer);

	while (true)
	{
		ALLEGRO_EVENT evento;
		al_wait_for_event(event_queue, &evento);


		if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (evento.keyboard.keycode)
			{

				case ALLEGRO_KEY_W:
				{
					dir = W;
					break;
				}
				case ALLEGRO_KEY_UP:
				{
					dir = W;
					break;
				}
				case ALLEGRO_KEY_S:
				{
					dir = S;
					break;
				}
				case ALLEGRO_KEY_DOWN:
				{
					dir = S;
					break;
				}
				case ALLEGRO_KEY_A:
				{
					dir = A;
					break;
				}
				case ALLEGRO_KEY_LEFT:
				{
					dir = A;
					break;
				}
				case ALLEGRO_KEY_D: 
				{
					dir = D;
					break;
				}
				case ALLEGRO_KEY_RIGHT:
				{
					dir = D;
					break;
				}
			default:
				break;
			}
		}
			
			if (evento.type == ALLEGRO_EVENT_TIMER)
			{
				al_get_keyboard_state(&teclado);
				activo = true;
				previadir = dir;
				if (al_key_down(&teclado, ALLEGRO_KEY_W))
					y -= movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_UP))
					y -= movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_S))
					y += movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_DOWN))
					y += movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_A))
					x -= movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_LEFT))
					x -= movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_D))
					x += movimiento;
				else if (al_key_down(&teclado, ALLEGRO_KEY_RIGHT))
					x += movimiento;
				else
					activo = false;
			
				if (activo)			//lee el cuadernito
					indice++;
				else
					indice = dir + 1;

				if (indice >= dir + 3)
					indice = dir;

				if (dir != previadir)
					indice = dir;

				dibujo = true;

			}



			for (int i = 0; i < 12; i++)
			{
				std::stringstream str;
				str << i + 1 << ".png";
				player[i] = al_load_bitmap(str.str().c_str());

			}


		if (dibujo)
		{
			al_draw_bitmap(player[indice], x, y, NULL);

			al_flip_display();
			al_clear_to_color(al_map_rgb(255, 255, 255));
		}

		


	}

	for (int i = 0; i < 12; i++)
		al_destroy_bitmap(player[i]);
	
}




// --------------------------------------------------------LOAD-----------------------------------------------------------------------



int load()
{

	while (true)
	{
		ALLEGRO_EVENT Evento;
		al_wait_for_event(event_queue, &Evento);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_circle(200, 600, 300, al_map_rgb(255, 255, 255),20);
		al_flip_display();
	}




}



// --------------------------------------------------------MENU-----------------------------------------------------------------------



int menu() {

	int segundos = 0;
	int x = -1, y = -1;
	ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR azar = al_map_rgb(255, 50, 65);

	al_reserve_samples(3);										//reservas de musica que en este caso solo estoy usando 1

	ALLEGRO_SAMPLE* teclas = NULL;
	ALLEGRO_SAMPLE* enter = NULL;
	ALLEGRO_SAMPLE* musica = NULL;
	ALLEGRO_SAMPLE_INSTANCE* musicafondo = NULL;
	teclas = al_load_sample("move.wav");
	enter = al_load_sample("click.wav");
	musica = al_load_sample("promise.wav");
	musicafondo = al_create_sample_instance(musica);
	
	al_set_sample_instance_playmode(musicafondo, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(musicafondo, al_get_default_mixer());

	ALLEGRO_BITMAP* menu_null = al_load_bitmap("silenthill1.bmp");
	ALLEGRO_BITMAP* menu_jugar = al_load_bitmap("silenthill2.bmp");
	ALLEGRO_BITMAP* menu_salir = al_load_bitmap("silenthill3.bmp");
	ALLEGRO_BITMAP* menu_exit = al_load_bitmap("silenthill4.bmp");
	//menu
	int botones[] = { 0 };


	al_play_sample(musica, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

	al_start_timer(fps);
	al_start_timer(segundoTimer);

	while (true)
	{
		ALLEGRO_EVENT Evento;
		al_wait_for_event(event_queue, &Evento);

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == segundoTimer) {
				segundos++;
			}

		}
		

		if (botones[0] == 0)
			al_draw_bitmap(menu_null, 0, 0, 0);
		else if (botones[0] == 1)
			al_draw_bitmap(menu_jugar, 0, 0, 0);
		else if (botones[0] == 2)
			al_draw_bitmap(menu_salir, 0, 0, 0);
		else
			al_draw_bitmap(menu_exit, 0, 0, 0);


		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{

			x = Evento.mouse.x;
			y = Evento.mouse.y;

			if (x >= 685 && x <= 780 && y >= 275 && y <= 290) {
				botones[0] = 1;
				if (Evento.mouse.button & 1)
					jugar();
			}
			else {
				if (x >= 685 && x <= 790 && y >= 340 && y <= 350) {
					botones[0] = 2;
					if (Evento.mouse.button & 1)
						load();
				}
				else {
					if (x >= 688 && x <= 790 && y >= 400 && y <= 415) {
						botones[0] = 3;
						if (Evento.mouse.button & 1)
							return 1;
					}
					else {
						botones[0] = 0;
					}

				}


			}

		}
		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (Evento.keyboard.keycode)
			{

			case ALLEGRO_KEY_DOWN:
			case ALLEGRO_KEY_S: {

				al_play_sample(teclas, 4.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				botones[0] += 1;

				if (botones[0] >= 4)
				{
					botones[0] = 1;
					botones[0] + 1;
				}
				cout << "presionaste abajo\n";
				break;
			}

			case ALLEGRO_KEY_UP:
			case ALLEGRO_KEY_W: {

				al_play_sample(teclas, 4.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				botones[0] -= 1;

				if (botones[0] <= 0)
				{
					botones[0] = 3;
					botones[0] - 1;
				}
				cout << "presionaste arriba\n";
				break;
			}
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_PAD_ENTER:
			{
				al_play_sample(enter, 5.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				if (botones[0] == 1)
				{
					jugar();
				}
				else if (botones[0] == 2)
				{
					load();
				}
				else if (botones[0] == 3)
				{
					return 1;
				}

			}


			default:
				break;
			}
		}

		al_flip_display();
	}
	
}