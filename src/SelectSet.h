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
 *  @file   /home/josh/Codes/cpp/openbookfs/src/SelectSet.h
 *
 *  @date   Feb 13, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */

#ifndef OPENBOOK_SELECTSET_H_
#define OPENBOOK_SELECTSET_H_

#include <vector>
#include <cstdlib>
#include <bitset>
#include <unistd.h>
#include "FdSet.h"

namespace   openbook {
namespace filesystem {


/// stores a list of file descriptors to select() and manages reuse of the
/// fd_set
class SelectSet
{
    public:
        /// just some constants we can use for the three sets
        enum Which
        {
            READ    =0,
            WRITE   =1,
            EXCEPT  =2,
            NUM_WHICH
        };

        class Ref
        {
            SelectSet*  m_set;
            int         m_ifd;

            friend class SelectSet;
            Ref( SelectSet* set, int ifd );

            public:
                /// clear events
                Ref& clear();

                /// assignment of an event to select
                Ref& operator<< ( Which );

                /// assignment of an event to select
                Ref& operator,  ( Which );

                /// assignment of a file descriptor
                Ref& operator=( int fd );

                /// retrieval of an event assignment
                bool operator[]( Which );

                /// conversion to a file descriptor
                operator int();
        };


    private:
        friend class Ref;

        std::vector<int>    m_fd;
        std::vector<bool>   m_listen;
        FdSet               m_set[NUM_WHICH];
        timeval             m_timeout;
        int                 m_maxfd;


    public:
        SelectSet( );

        /// access / assign a file descriptor and it's events
        Ref operator[]( unsigned int i_fd  );

        /// returns true if the file descriptor with index i_fd is ready,
        /// i.e. the operation specified in @p which will not block
        bool operator()( unsigned int i_fd, Which which=READ );

        /// set the timeout of the select
        void setTimeout( unsigned int sec, unsigned long int usec=0 );

        /// determines the max fd
        void init();

        /// calls select() and blocks until a file descriptor is
        /// ready
        int wait();

};



} // namespace filesystem
} // namespace openbook






#endif // SELECTSET_H_
