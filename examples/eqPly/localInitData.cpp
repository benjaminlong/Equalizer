
/*
 * Copyright (c) 2007, Stefan Eilemann <eile@equalizergraphics.com> 
 * All rights reserved. 
 */

#include "localInitData.h"
#include "frameData.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <tclap/CmdLine.h>

using namespace std;

namespace eqPly
{
LocalInitData::LocalInitData()
        : _maxFrames( 0xffffffffu ),
          _color( true ),
          _isResident( false )
{}

const LocalInitData& LocalInitData::operator = ( const LocalInitData& from )
{
    _trackerPort = from._trackerPort;  
    _maxFrames   = from._maxFrames;    
    _color       = from._color;        
    _isResident  = from._isResident;
    setFilename( from.getFilename( ));
    setWindowSystem( from.getWindowSystem( ));
    if( from.useVBOs() ) 
        enableVBOs();
    if( from.useShaders() ) 
        enableShaders();
    return *this;
}

void LocalInitData::parseArguments( const int argc, char** argv )
{
    try
    {
        TCLAP::CmdLine command( "eqPly - Equalizer polygonal rendering example" );
        TCLAP::ValueArg<string> modelArg( "m", "model", "ply model file name", 
                                          false, "rockerArm.ply", "string", 
                                          command );
        TCLAP::ValueArg<string> portArg( "p", "port", "tracking device port",
                                         false, "/dev/ttyS0", "string",
                                         command );
        TCLAP::SwitchArg colorArg( "b", "bw", "Don't use colors from ply file", 
                                   command, false );
        TCLAP::SwitchArg residentArg( "r", "resident", 
           "Keep client resident (see resident node documentation on website)", 
                                      command, false );
        TCLAP::ValueArg<uint32_t> framesArg( "n", "numFrames", 
                                           "Maximum number of rendered frames", 
                                             false, 0xffffffffu, "unsigned",
                                             command );

        string wsHelp = "Window System API ( one of: ";
#ifdef AGL
        wsHelp += "AGL ";
#endif
#ifdef GLX
        wsHelp += "glX ";
#endif
#ifdef WGL
        wsHelp += "WGL ";
#endif
        wsHelp += ")";

        TCLAP::ValueArg<string> wsArg( "w", "windowSystem", wsHelp,
                                       false, "auto", "string", command );
        
        TCLAP::SwitchArg vboArg( "v", "vbo", "Use the new VBO rendering", 
                                 command, false );
        TCLAP::SwitchArg shaderArg( "s", "glsl", "Enable GLSL shaders", 
                                    command, false );
        
        command.parse( argc, argv );

        if( modelArg.isSet( ))
            setFilename( modelArg.getValue( ));
        if( portArg.isSet( ))
            _trackerPort = portArg.getValue();
        if( wsArg.isSet( ))
        {
            string windowSystem = wsArg.getValue();
            transform( windowSystem.begin(), windowSystem.end(),
                       windowSystem.begin(), (int(*)(int))std::tolower );

            if( windowSystem == "glx" )
                setWindowSystem( eq::WINDOW_SYSTEM_GLX );
            else if( windowSystem == "agl" )
                setWindowSystem( eq::WINDOW_SYSTEM_AGL );
            else if( windowSystem == "wgl" )
                setWindowSystem( eq::WINDOW_SYSTEM_WGL );
        }

        _color = !colorArg.isSet();

        if( framesArg.isSet( ))
            _maxFrames = framesArg.getValue();

        if( residentArg.isSet( ))
            _isResident = true;
        
        if( vboArg.isSet() )
            enableVBOs();
        if( shaderArg.isSet() )
            enableShaders();
    }
    catch( TCLAP::ArgException& exception )
    {
        EQERROR << "Command line parse error: " << exception.error() 
                << " for argument " << exception.argId() << endl;
    }
}
}

