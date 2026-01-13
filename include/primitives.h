#ifndef primitives_h
#define primitives_h
#include <shapes.h>

void CustomDrawPolygonShaded(Poly4FShaded customInfo, PrimitiveCode shapeCode);
void CustomDrawPolygon(Poly4F customInfo, PrimitiveCode shapeCode);
void CustomDrawPolygonTextured(Poly4FTextured customInfo, PrimitiveCode shapeCode);
void CustomDrawPolygonShadedTextured(Poly4FShadedTextured customInfo, PrimitiveCode shapeCode);
void CustomDrawLine(Line customLineData, PrimitiveCode shapeCode);
void CustomDrawLineShaded(ShadedLine customLineData, PrimitiveCode shapeCode);

#endif primitives_h