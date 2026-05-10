#pragma once

#include "problem.h"
#include "user.h"

namespace scheduler {

  double calculate_benefit(
      const Problem& problem,
      const UserProfile& user_profile
  );

}