#include "define.h"
#include <string>
#include <iostream>

using namespace std;
using namespace net;
using namespace sys;
using namespace server;

int main(int argc , char *argv[])
{
	std::string file_name;
	int32_t is_daemon =0, help = 0;
	int ret = 0;
	int pid = 0;
	int32_t m;
	
	while((m = getopt(argc,argv,"f:dvh")) != EOF) 
    {
		switch(m) 
        {
			case 'f':
				file_name = optarg;	
				break;	
			case 'd':
				is_daemon = 1;
				break;
			case 'v':
				cout <<"server version: 1.0" << endl;
				return 1;	
			case 'h':
				help = 1;
				break;	
		}		
	}	
	
	if(( 0 == file_name.size()) || help ) {
		cout << "useage:" << argv[0] << " -f conf_file -d -h -v" <<endl;
		cout << "-f conf_file : configurefile" << endl;
		cout << "-d : run background" << endl;
		cout << "-v : server version" << endl;
		cout << "-h : help" << endl;
		return ERROR;
	}
	
	if (CONFIG.load(file_name) != SUCCESS) {
		std::cout<< "config load to fail" <<endl;;		
		return ERROR;
	} else {
		//std::cout<< "config load to succss" <<endl;;		
	}

	SYS_LOGGER.setLogLevel(CONFIG.get_string_value(CONFIG_SERVER_INT, (char*)"debug", (char*)"debug"));
	SYS_LOGGER.setMaxFileSize(CONFIG.get_int_value(CONFIG_SERVER_INT, (char*)"log_size", 0x40000000));
	SYS_LOGGER.setMaxFileIndex(CONFIG.get_int_value(CONFIG_SERVER_INT, (char*)"log_num", 0x0A));
	
	const char * logFilePath = CONFIG.get_string_value(CONFIG_SERVER_INT, (char*)"log_path", (char*)"./");
	
	char tmpLog[256];	
	sprintf(tmpLog,"%s.%s", logFilePath, Func::time_to_str(time(NULL),1).c_str());	
	
	SYS_LOGGER.setFileName(tmpLog, true);
	
	/*	
	if (is_daemon){
		pid = sys:process::startDaemon(pid_file.c_str(),log_file.c_str());
	}	
  	*/
	
	if (pid == 0){
		signal(SIGPIPE, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
        
        /*
		signal(SIGINT, maaserver_signal_handler);
		signal(SIGTERM, maaserver_signal_handler);
		signal(40, maaserver_signal_handler);
        */

		server* _server = new Server();
		_server->start();

		///unlink(pid_file.c_str());
		//maasys::gdelete(maa_server);
		//sleep(100);	

		return ret;
  	}

	return 0;
}

namespace server 
{
    server::server() 
    {
    }

    server::~server() 
    {
    }

    void server::stop() 
    {
	    _maaservice.stop();	
    }

    void server::start() 
    {
	    _service.start();	
	    
        _service.wait();		
    }	

}//namespace server

