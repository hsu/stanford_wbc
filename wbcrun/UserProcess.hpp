/*
 * Copyright (c) 2008 Roland Philippsen <roland DOT philippsen AT gmx DOT net>
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WBCRUN_USER_PROCESS_HPP
#define WBCRUN_USER_PROCESS_HPP

#include <wbcrun/Process.hpp>
#include <wbcrun/service.hpp>
#include <wbcrun/directory.hpp>
#include <wbcnet/msg/TaskSpec.hpp>

namespace wbcnet {
  class NetConfig;
}

namespace wbcrun {
  
  class DirectoryCmdClient;
  
  /**
     Do not forget to call UserProcess::Cleanup() at exit.
  */
  class UserProcess
    : public Process
  {
  public:
    UserProcess();
    ~UserProcess();
    
    /** Has to be called at exit. */
    static void Cleanup();
    
    virtual bool Step() throw(std::exception);
    virtual int HandleMessagePayload(wbcnet::unique_id_t msg_id);
    
    void Init(wbcnet::NetConfig const & netconf) throw(std::exception);
    void InteractiveGoalRequest();
    void InteractiveKeyPressLoop();
    void XmlRpcLoop();
    
    wbcnet::Channel * m_channel;
    ServiceMessage m_user_request;
    ServiceMessage m_user_reply;
    wbcnet::msg::TaskSpec m_task_spec;
    
  protected:
    DirectoryCmdClient * m_directory_client;
    
    listing_t const & GetBehaviorList() const throw(std::exception);
    
  private:
    mutable listing_t m_lazy_behavior_list;
  };
  
}

#endif // WBCRUN_USER_PROCESS_HPP