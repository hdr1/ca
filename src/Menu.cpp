#include "config.h"
#include "Menu.h"
#include "Utils.h"

Menu::Menu() { }
Menu::~Menu() 
{
    for (int i = 0; i < this->entries.size(); i++)
    {
        delete this->entries[i];
    }
}

Menu::Menu(SDL_Renderer* render, TTF_Font* font)
{
    this->render = render;
    this->font = font;
    this->height = 0;
    this->width = 0;
}

void Menu::Show(int x, int y)
{
    this->x = x;
    this->y = y;
    this->showing = true;

    Uint32 sTicks;
    while (this->showing)
    {
        sTicks = SDL_GetTicks();

        this->ProcessInput();
        this->Draw(x, y);
        this->Update();

        Uint32 espera = 30 - (SDL_GetTicks() - sTicks);
        SDL_Delay(espera > 30 ? 30 : espera);
    }
}

void Menu::ProcessInput()
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)
            this->showing = false;
    }
}

void Menu::Update()
{
    SDL_RenderPresent(this->render);
}

void Menu::AddNewEntry(std::string text)
{
    MenuEntry* entry = new MenuEntry(this, text);

    this->width = std::max(this->width, entry->getRect().w + 6);
    this->height += FONT_SIZE + 10;
    this->entries.push_back(entry);
}

void Menu::Draw(int x, int y)
{
    int xx = x, yy = y;
    for (int i = 0; i < this->entries.size(); i++)
    {
        SDL_Rect dstRect = this->entries[i]->getRect();
        dstRect.x = xx + 3;
        dstRect.y = yy + 4;

        Poligono h(4);
        h[0] = {xx, yy};
        h[1] = {xx + this->width, yy};
        h[2] = {xx + this->width, yy + FONT_SIZE + 10};
        h[3] = {xx, yy + FONT_SIZE + 10};
        h.fill(this->render, 0xc4, 0xc6, 0xc7);
        h.draw(this->render, 0x3f, 0x40, 0x40);

        

        SDL_RenderCopy(this->render, this->entries[i]->getTexture(), NULL, &dstRect);
        yy += FONT_SIZE + 10;
    }
}

SDL_Renderer* Menu::getRender() { return this->render; }
TTF_Font* Menu::getFont() { return this->font; }

MenuEntry::MenuEntry (Menu* menu, std::string text)
{
    this->menu = menu;
    this->text = text;
    this->hasChild = false;
    this->child = NULL;

    Utils::CreateText(this->menu->getRender(), this->menu->getFont(), this->text.c_str(), {0x0, 0x0, 0x0, 0x0}, &(this->texture), &(this->rect));
}

MenuEntry::~MenuEntry()
{
    if (this->texture)
        SDL_DestroyTexture(this->texture);
    if (this->child)
        delete child;
}

std::string MenuEntry::getText()
{
    return this->text;
}

SDL_Texture* MenuEntry::getTexture() { return this->texture; }
SDL_Rect MenuEntry::getRect() { return this->rect; }