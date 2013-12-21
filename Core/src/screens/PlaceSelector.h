#ifndef PLACE_SELECTOR_H 
#define PLACE_SELECTOR_H

#include <SDL_gfxPrimitives.h>

#include <Surface.h>
#include <SurfaceUtil.h>

#include <Font.h>
#include <FontManager.h>
#include <EventHandler.h>

#include <Text.h>

#include <SensitiveAreas.h>

#include "entities/Place.h"
#include "entities/PlacesManager.h"

class PlaceSelector : public EventHandler {

	private:
		SensitiveAreas sensAreas;
		
		bool quit;
		
		Place places[3];
		
		Surface *screen;
		Surface *surface;
		Surface *dialogBackup;

		Point dialogPosition;
		Dimension dialogDimension;

		Surface *images[3];
		SDL_Rect areas[3];	
		
		int selectedIndex;
		int returnCode;
		
//		Surface *loadImage(const char *name) const;
		void update();
	public:
		PlaceSelector(Surface *screen, int *placesPrimaryKeys);
		~PlaceSelector();
		
		void onQuit(SDL_QuitEvent event) {
			
		}
		void onMouseMotion(SDL_MouseMotionEvent event);
		void onMouseButtonDown(SDL_MouseButtonEvent event);
		void onMouseButtonUp(SDL_MouseButtonEvent event) {
			
		}
		void onKeyDown(SDL_KeyboardEvent event);
		void onKeyUp(SDL_KeyboardEvent event) {
			
		}

		int showAndReturn();
};

#endif
