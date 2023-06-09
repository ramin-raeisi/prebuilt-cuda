/*
 *  Copyright 2008-2011 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include <thrust/swap.h>

namespace thrust
{

template<typename Assignable1, typename Assignable2>
  void swap(Assignable1 &a, Assignable2 &b)
{
  Assignable1 temp = a;
  a = b;
  b = temp;
} // end swap()

} // end namespace thrust

#include <thrust/detail/swap_ranges.inl>
