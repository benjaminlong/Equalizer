
/* Copyright (c) 2005-2007, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_PACKETS_H
#define EQ_PACKETS_H

#include <eq/client/channel.h>
#include <eq/client/commands.h>
#include <eq/client/frame.h>
#include <eq/client/pixelViewport.h>
#include <eq/client/renderContext.h>
#include <eq/client/statEvent.h>
#include <eq/client/viewport.h>
#include <eq/client/window.h>

#include <eq/net/packets.h>

namespace eq
{
    class Pipe;
    class Window;

    enum DataType
    {
        DATATYPE_EQ_CLIENT = eqNet::DATATYPE_EQNET_CUSTOM, // 128
        DATATYPE_EQ_SERVER,
        DATATYPE_EQ_CUSTOM = 1<<8 // 256
    };

    //------------------------------------------------------------
    // Client
    //------------------------------------------------------------
    struct ClientPacket : public eqNet::Packet
    {
        ClientPacket(){ datatype = DATATYPE_EQ_CLIENT; }
    };

    struct ClientExitPacket : public ClientPacket
    {
        ClientExitPacket()
            {
                command = CMD_CLIENT_EXIT;
                size    = sizeof( ClientExitPacket );
            }
    };

    //------------------------------------------------------------
    // Server
    //------------------------------------------------------------
    struct ServerPacket : public eqNet::Packet
    {
        ServerPacket(){ datatype = DATATYPE_EQ_SERVER; }
    };

    struct ServerChooseConfigPacket : public ServerPacket
    {
        ServerChooseConfigPacket()
            {
                command = CMD_SERVER_CHOOSE_CONFIG;
                size    = sizeof( ServerChooseConfigPacket );
                rendererInfo[0] = '\0';
            }

        uint32_t requestID;
        EQ_ALIGN8( char     rendererInfo[8] );
    };

    struct ServerCreateConfigPacket : public ServerPacket
    {
        ServerCreateConfigPacket()
            {
                command = CMD_SERVER_CREATE_CONFIG;
                size    = sizeof( ServerCreateConfigPacket );
                name[0] = '\0';
            }

        uint32_t      configID;
        eqNet::NodeID appNodeID;
        EQ_ALIGN8( char          name[8] );
    };

    struct ServerDestroyConfigPacket : public ServerPacket
    {
        ServerDestroyConfigPacket()
            {
                command = CMD_SERVER_DESTROY_CONFIG;
                size    = sizeof( ServerDestroyConfigPacket );
            }

        uint32_t      configID;
    };

    struct ServerChooseConfigReplyPacket : public ServerPacket
    {
        ServerChooseConfigReplyPacket( const ServerChooseConfigPacket*
                                       requestPacket )
            {
                command   = CMD_SERVER_CHOOSE_CONFIG_REPLY;
                size      = sizeof( ServerChooseConfigReplyPacket );
                requestID = requestPacket->requestID;
                sessionName[0] = '\0';
            }

        uint32_t requestID;
        uint32_t configID;
        EQ_ALIGN8( char     sessionName[8] );
    };

    struct ServerReleaseConfigPacket : public ServerPacket
    {
        ServerReleaseConfigPacket()
            {
                command = CMD_SERVER_RELEASE_CONFIG;
                size    = sizeof( ServerReleaseConfigPacket );
            }

        uint32_t configID;
        uint32_t requestID;
    };

    struct ServerReleaseConfigReplyPacket : public ServerPacket
    {
        ServerReleaseConfigReplyPacket( const ServerReleaseConfigPacket*
                                        requestPacket )
            {
                command   = CMD_SERVER_RELEASE_CONFIG_REPLY;
                size      = sizeof( ServerReleaseConfigReplyPacket );
                requestID = requestPacket->requestID;
            }

        uint32_t requestID;
    };

    //------------------------------------------------------------
    // Config
    //------------------------------------------------------------
    typedef eqNet::SessionPacket ConfigPacket;
    struct ConfigCreateNodePacket : public ConfigPacket
    {
        ConfigCreateNodePacket()
            {
                command = CMD_CONFIG_CREATE_NODE;
                size    = sizeof( ConfigCreateNodePacket );
            }

        uint32_t nodeID;
    };

    struct ConfigDestroyNodePacket : public ConfigPacket
    {
        ConfigDestroyNodePacket()
            {
                command = CMD_CONFIG_DESTROY_NODE;
                size    = sizeof( ConfigDestroyNodePacket );
            }

        uint32_t nodeID;
    };

    struct ConfigInitPacket : public ConfigPacket
    {
        ConfigInitPacket()
            {
                command   = CMD_CONFIG_INIT;
                size      = sizeof( ConfigInitPacket );
            }

        uint32_t requestID;
        uint32_t initID;
        uint32_t headMatrixID;
    };

    struct ConfigInitReplyPacket : public ConfigPacket
    {
        ConfigInitReplyPacket( const ConfigInitPacket* requestPacket )
            {
                command   = CMD_CONFIG_INIT_REPLY;
                size      = sizeof( ConfigInitReplyPacket );
                requestID = requestPacket->requestID;
                error[0]  = '\0';
            }

        uint32_t requestID;
        bool     result;
        EQ_ALIGN8( char error[8] );
    };

    struct ConfigExitPacket : public ConfigPacket
    {
        ConfigExitPacket()
            {
                command   = CMD_CONFIG_EXIT;
                size      = sizeof( ConfigExitPacket );
            }
        uint32_t requestID;
    };

    struct ConfigExitReplyPacket : public ConfigPacket
    {
        ConfigExitReplyPacket( const ConfigExitPacket* requestPacket )
            {
                command   = CMD_CONFIG_EXIT_REPLY;
                size      = sizeof( ConfigExitReplyPacket );
                requestID = requestPacket->requestID;
            }
        uint32_t requestID;
        bool     result;
    };

    struct ConfigStartFramePacket : public ConfigPacket
    {
        ConfigStartFramePacket()
            {
                command   = CMD_CONFIG_START_FRAME;
                size      = sizeof( ConfigStartFramePacket );
            }
        uint32_t requestID;
        uint32_t frameID;
    };

    struct ConfigStartFrameReplyPacket : public ConfigPacket
    {
        ConfigStartFrameReplyPacket(const ConfigStartFramePacket* requestPacket)
            {
                command   = CMD_CONFIG_START_FRAME_REPLY;
                size      = sizeof( ConfigStartFrameReplyPacket );
                sessionID = requestPacket->sessionID;
                requestID = requestPacket->requestID;
            }
        uint32_t requestID;
        uint32_t frameNumber;
        uint32_t nNodeIDs;
        EQ_ALIGN8( eqNet::NodeID nodeIDs[1] );
    };

    struct ConfigFinishFramePacket : public ConfigPacket
    {
        ConfigFinishFramePacket()
            {
                command   = CMD_CONFIG_FINISH_FRAME;
                size      = sizeof( ConfigFinishFramePacket );
            }
        uint32_t requestID;
    };

    struct ConfigFinishFrameReplyPacket : public ConfigPacket
    {
        ConfigFinishFrameReplyPacket( const ConfigFinishFramePacket* 
                                      requestPacket )
            {
                command   = CMD_CONFIG_FINISH_FRAME_REPLY;
                size      = sizeof( ConfigFinishFrameReplyPacket );
                requestID = requestPacket->requestID;
            }
        uint32_t requestID;
        uint32_t result;
    };

    struct ConfigFinishAllFramesPacket : public ConfigPacket
    {
        ConfigFinishAllFramesPacket()
            {
                command   = CMD_CONFIG_FINISH_ALL_FRAMES;
                size      = sizeof( ConfigFinishAllFramesPacket );
            }
        uint32_t requestID;
    };

    struct ConfigFinishAllFramesReplyPacket : public ConfigPacket
    {
        ConfigFinishAllFramesReplyPacket( const ConfigFinishAllFramesPacket* 
                                       requestPacket )
            {
                command   = CMD_CONFIG_FINISH_ALL_FRAMES_REPLY;
                size      = sizeof( ConfigFinishAllFramesReplyPacket );
                requestID = requestPacket->requestID;
            }
        uint32_t requestID;
        uint32_t result;
    };

    //------------------------------------------------------------
    // Node
    //------------------------------------------------------------
    struct NodeConfigInitPacket : public eqNet::ObjectPacket
    {
        NodeConfigInitPacket()
            {
                command        = CMD_NODE_CONFIG_INIT;
                size           = sizeof( NodeConfigInitPacket );
            }

        uint32_t requestID;
        uint32_t initID;
    };

    struct NodeConfigInitReplyPacket : public eqNet::ObjectPacket
    {
        NodeConfigInitReplyPacket( const NodeConfigInitPacket* requestPacket )
            {
                command   = CMD_NODE_CONFIG_INIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( NodeConfigInitReplyPacket );
                error[0]  = '\0';
            }

        uint32_t requestID;
        bool     result;
        EQ_ALIGN8( char error[8] );
    };

    struct NodeConfigExitPacket : public eqNet::ObjectPacket
    {
        NodeConfigExitPacket()
            {
                command = CMD_NODE_CONFIG_EXIT;
                size    = sizeof( NodeConfigExitPacket );
            }

        uint32_t requestID;
    };

    struct NodeConfigExitReplyPacket : public eqNet::ObjectPacket
    {
        NodeConfigExitReplyPacket( const NodeConfigExitPacket* requestPacket )
            {
                command   = CMD_NODE_CONFIG_EXIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( NodeConfigExitReplyPacket );
            }

        uint32_t requestID;
        bool     result;
    };

    struct NodeCreatePipePacket : public eqNet::ObjectPacket
    {
        NodeCreatePipePacket()
            {
                command = CMD_NODE_CREATE_PIPE;
                size    = sizeof( NodeCreatePipePacket );
            }

        uint32_t pipeID;
    };

    struct NodeDestroyPipePacket : public eqNet::ObjectPacket
    {
        NodeDestroyPipePacket()
            {
                command = CMD_NODE_DESTROY_PIPE;
                size    = sizeof( NodeDestroyPipePacket );
            }

        uint32_t pipeID;
    };
    
    struct NodeFrameStartPacket : public eqNet::ObjectPacket
    {
        NodeFrameStartPacket()
            {
                command        = CMD_NODE_FRAME_START;
                size           = sizeof( NodeFrameStartPacket );
            }

        uint32_t frameID;
        uint32_t frameNumber;
    };

    struct NodeFrameFinishPacket : public eqNet::ObjectPacket
    {
        NodeFrameFinishPacket()
            {
                command        = CMD_NODE_FRAME_FINISH;
                size           = sizeof( NodeFrameFinishPacket );
            }

        uint32_t frameID;
        uint32_t frameNumber;
    };

    struct NodeFrameFinishReplyPacket : public eqNet::ObjectPacket
    {
        NodeFrameFinishReplyPacket()
            {
                command        = CMD_NODE_FRAME_FINISH_REPLY;
                size           = sizeof( NodeFrameFinishReplyPacket );
            }

        uint32_t frameNumber;
        uint32_t nStatEvents;
        EQ_ALIGN8( StatEvent statEvents[1] );
    };

    //------------------------------------------------------------
    // Pipe
    //------------------------------------------------------------
    struct PipeCreateWindowPacket : public eqNet::ObjectPacket
    {
        PipeCreateWindowPacket()
            {
                command = CMD_PIPE_CREATE_WINDOW;
                size    = sizeof( PipeCreateWindowPacket );
            }

        uint32_t windowID;
    };

    struct PipeDestroyWindowPacket : public eqNet::ObjectPacket
    {
        PipeDestroyWindowPacket()
            {
                command = CMD_PIPE_DESTROY_WINDOW;
                size    = sizeof( PipeDestroyWindowPacket );
            }

        uint32_t windowID;
    };

    struct PipeConfigInitPacket : public eqNet::ObjectPacket
    {
        PipeConfigInitPacket()
            {
                command = CMD_PIPE_CONFIG_INIT;
                size    = sizeof( PipeConfigInitPacket );
            }

        uint32_t      requestID;
        uint32_t      initID;
        uint32_t      display;
        uint32_t      screen;
        PixelViewport pvp;
    };

    struct PipeConfigInitReplyPacket : public eqNet::ObjectPacket
    {
        PipeConfigInitReplyPacket( const PipeConfigInitPacket* requestPacket )
            {
                command   = CMD_PIPE_CONFIG_INIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( PipeConfigInitReplyPacket );
                error[0]  = '\0';
            }

        uint32_t      requestID;
        bool          result;
        PixelViewport pvp;
        EQ_ALIGN8( char error[8] );
    };

    struct PipeConfigExitPacket : public eqNet::ObjectPacket
    {
        PipeConfigExitPacket()
            {
                command = CMD_PIPE_CONFIG_EXIT;
                size    = sizeof( PipeConfigExitPacket );
            }

        uint32_t requestID;
    };

    struct PipeConfigExitReplyPacket : public eqNet::ObjectPacket
    {
        PipeConfigExitReplyPacket( const PipeConfigExitPacket* requestPacket )
            {
                command   = CMD_PIPE_CONFIG_EXIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( PipeConfigExitReplyPacket );
            }

        uint32_t requestID;
        bool     result;
    };

    struct PipeFrameStartPacket : public eqNet::ObjectPacket
    {
        PipeFrameStartPacket()
            {
                command        = CMD_PIPE_FRAME_START;
                size           = sizeof( PipeFrameStartPacket );
            }

        uint32_t frameID;
        uint32_t frameNumber;
    };

    struct PipeFrameFinishPacket : public eqNet::ObjectPacket
    {
        PipeFrameFinishPacket()
            {
                command        = CMD_PIPE_FRAME_FINISH;
                size           = sizeof( PipeFrameFinishPacket );
            }

        uint32_t frameID;
        uint32_t frameNumber;
    };

    //------------------------------------------------------------
    // Window
    //------------------------------------------------------------
    struct WindowConfigInitPacket : public eqNet::ObjectPacket
    {
        WindowConfigInitPacket()
            {
                command = CMD_WINDOW_CONFIG_INIT;
                size    = sizeof( WindowConfigInitPacket );
                name[0]   = '\0';
            }

        uint32_t       requestID;
        uint32_t       initID;
        int32_t        iattr[eq::Window::IATTR_ALL];
        PixelViewport  pvp;
        Viewport       vp;
        EQ_ALIGN8( char name[8] );
    };

    struct WindowConfigInitReplyPacket : public eqNet::ObjectPacket
    {
        WindowConfigInitReplyPacket( const WindowConfigInitPacket* request )
            {
                command   = CMD_WINDOW_CONFIG_INIT_REPLY;
                size      = sizeof( WindowConfigInitReplyPacket );
                requestID = request->requestID;
                error[0]  = '\0';
            }

        uint32_t                requestID;
        bool                    result;
        PixelViewport           pvp;
        Window::DrawableConfig  drawableConfig;
        EQ_ALIGN8( char error[8] );
    };

    struct WindowConfigExitPacket : public eqNet::ObjectPacket
    {
        WindowConfigExitPacket()
            {
                command = CMD_WINDOW_CONFIG_EXIT;
                size    = sizeof( WindowConfigExitPacket );
            }

        uint32_t requestID;
    };

    struct WindowConfigExitReplyPacket : public eqNet::ObjectPacket
    {
        WindowConfigExitReplyPacket( const WindowConfigExitPacket* request )
            {
                command   = CMD_WINDOW_CONFIG_EXIT_REPLY;
                size      = sizeof( WindowConfigExitReplyPacket );
                requestID = request->requestID;
            }

        uint32_t requestID;
        bool     result;
    };

    struct WindowCreateChannelPacket : public eqNet::ObjectPacket
    {
        WindowCreateChannelPacket()
            {
                command = CMD_WINDOW_CREATE_CHANNEL;
                size    = sizeof( WindowCreateChannelPacket );
            }

        uint32_t channelID;
    };

    struct WindowDestroyChannelPacket : public eqNet::ObjectPacket
    {
        WindowDestroyChannelPacket()
            {
                command = CMD_WINDOW_DESTROY_CHANNEL;
                size    = sizeof( WindowDestroyChannelPacket );
            }

        uint32_t channelID;
    };

    struct WindowSetPVPPacket : public eqNet::ObjectPacket
    {
        WindowSetPVPPacket()
            {
                command = CMD_WINDOW_SET_PVP;
                size    = sizeof( WindowSetPVPPacket );
            }

        PixelViewport pvp;
    };

    struct WindowFinishPacket : public eqNet::ObjectPacket
    {
        WindowFinishPacket()
            {
                command = CMD_WINDOW_FINISH;
                size    = sizeof( WindowFinishPacket );
            }
    };

    struct WindowBarrierPacket : public eqNet::ObjectPacket
    {
        WindowBarrierPacket()
            {
                command = CMD_WINDOW_BARRIER;
                size    = sizeof( WindowBarrierPacket );
            }
        uint32_t barrierID;
        uint32_t barrierVersion;
    };

    struct WindowSwapPacket : public eqNet::ObjectPacket
    {
        WindowSwapPacket()
            {
                command = CMD_WINDOW_SWAP;
                size    = sizeof( WindowSwapPacket );
            }
    };

    struct WindowStartFramePacket : public eqNet::ObjectPacket
    {
        WindowStartFramePacket()
            {
                command = CMD_WINDOW_STARTFRAME;
                size    = sizeof( WindowStartFramePacket );
            }
        uint32_t frameID;
    };
    struct WindowEndFramePacket : public eqNet::ObjectPacket
    {
        WindowEndFramePacket()
            {
                command = CMD_WINDOW_ENDFRAME;
                size    = sizeof( WindowEndFramePacket );
            }
        uint32_t frameID;
    };

    //------------------------------------------------------------
    // Channel
    //------------------------------------------------------------
    struct ChannelConfigInitPacket : public eqNet::ObjectPacket
    {
        ChannelConfigInitPacket()
            {
                command = CMD_CHANNEL_CONFIG_INIT;
                size    = sizeof( ChannelConfigInitPacket );
                name[0] = '\0';
            }

        uint32_t      requestID;
        uint32_t      initID;
        int32_t       iattr[eq::Channel::IATTR_ALL];
        PixelViewport pvp;
        Viewport      vp;
        EQ_ALIGN8( char          name[8] );
    };

    struct ChannelConfigInitReplyPacket : public eqNet::ObjectPacket
    {
        ChannelConfigInitReplyPacket( const ChannelConfigInitPacket* 
                                      requestPacket )
            {
                command   = CMD_CHANNEL_CONFIG_INIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( ChannelConfigInitReplyPacket );
                error[0]  = '\0';
            }

        uint32_t requestID;
        float    nearPlane;
        float    farPlane;
        bool     result;
        EQ_ALIGN8( char error[8] );
    };

    struct ChannelConfigExitPacket : public eqNet::ObjectPacket
    {
        ChannelConfigExitPacket()
            {
                command = CMD_CHANNEL_CONFIG_EXIT;
                size    = sizeof( ChannelConfigExitPacket );
            }

        uint32_t requestID;
    };

    struct ChannelConfigExitReplyPacket : public eqNet::ObjectPacket
    {
        ChannelConfigExitReplyPacket( const ChannelConfigExitPacket* 
                                      requestPacket )
            {
                command   = CMD_CHANNEL_CONFIG_EXIT_REPLY;
                requestID = requestPacket->requestID;
                size      = sizeof( ChannelConfigExitReplyPacket );
            }

        uint32_t requestID;
        bool     result;
    };

    struct ChannelSetNearFarPacket : public eqNet::ObjectPacket
    {
        ChannelSetNearFarPacket()
            {
                command   = CMD_CHANNEL_SET_NEARFAR;
                size      = sizeof( ChannelSetNearFarPacket );
            }

        float    nearPlane;
        float    farPlane;
    };

    struct ChannelTaskPacket : public eqNet::ObjectPacket
    {
        RenderContext context;
    };

    struct ChannelClearPacket : public ChannelTaskPacket
    {
        ChannelClearPacket()
            {
                command       = CMD_CHANNEL_CLEAR;
                size          = sizeof( ChannelClearPacket );
            }
    };
        
    struct ChannelDrawPacket : public ChannelTaskPacket
    {
        ChannelDrawPacket()
            {
                command       = CMD_CHANNEL_DRAW;
                size          = sizeof( ChannelDrawPacket );
            }
    };
        
    struct ChannelAssemblePacket : public ChannelTaskPacket
    {
        ChannelAssemblePacket()
            {
                command       = CMD_CHANNEL_ASSEMBLE;
                size          = sizeof( ChannelAssemblePacket );
            }

        uint32_t             nFrames;
        EQ_ALIGN8( eqNet::ObjectVersion frames[1] );
    };
        
    struct ChannelReadbackPacket : public ChannelTaskPacket
    {
        ChannelReadbackPacket()
            {
                command       = CMD_CHANNEL_READBACK;
                size          = sizeof( ChannelReadbackPacket );
            }

        uint32_t             nFrames;
        EQ_ALIGN8( eqNet::ObjectVersion frames[1] );
    };
        
    struct ChannelTransmitPacket : public eqNet::ObjectPacket
    {
        ChannelTransmitPacket()
            {
                command       = CMD_CHANNEL_TRANSMIT;
                size          = sizeof( ChannelTransmitPacket );
            }

        
        eqNet::ObjectVersion frame;
        uint32_t             nNodes;
        EQ_ALIGN8( eqNet::NodeID        nodes[1] );
    };

    //------------------------------------------------------------
    // Frame Data
    //------------------------------------------------------------
    struct FrameDataTransmitPacket : public eqNet::ObjectPacket
    {
        FrameDataTransmitPacket()
            {
                command = CMD_FRAMEDATA_TRANSMIT;
                size    = sizeof( FrameDataTransmitPacket );
            }

        uint32_t      version;
        uint32_t      buffers;
        PixelViewport pvp;
        EQ_ALIGN8( uint8_t       data[8] ); // size is pvp.w * pvp.h * depth
    };

    struct FrameDataReadyPacket : public eqNet::ObjectPacket
    {
        FrameDataReadyPacket()
            {
                command = CMD_FRAMEDATA_READY;
                size    = sizeof( FrameDataReadyPacket );
            }
        uint32_t version;
    };

    //------------------------------------------------------------
    // Event Thread
    //------------------------------------------------------------
    struct GLXEventThreadAddPipePacket : public eqNet::Packet
    {
        GLXEventThreadAddPipePacket()
            {
                command = CMD_GLXEVENTTHREAD_ADD_PIPE;
                size    = sizeof( GLXEventThreadAddPipePacket );
            }
        Pipe* pipe;
    };
    struct GLXEventThreadRemovePipePacket : public eqNet::Packet
    {
        GLXEventThreadRemovePipePacket()
            {
                command = CMD_GLXEVENTTHREAD_REMOVE_PIPE;
                size    = sizeof( GLXEventThreadRemovePipePacket );
            }
        uint32_t requestID;
        Pipe*    pipe;
    };
    struct GLXEventThreadAddWindowPacket : public eqNet::Packet
    {
        GLXEventThreadAddWindowPacket()
            {
                command = CMD_GLXEVENTTHREAD_ADD_WINDOW;
                size    = sizeof( GLXEventThreadAddWindowPacket );
            }
        Window* window;
    };
    struct GLXEventThreadRemoveWindowPacket : public eqNet::Packet
    {
        GLXEventThreadRemoveWindowPacket()
            {
                command = CMD_GLXEVENTTHREAD_REMOVE_WINDOW;
                size    = sizeof( GLXEventThreadRemoveWindowPacket );
            }
        uint32_t requestID;
        Window*  window;
    };

    //------------------------------------------------------------
    inline std::ostream& operator << ( std::ostream& os, 
                                       const ServerChooseConfigPacket* packet )
    {
        os << (ServerPacket*)packet << " req " << packet->requestID
           << " renderer " << packet->rendererInfo;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                   const ServerChooseConfigReplyPacket* packet )
    {
        os << (ServerPacket*)packet << " req " << packet->requestID << " id " 
           << packet->configID;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                       const ServerReleaseConfigPacket* packet )
    {
        os << (ServerPacket*)packet << " config " << packet->configID;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                       const NodeCreatePipePacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " id " << packet->pipeID;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os,
                                       const ConfigInitReplyPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " requestID " <<packet->requestID;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                     const ConfigStartFrameReplyPacket* packet )
    {
        os << (ConfigPacket*)packet << " frame #" << packet->frameNumber
           << ", " << packet->nNodeIDs << " nodes";

        for( uint32_t i=0 ; i<4 && i<packet->nNodeIDs; ++i )
            os << " " << i << ":" << packet->nodeIDs[i];

        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                       const PipeConfigInitPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " init id " << packet->initID
           << " display " << packet->display << " screen " << packet->screen;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const PipeConfigInitReplyPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " result " << packet->result;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                     const WindowConfigInitReplyPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " result " << packet->result
           << " pvp " << packet->pvp;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                       const WindowBarrierPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " barrier " << packet->barrierID
           << " version " << packet->barrierVersion;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const WindowStartFramePacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " frame " << packet->frameID;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const WindowEndFramePacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " frame " << packet->frameID;
        return os;
    }

    inline std::ostream& operator << ( std::ostream& os, 
                                    const ChannelConfigInitReplyPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " result " << packet->result;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const ChannelTaskPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " " << packet->context;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const ChannelReadbackPacket* packet )
    {
        os << (ChannelTaskPacket*)packet << " nFrames " << packet->nFrames;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const ChannelTransmitPacket* packet )
    {
        os << (eqNet::ObjectPacket*)packet << " frame " << packet->frame
           << " nNodes " << packet->nNodes;
        return os;
    }
    inline std::ostream& operator << ( std::ostream& os, 
                                       const ChannelAssemblePacket* packet )
    {
        os << (ChannelTaskPacket*)packet << " nFrames " << packet->nFrames;
        return os;
    }
}

#endif // EQ_PACKETS_H

