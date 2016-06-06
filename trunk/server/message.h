#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include "define.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#include <string>

namespace server {

#ifdef FLAG	
	class message;
	typedef message* (*message_handler_t)(int32_t);
	typedef hash_map<int32_t, message_handler_t> CREATE_MESSAGE_MAP;
	typedef CREATE_MESSAGE_MAP::iterator CREATE_MESSAGE_MAP_ITER;
#endif

static const uint32_t PACKET_HEADER_SIZE = sizeof(net::packetHeader);

class message: public net::packet {

public:
	message() 
	 :raw_data_(NULL), 
	  raw_size_(0), 
	  connection_(NULL), 
	  auto_free_(true) {
	}
	
    message(message *p1) { 
         
    }
	
	~message() {
		gDeleteA(raw_data_);	
	} 
	
	inline void setConnection(net::connection* _connection) {
		connection_ = _connection;
	}

	inline net::connection* getConnection() const {
		return connection_;
	}


	inline void setMessageType(const int32_t type) {
		_packetHeader.type = type;
	}
	
	inline void setAppID(const int32_t id) {
		_packetHeader.appID = id; 
	}
	
	inline int getAppID() {
		return _packetHeader.appID; 
	}

	inline void setUserID(const int32_t id) {
		_packetHeader.userID = id; 
	}
	
	inline void setDataLen(const int32_t len) {
		_packetHeader.dataLen = len; 
	}
	
	inline int32_t getDataLen() {
		return _packetHeader.dataLen ; 
	}
	
	inline int getUserID() {
		return _packetHeader.userID; 
	}

	inline int32_t getMessageType() const {
		return _packetHeader.type;
	}

	inline bool encode(net::dataBuffer* output) {
		if ((raw_size_ <= 0) || (raw_data_ == NULL)) {
			return false;
		}

		output->writeBytes(raw_data_, raw_size_);

		return true;
	}

	inline bool decode(net::dataBuffer* input, net::packetHeader* header) {
		if (input == NULL || header == NULL) {
			return false;	
		}
	
		int32_t size = header->dataLen;
		if (size <= 0) {
			return false;
		}

		gDeleteA(raw_data_);

		raw_size_ = size;
		raw_data_ = new char[size];

		char* data = input->getData();
		memcpy(raw_data_, data, size);
		input->drainData(size);

		//SYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d,raw_data_=%s", 
		//header->type, header->dataLen,header->appID,header->userID, raw_data_);

		return true;	
	}

	inline void replyMessage(message* _message) {
		SYS_LOG(DEBUG, "raw_data_=%s", _message->raw_data_);

		if (connection_->postPacket(_message) == false) {
		    SYS_LOG(DEBUG, "delete raw_data_=%s", _message->raw_data_);
			delete _message;
			return;
		}
	}

	inline char* getRawData() const {
		return raw_data_;
	}

	inline int32_t getRawSize() const {
		return raw_size_;
	}
	
	inline void free() {
		if (auto_free_ == true) {
			delete this;
		}
	}

	inline void setAutoFree(bool auto_free) {
		auto_free_ = auto_free;
	}

public:
	char* raw_data_;

	//char* raw_data_;
	int32_t raw_size_;

protected:
	net::connection* connection_;
	bool auto_free_;

#ifdef FLAG	
	inline int get_int32(char** data, int32_t* len, int32_t* value) {
		if ((*len) < common::INT_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		(*value) = *(reinterpret_cast<int32_t*> (*data));
		(*len) -= common::INT_SIZE;
		(*data) += common::INT_SIZE;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_uint32(char** data, int32_t* len, uint32_t* value) {
		if ((*len) < common::INT_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		(*value) = *(reinterpret_cast<uint32_t*> (*data));
		(*len) -= common::INT_SIZE;
		(*data) += common::INT_SIZE;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_int64(char** data, int32_t* len, int64_t* value) {
		if ((*len) < common::INT64_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		(*value) = *(reinterpret_cast<int64_t*> (*data));
		(*len) -= common::INT64_SIZE;
		(*data) += common::INT64_SIZE;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_string(char** data, int32_t* len, char** value) {
		if ((*len) < common::INT_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		int32_t string_size = *(reinterpret_cast<int32_t*> (*data));
		if ((*len) < common::INT_SIZE + string_size) {
			return common::TFS_ERROR;
		}

		if (string_size > 0) {
			(*value) = (*data) + common::INT_SIZE;
		} else {
			(*value) = NULL;
		}

		(*len) -= (common::INT_SIZE + string_size);
		(*data) += (common::INT_SIZE + string_size);

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	// get fixed length object, such as structure
	inline int get_object(char** data, int32_t* len, void** object, int32_t obj_len) {
		if ((*len) < obj_len || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		(*object) = (*data);
		(*len) -= obj_len;
		(*data) += obj_len;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	// get fixed length object and copy the object
	inline int get_object_copy(char** data, int32_t* len, void* object, int32_t obj_len) {
		if ((*len) < obj_len || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		memcpy(object, *data, obj_len);
		(*len) -= obj_len;
		(*data) += obj_len;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_vint32(char** data, int32_t* len, common::VUINT32& value) {
		if ((*len) < common::INT_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		int32_t i = 0;
		int32_t count = *(reinterpret_cast<int32_t*> (*data));
		int32_t tlen = common::INT_SIZE + count * common::INT_SIZE;
		if ((*len) < tlen) {
			return common::TFS_ERROR;
		}

		uint32_t* int_array = reinterpret_cast<uint32_t*> ((*data) + common::INT_SIZE);
		for (i = 0; i < count; i++) {
			value.push_back(int_array[i]);
		}

		(*len) -= tlen;
		(*data) += tlen;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_vint64(char** data, int32_t* len, common::VUINT64& value) {
		if ((*len) < common::INT_SIZE || 
			(*len) > common::TFS_MALLOC_MAX_SIZE || 
			data == NULL || 
			(*data) == NULL) {
			return common::TFS_ERROR;
		}

		int32_t i = 0;
		int32_t count = *(reinterpret_cast<int32_t*> (*data));
		int32_t tlen = common::INT_SIZE + count * common::INT64_SIZE;
		if ((*len) < tlen) {
			return common::TFS_ERROR;
		}

		uint64_t* int_array = reinterpret_cast<uint64_t*>((*data) + common::INT_SIZE);
		for (i = 0; i < count; i++) {
			value.push_back(int_array[i]);
		}

		(*len) -= tlen;
		(*data) += tlen;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int get_string_len(const char* value) const {
		if (value == NULL) {
			return common::INT_SIZE;
		} else {
			return strlen(value) + common::INT_SIZE + 1;
		}
	}

	inline int get_vint_len(const common::VUINT32& value) const {
		return value.size() * common::INT_SIZE + common::INT_SIZE;
	}

	inline int get_vint64_len(common::VUINT64& value) const {
		return value.size() * common::INT64_SIZE + common::INT_SIZE;
	}

	inline int set_int32(char** data, int32_t* len, int32_t status_) {
		if ((*len) < common::INT_SIZE) {
			return common::TFS_ERROR;
		}

		memcpy(*data, &status_, common::INT_SIZE);
		(*data) += common::INT_SIZE;
		(*len) -= common::INT_SIZE;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int set_int64(char** data, int32_t* len, int64_t value) {
		if ((*len) < common::INT64_SIZE) {
			return common::TFS_ERROR;
		}

		memcpy(*data, &value, common::INT64_SIZE);
		(*data) += common::INT64_SIZE;
		(*len) -= common::INT64_SIZE;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int set_string(char** data, int32_t* len, char* str_) {
		int32_t string_size = 0;
		if (str_ != NULL) {
			string_size = strlen(str_) + 1;
		}

		if ((*len) < (string_size + common::INT_SIZE)) {
			return common::TFS_ERROR;
		}

		memcpy(*data, &string_size, common::INT_SIZE);
		if (string_size > 0) {
			memcpy((*data) + common::INT_SIZE, str_, string_size);
		}

		(*data) += (string_size + common::INT_SIZE);
		(*len) -= (string_size + common::INT_SIZE);

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int set_object(char** data, int32_t* len, void* value, int32_t length) {
		if ((*len) < length) {
			return common::TFS_ERROR;
		}

		memcpy(*data, value, length);
		(*data) += length;
		(*len) -= length;

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int32_t set_vint32(char** data, int32_t* len, common::VUINT32& value) {
		int32_t i = 0;
		int32_t count = value.size();

		if ((*len) < common::INT_SIZE + common::INT_SIZE * count) {
			return common::TFS_ERROR;
		}

		memcpy(*data, &count, common::INT_SIZE);
		(*data) += common::INT_SIZE;

		for (i = 0; i < count; i++) {
			memcpy(*data, &(value[i]), common::INT_SIZE);
			(*data) += common::INT_SIZE;
		}

		(*len) -= (common::INT_SIZE + common::INT_SIZE * count);

		assert(*len >= 0);

		return common::TFS_SUCCESS;
	}

	inline int set_vint64(char** data, int32_t* len, common::VUINT64& value) {
		int32_t i = 0;
		int32_t count = value.size();

		if ((*len) < common::INT_SIZE + common::INT64_SIZE * count) {
			return common::TFS_ERROR;
		}

		memcpy(*data, &count, common::INT_SIZE);
		(*data) += common::INT_SIZE;
		
		for (i = 0; i < count; i++) {
			memcpy(*data, &(value[i]), common::INT64_SIZE);
			(*data) += common::INT64_SIZE;
		}

		(*len) -= (common::INT_SIZE + common::INT64_SIZE * count);
		
		assert(*len >= 0);
		
		return common::TFS_SUCCESS;
	}

	static bool parse_special_ds(common::VUINT64& ds, int32_t& version, int32_t& lease) {
		common::VUINT64::iterator it = find(ds.begin(), ds.end(), static_cast<uint64_t> (ULONG_LONG_MAX));
		common::VUINT64::iterator start = it;
		
		if (it != ds.end() && ((it + 2) < ds.end())) {
			version = static_cast<int32_t> (*(it + 1));
			lease = static_cast<int32_t> (*(it + 2));
			ds.erase(start, ds.end());

			return true;
		}

		return false;
	}
#endif
	};

}//namespace server

#endif//SERVER_MESSAGE_H_ 


