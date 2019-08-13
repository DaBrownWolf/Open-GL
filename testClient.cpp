// $Id: hello-world.cpp,v 1.26 2019-05-15 17:13:01-07 - - $

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include <GL/freeglut.h>
//#include <libgen.h>
#include "getopt.h"
#include "graphics.h"
#include "util.h"
#include "interp.h"

// Characteristics of the window.
struct window window;

template <typename item_t>
struct cycle_iterator {
    const vector<item_t> items;
    size_t curr_item{ 0 };
    cycle_iterator(initializer_list<item_t> list) :
        items(vector<item_t>(list)) {}
    const item_t& operator*() { return items[curr_item]; }
    const item_t& operator[] (size_t index) {
        return items[(curr_item + index) % items.size()];
    }
    cycle_iterator& operator++() {
        curr_item = (curr_item + 1) % items.size();
        return *this;
    }
};

GLubyte RED[] = { 0xFF, 0x00, 0x00 };
GLubyte GREEN[] = { 0x00, 0xFF, 0x00 };
GLubyte BLUE[] = { 0x00, 0x00, 0xFF };
cycle_iterator<GLubyte*> colors{ RED, GREEN, BLUE };

cycle_iterator<void*> glut_fonts{
   GLUT_BITMAP_TIMES_ROMAN_24,
   GLUT_BITMAP_HELVETICA_18,
   GLUT_BITMAP_9_BY_15,
};

void draw_greeting(const string& greeting, const GLubyte* color) {
    const GLubyte* glgreeting =
        reinterpret_cast<const GLubyte*> (greeting.c_str());

    void* font = *glut_fonts;
    int greeting_width = glutBitmapLength(font, glgreeting);
    int greeting_height = glutBitmapHeight(font);

    glColor3ubv(color);
    float width = 100;
    float height = 100;
    float xpos = width / 2.0 - greeting_width / 2.0;
    float ypos = height / 2.0 - greeting_height / 4.0;
    //glRasterPos2f(xpos, ypos);
    window.setwidth(xpos);
    window.setheight(ypos);

    glutBitmapString(font, glgreeting);
}


void invert_colors() {
    for (size_t color = 0; color < 3; ++color) {
        for (size_t rgb = 0; rgb < 3; ++rgb) {
            colors[color][rgb] ^= 0xFF;
        }
    }
}

void print_howto() {
    cout << R"(
To cycle the colors: right mouse button or key 'c' or 'C'.
To cycle the fonts: left mouse button or key 'f' or 'F'.
To cycle the greetings: middle mouse button or key 'g' or 'G'.
To invert the colors: key Space, Backspace, Return, or Delete.
To quit: key 'q' or 'Q' or ESC.
)";
}

int main(int argc, char** argv) {
    print_howto();
    sys_info::execname(argv[0]);
    interpreter::shape_map shapemap;
    interpreter interp;

    string line = "define t1 triangle 50 30 50 100 10 80";
    interpreter::parameters words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "define r1 rectangle 50 30";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "define p1 polygon 50 30 50 100 10 80 60 30";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "define d1 diamond 50 30";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "draw red r1 90 90";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "draw red t1 30 90";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "draw red p1 90 40";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    line = "draw red d1 70 40";
    words = split(line, " \t");
    if (words.size() == 0 or words.front()[0] == '#') return 0;
    interp.interpret(words);

    window.main();
    return 0;
}

//TEST// mkpspdf hello-world.cpp.ps hello-world.cpp*
