#ifndef ISHADER_H
#define ISHADER_H

#include "../model.h"
#include "../tgaimage.h"

class IShader {
    protected:
    Model& m_model;
    public:

    IShader(Model& model): m_model{model} {};
    virtual Vec4f vertex(const int iface, const int nthvert) = 0;
    virtual bool fragment(const Vec3f bar, TGAColor& color) = 0;
    virtual ~IShader() {};
};

#endif // ISHADER_H