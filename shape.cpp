// $Id: shape.cpp,v 1.2 2019-02-28 15:24:20-08 - - $
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#endif
#include <typeinfo>
#include <unordered_map>
#include <math.h>
using namespace std;

#include "shape.h"
#include "util.h"

static unordered_map<void*, string> fontname{
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string, void*> fontcode{
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
    out << "(" << where.xpos << "," << where.ypos << ")";
    return out;
}

shape::shape() {

}

text::text(void* glut_bitmap_font_, const string& textdata_) :
    glut_bitmap_font(glut_bitmap_font_), textdata(textdata_) {
    size_t txtLen = strlen(textdata_.c_str());
    char textArr[1000];
    strcpy(textArr, textdata.c_str());
    string fontStr = strtok(textArr, " ");
    auto i = textdata.find(" ");
    textdata = textdata.substr(i);
    glut_bitmap_font = fontcode[fontStr];
}

ellipse::ellipse(GLfloat width, GLfloat height) :
    dimension({ width, height }) {
}

circle::circle(GLfloat diameter) : ellipse(diameter, diameter) {

}


polygon::polygon(const vertex_list& vertices_) : vertices(vertices_) {

}

rectangle::rectangle(GLfloat width, GLfloat height) :
    polygon({}) {
}

square::square(GLfloat width) : rectangle(width, width) {}

void text::draw(const vertex& center, const rgbcolor& color) const {
    const GLubyte* glgreeting =
        reinterpret_cast<const GLubyte*> (textdata.c_str());
    glColor3f(color.ubvec[0], color.ubvec[1], color.ubvec[2]);
    glRasterPos2f(center.xpos, center.ypos);
    glutBitmapString(glut_bitmap_font, glgreeting);
}

void ellipse::draw(const vertex& center, const rgbcolor& color) const {
    glBegin(GL_POLYGON);
    glColor3f(color.ubvec[0], color.ubvec[1], color.ubvec[2]);
    const GLfloat delta = 2 * M_PI / 64;
    //const GLfloat radius = window.height * 3.0 / 10.0;
    //const GLfloat radius = window.height * 3.0 / 10.0;
    //const GLfloat xoffset = window.width / 2.0;
    //const GLfloat yoffset = window.height / 2.0;
    for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
        GLfloat xpos = dimension.xpos * cos(theta) + center.xpos;
        GLfloat ypos = dimension.ypos * sin(theta) + center.ypos;
        glVertex2f(xpos, ypos);
    }
    glEnd();
}

void polygon::draw(const vertex& center, const rgbcolor& color) const {
    //DEBUGF ('d', this << "(" << center << "," << color << ")");
}

void shape::show(ostream& out) const {
    out << this << "->" << demangle(*this) << ": ";
}

void text::show(ostream& out) const {
    shape::show(out);
    out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
        << ") \"" << textdata << "\"";
}

void ellipse::show(ostream& out) const {
    shape::show(out);
    out << "{" << dimension << "}";
}

void polygon::show(ostream& out) const {
    shape::show(out);
    out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
    obj.show(out);
    return out;
}

