#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimaze("unroll-loops")

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <bits/stdc++.h>
#include "FormSFML.h"
#include <omp.h>

using namespace sf;
using namespace std;
#define ld long double
//#define int float

ld cen_x = 0, cen_y = 0;
unsigned int width = 1, height = 1;
unsigned int curw = 1, curh = 1;
ld sz_h = 2;
ld sz_w = sz_h * ((ld)width / height);
ld coef = 0.9;
ld shift = 0.01;
const unsigned int MX_TOTAL = 80;
unsigned int TOTAL = 80;
int seed = 23874;//228
const ld EPS = 1e-10;
Font fnt;
int Type = 1;


struct Complex {
    ld x, y;
    Complex() = default;
    Complex(ld x, ld y): x(x), y(y){}
    Complex operator*(const Complex b) const {
        return {x * b.x - y * b.y, x * b.y + y * b.x};
    }
    Complex operator+(const Complex b) const {
        return {x + b.x, y + b.y};
    }
    ld len() const {
        return sqrtl(x * x + y * y);
    }
    ld len2() const {
        return x * x + y * y;
    }
};

int get_cnt(Complex c, Complex add, const ld k) {
    if (Type == 2)
        swap(c, add);
    Complex z = add;
    int ct = 0;
    while (ct < TOTAL) {
        ct++;
        z = z * z + c;
        if (z.len() > k - EPS)
            return ct;
    }
    return ct;
}

array<int, 3> colors[MX_TOTAL];

void draw_point(Image &image, int x, int y, const array<int, 3> &col) {
    Color cur;
    cur.r = col[0];
    cur.g = col[1];
    cur.b = col[2];
    image.setPixel(x, y, cur);
}

Image draw(const ld k, const Complex add) {
    Image image;
    image.create(width, height);
    ld w = sz_w * ((ld)curw / width);
    ld h = sz_h * ((ld)curh / height);
    ld l = cen_x - w, r = cen_x + w, u = cen_y + h, d = cen_y - h;
#pragma omp parallel for schedule(dynamic) num_threads(16)
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ld rx = l + ((r - l) / width) * x;
            ld ry = u - ((u - d) / height) * y;
            int ct = get_cnt(Complex(rx, ry), add, k);
            array col = {0, 0, 0};
            if (ct != TOTAL)
                col = colors[ct];
            draw_point(image, x, y, col);
        }
    }
    return image;
}

void random_colors() {
    for (int i = 0; i < MX_TOTAL; i++) {
        colors[i] = {rand() % 256, rand() % 256, rand() % 256};
    }
}

void green() {
    for (int i = 0; i < MX_TOTAL; i++) {
        colors[i] = {30, 250 - 3 * i, 50};
    }
}

void random_colors2() {
    colors[0] = {255, 255, 255};
    for (int i = 1; i < MX_TOTAL; i++) {
        colors[i] = colors[i - 1];
        int k = rand() % 3;
        while (colors[i][k] <= 10)
            k = rand() % 3;
        colors[i][k] -= 9;
    }
}

void draw_lab(RenderWindow &window, string str, int posx, int posy) {
    Text lab;
    lab.setString(str);
    lab.setFont(fnt);
    lab.setPosition(posx, posy);
    lab.setFillColor(Color::Black);
    lab.setOutlineThickness(1);
    lab.setLetterSpacing(2);
    lab.setOutlineColor(Color::White);
    lab.setCharacterSize(20);
    window.draw(lab);
}

void draw() {
    srand(seed);
//        random_colors();
//        green();
    random_colors2();
    RenderWindow window(VideoMode(1200, 900), "Fractals");
    width = window.getSize().x, height = window.getSize().y;
    curw = width, curh = height;
    sz_h = 2;
    sz_w = sz_h * ((ld)width / height);



    Complex add = Complex();
    window.setFramerateLimit(30);
    while (window.isOpen()) {
        Event event;
        while ((window.pollEvent(event) && event.type == sf::Event::Closed) ||
               Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::PageUp)) {
            sz_h *= coef;
            sz_w *= coef;
            shift *= coef;
        }
        if (Keyboard::isKeyPressed(Keyboard::PageDown)) {
            sz_h /= coef;
            sz_w /= coef;
            shift /= coef;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
            cen_x -= sz_w * (1.0 - coef);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
            cen_x += sz_w * (1.0 - coef);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
            cen_y += sz_h * (1.0 - coef);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            cen_y -= sz_h * (1.0 - coef);
        }
        if (Keyboard::isKeyPressed(Keyboard::R)) {
            add.x += shift;
        }
        if (Keyboard::isKeyPressed(Keyboard::F)) {
            add.x -= shift;
        }
        if (Keyboard::isKeyPressed(Keyboard::T)) {
            add.y += shift;
        }
        if (Keyboard::isKeyPressed(Keyboard::G)) {
            add.y -= shift;
        }

        window.clear();
        curw = window.getSize().x;
        curh = window.getSize().y;
        if (curw <= 0) {
            window.display();
            continue;
        }

        Texture tmp;
        tmp.create(width, height);
        tmp.update(draw(2, add));
        Sprite cur;
        cur.setTexture(tmp);
        window.draw(cur);

        ld w = sz_w * ((ld) curw / width);
        ld h = sz_h * ((ld) curh / height);
        ld l = cen_x - w, r = cen_x + w, u = cen_y + h, d = cen_y - h;
        string lt = to_string(l), rt = to_string(r), ut = to_string(u), dt = to_string(d);

        draw_lab(window, lt, 5, height / 2);
        draw_lab(window, rt, width - 90, height / 2);
        draw_lab(window, ut, width / 2 - 45, 3);
        draw_lab(window, dt, width / 2 - 45, height - 25);

        window.display();
    }
}

int32_t main() {
    fnt.loadFromFile("Myriad-Pro_31655.ttf");
    RenderWindow dialog(VideoMode(300, 200), "Fractals");
    form::Button mobius(33, 85, 100, 30, "Run Mandel");
    form::Button julia(167, 85, 100, 30, "Run Julia");

    while (dialog.isOpen()) {
        Event event;
        while ((dialog.pollEvent(event) && event.type == sf::Event::Closed) ||
               Keyboard::isKeyPressed(Keyboard::Escape)) {
            dialog.close();
        }
        dialog.clear(Color(200, 200, 200));

        dialog.draw(mobius.displayButton());
        dialog.draw(mobius.displayText());

        dialog.draw(julia.displayButton());
        dialog.draw(julia.displayText());

        dialog.display();
        Vector2i mouse = Mouse::getPosition(dialog);
        if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (mobius.select(mouse)) {
                Type = 1;
                draw();
            }
            if (julia.select(mouse)) {
                Type = 2;
                draw();
            }
        }
    }
}