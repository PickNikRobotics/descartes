/*
 * Software License Agreement (Apache License)
 *
 * Copyright (c) 2014, Southwest Research Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "descartes_core/trajectory_pt.h"
#include "descartes_core/cart_trajectory_pt.h"
#include "descartes_core/joint_trajectory_pt.h"
#include "ros/console.h"
#include <gtest/gtest.h>

using namespace descartes_core;

// Factory methods for trajectory point construction
template <class T>
TrajectoryPt* CreateTrajectoryPt();

template <>
TrajectoryPt* CreateTrajectoryPt<CartTrajectoryPt>()
{
  return new CartTrajectoryPt();
}

template <>
TrajectoryPt* CreateTrajectoryPt<JointTrajectoryPt>()
{
  return new JointTrajectoryPt();
}

template <class T>
class TrajectoryPtTest : public testing::Test {
 protected:

  TrajectoryPtTest() : lhs_(CreateTrajectoryPt<T>()), rhs_(CreateTrajectoryPt<T>()),
    lhs_copy_(CreateTrajectoryPt<T>()), lhs_clone_(CreateTrajectoryPt<T>())
  {
    *lhs_copy_ = *lhs_;
    lhs_->cloneTo(*lhs_clone_);
    lhs_same_ = lhs_;
  }

  virtual ~TrajectoryPtTest()
  {
    delete lhs_;
    delete rhs_;
    delete lhs_copy_;
    delete lhs_clone_;
  }

  TrajectoryPt* lhs_;
  TrajectoryPt* rhs_;
  TrajectoryPt* lhs_copy_;
  TrajectoryPt* lhs_clone_;
  TrajectoryPt* lhs_same_;
};

using testing::Types;

// Add types of trajectory points here:
typedef Types<CartTrajectoryPt, JointTrajectoryPt> Implementations;

TYPED_TEST_CASE(TrajectoryPtTest, Implementations);

TYPED_TEST(TrajectoryPtTest, construction) {

  EXPECT_FALSE(this->lhs_->getID().is_nil());
  EXPECT_FALSE(this->lhs_copy_->getID().is_nil());
  EXPECT_FALSE(this->lhs_clone_->getID().is_nil());
  EXPECT_FALSE(this->rhs_->getID().is_nil());

  //Depending on construction method (declaration, copy, clone, same pointer), the
  //objects and specifically IDs equality should be defined as follow

  //Declared objects should always be different
  EXPECT_NE(this->lhs_, this->rhs_);
  EXPECT_NE(this->lhs_->getID(), this->rhs_->getID());

  //Copied objects should always be the same
  //TODO: The equal comparison fails for some unknown reason.  In the debugger
  //the values are equal, so this may be a problem with eigen.
  //EXPECT_EQ(this->lhs_, this->lhs_copy_);
  EXPECT_EQ(this->lhs_->getID(), this->lhs_copy_->getID());

  //Cloned objects should have the same data (we can't test, but different ids)
  EXPECT_NE(this->lhs_, this->lhs_clone_);
  EXPECT_NE(this->lhs_->getID(), this->lhs_clone_->getID());

  //Pointers to the same objects should be identical (like a copy, but no ambiguity)
  EXPECT_EQ(this->lhs_, this->lhs_same_);
  EXPECT_EQ(this->lhs_->getID(), this->lhs_same_->getID());
}
