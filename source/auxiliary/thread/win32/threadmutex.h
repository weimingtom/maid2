/*!
  @file
  @brief	スレッド同期クラス
 */

#ifndef maid2_auxiliary_thread_win32_threadmutex_h
#define maid2_auxiliary_thread_win32_threadmutex_h

#include"../../../config/define.h"
#include"../../../config/Win32.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class ThreadMutex
  {
  public:
    ThreadMutex();

    void Lock();
    void Unlock();

  private:
    class Impl;
    boost::shared_ptr<Impl>	m_pImpl;
  };

  class ThreadMutexLocker
  {
  public:
    ThreadMutexLocker(ThreadMutex& cs) : m_rSection(cs) { m_rSection.Lock(); }
    ~ThreadMutexLocker() { m_rSection.Unlock(); }
  protected:
    ThreadMutex& m_rSection;
  };
}

#endif