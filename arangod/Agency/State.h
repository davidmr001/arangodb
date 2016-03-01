////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Kaveh Vahedipour
////////////////////////////////////////////////////////////////////////////////

#ifndef __ARANGODB_CONSENSUS_STATE__
#define __ARANGODB_CONSENSUS_STATE__


#include "AgencyCommon.h"
#include "State.h"

#include <Basics/Thread.h>
#include <Cluster/ClusterComm.h>
#include <velocypack/vpack.h>

#include <cstdint>
#include <functional>


//using namespace arangodb::velocypack;

class Slice {};

namespace arangodb {
namespace consensus {

class Agent;

/**
 * @brief State replica
 */
class State {
  
public:
  
  /**
   * @brief Default constructor
   */
  State ();
  
  /**
   * @brief Default Destructor
   */
  virtual ~State();
  
  /**
   * @brief Configure this state machine
   */
  void configure (size_t size);

  /**
   * @brief Append log entry
   */
  void append (query_t const& query);

  /**
   * @brief Log entries (leader)
   */
  std::vector<index_t> log (query_t const& query, term_t term, id_t lid, size_t size);

  /**
   * @brief 
   */
  void log (query_t const& query, index_t, term_t term, id_t lid, size_t size);
  
  /**
   * @brief Save currentTerm, votedFor, log entries
   */
  bool save (std::string const& ep = "tcp://localhost:8529");

  /**
   * @brief Load persisted data from above or start with empty log
   */
  bool load (std::string const& ep = "tcp://localhost:8529");

  /**
   * @brief Find entry at index with term
   */
  bool findit (index_t index, term_t term) const;

  /**
   * @brief Confirm entry index for agent id
   */
  void confirm (id_t id, index_t idx);

  /**
   * @brief Collect all unacknowledged for agent id
   */
  collect_ret_t collectUnacked (id_t id);
  
private:
  
  arangodb::Mutex _logLock;          /**< @brief Mutex for modifying _log */
  std::vector<log_t> _log;          /**< @brief  State entries */
  
};

}}

#endif
