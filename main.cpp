#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

void redraw(ALLEGRO_DISPLAY *pDisplay, ALLEGRO_BITMAP* pBitmap, ALLEGRO_BITMAP* pBuffer, ALLEGRO_COLOR oBgColor, float fX, float fY) {	
	al_set_target_bitmap(pBuffer);
	al_clear_to_color(oBgColor);
	al_draw_bitmap(pBitmap, fX, fY, 0);
	
}

void redrawEnemies(ALLEGRO_DISPLAY *pDisplay, ALLEGRO_BITMAP** arrPtrEnemies, ALLEGRO_BITMAP* pBuffer, float* fX, float* fY, int iEnemies, float fSpeedMult = 0.0f) {
	int iBitmapHeight = al_get_bitmap_height(pBuffer);	
	for (int i = 0; i < iEnemies; i++) {
		fY[i] += (fSpeedMult == 0.0f) ? 0.1f : 0.1f * fSpeedMult;
		if (fY[i] > iBitmapHeight) { fY[i] -= iBitmapHeight; }
		if (fY[i] < 0) { fY[i] = al_get_bitmap_height(pBuffer); }
		al_set_target_bitmap(pBuffer);
		al_draw_bitmap(arrPtrEnemies[i], fX[i], fY[i], 0);
	}
}

int main(int argc, char** argv) {

	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();

	std::string sTitleApp = "Allegro 5 Timers and Smooth Animation";
	int gbl_iAppWidth = 800;
	int gbl_iAppHeight = 800;

	ALLEGRO_DISPLAY* pDisplay = al_create_display(gbl_iAppWidth, gbl_iAppHeight);
	int iFPS = al_get_display_refresh_rate(pDisplay);
	if (iFPS == 0) {
		iFPS = 60;
	}

	ALLEGRO_TIMER* pTimer = al_create_timer(1.0 / iFPS);	
	ALLEGRO_EVENT_QUEUE* pQueue = al_create_event_queue();

	al_set_window_title(pDisplay, sTitleApp.c_str());

	al_register_event_source(pQueue, al_get_keyboard_event_source());	
	al_register_event_source(pQueue, al_get_mouse_event_source());
	al_register_event_source(pQueue, al_get_timer_event_source(pTimer));
	al_start_timer(pTimer);
	
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	ALLEGRO_BITMAP* pBitmap = al_load_bitmap("spaceship_2.png");
	
	unsigned int iEnemies = 24;
	
	ALLEGRO_BITMAP** arrPtrEnemies = new ALLEGRO_BITMAP*[iEnemies];	
	float* fEnemiesPosX = new float[iEnemies];
	float* fEnemiesPosY = new float[iEnemies];

	int xPlus = 1;
	int yPlus = 1;
	int iEnemiesPerRow = 8;

	for (unsigned int i = 0; i < iEnemies; i++) {		
		if (i % iEnemiesPerRow == 0) { ++yPlus; xPlus = 1; }
		fEnemiesPosX[i] = xPlus * 75.0f;
		fEnemiesPosY[i] = yPlus * 75.0f;
		arrPtrEnemies[i] = al_load_bitmap("spaceship_enemy_2.png");
		++xPlus;
	}
	
	ALLEGRO_BITMAP* pDoubleBuffer = al_create_bitmap(gbl_iAppWidth, gbl_iAppHeight);
	ALLEGRO_COLOR oBlack = al_map_rgb(0, 0, 0);
	
	float fPlayerShipMovement = 10.0f;
	float fPosX = (600.0f / 2) + (128.0f / 2);
	float fPosY = (600.0f - 128.0f) - 15.0f;
	float fEnemiesSpeedMult = 0.0f;
	bool bMoveRight = false;
	bool bMoveLeft = false;
	bool bMoveUp = false;
	bool bMoveDown = false;

	ALLEGRO_EVENT oEvent;
	while (true) {

		al_wait_for_event(pQueue, &oEvent);

		if (oEvent.type == ALLEGRO_EVENT_TIMER && oEvent.timer.source == pTimer) {

			if (bMoveRight == true) {
				fPosX += fPlayerShipMovement;
			}

			if (bMoveLeft == true) {
				fPosX -= fPlayerShipMovement;
			}

			if (bMoveUp == true) {
				fPosY -= fPlayerShipMovement;
			}

			if (bMoveDown == true) {
				fPosY += fPlayerShipMovement;
			}

			redraw(pDisplay, pBitmap, pDoubleBuffer, oBlack, fPosX, fPosY);
			redrawEnemies(pDisplay, arrPtrEnemies, pDoubleBuffer, fEnemiesPosX, fEnemiesPosY, iEnemies, fEnemiesSpeedMult);
			al_set_target_bitmap(al_get_backbuffer(pDisplay));
			al_draw_bitmap(pDoubleBuffer, 0, 0, 0);
			al_flip_display();
		}

		if (oEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				std::cout << "ESCAPE, SALIENDO...\n";
				break;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				std::cout << "DERECHA\n";
				bMoveRight = true;
				bMoveLeft = false;
				bMoveUp = false;
				bMoveDown = false;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				std::cout << "IZQUIERDA\n";
				bMoveLeft = true;
				bMoveRight = false;
				bMoveUp = false;
				bMoveDown = false;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_UP) {
				std::cout << "ARRIBA\n";
				bMoveLeft = false;
				bMoveRight = false;
				bMoveUp = true;
				bMoveDown = false;
			}

			if (oEvent.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				std::cout << "ABAJO\n";
				bMoveLeft = false;
				bMoveRight = false;
				bMoveUp = false;
				bMoveDown = true;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_PAD_PLUS) {
				std::cout << "Velocidad enemigos: " << fEnemiesSpeedMult << "\n";
				fEnemiesSpeedMult += 0.5f;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_PAD_MINUS) {
				std::cout << "Velocidad enemigos: " << fEnemiesSpeedMult << "\n";
				fEnemiesSpeedMult -= 0.5f;
			}
		}


		if (oEvent.type == ALLEGRO_EVENT_KEY_UP) {
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				bMoveRight = false;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				bMoveLeft = false;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_UP) {
				bMoveUp = false;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				bMoveDown = false;
			}
		}

		if (oEvent.type == ALLEGRO_EVENT_KEY_CHAR) {
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_PAD_PLUS) {
				std::cout << "Velocidad enemigos: " << fEnemiesSpeedMult << "\n";
				fEnemiesSpeedMult += 0.5f;
			}
			if (oEvent.keyboard.keycode == ALLEGRO_KEY_PAD_MINUS) {
				std::cout << "Velocidad enemigos: " << fEnemiesSpeedMult << "\n";
				fEnemiesSpeedMult -= 0.5f;
			}
		}
	}

	al_destroy_timer(pTimer);
	al_destroy_bitmap(pBitmap);
	for (unsigned int i = 0; i < iEnemies; i++) {
		al_destroy_bitmap(arrPtrEnemies[i]);
	}
	al_destroy_display(pDisplay);
	al_destroy_event_queue(pQueue);

	delete fEnemiesPosX;
	delete fEnemiesPosY;

	return 0;
}