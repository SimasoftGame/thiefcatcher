#include "DossierScreen.h"

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

#include <Text.h>
#include <FontManager.h>

#include "utilities/Translator.h"
#include "entities/CriminalsManager.h"
#include "entities/format/GenreFormatter.h"
#include <GameObject.h>

class ArrowKeysMessage : public Kangaroo::GameObject {
public:

    const int seconds = 4;

    Uint8 alpha = 255;
    
public:
    ArrowKeysMessage(Renderer& renderer) :
    GameObject(renderer) {
        
    }

    virtual void update(double time) override {
        alpha = (Uint8) (time > seconds ? 0 : 255 - (255 / seconds) * time);
    }

    virtual void render() override {
        Font headerFont("resources/fonts/gtw.ttf", 18);
        headerFont.setColor(Color(0xff, 0xff, 0xff));
        Text headerText(_("Use the arrow keys to find more criminals"), &headerFont);
        headerText.setAlpha(alpha);

        renderer.drawText(&headerText, Point(140, 500));
    }
};

DossierScreen::DossierScreen(Window *window_) :
window(window_),
backgroundTexture(window_->renderer, 800, 600),
normalCursor(SDL_SYSTEM_CURSOR_ARROW),
handCursor(SDL_SYSTEM_CURSOR_HAND) {
    criminals = CriminalsManager::findAll();

    sensAreas.addArea(Area(Point(28, 511), Dimension(61, 61)));

    index = 0;
    quit = false;

    fontName.load("resources/fonts/FreeSansBold.ttf", 30);
    fontName.setColor(Color(255, 200, 100));

    font.load("resources/fonts/FreeSansBold.ttf", 18);

    Renderer renderer(window_->renderer);

    backgroundTexture.selectAsRenderingTarget(window_->renderer);

    Texture t(window->renderer, "resources/images/dossiers/background.png");
    renderer.drawTexture(&t);

    Font headerFont("resources/fonts/gtw.ttf", 45);
    headerFont.setColor(Color(255, 220, 220));
    Text headerText(_("Thief Catcher"), &headerFont);
    renderer.drawText(&headerText, Point(30, 10));

    backgroundTexture.unselectAsRenderingTarget(window_->renderer);
}

DossierScreen::~DossierScreen() {
}

void DossierScreen::show() {
    init();

    Renderer renderer(window->renderer);
    ArrowKeysMessage arrowKeysMessageGameObject(renderer);
    gameObjects.push_back(&arrowKeysMessageGameObject);

    while (!quit) {
        captureEvents();
        updateObjects();
        renderStaticElements(renderer);
        renderObjects(renderer);
        renderer.present();
    }
}

void DossierScreen::renderStaticElements(Renderer& renderer) {
    Criminal &criminal = criminals.at(index);

    ostringstream pathStream;
    pathStream << "data/criminals/" << criminal.getID() << ".jpg";

    renderer.drawTexture(&backgroundTexture);

    Texture avatarTexture(window->renderer, pathStream.str());
    renderer.drawTexture(&avatarTexture, Point(460, 90));

    Text nameText(criminal.getName(), &fontName);
    renderer.drawText(&nameText, Point(140, 140));

    const struct CriminalTrait traits[] = {
        {_("Genre"), GenreFormatter::format(criminal.getGenre())},
        {_("Build"), criminal.getBuild()},
        {_("Hair"), criminal.getHair()},
        {_("Feature"), criminal.getFeature()},
        {_("Complexion"), criminal.getComplexion()},
    };
    const size_t numTraits = sizeof (traits) / sizeof (traits[0]);

    int y = 250;
    for (size_t i = 0; i < numTraits; i++) {
        struct CriminalTrait trait = traits[i];

        Text labelText(trait.label, &font);
        font.setColor(Color(0, 0, 0));

        renderer.drawText(&labelText, Point(300 - labelText.getDimension().w, y));

        Text valueText(trait.value, &font);
        font.setColor(Color(200, 160, 60));
        renderer.drawText(&valueText, Point(320, y));

        y += labelText.getDimension().h + (labelText.getDimension().h >> 1);
    }
}

void DossierScreen::onMouseMotion(SDL_MouseMotionEvent e) {
    if (sensAreas.resolve(e.x, e.y) == -1)
        normalCursor.applyToWindow();
    else
        handCursor.applyToWindow();
}

void DossierScreen::onMouseButtonDown(SDL_MouseButtonEvent e) {
    if (sensAreas.resolve(e.x, e.y) != -1) {
        quit = true;
    }
    if (e.button == SDL_BUTTON_RIGHT) {
        quit = true;
    }
}

void DossierScreen::onKeyDown(SDL_KeyboardEvent e) {
    switch (e.keysym.sym) {
        case SDLK_RIGHT:
            if (index + 1 < static_cast<int> (criminals.size())) {
                index++;
            }
            break;
        case SDLK_LEFT:
            if (index > 0) {
                index--;
            }
            break;
        case SDLK_ESCAPE:
            quit = true;
            break;
        default:
            break;
    }
}
