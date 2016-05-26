#ifndef SERVER_GLOBLEINFORMATION_H_
#define SERVER_GLOBLEINFORMATION_H_

namespace server {
struct globalInformation {
	char 	ipPort[32];
	int 	pattern;
	STRING_MAP configServerMap;
	STRING_MAP configAppMap;
};

enum {
	PATTERN_EDGE_NODE 	  = 1,  // 边缘节点服务器
	PATTERN_REGION_NODE   = 2,  // 区域节点服务器
	PATTERN_SOURCE_NODE   = 3   // 源端节点服务器
};

enum {
	MSG_TYPE_AUTHEN 	      = 1,  // 鉴权指令   与边缘节点服务器交互
	MSG_TYPE_HEARTBEAT        = 2,  // 心跳指令   节点服务器内部交互
	MSG_TYPE_REQUESTINFO      = 3,  // 数据请求指令 节点服务器内部交互
	MSG_TYPE_RESPONSEINFO     = 4,  // 数据响应指令  节点服务器内部交互
	MSG_TYPE_SHUTDOWN_ERROR   = 5,  // 连接异常 shutdown指令  关闭与边缘和源端节点相关的连接
	MSG_TYPE_SHUTDOWN_TIMEOUT = 6   // 连接超时 shutdown指令  关闭与边缘和源端节点相关的连接
};


}//namespace server

#endif//MAASERVER_GLOBLEINFORMATION_H_


