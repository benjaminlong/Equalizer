
/* Copyright (c) 2005-2007, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_PIPE_H
#define EQ_PIPE_H

#include <eq/client/node.h>           // used in inline methods
#include <eq/client/pixelViewport.h>  // member
#include <eq/client/statEvent.h>      // member
#include <eq/client/X11Connection.h>  // member
#include <eq/client/windowSystem.h>   // member

#include <eq/base/refPtr.h>
#include <eq/base/spinLock.h>
#include <eq/base/thread.h>
#include <eq/net/base.h>
#include <eq/net/object.h>
#include <eq/net/commandQueue.h>

namespace eq
{
    class Frame;
    class Window;

    class EQ_EXPORT Pipe : public eqNet::Object
    {
    public:
        /** 
         * Constructs a new pipe.
         */
        Pipe();

        /** @name Data Access. */
        //*{
        Node* getNode() const { return _node; }
        Config* getConfig() const { return (_node ? _node->getConfig() : NULL);}
        eqBase::RefPtr<eqNet::Node> getServer() const
            { return (_node ? _node->getServer() : NULL);}

        /** @return the number of windows. */
        uint32_t nWindows() const { return _windows.size(); }
        /** 
         * Gets a window.
         * 
         * @param index the window's index. 
         * @return the window.
         */
        Window* getWindow( const uint32_t index ) const
            { return _windows[index]; }

        /** 
         * @return the pipe's pixel viewport
         */
        const PixelViewport& getPixelViewport() const { return _pvp; }

        /** 
         * Returns the display number of this pipe.
         * 
         * The display number identifies the X server for systems using the
         * X11/GLX window system. It currently has no meaning on other systems.
         *
         * @return the display number of this pipe, or 
         *         <code>EQ_UNDEFINED_UINT</code>.
         */
        uint32_t getDisplay() const { return _display; }

        /** 
         * Returns the screen number of this pipe.
         * 
         * The screen number identifies the X screen for systems using the
         * X11/GLX window system, or the number of the display for the CGL
         * window system. On Windows systems it identifies the graphics adapter.
         * Normally the screen identifies a graphics adapter.
         *
         * @return the screen number of this pipe, or 
         *         <code>EQ_UNDEFINED_UINT</code>.
         */
        uint32_t getScreen() const { return _screen; }

        /** 
         * @return The string representation of this pipe's display and screen
         *         setting, in the form used by XOpenDisplay().
         */
        std::string getXDisplayString();

        /** 
         * Return the window system used by this pipe. 
         * 
         * The return value is quaranteed to be constant for an initialised
         * pipe, that is, the window system is determined usign
         * selectWindowSystem() before the pipe init method is executed.
         * 
         * @return the window system used by this pipe.
         */
        WindowSystem getWindowSystem() const { return _windowSystem; }

        /** 
         * Set the X display connection for this pipe.
         * 
         * This function should only be called from configInit() or configExit(). Updates
         * the pixel viewport.
         *
         * @param display the X display connection for this pipe.
         */
        void setXDisplay( Display* display );

        /** @return the X display connection for this pipe. */
        Display* getXDisplay() const { return _xDisplay; }

        /** 
         * Set the X display connection for event processing
         * 
         * This function should only be called from the event thread.
         *
         * @param connection the X event display connection for this pipe.
         */
        void setXEventConnection( eqBase::RefPtr<X11Connection> connection );

        /** @return the X event display connection for this pipe. */
        eqBase::RefPtr<X11Connection> getXEventConnection() const
            { return _xEventConnection; }

        /** 
         * Set the CGL display ID for this pipe.
         * 
         * This function should only be called from configInit() or configExit().
         *
         * @param id the CGL display ID for this pipe.
         */
        void setCGLDisplayID( CGDirectDisplayID id );

        /** 
         * Returns the CGL display ID for this pipe.
         * @return the CGL display ID for this pipe.
         */
        CGDirectDisplayID getCGLDisplayID() const { return _cglDisplayID; }

        /** 
         * Set the Win32 device context for this pipe.
         * 
         * This function should only be called from configInit() or configExit(). Updates
         * the pixel viewport.
         *
         * @param dc the device context for this pipe.
         * @param deleteDC true if the dc has to be deleted using DeleteDC().
         */
        void setDC( HDC dc, bool deleteDC );

        /** @return the Win32 device context for this pipe. */
        HDC getDC() const { return _dc; }

        /** @return true if the dc has to be deleted. */
        bool needsDCDelete() const { return _dcDelete; }

        /** @return the time in ms elapsed since the frame started. */
        float getFrameTime() const { return _frameClock.getTimef(); }
        //*}

        /**
         * @name Operations
         */
        //*{
        /** Add a new statistics event to the current frame. */
        void addStatEvent( const StatEvent& event )
            { _statEvents.push_back( event ); }

        /**
         * Push a command to the pipe thread to be handled from there.
         * 
         * @param node the node sending the packet.
         * @param packet the command packet.
         */
        eqNet::CommandResult pushCommand( eqNet::Command& command )
            {_commandQueue.push( command ); return eqNet::COMMAND_HANDLED;}
        
        /** 
         * Test and sets the currently attached window.
         * 
         * The pipe maintains which window is current, in order to optimize make
         * current calls.
         *
         * @param window The new current window.
         * @return true if the new window needs to be made current, false if
         *         not.
         */
        void testMakeCurrentWindow( const Window* window );

        /** 
         * Get an assembly frame.
         * 
         * @param id the frame identifier.
         * @param version the frame's version.
         * @return the frame.
         */
        Frame* getFrame( const uint32_t id, const uint32_t version );
        //*}

        /** 
         * Wait for a frame to be finished.
         * 
         * @param frameNumber the frame number.
         * @sa releaseFrame()
         */
        void waitFrameFinished( const uint32_t frameNumber ) const
            { _finishedFrame.waitGE( frameNumber ); }
    protected:
        /**
         * Destructs the pipe.
         */
        virtual ~Pipe();

        /** @name Actions */
        //*{
        /** 
         * Start a frame by unlocking all child resources.
         * 
         * @param frameNumber the frame to start.
         */
        void startFrame( const uint32_t frameNumber ) { /* currently nop */ }

        /** 
         * Signal the completion of a frame to the parent.
         * 
         * @param frameNumber the frame to end.
         */
        void releaseFrame( const uint32_t frameNumber );
        //*}

        /**
         * @name Callbacks
         *
         * The callbacks are called by Equalizer during rendering to execute
         * various actions.
         */
        //@{
        /** 
         * Tests wether a particular windowing system is supported by this pipe
         * and all its windows.
         * 
         * @param system the window system to test.
         * @return <code>true</code> if the window system is supported,
         *         <code>false</code> if not.
         */
        virtual bool supportsWindowSystem( const WindowSystem system ) const;

        /** 
         * Return the window system to be used by this pipe.
         * 
         * This function determines which of the supported windowing systems is
         * used by this pipe instance. 
         * 
         * @return the window system currently used by this pipe.
         */
        virtual WindowSystem selectWindowSystem() const;

        /** 
         * Initialises this pipe.
         * 
         * @param initID the init identifier.
         */
        virtual bool configInit( const uint32_t initID );
        virtual bool configInitGLX();
        virtual bool configInitCGL();
        virtual bool configInitWGL();

        /** 
         * Exit this pipe.
         */
        virtual bool configExit();
        virtual void configExitGLX();
        virtual void configExitCGL();
        virtual void configExitWGL();

        /**
         * Start rendering a frame.
         *
         * Called once at the beginning of each frame, to do per-frame updates
         * of pipe-specific data, for example updating the rendering
         * engine. This method has to call startFrame().
         *
         * @param frameID the per-frame identifier.
         * @param frameNumber the frame to start.
         * @sa Config::beginFrame()
         */
        virtual void frameStart( const uint32_t frameID, 
                                 const uint32_t frameNumber ) 
            { startFrame( frameNumber ); }

        /**
         * Finish rendering a frame.
         *
         * Called once at the end of each frame, to do per-frame updates of
         * pipe-specific data, for example updating the rendering engine.  This
         * method has to call releaseFrame().
         *
         * @param frameID the per-frame identifier.
         * @param frameNumber the frame to finish.
         */
        virtual void frameFinish( const uint32_t frameID, 
                                  const uint32_t frameNumber ) 
            { releaseFrame( frameNumber ); }
        //@}

        /** @name Error information. */
        //@{
        /** 
         * Set a message why the last operation failed.
         * 
         * The message will be transmitted to the originator of the request, for
         * example to Config::init when set from within the configInit method.
         *
         * @param message the error message.
         */
        void setErrorMessage( const std::string& message ) { _error = message; }
        //@}
    private:
        /** The parent node. */
        friend class Node;
        Node*       _node;

        /** The windows of this pipe. */
        std::vector<Window*>     _windows;

        /** The currently attached window. */
        const Window* _currentGLWindow;

        /** The current window system. */
        WindowSystem _windowSystem;

        /** The size (and location) of the pipe. */
        PixelViewport _pvp;

        /** The reason for the last error. */
        std::string            _error;

        /** Window-system specific display information. */
        union
        {
            Display* _xDisplay;
            CGDirectDisplayID _cglDisplayID;
            struct
            {
                HDC  _dc;
                bool _dcDelete;
            };
            char _displayFill[16];
        };

        /** The X event display connection. */
        eqBase::RefPtr<X11Connection>     _xEventConnection;

        /** The display (GLX, CGL) or ignored (Win32). */
        uint32_t _display;

        /** The screen (GLX), adapter (Win32) or ignored (CGL). */
        uint32_t _screen;
        
        /** The number of the last finished frame. */
        eqBase::Monitor<uint32_t> _finishedFrame;

        /** The running per-frame statistic clocks. */
        std::deque<eqBase::Clock> _frameClocks;
		eqBase::SpinLock          _frameClockMutex;

        /** The clock for the currently active frame. */
        eqBase::Clock _frameClock;

        /** The statistics events gathered during the current frame. */
        std::vector<StatEvent> _statEvents;

        /** All assembly frames used by the pipe during rendering. */
        eqNet::IDHash< Frame* > _frames;

        static int XErrorHandler( Display* display, XErrorEvent* event );

        void _addWindow( Window* window );
        void _removeWindow( Window* window );
        Window* _findWindow( const uint32_t id );

        void _flushFrames();

        void _grabFrame( const uint32_t frameNumber ) const
            { _node->waitFrameStarted( frameNumber ); }

        /** The pipe thread. */
        class PipeThread : public eqBase::Thread
        {
        public:
            PipeThread( Pipe* pipe ) 
                    : _pipe( pipe )
                {}
            
            virtual void* run(){ return _pipe->_runThread(); }

        private:
            Pipe* _pipe;
        };
        PipeThread* _thread;

        void* _runThread();

        /** The receiver->pipe thread command queue. */
        eqNet::CommandQueue    _commandQueue;

        /* The command functions. */
        eqNet::CommandResult _cmdCreateWindow( eqNet::Command& command );
        eqNet::CommandResult _cmdDestroyWindow( eqNet::Command& command );
        eqNet::CommandResult _cmdConfigInit( eqNet::Command& command );
        eqNet::CommandResult _reqConfigInit( eqNet::Command& command );
        eqNet::CommandResult _cmdConfigExit( eqNet::Command& command );
        eqNet::CommandResult _reqConfigExit( eqNet::Command& command );
        eqNet::CommandResult _cmdFrameStart( eqNet::Command& command );
        eqNet::CommandResult _reqFrameStart( eqNet::Command& command );
        eqNet::CommandResult _reqFrameFinish( eqNet::Command& command );
    };
}

#endif // EQ_PIPE_H

