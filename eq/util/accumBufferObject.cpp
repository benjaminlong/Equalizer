
/* Copyright (c) 2009-2010, Stefan Eilemann <eile@equalizergraphics.com>
 *               2009, Sarah Amsellem <sarah.amsellem@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "accumBufferObject.h"

#include <eq/client/gl.h>

namespace eq
{
namespace util
{

AccumBufferObject::AccumBufferObject( const GLEWContext* glewContext )
    : FrameBufferObject( glewContext )
    , _texture( 0 )
    , _pvp( 0, 0, 0, 0 )
{
}

AccumBufferObject::~AccumBufferObject()
{
    exit();
}

bool AccumBufferObject::init( const PixelViewport& pvp,
                              const GLuint textureFormat )
{
    _pvp = pvp;
    _texture = new Texture( GL_TEXTURE_RECTANGLE_ARB, glewGetContext( ));
    _texture->init( textureFormat, pvp.w, pvp.h );

    if( FrameBufferObject::init( pvp.w, pvp.h, GL_RGBA32F, 0, 0 ))
    {
        unbind();
        return true;
    }

    exit();
    return false;
}

void AccumBufferObject::exit()
{
    if( _texture )
        _texture->flush();

    delete _texture;
    _texture = 0;

    FrameBufferObject::exit();
}

void AccumBufferObject::load( const GLfloat value )
{
    EQ_GL_ERROR( "before AccumBufferObject::load" );
    _texture->copyFromFrameBuffer( _texture->getInternalFormat(), _pvp );

    const PixelViewport pvp( 0, 0, getWidth(), getHeight( ));
    _setup( pvp );
    _drawQuadWithTexture( _texture, pvp, value );
    _reset();

    EQ_GL_ERROR( "after AccumBufferObject::load" );

#if 0
    static a_int32_t i;
    std::ostringstream os;
    os << "abo" << ++i;
    getColorTextures()[0]->writeRGB( os.str( ));

    os << "tex";
    _texture->writeRGB( os.str( ));
#endif
}

void AccumBufferObject::accum( const GLfloat value )
{
    _texture->copyFromFrameBuffer( _texture->getInternalFormat(), _pvp );

    const PixelViewport pvp( 0, 0, getWidth(), getHeight( ));
    _setup( pvp );
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );

    _drawQuadWithTexture( _texture, pvp, value );

    glBlendFunc( GL_ONE, GL_ZERO );
    glDisable( GL_BLEND );
    _reset();
}

void AccumBufferObject::display( const GLfloat value )
{
    _drawQuadWithTexture( getColorTextures()[0], _pvp, value );
}

void AccumBufferObject::_setup( const PixelViewport& pvp )
{
    bind();
    glPushAttrib( GL_SCISSOR_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT );
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, pvp.w, 0, pvp.h, -1, 1);
    glScissor(0, 0, pvp.w, pvp.h);
    glViewport(0, 0, pvp.w, pvp.h);

}

void AccumBufferObject::_reset()
{
    glPopMatrix();
    glPopAttrib();
    unbind();
}

void AccumBufferObject::_drawQuadWithTexture( Texture* texture,
                                              const PixelViewport& pvp,
                                              const GLfloat value )
{
    texture->bind();

    glDepthMask( false );
    glDisable( GL_LIGHTING );
    glEnable( GL_TEXTURE_RECTANGLE_ARB );
    texture->applyWrap();
    texture->applyZoomFilter( FILTER_NEAREST );

    glColor3f( value, value, value );

    const float startX = static_cast< float >( pvp.x );
    const float endX   = static_cast< float >( pvp.x + pvp.w );
    const float startY = static_cast< float >( pvp.y );
    const float endY   = static_cast< float >( pvp.y + pvp.h );

    glBegin( GL_QUADS );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( startX, startY, 0.0f );

        glTexCoord2f( static_cast< float >( pvp.w ), 0.0f );
        glVertex3f( endX, startY, 0.0f );

        glTexCoord2f( static_cast<float>( pvp.w ), static_cast<float>( pvp.h ));
        glVertex3f( endX, endY, 0.0f );

        glTexCoord2f( 0.0f, static_cast< float >( pvp.h ));
        glVertex3f( startX, endY, 0.0f );
    glEnd();

    // restore state
    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    glDepthMask( true );
}

}
}

