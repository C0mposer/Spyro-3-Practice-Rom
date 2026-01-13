#include <symbols.h>
#include <shapes.h>

void CustomDrawPolygon(Poly4F customInfo, PrimitiveCode shapeCode)
{

    Poly4F *shapeInfo = (Poly4F *)ptrAvailablePrimitiveSlot;

    shapeInfo->code = shapeCode;

    if (shapeCode == POLY4F_OPAQUE || shapeCode == POLY4F_TRANSPARENT)
        shapeInfo->tag = 0x5000000;
    if (shapeCode == POLY3F_OPAQUE || shapeCode == POLY3F_TRANSPARENT)
        shapeInfo->tag = 0x4000000;

    shapeInfo->color.R = customInfo.color.R;
    shapeInfo->color.G = customInfo.color.G;
    shapeInfo->color.B = customInfo.color.B;

    shapeInfo->point1Pos.x = customInfo.point1Pos.x;
    shapeInfo->point1Pos.y = customInfo.point1Pos.y;
    shapeInfo->point2Pos.x = customInfo.point2Pos.x;
    shapeInfo->point2Pos.y = customInfo.point2Pos.y;
    shapeInfo->point3Pos.x = customInfo.point3Pos.x;
    shapeInfo->point3Pos.y = customInfo.point3Pos.y;

    if (shapeCode == POLY4F_OPAQUE || shapeCode == POLY4F_TRANSPARENT)
    {
        shapeInfo->point4Pos.x = customInfo.point4Pos.x;
        shapeInfo->point4Pos.y = customInfo.point4Pos.y;
    }
    DrawPrimitive(shapeInfo);

    ptrAvailablePrimitiveSlot += (sizeof(Poly4F) - 4);
}

void CustomDrawPolygonShaded(Poly4FShaded customInfo, PrimitiveCode shapeCode)
{

    Poly4FShaded *shapeInfo = (Poly4FShaded *)ptrAvailablePrimitiveSlot;

    shapeInfo->code = shapeCode;

    if (shapeCode == POLY4F_TRANSPARENT_SHADED || shapeCode == POLY4F_OPAQUE_SHADED)
        shapeInfo->tag = 0x8000000;
    if (shapeCode == POLY3F_TRANSPARENT_SHADED || shapeCode == POLY3F_OPAQUE_SHADED)
        shapeInfo->tag = 0x6000000;

    shapeInfo->color.R = customInfo.color.R;
    shapeInfo->color.G = customInfo.color.G;
    shapeInfo->color.B = customInfo.color.B;

    shapeInfo->color2.R = customInfo.color2.R;
    shapeInfo->color2.G = customInfo.color2.G;
    shapeInfo->color2.B = customInfo.color2.B;

    shapeInfo->color3.R = customInfo.color3.R;
    shapeInfo->color3.G = customInfo.color3.G;
    shapeInfo->color3.B = customInfo.color3.B;

    shapeInfo->point1Pos.x = customInfo.point1Pos.x;
    shapeInfo->point1Pos.y = customInfo.point1Pos.y;
    shapeInfo->point2Pos.x = customInfo.point2Pos.x;
    shapeInfo->point2Pos.y = customInfo.point2Pos.y;
    shapeInfo->point3Pos.x = customInfo.point3Pos.x;
    shapeInfo->point3Pos.y = customInfo.point3Pos.y;
    if (shapeCode == POLY4F_TRANSPARENT_SHADED || shapeCode == POLY4F_OPAQUE_SHADED)
    {
        shapeInfo->point4Pos.x = customInfo.point4Pos.x;
        shapeInfo->point4Pos.y = customInfo.point4Pos.y;

        shapeInfo->color4.R = customInfo.color4.R;
        shapeInfo->color4.G = customInfo.color4.G;
        shapeInfo->color4.B = customInfo.color4.B;
    }

    DrawPrimitive(shapeInfo);

    ptrAvailablePrimitiveSlot += (sizeof(Poly4FShaded) - 4);
}

void CustomDrawPolygonTextured(Poly4FTextured customInfo, PrimitiveCode shapeCode)
{

    Poly4FTextured *shapeInfo = (Poly4FTextured *)ptrAvailablePrimitiveSlot;

    shapeInfo->code = 0x2D;
    shapeInfo->tag = 0x9000000;

    shapeInfo->color.R = customInfo.color.R;
    shapeInfo->color.G = customInfo.color.G;
    shapeInfo->color.B = customInfo.color.B;

    shapeInfo->point1Pos.x = customInfo.point1Pos.x;
    shapeInfo->point1Pos.y = customInfo.point1Pos.y;
    shapeInfo->point2Pos.x = customInfo.point2Pos.x;
    shapeInfo->point2Pos.y = customInfo.point2Pos.y;
    shapeInfo->point3Pos.x = customInfo.point3Pos.x;
    shapeInfo->point3Pos.y = customInfo.point3Pos.y;
    shapeInfo->point4Pos.x = customInfo.point4Pos.x;
    shapeInfo->point4Pos.y = customInfo.point4Pos.y;

    shapeInfo->texturePoint1Pos.x = customInfo.texturePoint1Pos.x;
    shapeInfo->texturePoint1Pos.y = customInfo.texturePoint1Pos.y;
    shapeInfo->texturePoint2Pos.x = customInfo.texturePoint2Pos.x;
    shapeInfo->texturePoint2Pos.y = customInfo.texturePoint2Pos.y;
    shapeInfo->texturePoint3Pos.x = customInfo.texturePoint3Pos.x;
    shapeInfo->texturePoint3Pos.y = customInfo.texturePoint3Pos.y;
    shapeInfo->texturePoint4Pos.x = customInfo.texturePoint4Pos.x;
    shapeInfo->texturePoint4Pos.y = customInfo.texturePoint4Pos.y;

    shapeInfo->clut = customInfo.clut;
    shapeInfo->texturePage = customInfo.texturePage;

    DrawPrimitive(shapeInfo);

    ptrAvailablePrimitiveSlot += (sizeof(Poly4FTextured) - 4);
}

void CustomDrawPolygonShadedTextured(Poly4FShadedTextured customInfo, PrimitiveCode shapeCode)
{

    Poly4FShadedTextured *shapeInfo = (Poly4FShadedTextured *)ptrAvailablePrimitiveSlot;

    shapeInfo->code = 0x3C;
    shapeInfo->tag = 0xC000000;

    shapeInfo->color.R = customInfo.color.R;
    shapeInfo->color.G = customInfo.color.G;
    shapeInfo->color.B = customInfo.color.B;

    shapeInfo->color2.R = customInfo.color2.R;
    shapeInfo->color2.G = customInfo.color2.G;
    shapeInfo->color2.B = customInfo.color2.B;

    shapeInfo->color3.R = customInfo.color3.R;
    shapeInfo->color3.G = customInfo.color3.G;
    shapeInfo->color3.B = customInfo.color3.B;

    shapeInfo->color4.R = customInfo.color4.R;
    shapeInfo->color4.G = customInfo.color4.G;
    shapeInfo->color4.B = customInfo.color4.B;

    shapeInfo->point1Pos.x = customInfo.point1Pos.x;
    shapeInfo->point1Pos.y = customInfo.point1Pos.y;
    shapeInfo->point2Pos.x = customInfo.point2Pos.x;
    shapeInfo->point2Pos.y = customInfo.point2Pos.y;
    shapeInfo->point3Pos.x = customInfo.point3Pos.x;
    shapeInfo->point3Pos.y = customInfo.point3Pos.y;
    shapeInfo->point4Pos.x = customInfo.point4Pos.x;
    shapeInfo->point4Pos.y = customInfo.point4Pos.y;

    shapeInfo->texturePoint1Pos.x = customInfo.texturePoint1Pos.x;
    shapeInfo->texturePoint1Pos.y = customInfo.texturePoint1Pos.y;
    shapeInfo->texturePoint2Pos.x = customInfo.texturePoint2Pos.x;
    shapeInfo->texturePoint2Pos.y = customInfo.texturePoint2Pos.y;
    shapeInfo->texturePoint3Pos.x = customInfo.texturePoint3Pos.x;
    shapeInfo->texturePoint3Pos.y = customInfo.texturePoint3Pos.y;
    shapeInfo->texturePoint4Pos.x = customInfo.texturePoint4Pos.x;
    shapeInfo->texturePoint4Pos.y = customInfo.texturePoint4Pos.y;

    shapeInfo->clut = customInfo.clut;
    shapeInfo->texturePage = customInfo.texturePage;

    DrawPrimitive(shapeInfo);

    ptrAvailablePrimitiveSlot += (sizeof(Poly4FShadedTextured) - 4);
}

//* My custom draw line function, that takes my custom Line data type. I made this so I can change the color of the line.
void CustomDrawLine(Line customLineData, PrimitiveCode shapeCode)
{
    Line *lineData;

    lineData = (Line *)ptrAvailablePrimitiveSlot;
    lineData->tag = 0x4000000;  //? Tag?
    lineData->code = shapeCode; //? Type of shape

    lineData->point1Pos.x = customLineData.point1Pos.x;
    lineData->point1Pos.y = customLineData.point1Pos.y;
    lineData->point2Pos.x = customLineData.point2Pos.x;
    lineData->point2Pos.y = customLineData.point2Pos.y;

    lineData->color.R = customLineData.color.R; //? First half R color
    lineData->color.G = customLineData.color.G; //? First half G color
    lineData->color.B = customLineData.color.B; //? First half B color

    DrawPrimitive(lineData);
    ptrAvailablePrimitiveSlot += (sizeof(Poly4F) - 4);
}

//* My custom draw line function, that takes my custom LineGradient data type. I made this so I can change the color of the line, and have it gradient from point 1 to point 2.
void CustomDrawLineShaded(ShadedLine customLineData, PrimitiveCode shapeCode)

{
    struct ShadedLine *lineData;

    lineData = (ShadedLine *)ptrAvailablePrimitiveSlot;
    lineData->tag = 0x4000000;  //? Tag?
    lineData->code = shapeCode; //? Type of shape maybe?

    lineData->point1Pos.x = customLineData.point1Pos.x;
    lineData->point1Pos.y = customLineData.point1Pos.y;
    lineData->point2Pos.x = customLineData.point2Pos.x;
    lineData->point2Pos.y = customLineData.point2Pos.y;

    lineData->colorP1.R = customLineData.colorP1.R; //? First half R color
    lineData->colorP1.G = customLineData.colorP1.G; //? First half G color
    lineData->colorP1.B = customLineData.colorP1.B; //? First half B color

    lineData->colorP2.R = customLineData.colorP2.R; //? Second half R color
    lineData->colorP2.G = customLineData.colorP2.G; //? Second half G color
    lineData->colorP2.B = customLineData.colorP2.B; //? Second half B color

    DrawPrimitive(lineData);
    ptrAvailablePrimitiveSlot += (sizeof(Poly4F) - 4);
}

//* Draws a shape and cycles between R G B

// CustomDrawShapeTransparentSubtractive(myShape);

// void CustomDrawPrimitive(void* customInfo, int shapeCode)
// {

//     void* shapeInfo = (void*)ptrAvailablePrimitiveSlot;          //void* for generic shape

//     enum TypeOfShape typeOfShape = 0;
//     int amountOfSides = 0;

//     (Poly3F*)shapeInfo->code = shapeCode;                  //Poly3F just because code is in the same spot for all the structs

//     if(shapeCode == POLY4F_OPAQUE || shapeCode == POLY4F_TRANSPARENT)
//     {
//         (Poly4F*)shapeInfo->tag = 0x5000000;
//         typeOfShape = POLY;
//         amountOfSides = 4;
//     }
//     if(shapeCode == POLY3F_OPAQUE || shapeCode == POLY3F_TRANSPARENT)
//     {
//         (Poly3F*)shapeInfo->tag = 0x4000000;
//         typeOfShape = POLY;
//         amountOfSides = 3;
//     }
//     if(shapeCode == POLY4F_TRANSPARENT_SHADED || shapeCode == POLY4F_OPAQUE_SHADED)
//     {
//         (Poly4FShaded*)shapeInfo->tag = 0x8000000;
//         typeOfShape = POLY_SHADED;
//         amountOfSides = 4;
//     }
//     if(shapeCode == POLY3F_TRANSPARENT_SHADED || shapeCode == POLY3F_OPAQUE_SHADED)
//     {
//         (Poly4FShaded*)shapeInfo->tag = 0x6000000;
//         typeOfShape = POLY_SHADED;
//         amountOfSides = 3;
//     }
//     if(shapeCode == POLY3F_OPAQUE_TEXURED_RAW || shapeCode == POLY3F_TRANSPARENT_TEXTURED_RAW)
//     {
//         (Poly4FTextured*)shapeInfo->tag = 0x8000000;
//         typeOfShape = POLY_TEXTURED;
//         amountOfSides = 3;
//     }
//     if(shapeCode == POLY4F_OPAQUE_TEXTURED_RAW || shapeCode == POLY4F_TRANSPARENT_TEXTURED_RAW)
//     {
//         (Poly4FTextured*)shapeInfo->tag = 0x9000000;
//         typeOfShape = POLY_TEXTURED;
//         amountOfSides = 4;
//     }
//     if(shapeCode == POLY3F_OPAQUE_SHADEDANDTEXTURED_RAW || POLY3F_TRANSPARENT_SHADEDANDTEXTURED_RAW)
//     {
//         (Poly4FShadedTextured*)shapeInfo->tag = 0xB000000;
//         typeOfShape = POLY_TEXTUREDANDSHADED;
//         amountOfSides = 3;
//     }
//     if(shapeCode == POLY4F_OPAQUE_SHADEDANDTEXTURED_RAW || POLY4F_TRANSPARENT_SHADEDANDTEXTURED_RAW)
//     {
//         (Poly4FShadedTextured*)shapeInfo->tag = 0xC000000;
//         typeOfShape = POLY_TEXTUREDANDSHADED;
//         amountOfSides = 4;
//     }

//     (PolyshapeInfo->color.R = customInfo.color.R;
//     shapeInfo->color.G = customInfo.color.G;
//     shapeInfo->color.B = customInfo.color.B;

//     shapeInfo->point1Pos.x = customInfo.point1Pos.x;
//     shapeInfo->point1Pos.y = customInfo.point1Pos.y;
//     shapeInfo->point2Pos.x = customInfo.point2Pos.x;
//     shapeInfo->point2Pos.y = customInfo.point2Pos.y;
//     shapeInfo->point3Pos.x = customInfo.point3Pos.x;
//     shapeInfo->point3Pos.y = customInfo.point3Pos.y;
//     if(amountOfSides == 4)
//     {
//         shapeInfo->point4Pos.x = customInfo.point4Pos.x;
//         shapeInfo->point4Pos.y = customInfo.point4Pos.y;
//     }

//     if(typeOfShape == POLY_SHADED && amountOfSides == 3 || amountOfSides == 4)
//     {
//         shapeInfo->color2.R = customInfo.color2.R;
//         shapeInfo->color2.G = customInfo.color2.G;
//         shapeInfo->color2.B = customInfo.color2.B;

//         shapeInfo->color3.R = customInfo.color3.R;
//         shapeInfo->color3.G = customInfo.color3.G;
//         shapeInfo->color3.B = customInfo.color3.B;
//     }
//     if(typeOfShape == POLY_SHADED && amountOfSides == 4)
//     {
//         shapeInfo->color4.R = customInfo.color4.R;
//         shapeInfo->color4.G = customInfo.color4.G;
//         shapeInfo->color4.B = customInfo.color4.B;
//     }

//     DrawPrimitive(shapeInfo);                               //? Main Call

//     ptrAvailablePrimitiveSlot += (sizeof(Poly4F) - 4);           //! Look into this. Increasing the buffer for drawing?
// }
