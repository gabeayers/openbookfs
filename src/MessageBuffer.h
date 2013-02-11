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
 *  @file   /home/josh/Codes/cpp/openbookfs/src/MessageBuffer.h
 *
 *  @date   Feb 11, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */

#ifndef OPENBOOK_MESSAGEBUFFER_H_
#define OPENBOOK_MESSAGEBUFFER_H_


#include <exception>
#include <stdexcept>
#include <string>
#include <protobuf/message.h>
#include "ExceptionStream.h"



namespace   openbook {
namespace filesystem {


class MessageException :
    public std::runtime_error
{
    public:
        MessageException( const std::string& msg ) throw():
            std::runtime_error(msg)
        {}

        virtual ~MessageException() throw(){}
};


/// storage for a block of data which will eventually be parsed as a
/// protocol buffer
class MessageBuffer
{
    public:
        typedef ExceptionStream<MessageException> ex;

    private:
        /// size of the static buffer used to cache data
        static const unsigned int BUFSIZE = 1024;

        char         m_type;   ///< enum with the message type
        unsigned int m_size;   ///< length (in bytes) of the message
        char         m_buf[BUFSIZE];    ///< buffer where data is stored

        /// throws a MessageException if value is 0
        void checkForDisconnect( int value );

    public:
        /// return the type of the message
        const char type() const;

        /// return the length of the message
        const unsigned int size() const;

        /// return a pointer to the message buffer
        const char* buf() const;

        /// read a message from a socket, will throw a MessageException
        /// on any problems
        int read( int sockfd );

        /// write a message to a socket, will throw a MessageException on
        /// any problems
        int write( int sockfd, char type, google::protobuf::Message& msg );

};



} // namespace filesystem
} // namespace openbook



#endif // MESSAGEBUFFER_H_