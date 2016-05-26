#ifndef NET_TCPCONNECTION_H_
#define NET_TCPCONNECTION_H_

namespace net {

class tcpConnection : public connection {

public:
    tcpConnection(Socket *socket, packetStreamer *streamer, 
                  serverAdapter *_serverAdapter,  bool appTcp);
    ~tcpConnection();

    bool writeData();

    bool readData();

    void setWriteFinishClose(bool v) {
        _writeFinishClose = v;
    }

    void clearOutputBuffer() {
        _output.clear();
    }

    void clearInputBuffer() {
        _input.clear();
    }

    void setDisconnState();

private:
	dataBuffer _output;      	    // 输出的buffer
	dataBuffer _input;       	    // 读入的buffer

	packetHeader _packetHeader; 	// 指令header

	bool _gotHeader;            	// packet header已经取过
	bool _writeFinishClose;     	// 写完断开
};

}//namespace net

#endif//NET_TCPCONNECTION_H_
 
