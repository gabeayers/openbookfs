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
 *  along with gltk.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 *  @file   src/server/main.cpp
 *
 *  @date   Feb 8, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */


#include <algorithm>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <dirent.h>
#include <sys/time.h>

#include <boost/filesystem.hpp>
#include <tclap/CmdLine.h>
#include <fstream>
#include <crypto++/files.h>
#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/dh.h>
#include <crypto++/dh2.h>
#include <re2/re2.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#include "Bytes.h"
#include "MessageBuffer.h"
#include "messages.h"
#include "messages.pb.h"

using namespace openbook::filesystem;

int main(int argc, char** argv)
{
    namespace fs = boost::filesystem;

    std::string pubKey;
    std::string privKey;
    std::string host;

    // Wrap everything in a try block.  Do this every time,
    // because exceptions will be thrown for problems.
    try {

    time_t      rawtime;
    tm*         timeinfo;
    char        currentYear[5];

    ::time( &rawtime );
    timeinfo = ::localtime( &rawtime );
    strftime (currentYear,5,"%Y",timeinfo);

    fs::path homeDir     = getenv("HOME");
    fs::path dfltPubKey  = "./rsa-openssl-pub.der";
    fs::path dfltPrivKey = "./rsa-openssl-priv.der";

    std::string dfltServer = "localhost:3031";

    std::stringstream sstream;
    sstream << "Openbook Filesystem\n"
            << "Copyright (c) 2012-" << currentYear
            << " Josh Bialkowski <jbialk@mit.edu>";

    // Define the command line object, and insert a message
    // that describes the program. The "Command description message"
    // is printed last in the help text. The second argument is the
    // delimiter (usually space) and the last one is the version number.
    // The CmdLine object parses the argv array based on the Arg objects
    // that it contains.
    TCLAP::CmdLine cmd(sstream.str().c_str(), ' ', "0.1.0");

    // Define a value argument and add it to the command line.
    // A value arg defines a flag and a type of value that it expects,
    // such as "-n Bishop".
    TCLAP::ValueArg<std::string> pubKeyArg(
            "b",
            "pubkey",
            "path to the ssh public key",
            false,
            dfltPubKey.string(),
            "path"
            );

    TCLAP::ValueArg<std::string> privKeyArg(
            "v",
            "privkey",
            "path to the ssh private key",
            false,
            dfltPrivKey.string(),
            "path"
            );

    TCLAP::ValueArg<std::string> serverArg(
            "s",
            "server",
            "hostname and port of the server",
            false,
            dfltServer,
            "string [HOST]:[PORT]"
            );

    // Add the argument nameArg to the CmdLine object. The CmdLine object
    // uses this Arg to parse the command line.
    cmd.add( pubKeyArg );
    cmd.add( privKeyArg );
    cmd.add( serverArg );

    // Parse the argv array.
    cmd.parse( argc, argv );

    // Get the value parsed by each arg.
    pubKey  = pubKeyArg.getValue();
    privKey = privKeyArg.getValue();
    host    = serverArg.getValue();

    }

    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr   << "error: " << e.error() << " for arg "
                    << e.argId() << std::endl;
        return 1;
    }

    // attempt to open the public and private key files and read them in
    if( !fs::exists( fs::path(pubKey) ) )
    {
        std::cerr << "public key file: " << pubKey << " does not exist\n";
        return 1;
    }



    std::string                 rsaPubStr;
    CryptoPP::RSA::PublicKey    rsaPubKey;
    CryptoPP::RSA::PrivateKey   rsaPrivKey;
    CryptoPP::AutoSeededRandomPool  rng;
    try
    {
        // open a stream to the public key file
        std::ifstream in(pubKey.c_str(), std::ios::in | std::ios::binary);
        if (!in)
            ex()() << "Failed to open " << pubKey << " for reading ";

        // seek to the end of the file to get it's size
        in.seekg(0, std::ios::end);

        // resize the storage space
        rsaPubStr.resize((unsigned int)in.tellg(),'\0');

        // seek back to the beginning
        in.seekg(0, std::ios::beg);

        // read in the entire file
        in.read(&rsaPubStr[0], rsaPubStr.size());

        // seek back to the beginning again
        in.seekg(0, std::ios::beg);

        // read into public key
        CryptoPP::FileSource keyFile(in,true);
        CryptoPP::ByteQueue  queue;
        keyFile.TransferTo(queue);
        queue.MessageEnd();

        rsaPubKey.Load(queue);
    }
    catch( CryptoPP::Exception& ex )
    {
        std::cerr << "Failed to load public key from " << pubKey
                  <<  " : " << ex.what() << std::endl;
        return 1;
    }

    try
    {
        CryptoPP::FileSource keyFile(privKey.c_str(),true);
        CryptoPP::ByteQueue  queue;
        keyFile.TransferTo(queue);
        queue.MessageEnd();

        rsaPrivKey.Load(queue);
    }
    catch( CryptoPP::Exception& ex )
    {
        std::cerr << "Failed to load private key from " << privKey
                  <<  " : " << ex.what() << std::endl;
        return 1;
    }

    if( !rsaPubKey.Validate(rng,3) )
    {
        std::cerr << "Failed to validate public key" << std::endl;
        return 1;
    }
    if( !rsaPrivKey.Validate(rng,3) )
    {
        std::cerr << "Failed to validate private key" << std::endl;
        return 1;
    }

    CryptoPP::DH                dh;         //< Diffie-Hellman structure
    CryptoPP::DH2               dh2(dh);    //< Diffie-Hellman structure
    CryptoPP::SecByteBlock      spriv;      //< static private key
    CryptoPP::SecByteBlock      spub;       //< static public key
    CryptoPP::SecByteBlock      epriv;      //< ephemeral private key
    CryptoPP::SecByteBlock      epub;       //< ephemeral public key
    CryptoPP::SecByteBlock      shared;     //< shared key

    // initialize DH
    {
        std::cout << "Generating DH parameters, this may take a minute"
                  << std::endl;
        using namespace CryptoPP;
        dh.AccessGroupParameters().GenerateRandomWithKeySize(rng,1024);
        spriv = SecByteBlock( dh2.StaticPrivateKeyLength() );
        spub  = SecByteBlock( dh2.StaticPublicKeyLength() );
        epriv = SecByteBlock( dh2.EphemeralPrivateKeyLength() );
        epub  = SecByteBlock( dh2.EphemeralPublicKeyLength() );

        dh2.GenerateStaticKeyPair(rng,spriv,spub);
        dh2.GenerateEphemeralKeyPair(rng,epriv, epub);
        shared= SecByteBlock( dh2.AgreedValueLength() );

        std::cout << "Finished generating DH parameters" << std::endl;
    }

    // Create the TCP socket
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( sockfd < 0 )
    {
        std::cerr << "Failed to open tcp socket" << std::endl;
        return 1;
    }

    // parse the server string
    std::string hostName;
    int         hostPort;
    RE2         serverRegex( "([^:]+):(\\d+)" );

    if( !serverRegex.ok() )
    {
        std::cerr << "Something is wrong with the regex: ([^:]+):(\\d+)"
                  << std::endl;
        return 1;
    }

    bool matchResult =
            RE2::FullMatch(host.c_str(),serverRegex,&hostName,&hostPort);
    if( !matchResult )
    {
        std::cerr << "Failed to match [HOST]:[PORT] in string " << host
                  << std::endl;
        return 1;
    }

    hostent* hostInfo;
    in_addr  hostAddr;

    if( !inet_aton(hostName.c_str(),&hostAddr) )
    {
        hostInfo        = gethostbyname(hostName.c_str());
        hostAddr.s_addr = * ( (unsigned long*)hostInfo->h_addr_list[0] );
    }

    Bytes<in_addr_t> hbytes( &hostAddr.s_addr );
    std::cout << "Host address: "
              << hbytes[0] << "."
              << hbytes[1] << "."
              << hbytes[2] << "."
              << hbytes[3] << "\n";


    sockaddr_in         server;

    memset(&server, 0, sizeof(server));         //< Clear struct
    server.sin_family       = AF_INET;          //< Internet/IP
    server.sin_addr         = hostAddr;         //< IP address
    server.sin_port         = htons(hostPort);  //< server port

    const unsigned int  bufsize = 1024;
    char                buffer[bufsize];
    unsigned int        len;
    int                 received = 0;

    // Establish connection
    int connectResult = connect( sockfd, (sockaddr*)&server, sizeof(server) );
    if( connectResult < 0 )
    {
        std::cerr << "Failed to open connection to host "
                  << hostName << " on port " << hostPort << std::endl;
        return 1;
    }

    try
    {

    MessageBuffer msg;  //< rpc middle man
    char          type; //< type of reponse message

    // the first message is a DH key exchange
    messages::KeyExchange* keyEx =
            static_cast<messages::KeyExchange*>( msg[MSG_KEY_EXCHANGE] );
    keyEx->set_skey(spub.BytePtr(),spub.SizeInBytes());
    keyEx->set_ekey(epub.BytePtr(),epub.SizeInBytes());

    // send unencrypted key exchange
    msg.write(sockfd,MSG_KEY_EXCHANGE);
    type = msg.read(sockfd);

    if( type != MSG_KEY_EXCHANGE )
    {
        std::cerr << "Protocol error: expected KEY_EXCHANGE message, got "
                  << messageIdToString(type) << std::endl;
        return 1;
    }

    // read the servers keys
    CryptoPP::SecByteBlock epubServer(
                    (unsigned char*)&keyEx->ekey()[0], keyEx->ekey().size() );
    CryptoPP::SecByteBlock spubServer(
                    (unsigned char*)&keyEx->skey()[0], keyEx->skey().size() );

    // generate shared key
    if (!dh2.Agree(shared,spriv,epriv,spubServer,epubServer))
    {
        std::cerr << "Failed to agree on a shared key" << std::endl;
        return 1;
    }

    CryptoPP::Integer epubOut, spubOut, sharedOut;
    epubOut.Decode(epubServer.BytePtr(), epubServer.SizeInBytes() );
    spubOut.Decode(spubServer.BytePtr(), spubServer.SizeInBytes() );
    sharedOut.Decode(shared.BytePtr(),shared.SizeInBytes() );
    std::cout << "Shared secret (client): "
              << "\n     epub: " << std::hex << epubOut
              << "\n     spub: " << std::hex << spubOut
              << "\n   shared: " << std::hex << sharedOut << std::endl;

    sleep(1);

    }
    catch( std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
    }

    // close the socket
    close(sockfd);



}




