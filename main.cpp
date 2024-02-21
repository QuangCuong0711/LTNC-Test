#include <bits/stdc++.h>
using namespace std;
int main(){
    bool a=(2.0==sqrt(2)*sqrt(2));
    cout<< a;
}
/*#include <SDL.h>
#include <bits/stdc++.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello SDL";
const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};
const SDL_Color DEFAULT_COLOR = BLACK_COLOR;


class Painter{
  float x;
  float y;
  float angle;
  SDL_Color color;
  SDL_Renderer* renderer;
public:
    Painter(SDL_Window* window,
            SDL_Renderer* renderer);
    void setPosition(float x, float y);
    float getX() const { return x; }
    float getY() const { return y; }

    void setAngle(float angle);
    float getAngle() const { return angle; }

    void setColor(SDL_Color color);
    SDL_Color getColor() const
        { return color; }

    SDL_Renderer* getRenderer() const
        { return renderer; }
    void clearWithBgColor(SDL_Color bgColor) const;
    void moveForward(float length);
    void jumpForward(float length);
    void moveBackward(float length) { moveForward(-length); }
    void jumpBackward(float length) { jumpForward(-length); }
    void turnLeft(float angle) { setAngle(this->angle + angle); }
    void turnRight(float angle) { turnLeft(-angle); }
    SDL_Texture* loadTexture( string path );
    bool createImage( SDL_Texture* texture );
};
Painter::Painter(SDL_Window* window, SDL_Renderer* renderer_): renderer(renderer_){
	int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);
    if (width == 0 && height == 0)
        SDL_GetWindowSize(window, &width, &height);

    setPosition(width/2, height/2);
    setAngle(0);
    setColor(WHITE_COLOR);
    clearWithBgColor(BLUE_COLOR);
}
void Painter::setPosition(float x, float y){
    this->x = x;
    this->y = y;
}
void Painter::setAngle(float angle){
    this->angle = angle
        - floor(angle/360)*360;
}
void Painter::setColor(SDL_Color color){
    this->color = color;
    SDL_SetRenderDrawColor(
        renderer, color.r, color.g, color.b, 0);
}
void Painter::clearWithBgColor(SDL_Color bgColor)const {
  SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}
void Painter::moveForward(float length){
    float prevX = x, prevY = y;
    jumpForward(length);
    SDL_RenderDrawLine(renderer, (int)prevX, (int)prevY, (int)x, (int)y);
}
void Painter::jumpForward(float length){
    float rad = (angle / 180) * M_PI;
    x += cos(rad) * length;
    y -= sin(rad) * length;
}
void logErrorAndExit(const char* msg, const char* error){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_ERROR,
                   "%s: %s", msg, error);
    SDL_Quit();
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void waitUntilKeyPressed(){
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
SDL_Window* initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());
    return window;
}
SDL_Renderer* createRenderer(SDL_Window* window){
    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    //SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}
float generateRandomNumber(){
    return (float) rand() / RAND_MAX;
}
void randomWalk(Painter& painter){
    const int STEPS = 10;
    const float MAX_LENGTH = 100;
    for (int i = 0; i < STEPS; i++) {
        float length = generateRandomNumber() * MAX_LENGTH;
        painter.moveForward(length);
        float angle = generateRandomNumber() * 360;
        painter.turnLeft(angle);
    }
}
SDL_Texture* Painter::loadTexture( string path ){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}
bool Painter::createImage( SDL_Texture* texture ){
    if( texture == NULL ) return false;
    SDL_RenderCopy( renderer, texture, NULL, NULL );
    return true;
}

int main(int argc, char* argv[])
{
    SDL_Window* window = initSDL();
    SDL_Renderer* renderer = createRenderer(window);
    Painter painter(window, renderer);
    SDL_Texture* texture = NULL;
    texture = painter.loadTexture("D:\Anh\_MG_1847.JPG");
            painter.createImage(texture);
    for (int i = 0; i < 4; ++i)
        {
            painter.moveForward(100);
            painter.turnRight(90);
        }

    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    SDL_DestroyTexture(texture);
    quitSDL(window, renderer);
    return 0;

}
*/
