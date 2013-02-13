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
 *  @file   /home/josh/Codes/cpp/openbookfs/src/server/Server.cpp
 *
 *  @date   Feb 11, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */


#include "Server.h"
#include "ExceptionStream.h"
#include <iostream>
#include <fstream>
#include <string>

#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>

namespace   openbook {
namespace filesystem {


Server::Server()
{
    m_mutex.init();
}

Server::~Server()
{
    m_mutex.destroy();
}


void Server::initConfig(const std::string& configFile)
{
    namespace fs = boost::filesystem;

    // verify that the config file exists
    if( !fs::exists( fs::path(configFile) ) )
        ex()() << "Server configuration file not found: " << configFile;

    std::ifstream in(configFile.c_str());
    if(!in)
        ex()() << "Failed to open " << configFile << " for reading";

    YAML::Parser parser(in);
    YAML::Node   config;
    parser.GetNextDocument(config);

    // any errors will throw an exception
    const YAML::Node& auth = config["auth"];

    bool authOpt;
    auth["password"]        >> authOpt; m_auth[AUTH_PASSWORD]  = authOpt;
    auth["vouch"]           >> authOpt; m_auth[AUTH_VOUCH_FOR] = authOpt;
    config["password"]      >> m_password;
    config["dataDir"]       >> m_dataDir;
    config["rootDir"]       >> m_rootDir;
    config["pubKeyFile"]    >> m_pubKeyFile;
    config["privKeyFile"]   >> m_privKeyFile;
    config["addressFamily"] >> m_addressFamily;
    config["iface"]         >> m_iface;
    config["port"]          >> m_port;
    config["maxConn"]       >> m_maxConn;

    namespace fs = boost::filesystem;

    // check that the data directory and subdirectories exist
    if( !fs::exists( fs::path(m_dataDir) ) )
    {
        std::cout << "creating data directory: "
                  << fs::absolute( fs::path(m_dataDir) )
                  << std::endl;
        bool result = fs::create_directories( fs::path(m_dataDir ) );
        if( !result )
            ex()() << "failed to create data directory: " << m_dataDir;
    }
}





} // namespace filesystem
} // namespace openbook
