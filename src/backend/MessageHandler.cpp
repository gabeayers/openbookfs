/*
 *  Copyright (C) 2012 Josh Bialkowski (jbialk@mit.edu)
 *
 *  This file is part of openbook.
 *
 *  openbook is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  openbook is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with openbook.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 *  @file   /home/josh/Codes/cpp/openbookfs/src/backend/MessageHandler.cpp
 *
 *  @date   Apr 14, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */


#include <iostream>
#include "Backend.h"
#include "MessageHandler.h"
#include "Marshall.h"
#include "jobs/PingJob.h"



namespace   openbook {
namespace filesystem {

MessageHandler::MessageHandler()
{
    m_backend       = 0;
    m_pool          = 0;
    m_inboundQueue  = 0;
    m_outboundQueue = 0;
    m_shouldQuit    = false;
    m_mutex.init();
}

MessageHandler::~MessageHandler()
{
    m_mutex.destroy();
}

void MessageHandler::init(Backend* backend, Pool_t* pool)
{
    m_backend = backend;
    m_pool    = pool;
    returnToPool();
}

void MessageHandler::go( int peerId, MsgQueue_t* in, MsgQueue_t* out )
{
    m_inboundQueue  = in;
    m_outboundQueue = out;
    m_peerId        = peerId;
    main();
}

void MessageHandler::returnToPool()
{
    m_inboundQueue  = 0;
    m_outboundQueue = 0;
    m_pool->reassign(this);
}

void MessageHandler::main()
{
    std::cout << "Message Handler " << (void*)this << "Starting up\n";

    MsgPtr_t msg;
    while(!m_shouldQuit)
    {
        m_inboundQueue->extract(msg);
        MessageDispatcher<0,NUM_MSG-2>::dispatch( this, msg );
    }

    std::cout << "Message Handler " << (void*)this << "Shutting down\n";
}




void MessageHandler::handleMessage( messages::LeaderElect* msg )        { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::DiffieHellmanParams* msg ){ exceptMessage(msg); }
void MessageHandler::handleMessage( messages::KeyExchange* msg )        { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::ContentKey* msg )         { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::AuthRequest* msg )        { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::AuthChallenge* msg )      { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::AuthSolution* msg )       { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::AuthResult* msg )         { exceptMessage(msg); }
void MessageHandler::handleMessage( messages::UserInterfaceReply*  msg) { exceptMessage(msg); }

void MessageHandler::handleMessage( messages::SetDisplayName* msg)
{
    m_backend->setDisplayName(
            msg->displayname());

    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    std::stringstream strm;
    strm << "Changed display name to: " << msg->displayname();
    reply->set_msg(strm.str());
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SetDataDir* msg)
{
    m_backend->setDataDir(
            msg->datadir() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SetLocalSocket* msg)
{
    m_backend->setLocalSocket(
            msg->port());

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SetRemoteSocket* msg)
{
    m_backend->setRemoteSocket(
            msg->addressfamily(),
            msg->node(),
            msg->service() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SetClientSocket* msg)
{
    m_backend->setClientSocket(
            msg->addressfamily(),
            msg->node() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SetMaxConnections* msg)
{
    m_backend->setMaxConnections(
            msg->maxconn() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::LoadConfig* msg)
{
    m_backend->loadConfig(
            msg->filename() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::SaveConfig* msg)
{
    m_backend->saveConfig(
            msg->filename() );

    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();
    reply->set_ok(true);
    m_outboundQueue->insert( new AutoMessage(reply) );
}

void MessageHandler::handleMessage( messages::AttemptConnection* msg )
{
    // for just send an empty OK response
    messages::UserInterfaceReply* reply = new messages::UserInterfaceReply();

    try
    {
        m_backend->attemptConnection(
                msg->isremote(),
                msg->node(),
                msg->service() );
        reply->set_ok(true);
    }
    catch (std::exception& ex)
    {
        reply->set_ok(false);
        reply->set_msg(ex.what());
    }

    m_outboundQueue->insert( new AutoMessage(reply) );
}


void MessageHandler::handleMessage( messages::Quit* msg )
{
    m_shouldQuit = true;
}

void MessageHandler::handleMessage( messages::Ping* msg )
{
    std::cout << "Handling PING\n";
    jobs::Pong* pong = new jobs::Pong(m_backend,m_peerId);
    m_backend->jobs()->enqueue(pong);
}

void MessageHandler::handleMessage( messages::Pong* msg )
{
    std::cout << "Handling PONG\n";
    jobs::Pong* pong = new jobs::Pong(m_backend,m_peerId);
    m_backend->jobs()->enqueue(pong);
}


void MessageHandler::handleMessage( messages::Subscribe* msg )
{

}


void MessageHandler::handleMessage( messages::Unsubscribe* msg )
{

}


void MessageHandler::handleMessage( messages::NewVersion* msg )
{

}


void MessageHandler::handleMessage( messages::RequestFile* msg )
{

}


void MessageHandler::handleMessage( messages::FileInfo* msg )
{

}


void MessageHandler::handleMessage( messages::FileChunk* msg )
{

}


void MessageHandler::handleMessage( messages::DirInfo* msg )
{

}


void MessageHandler::handleMessage( messages::DirChunk* msg )
{

}


void MessageHandler::handleMessage( messages::Invalid* msg )
{

}





} // namespace filesystem
} // namespace openbook
