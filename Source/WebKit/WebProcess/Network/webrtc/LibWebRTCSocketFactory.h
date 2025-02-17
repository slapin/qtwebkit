/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if USE(LIBWEBRTC)

#include "LibWebRTCResolver.h"
#include "LibWebRTCSocket.h"
#include <WebCore/LibWebRTCMacros.h>
#include <webrtc/rtc_base/nethelpers.h>
#include <webrtc/p2p/base/packetsocketfactory.h>
#include <wtf/HashMap.h>

namespace WebKit {

class LibWebRTCSocketFactory {
public:
    LibWebRTCSocketFactory() { }

    void detach(LibWebRTCSocket&);

    LibWebRTCSocket* socket(uint64_t identifier) { return m_sockets.get(identifier); }
    LibWebRTCResolver* resolver(uint64_t identifier) { return m_resolvers.get(identifier); }

    std::unique_ptr<LibWebRTCResolver> takeResolver(uint64_t identifier) { return m_resolvers.take(identifier); }

    rtc::AsyncPacketSocket* createNewConnectionSocket(LibWebRTCSocket&, uint64_t newConnectionSocketIdentifier, const rtc::SocketAddress&);

    void disableNonLocalhostConnections() { m_disableNonLocalhostConnections = true; }

    rtc::AsyncPacketSocket* createUdpSocket(const rtc::SocketAddress&, uint16_t minPort, uint16_t maxPort);
    rtc::AsyncPacketSocket* createServerTcpSocket(const rtc::SocketAddress&, uint16_t minPort, uint16_t maxPort, int options);
    rtc::AsyncPacketSocket* createClientTcpSocket(const rtc::SocketAddress& localAddress, const rtc::SocketAddress& remoteAddress, PAL::SessionID, String&& userAgent, int options);
    rtc::AsyncResolverInterface* createAsyncResolver();

private:

    // We cannot own sockets, clients of the factory are responsible to free them.
    HashMap<uint64_t, LibWebRTCSocket*> m_sockets;
    static uint64_t s_uniqueSocketIdentifier;
    
    // We can own resolvers as we control their Destroy method.
    HashMap<uint64_t, std::unique_ptr<LibWebRTCResolver>> m_resolvers;
    static uint64_t s_uniqueResolverIdentifier;
    bool m_disableNonLocalhostConnections { false };
};

} // namespace WebKit

#endif // USE(LIBWEBRTC)
