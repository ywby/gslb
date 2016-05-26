#ifndef MAASERVER_CONFIG_H_
#define MAASERVER_CONFIG_H_

#include <string>
#include <map>

namespace server {

enum {
	CONFIG_SERVER_INT = 0,
	CONFIG_APP_INT,
};

#define CONFIG_SERVER	"[SERVER]"
#define CONFIG_APP	"[APP]"

class configure {

public:
	configure();
	~configure();

	int load(const std::string& filename);
	std::string to_string();

	char* get_string_value(const int section, const std::string& key, char *d = NULL);
	
  int get_int_value(const int section, const std::string& key, const int d = 0);
	int parse_value(char *str, char *key, char *val);
	
	STRING_MAP getConfigMap(const int section);
     	
	std::string get_config_file_name() const {
    return configFileName;
  }

  static configure& get_configure();
	
	int check_load();
	
protected:
  static const int32_t CFG_KEY_LEN = 128;
  static const int32_t CFG_BUFFER_LEN = 4096;

protected:
  void clearConfigMap();
	STRING_MAP configServerMap;
  STRING_MAP configAppMap;

protected:
  bool isLoad;
  std::string configFileName;

};

#define CONFIG configure::get_configure()

}//namespace server

#endif//MAASERVER_CONFIG_H_

