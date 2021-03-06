#pragma once

#include "envoy/access_log/access_log.h"
#include "envoy/api/api.h"
#include "envoy/ratelimit/ratelimit.h"
#include "envoy/runtime/runtime.h"
#include "envoy/server/admin.h"
#include "envoy/server/drain_manager.h"
#include "envoy/server/hot_restart.h"
#include "envoy/server/options.h"
#include "envoy/ssl/context_manager.h"
#include "envoy/thread/thread.h"
#include "envoy/thread_local/thread_local.h"
#include "envoy/tracing/http_tracer.h"
#include "envoy/upstream/cluster_manager.h"

namespace Server {

/**
 * An instance of the running server.
 */
class Instance {
public:
  virtual ~Instance() {}

  /**
   * @return Thread::BasicLockable& the single lock used for writing out to access logs. In the
   *         future we may decide to allow multiple access log locks.
   */
  virtual Thread::BasicLockable& accessLogLock() PURE;

  /**
   * @return Admin& the global HTTP admin endpoint for the server.
   */
  virtual Admin& admin() PURE;

  /**
   * @return Api::Api& the API used by the server.
   */
  virtual Api::Api& api() PURE;

  /**
   * @return Upstream::ClusterManager& singleton for use by the entire server.
   */
  virtual Upstream::ClusterManager& clusterManager() PURE;

  /**
   * @return Ssl::ContextManager& singleton for use by the entire server.
   */
  virtual Ssl::ContextManager& sslContextManager() PURE;

  /**
   * @return Event::Dispatcher& the main thread's dispatcher. This dispatcher should be used
   *         for all singleton processing.
   */
  virtual Event::Dispatcher& dispatcher() PURE;

  /**
   * @return Network::DnsResolver& the singleton DNS resolver for the server.
   */
  virtual Network::DnsResolver& dnsResolver() PURE;

  /**
   * @return TRUE if the server is currently draining. No new connections will be received and
   *         filters should shed connections where possible.
   */
  virtual bool draining() PURE;

  /**
   * Close the server's listening sockets and begin draining the listeners.
   */
  virtual void drainListeners() PURE;

  /**
   * @return DrainManager& singleton for use by the entire server.
   */
  virtual DrainManager& drainManager() PURE;

  /**
   * @return AccessLogManager for use by the entire server.
   */
  virtual AccessLog::AccessLogManager& accessLogManager() PURE;

  /**
   * Toggle whether the server fails or passes external healthchecks.
   */
  virtual void failHealthcheck(bool fail) PURE;

  /**
   * Fetch a listen socket fd based on the listening port.
   * @param port supplies the port to look up.
   * @return the fd or -1 if there is no listening socket for the port.
   */
  virtual int getListenSocketFd(uint32_t port) PURE;

  /**
   * Fetch server stats specific to this process vs. global shared stats in a hot restart scenario.
   * @param info supplies the stats structure to fill.
   */
  virtual void getParentStats(HotRestart::GetParentStatsInfo& info) PURE;

  /**
   * @return whether external healthchecks are currently failed or not.
   */
  virtual bool healthCheckFailed() PURE;

  /**
   * @return the server's hot restarter.
   */
  virtual HotRestart& hotRestart() PURE;

  /**
   * @return the server's CLI options.
   */
  virtual Options& options() PURE;

  /**
   * @return RandomGenerator& the random generator for the server.
   */
  virtual Runtime::RandomGenerator& random() PURE;

  /**
   * @return a new ratelimit client. The implementation depends on the configuration of the server.
   */
  virtual RateLimit::ClientPtr
  rateLimitClient(const Optional<std::chrono::milliseconds>& timeout) PURE;

  /**
   * @return Runtime::Loader& the singleton runtime loader for the server.
   */
  virtual Runtime::Loader& runtime() PURE;

  /**
   * Shutdown the server gracefully.
   */
  virtual void shutdown() PURE;

  /**
   * Shutdown the server's admin processing. This includes the admin API, stat flushing, etc.
   */
  virtual void shutdownAdmin() PURE;

  /**
   * @return the time that the server started during the current hot restart epoch.
   */
  virtual time_t startTimeCurrentEpoch() PURE;

  /**
   * @return the time that the server started the first hot restart epoch.
   */
  virtual time_t startTimeFirstEpoch() PURE;

  /**
   * @return the server-wide stats store.
   */
  virtual Stats::Store& stats() PURE;

  /**
   * @return the server-wide http tracer.
   */
  virtual Tracing::HttpTracer& httpTracer() PURE;

  /**
   * @return ThreadLocal::Instance& the thread local storage engine for the server. This is used to
   *         allow runtime lockless updates to configuration, etc. across multiple threads.
   */
  virtual ThreadLocal::Instance& threadLocal() PURE;

  /**
   * @return the local IP address of the server
   */
  virtual const std::string& getLocalAddress() PURE;
};

} // Server
