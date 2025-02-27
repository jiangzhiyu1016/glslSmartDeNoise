//------------------------------------------------------------------------------
//  Copyright (c) 2018-2024 Michele Morrone
//  All rights reserved.
//
//  https://michelemorrone.eu - https://brutpitt.com
//
//  X: https://x.com/BrutPitt - GitHub: https://github.com/BrutPitt
//
//  direct mail: brutpitt(at)gmail.com - me(at)michelemorrone.eu
//
//  This software is distributed under the terms of the BSD 2-Clause license
//------------------------------------------------------------------------------
#include <vgMath.h>

#include <lodePNG/lodepng.h>

#include "glWindow.h"


void textureBaseClass::genTex(int idx)
{

    if(generated) {
#if !defined(GLCHAOSP_LIGHTVER)
        glDeleteTextures(1,&texID[idx]);
        CHECK_GL_ERROR();
#else
        return;
#endif
    }

#ifdef GLAPP_REQUIRE_OGL45
    glCreateTextures(GL_TEXTURE_2D, 1, &texID[idx]);
#else
    glGenTextures(1, &texID[idx]);   // Generate OpenGL texture IDs
#endif

    generated = true;
}

void textureBaseClass::assignAttribs(int idx, GLint filterMin, GLint filterMag, GLint wrap)
{
#ifdef GLAPP_REQUIRE_OGL45
    glTextureParameteri(texID[idx], GL_TEXTURE_MAG_FILTER, filterMag);
    glTextureParameteri(texID[idx], GL_TEXTURE_MIN_FILTER, filterMin);
    glTextureParameteri(texID[idx], GL_TEXTURE_WRAP_S, wrap );
    glTextureParameteri(texID[idx], GL_TEXTURE_WRAP_T, wrap );
#else
    glBindTexture(GL_TEXTURE_2D, texID[idx]);            // Bind Our Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
#endif
}

void textureBaseClass::buildTex(int idx, void *buffer, const int x, const int y)
{
    genTex(idx);
    const GLint texInternal = GL_RGBA;

    glActiveTexture(GL_TEXTURE0 + texID[idx]);
    glBindTexture(GL_TEXTURE_2D, texID[idx]);            // Bind Our Texture
    glTexImage2D(GL_TEXTURE_2D, 0, texInternal, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    assignAttribs(idx, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();

}

void deNoiseClass::render()
{

    glClearBufferfv(GL_COLOR, 0, value_ptr(vec4(0.0f)));

    bindPipeline();

    glUseProgram(program);

    int idx = theWnd->getDeNoise().getTexture().whichImg;

    glUniform1f(locSigma, aSigma[idx]);
    glUniform1f(locThreshold, aThreshold[idx]);
    glUniform1f(locSlider, slider); 
    glUniform1f(locKSigma, aKSigma[idx]);
    glUniform1f(locInvGamma, 1.0/gamma);
    glUniform1i(locUseTest, useTest);
    glUniform1i(locWhichTest, whichTest);
    glUniform1f(locLowHi, lowHi);
    glUniform2f(locWSize,theApp->GetWidth(),theApp->GetHeight());
    
#ifdef GLAPP_REQUIRE_OGL45
    glBindTextureUnit(1, theWnd->getDeNoise().getTexture().getTexID());
#else
    glActiveTexture(GL_TEXTURE0+theWnd->getDeNoise().getTexture().getTexID());
    glBindTexture(GL_TEXTURE_2D, theWnd->getDeNoise().getTexture().getTexID());
    glUniform1i(locImgData, theWnd->getDeNoise().getTexture().getTexID());
#endif
    //updateBufferData();
    // Draw
    vao.draw();


}

void setView() {}
void setViewOrtho() {}
glWindow::glWindow() {}
glWindow::~glWindow() {}

#define IMG_IDX 2


void buildImage(const char *filename, int index)
{
    std::vector<unsigned char> image;
    unsigned width, height;

    lodepng::decode(image, width, height, filename);
        //ivec2 size(x,y);

    theWnd->getDeNoise().getTexture().buildTex(index, image.data(), width, height);
}


//
/////////////////////////////////////////////////
void glWindow::onInit()
{
    unsigned x = 1280,y = 800;

    deNoise = new deNoiseClass;

    //getDeNoise().sigma = getDeNoise().aSigma[IMG_IDX];
    //getDeNoise().threshold = getDeNoise().aThreshold[IMG_IDX];
    //getDeNoise().kSigma = getDeNoise().aKSigma[IMG_IDX];
    char str[32];
    for(int i=0; i<NUM_TEXTURES; i++) {
        sprintf(str,"image%d.png",i);
        buildImage(str, i);
    }

    glfwSetWindowSize(theApp->getGLFWWnd(),x,y);
    theApp->SetWidth(x); theApp->SetHeight(y);
    glViewport(0, 0, x, y);
    getDeNoise().initShader();
}



//
/////////////////////////////////////////////////
void glWindow::onExit()
{
    delete deNoise;
    
}

//
/////////////////////////////////////////////////
void glWindow::onRender()
{
    getDeNoise().render();
}



//
/////////////////////////////////////////////////
void glWindow::onIdle()
{
}


//
/////////////////////////////////////////////////
void glWindow::onReshape(GLint w, GLint h)
{
    getDeNoise().rescale(w,h);


}

//
/////////////////////////////////////////////////
void glWindow::onKeyUp(unsigned char key, int x, int y)
{

}


//
/////////////////////////////////////////////////
void glWindow::onSpecialKeyDown(int key, int x, int y)
{


}


//
/////////////////////////////////////////////////
void glWindow::onKeyDown(unsigned char key, int x, int y)
{



}



//
/////////////////////////////////////////////////
void glWindow::onSpecialKeyUp(int key, int x, int y)
{



}


//
/////////////////////////////////////////////////
void glWindow::onMouseButton(int button, int upOrDown, int x, int y)
{


}

//
/////////////////////////////////////////////////
void glWindow::onMouseWheel(int wheel, int direction, int x, int y)
{

}

//
/////////////////////////////////////////////////
void glWindow::onMotion(int x, int y)
{
    
}

//
/////////////////////////////////////////////////
void glWindow::onPassiveMotion(int x, int y)
{

}
