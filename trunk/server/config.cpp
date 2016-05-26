#include "define.h"

using namespace std;

namespace server {

static configure _configure;

configure::configure() {
	isLoad = false;
}

configure::~configure() {
}

configure& configure::get_configure() {
	_configure.check_load();
	return _configure;
}

STRING_MAP configure::getConfigMap(const int section) {
	if (section == CONFIG_SERVER_INT) {
		return configServerMap;	
	} else {
		return configAppMap; 	
	}
}

int configure::parse_value(char* str, char* key, char* val) {
	char *p, *p1, *name, *value;

	if (str == NULL) {
		return -1;
	}

	p = str;
	while ((*p) == ' ' || (*p) == '\t' || (*p) == '\r' || (*p) == '\n') {
		p++;
	}

	p1 = p + strlen(p);
	while ((*(p1 - 1)) == ' ' || (*(p1 - 1)) == '\t' || (*(p1 - 1)) == '\r' || (*(p1 - 1)) == '\n') {
		p1--;
	}

	(*p1) = '\0';
	if (*p == '#' || *p == '\0') {
		return -1;
	}

	p1 = strchr(str, '=');
	if (p1 == NULL) {
		return -2;
	}

	name = p;
	value = p1 + 1;

	while ((*(p1 - 1)) == ' ') {
		p1--;
	}

	(*p1) = '\0';

	while ((*value) == ' ') {
		value++;
	}

	p = strchr(value, '#');
	if (p == NULL) {
		p = value + strlen(value);
	}

	while ((*(p - 1)) <= ' ') {
		p--;
	}

	(*p) = '\0';
	if (name[0] == '\0') {
		return -2;
	}

	strcpy(key, name);
	strcpy(val, value);

	return MAA_SUCCESS;
}

int configure::load(const std::string& filename) {
	FILE *fp;
	char key[CFG_KEY_LEN], value[CFG_BUFFER_LEN], data[CFG_BUFFER_LEN];
	int ret, line = 0;
	
	if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
		fprintf(stderr, "cant open config file '%s'\n", filename.c_str());
		isLoad = true;
		return MAA_ERROR;
	}

	clearConfigMap();
	STRING_MAP *m = &configServerMap;

	while (fgets(data, 4096, fp)) {
		line++;
		
		if (strncmp(CONFIG_SERVER, data, strlen(CONFIG_SERVER)) == 0) {
			m = &configServerMap;
			continue;
		} else if (strncmp(CONFIG_APP, data, strlen(CONFIG_APP)) == 0) {
			m = &configAppMap;
			continue;
		}
		
		ret = parse_value(data, key, value);
		if (ret == -2) {
			std::cout <<"parse error: Line =" << line << "   data =" << data <<endl;
			return MAA_ERROR;
		}

		if (ret < 0) {
			printf("parse zero\n");
			std::cout <<"parse zero " << std::endl;
			continue;
		}

		if (m->find(key) != m->end()) {
			m->erase(key);
		}

		m->insert(STRING_MAP::value_type(Func::str_to_lower(key), value));
	}

	fclose(fp);
	isLoad = true;
	configFileName = filename;

	return MAA_SUCCESS;
}

void configure::clearConfigMap() {
  	configServerMap.clear();
	configAppMap.clear();
}

int configure::check_load() {
	if (!isLoad) {
		//load();
		isLoad = true;
	}

	return MAA_SUCCESS;
}

char* configure::get_string_value(const int section, const std::string& key, char *d) {
	STRING_MAP *m;

	switch (section) {
	case CONFIG_SERVER_INT:
		m = &configServerMap;
		break;
	case CONFIG_APP_INT:
		m = &configAppMap;
		break;
	default:
		return d;
		break;
	}

	STRING_MAP_ITER it = m->find(key);
	if (it != m->end()) {
		return (char *) it->second.c_str();
	} else {
		return d;
	}
}

int configure::get_int_value(const int section, const std::string& key, const int d) {
	char* str = get_string_value(section, key);
	if (str == NULL || (*str) == '\0') {
		return d;
	}

	char *p = str;
	while ((*p)) {
		if ((*p) < '0' || (*p) > '9') {
			return d;
		}
		p++;
	}

	return atoi(str);
}


// string configure::to_string() {
// 	STRING_MAP_ITER it;
// 	string result;	
// 	return result;
// }

}//namespace server
