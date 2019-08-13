// $Id: interp.cpp,v 1.3 2019-03-19 16:18:22-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string, interpreter::interpreterfn>
interpreter::interp_map{
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
};

unordered_map<string, interpreter::factoryfn>
interpreter::factory_map{
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"triangle" , &interpreter::make_polygon  },
   {"diamond" ,  &interpreter::make_diamond  },
   {"equilateral", &interpreter::make_eq     }
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
    for (const auto& itor : objmap) {
        cout << "objmap[" << itor.first << "] = "
            << *itor.second << endl;
    }
}

void interpreter::interpret(const parameters& params) {
    param begin = params.cbegin();
    string command = *begin;
    auto itor = interp_map.find(command);
    if (itor == interp_map.end()) throw runtime_error("syntax error");
    interpreterfn func = itor->second;
    func(++begin, params.cend());
}

void interpreter::do_define(param begin, param end) {
    string name = *begin;
    objmap.emplace(name, make_shape(++begin, end));
}


void interpreter::do_draw(param begin, param end) {
    if (end - begin != 4) throw runtime_error("syntax error");
    string name = begin[1];
    shape_map::const_iterator itor = objmap.find(name);
    if (itor == objmap.end()) {
        throw runtime_error(name + ": no such shape");
    }
    rgbcolor color{ begin[0] };
    vertex where{ from_string<GLfloat>(begin[2]),
                  from_string<GLfloat>(begin[3]) };
    window::push_back(object(itor->second, where, color));
}

shape_ptr interpreter::make_shape(param begin, param end) {
    string type = *begin++;
    auto itor = factory_map.find(type);
    if (itor == factory_map.end()) {
        throw runtime_error(type + ": no such shape");
    }
    factoryfn func = itor->second;
    return func(begin, end);
}

shape_ptr interpreter::make_text(param begin, param end) {
    string textData = *begin;
    textData.append(" ");
    textData.append(*(end - 1));
    return make_shared<text>(&begin, textData);
}

shape_ptr interpreter::make_ellipse(param begin, param end) {
    GLfloat width = stof(*begin);
    GLfloat height = stof(*(end - 1));
    return make_shared<ellipse>(width, height);
}

shape_ptr interpreter::make_circle(param begin, param end) {
    GLfloat radius = stof(*begin);
    return make_shared<circle>(radius);
}

shape_ptr interpreter::make_polygon(param begin, param end) {
    vertex_list vList;
    for (auto i = begin; i != end; i += 2) {
        GLfloat xpos = stof(*i);
        GLfloat ypos = stof(*(i + 1));
        vList.push_back({ xpos, ypos });
    }
    return make_shared<polygon>(vList);
}

shape_ptr interpreter::make_rectangle(param begin, param end) {
    GLfloat width = stof(*begin);
    GLfloat height = stof(*(end - 1));
    return make_shared<rectangle>(width, height);
}

shape_ptr interpreter::make_square(param begin, param end) {
    return make_shared<square>(stof(*begin));
}

shape_ptr interpreter::make_eq(param begin, param end) {
    vertex_list vList;
    vList.push_back({ stof(*begin), 0 });
    vList.push_back({ stof(*begin), stof(*begin) });
    vList.push_back({ 0, stof(*begin) });
    return make_shared<polygon>(vList);
}

shape_ptr interpreter::make_diamond(param begin, param end)
{
    GLfloat width = stof(*begin);
    GLfloat height = stof(*(end - 1));
    return make_shared<diamond>(width, height);
}
