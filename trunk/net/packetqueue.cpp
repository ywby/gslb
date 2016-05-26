#include "net.h"

namespace net {

packetQueue::packetQueue() {
    _head = NULL;
    _tail = NULL;
    _size = 0;
}

packetQueue::~packetQueue() {
    clear();
}

packet *packetQueue::pop() {
    if (_head == NULL) {
        return NULL;
    }

    packet *packet = _head;
    _head = _head->_next;
    if (_head == NULL) {
        _tail = NULL;
    }

    _size --;
    return packet;
}

void packetQueue::clear() {
    if (_head == NULL) {
        return;
    }

    while (_head != NULL) {
        packet *packet = _head;
        _head = packet->_next;
        packet->free();
    }

    _head = _tail = NULL;
    _size = 0;
}

void packetQueue::push(packet *_packet) {
    if (_packet == NULL) {
        return;
    }

    _packet->_next = NULL;
    if (_tail == NULL) {
        _head = _packet;
    } else {
        _tail->_next = _packet;
    }

    _tail = _packet;
    _size++;
}

int packetQueue::size() {
    return _size;
}

bool packetQueue::empty() {
    return (_size == 0);
}

void packetQueue::moveTo(packetQueue *destQueue) {
    if (_head == NULL) { 
        return;
    }

    if (destQueue->_tail == NULL) {
        destQueue->_head = _head;
    } else {
        destQueue->_tail->_next = _head;
    }

    destQueue->_tail = _tail;
    destQueue->_size += _size;
    _head = _tail = NULL;
    _size = 0;
}

packet *packetQueue::getTimeoutList(int64_t now) {
    packet *list, *tail;
    list = tail = NULL;

    while (_head != NULL) {
        int64_t t = _head->getExpireTime();
        if (t == 0 || t >= now) {
            break;
        }

        if (tail == NULL) {
            list = _head;
        } else {
            tail->_next = _head;
        }
        tail = _head;

        _head = _head->_next;
        if (_head == NULL) {
            _tail = NULL;
        }
        _size --;
    }

    if (tail) {
        tail->_next = NULL;
    }

    return list;
}

packet *packetQueue::getPacketList() {
    return _head;
}

}//namespace net


