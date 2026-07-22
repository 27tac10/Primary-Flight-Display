#ifndef DRAW_COMMAND_H
#define DRAW_COMMAND_H

#include <vector>
#include <string>
#include <variant>

struct Point {
    double x = 0.0;
    double y = 0.0;
};

struct Color {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    double a = 1.0; // Alpha channel for transparency
};

struct LineCommand {
    Point start;
    Point end;
    Color color;
    double width;
};

struct FilledPolygonCommand {
    std::vector<Point> listOfPoints;
    Color fillColor;  
};

enum class TextAnchor {
    TopLeft,
    Center
};

struct TextCommand {
    std::string text;
    Point position;
    Color color;
    double fontSize;
    TextAnchor anchor;
};

struct Rectangle {
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;
};

struct PushClipCommand {
    Rectangle clipRect;
};

struct PopClipCommand {
    // No additional data needed for pop clip command
};

struct PushTransformCommand {
    Point pivot;
    double rotationAngle = 0.0; // In degrees
};

struct PopTransformCommand {
    // No additional data needed for pop transform command
};

using DrawCommand = std::variant<   LineCommand, 
                                    FilledPolygonCommand, 
                                    TextCommand, 
                                    PushClipCommand, 
                                    PopClipCommand, 
                                    PushTransformCommand, 
                                    PopTransformCommand>;

using DrawList = std::vector<DrawCommand>;

#endif // DRAW_COMMAND_H