#include <nlohmann/json.hpp>

#include "lib/index/types.hpp"
#include "lib/index/deserialize.hpp"
#include "lib/index/get_idx_file.hpp"

#include "lib/types.hpp"

namespace ck::index { 
  
  nlohmann::json serialize_obj(const IndexObj& entry);
  
  IndexObj create_obj(const ck::secret::Secret& secret);
  
  nlohmann::json serialize(const Index&);
}