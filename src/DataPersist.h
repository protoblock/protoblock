#ifndef fantasybit_datapersist_h
#define fantasybit_datapersist_h

#include <fstream>
#include <iostream>
#include <cstdio>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message.h>
#include <fcntl.h>

namespace fantasybit
{

template<class T>
class Writer
{
	std::ofstream mFs;

	google::protobuf::io::OstreamOutputStream *_OstreamOutputStream;
	google::protobuf::io::CodedOutputStream *_CodedOutputStream;

public:
	typedef T indata;
	Writer(const std::string &file,std::ios::openmode flags = 0) :
		mFs(file,std::ios::out | std::ios::binary | flags)
	{
		assert(mFs.good());

		_OstreamOutputStream = new google::protobuf::io::OstreamOutputStream(&mFs);
		_CodedOutputStream = new google::protobuf::io::CodedOutputStream(_OstreamOutputStream);
	}

	inline void operator()(const google::protobuf::Message &msg)
	{
		_CodedOutputStream->WriteVarint32(msg.ByteSize());

		if ( !msg.SerializeToCodedStream(_CodedOutputStream) )
			std::cout << "SerializeToCodedStream error " << std::endl;
	}
    
	~Writer()
	{
   		delete _CodedOutputStream;
		delete _OstreamOutputStream;    
		mFs.close();
	}
};

template<class T>
class Reader
{
	std::string mFeed;
	std::ifstream mFs;

	google::protobuf::io::IstreamInputStream *_IstreamInputStream;
	google::protobuf::io::CodedInputStream *_CodedInputStream;
    
    bool gd = false;

public:
	Reader(const std::string &file, std::ios::openmode flags = 0):
		mFs(file,std::ios::in | std::ios::binary | flags)
	{
		if (mFs.good())
        {
            gd = true;
            _IstreamInputStream = new google::protobuf::io::IstreamInputStream(&mFs);
            _CodedInputStream = new google::protobuf::io::CodedInputStream(_IstreamInputStream);
        }
	}

    bool good() const {
        return gd;
    }

	bool ReadNext(T &msg)
	{
		uint32_t size;

		bool ret;
		if ( (ret = _CodedInputStream->ReadVarint32(&size)) )
		{	
			google::protobuf::io::CodedInputStream::Limit msgLimit = _CodedInputStream->PushLimit(size);
			ret = msg.ParseFromCodedStream(_CodedInputStream);
			_CodedInputStream->PopLimit(msgLimit);
		
#ifdef _TRACE
			if ( ret ) 
				std::cout << mFeed << " FASReader ReadNext: " << msg.DebugString() << std::endl;
			else
				std::cout << mFeed << " FASReader ReadNext: fasle" << std::endl;
#endif
		}
#ifdef _TRACE
		else
			std::cout << mFeed << " FASReader ReadNext: fasle" << std::endl;
#endif

		return ret;

	}
	
	~Reader()
	{
   		delete _CodedInputStream;
		delete _IstreamInputStream;
   		mFs.close();
	}
};

};

#endif