/*
 Itay Marom
 Cisco Systems, Inc.
*/

/*
Copyright (c) 2015-2015 Cisco Systems, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef __TREX_STREAM_API_H__
#define __TREX_STREAM_API_H__

#include <unordered_map>
#include <stdint.h>

class TrexRpcCmdAddStream;

/**
 * Stateless Stream
 * 
 */
class TrexStream {
    /* provide the RPC parser a way to access private fields */
    friend class TrexRpcCmdAddStream;
    friend class TrexStreamTable;

public:
    TrexStream(uint8_t port_id, uint32_t stream_id);
    virtual ~TrexStream() = 0;

    static const uint32_t MIN_PKT_SIZE_BYTES = 1;
    static const uint32_t MAX_PKT_SIZE_BYTES = 9000;

private:
    /* basic */
    uint8_t       m_port_id;
    uint32_t      m_stream_id;
    

    /* config fields */
    double        m_isg_usec;
    uint32_t      m_next_stream_id;
    uint32_t      m_loop_count;

    /* indicators */
    bool          m_enable;
    bool          m_start;
    
    /* pkt */
    uint8_t      *m_pkt;
    uint16_t      m_pkt_len;

    /* VM */

    /* RX check */
    struct {
        bool      m_enable;
        bool      m_seq_enable;
        bool      m_latency;
        uint32_t  m_stream_id;

    } m_rx_check;

};

/**
 * continuous stream
 * 
 */
class TrexStreamContinuous : public TrexStream {
public:
    TrexStreamContinuous(uint8_t port_id, uint32_t stream_id, uint32_t pps) : TrexStream(port_id, stream_id), m_pps(pps) {
    }
protected:
    uint32_t m_pps;
};

/**
 * single burst
 * 
 */
class TrexStreamSingleBurst : public TrexStream {
public:
    TrexStreamSingleBurst(uint8_t port_id, uint32_t stream_id, uint32_t packets, uint32_t pps) : TrexStream(port_id, stream_id), m_pps(pps), m_packets(packets) {
    }
protected:
    uint32_t m_pps;
    uint32_t m_packets;
    
};

/**
 * multi burst
 * 
 */
class TrexStreamMultiBurst : public TrexStream {
public:
    TrexStreamMultiBurst(uint8_t port_id,
                         uint32_t stream_id,
                         uint32_t pps,
                         double   ibg_usec,
                         uint32_t pkts_per_burst,
                         uint32_t num_bursts) : TrexStream(port_id, stream_id), m_pps(pps), m_ibg_usec(ibg_usec), m_num_bursts(num_bursts), m_pkts_per_burst(pkts_per_burst) {

    }
protected:
    uint32_t m_pps;
    double   m_ibg_usec;
    uint32_t m_num_bursts;
    uint32_t m_pkts_per_burst;
};

/**
 * holds all the streams 
 *  
 */
class TrexStreamTable {
public:

    TrexStreamTable();
    ~TrexStreamTable();

    /**
     * add a stream 
     * if a previous one exists, the old one  will be deleted 
     */
    void add_stream(TrexStream *stream);

    /**
     * remove a stream
     * 
     */
    void remove_stream(TrexStream *stream);

    /**
     * fetch a stream if exists 
     * o.w NULL 
     *  
     */
    TrexStream * get_stream_by_id(uint32_t stream_id);

private:
    /**
     * holds all the stream in a hash table by stream id
     * 
     */
    std::unordered_map<int, TrexStream *> m_stream_table;
};

#endif /* __TREX_STREAM_API_H__ */
