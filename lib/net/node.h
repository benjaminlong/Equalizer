
/* Copyright (c) 2005-2007, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQNET_NODE_H
#define EQNET_NODE_H

#include <eq/net/base.h>                     // base class
#include <eq/net/commandCache.h>             // member
#include <eq/net/connectionSet.h>            // member
#include <eq/net/idHash.h>                   // member
#include <eq/net/nodeID.h>                   // member
#include <eq/net/nodeType.h>                 // for TYPE_EQNET_NODE enum

#include <eq/base/base.h>
#include <eq/base/perThread.h>
#include <eq/base/requestHandler.h>
#include <eq/base/thread.h>

#include <list>

extern "C" EQS_EXPORT eqBase::RefPtr< eqNet::Connection > eqsStartLocalServer();

namespace eqNet
{
    class Command;
    class ConnectionDescription;
    class Session;

    /**
     * Manages a node.
     *
     * A node represents a separate entity in the peer-to-peer network,
     * typically a process on a cluster node or on a shared-memory system. It
     * has at least one Connection through which is reachable. A Node provides
     * the basic communication facilities through message passing.
     */
    class EQ_EXPORT Node : public Base, public eqBase::Referenced
    {
    public:
        enum State 
        {
            STATE_STOPPED,   // initial               
            STATE_LAUNCHED,  // remote node, launched
            STATE_CONNECTED, // remote node, connected  
            STATE_LISTENING  // local node, listening
        };

        /** 
         * Constructs a new Node.
         */
        Node();

        /** @name Data Access. */
        //*{
        bool operator == ( const Node* n ) const;

        /** 
         * Returns the state of this node.
         * 
         * @return the state of this node.
         */
        State getState()    const { return _state; }
        bool  isConnected() const 
            { return (_state == STATE_CONNECTED || _state == STATE_LISTENING); }

        void setAutoLaunch( const bool autoLaunch ) { _autoLaunch = autoLaunch;}

        /** 
         * Set the program name to start this node.
         * 
         * @param name the program name to start this node.
         */
        void setProgramName( const std::string& name ) { _programName = name; }

        /** 
         * Set the working directory to start this node.
         * 
         * @param name the working directory to start this node.
         */
        void setWorkDir( const std::string& name ) { _workDir = name; }
        //*}

        /**
         * @name State Changes
         *
         * The following methods affect the state of the node by changing it's
         * connectivity to the network.
         */
        //*{
        /** 
         * Initialize a local listening node.
         *
         * This function does not return when the command line option
         * '--eq-client' is present. This is used for remote nodes which have
         * been auto-launched by another node, e.g., remote render clients.
         *
         * @param argc the command line argument count.
         * @param argv the command line argument values.
         * @return <code>true</code> if the client was successfully initialized,
         *         <code>false</code> otherwise.
         */
        virtual bool initLocal( const int argc, char** argv );

        /** Exit a local, listening node. */
        virtual bool exitLocal() { return stopListening(); }

        /** Exit a client node. */
        virtual bool exitClient() { return exitLocal(); }

        /** 
         * Initializes this node.
         *
         * The node will spawn a thread locally and listen on all connections
         * described for incoming commands. The node will be in the listening
         * state if the method completed successfully. A listening node can
         * connect to other nodes.
         * 
         * @return <code>true</code> if the node could be initialized,
         *         <code>false</code> if not.
         * @sa connect
         */
        virtual bool listen();

        /** 
         * Stops this node.
         * 
         * If this node is listening, the node will stop listening and terminate
         * its receiver thread.
         * 
         * @return <code>true</code> if the node was stopped, <code>false</code>
         *         if it was not stopped.
         */
        virtual bool stopListening();

        /** 
         * Connects a node to this listening node.
         *
         * @param node the remote node.
         * @param connection the connection to the remote node.
         * @return <code>true</code> if the node was connected correctly,
         *         <code>false</code> otherwise.
         */
        bool connect( eqBase::RefPtr<Node> node, 
                      eqBase::RefPtr<Connection> connection );

        /** 
         * Get a node by identifier.
         * 
         * @param id the node identifier.
         * @return the node.
         */
        eqBase::RefPtr<Node> getNode( const NodeID& id ) const;

        /** 
         * Connect and potentially launch a node to this listening node, using
         * the available connection descriptions.
         *
         * On success, the node is in the connected state, otherwise its state
         * is unchanged.
         *
         * @param node the remote node.
         * @return <code>true</code> if this node was connected,
         *         <code>false</code> otherwise.
         * @sa initConnect, syncConnect
         */
        bool connect( eqBase::RefPtr<Node> node );

        /** 
         * Start connecting and potentially launching a node using the
         * available connection descriptions.
         *
         * On success, the node is in the launched or connected state, otherwise
         * its state is unchanged.
         *
         * @param node the remote node.
         * @return <code>true</code> if this node is connecting,
         *         <code>false</code> otherwise.
         * @sa syncConnect
         */
        bool initConnect( eqBase::RefPtr<Node> node );

        /** 
         * Synchronize the connection initiated by initConnect().
         *
         * On success, the node is in the connected state, otherwise its state
         * is unchanged.
         *
         * @param node the remote node.
         * @return <code>true</code> if this node is connected,
         *         <code>false</code> otherwise.
         * @sa initConnect
         */
        bool syncConnect( eqBase::RefPtr<Node> node );

        /** 
         * Create and connect a node given by an identifier.
         * 
         * @param nodeID the identifier of the node to connect.
         * @param server a node holding connection information to the
         *               destination node.
         * @return the connected node, or an invalid RefPtr if the node could
         *         not be connected.
         */
        eqBase::RefPtr<Node> connect( const NodeID& nodeID,
                                      eqBase::RefPtr<Node> server );

        /** 
         * Disconnects a connected node.
         *
         * @param node the remote node.
         * @return <code>true</code> if the node was disconnected correctly,
         *         <code>false</code> otherwise.
         */
        bool disconnect( eqBase::RefPtr<Node> node );

        /** 
         * Ensures the connectivity of this node.
         *
         * @return <code>true</code> if this node is connected,
         *         <code>false</code> otherwise.
         */
        bool checkConnection()
            {
                if( _state == STATE_CONNECTED || _state == STATE_LISTENING )
                    return true;
                return false;
            }
        //*}


        /**
         * @name Connectivity information. 
         */
        //*{
        /** 
         * Returns if the node is local.
         * 
         * @return <code>true</code> if the node is local, <code>false</false>
         *         otherwise.
         */
        bool isLocal() const { return (_state==STATE_LISTENING); }

        /** 
         * Adds a new description how this node can be reached.
         * 
         * @param cd the connection description.
         */
        void addConnectionDescription( eqBase::RefPtr<ConnectionDescription> cd );
        
        /** 
         * Removes a connection description.
         * 
         * @param index the index of the connection description.
         */
        void removeConnectionDescription( const uint32_t index );

        /** 
         * Returns the number of stored connection descriptions. 
         * 
         * @return the number of stored connection descriptions. 
         */
        uint32_t nConnectionDescriptions() const 
            { return _connectionDescriptions.size(); }

        /** 
         * Returns a connection description.
         * 
         * @param index the index of the connection description.
         * @return the connection description.
         */
        eqBase::RefPtr<ConnectionDescription> getConnectionDescription(
            const uint32_t index ) const
            { return _connectionDescriptions[index]; }

        /** 
         * Returns the connection to this node.
         * 
         * @return the connection to this node. 
         */
        eqBase::RefPtr<Connection> getConnection() const { return _connection; }
        //*}

        /**
         * @name Messaging API
         */
        //@{
        /** 
         * Sends a packet to this node.
         * 
         * @param packet the packet.
         * @return the success status of the transaction.
         */
        bool send( const Packet& packet )
            {
                if( !checkConnection() )
                    return false;
                return _connection->send( packet );
            }

        /** 
         * Sends a packet with a string to the node.
         * 
         * The data is send as a new packet containing the original packet and
         * the string, so that it is received as one packet by the node.
         *
         * It is assumed that the last 8 bytes in the packet are usable for the
         * string.  This is used for optimising the send of short strings and on
         * the receiver side to access the string. The node implementation gives
         * examples of this usage.
         *
         * @param packet the packet.
         * @param string the string.
         * @return the success status of the transaction.
         */
        bool send( Packet& packet, const std::string& string )
            {
                if( !checkConnection() )
                    return false;
                return _connection->send( packet, string );
            }

        /** 
         * Sends a packet with additional data to the node.
         * 
         * The data is send as a new packet containing the original packet and
         * the string, so that it is received as one packet by the node.
         *
         * It is assumed that the last item in the packet is of sizeof(T) and
         * usable for the data.
         *
         * @param packet the packet.
         * @param data the vector containing the data.
         * @return the success status of the transaction.
         */
        template< class T >
        bool send( Packet& packet, const std::vector<T>& data )
            {
                if( !checkConnection() )
                    return false;
                return _connection->send( packet, data );
            }

        /** 
         * Sends a packet with additional data to the node.
         * 
         * The data is send as a new packet containing the original packet and
         * the data, so that it is received as one packet by the node.
         *
         * It is assumed that the last 8 bytes in the packet are usable for the
         * data.  This is used for optimising the send of short data and on
         * the receiver side to access the data. The node implementation gives
         * examples of this usage.
         *
         * @param packet the packet.
         * @param data the data.
         * @param size the size of the data in bytes.
         * @return the success status of the transaction.
         */
        bool send( Packet& packet, const void* data, const uint64_t size )
            {
                if( !checkConnection() )
                    return false;
                return _connection->send( packet, data, size );
            }

        /**
         * Flush all pending commands on this listening node.
         *
         * This causes the receiver thread to redispatch all pending commands,
         * which are normally only redispatched when a new command is received.
         */
        void flushCommands() { _connectionSet.interrupt(); }
        //@}

        /**
         * @name Session management
         */
        //*{
        /**
         * Maps a local session object to a named session.
         *
         * @param server the node serving the session.
         * @param session the session.
         * @param name the name of the session.
         * @return <code>true</code> if the session was mapped,
         *         <code>false</code> if not.
         */
        bool mapSession( eqBase::RefPtr<Node> server, Session* session, 
                         const std::string& name );

        /**
         * Maps a local session object to an existing session.
         *
         * @param server the node serving the session.
         * @param session the session.
         * @param id the identifier of the session.
         * @return <code>true</code> if the session was mapped,
         *         <code>false</code> if not.
         */
        bool mapSession( eqBase::RefPtr<Node> server, Session* session, 
                         const uint32_t id );

        /** 
         * Unmaps a mapped session.
         * 
         * @param session the session.
         * @return <code>true</code> if the session was unmapped,
         *         <code>false</code> if there was an error.
         */
        bool unmapSession( Session* session );

        /** @return the mapped session with the given identifier, or NULL. */
        Session* getSession( const uint32_t id ) { return _sessions[id]; }

        /** 
         * Adds an already mapped session to this node.
         * 
         * @param session the session.
         * @param server the node serving the session.
         * @param sessionID the identifier of the session.
         * @param name the name of the session.
         */
        void addSession( Session* session, eqBase::RefPtr<Node> server, 
                         const uint32_t sessionID, const std::string& name );

        /** 
         * Removes an unmapped session from this node.
         * 
         * @param session the session.
         */
        void removeSession( Session* session );

        bool hasSessions() const { return !_sessions.empty(); }
        //*}

        /** 
         * Runs this node as a client to a server.
         * 
         * @param clientArgs the client arguments as specified by the server.
         * @return the success value of the run.
         */
        virtual bool runClient( const std::string& clientArgs );

        /** 
         * @return <code>true</code> if executed from the receiver thread,
         *         <code>false</code> if not.
         */
        bool inReceiverThread() const { return _receiverThread->isCurrent(); }

        const NodeID& getNodeID() const { return _id; }

    protected:
        /** Destructs this node. */
        virtual ~Node();

        /** 
         * Dispatches a packet to the appropriate object or to handleCommand.
         * 
         * @param command the command.
         * @return the result of the operation.
         * @sa handleCommand, Base::invokeCommand
         */
        CommandResult dispatchCommand( Command& command );

        /** 
         * The main loop for auto-launched clients. 
         *
         * @sa runClient()
         */
        virtual bool clientLoop() { return true; }

        /** 
         * Handles a packet which has been received by this node for a custom
         * data type.
         * 
         * @param command the command.
         * @return the result of the operation.
         * @sa dispatchCommand
         */
        virtual CommandResult handleCommand( Command& )
            { return COMMAND_ERROR; }

        /** 
         * Push a command to be handled by another entity, typically a thread.
         * 
         * @param command the command.
         * @return <code>true</code> if the command was pushed,
         *         <code>false</code> if not.
         */
        virtual bool pushCommand( Command& )
            { return false; }

        /** @return the type of the node, used during connect(). */
        virtual uint32_t getType() const { return TYPE_EQNET_NODE; }

        /** 
         * Factory method to create a new node.
         * 
         * @param the type the node type
         * @return the node.
         * @sa getType()
         */
        virtual eqBase::RefPtr<Node> createNode( const uint32_t type )
        { EQASSERTINFO( type == TYPE_EQNET_NODE, type ); return new Node(); }

        /** Serialize the node's information. */
        std::string serialize() const;
        /** Deserialize the node information, consumes data. */
        bool deserialize( std::string& data );

        /** Registers request packets waiting for a return value. */
        eqBase::RequestHandler _requestHandler;

    private:
        /** Determines if the node should be launched automatically. */
        bool _autoLaunch;

        /** Globally unique node identifier. */
        NodeID _id;

        /** The current state of this node. */
        State _state;

        /** The current mapped sessions of this node. */
        IDHash<Session*> _sessions;

        /** The connection to this node, for remote nodes. */
        eqBase::RefPtr<Connection> _connection;

        /** The connection set of all connections from/to this node. */
        ConnectionSet _connectionSet;
        friend eqBase::RefPtr< eqNet::Connection > (::eqsStartLocalServer());

        /** The connected nodes. */
        NodeIDHash< eqBase::RefPtr<Node> > _nodes;

        /** The node for each connection. */
        eqBase::PtrHash< Connection*, eqBase::RefPtr<Node> > _connectionNodes;

        /** Needed for thread-safety during nodeID-based connect() */
        eqBase::Lock _connectMutex;

        /** The cache to store the last received command for reuse */
        Command* _receivedCommand;

        /** The request id for the async launch operation. */
        uint32_t _launchID;

        /** The remaining timeout for the launch operation. */
        eqBase::Clock _launchTimeout;

        /** Commands re-scheduled for dispatch. */
        std::list<Command*> _pendingCommands;
        CommandCache        _commandCache;

        /** The list of descriptions on how this node is reachable. */
        std::vector< eqBase::RefPtr<ConnectionDescription> >
            _connectionDescriptions;

        /** The name of the program to autolaunch. */
        std::string _programName;
        /** The directory of the program to autolaunch. */
        std::string _workDir;

        bool _listenToSelf();
        void _cleanup();

        /** 
         * Launch a node using the parameters from the connection
         * description.
         * 
         * @param description the connection description.
         * @return <code>true</code> if the node was launched,
         *         <code>false</code> otherwise.
         */
        bool _launch( eqBase::RefPtr<Node> node, 
                      eqBase::RefPtr<ConnectionDescription> description );

        /** 
         * Composes the launch command by expanding the variables in the
         * description's launch command string.
         * 
         * @param description the connection description.
         * @param requestID the request identifier to be used by the launched
         *                  node when connecting to this node.
         * @return the expanded launch command.
         */
        std::string _createLaunchCommand( eqBase::RefPtr<Node> node,
                            eqBase::RefPtr<ConnectionDescription> description );
        std::string   _createRemoteCommand( eqBase::RefPtr<Node> node );

        /** 
         * Find a connected node using a connection description
         * 
         * @param connectionDescription the connection description for the node.
         * @return the node, or <code>NULL</code> if no node was found.
         */
        eqBase::RefPtr<Node> _findConnectedNode( const char* 
                                                 connectionDescription );

        /** 
         * Find a named, mapped session.
         * 
         * @param name the session name.
         * @return the session, or <code>NULL</code> if the session is not
         *         mapped on this node.
         */
        Session* _findSession( const std::string& name ) const;

        /** Generates a new, unique session identifier. */
        uint32_t _generateSessionID();

        /** The receiver thread. */
        class ReceiverThread : public eqBase::Thread
        {
        public:
            ReceiverThread( Node* node ) 
                    : _node( node )
                {}
            
            virtual void* run(){ return _node->_runReceiver(); }

        private:
            Node* _node;
        };
        ReceiverThread* _receiverThread;

        void* _runReceiver();
        void    _handleConnect();
        void    _handleDisconnect();
        bool    _handleData();
        void    _redispatchCommands();

        /** The command functions. */
        CommandResult _cmdStop( Command& command );
        CommandResult _cmdMapSession( Command& command );
        CommandResult _cmdMapSessionReply( Command& command );
        CommandResult _cmdUnmapSession( Command& command );
        CommandResult _cmdUnmapSessionReply( Command& command );
        CommandResult _cmdConnect( Command& command );
        CommandResult _cmdConnectReply( Command& command );
        CommandResult _cmdDisconnect( Command& command );
        CommandResult _cmdGetNodeData( Command& command );
        CommandResult _cmdGetNodeDataReply( Command& command );

        CHECK_THREAD_DECLARE( _thread );
    };

    inline std::ostream& operator << ( std::ostream& os, const Node* node )
    {
        if( node )
            os << "node " << node->getNodeID();
        else
            os << "NULL node";
        
        return os;
    }
};
#endif // EQNET_NODE_H
